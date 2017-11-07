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

#include "autoaccept.h"

#include "interfaces.h"
#include "offsets.h"
#include "menu.h"

#include "settings.h"

// misc autoaccept config
bool Config::Misc::AutoAccept::Enable = true;
float Config::Misc::AutoAccept::WaitTime = 0.f;

struct CServerConfirmedReservationCheckCallback
{
	char pad[0x2200];
};

void AutoAccept::PlaySound_(const char* fileName)
{
	if (!Config::Misc::AutoAccept::Enable)
		return;

	if (I::Engine->IsInGame())
		return;

	if (std::string(fileName).find("accept_beep.wav") == std::string::npos)
		return;

	reinterpret_cast<IsReadyCallbackFn>(Offsets::IsReady)();
}
