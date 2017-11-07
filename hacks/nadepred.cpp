#include "nadepred.h"

#include "esp.h"
#include "math.h"
#include "render.h"
#include "entity.h"
#include "dlights.h"
#include "settings.h"
#include "globals.h"

bool Config::Misc::GrenadeHelper::Prediction::Enable = true;
bool Config::Misc::GrenadeHelper::Prediction::DlightAtEnd = true;

float GrenadePrediction::CurrentThrowStrength = 0.0f;
std::vector<Vector> GrenadePrediction::path = {};
C_BaseCSGrenade* GrenadePrediction::throwable = nullptr;
NadeInfo_t ni = { Color(), "" };

int ticksSinceGrenade = 0;

void GrenadePrediction::CreateMove()
{
	ticksSinceGrenade++;

	if (!Config::Misc::GrenadeHelper::Prediction::Enable)
		return;

	if (!G::ActiveWeapon)
		return;

	auto id = G::ActiveWeapon ? *G::ActiveWeapon->GetItemDefinitionIndex() : ItemDefinitionIndex::INVALID;

	if (!IsGrenade(id))
	{
		throwable = nullptr;
		return;
	}

	if (G::ActiveWeapon != throwable)
	{
		throwable = reinterpret_cast<C_BaseCSGrenade*>(G::ActiveWeapon);
		ni = ESP::GetNadeInfo(throwable);
	}
	
	CurrentThrowStrength = throwable->GetThrowStrength();

	if (throwable->IsHeldByPlayer())
	{
		ticksSinceGrenade = 0;
		Simulate(G::LocalPlayer);
	}
}

void GrenadePrediction::PaintTraverse()
{
	if (!Config::Misc::GrenadeHelper::Prediction::Enable)
		return;

	if (path.size() > 1)
	{
		if (ticksSinceGrenade > 200)
		{
			path.clear();
			return;
		}

		Vector prev = path[0];
		Vector vStart, vEnd;
		for (auto it = path.begin(), end = path.end(); it != end; ++it)
		{
			if (Render::WorldToScreen(prev, vStart) && Render::WorldToScreen(*it, vEnd))
				Render::Line(vStart.x, vStart.y, vEnd.x, vEnd.y, Entity::IsPositionVisible(nullptr, prev) ? ni.color : ni.color.CustomAlpha(0.25f));

			prev = *it;
		}
		
		if (Config::Misc::GrenadeHelper::Prediction::DlightAtEnd)
			DLights::AllocDlight(ni.color, 3.f, 80.f, 0.f, 699 + rand() % 1000, Vector(), prev);
	}
}

bool GrenadePrediction::IsGrenade(ItemDefinitionIndex id)
{
	return id >= ItemDefinitionIndex::WEAPON_FLASHBANG && id <= ItemDefinitionIndex::WEAPON_INCGRENADE;
}

void GrenadePrediction::Setup(C_BasePlayer* localplayer, Vector& vecSource, Vector& vecThrow)
{
	QAngle angThrow = *localplayer->GetEyeAngles();
	float pitch = angThrow.x;

	if (pitch <= 90.0f)
	{
		if (pitch<-90.0f)
		{
			pitch += 360.0f;
		}
	}
	else
	{
		pitch -= 360.0f;
	}

	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	angThrow.x = a;

	// Gets ThrowVelocity from weapon files
	// Clamped to [15,750]
	float flVel = 750.0f * 0.9f;

	// Do magic on member of grenade object [esi+9E4h]
	// m1=1  m1+m2=0.5  m2=0
	//static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	// Clamped to [0,1]
	float b = CurrentThrowStrength;
	b = b * 0.7f;
	b = b + 0.3f;
	flVel *= b;

	Vector vForward;
	Math::AngleVectors(angThrow, vForward);

	vecSource = localplayer->GetVecOrigin();
	vecSource += localplayer->GetVecViewOffset();
	float off = (CurrentThrowStrength * 12.0f) - 12.0f;
	vecSource.z += off;

	// Game calls UTIL_TraceHull here with hull and assigns vecSrc tr.endpos
	trace_t tr;
	Vector vecDest = vecSource;
	vecDest += vForward * 22;
	TraceHull(vecSource, vecDest, tr);

	// After the hull trace it moves 6 units back along vForward
	// vecSrc = tr.endpos - vForward * 6
	Vector vecBack = vForward; vecBack *= 6.0f;
	vecSource = tr.endpos;
	vecSource -= vecBack;

	// Finally calculate velocity
	vecThrow = localplayer->GetVelocity(); 
	vecThrow *= 1.25f;
	vecThrow += vForward * flVel;
}

void GrenadePrediction::Simulate(C_BasePlayer* pLocal)
{
	Vector vecSrc, vecThrow;
	Setup(pLocal, vecSrc, vecThrow);

	float interval = I::globalVars->interval_per_tick;

	// Log positions 20 times per sec
	int logstep = static_cast<int>(0.05f / interval);
	int logtimer = 0;

	path.clear();
	for (unsigned int i = 0; i < 1024; ++i)
	{
		if (!logtimer)
			path.push_back(vecSrc);

		int s = Step(vecSrc, vecThrow, i, interval);
		if ((s & 1)) break;

		// Reset the log timer every logstep OR we bounced
		if ((s & 2) || logtimer >= logstep) logtimer = 0;
		else ++logtimer;
	}
	path.push_back(vecSrc);
}

int GrenadePrediction::Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval)
{
	// Apply gravity
	Vector move;
	AddGravityMove(move, vecThrow, interval, false);

	// Push entity
	trace_t tr;
	PushEntity(vecSrc, move, tr);

	int result = 0;
	// Check ending conditions
	if (CheckDetonate(vecThrow, tr, tick, interval))
	{
		result |= 1;
	}

	// Resolve collisions
	if (tr.fraction != 1.0f)
	{
		result |= 2; // Collision!
		ResolveFlyCollisionCustom(tr, vecThrow, interval);
	}

	// Set new position
	vecSrc = tr.endpos;

	return result;
}

bool GrenadePrediction::CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval)
{
	switch (*throwable->GetItemDefinitionIndex())
	{
	case ItemDefinitionIndex::WEAPON_SMOKEGRENADE:
	case ItemDefinitionIndex::WEAPON_DECOY:
		// Velocity must be <0.1, this is only checked every 0.2s
		if (vecThrow.Length() < 0.1f)
		{
			int det_tick_mod = static_cast<int>(0.2f / interval);
			return !(tick % det_tick_mod);
		}
		return false;

	case ItemDefinitionIndex::WEAPON_MOLOTOV:
	case ItemDefinitionIndex::WEAPON_INCGRENADE:
		// Detonate when hitting the floor
		if (tr.fraction != 1.0f && tr.plane.normal.z>0.7f)
			return true;
		// OR we've been flying for too long
		return static_cast<float>(tick) * interval > 2.f && !(tick % static_cast<int>(0.2f / interval));

	case ItemDefinitionIndex::WEAPON_FLASHBANG:
	case ItemDefinitionIndex::WEAPON_HEGRENADE:
		// Pure timer based, detonate at 1.5s, checked every 0.2s
		return static_cast<float>(tick) * interval > 1.5f && !(tick % static_cast<int>(0.2f / interval));

	default:
		return false;
	}
}

void GrenadePrediction::TraceHull(const Vector& src, const Vector& end, trace_t& tr)
{
	// Setup grenade hull
	static const Vector hull[2] = { Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f) };

	// FIXME! Bad codings...
	CGrenadePredictTraceFilter traceFilter;
	traceFilter.pSkip = throwable; // I::EntList->GetClientEntity(I::Engine->GetLocalPlayer());

	unsigned int mask = 0x200400B;
	Ray_t ray;
	ray.Init(src, end);
	I::Trace->TraceRay(ray, mask, &traceFilter, &tr);
}

void GrenadePrediction::AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground)
{
	Vector basevel(0.0f);

	move.x = (vel.x + basevel.x) * frametime;
	move.y = (vel.y + basevel.y) * frametime;

	if (onground)
	{
		move.z = (vel.z + basevel.z) * frametime;
	}
	else
	{
		// Game calls GetActualGravity( this );
		float gravity = 800.0f * 0.4f;

		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;

		vel.z = newZ;
	}
}

void GrenadePrediction::PushEntity(Vector& src, const Vector& move, trace_t& tr)
{
	Vector vecAbsEnd = src;
	vecAbsEnd += move;

	// Trace through world
	TraceHull(src, vecAbsEnd, tr);
}

void GrenadePrediction::ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval)
{
	// Calculate elasticity
	float flSurfaceElasticity = 1.0;  // Assume all surfaces have the same elasticity
	float flGrenadeElasticity = 0.45f; // GetGrenadeElasticity()
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity>0.9f) flTotalElasticity = 0.9f;
	if (flTotalElasticity<0.0f) flTotalElasticity = 0.0f;

	// Calculate bounce
	Vector vecAbsVelocity;
	PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;

	// Stop completely once we move too slow
	float flSpeedSqr = vecAbsVelocity.LengthSqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f; // 30.0f * 30.0f in CSS
	if (flSpeedSqr<flMinSpeedSqr)
	{
		vecAbsVelocity.Zero();
	}

	// Stop if on ground
	if (tr.plane.normal.z>0.7f)
	{
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.fraction) * interval);
		PushEntity(tr.endpos, vecAbsVelocity, tr);
	}
	else
	{
		vecVelocity = vecAbsVelocity;
	}
}

int GrenadePrediction::PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce)
{
	static const float STOP_EPSILON = 0.1f;

	float    backoff;
	float    change;
	float    angle;
	int        i, blocked;

	blocked = 0;

	angle = normal[2];

	if (angle > 0)
	{
		blocked |= 1;        // floor
	}
	if (!angle)
	{
		blocked |= 2;        // step
	}

	backoff = in.Dot(normal) * overbounce;

	for (i = 0; i<3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
		{
			out[i] = 0;
		}
	}

	return blocked;
}
