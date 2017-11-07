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
*/

#pragma once

#ifndef OFFSETS_H
#define OFFSETS_H

#include "utilities.h"

namespace Offsets
{
	uintptr_t FindPattern(std::string module, std::string pattern);
	uintptr_t FindPatternMask(std::string, BYTE*, char*);

	extern uintptr_t GetLastCompetitiveMatchID;
	extern uintptr_t LoadSky;
	extern uintptr_t KeyValues_Init;
	extern uintptr_t KeyValues_LoadFromBuffer;
	extern uintptr_t LineGoesThroughSmoke;
	extern uintptr_t IsReady;
	extern uintptr_t RankRevealAll;
	extern uintptr_t PaintkitSig;
	extern uintptr_t StickerSig;

	// Grabs the Modules
	void Initialize();

	// Addresses of loaded game modules
	namespace Modules
	{
		extern DWORD Client;
		extern DWORD Engine;
		extern DWORD VGUI2;
		extern DWORD VGUISurface;
		extern DWORD Material;
		extern DWORD VPhysics;
		extern DWORD Stdlib;
	};
};

#endif