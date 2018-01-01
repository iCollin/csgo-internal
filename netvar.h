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

#ifndef NETVAR_H
#define NETVAR_H

#include <map>
#include <vector>

#include "IBaseClientDLL.h"
namespace NetVarManager
{
	std::vector<RecvTable*> GetTables();
	RecvTable* GetTable(std::vector<RecvTable*> tables, const char* tableName);
	int GetOffset(std::vector<RecvTable*> tables, const char* tableName, const char *propName);
	int GetRecvProp(std::vector<RecvTable*> tables, const char* tableName, const char* propName, RecvProp** prop = 0);
	int GetRecvProp(std::vector<RecvTable*> tables, RecvTable* recvTable, const char* propName, RecvProp** prop = 0);
	std::string DumpTable(RecvTable* table, int depth);
	bool HookProp(const char* tableName, const char* propName, RecvVarProxyFn);
	bool HookProp(const char* tableName, const char* propName, RecvVarProxyFn, RecvVarProxyFn&);
	void DumpNetvars();
}

namespace NetVars
{
	void Initialize();

	namespace DT_BasePlayer
	{
		extern std::ptrdiff_t m_Local;
		extern std::ptrdiff_t m_aimPunchAngle;
		extern std::ptrdiff_t m_viewPunchAngle;
		extern std::ptrdiff_t m_vecViewOffset;
		extern std::ptrdiff_t m_nTickBase;
		extern std::ptrdiff_t m_vecVelocity;
		extern std::ptrdiff_t m_iHealth;
		extern std::ptrdiff_t m_lifeState;
		extern std::ptrdiff_t m_fFlags;
		extern std::ptrdiff_t m_iObserverMode;
		extern std::ptrdiff_t m_hObserverTarget;
		extern std::ptrdiff_t m_hViewModel;
		extern std::ptrdiff_t m_szLastPlaceName;
		extern std::ptrdiff_t deadflag;
	}

	namespace DT_BaseEntity
	{
		extern std::ptrdiff_t m_flAnimTime;
		extern std::ptrdiff_t m_flSimulationTime;
		extern std::ptrdiff_t m_vecOrigin;
		extern std::ptrdiff_t m_nRenderMode;
		extern std::ptrdiff_t m_iTeamNum;
		extern std::ptrdiff_t m_MoveType;
		extern std::ptrdiff_t m_Collision;
		extern std::ptrdiff_t m_bSpotted;
	}

	namespace DT_BaseCombatCharacter
	{
		extern std::ptrdiff_t m_hActiveWeapon;
		extern std::ptrdiff_t m_hMyWeapons;
		extern std::ptrdiff_t m_hMyWearables;
	}

	namespace DT_PlayerResource
	{
		extern std::ptrdiff_t m_iPing;
		extern std::ptrdiff_t m_iKills;
		extern std::ptrdiff_t m_iAssists;
		extern std::ptrdiff_t m_iDeaths;
		extern std::ptrdiff_t m_bConnected;
		extern std::ptrdiff_t m_iTeam;
		extern std::ptrdiff_t m_iPendingTeam;
		extern std::ptrdiff_t m_bAlive;
		extern std::ptrdiff_t m_iHealth;
	}

	namespace DT_CSPlayerResource
	{
		extern std::ptrdiff_t m_iPlayerC4;
		extern std::ptrdiff_t m_iPlayerVIP;
		extern std::ptrdiff_t m_vecC4;
		extern std::ptrdiff_t m_bombsiteCenterA;
		extern std::ptrdiff_t m_bombsiteCenterB;
		extern std::ptrdiff_t m_bHostageAlive;
		extern std::ptrdiff_t m_isHostageFollowingSomeone;
		extern std::ptrdiff_t m_iHostageEntityIDs;
		extern std::ptrdiff_t m_bombsiteCenterB;
		extern std::ptrdiff_t m_hostageRescueX;
		extern std::ptrdiff_t m_hostageRescueY;
		extern std::ptrdiff_t m_hostageRescueZ;
		extern std::ptrdiff_t m_iMVPs;
		extern std::ptrdiff_t m_iArmor;
		extern std::ptrdiff_t m_bHasHelmet;
		extern std::ptrdiff_t m_bHasDefuser;
		extern std::ptrdiff_t m_iScore;
		extern std::ptrdiff_t m_iCompetitiveRanking;
		extern std::ptrdiff_t m_iCompetitiveWins;
		extern std::ptrdiff_t m_iCompTeammateColor;
		extern std::ptrdiff_t m_bControllingBot;
		extern std::ptrdiff_t m_iControlledPlayer;
		extern std::ptrdiff_t m_iControlledByPlayer;
		extern std::ptrdiff_t m_iBotDifficulty;
		extern std::ptrdiff_t m_szClan;
		extern std::ptrdiff_t m_iTotalCashSpent;
		extern std::ptrdiff_t m_iCashSpentThisRound;
		extern std::ptrdiff_t m_nEndMatchNextMapVotes;
		extern std::ptrdiff_t m_bEndMatchNextMapAllVoted;
		extern std::ptrdiff_t m_nActiveCoinRank;
		extern std::ptrdiff_t m_nMusicID;
		extern std::ptrdiff_t m_nPersonaDataPublicLevel;
		extern std::ptrdiff_t m_nPersonaDataPublicCommendsLeader;
		extern std::ptrdiff_t m_nPersonaDataPublicCommendsTeacher;
		extern std::ptrdiff_t m_nPersonaDataPublicCommendsFriendly;
	}

	namespace DT_PlantedC4
	{
		extern std::ptrdiff_t m_bBombTicking;
		extern std::ptrdiff_t m_flC4Blow;
		extern std::ptrdiff_t m_bBombDefused;
		extern std::ptrdiff_t m_hBombDefuser;
		extern std::ptrdiff_t m_flDefuseCountDown;
	}

	namespace DT_CSPlayer
	{
		extern std::ptrdiff_t m_iShotsFired;
		extern std::ptrdiff_t m_angEyeAngles[2];
		extern std::ptrdiff_t m_iAccount;
		extern std::ptrdiff_t m_totalHitsOnServer;
		extern std::ptrdiff_t m_ArmorValue;
		extern std::ptrdiff_t m_bHasDefuser;
		extern std::ptrdiff_t m_bIsDefusing;
		extern std::ptrdiff_t m_bIsGrabbingHostage;
		extern std::ptrdiff_t m_bIsScoped;
		extern std::ptrdiff_t m_bGunGameImmunity;
		extern std::ptrdiff_t m_bIsRescuing;
		extern std::ptrdiff_t m_bHasHelmet;
		extern std::ptrdiff_t m_flFlashDuration;
		extern std::ptrdiff_t m_flFlashMaxAlpha;
		extern std::ptrdiff_t m_flLowerBodyYawTarget;
	}

	namespace DT_BaseAttributableItem
	{
		extern std::ptrdiff_t m_iItemDefinitionIndex;
		extern std::ptrdiff_t m_iItemIDHigh;
		extern std::ptrdiff_t m_iAccountID;
		extern std::ptrdiff_t m_iEntityQuality;
		extern std::ptrdiff_t m_szCustomName;
		extern std::ptrdiff_t m_nFallbackPaintKit;
		extern std::ptrdiff_t m_nFallbackSeed;
		extern std::ptrdiff_t m_flFallbackWear;
		extern std::ptrdiff_t m_nFallbackStatTrak;
	}

	namespace DT_BaseViewModel
	{
		extern std::ptrdiff_t m_nModelIndex;
		extern std::ptrdiff_t m_hWeapon;
		extern std::ptrdiff_t m_hOwner;
	}

	namespace DT_WeaponCSBase
	{
		extern std::ptrdiff_t m_fAccuracyPenalty;
	}

	namespace DT_WeaponC4
	{
		extern std::ptrdiff_t m_bStartedArming;
	}

	namespace DT_BaseCSGrenade
	{
		extern std::ptrdiff_t m_bRedraw;
		extern std::ptrdiff_t m_bIsHeldByPlayer;
		extern std::ptrdiff_t m_bPinPulled;
		extern std::ptrdiff_t m_fThrowTime;
		extern std::ptrdiff_t m_bLoopingSoundPlaying;
		extern std::ptrdiff_t m_flThrowStrength;
	}

	namespace DT_BaseCombatWeapon
	{
		extern std::ptrdiff_t m_flNextPrimaryAttack;
		extern std::ptrdiff_t m_hOwner;
		extern std::ptrdiff_t m_iClip1;
		extern std::ptrdiff_t m_bInReload;
		extern std::ptrdiff_t m_iPrimaryReserveAmmoCount;
	}

	namespace DT_DynamicProp
	{
		extern std::ptrdiff_t m_bShouldGlow;
	}

	namespace DT_CSGameRulesProxy
	{
		extern std::ptrdiff_t m_bFreezePeriod;
		extern std::ptrdiff_t m_bMatchWaitingForResume;
		extern std::ptrdiff_t m_bWarmupPeriod;
		extern std::ptrdiff_t m_fWarmupPeriodEnd;
		extern std::ptrdiff_t m_fWarmupPeriodStart;
		extern std::ptrdiff_t m_bTerroristTimeOutActive;
		extern std::ptrdiff_t m_bCTTimeOutActive;
		extern std::ptrdiff_t m_flTerroristTimeOutRemaining;
		extern std::ptrdiff_t m_flCTTimeOutRemaining;
		extern std::ptrdiff_t m_nTerroristTimeOuts;
		extern std::ptrdiff_t m_nCTTimeOuts;
		extern std::ptrdiff_t m_iRoundTime;
		extern std::ptrdiff_t m_gamePhase;
		extern std::ptrdiff_t m_totalRoundsPlayed;
		extern std::ptrdiff_t m_nOvertimePlaying;
		extern std::ptrdiff_t m_timeUntilNextPhaseStarts;
		extern std::ptrdiff_t m_flCMMItemDropRevealStartTime;
		extern std::ptrdiff_t m_flCMMItemDropRevealEndTime;
		extern std::ptrdiff_t m_fRoundStartTime;
		extern std::ptrdiff_t m_bGameRestart;
		extern std::ptrdiff_t m_flRestartRoundTime;
		extern std::ptrdiff_t m_flGameStartTime;
		extern std::ptrdiff_t m_iHostagesRemaining;
		extern std::ptrdiff_t m_bAnyHostageReached;
		extern std::ptrdiff_t m_bMapHasBombTarget;
		extern std::ptrdiff_t m_bMapHasRescueZone;
		extern std::ptrdiff_t m_bMapHasBuyZone;
		extern std::ptrdiff_t m_bIsQueuedMatchmaking;
		extern std::ptrdiff_t m_bIsValveDS;
		extern std::ptrdiff_t m_bIsQuestEligible;
		extern std::ptrdiff_t m_bLogoMap;
		extern std::ptrdiff_t m_iNumGunGameProgressiveWeaponsCT;
		extern std::ptrdiff_t m_iNumGunGameProgressiveWeaponsT;
		extern std::ptrdiff_t m_iSpectatorSlotCount;
		extern std::ptrdiff_t m_bBombDropped;
		extern std::ptrdiff_t m_bBombPlanted;
		extern std::ptrdiff_t m_iRoundWinStatus;
		extern std::ptrdiff_t m_eRoundWinReason;
		extern std::ptrdiff_t m_flDMBonusStartTime;
		extern std::ptrdiff_t m_flDMBonusTimeLength;
		extern std::ptrdiff_t m_unDMBonusWeaponLoadoutSlot;
		extern std::ptrdiff_t m_bDMBonusActive;
		extern std::ptrdiff_t m_bTCantBuy;
		extern std::ptrdiff_t m_bCTCantBuy;
		extern std::ptrdiff_t m_flGuardianBuyUntilTime;
		extern std::ptrdiff_t m_iMatchStats_RoundResults;
		extern std::ptrdiff_t m_iMatchStats_PlayersAlive_T;
		extern std::ptrdiff_t m_iMatchStats_PlayersAlive_CT;
		extern std::ptrdiff_t m_GGProgressiveWeaponOrderC;
		extern std::ptrdiff_t m_GGProgressiveWeaponOrderT;
		extern std::ptrdiff_t m_GGProgressiveWeaponKillUpgradeOrderCT;
		extern std::ptrdiff_t m_GGProgressiveWeaponKillUpgradeOrderT;
		extern std::ptrdiff_t m_MatchDevice;
		extern std::ptrdiff_t m_bHasMatchStarted;
		extern std::ptrdiff_t m_TeamRespawnWaveTimes;
		extern std::ptrdiff_t m_flNextRespawnWave;
		extern std::ptrdiff_t m_nNextMapInMapgroup;
		extern std::ptrdiff_t m_nEndMatchMapGroupVoteOptions;
		extern std::ptrdiff_t m_bIsDroppingItems;
		extern std::ptrdiff_t m_iActiveAssassinationTargetMissionID;
		extern std::ptrdiff_t m_fMatchStartTime;
		extern std::ptrdiff_t m_szTournamentEventName;
		extern std::ptrdiff_t m_szTournamentEventStage;
		extern std::ptrdiff_t m_szTournamentPredictionsTxt;
		extern std::ptrdiff_t m_nTournamentPredictionsPct;
		extern std::ptrdiff_t m_szMatchStatTxt;
		extern std::ptrdiff_t m_nGuardianModeWaveNumber;
		extern std::ptrdiff_t m_nGuardianModeSpecialKillsRemaining;
		extern std::ptrdiff_t m_nGuardianModeSpecialWeaponNeeded;
		extern std::ptrdiff_t m_nHalloweenMaskListSeed;
		extern std::ptrdiff_t m_numGlobalGiftsGiven;
		extern std::ptrdiff_t m_numGlobalGifters;
		extern std::ptrdiff_t m_numGlobalGiftsPeriodSeconds;
		extern std::ptrdiff_t m_arrFeaturedGiftersAccounts;
		extern std::ptrdiff_t m_arrFeaturedGiftersGifts;
		extern std::ptrdiff_t m_arrTournamentActiveCasterAccounts;
	}

	namespace CEconEntity
	{
		extern std::ptrdiff_t m_GIDEIIW;
	}
}

#endif
