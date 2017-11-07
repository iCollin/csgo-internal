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

#include "hooks.h"

#include "menu.h"
#include "globals.h"
#include "definitions.h"

#include "GUI.h"
#include "hacks.h"

// function indicies
#define PAINT_TRAVERSE_INDEX 41
#define FRAME_STAGE_NOTIFY_INDEX 36
#define CREATE_MOVE_CLIENT_INDEX 24
#define OVERRIDE_VIEW_INDEX 18
#define DRAW_MODEL_EXECUTE_INDEX 21
#define RUN_COMMAND_INDEX 19
#define PLAY_SOUND_INDEX 82
#define GET_PLAYER_INFO_INDEX 8
#define EMIT_SOUND_INDEX 5

// our VMT managers for each interface
Utilities::Memory::VMTManager* Hooks::panelVMT = new Utilities::Memory::VMTManager;
Utilities::Memory::VMTManager* Hooks::clientVMT = new Utilities::Memory::VMTManager;
Utilities::Memory::VMTManager* Hooks::engineVMT = new Utilities::Memory::VMTManager;
Utilities::Memory::VMTManager* Hooks::clientModeVMT = new Utilities::Memory::VMTManager;
Utilities::Memory::VMTManager* Hooks::modelRenderVMT = new Utilities::Memory::VMTManager;
Utilities::Memory::VMTManager* Hooks::predictionVMT = new Utilities::Memory::VMTManager;
Utilities::Memory::VMTManager* Hooks::surfaceVMT = new Utilities::Memory::VMTManager;
Utilities::Memory::VMTManager* Hooks::soundVMT = new Utilities::Memory::VMTManager;
Utilities::Memory::VMTManager* Hooks::MaterialSystem = new Utilities::Memory::VMTManager;

// settings init which shouldn't really be here
bool Config::Misc::Stream::HideNames = true;

// typedefs of functions we need to replace
typedef void(__thiscall* DrawModelExecuteFn)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4_t*);
typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
typedef bool(__thiscall* InPredictionFn)(PVOID);
typedef bool(__thiscall* GetPlayerInfoFn)(void*, int, IEngineClient::player_info_t*);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
typedef void(__thiscall* RunCommandFn)(PVOID, IClientEntity*, CUserCmd*, IMoveHelper*);
typedef void(__stdcall *OverrideViewFn)(CViewSetup*);
typedef void(__thiscall* PlaySoundFn)(ISurface*, const char*);
typedef void(__fastcall* EmitSoundFn)(void*, void*, void*, int, int, const char *, unsigned int, const char *, float, float, int, int, int, const Vector *, const Vector *, Vector *, bool, float, int);
typedef float(__stdcall* GetViewModelFOVFn)(void);

// pointers to original functions so we can pass thru
DrawModelExecuteFn oDrawModelExecute;
PaintTraverseFn oPaintTraverse;
InPredictionFn oInPrediction;
FrameStageNotifyFn oFrameStageNotify;
RunCommandFn oRunCommand;
OverrideViewFn oOverrideView;
PlaySoundFn oPlaySound;
EmitSoundFn oEmitSound;
GetViewModelFOVFn oGetViewModelFOV;

// the functions we will replace game functions with
void __fastcall PaintTraverseHooked(void* pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
void __stdcall FrameStageNotifyHooked(ClientFrameStage_t curStage);
bool __stdcall CreateMoveClientHooked(float frametime, CUserCmd* pCmd);
void __stdcall OverrideViewHooked(CViewSetup* vsView);
void __fastcall DrawModelExecuteHooked(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
void __stdcall RunCommandHooked(IClientEntity* pPlayer, CUserCmd* pCmd, IMoveHelper* pMoveHelper);
void __stdcall PlaySoundHooked(const char* filename);
void __fastcall EmitSoundHooked(void* ecx, void* edx, void* filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vector *pOrigin, const Vector *pDirection, Vector * pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity);
namespace MatSysConfig { class MaterialSystem_Config_t; }
bool __fastcall OverrideConfigHooked(IMaterialSystem* this0, void* ecx, MatSysConfig::MaterialSystem_Config_t* config, bool forceUpdate);
float __stdcall GetViewModelFOVHooked();

void Hooks::Apply()
{
	panelVMT->Initialize((uintptr_t*)I::Panels);
	panelVMT->HookMethod((uintptr_t)&PaintTraverseHooked, PAINT_TRAVERSE_INDEX);
	oPaintTraverse = (PaintTraverseFn)panelVMT->GetOriginalFunction(PAINT_TRAVERSE_INDEX);

	clientVMT->Initialize((uintptr_t*)I::Client);
	clientVMT->HookMethod((uintptr_t)&FrameStageNotifyHooked, FRAME_STAGE_NOTIFY_INDEX);
	oFrameStageNotify = (FrameStageNotifyFn)clientVMT->GetOriginalFunction(FRAME_STAGE_NOTIFY_INDEX);

	//engineVMT->Initialize((uintptr_t*)I::Engine);
	//engineVMT->HookMethod((uintptr_t)&GetPlayerInfoHooked, GET_PLAYER_INFO_INDEX);

	clientModeVMT->Initialize((uintptr_t*)I::ClientMode);
	clientModeVMT->HookMethod((uintptr_t)&CreateMoveClientHooked, CREATE_MOVE_CLIENT_INDEX);
	clientModeVMT->HookMethod((uintptr_t)&OverrideViewHooked, OVERRIDE_VIEW_INDEX);
	oGetViewModelFOV = (GetViewModelFOVFn)clientModeVMT->HookMethod((uintptr_t)&GetViewModelFOVHooked, 35);
	oOverrideView = (OverrideViewFn)clientModeVMT->GetOriginalFunction(OVERRIDE_VIEW_INDEX);

	modelRenderVMT->Initialize((uintptr_t*)I::ModelRender);
	modelRenderVMT->HookMethod((uintptr_t)&DrawModelExecuteHooked, DRAW_MODEL_EXECUTE_INDEX);
	oDrawModelExecute = (DrawModelExecuteFn)modelRenderVMT->GetOriginalFunction(DRAW_MODEL_EXECUTE_INDEX);

	predictionVMT->Initialize((uintptr_t*)I::Prediction);
	predictionVMT->HookMethod((uintptr_t)&RunCommandHooked, RUN_COMMAND_INDEX);
	oRunCommand = (RunCommandFn)predictionVMT->GetOriginalFunction(RUN_COMMAND_INDEX);

	surfaceVMT->Initialize((uintptr_t*)I::Surface);
	surfaceVMT->HookMethod((uintptr_t)&PlaySoundHooked, PLAY_SOUND_INDEX);
	oPlaySound = (PlaySoundFn)surfaceVMT->GetOriginalFunction(PLAY_SOUND_INDEX);

	soundVMT->Initialize((uintptr_t*)I::EngineSound);
	soundVMT->HookMethod((uintptr_t)&EmitSoundHooked, EMIT_SOUND_INDEX);
	oEmitSound = (EmitSoundFn)soundVMT->GetOriginalFunction(EMIT_SOUND_INDEX);

	MaterialSystem->Initialize((uintptr_t*)I::MaterialSystem);
	MaterialSystem->HookMethod((uintptr_t)&OverrideConfigHooked, 21);
}

void Hooks::Restore()
{
	panelVMT->RestoreOriginal();
	clientVMT->RestoreOriginal();
	clientModeVMT->RestoreOriginal();
	modelRenderVMT->RestoreOriginal();
	predictionVMT->RestoreOriginal();
	surfaceVMT->RestoreOriginal();
	soundVMT->RestoreOriginal();
}

#pragma region GetViewModelFOV Helpers

bool Config::Misc::ViewModelFOV::Enable = true;
float Config::Misc::ViewModelFOV::Modifier = 40.f;

#pragma endregion Helper functions used by GetViewModelFOVHooked

float __stdcall GetViewModelFOVHooked()
{
	return Config::Misc::ViewModelFOV::Enable ? oGetViewModelFOV() : oGetViewModelFOV() + Config::Misc::ViewModelFOV::Modifier;
}

bool __fastcall OverrideConfigHooked(IMaterialSystem* this0, void* ecx, MatSysConfig::MaterialSystem_Config_t* config, bool forceUpdate)
{
	if (I::Engine->IsInGame() && config) MatSysConfig::Hook(config);

	return reinterpret_cast<bool(__fastcall*)(IMaterialSystem*, void*, MatSysConfig::MaterialSystem_Config_t*, bool)>(Hooks::MaterialSystem->GetOriginalFunction(21))(this0, ecx, config, forceUpdate);
}

#pragma region Create Move Helpers

// TODO clean up. move out of hooks.cpp
BYTE moveData[0x200];

void Prediction()
{
	if (I::MoveHelper && Config::Aimbot::Prediction && G::LocalPlayer->GetAlive())
	{
		float curtime = I::globalVars->curtime;
		float frametime = I::globalVars->frametime;
		int iFlags = G::LocalPlayer->GetFlags();

		I::globalVars->curtime = (float)G::LocalPlayer->GetTickBase() * I::globalVars->interval_per_tick;
		I::globalVars->frametime = I::globalVars->interval_per_tick;

		I::MoveHelper->SetHost(G::LocalPlayer);

		I::Prediction->SetupMove(G::LocalPlayer, G::UserCmd, I::MoveHelper, moveData);
		I::GameMovement->ProcessMovement(G::LocalPlayer, moveData);
		I::Prediction->FinishMove(G::LocalPlayer, G::UserCmd, moveData);

		I::MoveHelper->SetHost(0);

		I::globalVars->curtime = curtime;
		I::globalVars->frametime = frametime;
		G::LocalPlayer->SetFlags(iFlags);
	}
}

#pragma endregion Functions called from within Create Move

void __fastcall PaintTraverseHooked(void* pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;

	if (!FoundPanel)
	{
		const char* panelName = I::Panels->GetName(vguiPanel);

		if (strstr(panelName, "MatSystemTopPanel"))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
		else
			return;
	}

	if (FocusOverlayPanel != vguiPanel)
		return;

	// must be called from game thread
	GUI::UpdateHook();

	if (I::Engine->IsTakingScreenshot() || Config::Debug::StreamMode)
		return;

	if (I::Engine->IsInGame())
	{
		ESP::PaintTraverse();
		GrenadePrediction::PaintTraverse();
		NadeHelper::PaintTraverse();
		SpectatorList::PaintTraverse();
	}

	GUI::PaintTraverse();
}

#pragma region FSN HELPERS

void NoVisualRecoil()
{
	if (G::CurStage == ClientFrameStage_t::FRAME_RENDER_START && !Config::Visuals::OverrideView::VisibleRecoil && G::LocalPlayer && G::LocalPlayer->GetAlive())
	{
		QAngle* pAimPunch = G::LocalPlayer->GetAimPunchAngle();
		QAngle* pViewPunch = G::LocalPlayer->GetViewPunchAngle();

		QAngle vecAimPunch = *pAimPunch;
		QAngle vecViewPunch = *pViewPunch;

		*pAimPunch = QAngle(0, 0, 0);
		*pViewPunch = QAngle(0, 0, 0);

		oFrameStageNotify(G::CurStage); // WEAPON SCRIPT FILE ACCESSED ASSERT

		*pAimPunch += vecAimPunch;
		*pViewPunch += vecViewPunch;
	}
	else
	{
		oFrameStageNotify(G::CurStage);
	}
}

#pragma endregion Functions called from the FrameStageNotify hook.

void __stdcall FrameStageNotifyHooked(ClientFrameStage_t stage)
{
	G::CurStage = stage;

	if (!G::LocalPlayer) return oFrameStageNotify(stage);

	if (I::Engine->IsTakingScreenshot() || Config::Debug::StreamMode)
		return oFrameStageNotify(stage);

	ThirdPerson::FrameStageNotify();
	NoVisualRecoil();
	Glow::FrameStageNotify();
	NoFlash::FrameStageNotify();
	SkinChanger::FrameStageNotify();
	GloveChanger::FrameStageNotify();  // SCRIPT FILE CRASH
	//PVSfix::FrameStageNotify(stage); // TODO update offsets
	NoSmoke::FrameStageNotify();
}

bool __stdcall CreateMoveClientHooked(float frametime, CUserCmd* pCmd)
{
	G::UserCmd = pCmd;
	G::LocalPlayer = I::EntList->GetClientEntity(I::Engine->GetLocalPlayer());

	if (!pCmd->command_number || !G::LocalPlayer)
		return reinterpret_cast<bool(__stdcall*)(float, CUserCmd*)>(Hooks::clientModeVMT->GetOriginalFunction(CREATE_MOVE_CLIENT_INDEX))(frametime, pCmd);

	G::ActiveWeapon = reinterpret_cast<C_BaseCombatWeapon*>(I::EntList->GetClientEntityFromHandle(G::LocalPlayer->GetActiveWeapon()));

	if (G::ActiveWeapon) G::ActiveWeaponInfo = G::ActiveWeapon->GetCSWpnData();

	RankReveal::CreateMove();
	Prediction();
	GrenadePrediction::CreateMove();
	NadeHelper::CreateMove();
	Bunnyhop::CreateMove();
	Aimbot::CreateMove();
	Triggerbot::CreateMove();

	pCmd->viewangles.Trust();

	// fake angles when sending packet, real angles when choking
	return false;
}

void __stdcall OverrideViewHooked(CViewSetup* vsView)
{
	if (G::LocalPlayer && vsView)
	{
		OverrideView::Hook(vsView);
		
	}

	oOverrideView(vsView);
}

void __fastcall DrawModelExecuteHooked(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	if (G::LocalPlayer) Chams::DrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld, oDrawModelExecute);

	oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	I::ModelRender->ForcedMaterialOverride(NULL);
}

void __stdcall RunCommandHooked(IClientEntity* pPlayer, CUserCmd* pCmd, IMoveHelper* pMoveHelper)
{
	oRunCommand(I::Prediction, pPlayer, pCmd, pMoveHelper);
	I::MoveHelper = pMoveHelper;
}

void __stdcall PlaySoundHooked(const char* filename)
{
	oPlaySound(I::Surface, filename);

	AutoAccept::PlaySound_(filename);
}

void __fastcall EmitSoundHooked(void* ecx, void* edx, void* filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vector *pOrigin, const Vector *pDirection, Vector * pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity)
{
	ESP::EmitSound(iEntIndex, pSample, flVolume);

	oEmitSound(ecx, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity);
}
