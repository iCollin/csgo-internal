#include "globals.h"

#define DECLARE_PTR(type, name)	type* name = nullptr;
#define DECLARE_PROP(type, name) type name = type();
#define DECLARE_PTR_ARY(type, name, size) type* name[size];

namespace G
{
	DECLARE_PTR(C_BasePlayer, LocalPlayer)
	DECLARE_PTR(C_BaseCombatWeapon, ActiveWeapon)
	DECLARE_PTR(CCSWeaponInfo, ActiveWeaponInfo)

	DECLARE_PTR(CUserCmd, UserCmd)

	DECLARE_PROP(ClientFrameStage_t, CurStage);

	DECLARE_PTR_ARY(C_BasePlayer, Players, 10)
}