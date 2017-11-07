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

// comment when distributed
#define __bypass

#include <Windows.h>

#include "skinchanger.h"
#include "offsets.h"
#include "interfaces.h"
#include "netvar.h"
#include "fonts.h"
#include "menu.h"
#include "hooks.h"
#include "GUI.h"
#include "skinparser.h"
#include "PhoneHome.h"

//#include "OverlayRendering.h"

HINSTANCE Utilities::hModule;
int Utilities::LocalSteamID = -99999999;

DWORD WINAPI init(LPVOID lpParam)
{
	I::Initialize();
	Utilities::Debug("Injection begun...");
	Offsets::Initialize();

	NetVars::Initialize();
	Fonts::Initialize();

	SkinParser::InitializeKits();
	GUI::Initialize();
	SkinChanger::Initialize();

	Hooks::Apply();
	//OverlayRender::Initialize();

#ifdef __cyandbg
	Utilities::VersionString += "_DEBUG";
#endif

#ifdef __bypass
	Utilities::VersionString += "_BYPASS";
#endif

	Utilities::Log("cyan " + Utilities::VersionString + " successfully injected");
	Utilities::csMessageBox("cyan " + Utilities::VersionString, "Injection successful.", true, false);

#ifdef __cyandbg
	while (!Utilities::unload)
		Sleep(1000);

	Utilities::Warn("Unloading cyan...");

	I::Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
	I::InputSystem->EnableInput(true);

	Hooks::Restore();
	GUI::RemoveHook();

	Sleep(2000);
	Utilities::Log("Successful.");
	FreeLibraryAndExitThread(Utilities::hModule, 0);
#endif

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		Utilities::hModule = instance;

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)init, NULL, NULL, NULL);
	}

	return TRUE;
}

// ----- TODO LIST -----

// CACHE CONTROL SAFE NAMES ON CONSTRUCTION
// AMBIENT SOUND HOOK
// DLIGHT menu controls
// thirdperson
// AUTODEFUSE
// color the event names!
// implement the triggerbot wait
// move all the hitmarkers settings into a combo box of presets

// MAYBE TO DO

// GUI groups open and close
// anti aims


