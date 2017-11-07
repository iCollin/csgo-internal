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

#include "autodefuse.h"

#include "menu.h"
#include "globals.h"

#include "settings.h"

// misc autodefuse config
bool Config::Misc::AutoDefuse::Enable = true;
bool Config::Misc::AutoDefuse::AimAtBomb = true;
float Config::Misc::AutoDefuse::ExtraTime = 0.5f;

void AutoDefuse::CreateMove()
{
	if (!Config::Misc::AutoDefuse::Enable)
		return;

	if (!G::LocalPlayer->GetAlive())
		return;

	C_PlantedC4* bomb = nullptr;

	for (int i = 1; i < I::EntList->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity* pEntity = I::EntList->GetClientEntity(i);
		if (!pEntity)
			return;

		if (pEntity->GetClientClass()->m_ClassID == EClassIds::CPlantedC4)
		{
			bomb = reinterpret_cast<C_PlantedC4*>(pEntity);
			break;
		}
	}

	if (!bomb || bomb->IsBombDefused())
		return;

	float bombTimer = bomb->GetBombTime() - I::globalVars->curtime;

	float bufferTime = Config::Misc::AutoDefuse::ExtraTime;
	bufferTime += G::LocalPlayer->HasDefuser() ? 5.1f : 10.1f;

	if (bombTimer > bufferTime)
		return;

	float distance = G::LocalPlayer->GetVecOrigin().DistTo(bomb->GetVecOrigin());
	if (distance < 75.f)
		G::UserCmd->buttons |= IN_USE;
}
