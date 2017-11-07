#pragma once

#include "interfaces.h"

struct Event_t
{
	Event_t(C_BasePlayer* entity, float expiretime, Vector origin) : mEntity(entity), mExpireTime(expiretime), mOrigin(origin) {}

	C_BasePlayer* mEntity;
	float mExpireTime;
	Vector mOrigin;
};

struct SoundEvent_t : public Event_t
{
	SoundEvent_t(C_BasePlayer* entity, float expireTime, Vector origin, char* soundName) : Event_t(entity, expireTime, origin), mSoundName(soundName) {}

	char* mSoundName;
};

class GameEventListener : public IGameEventListener2
{
protected:
	std::vector<SoundEvent_t> Sounds;
	std::vector<Event_t> BulletImpacts;

public:
	GameEventListener();
	~GameEventListener();

	void RegisterEvent(const char* name);
	void FireGameEvent(IGameEvent* event) override;
	int GetEventDebugID(void) override;
	void PaintTraverse();
};

namespace EventTrackers
{
	extern bool BombPlanted;
}