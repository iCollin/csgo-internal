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

#include "overrideview.h"

#include "menu.h"
#include "globals.h"

#include "settings.h"

// visuals override view config
bool Config::Visuals::OverrideView::CustomFOV = true;
bool Config::Visuals::OverrideView::CustomScopedFOV = false;
float Config::Visuals::OverrideView::minFOV = 60.f;
float Config::Visuals::OverrideView::FOV = 100.f;
float Config::Visuals::OverrideView::maxFOV = 160.f;
bool Config::Visuals::OverrideView::Thirdperson = false;
float Config::Visuals::OverrideView::ThirdpersonDistance = 150.f;
bool Config::Visuals::OverrideView::VisibleRecoil = false;
bool Config::Visuals::OverrideView::NoSmoke = false;
bool Config::Visuals::OverrideView::NoFlash = true;
float Config::Visuals::OverrideView::FlashMaxAlpha = 155.f;

void OverrideView::Hook(CViewSetup* view)
{
	if (!I::Engine->IsInGame())
		return;

	if (I::Engine->IsTakingScreenshot() || Config::Debug::StreamMode)
		return;

	if (Config::Visuals::OverrideView::CustomFOV)
		if (Config::Visuals::OverrideView::CustomScopedFOV || !G::LocalPlayer->IsScoped())
			view->fov += Config::Visuals::OverrideView::FOV - 68.f;
}