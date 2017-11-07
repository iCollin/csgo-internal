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

#pragma once

#ifndef AIMBOT_H
#define AIMBOT_H

#include "interfaces.h"

namespace Aimbot
{
	float CalculateFOV(Vector src, Vector dst, QAngle view);
	QAngle CalculateAngle(Vector src, Vector dst);

	Bone GetTargetBone(C_BasePlayer* pLocal, C_BasePlayer* target, WeaponType_t type = WeaponType_t::INVALID);

	void CreateMove();

	/*
	extern bool aimStepInProgress;
	extern std::vector<int64_t> friends;

	void RCS(QAngle& angle, C_BasePlayer* player, CUserCmd* cmd, WeaponType_t type);
	void AimStep(C_BasePlayer* player, QAngle& angle, CUserCmd* cmd, WeaponType_t type);
	void Smooth(C_BasePlayer* player, QAngle& angle, CUserCmd* cmd, WeaponType_t type);
	void AutoCrouch(C_BasePlayer* player, CUserCmd* cmd);
	void AutoSlow(C_BasePlayer* player, float& forward, float& sideMove, float& bestDamage, C_BaseCombatWeapon* active_weapon, CUserCmd* cmd);
	void AutoPistol(C_BaseCombatWeapon* activeWeapon, CUserCmd* cmd);
	void AutoShoot(C_BasePlayer* player, C_BaseCombatWeapon* activeWeapon, CUserCmd* cmd);
	void ShootCheck(C_BaseCombatWeapon* activeWeapon, CUserCmd* cmd);
	void NoShoot(C_BaseCombatWeapon* activeWeapon, C_BasePlayer* player, CUserCmd* cmd);

	void CreateMove(CUserCmd* cmd);
	void FireGameEvent(IGameEvent* event);*/
}

#endif // !AIMBOT_H


