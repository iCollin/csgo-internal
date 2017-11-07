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

#ifndef HOOKS_H
#define HOOKS_H

#include "utilities.h"

namespace Hooks
{
	void Apply();
	void Restore();

	extern Utilities::Memory::VMTManager* panelVMT;
	extern Utilities::Memory::VMTManager* clientVMT;
	extern Utilities::Memory::VMTManager* engineVMT;
	extern Utilities::Memory::VMTManager* clientModeVMT;
	extern Utilities::Memory::VMTManager* modelRenderVMT;
	extern Utilities::Memory::VMTManager* predictionVMT;
	extern Utilities::Memory::VMTManager* surfaceVMT;
	extern Utilities::Memory::VMTManager* soundVMT;
	extern Utilities::Memory::VMTManager* MaterialSystem;
}

#endif // !HOOKS_H