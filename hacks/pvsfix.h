#pragma once

#include "hooks.h"
#include "interfaces.h"

namespace PVSfix
{
	void FrameStageNotify(ClientFrameStage_t stage)
	{
		if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
			return;

		int iLP = I::Engine->GetLocalPlayer();

		for (int i = 1; i <= I::globalVars->maxClients; i++)
		{
			if (i == iLP) 
				continue;

			IClientEntity* pEntity = I::EntList->GetClientEntity(i);
			if (!pEntity) 
				continue;

			*(int*)((uintptr_t)pEntity + 0xA30) = I::globalVars->framecount; // last occlusion check
			*(int*)((uintptr_t)pEntity + 0xA28) = 0; // occlusion flags
		}
	}
}