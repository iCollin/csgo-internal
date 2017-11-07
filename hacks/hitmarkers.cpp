#include "hitmarkers.h"

#include "fonts.h"
#include "settings.h"

bool Config::Visuals::HitMarkers::Enable = true;
float Config::Visuals::HitMarkers::ExpireTime = 0.5f;
float Config::Visuals::HitMarkers::MarkerSize = 12.f;
float Config::Visuals::HitMarkers::TravelDistance = 24.f;

DamageListener::DamageListener()
{
	if (!I::GameEventManager->AddListener(this, "player_hurt", false))
		Utilities::Error("Unable to add player_hurt listener.");
	else
		Utilities::Log("GameEvent player_hurt registered successfully.");
}

DamageListener::~DamageListener()
{
	I::GameEventManager->RemoveListener(this);
}

void DamageListener::FireGameEvent(IGameEvent* event)
{
	if (!strcmp(event->GetName(), "player_hurt"))
		if (I::Engine->GetPlayerForUserID(event->GetInt("attacker")) == I::Engine->GetLocalPlayer())
		{
			HitMarker_t temp = { I::globalVars->curtime + Config::Visuals::HitMarkers::ExpireTime, event->GetInt("dmg_health") };

			if (temp.damage > 0 && temp.damage < 100)
				HitMarkers.push_back(temp);
		}
}

int DamageListener::GetEventDebugID(void)
{
	return EVENT_DEBUG_ID_INIT;
}

void DamageListener::PaintTraverse()
{
	static int width = 0;
	static int height = 0;
	if (height == 0 || width == 0)
		I::Engine->GetScreenSize(width, height);

	float alpha = 0.f;

	for (size_t i = 0; i < HitMarkers.size(); i++)
	{
		float timeDiff = HitMarkers[i].expireTime - I::globalVars->curtime;

		if (timeDiff < 0.f)
		{
			HitMarkers.erase(HitMarkers.begin() + i);
			continue;
		}

		int moveDist = (int)Config::Visuals::HitMarkers::TravelDistance;

		float ratio = 1.f - (timeDiff / Config::Visuals::HitMarkers::ExpireTime);
		alpha = (timeDiff / Config::Visuals::HitMarkers::ExpireTime) * 2.f;
		Render::Text(width / 2 + 5 + ratio * moveDist / 2, height / 2 + 5 + ratio * moveDist, 
			Colors::BLACK15.CustomAlpha(alpha), Fonts::tahoma16, std::to_string(HitMarkers[i].damage).c_str());
		Render::Text(width / 2 + 7 + ratio * moveDist / 2, height / 2 + 7 + ratio * moveDist, Colors::BLACK15.CustomAlpha(alpha), Fonts::tahoma16, std::to_string(HitMarkers[i].damage).c_str());
		Render::Text(width / 2 + 6 + ratio * moveDist / 2, height / 2 + 6 + ratio * moveDist, Colors::ESP::HITMARKERS.CustomAlpha(alpha), Fonts::tahoma16, std::to_string(HitMarkers[i].damage).c_str());
	}

	if (HitMarkers.size() > 0)
	{
		int lineSize = (int)Config::Visuals::HitMarkers::MarkerSize;
		Render::Line(width / 2 - lineSize / 2, height / 2 - lineSize / 2, width / 2 + lineSize / 2, height / 2 + lineSize / 2, Colors::ESP::HITMARKERS.CustomAlpha(alpha));
		Render::Line(width / 2 + lineSize / 2, height / 2 - lineSize / 2, width / 2 - lineSize / 2, height / 2 + lineSize / 2, Colors::ESP::HITMARKERS.CustomAlpha(alpha));
	}
}