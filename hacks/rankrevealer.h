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

#ifndef RANKREVEALER_H
#define RANKREVEALER_H

#include "offsets.h"

namespace RankReveal
{
	void CreateMove()
	{
		if (I::Engine->IsTakingScreenshot() || Config::Debug::StreamMode)
			return;

		using MsgFunc_ServerRankRevealAllFn = bool(__cdecl*)(float*);

		float fArray[3] = { 0.f, 0.f, 0.f };

		reinterpret_cast<MsgFunc_ServerRankRevealAllFn>(Offsets::RankRevealAll)(fArray);
	}
}

#endif