#pragma once

#include "interfaces.h"

#define DECLARE_PTR(type, name)	extern type* name;
#define DECLARE_PROP(type, name) extern type name;
#define DECLARE_PTR_ARY(type, name, size) extern type* name[size];

// GLOBAL VARS THAT WILL BE UPDATED ONCE PER TICK AND REFERENCED FROM WHEREVER
// THIS IS TO PREVENT CODE WASTE AND REDUCE POSSIBLE PLACES FOR ERRORS TO OCCUR
namespace G
{
	DECLARE_PTR(C_BasePlayer, LocalPlayer)
	DECLARE_PTR(C_BaseCombatWeapon, ActiveWeapon)
	DECLARE_PTR(CCSWeaponInfo, ActiveWeaponInfo)

	DECLARE_PTR(CUserCmd, UserCmd)

	DECLARE_PROP(ClientFrameStage_t, CurStage);

	DECLARE_PTR_ARY(C_BasePlayer, Players, 10)
}