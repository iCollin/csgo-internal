#include "entity.h"
#include "math.h"
#include "globals.h"
#include "offsets.h"

bool Entity::LineGoesThroughSmoke(Vector src, Vector dst, int16_t b)
{
	return reinterpret_cast<LineGoesThroughSmokeFn>(Offsets::LineGoesThroughSmoke)(src, dst, b);
}

bool Entity::IsPlayerVisible(C_BasePlayer* player, Bone bone, float fov, bool smoke_check)
{
	C_BasePlayer* pPlayer(G::LocalPlayer);
	if (!pPlayer)
		return true;

	if (player == pPlayer)
		return true;

	if (!pPlayer->GetAlive())
	{
		if (*pPlayer->GetObserverMode() == ObserverMode_t::OBS_MODE_IN_EYE && pPlayer->GetObserverTarget())
			pPlayer = (C_BasePlayer*)I::EntList->GetClientEntityFromHandle(pPlayer->GetObserverTarget());

		if (!pPlayer)
			return true;
	}

	Vector e_vecHead = player->GetBonePosition((int)bone);
	Vector p_vecHead = pPlayer->GetEyePosition();

	QAngle viewAngles;
	I::Engine->GetViewAngles(viewAngles);
	viewAngles += *pPlayer->GetAimPunchAngle();

	// TODO: scale fov by distance? its not really working that well...
	if (Math::GetFov(viewAngles, Math::CalcAngle(p_vecHead, e_vecHead)) > fov)
		return false;

	Ray_t ray;
	trace_t tr;
	ray.Init(p_vecHead, e_vecHead);
	CTraceFilter traceFilter;
	traceFilter.pSkip = pPlayer;
	I::Trace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

	// uncomment me :)
	if (smoke_check && LineGoesThroughSmoke(p_vecHead, e_vecHead, true))
		return false;

	return tr.m_pEntityHit == player;
}

bool Entity::IsVisible(C_BaseEntity* player, float fov, bool smoke_check)
{
	C_BasePlayer* pPlayer = G::LocalPlayer;
	if (!pPlayer)
		return true;

	if (player == pPlayer)
		return true;

	if (!pPlayer->GetAlive())
	{
		if (*pPlayer->GetObserverMode() == ObserverMode_t::OBS_MODE_IN_EYE && pPlayer->GetObserverTarget())
			pPlayer = (C_BasePlayer*)I::EntList->GetClientEntityFromHandle(pPlayer->GetObserverTarget());

		if (!pPlayer)
			return true;
	}

	Vector e_vecHead = player->GetVecOrigin();
	Vector p_vecHead = pPlayer->GetEyePosition();

	QAngle viewAngles;
	I::Engine->GetViewAngles(viewAngles);

	// TODO: scale fov by distance? its not really working that well...
	if (Math::GetFov(viewAngles, Math::CalcAngle(p_vecHead, e_vecHead)) > fov)
		return false;

	Ray_t ray;
	trace_t tr;
	ray.Init(p_vecHead, e_vecHead);
	CTraceFilter traceFilter;
	traceFilter.pSkip = pPlayer;
	I::Trace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

	// TODO find this function lol, think sigs are public
	if (smoke_check && LineGoesThroughSmoke(p_vecHead, e_vecHead, true))
		return false;

	return tr.m_pEntityHit == player;
}

bool Entity::IsPositionVisible(C_BasePlayer* pPlayer, Vector origin, float fov, bool smoke_check)
{
	if (!pPlayer)
	{ // SHOULD OVERLOAD. THIS IS DUMB. TODO FIX THIS
		pPlayer = G::LocalPlayer;

		if (!pPlayer)
			return true;
	}

	if (!pPlayer->GetAlive())
	{
		if (*pPlayer->GetObserverMode() == ObserverMode_t::OBS_MODE_IN_EYE && pPlayer->GetObserverTarget())
			pPlayer = (C_BasePlayer*)I::EntList->GetClientEntityFromHandle(pPlayer->GetObserverTarget());

		if (!pPlayer)
			return true;
	}

	origin.z += 0.65f;
	Vector e_vecHead = origin;
	Vector p_vecHead = pPlayer->GetEyePosition();

	QAngle viewAngles;
	I::Engine->GetViewAngles(viewAngles);

	// TODO: scale fov by distance? its not really working that well...
	if (Math::GetFov(viewAngles, Math::CalcAngle(p_vecHead, e_vecHead)) > fov)
		return false;

	Ray_t ray;
	trace_t tr;
	ray.Init(p_vecHead, e_vecHead);
	CTraceFilter traceFilter;
	traceFilter.pSkip = pPlayer;
	I::Trace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

	// TODO find this function lol, think sigs are public
	if (smoke_check && LineGoesThroughSmoke(p_vecHead, e_vecHead, true))
		return false;

	return tr.fraction == 1.0f;
}

bool Entity::IsPlanting(C_BasePlayer* player)
{
	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(player->GetActiveWeapon());
	if (!activeWeapon)
		return false;

	ClientClass* clientClass = activeWeapon->GetClientClass();
	if (!clientClass)
		return false;

	if (clientClass->m_ClassID != EClassIds::CC4)
		return false;

	return ((C_WeaponC4*)activeWeapon)->GetStartedArming();
}

Bone Entity::GetBoneByName(C_BasePlayer* player, const char* boneName)
{
	studiohdr_t* pStudioModel = I::ModelInfo->GetStudioModel(player->GetModel());
	if (!pStudioModel)
		return Bone::INVALID;

	matrix3x4_t pBoneToWorldOut[128];
	if (!player->SetupBones(pBoneToWorldOut, 128, 256, 0))
		return Bone::INVALID;

	for (int i = 0; i < pStudioModel->numbones; i++)
	{
		mstudiobone_t *pBone = pStudioModel->pBone(i);
		if (!pBone)
			continue;

		if (pBone->pszName() && strcmp(pBone->pszName(), boneName) == 0)
			return (Bone)i;
	}

	return Bone::INVALID;
}