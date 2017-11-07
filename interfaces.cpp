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
credit: teamgamerfood.com
*/

// #define __ifacedump

#include "interfaces.h"

#include "Path.h"
#include "offsets.h"
#include "utilities.h"

namespace I
{
	InterfaceManager ClientManager("client.dll");
	InterfaceManager EngineManager("engine.dll");
	InterfaceManager VGUI2Manager("vgui2.dll");
	InterfaceManager MaterialSurfaceManager("vguimatsurface.dll");
	InterfaceManager MaterialSystemManager("materialsystem.dll");
	InterfaceManager PhysicsManager("vphysics.dll");
	InterfaceManager STDLibManager("vstdlib.dll");
	InterfaceManager LocalizeManager("localize.dll");
	InterfaceManager InputSystemManager("inputsystem.dll");

	IBaseClientDLL* Client;
	IEngineClient* Engine;
	IVPanel* Panels;
	IClientEntityList* EntList;
	ISurface* Surface;
	IVDebugOverlay* DebugOverlay;
	DWORD* ClientMode;
	CGlobalVars* globalVars;
	IPrediction* Prediction;
	IMaterialSystem* MaterialSystem;
	CViewRender* RenderView;
	IVModelRender* ModelRender;
	IVModelInfo* ModelInfo;
	IEngineTrace* Trace;
	IPhysicsSurfaceProps* PhysProps;
	ICvar* CVar;
	IGameMovement* GameMovement;
	IGameEventManager2* GameEventManager;
	IEngineSound* EngineSound;
	IMoveHelper* MoveHelper;
	C_CSGameRules* csGameRules;
	CEffects* Effects;
	ILocalize* Localize;
	CBaseClientState* ClientState;
	IInputSystem* InputSystem;
	C_CSPlayerResource** PlayerResourcePtr;
	IWeaponSystem* WeaponSystem;
	CGlowObjectManager* GlowManager;
	CInput* Input;
	CGameUI* GameUI;

	void Initialize()
	{
#ifdef __ifacedump
		std::string path = Path::Get("iface.txt");

#pragma warning(disable:4996)
		freopen(path.c_str(), "w", stdout);
#pragma warning(default:4996)

		ClientManager.DumpAllInterfaces();
		EngineManager.DumpAllInterfaces();
		VGUI2Manager.DumpAllInterfaces();
		MaterialSurfaceManager.DumpAllInterfaces();
		MaterialSystemManager.DumpAllInterfaces();
		PhysicsManager.DumpAllInterfaces();
		STDLibManager.DumpAllInterfaces();
		LocalizeManager.DumpAllInterfaces();
		InputSystemManager.DumpAllInterfaces();
#endif

		char* GameEventsManagerName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "GAMEEVENTSMANAGER00");

		Client = ClientManager.GetInterface<IBaseClientDLL>("VClient0");
		Engine = EngineManager.GetInterface<IEngineClient>("VEngineClient0");
		Panels = VGUI2Manager.GetInterface<IVPanel>("VGUI_Panel0");
		Surface = MaterialSurfaceManager.GetInterface<ISurface>("VGUI_Surface0");
		EntList = ClientManager.GetInterface<IClientEntityList>("VClientEntityList0");
		DebugOverlay = EngineManager.GetInterface<IVDebugOverlay>("VDebugOverlay0");
		Prediction = ClientManager.GetInterface<IPrediction>("VClientPrediction0");
		MaterialSystem = MaterialSystemManager.GetInterface<IMaterialSystem>("VMaterialSystem0");
		RenderView = EngineManager.GetInterface<CViewRender>("VEngineRenderView0");
		ModelRender = EngineManager.GetInterface<IVModelRender>("VEngineModel0");
		ModelInfo = EngineManager.GetInterface<IVModelInfo>("VModelInfoClient0");
		Trace = EngineManager.GetInterface<IEngineTrace>("EngineTraceClient0");
		PhysProps = PhysicsManager.GetInterface<IPhysicsSurfaceProps>("VPhysicsSurfaceProps0");
		CVar = STDLibManager.GetInterface<ICvar>("VEngineCvar0");
		GameMovement = ClientManager.GetInterface<IGameMovement>("GameMovement");
		GameEventManager = EngineManager.GetInterface<IGameEventManager2>(GameEventsManagerName);
		EngineSound = EngineManager.GetInterface<IEngineSound>("IEngineSoundClient0");
		Effects = EngineManager.GetInterface<CEffects>("VEngineEffects0");
		Localize = LocalizeManager.GetInterface<ILocalize>("Localize_");
		InputSystem = InputSystemManager.GetInterface<IInputSystem>("InputSystemVersion");
		GameUI = ClientManager.GetInterface<CGameUI>("GameUI");

		ClientMode = **(DWORD***)((*(uintptr_t**)Client)[10] + 0x5);
		globalVars = **reinterpret_cast<CGlobalVars***>((*reinterpret_cast<uintptr_t**>(Client))[0] + 0x1B);
		ClientState = **reinterpret_cast<CBaseClientState***>(getvfunc<uintptr_t>(I::Engine, 12) + 0x10);
		csGameRules = reinterpret_cast<C_CSGameRules*>(Offsets::FindPattern("client.dll", "A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 0F 84 ? ? ? ? 0F 10 05") + 1);
		WeaponSystem = reinterpret_cast<IWeaponSystem*>(*(uintptr_t*)Offsets::FindPattern("client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0") + 2);
		GlowManager = reinterpret_cast<CGlowObjectManager*>(*(uintptr_t*)(Offsets::FindPattern(("client.dll"), ("0F 11 05 ?? ?? ?? ?? 83 C8 01 C7 05 ?? ?? ?? ?? 00 00 00 00")) + 3));
		PlayerResourcePtr = reinterpret_cast<C_CSPlayerResource**>(*(uintptr_t**)(Offsets::FindPattern("client.dll", "A1 ? ? ? ? 57 85 C0 74 08") + 1));

		Input = *(CInput**)((*(DWORD**)Client)[15] + 0x1);
	}

	void InterfaceManager::Setup(std::string strName)
	{
		strModuleName = strName;
		ModuleBase = (HMODULE)Utilities::Memory::WaitOnModuleHandle(strName.c_str());

		if (!ModuleBase)
		{
			Utilities::Error("Module " + strName + " could not be found.");
			return;
		}

		DWORD* pCreateInterfaceFn = (DWORD*)GetProcAddress(ModuleBase, "CreateInterface");

		// Step 1: Find the jump to the main block of code
		pCreateInterfaceFn = (DWORD*)((DWORD)pCreateInterfaceFn + 0x4); // Later on replace this +4 with a search for the first jmp(0xE9)
		int JmpConv = *(int*)((DWORD)pCreateInterfaceFn + 1); // + 1 to skip the opcode		
		pCreateInterfaceFn = (DWORD*)(5 + (DWORD)pCreateInterfaceFn + (DWORD)JmpConv); // Jmp is relative to the end of this line (hence the +5 bytes)

																					   // Step 2: Get the head of the list
																					   // MOV ESI,DWORD PTR DS:[1A69F904]
		pIntHead = *(InterfacesHead**)((DWORD)pCreateInterfaceFn + 0x6); // Later use a search instead
	}

	template <class T>
	T* InterfaceManager::GetInterface(std::string strName)
	{
		if (pIntHead)
			for (InterfaceNode* pNode = pIntHead->HeadNode; pNode; pNode = pNode->pNext)
				if (std::string(pNode->pName).find(strName) != std::string::npos)
					return reinterpret_cast<T*>(pNode->fncGet());

		Utilities::Error("Did not find interface: " + strName);
		return nullptr;
	}

	void InterfaceManager::DumpAllInterfaces()
	{
		printf("%-16s%-44s%10s\n", "Module", "Interface", "Instance");
		printf("----------------------------------------------------------------------\n");

		if (pIntHead)
			for (InterfaceNode* pNode = pIntHead->HeadNode; pNode; pNode = pNode->pNext)
				printf("%-16s%-44s%10x\n", strModuleName.c_str(), pNode->pName, pNode->fncGet());

		printf("----------------------------------------------------------------------\n");
	}
}