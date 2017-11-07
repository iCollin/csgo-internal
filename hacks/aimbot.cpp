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

#include "aimbot.h"
#include "settings.h"

#include "math.h"
#include "entity.h"
#include "autowall.h"
#include "globals.h"

// general aimbot settings declarations
bool Config::Aimbot::Enable = true;
bool Config::Aimbot::FriendlyFire = false;
int Config::Aimbot::SelectionStyle = 0;
std::vector<std::string> Config::Aimbot::SelectionStyles = { "Crosshair", "Health", "Distance" };
bool Config::Aimbot::RequiresKey = true;
int Config::Aimbot::Key = 1;
std::vector<std::string> Config::Aimbot::Bones = { "Head", "Neck", "Chest", "Stomach", "Closest", "Best" };
float Config::Aimbot::SaltAmount = 0.01f;
bool Config::Aimbot::AutoCrouch = false;
bool Config::Aimbot::AutoScope = true;
bool Config::Aimbot::IgnoreJump = false;
bool Config::Aimbot::VisCheck = true;
bool Config::Aimbot::OneTarget = true;
float Config::Aimbot::HitChance = 50.f;

// autowall config
bool Config::Aimbot::AutoWall::Enable = false;
float Config::Aimbot::AutoWall::MinDamage = 25.f;

// RCS config
bool Config::Aimbot::Prediction = false;
int Config::Aimbot::RCS::Mode = 0;
std::vector<std::string> Config::Aimbot::RCS::Modes = { "Off", "Perfect", "Great", "Good", "Average", "Pull Down Nerd" };
float Config::Aimbot::RCS::xMin = 1.7f;
float Config::Aimbot::RCS::xMax = 2.0f;
float Config::Aimbot::RCS::yMin = 1.4f;
float Config::Aimbot::RCS::yMax = 2.0f;

// aimbot rifles config
bool Config::Aimbot::Rifles::Enable = true;
bool Config::Aimbot::Rifles::MaxFOV = false;
float Config::Aimbot::Rifles::minFOV = 0;
float Config::Aimbot::Rifles::FOV = 1.2;
float Config::Aimbot::Rifles::maxFOV = 8;
int Config::Aimbot::Rifles::Bone = 0;
bool Config::Aimbot::Rifles::RCS::Enable = true;
bool Config::Aimbot::Rifles::Smooth = true;
float Config::Aimbot::Rifles::SmoothingRatio = 16.f;
bool Config::Aimbot::Rifles::Salt = true;
bool Config::Triggerbot::Rifles::SmokeCheck = true;


// aimbot snipers config
bool Config::Aimbot::Snipers::Enable = true;
bool Config::Aimbot::Snipers::MaxFOV = false;
float Config::Aimbot::Snipers::minFOV = 0;
float Config::Aimbot::Snipers::FOV = 1.2;
float Config::Aimbot::Snipers::maxFOV = 8;
int Config::Aimbot::Snipers::Bone = 0;
bool Config::Aimbot::Snipers::RCS::Enable = true;
bool Config::Aimbot::Snipers::Smooth = true;
float Config::Aimbot::Snipers::SmoothingRatio = 16.f;
bool Config::Aimbot::Snipers::Salt = true;
bool Config::Triggerbot::Snipers::SmokeCheck = true;

// aimbot pistols config
bool Config::Aimbot::Pistols::Enable = true;
bool Config::Aimbot::Pistols::MaxFOV = false;
float Config::Aimbot::Pistols::minFOV = 0;
float Config::Aimbot::Pistols::FOV = 1.2;
float Config::Aimbot::Pistols::maxFOV = 8;
int Config::Aimbot::Pistols::Bone = 0;
bool Config::Aimbot::Pistols::RCS::Enable = true;
bool Config::Aimbot::Pistols::Smooth = true;
float Config::Aimbot::Pistols::SmoothingRatio = 16.f;
bool Config::Aimbot::Pistols::Salt = true;
bool Config::Triggerbot::Pistols::SmokeCheck = true;

WeaponType_t GetWeaponTypeFromClassID(EClassIds id)
{
	switch (id)
	{
	case EClassIds::CWeaponAWP:
	case EClassIds::CWeaponSSG08:
	case EClassIds::CWeaponSCAR20:
	case EClassIds::CWeaponG3SG1:
		return WeaponType_t::SNIPER;
	case EClassIds::CWeaponGlock:
	case EClassIds::CWeaponUSP:
	case EClassIds::CWeaponHKP2000:
	case EClassIds::CWeaponP250:
	case EClassIds::CWeaponFiveSeven:
	case EClassIds::CWeaponTec9:
	case EClassIds::CDEagle:
	case EClassIds::CWeaponElite:
		return WeaponType_t::PISTOL;
	case EClassIds::CAK47:
	case EClassIds::CWeaponM4A1:
	case EClassIds::CWeaponFamas:
	case EClassIds::CWeaponGalil:
	case EClassIds::CWeaponAug:
	case EClassIds::CWeaponSG550:
	case EClassIds::CWeaponSG552:
	case EClassIds::CWeaponSG556:
		return WeaponType_t::RIFLE;
	case EClassIds::CWeaponUMP45:
	case EClassIds::CWeaponP90:
	case EClassIds::CWeaponMP9:
	case EClassIds::CWeaponMP7:
	case EClassIds::CWeaponMAC10:
	case EClassIds::CWeaponBizon:
		return WeaponType_t::SMG;
	default:
		return WeaponType_t::INVALID;
	}
}

static void ApplyErrorToAngle(QAngle* angles, float margin)
{
	QAngle error;
	error.Random(-1.0f, 1.0f);
	error *= margin;
	angles->operator+=(error);
}

Vector VelocityExtrapolate(C_BasePlayer* player, Vector aimPos)
{
	return aimPos + (player->GetVelocity() * I::globalVars->interval_per_tick);
}

int GetConfigBone(WeaponType_t type)
{
	if (type == WeaponType_t::SNIPER)
		return Config::Aimbot::Snipers::Bone;

	if (type == WeaponType_t::PISTOL)
		return Config::Aimbot::Pistols::Bone;

	return Config::Aimbot::Rifles::Bone;
}

int GetSmoothAmount(WeaponType_t type)
{
	if (type == WeaponType_t::SNIPER)
		return Config::Aimbot::Snipers::SmoothingRatio;

	if (type == WeaponType_t::PISTOL)
		return Config::Aimbot::Pistols::SmoothingRatio;

	return Config::Aimbot::Rifles::SmoothingRatio;
}

float GetMaxFOV(WeaponType_t type)
{
	if (type == WeaponType_t::SNIPER)
		return Config::Aimbot::Snipers::MaxFOV ? 180.f : Config::Aimbot::Snipers::FOV;

	if (type == WeaponType_t::PISTOL)
		return Config::Aimbot::Pistols::MaxFOV ? 180.f : Config::Aimbot::Pistols::FOV;

	return Config::Aimbot::Rifles::MaxFOV ? 180.f : Config::Aimbot::Rifles::FOV;
}

bool GetShouldSmooth(WeaponType_t type)
{
	if (type == WeaponType_t::SNIPER)
		return Config::Aimbot::Snipers::Smooth;

	if (type == WeaponType_t::PISTOL)
		return Config::Aimbot::Pistols::Smooth;

	return Config::Aimbot::Rifles::Smooth;
}

bool GetShouldSalt(WeaponType_t type)
{
	if (type == WeaponType_t::SNIPER)
		return Config::Aimbot::Snipers::Salt;

	if (type == WeaponType_t::PISTOL)
		return Config::Aimbot::Pistols::Salt;

	return Config::Aimbot::Rifles::Salt;
}

bool GetShouldRCS(WeaponType_t type)
{
	if (type == WeaponType_t::SNIPER)
		return Config::Aimbot::Snipers::RCS::Enable;

	if (type == WeaponType_t::PISTOL)
		return Config::Aimbot::Pistols::RCS::Enable;

	return Config::Aimbot::Rifles::RCS::Enable;
}

bool GetFlashCheck(WeaponType_t type)
{
	if (type == WeaponType_t::SNIPER)
		return Config::Triggerbot::Snipers::FlashCheck;

	if (type == WeaponType_t::PISTOL)
		return Config::Triggerbot::Pistols::FlashCheck;

	return Config::Triggerbot::Rifles::FlashCheck;
}

bool GetSmokeCheck(WeaponType_t type)
{
	if (type == WeaponType_t::SNIPER)
		return Config::Triggerbot::Snipers::SmokeCheck;

	if (type == WeaponType_t::PISTOL)
		return Config::Triggerbot::Pistols::SmokeCheck;

	return Config::Triggerbot::Rifles::SmokeCheck;
}

bool GetRCSEnabled(WeaponType_t type)
{
	if (type == WeaponType_t::SNIPER)
		return Config::Aimbot::Snipers::RCS::Enable;

	if (type == WeaponType_t::PISTOL)
		return Config::Aimbot::Pistols::RCS::Enable;

	return Config::Aimbot::Rifles::RCS::Enable;
}

static std::vector<Bone> possibleBones = { Bone::BONE_HEAD, Bone::CHEST, Bone::BONE_NECK, Bone::BONE_PELVIS, Bone::STOMACH, Bone::LEFT_ELBOW, Bone::RIGHT_ELBOW, Bone::LEFT_HAND, Bone::RIGHT_HAND };

Bone GetClosestBone(C_BasePlayer* local, C_BasePlayer* target)
{
	Bone desired = Bone::BONE_HEAD;

	Vector source = local->GetEyePosition();

	QAngle viewAngles;
	I::Engine->GetViewAngles(viewAngles);

	float minFOV = 180.f;
	for (auto bone : possibleBones)
	{
		float fov = Aimbot::CalculateFOV(source, target->GetBonePosition((int)bone), viewAngles);

		if (fov < minFOV)
		{
			desired = bone;
			minFOV = fov;
		}
	}

	return desired;
}

Bone GetBestBone(C_BasePlayer* local, C_BasePlayer* target)
{
	Bone desired = Bone::BONE_HEAD;

	Vector source = local->GetEyePosition();

	QAngle viewAngles;
	I::Engine->GetViewAngles(viewAngles);

	float maxDamage = 1.f;
	for (auto bone : possibleBones)
	{
		Autowall::FireBulletData fbd;
		float damage = Autowall::GetDamage(target->GetBonePosition((int)bone), !Config::Aimbot::FriendlyFire, fbd);

		if (damage > maxDamage)
		{
			desired = bone;
			maxDamage = damage;
		}
	}

	return desired;
}

Bone Aimbot::GetTargetBone(C_BasePlayer* pLocal, C_BasePlayer* target, WeaponType_t type)
{
	if (type == WeaponType_t::INVALID)
	{
		auto pWeapon = I::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeapon());

		if (!pWeapon)
			return Bone::BONE_HEAD;

		type = GetWeaponTypeFromClassID(pWeapon->GetClientClass()->m_ClassID);
	}

	int item = Config::Aimbot::Rifles::Bone;
	if (type == WeaponType_t::SNIPER) item = Config::Aimbot::Snipers::Bone;
	else if (type == WeaponType_t::PISTOL) item = Config::Aimbot::Pistols::Bone;

	switch (item)
	{
	case 0:
		return Bone::BONE_HEAD;
	case 1:
		return Bone::BONE_NECK;
	case 2:
		return Bone::CHEST;
	case 3:
		return Bone::STOMACH;
	case 4:
		return GetClosestBone(pLocal, target);
	case 5:
		return GetBestBone(pLocal, target);
	default:
		return Bone::BONE_HEAD;
	}
}

QAngle Aimbot::CalculateAngle(Vector src, Vector dst)
{
	QAngle angles;
	Vector delta = src - dst;

	Math::VectorAngles(delta, angles);

	Math::NormalizeAngles(angles);

	return angles;
}

float Aimbot::CalculateFOV(Vector src, Vector dst, QAngle view)
{
	QAngle delta = CalculateAngle(src, dst) - view;
	
	Math::NormalizeAngles(delta);

	return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}

C_BasePlayer* GetTargetByFOV(C_BasePlayer* pLocal, CUserCmd* pCmd) // TODO intake target bone
{
	auto pWeapon = I::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeapon());
	auto type = GetWeaponTypeFromClassID(pWeapon->GetClientClass()->m_ClassID);

	float minFOV = GetMaxFOV(type);

	C_BasePlayer* pTarget = nullptr;

	Vector src = pLocal->GetEyePosition();
	QAngle viewAngles(pCmd->viewangles);
	viewAngles += *pLocal->GetAimPunchAngle();

	for (int i = 1; i < I::Engine->GetMaxClients(); i++)
	{
		C_BasePlayer* pEntity = (C_BasePlayer*)I::EntList->GetClientEntity(i);

		if (!pEntity || pEntity == pLocal || pEntity->GetDormant() || !pEntity->GetAlive() || pEntity->GetImmune())
			continue;

		if (!Config::Aimbot::FriendlyFire && pEntity->GetTeam() == pLocal->GetTeam())
			continue;

		Vector dst = pEntity->GetBonePosition((int)Bone::CHEST);

		float fov = Aimbot::CalculateFOV(src, dst, viewAngles);

		if (fov < minFOV)
		{
			minFOV = fov;
			pTarget = pEntity;
		}
	}

	return pTarget;
}

C_BasePlayer* GetTargetByHealth(C_BasePlayer* pLocal, CUserCmd* pCmd) // TODO intake target bone
{
	auto pWeapon = I::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeapon());
	auto type = GetWeaponTypeFromClassID(pWeapon->GetClientClass()->m_ClassID);

	int minHealth = 101;
	float maxFOV = GetMaxFOV(type);

	C_BasePlayer* pTarget = nullptr;

	Vector src = pLocal->GetEyePosition();
	QAngle viewAngles;
	I::Engine->GetViewAngles(viewAngles);

	for (int i = 1; i < I::Engine->GetMaxClients(); i++)
	{
		C_BasePlayer* pEntity = (C_BasePlayer*)I::EntList->GetClientEntity(i);

		if (!pEntity || pEntity == pLocal || pEntity->GetDormant() || !pEntity->GetAlive() || pEntity->GetImmune())
			continue;

		if (!Config::Aimbot::FriendlyFire && pEntity->GetTeam() == pLocal->GetTeam())
			continue;

		if (Math::GetFov(viewAngles, Math::CalcAngle(src, pEntity->GetBonePosition((int)Bone::CHEST))) > maxFOV)
			continue;

		int health = pEntity->GetHealth();

		if (health < minHealth)
		{
			minHealth = health;
			pTarget = pEntity;
		}
		else if (health == minHealth)
		{
			float targetFOV = Math::GetFov(viewAngles, Math::CalcAngle(src, pTarget->GetBonePosition((int)Bone::CHEST)));
			float entityFOV = Math::GetFov(viewAngles, Math::CalcAngle(src, pEntity->GetBonePosition((int)Bone::CHEST)));

			if (entityFOV < targetFOV)
				pTarget = pEntity;
		}
	}

	return pTarget;
}

C_BasePlayer* GetTargetByDistance(C_BasePlayer* pLocal, CUserCmd* pCmd) // TODO intake target bone
{
	auto pWeapon = I::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeapon());
	auto type = GetWeaponTypeFromClassID(pWeapon->GetClientClass()->m_ClassID);

	float minDistance = 99999.f;
	float maxFOV = GetMaxFOV(type);

	Vector localPos = pLocal->GetVecOrigin();

	Vector src = pLocal->GetEyePosition();
	QAngle viewAngles;
	I::Engine->GetViewAngles(viewAngles);

	C_BasePlayer* pTarget = nullptr;

	for (int i = 1; i < I::Engine->GetMaxClients(); i++)
	{
		C_BasePlayer* pEntity = (C_BasePlayer*)I::EntList->GetClientEntity(i);

		if (!pEntity || pEntity == pLocal || pEntity->GetDormant() || !pEntity->GetAlive() || pEntity->GetImmune())
			continue;

		if (!Config::Aimbot::FriendlyFire && pEntity->GetTeam() == pLocal->GetTeam())
			continue;

		if (Math::GetFov(viewAngles, Math::CalcAngle(src, pEntity->GetBonePosition((int)Bone::CHEST))) > maxFOV)
			continue;

		float distance = (pEntity->GetVecOrigin() - localPos).Length();

		if (distance < minDistance)
		{
			minDistance = distance;
			pTarget = pEntity;
		}
	}

	return pTarget;
}

C_BasePlayer* GetTarget(C_BasePlayer* local, CUserCmd* cmd)
{
	if (Config::Aimbot::SelectionStyle == 0)
		return GetTargetByFOV(local, cmd);
	
	if (Config::Aimbot::SelectionStyle == 1)
		return GetTargetByHealth(local, cmd);

	if (Config::Aimbot::SelectionStyle == 2)
		return GetTargetByDistance(local, cmd);

	return nullptr;  // shut up compiler
}

void ApplySmooth(QAngle* viewAngles, QAngle& aimAngles, int divisor)
{
	// TODO make legit, also add config for all values
	Vector delta = Vector(aimAngles - *viewAngles);

	Math::NormalizeAngles(delta);

	//int divisor = type == WeaponType_t::PISTOL ? Config::Aimbot::Pistols::SmoothingRatio : type == WeaponType_t::SNIPER ? Config::Aimbot::Snipers::SmoothingRatio : Config::Aimbot::Rifles::SmoothingRatio;

	// basically an aimstep
	if (delta.Length() > .05f)
	{
		delta /= divisor;
	}

	aimAngles = *viewAngles + delta;
}

void ApplySalt(QAngle* viewAngles, float chance, float effect)
{
	if (rand() % 100 > chance)
		return;

	// do we want this? not sure
	if (rand() % 100 < 50) effect /= 2.f;

	viewAngles->x += rand() % 100 < 50 ? effect : -effect;
	viewAngles->y += rand() % 100 < 50 ? effect : -effect;
}

Vector GetRecoilCompensation()
{
	switch (Config::Aimbot::RCS::Mode)
	{
	case 1:
		return Vector(2.f, 2.f, 0.f);
	case 2:
		return Vector(1.9f, 1.8f, 0.f);
	case 3:
		return Vector(1.8f, 1.6f, 0.f);
	case 4:
		return Vector(1.6f, 1.2f, 0.f);
	case 5:
		return Vector(1.2f, 0.8f, 0.f);
	}

	return Vector(0.f, 0.f, 0.f);
}

void ApplyRCS(CUserCmd* pCmd, C_BasePlayer* pLocal)
{
	QAngle aimPunch = *pLocal->GetAimPunchAngle();
	vec_t punchLen = aimPunch.Length2D();

	if (punchLen < 0 || punchLen == 0 || punchLen > 120)
		return;

	Vector compensation = GetRecoilCompensation();

	if (compensation.x == 0.f)
		return;

	pCmd->viewangles -= compensation * aimPunch;
}

void AutoScope(CUserCmd* pCmd, C_BasePlayer* local, WeaponType_t type)
{
	if (!Config::Aimbot::AutoScope)
		return;

	if (type != WeaponType_t::SNIPER)
		return;

	if (local->IsScoped())
		return;

	pCmd->buttons |= IN_ATTACK2;
}

void AutoCrouch(CUserCmd* pCmd)
{
	if (!Config::Aimbot::AutoCrouch)
		return;

	pCmd->buttons |= IN_DUCK;
}

void AutoStop(CUserCmd* pCmd, C_BasePlayer* pLocal)
{
	Vector velocity = pLocal->GetVelocity();
	QAngle direction;
	Math::VectorAngles(velocity, direction);
	float speed = velocity.Length();

	direction.y = pCmd->viewangles.y - direction.y;

	Vector negated_direction = direction.Forward() * -speed;
	pCmd->forwardmove = negated_direction.x;
	pCmd->sidemove = negated_direction.y;
}

void Aimbot::CreateMove()
{
	if (!Config::Aimbot::Enable)
		return;

	if (!I::Engine->IsInGame())
		return;

	if (!G::LocalPlayer->GetAlive())
		return;

	static C_BasePlayer* pTarget = nullptr;

	C_BaseCombatWeapon* pWeapon = reinterpret_cast<C_BaseCombatWeapon*>(I::EntList->GetClientEntityFromHandle(G::LocalPlayer->GetActiveWeapon()));

	if (!pWeapon || pWeapon->GetAmmo() < 1 || pWeapon->GetInReload())
		return;

	WeaponType_t type = GetWeaponTypeFromClassID(pWeapon->GetClientClass()->m_ClassID);

	if (pWeapon->GetNextPrimaryAttack() > I::globalVars->curtime)
	{
		pTarget = nullptr;

		if (GetShouldRCS(type))
			ApplyRCS(G::UserCmd, G::LocalPlayer);

		return;
	}

	CSWeaponType weaponType = G::ActiveWeaponInfo->GetWeaponType(); //pWeapon->GetCSWpnData()->GetWeaponType();
	if (weaponType == CSWeaponType::WEAPONTYPE_C4 || weaponType == CSWeaponType::WEAPONTYPE_GRENADE || weaponType == CSWeaponType::WEAPONTYPE_KNIFE)
		return;

	if (Config::Aimbot::RequiresKey && !(GetAsyncKeyState(Config::Aimbot::Key) & 0x8000))
	{
		pTarget = nullptr;

		if (GetShouldRCS(type))
			ApplyRCS(G::UserCmd, G::LocalPlayer);

		return;
	}

	QAngle oldAngle;
	I::Engine->GetViewAngles(oldAngle);
	float oldForward = G::UserCmd->forwardmove;
	float oldSideMove = G::UserCmd->sidemove;

	// TODO: TARGET SELECTION
	if (!pTarget || !Config::Aimbot::OneTarget)
		pTarget = GetTarget(G::LocalPlayer, G::UserCmd);

	if (!pTarget || !pTarget->GetAlive())
	{
		if (GetShouldRCS(type))
			ApplyRCS(G::UserCmd, G::LocalPlayer);

		return;
	}

	int targetBone = (int)GetTargetBone(G::LocalPlayer, pTarget, type);

	if (Config::Aimbot::AutoWall::Enable)
	{
		Autowall::FireBulletData data;
		float damage = Autowall::GetDamage(pTarget->GetBonePosition(targetBone), false, data);

		if (damage < Config::Aimbot::AutoWall::MinDamage)
		{
			pTarget = nullptr;

			if (GetShouldRCS(type))
				ApplyRCS(G::UserCmd, G::LocalPlayer);

			return;
		}
	}
	else if (Config::Aimbot::VisCheck && !Entity::IsPlayerVisible(pTarget, (Bone)targetBone, 180.f, GetSmokeCheck(type)))
	{
		pTarget = nullptr;

		if (GetShouldRCS(type))
			ApplyRCS(G::UserCmd, G::LocalPlayer);

		return;
	}

	Vector ePos = G::LocalPlayer->GetEyePosition();
	Vector pPos = pTarget->GetBonePosition(targetBone);

	if (Config::Aimbot::Prediction)
	{
		ePos = VelocityExtrapolate(G::LocalPlayer, ePos);
		pPos = VelocityExtrapolate(pTarget, pPos);
	}

	QAngle angle = Math::CalcAngle(ePos, pPos);
	Math::NormalizeAngles(angle);
	Math::ClampAngles(angle);

	if (GetShouldSmooth(type))
		ApplySmooth(&G::UserCmd->viewangles, angle, GetSmoothAmount(type));

	if (GetShouldRCS(type))
		ApplyRCS(G::UserCmd, G::LocalPlayer);

	if (GetShouldSalt(type))
		ApplyErrorToAngle(&angle, Config::Aimbot::SaltAmount);

	// if (GetShouldStop()
	//AutoStop(G::UserCmd, G::LocalPlayer);

	if (angle != G::UserCmd->viewangles)
	{
		G::UserCmd->viewangles = angle;
	}

	AutoScope(G::UserCmd, G::LocalPlayer, type);
	AutoCrouch(G::UserCmd);

	Math::NormalizeAngles(G::UserCmd->viewangles);
	Math::ClampAngles(G::UserCmd->viewangles);
	Math::CorrectMovement(oldAngle, G::UserCmd, oldForward, oldSideMove);

	// IF (!SILENT_AIM)
	I::Engine->SetViewAngles(G::UserCmd->viewangles);
}