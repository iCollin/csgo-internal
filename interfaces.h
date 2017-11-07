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
credit: Synraw @ unknowncheats.me
*/

#pragma once

#ifndef INTERFACES_H
#define INTERFACES_H

#include "IBaseClientDLL.h"
#include "IEngineClient.h"
#include "IVPanel.h"
#include "IClientEntityList.h"
#include "ISurface.h"
#include "IVDebugOverlay.h"
#include "CGlobalVars.h"
#include "IPrediction.h"
#include "IMaterialSystem.h"
#include "CViewRender.h"
#include "IVModelRender.h"
#include "IVModelInfo.h"
#include "IEngineTrace.h"
#include "IPhysicsSurfaceProps.h"
#include "ICvar.h"
#include "IGameMovement.h"
#include "IGameEvent.h"
#include "IEngineSound.h"
#include "CCSGameRules.h"
#include "CEffects.h"
#include "ILocalize.h"
#include "CBaseClientState.h"
#include "IInputSystem.h"
#include "CPlayerResource.h"
#include "CCSWeaponSystem.h"
#include "CGlowObjectManager.h"
#include "CInput.h"
#include "CGameUI.h"

namespace I
{
	void Initialize();

	extern IBaseClientDLL* Client;
	extern IEngineClient* Engine;
	extern IVPanel* Panels;
	extern IClientEntityList* EntList;
	extern ISurface* Surface;
	extern IVDebugOverlay* DebugOverlay;
	extern DWORD *ClientMode;
	extern CGlobalVars *globalVars;
	extern IPrediction *Prediction;
	extern IMaterialSystem* MaterialSystem;
	extern CViewRender* RenderView;
	extern IVModelRender* ModelRender;
	extern IVModelInfo* ModelInfo;
	extern IEngineTrace* Trace;
	extern IPhysicsSurfaceProps* PhysProps;
	extern ICvar* CVar;
	extern IGameMovement *GameMovement;
	extern IGameEventManager2 *GameEventManager;
	extern IEngineSound *EngineSound;
	extern IMoveHelper* MoveHelper;
	extern C_CSGameRules* csGameRules;
	extern CEffects* Effects;
	extern ILocalize* Localize;
	extern CBaseClientState* ClientState;
	extern IInputSystem* InputSystem;
	extern C_CSPlayerResource** PlayerResourcePtr;
	extern IWeaponSystem* WeaponSystem;
	extern CGlowObjectManager* GlowManager;
	extern CInput* Input;
	extern CGameUI* GameUI;

	class InterfacesHead;
	class InterfaceNode;

	class InterfacesHead
	{
	public:
		InterfaceNode* HeadNode; //0x0000 
	};//Size=0x0040

	typedef int(*oCapture)(void);

	class InterfaceNode
	{
	public:
		oCapture fncGet; //0x0000 
		char* pName; //0x0004 
		InterfaceNode* pNext; //0x0008 
	};//Size=0x001C

	class InterfaceManager
	{
	public:
		InterfaceManager(std::string strModule) : ModuleBase(0), pIntHead(0) { Setup(strModule); }
		InterfaceManager() : ModuleBase(0), pIntHead(0) {}

		void Setup(std::string strModule);
		template <class T>
		T* GetInterface(std::string strName);
		void DumpAllInterfaces();

	private:
		std::string strModuleName;
		HMODULE ModuleBase;
		InterfacesHead* pIntHead;
	};
};

#endif