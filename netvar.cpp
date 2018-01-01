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

#include "Path.h"
#include "netvar.h"

#include "interfaces.h"

#include <sstream>
#include <fstream>

#define ALIGN_SIZE 8

// comment if you don't want a netvar dump done at startup (MASTER_DIRECTORY \ netvars.txt)
// #define DUMP_NETVARS

void NetVars::Initialize()
{
	std::vector<RecvTable *> tables = NetVarManager::GetTables();

#ifdef DUMP_NETVARS
	NetVarManager::DumpNetvars();
#endif

	NetVars::DT_BasePlayer::m_Local = NetVarManager::GetOffset(tables, "DT_BasePlayer", "m_Local");
	NetVars::DT_BasePlayer::m_aimPunchAngle = NetVarManager::GetOffset(tables, "DT_BasePlayer", "m_aimPunchAngle");
	NetVars::DT_BasePlayer::m_viewPunchAngle = NetVarManager::GetOffset(tables, "DT_BasePlayer", "m_viewPunchAngle");
	NetVars::DT_BasePlayer::m_vecViewOffset = NetVarManager::GetOffset(tables, "DT_BasePlayer", "m_vecViewOffset[0]");
	NetVars::DT_BasePlayer::m_nTickBase = NetVarManager::GetOffset(tables, "DT_BasePlayer", "m_nTickBase");
	NetVars::DT_BasePlayer::m_vecVelocity = NetVarManager::GetOffset(tables, "DT_BasePlayer", "m_vecVelocity[0]");
	NetVars::DT_BasePlayer::m_iHealth = NetVarManager::GetOffset(tables, "DT_BasePlayer", "m_iHealth");
	NetVars::DT_BasePlayer::m_lifeState = NetVarManager::GetOffset(tables, "DT_BasePlayer", "m_lifeState");
	NetVars::DT_BasePlayer::m_fFlags = NetVarManager::GetOffset(tables, "DT_BasePlayer", "m_fFlags");
	NetVars::DT_BasePlayer::m_iObserverMode = NetVarManager::GetOffset(tables, "DT_BasePlayer", "m_iObserverMode");
	NetVars::DT_BasePlayer::m_hObserverTarget = NetVarManager::GetOffset(tables, "DT_BasePlayer", "m_hObserverTarget");
	NetVars::DT_BasePlayer::m_hViewModel = NetVarManager::GetOffset(tables, "DT_BasePlayer", "m_hViewModel[0]");
	NetVars::DT_BasePlayer::m_szLastPlaceName = NetVarManager::GetOffset(tables, "DT_BasePlayer", "m_szLastPlaceName");
	NetVars::DT_BasePlayer::deadflag = NetVarManager::GetOffset(tables, "DT_BasePlayer", "deadflag");

	NetVars::DT_BaseEntity::m_flAnimTime = NetVarManager::GetOffset(tables, "DT_BaseEntity", "m_flAnimTime");
	NetVars::DT_BaseEntity::m_flSimulationTime = NetVarManager::GetOffset(tables, "DT_BaseEntity", "m_flSimulationTime");
	NetVars::DT_BaseEntity::m_vecOrigin = NetVarManager::GetOffset(tables, "DT_BaseEntity", "m_vecOrigin");
	NetVars::DT_BaseEntity::m_nRenderMode = NetVarManager::GetOffset(tables, "DT_BaseEntity", "m_nRenderMode");
	NetVars::DT_BaseEntity::m_iTeamNum = NetVarManager::GetOffset(tables, "DT_BaseEntity", "m_iTeamNum");
	NetVars::DT_BaseEntity::m_MoveType = NetVars::DT_BaseEntity::m_nRenderMode + 1;
	NetVars::DT_BaseEntity::m_Collision = NetVarManager::GetOffset(tables, "DT_BaseEntity", "m_Collision");
	NetVars::DT_BaseEntity::m_bSpotted = NetVarManager::GetOffset(tables, "DT_BaseEntity", "m_bSpotted");

	NetVars::DT_BaseCombatCharacter::m_hActiveWeapon = NetVarManager::GetOffset(tables, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	NetVars::DT_BaseCombatCharacter::m_hMyWeapons = NetVarManager::GetOffset(tables, "DT_BaseCombatCharacter", "m_hMyWeapons") / 2;
	NetVars::DT_BaseCombatCharacter::m_hMyWearables = NetVarManager::GetOffset(tables, "DT_BaseCombatCharacter", "m_hMyWearables");

	NetVars::DT_PlayerResource::m_iPing = NetVarManager::GetOffset(tables, "DT_PlayerResource", "m_iPing");
	NetVars::DT_PlayerResource::m_iKills = NetVarManager::GetOffset(tables, "DT_PlayerResource", "m_iKills");
	NetVars::DT_PlayerResource::m_iAssists = NetVarManager::GetOffset(tables, "DT_PlayerResource", "m_iAssists");
	NetVars::DT_PlayerResource::m_iDeaths = NetVarManager::GetOffset(tables, "DT_PlayerResource", "m_iDeaths");
	NetVars::DT_PlayerResource::m_bConnected = NetVarManager::GetOffset(tables, "DT_PlayerResource", "m_bConnected");
	NetVars::DT_PlayerResource::m_iTeam = NetVarManager::GetOffset(tables, "DT_PlayerResource", "m_iTeam");
	NetVars::DT_PlayerResource::m_iPendingTeam = NetVarManager::GetOffset(tables, "DT_PlayerResource", "m_iPendingTeam");
	NetVars::DT_PlayerResource::m_bAlive = NetVarManager::GetOffset(tables, "DT_PlayerResource", "m_bAlive");
	NetVars::DT_PlayerResource::m_iHealth = NetVarManager::GetOffset(tables, "DT_PlayerResource", "m_iHealth");

	NetVars::DT_CSPlayerResource::m_iPlayerC4 = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iPlayerC4");
	NetVars::DT_CSPlayerResource::m_iPlayerVIP = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iPlayerVIP");
	NetVars::DT_CSPlayerResource::m_vecC4 = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_vecC4");
	NetVars::DT_CSPlayerResource::m_bombsiteCenterA = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_bombsiteCenterA");
	NetVars::DT_CSPlayerResource::m_bombsiteCenterB = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_bombsiteCenterB");
	NetVars::DT_CSPlayerResource::m_bHostageAlive = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_bHostageAlive");
	NetVars::DT_CSPlayerResource::m_isHostageFollowingSomeone = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_isHostageFollowingSomeone");
	NetVars::DT_CSPlayerResource::m_iHostageEntityIDs = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iHostageEntityIDs");
	NetVars::DT_CSPlayerResource::m_bombsiteCenterB = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_bombsiteCenterB");
	NetVars::DT_CSPlayerResource::m_hostageRescueX = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_hostageRescueX");
	NetVars::DT_CSPlayerResource::m_hostageRescueY = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_hostageRescueY");
	NetVars::DT_CSPlayerResource::m_hostageRescueZ = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_hostageRescueZ");
	NetVars::DT_CSPlayerResource::m_iMVPs = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iMVPs");
	NetVars::DT_CSPlayerResource::m_iArmor = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iArmor");
	NetVars::DT_CSPlayerResource::m_bHasHelmet = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_bHasHelmet");
	NetVars::DT_CSPlayerResource::m_bHasDefuser = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_bHasDefuser");
	NetVars::DT_CSPlayerResource::m_iScore = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iScore");
	NetVars::DT_CSPlayerResource::m_iCompetitiveRanking = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iCompetitiveRanking");
	NetVars::DT_CSPlayerResource::m_iCompetitiveWins = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iCompetitiveWins");
	NetVars::DT_CSPlayerResource::m_iCompTeammateColor = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iCompTeammateColor");
	NetVars::DT_CSPlayerResource::m_bControllingBot = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_bControllingBot");
	NetVars::DT_CSPlayerResource::m_iControlledPlayer = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iControlledPlayer");
	NetVars::DT_CSPlayerResource::m_iControlledByPlayer = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iControlledByPlayer");
	NetVars::DT_CSPlayerResource::m_iBotDifficulty = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iBotDifficulty");
	NetVars::DT_CSPlayerResource::m_szClan = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_szClan");
	NetVars::DT_CSPlayerResource::m_iTotalCashSpent = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iTotalCashSpent");
	NetVars::DT_CSPlayerResource::m_iCashSpentThisRound = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_iCashSpentThisRound");
	NetVars::DT_CSPlayerResource::m_nEndMatchNextMapVotes = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_nEndMatchNextMapVotes");
	NetVars::DT_CSPlayerResource::m_bEndMatchNextMapAllVoted = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_bEndMatchNextMapAllVoted");
	NetVars::DT_CSPlayerResource::m_nActiveCoinRank = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_nActiveCoinRank");
	NetVars::DT_CSPlayerResource::m_nMusicID = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_nMusicID");
	NetVars::DT_CSPlayerResource::m_nPersonaDataPublicLevel = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_nPersonaDataPublicLevel");
	NetVars::DT_CSPlayerResource::m_nPersonaDataPublicCommendsLeader = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_nPersonaDataPublicCommendsLeader");
	NetVars::DT_CSPlayerResource::m_nPersonaDataPublicCommendsTeacher = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_nPersonaDataPublicCommendsTeacher");
	NetVars::DT_CSPlayerResource::m_nPersonaDataPublicCommendsFriendly = NetVarManager::GetOffset(tables, "DT_CSPlayerResource", "m_nPersonaDataPublicCommendsFriendly");

	NetVars::DT_PlantedC4::m_bBombTicking = NetVarManager::GetOffset(tables, "DT_PlantedC4", "m_bBombTicking");
	NetVars::DT_PlantedC4::m_flC4Blow = NetVarManager::GetOffset(tables, "DT_PlantedC4", "m_flC4Blow");
	NetVars::DT_PlantedC4::m_bBombDefused = NetVarManager::GetOffset(tables, "DT_PlantedC4", "m_bBombDefused");
	NetVars::DT_PlantedC4::m_hBombDefuser = NetVarManager::GetOffset(tables, "DT_PlantedC4", "m_hBombDefuser");
	NetVars::DT_PlantedC4::m_flDefuseCountDown = NetVarManager::GetOffset(tables, "DT_PlantedC4", "m_flDefuseCountDown");

	NetVars::DT_CSPlayer::m_iShotsFired = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_iShotsFired");
	NetVars::DT_CSPlayer::m_angEyeAngles[0] = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_angEyeAngles[0]");
	NetVars::DT_CSPlayer::m_angEyeAngles[1] = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_angEyeAngles[1]");
	NetVars::DT_CSPlayer::m_iAccount = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_iAccount");
	NetVars::DT_CSPlayer::m_totalHitsOnServer = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_totalHitsOnServer");
	NetVars::DT_CSPlayer::m_ArmorValue = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_ArmorValue");
	NetVars::DT_CSPlayer::m_bIsDefusing = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_bIsDefusing");
	NetVars::DT_CSPlayer::m_bIsGrabbingHostage = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_bIsGrabbingHostage");
	NetVars::DT_CSPlayer::m_bIsScoped = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_bIsScoped");
	NetVars::DT_CSPlayer::m_bGunGameImmunity = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_bGunGameImmunity");
	NetVars::DT_CSPlayer::m_bIsRescuing = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_bIsRescuing");
	NetVars::DT_CSPlayer::m_bHasHelmet = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_bHasHelmet");
	NetVars::DT_CSPlayer::m_bHasDefuser = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_bHasDefuser");
	NetVars::DT_CSPlayer::m_flFlashDuration = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_flFlashDuration");
	NetVars::DT_CSPlayer::m_flFlashMaxAlpha = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_flFlashMaxAlpha");
	NetVars::DT_CSPlayer::m_flLowerBodyYawTarget = NetVarManager::GetOffset(tables, "DT_CSPlayer", "m_flLowerBodyYawTarget");

	NetVars::DT_BaseAttributableItem::m_iItemDefinitionIndex = NetVarManager::GetOffset(tables, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	NetVars::DT_BaseAttributableItem::m_iItemIDHigh = NetVarManager::GetOffset(tables, "DT_BaseAttributableItem", "m_iItemIDHigh");
	NetVars::DT_BaseAttributableItem::m_iAccountID = NetVarManager::GetOffset(tables, "DT_BaseAttributableItem", "m_iAccountID");
	NetVars::DT_BaseAttributableItem::m_iEntityQuality = NetVarManager::GetOffset(tables, "DT_BaseAttributableItem", "m_iEntityQuality");
	NetVars::DT_BaseAttributableItem::m_szCustomName = NetVarManager::GetOffset(tables, "DT_BaseAttributableItem", "m_szCustomName");
	NetVars::DT_BaseAttributableItem::m_nFallbackPaintKit = NetVarManager::GetOffset(tables, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
	NetVars::DT_BaseAttributableItem::m_nFallbackSeed = NetVarManager::GetOffset(tables, "DT_BaseAttributableItem", "m_nFallbackSeed");
	NetVars::DT_BaseAttributableItem::m_flFallbackWear = NetVarManager::GetOffset(tables, "DT_BaseAttributableItem", "m_flFallbackWear");
	NetVars::DT_BaseAttributableItem::m_nFallbackStatTrak = NetVarManager::GetOffset(tables, "DT_BaseAttributableItem", "m_nFallbackStatTrak");

	NetVars::DT_BaseViewModel::m_nModelIndex = NetVarManager::GetOffset(tables, "DT_BaseViewModel", "m_nModelIndex");
	NetVars::DT_BaseViewModel::m_hWeapon = NetVarManager::GetOffset(tables, "DT_BaseViewModel", "m_hWeapon");
	NetVars::DT_BaseViewModel::m_hOwner = NetVarManager::GetOffset(tables, "DT_BaseViewModel", "m_hOwner");

	NetVars::DT_WeaponCSBase::m_fAccuracyPenalty = NetVarManager::GetOffset(tables, "DT_WeaponCSBase", "m_fAccuracyPenalty");

	NetVars::DT_WeaponC4::m_bStartedArming = NetVarManager::GetOffset(tables, "DT_WeaponC4", "m_bStartedArming");

	NetVars::DT_BaseCombatWeapon::m_flNextPrimaryAttack = NetVarManager::GetOffset(tables, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	NetVars::DT_BaseCombatWeapon::m_hOwner = NetVarManager::GetOffset(tables, "DT_BaseCombatWeapon", "m_hOwner");
	NetVars::DT_BaseCombatWeapon::m_iClip1 = NetVarManager::GetOffset(tables, "DT_BaseCombatWeapon", "m_iClip1");
	NetVars::DT_BaseCombatWeapon::m_bInReload = NetVars::DT_BaseCombatWeapon::m_flNextPrimaryAttack + 113;
	NetVars::DT_BaseCombatWeapon::m_iPrimaryReserveAmmoCount = NetVarManager::GetOffset(tables, "DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount");

	NetVars::DT_BaseCSGrenade::m_bRedraw = NetVarManager::GetOffset(tables, "DT_BaseCSGrenade", "m_bRedraw");
	NetVars::DT_BaseCSGrenade::m_bIsHeldByPlayer = NetVarManager::GetOffset(tables, "DT_BaseCSGrenade", "m_bIsHeldByPlayer");
	NetVars::DT_BaseCSGrenade::m_bPinPulled = NetVarManager::GetOffset(tables, "DT_BaseCSGrenade", "m_bPinPulled");
	NetVars::DT_BaseCSGrenade::m_fThrowTime = NetVarManager::GetOffset(tables, "DT_BaseCSGrenade", "m_fThrowTime");
	NetVars::DT_BaseCSGrenade::m_bLoopingSoundPlaying = NetVarManager::GetOffset(tables, "DT_BaseCSGrenade", "m_bLoopingSoundPlaying");
	NetVars::DT_BaseCSGrenade::m_flThrowStrength = NetVarManager::GetOffset(tables, "DT_BaseCSGrenade", "m_flThrowStrength");

	NetVars::DT_DynamicProp::m_bShouldGlow = NetVarManager::GetOffset(tables, "DT_DynamicProp", "m_bShouldGlow");

	NetVars::DT_CSGameRulesProxy::m_bBombPlanted = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bBombPlanted");
	NetVars::DT_CSGameRulesProxy::m_bIsValveDS = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bIsValveDS");

	NetVars::DT_CSGameRulesProxy::m_bFreezePeriod = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bFreezePeriod");
	NetVars::DT_CSGameRulesProxy::m_bMatchWaitingForResume = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bMatchWaitingForResume");
	NetVars::DT_CSGameRulesProxy::m_bWarmupPeriod = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bWarmupPeriod");
	NetVars::DT_CSGameRulesProxy::m_fWarmupPeriodEnd = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_fWarmupPeriodEnd");
	NetVars::DT_CSGameRulesProxy::m_fWarmupPeriodStart = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_fWarmupPeriodStart");
	NetVars::DT_CSGameRulesProxy::m_bTerroristTimeOutActive = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bTerroristTimeOutActive");
	NetVars::DT_CSGameRulesProxy::m_bCTTimeOutActive = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bCTTimeOutActive");
	NetVars::DT_CSGameRulesProxy::m_flTerroristTimeOutRemaining = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_flTerroristTimeOutRemaining");
	NetVars::DT_CSGameRulesProxy::m_flCTTimeOutRemaining = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_flCTTimeOutRemaining");
	NetVars::DT_CSGameRulesProxy::m_nTerroristTimeOuts = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_nTerroristTimeOuts");
	NetVars::DT_CSGameRulesProxy::m_nCTTimeOuts = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_nCTTimeOuts");
	NetVars::DT_CSGameRulesProxy::m_iRoundTime = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_iRoundTime");
	NetVars::DT_CSGameRulesProxy::m_gamePhase = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_gamePhase");
	NetVars::DT_CSGameRulesProxy::m_totalRoundsPlayed = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_totalRoundsPlayed");
	NetVars::DT_CSGameRulesProxy::m_nOvertimePlaying = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_nOvertimePlaying");
	NetVars::DT_CSGameRulesProxy::m_timeUntilNextPhaseStarts = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_timeUntilNextPhaseStarts");
	NetVars::DT_CSGameRulesProxy::m_flCMMItemDropRevealStartTime = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_flCMMItemDropRevealStartTime");
	NetVars::DT_CSGameRulesProxy::m_flCMMItemDropRevealEndTime = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_flCMMItemDropRevealEndTime");
	NetVars::DT_CSGameRulesProxy::m_fRoundStartTime = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_fRoundStartTime");
	NetVars::DT_CSGameRulesProxy::m_bGameRestart = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bGameRestart");
	NetVars::DT_CSGameRulesProxy::m_flRestartRoundTime = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_flRestartRoundTime");
	NetVars::DT_CSGameRulesProxy::m_flGameStartTime = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_flGameStartTime");
	NetVars::DT_CSGameRulesProxy::m_iHostagesRemaining = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_iHostagesRemaining");
	NetVars::DT_CSGameRulesProxy::m_bAnyHostageReached = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bAnyHostageReached");
	NetVars::DT_CSGameRulesProxy::m_bMapHasBombTarget = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bMapHasBombTarget");
	NetVars::DT_CSGameRulesProxy::m_bMapHasRescueZone = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bMapHasRescueZone");
	NetVars::DT_CSGameRulesProxy::m_bMapHasBuyZone = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bMapHasBuyZone");
	NetVars::DT_CSGameRulesProxy::m_bIsQueuedMatchmaking = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bIsQueuedMatchmaking");
	NetVars::DT_CSGameRulesProxy::m_bIsValveDS = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bIsValveDS");
	NetVars::DT_CSGameRulesProxy::m_bIsQuestEligible = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bIsQuestEligible");
	NetVars::DT_CSGameRulesProxy::m_bLogoMap = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bLogoMap");
	NetVars::DT_CSGameRulesProxy::m_iNumGunGameProgressiveWeaponsCT = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_iNumGunGameProgressiveWeaponsCT");
	NetVars::DT_CSGameRulesProxy::m_iNumGunGameProgressiveWeaponsT = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_iNumGunGameProgressiveWeaponsT");
	NetVars::DT_CSGameRulesProxy::m_iSpectatorSlotCount = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_iSpectatorSlotCount");
	NetVars::DT_CSGameRulesProxy::m_bBombDropped = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bBombDropped");
	NetVars::DT_CSGameRulesProxy::m_bBombPlanted = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bBombPlanted");
	NetVars::DT_CSGameRulesProxy::m_iRoundWinStatus = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_iRoundWinStatus");
	NetVars::DT_CSGameRulesProxy::m_eRoundWinReason = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_eRoundWinReason");
	NetVars::DT_CSGameRulesProxy::m_flDMBonusStartTime = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_flDMBonusStartTime");
	NetVars::DT_CSGameRulesProxy::m_flDMBonusTimeLength = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_flDMBonusTimeLength");
	NetVars::DT_CSGameRulesProxy::m_unDMBonusWeaponLoadoutSlot = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_unDMBonusWeaponLoadoutSlot");
	NetVars::DT_CSGameRulesProxy::m_bDMBonusActive = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bDMBonusActive");
	NetVars::DT_CSGameRulesProxy::m_bTCantBuy = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bTCantBuy");
	NetVars::DT_CSGameRulesProxy::m_bCTCantBuy = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bCTCantBuy");
	NetVars::DT_CSGameRulesProxy::m_flGuardianBuyUntilTime = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_flGuardianBuyUntilTime");
	NetVars::DT_CSGameRulesProxy::m_iMatchStats_RoundResults = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_iMatchStats_RoundResults");
	NetVars::DT_CSGameRulesProxy::m_iMatchStats_PlayersAlive_T = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_iMatchStats_PlayersAlive_T");
	NetVars::DT_CSGameRulesProxy::m_iMatchStats_PlayersAlive_CT = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_iMatchStats_PlayersAlive_CT");
	NetVars::DT_CSGameRulesProxy::m_GGProgressiveWeaponOrderC = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_GGProgressiveWeaponOrderC");
	NetVars::DT_CSGameRulesProxy::m_GGProgressiveWeaponOrderT = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_GGProgressiveWeaponOrderT");
	NetVars::DT_CSGameRulesProxy::m_GGProgressiveWeaponKillUpgradeOrderCT = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_GGProgressiveWeaponKillUpgradeOrderCT");
	NetVars::DT_CSGameRulesProxy::m_GGProgressiveWeaponKillUpgradeOrderT = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_GGProgressiveWeaponKillUpgradeOrderT");
	NetVars::DT_CSGameRulesProxy::m_MatchDevice = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_MatchDevice");
	NetVars::DT_CSGameRulesProxy::m_bHasMatchStarted = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bHasMatchStarted");
	NetVars::DT_CSGameRulesProxy::m_TeamRespawnWaveTimes = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_TeamRespawnWaveTimes");
	NetVars::DT_CSGameRulesProxy::m_flNextRespawnWave = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_flNextRespawnWave");
	NetVars::DT_CSGameRulesProxy::m_nNextMapInMapgroup = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_nNextMapInMapgroup");
	NetVars::DT_CSGameRulesProxy::m_nEndMatchMapGroupVoteOptions = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_nEndMatchMapGroupVoteOptions");
	NetVars::DT_CSGameRulesProxy::m_bIsDroppingItems = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_bIsDroppingItems");
	NetVars::DT_CSGameRulesProxy::m_iActiveAssassinationTargetMissionID = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_iActiveAssassinationTargetMissionID");
	NetVars::DT_CSGameRulesProxy::m_fMatchStartTime = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_fMatchStartTime");
	NetVars::DT_CSGameRulesProxy::m_szTournamentEventName = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_szTournamentEventName");
	NetVars::DT_CSGameRulesProxy::m_szTournamentEventStage = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_szTournamentEventStage");
	NetVars::DT_CSGameRulesProxy::m_szTournamentPredictionsTxt = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_szTournamentPredictionsTxt");
	NetVars::DT_CSGameRulesProxy::m_nTournamentPredictionsPct = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_nTournamentPredictionsPct");
	NetVars::DT_CSGameRulesProxy::m_szMatchStatTxt = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_szMatchStatTxt");
	NetVars::DT_CSGameRulesProxy::m_nGuardianModeWaveNumber = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_nGuardianModeWaveNumber");
	NetVars::DT_CSGameRulesProxy::m_nGuardianModeSpecialKillsRemaining = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_nGuardianModeSpecialKillsRemaining");
	NetVars::DT_CSGameRulesProxy::m_nGuardianModeSpecialWeaponNeeded = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_nGuardianModeSpecialWeaponNeeded");
	NetVars::DT_CSGameRulesProxy::m_nHalloweenMaskListSeed = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_nHalloweenMaskListSeed");
	NetVars::DT_CSGameRulesProxy::m_numGlobalGiftsGiven = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_numGlobalGiftsGiven");
	NetVars::DT_CSGameRulesProxy::m_numGlobalGifters = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_numGlobalGifters");
	NetVars::DT_CSGameRulesProxy::m_numGlobalGiftsPeriodSeconds = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_numGlobalGiftsPeriodSeconds");
	NetVars::DT_CSGameRulesProxy::m_arrFeaturedGiftersAccounts = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_arrFeaturedGiftersAccounts");
	NetVars::DT_CSGameRulesProxy::m_arrFeaturedGiftersGifts = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_arrFeaturedGiftersGifts");
	NetVars::DT_CSGameRulesProxy::m_arrTournamentActiveCasterAccounts = NetVarManager::GetOffset(tables, "DT_CSGameRulesProxy", "m_arrTournamentActiveCasterAccounts");

	NetVars::CEconEntity::m_GIDEIIW = NetVarManager::GetOffset(tables, "DT_EconEntity", "m_Item") + 0xC;
	if (NetVars::CEconEntity::m_Item == 0xC)
		Utilities::Error("CEconEntity->m_Item failed to find netvar.");
}

std::vector<RecvTable*> NetVarManager::GetTables()
{
	std::vector<RecvTable*> tables;

	ClientClass* clientClass = I::Client->GetAllClasses();
	if (!clientClass)
		return std::vector<RecvTable*>();

	while (clientClass)
	{
		RecvTable* recvTable = clientClass->m_pRecvTable;
		tables.push_back(recvTable);

		clientClass = clientClass->m_pNext;
	}

	return tables;
}

RecvTable* NetVarManager::GetTable(std::vector<RecvTable*> tables, const char* tableName)
{
	if (tables.empty())
		return NULL;

	for (unsigned long i = 0; i < tables.size(); i++)
	{
		RecvTable* table = tables[i];

		if (!table)
			continue;

		if (_strnicmp(table->m_pNetTableName, tableName, 128) == 0)
			return table;
	}

	return NULL;
}

int NetVarManager::GetOffset(std::vector<RecvTable*> tables, const char* tableName, const char* propName)
{
	int offset = GetRecvProp(tables, tableName, propName);
	if (!offset)
		return 0;

	return offset;
}

int NetVarManager::GetRecvProp(std::vector<RecvTable*> tables, const char* tableName, const char* propName, RecvProp** prop)
{
	RecvTable* recvTable = GetTable(tables, tableName);
	if (!recvTable)
		return 0;

	int offset = GetRecvProp(tables, recvTable, propName, prop);
	if (!offset)
		return 0;

	return offset;
}

int NetVarManager::GetRecvProp(std::vector<RecvTable*> tables, RecvTable* recvTable, const char* propName, RecvProp** prop)
{
	int extraOffset = 0;

	for (int i = 0; i < recvTable->m_nProps; ++i) {
		RecvProp* recvProp = &recvTable->m_pProps[i];
		RecvTable* child = recvProp->m_pDataTable;

		if (child && (child->m_nProps > 0))
		{
			int tmp = GetRecvProp(tables, child, propName, prop);
			if (tmp)
				extraOffset += (recvProp->m_Offset + tmp);
		}

		if (_strnicmp(recvProp->m_pVarName, propName, 128))
			continue;

		if (prop)
			*prop = recvProp;

		return (recvProp->m_Offset + extraOffset);
	}

	return extraOffset;
}

std::string NetVarManager::DumpTable(RecvTable* table, int depth)
{
	std::string pre("");
	std::stringstream ss;

	for (int i = 0; i < depth; i++)
		pre.append("\t");

	ss << pre << table->m_pNetTableName << "\n";

	for (int i = 0; i < table->m_nProps; i++) {
		RecvProp* prop = &table->m_pProps[i];
		if (!prop)
			continue;

		std::string varName(prop->m_pVarName);

		if (varName.find("baseclass") == 0 || varName.find("0") == 0 || varName.find("1") == 0 || varName.find("2") == 0)
			continue;

		ss << pre << "\t" << varName << " [0x" << std::hex << prop->m_Offset << "]\n";

		if (prop->m_pDataTable)
			ss << DumpTable(prop->m_pDataTable, depth + 1);
	}

	return ss.str();
}

bool NetVarManager::HookProp(const char* tableName, const char* propName, RecvVarProxyFn newFn)
{
	RecvProp* recvProp = 0;
	NetVarManager::GetRecvProp(NetVarManager::GetTables(), tableName, propName, &recvProp);
	if (!recvProp)
		return false;

	recvProp->m_ProxyFn = newFn;

	return true;
}

bool NetVarManager::HookProp(const char* tableName, const char* propName, RecvVarProxyFn newFn, RecvVarProxyFn& oldFn)
{
	RecvProp* recvProp = 0;
	NetVarManager::GetRecvProp(NetVarManager::GetTables(), tableName, propName, &recvProp);
	if (!recvProp)
		return false;

	oldFn = recvProp->m_ProxyFn;
	recvProp->m_ProxyFn = newFn;

	return true;
}

void NetVarManager::DumpNetvars()
{
	std::stringstream ss;

	for (ClientClass* pClass = I::Client->GetAllClasses(); pClass != NULL; pClass = pClass->m_pNext)
	{
		RecvTable* table = pClass->m_pRecvTable;
		ss << NetVarManager::DumpTable(table, 0);
	}

	std::string fileName = Path::Get("netvars.txt");
	std::ofstream(fileName) << ss.str();
}

namespace NetVars
{
	void Initialize();

	namespace DT_BasePlayer
	{
		std::ptrdiff_t m_Local;
		std::ptrdiff_t m_aimPunchAngle;
		std::ptrdiff_t m_viewPunchAngle;
		std::ptrdiff_t m_vecViewOffset;
		std::ptrdiff_t m_nTickBase;
		std::ptrdiff_t m_vecVelocity;
		std::ptrdiff_t m_iHealth;
		std::ptrdiff_t m_lifeState;
		std::ptrdiff_t m_fFlags;
		std::ptrdiff_t m_iObserverMode;
		std::ptrdiff_t m_hObserverTarget;
		std::ptrdiff_t m_hViewModel;
		std::ptrdiff_t m_szLastPlaceName;
		std::ptrdiff_t deadflag;
	}

	namespace DT_BaseEntity
	{
		std::ptrdiff_t m_flAnimTime;
		std::ptrdiff_t m_flSimulationTime;
		std::ptrdiff_t m_vecOrigin;
		std::ptrdiff_t m_nRenderMode;
		std::ptrdiff_t m_iTeamNum;
		std::ptrdiff_t m_MoveType;
		std::ptrdiff_t m_Collision;
		std::ptrdiff_t m_bSpotted;
	}

	namespace DT_BaseCombatCharacter
	{
		std::ptrdiff_t m_hActiveWeapon;
		std::ptrdiff_t m_hMyWeapons;
		std::ptrdiff_t m_hMyWearables;
	}

	namespace DT_PlayerResource
	{
		std::ptrdiff_t m_iPing;
		std::ptrdiff_t m_iKills;
		std::ptrdiff_t m_iAssists;
		std::ptrdiff_t m_iDeaths;
		std::ptrdiff_t m_bConnected;
		std::ptrdiff_t m_iTeam;
		std::ptrdiff_t m_iPendingTeam;
		std::ptrdiff_t m_bAlive;
		std::ptrdiff_t m_iHealth;
	}

	namespace DT_CSPlayerResource
	{
		std::ptrdiff_t m_iPlayerC4;
		std::ptrdiff_t m_iPlayerVIP;
		std::ptrdiff_t m_vecC4;
		std::ptrdiff_t m_bombsiteCenterA;
		std::ptrdiff_t m_bombsiteCenterB;
		std::ptrdiff_t m_bHostageAlive;
		std::ptrdiff_t m_isHostageFollowingSomeone;
		std::ptrdiff_t m_iHostageEntityIDs;
		std::ptrdiff_t m_hostageRescueX;
		std::ptrdiff_t m_hostageRescueY;
		std::ptrdiff_t m_hostageRescueZ;
		std::ptrdiff_t m_iMVPs;
		std::ptrdiff_t m_iArmor;
		std::ptrdiff_t m_bHasHelmet;
		std::ptrdiff_t m_bHasDefuser;
		std::ptrdiff_t m_iScore;
		std::ptrdiff_t m_iCompetitiveRanking;
		std::ptrdiff_t m_iCompetitiveWins;
		std::ptrdiff_t m_iCompTeammateColor;
		std::ptrdiff_t m_bControllingBot;
		std::ptrdiff_t m_iControlledPlayer;
		std::ptrdiff_t m_iControlledByPlayer;
		std::ptrdiff_t m_iBotDifficulty;
		std::ptrdiff_t m_szClan;
		std::ptrdiff_t m_iTotalCashSpent;
		std::ptrdiff_t m_iCashSpentThisRound;
		std::ptrdiff_t m_nEndMatchNextMapVotes;
		std::ptrdiff_t m_bEndMatchNextMapAllVoted;
		std::ptrdiff_t m_nActiveCoinRank;
		std::ptrdiff_t m_nMusicID;
		std::ptrdiff_t m_nPersonaDataPublicLevel;
		std::ptrdiff_t m_nPersonaDataPublicCommendsLeader;
		std::ptrdiff_t m_nPersonaDataPublicCommendsTeacher;
		std::ptrdiff_t m_nPersonaDataPublicCommendsFriendly;
	}

	namespace DT_PlantedC4
	{
		std::ptrdiff_t m_bBombTicking;
		std::ptrdiff_t m_flC4Blow;
		std::ptrdiff_t m_bBombDefused;
		std::ptrdiff_t m_hBombDefuser;
		std::ptrdiff_t m_flDefuseCountDown;
	}

	namespace DT_CSPlayer
	{
		std::ptrdiff_t m_iShotsFired;
		std::ptrdiff_t m_angEyeAngles[2];
		std::ptrdiff_t m_iAccount;
		std::ptrdiff_t m_totalHitsOnServer;
		std::ptrdiff_t m_ArmorValue;
		std::ptrdiff_t m_bHasDefuser;
		std::ptrdiff_t m_bIsDefusing;
		std::ptrdiff_t m_bIsGrabbingHostage;
		std::ptrdiff_t m_bIsScoped;
		std::ptrdiff_t m_bGunGameImmunity;
		std::ptrdiff_t m_bIsRescuing;
		std::ptrdiff_t m_bHasHelmet;
		std::ptrdiff_t m_flFlashDuration;
		std::ptrdiff_t m_flFlashMaxAlpha;
		std::ptrdiff_t m_flLowerBodyYawTarget;
	}

	namespace DT_BaseAttributableItem
	{
		std::ptrdiff_t m_iItemDefinitionIndex;
		std::ptrdiff_t m_iItemIDHigh;
		std::ptrdiff_t m_iAccountID;
		std::ptrdiff_t m_iEntityQuality;
		std::ptrdiff_t m_szCustomName;
		std::ptrdiff_t m_nFallbackPaintKit;
		std::ptrdiff_t m_nFallbackSeed;
		std::ptrdiff_t m_flFallbackWear;
		std::ptrdiff_t m_nFallbackStatTrak;
	}

	namespace DT_BaseViewModel
	{
		std::ptrdiff_t m_nModelIndex;
		std::ptrdiff_t m_hWeapon;
		std::ptrdiff_t m_hOwner;
	}

	namespace DT_WeaponCSBase
	{
		std::ptrdiff_t m_fAccuracyPenalty;
	}

	namespace DT_WeaponC4
	{
		std::ptrdiff_t m_bStartedArming;
	}

	namespace DT_BaseCSGrenade
	{
		std::ptrdiff_t m_bRedraw;
		std::ptrdiff_t m_bIsHeldByPlayer;
		std::ptrdiff_t m_bPinPulled;
		std::ptrdiff_t m_fThrowTime;
		std::ptrdiff_t m_bLoopingSoundPlaying;
		std::ptrdiff_t m_flThrowStrength;
	}

	namespace DT_BaseCombatWeapon
	{
		std::ptrdiff_t m_flNextPrimaryAttack;
		std::ptrdiff_t m_hOwner;
		std::ptrdiff_t m_iClip1;
		std::ptrdiff_t m_bInReload;
		std::ptrdiff_t m_iPrimaryReserveAmmoCount;
	}

	namespace DT_DynamicProp
	{
		std::ptrdiff_t m_bShouldGlow;
	}

	namespace DT_CSGameRulesProxy
	{
		std::ptrdiff_t m_bFreezePeriod;
		std::ptrdiff_t m_bMatchWaitingForResume;
		std::ptrdiff_t m_bWarmupPeriod;
		std::ptrdiff_t m_fWarmupPeriodEnd;
		std::ptrdiff_t m_fWarmupPeriodStart;
		std::ptrdiff_t m_bTerroristTimeOutActive;
		std::ptrdiff_t m_bCTTimeOutActive;
		std::ptrdiff_t m_flTerroristTimeOutRemaining;
		std::ptrdiff_t m_flCTTimeOutRemaining;
		std::ptrdiff_t m_nTerroristTimeOuts;
		std::ptrdiff_t m_nCTTimeOuts;
		std::ptrdiff_t m_iRoundTime;
		std::ptrdiff_t m_gamePhase;
		std::ptrdiff_t m_totalRoundsPlayed;
		std::ptrdiff_t m_nOvertimePlaying;
		std::ptrdiff_t m_timeUntilNextPhaseStarts;
		std::ptrdiff_t m_flCMMItemDropRevealStartTime;
		std::ptrdiff_t m_flCMMItemDropRevealEndTime;
		std::ptrdiff_t m_fRoundStartTime;
		std::ptrdiff_t m_bGameRestart;
		std::ptrdiff_t m_flRestartRoundTime;
		std::ptrdiff_t m_flGameStartTime;
		std::ptrdiff_t m_iHostagesRemaining;
		std::ptrdiff_t m_bAnyHostageReached;
		std::ptrdiff_t m_bMapHasBombTarget;
		std::ptrdiff_t m_bMapHasRescueZone;
		std::ptrdiff_t m_bMapHasBuyZone;
		std::ptrdiff_t m_bIsQueuedMatchmaking;
		std::ptrdiff_t m_bIsValveDS;
		std::ptrdiff_t m_bIsQuestEligible;
		std::ptrdiff_t m_bLogoMap;
		std::ptrdiff_t m_iNumGunGameProgressiveWeaponsCT;
		std::ptrdiff_t m_iNumGunGameProgressiveWeaponsT;
		std::ptrdiff_t m_iSpectatorSlotCount;
		std::ptrdiff_t m_bBombDropped;
		std::ptrdiff_t m_bBombPlanted;
		std::ptrdiff_t m_iRoundWinStatus;
		std::ptrdiff_t m_eRoundWinReason;
		std::ptrdiff_t m_flDMBonusStartTime;
		std::ptrdiff_t m_flDMBonusTimeLength;
		std::ptrdiff_t m_unDMBonusWeaponLoadoutSlot;
		std::ptrdiff_t m_bDMBonusActive;
		std::ptrdiff_t m_bTCantBuy;
		std::ptrdiff_t m_bCTCantBuy;
		std::ptrdiff_t m_flGuardianBuyUntilTime;
		std::ptrdiff_t m_iMatchStats_RoundResults;
		std::ptrdiff_t m_iMatchStats_PlayersAlive_T;
		std::ptrdiff_t m_iMatchStats_PlayersAlive_CT;
		std::ptrdiff_t m_GGProgressiveWeaponOrderC;
		std::ptrdiff_t m_GGProgressiveWeaponOrderT;
		std::ptrdiff_t m_GGProgressiveWeaponKillUpgradeOrderCT;
		std::ptrdiff_t m_GGProgressiveWeaponKillUpgradeOrderT;
		std::ptrdiff_t m_MatchDevice;
		std::ptrdiff_t m_bHasMatchStarted;
		std::ptrdiff_t m_TeamRespawnWaveTimes;
		std::ptrdiff_t m_flNextRespawnWave;
		std::ptrdiff_t m_nNextMapInMapgroup;
		std::ptrdiff_t m_nEndMatchMapGroupVoteOptions;
		std::ptrdiff_t m_bIsDroppingItems;
		std::ptrdiff_t m_iActiveAssassinationTargetMissionID;
		std::ptrdiff_t m_fMatchStartTime;
		std::ptrdiff_t m_szTournamentEventName;
		std::ptrdiff_t m_szTournamentEventStage;
		std::ptrdiff_t m_szTournamentPredictionsTxt;
		std::ptrdiff_t m_nTournamentPredictionsPct;
		std::ptrdiff_t m_szMatchStatTxt;
		std::ptrdiff_t m_nGuardianModeWaveNumber;
		std::ptrdiff_t m_nGuardianModeSpecialKillsRemaining;
		std::ptrdiff_t m_nGuardianModeSpecialWeaponNeeded;
		std::ptrdiff_t m_nHalloweenMaskListSeed;
		std::ptrdiff_t m_numGlobalGiftsGiven;
		std::ptrdiff_t m_numGlobalGifters;
		std::ptrdiff_t m_numGlobalGiftsPeriodSeconds;
		std::ptrdiff_t m_arrFeaturedGiftersAccounts;
		std::ptrdiff_t m_arrFeaturedGiftersGifts;
		std::ptrdiff_t m_arrTournamentActiveCasterAccounts;
	}

	namespace CEconEntity
	{
		std::ptrdiff_t m_Item;
	}
}
