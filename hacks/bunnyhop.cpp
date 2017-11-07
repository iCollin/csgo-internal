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

#include "bunnyhop.h"

#include "menu.h"
#include "globals.h"

#include "settings.h"

// misc bunnyhop config
bool Config::Misc::Bunnyhop::Enable = true;
bool Config::Misc::Bunnyhop::SMACSafe = false;
bool Config::Misc::Bunnyhop::AutoStrafe = true;

void Bunnyhop::CreateMove()
{
	if (!Config::Misc::Bunnyhop::Enable)
		return;

	if (!(G::UserCmd->buttons & IN_JUMP))
		return;

	if (!G::LocalPlayer->GetAlive())
		return;

	if (!(G::LocalPlayer->GetFlags() & FL_ONGROUND))
		G::UserCmd->buttons &= ~IN_JUMP;

	if (!Config::Misc::Bunnyhop::AutoStrafe)
		return;

	if (G::UserCmd->mousedx < 0)
		G::UserCmd->sidemove = -415.f;
	else if (G::UserCmd->mousedx > 0)
		G::UserCmd->sidemove = 415.f;
}
