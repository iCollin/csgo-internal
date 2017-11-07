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

#ifndef ESP_H
#define ESP_H

#include "interfaces.h"

struct NadeInfo_t
{
	Color color;
	std::string name;
};

namespace ESP
{
	void EmitSound(int, const char*, float);
	void PaintTraverse();

	std::string FriendlyWeaponName(int index);
	NadeInfo_t GetNadeInfo(C_BaseEntity* nade);

	extern int bombCarrierIndex;
}

#endif // !ESP_H