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

#include "IClientEntity.h"

class CBaseHandle;

class IClientEntityList
{
public:
	virtual IClientNetworkable* GetClientNetworkable(int entindex) = 0;
	virtual IClientNetworkable* GetClientNetworkableFromHandle(CBaseHandle handle) = 0;
	virtual IClientUnknown* GetClientUnknownFromHandle(CBaseHandle handle) = 0;
	virtual C_BasePlayer* GetClientEntity(int entindex) = 0;
	// FIXME: GetClientEntityFromHandle should accept a CBaseHandle.
	virtual C_BasePlayer* GetClientEntityFromHandle(HANDLE handle) = 0;
	virtual int NumberOfEntities(bool include_non_networkable) = 0;
	virtual int GetHighestEntityIndex(void) = 0;
	virtual void SetMaxEntities(int max_entities) = 0;
	virtual int GetMaxEntities() = 0;
};
