#pragma once

#include "render.h"
#include "IGameEvent.h"
#include "interfaces.h"

/*
player_hurt

short   userid		user ID who was hurt
short   attacker	user ID who attacked
byte	health		remaining health points
byte	armor		remaining armor points
string	weapon		weapon name attacker used, if not the world
short	dmg_health	damage done to health
byte    dmg_armor	damage done to armor
byte	hitgroup	hitgroup that was damaged
*/

struct HitMarker_t
{
	float expireTime;
	int damage;
};

class DamageListener : public IGameEventListener2
{
protected:
	std::vector<HitMarker_t> HitMarkers;

public:
	DamageListener();
	~DamageListener();

	void FireGameEvent(IGameEvent* event) override;
	int GetEventDebugID(void) override;
	void PaintTraverse();
};