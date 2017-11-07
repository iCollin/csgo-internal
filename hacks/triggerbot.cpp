/*
      ___           ___           ___           ___     
     /\  \         |\__\         /\  \         /\__\    
    /::\  \        |:|  |       /::\  \       /::|  |   
   /:/\:\  \       |:|  |      /:/\:\  \     /:|:|  |   
  /:/  \:\  \      |:|__|__   /::\~\:\  \   /:/|:|  |__ 
 /:/__/ \:\__\     /::::\__\ /:/\:\ \:\__\ /:/ |:| /\__\
 \:\  \  \/__/    /:/~~/~~   \/__\:\/:/  / \/__|:|/:/  /
  \:\  \         /:/  /           \::/  /      |:/:/  / 
   \:\  \        \/__/            /:/  /       |::/  /  
    \:\__\                       /:/  /        /:/  /   
     \/__/                       \/__/         \/__/    

revolt (4/2017)
credit: AimTux (https://github.com/AimTuxOfficial/AimTux)
*/

#include "triggerbot.h"

#include "menu.h"
#include "utilities.h"
#include "globals.h"

#include "math.h"
#include "settings.h"
#include "autowall.h"
#include "render.h"

// triggerbot general config
bool Config::Triggerbot::Enable = true;
bool Config::Triggerbot::RequiresKey = true;
int Config::Triggerbot::Key = 6;

// triggerbot rifles config
bool Config::Triggerbot::Rifles::Enable = true;
float Config::Triggerbot::Rifles::WaitTime = 0.f;
bool Config::Triggerbot::Rifles::FlashCheck = true;

// triggerbot snipers config
bool Config::Triggerbot::Snipers::Enable = true;
float Config::Triggerbot::Snipers::WaitTime = 0.f;
bool Config::Triggerbot::Snipers::FlashCheck = true;

// triggerbot pistols config
bool Config::Triggerbot::Pistols::Enable = true;
float Config::Triggerbot::Pistols::WaitTime = 0.f;
bool Config::Triggerbot::Pistols::FlashCheck = true;

std::vector<std::pair<Vector, Vector>> Triggerbot::hitchanceLines;

bool HitChance(C_BasePlayer* local, C_BaseCombatWeapon* weapon)
{
	Vector forward, right, up;
	Math::AngleVectors(*local->GetEyeAngles(), forward, right, up);

	int minHits = Config::Aimbot::HitChance / 100.f * 256.f;
	int currentHits = 0;

	Triggerbot::hitchanceLines.clear();
	for (int i = 0; i<256; i++) {
		if ((255 - i + currentHits) < minHits) // still possible?
			break;

		weapon->UpdateAccuracyPenalty();
		auto a = Utilities::RandomFloat(0.f, M_PI * 2.f);
		auto b = Utilities::RandomFloat(0.f, weapon->GetSpread());
		auto c = Utilities::RandomFloat(0.f, M_PI * 2.f);
		auto d = Utilities::RandomFloat(0.f, weapon->GetInaccuracy());

		Vector vSpread;
		vSpread.x = (cos(a) * b) + (cos(c) * d);
		vSpread.y = (sin(a) * b) + (sin(c) * d);

		Vector vSpreadForward;
		vSpreadForward.x = forward.x + vSpread.x * right.x + vSpread.y * up.x;
		vSpreadForward.y = forward.y + vSpread.x * right.y + vSpread.y * up.y;
		vSpreadForward.z = forward.z + vSpread.x * right.z + vSpread.y * up.z;
		Math::NormalizeAngles(vSpreadForward);

		QAngle viewAngles;
		I::Engine->GetViewAngles(viewAngles);
		QAngle viewAngles_rcs = viewAngles + *local->GetAimPunchAngle() * 2.f;

		Vector end;
		Vector start = local->GetEyePosition();
		Math::AngleVectors(viewAngles_rcs, end);
		end = start + end * G::ActiveWeaponInfo->flRange; // weapon->GetCSWpnData()->flRange;

		Triggerbot::hitchanceLines.push_back({ start, end });

		Autowall::FireBulletData fbd;
		float damage = Autowall::GetDamage(end, !Config::Aimbot::FriendlyFire, fbd);

		if (damage < Config::Aimbot::AutoWall::MinDamage)
			continue;

		trace_t tr = fbd.enter_trace;

	/*	trace_t tr;
		Ray_t ray;
		ray.Init(local->GetEyePosition(), end);
		CTraceFilter traceFilter;
		traceFilter.pSkip = local;
		I::Trace->TraceRay(ray, 0x46004003, &traceFilter, &tr);*/

		C_BasePlayer* pEntity = (C_BasePlayer*)fbd.enter_trace.m_pEntityHit;
		if (!pEntity)
			continue;

		if (pEntity->GetClientClass()->m_ClassID != EClassIds::CCSPlayer)
			continue;

		if (pEntity == local || pEntity->GetDormant() || !pEntity->GetAlive() || pEntity->GetImmune())
			continue;

		if (!Config::Aimbot::FriendlyFire && pEntity->GetTeam() == local->GetTeam())
			continue;

		if (fbd.enter_trace.hitgroup == HitGroups::HITGROUP_GENERIC)
			continue;

		currentHits++;

		if (currentHits == minHits)
			return true;
	}

	return false;
}

void Triggerbot::CreateMove()
{
	if (!Config::Triggerbot::Enable)
		return;

	if (Config::Triggerbot::RequiresKey && !(GetAsyncKeyState(Config::Triggerbot::Key) & 0x8000))
		return;

	if (!G::LocalPlayer->GetAlive())
		return;

	// TODO split codepath for weapon types
	if (Config::Triggerbot::Rifles::FlashCheck && G::LocalPlayer->GetFlashBangTime() - I::globalVars->curtime > 2.f)
		return;

	Vector traceStart, traceEnd;
	trace_t tr;

	QAngle viewAngles;
	I::Engine->GetViewAngles(viewAngles);
	QAngle viewAngles_rcs = viewAngles + *G::LocalPlayer->GetAimPunchAngle() * 2.f;

	Math::AngleVectors(viewAngles_rcs, traceEnd);

	traceStart = G::LocalPlayer->GetEyePosition();
	traceEnd = traceStart + (traceEnd * 8192.f);

	if (Config::Aimbot::AutoWall::Enable)
	{
		Autowall::FireBulletData data;
		if (Autowall::GetDamage(traceEnd, !Config::Aimbot::FriendlyFire, data) == 0.0f)
			return;

		tr = data.enter_trace;
	}
	else
	{
		Ray_t ray;
		ray.Init(traceStart, traceEnd);
		CTraceFilter traceFilter;
		traceFilter.pSkip = G::LocalPlayer;
		I::Trace->TraceRay(ray, 0x46004003, &traceFilter, &tr);
	}

	C_BasePlayer* pEntity = (C_BasePlayer*)tr.m_pEntityHit;
	if (!pEntity) // fix the trace struct for windows (2 bytes off)
		return;

	if (tr.hitgroup == HitGroups::HITGROUP_GENERIC)
		return;

	if (pEntity->GetClientClass()->m_ClassID != EClassIds::CCSPlayer)
		return;

	if (pEntity == G::LocalPlayer || pEntity->GetDormant() || !pEntity->GetAlive() || pEntity->GetImmune())
		return;

	if (!Config::Aimbot::FriendlyFire && pEntity->GetTeam() == G::LocalPlayer->GetTeam())
		return;

	if (!G::ActiveWeapon || G::ActiveWeapon->GetAmmo() < 1)
		return;

	if (G::ActiveWeapon->GetNextPrimaryAttack() > I::globalVars->curtime)
		return;

	ItemDefinitionIndex itemDefinitionIndex = *G::ActiveWeapon->GetItemDefinitionIndex();
	if (itemDefinitionIndex == ItemDefinitionIndex::WEAPON_KNIFE || itemDefinitionIndex >= ItemDefinitionIndex::WEAPON_KNIFE_BAYONET)
		return;

	CSWeaponType weaponType = G::ActiveWeaponInfo->GetWeaponType(); // pWeapon->GetCSWpnData()->GetWeaponType(); //I::WeaponSystem->GetWpnData(*pWeapon->GetItemDefinitionIndex())->GetWeaponType();
	if (weaponType == CSWeaponType::WEAPONTYPE_C4 || weaponType == CSWeaponType::WEAPONTYPE_GRENADE)
		return;

	// ghetto ass hitchance please fix with real hitchance;
	//float hitchance = (1.f - pWeapon->GetAccuracyPenalty()) * 4.f;
	//if (hitchance < Config::Aimbot::HitChance / 25.f)
	//	return;

	// real hitchance?
	if (!HitChance(G::LocalPlayer, G::ActiveWeapon))
		return;

	if (G::ActiveWeapon->GetNextPrimaryAttack() > I::globalVars->curtime)
	{
		if (itemDefinitionIndex == ItemDefinitionIndex::WEAPON_REVOLVER)
			G::UserCmd->buttons &= ~IN_ATTACK2;
		else
			G::UserCmd->buttons &= ~IN_ATTACK;
	}
	else
	{
		if (itemDefinitionIndex == ItemDefinitionIndex::WEAPON_REVOLVER)
			G::UserCmd->buttons |= IN_ATTACK2;
		else
			G::UserCmd->buttons |= IN_ATTACK;
	}
}

void Triggerbot::PaintTraverse()
{
	for (auto pair : hitchanceLines)
	{
		Vector vStart, vEnd;

		if (!Render::WorldToScreen(pair.second, vEnd))
			continue;

		if (!Render::WorldToScreen(pair.first, vStart))
			vStart = Vector(960, 1080, 0);

		Render::Line(vStart.x, vStart.y, vEnd.x, vEnd.y, Color(5, 235, 55));
	}
}
