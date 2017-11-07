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
credit: Synraw @ unknowncheats.me
*/

#pragma once

#ifndef CHAMS_H
#define CHAMS_H

#include "Interfaces.h"

typedef void(__thiscall* DrawModelExecuteFn)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4_t*);

namespace Chams
{
	//void ShutDown();

	void DrawModelExecute(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4_t*, DrawModelExecuteFn);
}

#endif // !CHAMS_H