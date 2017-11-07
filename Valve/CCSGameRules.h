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

#include <cstdint>
#include "netvar.h"

class C_CSGameRules
{
public:
	bool IsFreezeTime()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bFreezePeriod);
	}

	bool IsMMWaitingForResume()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bMatchWaitingForResume);
	}

	bool IsWarmup()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bWarmupPeriod);
	}

	bool TerroristTimeoutActive()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bTerroristTimeOutActive);
	}

	bool CTTimeoutActive()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bCTTimeOutActive);
	}

	float GetTimeoutRemainingTime()
	{
		if (TerroristTimeoutActive()) return *(float*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_flTerroristTimeOutRemaining);
		if (CTTimeoutActive()) return *(float*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_flCTTimeOutRemaining);

		return 0.f;
	}

	int GetRoundTime()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_iRoundTime);
	}

	int GetHostagesRemaining()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_iHostagesRemaining);
	}

	bool AnyHostagesReached()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bAnyHostageReached);
	}

	bool MapHasBombTarget()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bMapHasBombTarget);
	}

	bool MapHasRescueZone()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bMapHasRescueZone);
	}

	bool MapHasBuyZone()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bMapHasBuyZone);
	}

	bool GetQueuedMM()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bIsQueuedMatchmaking);
	}

	bool IsValveDS()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bIsValveDS);
	}

	int GetSpectatorSlotCount()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_iSpectatorSlotCount);
	}

	bool IsBombDropped()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bBombDropped);
	}

	bool IsBombPlanted()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bBombPlanted);
	}

	float GetDMBonusStartTime()
	{
		return *(float*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_flDMBonusStartTime);
	}

	float GetDMBonusTimeLength()
	{
		return *(float*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_flDMBonusTimeLength);
	}

	bool GetDMBonusActive()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_bDMBonusActive);
	}

	int GetTerroristsAlive()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_iMatchStats_PlayersAlive_T);
	}

	int GetCTsAlive()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSGameRulesProxy::m_iMatchStats_PlayersAlive_CT);
	}
};