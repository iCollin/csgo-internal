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

typedef unsigned short MaterialHandle_t;

class KeyValues;

class IMaterialSystem : public IAppSystem
{
public:
	IMaterial* CreateMaterial(const char* pMaterialName, KeyValues* pVMTKeyValues)
	{
		typedef IMaterial* (__thiscall* oCreateMaterial)(void*, const char*, KeyValues*);
		return getvfunc<oCreateMaterial>(this, 83)(this, pMaterialName, pVMTKeyValues);
	}

	IMaterial* FindMaterial(char const* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = NULL)
	{
		typedef IMaterial* (__thiscall* oFindMaterial)(void*, char const*, const char*, bool, const char*);
		return getvfunc<oFindMaterial>(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}

	MaterialHandle_t FirstMaterial()
	{
		typedef MaterialHandle_t (* oFirstMaterial)(void*);
		return getvfunc<oFirstMaterial>(this, 86)(this);
	}

	MaterialHandle_t NextMaterial(MaterialHandle_t h)
	{
		typedef MaterialHandle_t (* oNextMaterial)(void*, MaterialHandle_t);
		return getvfunc<oNextMaterial>(this, 87)(this, h);
	}

	MaterialHandle_t InvalidMaterial()
	{
		typedef MaterialHandle_t (* oInvalidMaterial)(void*);
		return getvfunc<oInvalidMaterial>(this, 88)(this);
	}

	IMaterial* GetMaterial(MaterialHandle_t h)
	{
		typedef IMaterial* (* oGetMaterial)(void*, MaterialHandle_t);
		return getvfunc<oGetMaterial>(this, 89)(this, h);
	}
};
