#pragma once

#include "interfaces.h"

namespace Entity
{
	bool LineGoesThroughSmoke(Vector, Vector, int16_t);
	bool IsPlayerVisible(C_BasePlayer* player, Bone bone, float fov = 180.f, bool smoke_check = false);
	bool IsVisible(C_BaseEntity* player, float fov = 180.f, bool smoke_check = false);
	bool IsPositionVisible(C_BasePlayer* localplayer, Vector origin, float fov = 180.f, bool smoke_check = false);
	bool IsPlanting(C_BasePlayer* player);
	Bone GetBoneByName(C_BasePlayer* player, const char* boneName);
}