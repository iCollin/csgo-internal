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

#include "offsets.h"

namespace Offsets
{
	bool AllOffsetsFound = false;

	uintptr_t FindPattern(std::string module, std::string pattern)
	{
		auto temp = Utilities::Memory::FindPattern(module, pattern);

		if (!temp)
		{
			AllOffsetsFound = false;
			Utilities::Error("Failed to find pattern \"" + pattern + "\" in module " + module + ".");
		}

		return temp;
	}

	uintptr_t FindPatternMask(std::string module, BYTE* mask, char* szMask)
	{
		auto temp = Utilities::Memory::FindPatternMask(module, mask, szMask);

		if (!temp)
		{
			AllOffsetsFound = false;
			Utilities::Error("Failed to find mask \"" + std::string(szMask) + "\" in module " + module + ".");
		}

		return temp;
	}

	uintptr_t GetLastCompetitiveMatchID;
	uintptr_t LoadSky;
	uintptr_t KeyValues_Init;
	uintptr_t KeyValues_LoadFromBuffer;
	uintptr_t LineGoesThroughSmoke;
	uintptr_t IsReady;
	uintptr_t RankRevealAll;
	uintptr_t PaintkitSig;
	uintptr_t StickerSig;

	DWORD Modules::Client;
	DWORD Modules::Engine;
	DWORD Modules::VGUI2;
	DWORD Modules::VGUISurface;
	DWORD Modules::Material;
	DWORD Modules::VPhysics;
	DWORD Modules::Stdlib;

	void Initialize()
	{
		Modules::Client = Utilities::Memory::WaitOnModuleHandle("client.dll");
		Modules::Engine = Utilities::Memory::WaitOnModuleHandle("engine.dll");
		Modules::VGUI2 = Utilities::Memory::WaitOnModuleHandle("vgui2.dll");
		Modules::VGUISurface = Utilities::Memory::WaitOnModuleHandle("vguimatsurface.dll");
		Modules::Material = Utilities::Memory::WaitOnModuleHandle("materialsystem.dll");
		Modules::VPhysics = Utilities::Memory::WaitOnModuleHandle("vphysics.dll");
		Modules::Stdlib = Utilities::Memory::WaitOnModuleHandle("vstdlib.dll");

		AllOffsetsFound = true;
		GetLastCompetitiveMatchID = Offsets::FindPattern("client.dll", "55 8B EC A1 ? ? ? ? 83 EC 08 D1 E8 56 57 A8 01 74 59 8B ? ? ? ? ? 8B C1");
		LoadSky = Offsets::FindPattern("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45");
		KeyValues_Init = Offsets::FindPatternMask("client.dll", (BYTE*)"\x68\x00\x00\x00\x00\x8B\xC8\xE8\x00\x00\x00\x00\x89\x45\xFC\xEB\x07\xC7\x45\x00\x00\x00\x00\x00\x8B\x03\x56", "x????xxx????xxxxxxx?????xxx") + 7; // 10/1/17 "E8 ? ? ? ? 8B 87 EC 09 00 00" // "55 8B EC 51 33 C0 C7 45" <- first ref (luck) 10/3/17
		KeyValues_LoadFromBuffer = Offsets::FindPattern("client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04");
		LineGoesThroughSmoke = Offsets::FindPattern("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
		IsReady = Offsets::FindPattern("client.dll", "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE");
		RankRevealAll = Offsets::FindPatternMask("client.dll", (BYTE*)"\x8B\xEC\x8B\x0D\x00\x00\x00\x00\x68", "xxxx????x") - 1;
		PaintkitSig = Offsets::FindPatternMask("client.dll", (BYTE*)"\xE8\x00\x00\x00\x00\xFF\x76\x0C\x8D\x48\x04\xE8", "x????xxxxxxx");
		StickerSig = Offsets::FindPatternMask("client.dll", (BYTE*)"\x53\x8D\x48\x04\xE8\x00\x00\x00\x00\x8B\x4D\x10", "xxxxx????xxx") + 4;

		if (!AllOffsetsFound) Utilities::Error(std::string('*', 40) + "\n FAILED TO FIND ALL OFFSETS, ONE OR MORE SIGNATURES IS OUTDATED. PROCEED WITH CAUTION.\n" + std::string('*', 40));
		else Utilities::Log("All offsets found successfully.");
	}
}