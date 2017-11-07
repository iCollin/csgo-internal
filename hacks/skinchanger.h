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
credit: Synraw @ unknowncheats.me
*/

#pragma once

#ifndef SKINCHANGER_H
#define SKINCHANGER_H

#include <unordered_map>
#include "IBaseClientDLL.h"

struct SkinObject_t
{
public:
	SkinObject_t() = default;
	SkinObject_t(int ind, int nSeed, int paint, float fl, bool stat, int trak, int quality, const char* n = "", int s1 = 0, int s2 = 0, int s3 = 0, int s4 = 0);

	int index;
	int seed;
	int iPaintKit;
	float flFloat;
	bool bStatTrak;
	int iStatTrak;
	int iEntityQuality;
	std::string name;

	int iStickerOne;
	int iStickerTwo;
	int iStickerThree;
	int iStickerFour;

	void AddStickers(int, int, int, int);
	int GetStickerID(int slot);
	AttribItem_t ToAttribItem();
};

namespace SkinChanger
{
	extern std::unordered_map<int, SkinObject_t> skin_map;
	
	void FrameStageNotify();

	SkinObject_t* GetSkinObjectByIndex(ItemDefinitionIndex);
	std::string GetSkinNameByIndex(int);

	void Initialize();
	void SaveConfig();
	void LoadConfig();

	void SaveMenuToMap();
	void ForceRefresh();

	void Hooked_RecvProxy_Viewmodel(const CRecvProxyData*, void*, void*);
}

#endif