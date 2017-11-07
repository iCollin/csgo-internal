#include "gameevents.h"

#include "fonts.h"
#include "render.h"
#include "settings.h"

// logic from events in these
#include "nadehelper.h"
#include "glovechanger.h"
#include "globals.h"
#include "SkyChanger.h"

bool Config::Visuals::Sounds::Events::item_pickup = true;
bool Config::Visuals::Sounds::Events::player_footstep = true;
bool Config::Visuals::Sounds::Events::player_falldamage = true;
bool Config::Visuals::Sounds::Events::door_moving = true;
bool Config::Visuals::Sounds::Events::silencer_on = true;
bool Config::Visuals::Sounds::Events::silencer_off = true;
bool Config::Visuals::Sounds::Events::weapon_zoom = true;
bool Config::Visuals::Sounds::Events::weapon_reload = true;
bool Config::Visuals::Sounds::Events::weapon_fire = true;
bool Config::Visuals::Sounds::Events::bomb_actions = true;
bool Config::Visuals::Sounds::Events::bomb_dropped = true;
bool Config::Visuals::Sounds::Events::ShowEventType = true;
float Config::Visuals::Sounds::Events::expireTime = 2.f;
float Config::Visuals::Sounds::Events::MaximumDistance = 1500.f;
bool Config::Visuals::Bomb::AnnouncePlant = false;
bool Config::Visuals::HitMarkers::ShowImpacts = true;

bool EventTrackers::BombPlanted = false;

GameEventListener::GameEventListener()
{
	RegisterEvent("item_pickup");
	RegisterEvent("player_footstep");
	RegisterEvent("player_falldamage");
	RegisterEvent("door_moving");
	RegisterEvent("silencer_on");
	RegisterEvent("silencer_off");
	RegisterEvent("weapon_zoom");
	RegisterEvent("weapon_reload");
	RegisterEvent("weapon_fire");
	RegisterEvent("bullet_impact");
	RegisterEvent("bomb_begindefuse");
	RegisterEvent("bomb_abortdefuse");
	RegisterEvent("bomb_beginplant");
	RegisterEvent("bomb_abortplant");
	RegisterEvent("bomb_dropped");
	RegisterEvent("bomb_planted");

	RegisterEvent("client_disconnect");
}

GameEventListener::~GameEventListener()
{
	I::GameEventManager->RemoveListener(this);
}

void GameEventListener::RegisterEvent(const char * name)
{
	if (!I::GameEventManager->AddListener(this, name, false))
		Utilities::Error("Unable to add " + std::string(name) + " listener.");
	else
		Utilities::Debug("GameEvent " + std::string(name) + " registered successfully.");
}

C_BasePlayer* EntFromEvent(IGameEvent* event)
{
	return I::EntList->GetClientEntity(I::Engine->GetPlayerForUserID(event->GetInt("userid")));
}

float ExpireTime()
{
	return I::globalVars->curtime + Config::Visuals::Sounds::Events::expireTime;
}

// bullet impaccts lol ( can even get user -> team and color by team )

void GameEventListener::FireGameEvent(IGameEvent* event)
{
	if (!event)
		return;

	const char* name = event->GetName();

	if (!strcmp(name, "client_disconnect"))
	{
		NadeHelper::currentMapName = "";
		SkyChanger::SkyNeedsUpdate = true;
		return;
		// all other events have entities and such
	}
	// need to gen new glove material
	else if (!strcmp(name, "switch_team"))
	{
		GloveChanger::NeedsUpdate = true;
		return;
	}
	// clear any hanging sounds. curtime messes up in freezetime? dormant entities?
	else if (!strcmp(name, "round_start") || !strcmp(name, "round_end"))
	{
		EventTrackers::BombPlanted = false;
		Sounds.clear();
		return;
	}
	else if (!strcmp(name, "bomb_planted"))
	{
		EventTrackers::BombPlanted = true;

		if (!Config::Visuals::Bomb::AnnouncePlant)
			return;

		C_BasePlayer* entity = EntFromEvent(event);

		if (!entity)
			return;

		Vector bombPos = entity->GetVecOrigin();

		if (bombPos.Length() < 0.1f)
			return;

		bool A = ((*I::PlayerResourcePtr)->GetBombsiteA() - bombPos).Length() < ((*I::PlayerResourcePtr)->GetBombsiteB() - bombPos).Length();

		std::string plantMessage = "say_team Bomb planted at site ";
		plantMessage += A ? "A!" : "B!";

		I::Engine->ClientCmd_Unrestricted(plantMessage.c_str());
	}
	else if (!strcmp(name, "bomb_defused") || !strcmp(name, "bomb_exploded"))
	{
		EventTrackers::BombPlanted = false;
	}

	C_BasePlayer* entity = EntFromEvent(event);

	if (!G::LocalPlayer || entity == G::LocalPlayer)
		return;

	if (!entity || entity->GetDormant())
		return;

	if (Vector(entity->GetVecOrigin() - G::LocalPlayer->GetVecOrigin()).Length() > Config::Visuals::Sounds::Events::MaximumDistance)
		return;

	if (Config::Visuals::Sounds::FilterTeam && entity->GetTeam() == G::LocalPlayer->GetTeam())
		return;

	if (Config::Visuals::Sounds::Events::item_pickup && !strcmp(name, "item_pickup"))
	{
		if (event->GetBool("silent"))
			return;

		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "item_pickup" });
	}
	else if (Config::Visuals::HitMarkers::ShowImpacts && !strcmp(name, "bullet_impact"))
	{
		BulletImpacts.push_back({ entity, ExpireTime(), Vector(event->GetFloat("x"), event->GetFloat("y") , event->GetFloat("z")) });
	}
	else if (Config::Visuals::Sounds::Events::player_footstep && !strcmp(name, "player_footstep"))
	{
		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "player_footstep" });
	}
	else if (Config::Visuals::Sounds::Events::player_falldamage && !strcmp(name, "player_falldamage"))
	{
		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "player_falldamage" });
	}
	else if (Config::Visuals::Sounds::Events::door_moving && !strcmp(name, "door_moving"))
	{
		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "door_moving" });
	}
	else if (Config::Visuals::Sounds::Events::silencer_on && !strcmp(name, "silencer_on"))
	{
		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "silencer_on" });
	}
	else if (Config::Visuals::Sounds::Events::silencer_off && (!strcmp(name, "silencer_off") || !strcmp(name, "silencer_detach")))
	{
		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "silencer_detach" });
	}
	else if (Config::Visuals::Sounds::Events::weapon_zoom && !strcmp(name, "weapon_zoom"))
	{
		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "weapon_zoom" });
	}
	else if (Config::Visuals::Sounds::Events::weapon_fire && !strcmp(name, "weapon_fire"))
	{
		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "weapon_fire" });
	}
	else if (Config::Visuals::Sounds::Events::weapon_reload && !strcmp(name, "weapon_reload"))
	{
		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "weapon_reload" });
	}
	else if (Config::Visuals::Sounds::Events::bomb_actions && !strcmp(name, "bomb_begindefuse"))
	{
		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "bomb_begindefuse" });
	}
	else if (Config::Visuals::Sounds::Events::bomb_actions && !strcmp(name, "bomb_abortdefuse"))
	{
		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "bomb_abortdefuse" });
	}
	else if (Config::Visuals::Sounds::Events::bomb_actions && !strcmp(name, "bomb_beginplant"))
	{
		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "bomb_beginplant" });
	}
	else if (Config::Visuals::Sounds::Events::bomb_actions && !strcmp(name, "bomb_abortplant"))
	{
		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "bomb_abortplant" });
	}
	else if (Config::Visuals::Sounds::Events::bomb_dropped && !strcmp(name, "bomb_dropped"))
	{
		Sounds.push_back({ entity, ExpireTime(), entity->GetVecOrigin(), "bomb_dropped" });
	}
}

int GameEventListener::GetEventDebugID(void)
{
	return EVENT_DEBUG_ID_INIT;
}

void DrawSound(Vector screen, char* name, Color col)
{
	Render::Circle(screen.x, screen.y, 3, col);

	if (Config::Visuals::Sounds::Events::ShowEventType)
		Render::Text(screen.x + 5, screen.y, col, Fonts::smallESP, name);
}

void DrawImpact(Vector screen, Color col)
{
	Render::Clear(screen.x, screen.y, 4, 4, col);
}

void GameEventListener::PaintTraverse()
{
	if (Config::Visuals::Sounds::Enable)
	{
		for (size_t i = 0; i < Sounds.size(); i++)
		{
			if (Sounds[i].mExpireTime < I::globalVars->curtime || !Sounds[i].mEntity->GetAlive())
			{
				Sounds.erase(Sounds.begin() + i);
				continue;
			}

			Vector vScreen;
			if (!Render::WorldToScreen(Sounds[i].mOrigin, vScreen))
				continue;

			DrawSound(vScreen, Sounds[i].mSoundName, Colors::ESP::EVENTS);
		}
	}

	// todo add config
	if (Config::Visuals::HitMarkers::ShowImpacts)
	{
		for (size_t i = 0; i < BulletImpacts.size(); i++)
		{
			if (BulletImpacts[i].mExpireTime < I::globalVars->curtime)
			{
				BulletImpacts.erase(BulletImpacts.begin() + i);
				continue;
			}

			Vector vScreen;
			if (!Render::WorldToScreen(BulletImpacts[i].mOrigin, vScreen))
				continue;

			DrawImpact(vScreen, Colors::ESP::BULLETIMPACTS);
		}
	}
}
