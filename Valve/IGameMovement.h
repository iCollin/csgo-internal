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

class CMoveData;

class IGameMovement
{
public:
	void ProcessMovement(IClientEntity* player, BYTE* move)
	{
		typedef void (__thiscall* oProcessMovement)(void*, IClientEntity*, BYTE*);
		return getvfunc<oProcessMovement>(this, 1)(this, player, move);
	}
};

class IMoveHelper
{
public:
	void SetHost(IClientEntity* host)
	{
		typedef void (__thiscall* oSetHost)(void*, IClientEntity*);
		return getvfunc<oSetHost>(this, 1)(this, host);
	}
};