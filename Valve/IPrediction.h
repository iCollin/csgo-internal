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

#include "IInputSystem.h"
#include "IGameMovement.h"

class IPrediction
{
public:
	void SetupMove(IClientEntity* player, CUserCmd* cmd, IMoveHelper* helper, BYTE* move)
	{
		typedef void (__thiscall* oSetupMove)(void*, IClientEntity*, CUserCmd*, IMoveHelper*, BYTE*);
		return getvfunc<oSetupMove>(this, 20)(this, player, cmd, helper, move);
	}

	void FinishMove(IClientEntity* player, CUserCmd* cmd, BYTE* move)
	{
		typedef void (__thiscall* oFinishMove)(void*, IClientEntity*, CUserCmd*, BYTE*);
		return getvfunc<oFinishMove>(this, 21)(this, player, cmd, move);
	}
};