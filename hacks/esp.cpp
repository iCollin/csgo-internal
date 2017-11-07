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
credit: Casual_Hacker @ unknowncheats.me (Bomb Damage Indication)
*/

#include "esp.h"

#include "menu.h"
#include "globals.h"
#include "time.h"

#include "math.h"
#include "render.h"
#include "fonts.h"
#include "settings.h"

#include "entity.h"
#include "dlights.h"
#include "aimbot.h"
#include "hitmarkers.h"
#include "skinchanger.h"
#include "gameevents.h"

bool Config::Visuals::ESP::Enable = true;
bool Config::Visuals::ESP::SpreadCircle = true;

// visuals bomb config
bool Config::Visuals::Bomb::ShowPlantedOnT = true;
bool Config::Visuals::Bomb::ShowPlantedOnCT = true;
bool Config::Visuals::Bomb::ShowWildOnT = true;
bool Config::Visuals::Bomb::ShowWildOnCT = false;
bool Config::Visuals::Bomb::ShowDefusingCT = true;
bool Config::Visuals::Bomb::BombTimer = true;
bool Config::Visuals::Bomb::PredictHealth = true;

// visuals ESP config
bool Config::Visuals::ESP::VisibleCheck = true;
bool Config::Visuals::ESP::PlayerESP = true;
bool Config::Visuals::ESP::DropESP = true;
bool Config::Visuals::ESP::ChickenESP = true;
bool Config::Visuals::ESP::DrawBoxes = true;
bool Config::Visuals::ESP::DrawNames = true;
bool Config::Visuals::ESP::DrawHealthBar = true;
bool Config::Visuals::ESP::HitMarkers = true;
bool Config::Visuals::ESP::FilterTeam = false;
bool Config::Visuals::ESP::GrenadeESP = true;
bool Config::Visuals::ESP::DrawDlights = true;
bool Config::Visuals::ESP::BarrelESP = true;
float Config::Visuals::ESP::BarrelESPLength = 250.f;
bool Config::Visuals::ESP::AimSpot = true;

// visuals player info controls
bool Config::Visuals::ESP::PlayerInfo::Enable = true;
bool Config::Visuals::ESP::PlayerInfo::Rank = true;
bool Config::Visuals::ESP::PlayerInfo::SteamID = false;
bool Config::Visuals::ESP::PlayerInfo::Weapon = true;
bool Config::Visuals::ESP::PlayerInfo::Health = true;
bool Config::Visuals::ESP::PlayerInfo::Armor = true;
bool Config::Visuals::ESP::PlayerInfo::Reloading = true;
bool Config::Visuals::ESP::PlayerInfo::Flashed = true;
bool Config::Visuals::ESP::PlayerInfo::Scoped = true;
bool Config::Visuals::ESP::PlayerInfo::BombCarrier = true;
bool Config::Visuals::ESP::PlayerInfo::DefuseKit = true;
bool Config::Visuals::ESP::PlayerInfo::Defusing = true;
bool Config::Visuals::ESP::PlayerInfo::HostageCarrier = true;
bool Config::Visuals::ESP::PlayerInfo::Rescuing = true;
bool Config::Visuals::ESP::PlayerInfo::Location = false;

// visuals sounds config
bool Config::Visuals::Sounds::Enable = true;
bool Config::Visuals::Sounds::FilterTeam = false;
bool Config::Visuals::Sounds::ShowOnRadar = true;
float Config::Visuals::Sounds::MinimumVolume = 0.36f;
bool Config::Visuals::Sounds::MuteAmbient = true;

bool Config::Debug::DrawBoneIDs = true;
bool Config::Debug::StreamMode = false;

int ESP::bombCarrierIndex = -1;

class Footstep
{
public:
	Footstep(int id, Vector pos, clock_t endtime)
	{
		entityID = id;
		position = pos;
		endTime = endtime;
	}

	int entityID;
	Vector position;
	clock_t endTime;
};

std::vector<Footstep> footsteps;

// DEBUG ASSERTION: .script file not found, but its data was accessed. (prob. get model)
NadeInfo_t ESP::GetNadeInfo(C_BaseEntity* nade)
{
	model_t* nadeModel = nade->GetModel();

	Color nadeColor = Color(255, 255, 255, 255);
	std::string nadeName = "";

	if (!nadeModel || !nadeModel->name)
	{
		return { nadeColor, nadeName };
	}
	
	if (strstr(nadeModel->name, "flashbang"))
	{
		nadeName = "Flashbang";
		nadeColor = Colors::Grenades::FLASH;
	}
	else if (strstr(nadeModel->name, "frag"))
	{
		nadeName = "Frag";
		nadeColor = Colors::Grenades::FRAG;
	}
	else if (strstr(nadeModel->name, "smoke"))
	{
		nadeName = "Smoke";
		nadeColor = Colors::Grenades::SMOKE;
	}
	else if (strstr(nadeModel->name, "decoy"))
	{
		nadeName = "Decoy";
		nadeColor = Colors::Grenades::DECOY;
	}
	else if (strstr(nadeModel->name, "incendiary") || strstr(nadeModel->name, "molotov"))
	{
		nadeName = "Molotov";
		nadeColor = Colors::Grenades::MOLLY;
	}

	return { nadeColor, nadeName };
}

void DrawOriginCircle(Vector origin, int radius, Color col = Colors::BLACK)
{
	Vector screen;

	if (!Render::WorldToScreen(origin, screen))
		return;

	Render::OutlineCircle(screen.x, screen.y, 3, 8, col);
}

void DrawSounds()
{
	for (size_t i = 0; i < footsteps.size(); i++)
	{
		clock_t timeRemaining = footsteps[i].endTime - clock();

		if (timeRemaining < 1)
		{
			footsteps.erase(footsteps.begin() + i);
			continue;
		}

		C_BasePlayer* pEntity = I::EntList->GetClientEntity(footsteps[i].entityID);
		if (!pEntity || !pEntity->GetAlive())
			continue;

		bool teamMate = pEntity->GetTeam() == G::LocalPlayer->GetTeam();
		if (teamMate && Config::Visuals::Sounds::FilterTeam)
			continue;

		if (Config::Visuals::Sounds::ShowOnRadar)
			*pEntity->GetSpotted() = true;

		DrawOriginCircle(footsteps[i].position, (5 * timeRemaining / CLOCKS_PER_SEC), teamMate ? Colors::ESP::FOOTSTEPS_TEAM : Colors::ESP::FOOTSTEPS_ENEMY);
	}
}

void ESP::EmitSound(int entIndex, const char* pSample, float volume)
{
	if (!Config::Visuals::Sounds::Enable || volume < Config::Visuals::Sounds::MinimumVolume)
		return;

	// footsteps, landing, reloading
	if (!strstr(pSample, "player/footsteps") && !strstr(pSample, "player/land") && !strstr(pSample, "clipout"))
		return;

	if (entIndex == I::Engine->GetLocalPlayer())
		return;

	footsteps.push_back(Footstep(entIndex, I::EntList->GetClientEntity(entIndex)->GetVecOrigin(), clock() + 1.5 * CLOCKS_PER_SEC));
}

std::string ESP::FriendlyWeaponName(int index)
{
	std::string longName = SkinChanger::GetSkinNameByIndex(index);

	return longName[0] == 'W' ? longName.substr(7) : longName;
	/*
	switch (index)
	{
	case (int)ItemDefinitionIndex::WEAPON_DEAGLE:
		return "DEAGLE";
	case (int)ItemDefinitionIndex::WEAPON_ELITE:
		return "DUAL";
	case (int)ItemDefinitionIndex::WEAPON_FIVESEVEN:
		return "FIVESEVEN";
	case (int)ItemDefinitionIndex::WEAPON_GLOCK:
		return "GLOCK";
	case (int)ItemDefinitionIndex::WEAPON_AK47:
		return "AK47";
	case (int)ItemDefinitionIndex::WEAPON_AUG:
		return "AUG";
	case (int)ItemDefinitionIndex::WEAPON_AWP:
		return "AWP";
	case (int)ItemDefinitionIndex::WEAPON_FAMAS:
		return "FAMAS";
	case (int)ItemDefinitionIndex::WEAPON_G3SG1:
		return "G3SG1";
	case (int)ItemDefinitionIndex::WEAPON_GALILAR:
		return "GALIL";
	case (int)ItemDefinitionIndex::WEAPON_M249:
		return "M249";
	case (int)ItemDefinitionIndex::WEAPON_M4A1:
		return "M4A4";
	case (int)ItemDefinitionIndex::WEAPON_MAC10:
		return "MAC10";
	case (int)ItemDefinitionIndex::WEAPON_P90:
		return "P90";
	case (int)ItemDefinitionIndex::WEAPON_UMP45:
		return "UMP";
	case (int)ItemDefinitionIndex::WEAPON_XM1014:
		return "XM1014";
	case (int)ItemDefinitionIndex::WEAPON_BIZON:
		return "BIZON";
	case (int)ItemDefinitionIndex::WEAPON_MAG7:
		return "MAG7";
	case (int)ItemDefinitionIndex::WEAPON_NEGEV:
		return "NEGEV";
	case (int)ItemDefinitionIndex::WEAPON_SAWEDOFF:
		return "SAWEDOFF";
	case (int)ItemDefinitionIndex::WEAPON_TEC9:
		return "TEC9";
	case (int)ItemDefinitionIndex::WEAPON_TASER:
		return "TASER";
	case (int)ItemDefinitionIndex::WEAPON_HKP2000:
		return "HKP2000";
	case (int)ItemDefinitionIndex::WEAPON_MP7:
		return "MP7";
	case (int)ItemDefinitionIndex::WEAPON_MP9:
		return "MP9";
	case (int)ItemDefinitionIndex::WEAPON_NOVA:
		return "NOVA";
	case (int)ItemDefinitionIndex::WEAPON_P250:
		return "P250";
	case (int)ItemDefinitionIndex::WEAPON_SCAR20:
		return "SCAR20";
	case (int)ItemDefinitionIndex::WEAPON_SG556:
		return "SG556";
	case (int)ItemDefinitionIndex::WEAPON_SSG08:
		return "SSG08";
	case (int)ItemDefinitionIndex::WEAPON_KNIFE:
		return "KNIFE_CT";
	case (int)ItemDefinitionIndex::WEAPON_FLASHBANG:
		return "FLASHBANG";
	case (int)ItemDefinitionIndex::WEAPON_HEGRENADE:
		return "HEGRENADE";
	case (int)ItemDefinitionIndex::WEAPON_SMOKEGRENADE:
		return "SMOKE";
	case (int)ItemDefinitionIndex::WEAPON_MOLOTOV:
		return "T_MOLOTOV";
	case (int)ItemDefinitionIndex::WEAPON_DECOY:
		return "DECOY";
	case (int)ItemDefinitionIndex::WEAPON_C4:
		return "C4";
	case (int)ItemDefinitionIndex::WEAPON_KNIFE_T:
		return "KNIFE_T";
	case (int)ItemDefinitionIndex::WEAPON_M4A1_SILENCER:
		return "M4A1";
	case (int)ItemDefinitionIndex::WEAPON_USP_SILENCER:
		return "USP";
	case (int)ItemDefinitionIndex::WEAPON_CZ75A:
		return "CZ75";
	}

	return "#UNDEF_" + std::to_string((int)index);*/
}

Color GetPlayerColor(C_BasePlayer* pEntity)
{
	if (!G::LocalPlayer)
		return Color(255, 0, 255, 255);

	if (pEntity == G::LocalPlayer) return Color(0, 0, 0, 0);

	bool visible = Entity::IsPlayerVisible(pEntity, (Bone)8, 180.f, true);

	if (pEntity->GetTeam() == TeamID::TEAM_TERRORIST)
		return visible ? Colors::ESP::TCHAMSVISIBLE : Colors::ESP::TCHAMS;

	return visible ? Colors::ESP::CTCHAMSVISIBLE : Colors::ESP::CTCHAMS;
}

bool GetBox(C_BaseEntity* entity, int& x, int& y, int& w, int& h)
{
	// Variables
	Vector vOrigin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = entity->GetVecOrigin();
	min = entity->GetCollideable()->OBBMins() + vOrigin;
	max = entity->GetCollideable()->OBBMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (I::DebugOverlay->ScreenPosition(points[3], flb) || I::DebugOverlay->ScreenPosition(points[5], brt)
		|| I::DebugOverlay->ScreenPosition(points[0], blb) || I::DebugOverlay->ScreenPosition(points[4], frt)
		|| I::DebugOverlay->ScreenPosition(points[2], frb) || I::DebugOverlay->ScreenPosition(points[1], brb)
		|| I::DebugOverlay->ScreenPosition(points[6], blt) || I::DebugOverlay->ScreenPosition(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	x = (int)left;
	y = (int)top;
	w = (int)(right - left);
	h = (int)(bottom - top);

	return true;
}

void DrawBox(C_BaseEntity* pEntity, int x, int y, int w, int h, Color col)
{
	Render::Outline(x - 1, y - 1, w + 2, h + 2, Colors::BLACK35);
	Render::Outline(x, y, w, h, col);
	Render::Outline(x + 1, y + 1, w - 2, h - 2, Colors::BLACK35);
}

void DrawHealthBar(C_BasePlayer* pEntity, int x, int y, int w, int h)
{
	// Armor
	float armorRatio = 1.f - (float)pEntity->GetArmor() / 100.f;
	Render::Outline(x - 6, y - 1, 4, h + 2, Colors::BLACK35);
	Render::Clear(x - 6, y - 1, 4, h + 2, Colors::BLACK35.CustomAlpha(.5f));
	int armor = ((float)h * armorRatio);
	Render::Clear(x - 5, y + armor, 2, h - armor, Colors::ESP::ARMORBAR);

	// Health
	float healthRatio = 1.f - (float)pEntity->GetHealth() / 100.f;
	Render::Outline(x + w + 2, y - 1, 4, h + 2, Colors::BLACK35);
	Render::Clear(x + w + 2, y - 1, 4, h + 2, Colors::BLACK35.CustomAlpha(.5f));
	int health = ((float)h * healthRatio);
	Render::Clear(x + w + 3, y + health, 2, h - health, Colors::ESP::HEALTHBAR);
}

const char* ranks[] = {
	"Unranked",
	"Silver I",
	"Silver II",
	"Silver III",
	"Silver IV",
	"Silver Elite",
	"Silver Elite Master",

	"Gold Nova I",
	"Gold Nova II",
	"Gold Nova III",
	"Gold Nova Master",
	"Master Guardian I",
	"Master Guardian II",

	"Master Guardian Elite",
	"Distinguished Master Guardian",
	"Legendary Eagle",
	"Legendary Eagle Master",
	"Supreme Master First Class",
	"The Global Elite"
};

void DrawInfo(C_BasePlayer* pEntity, IEngineClient::player_info_t pInfo, int x, int y, int w, int h, int index)
{
	static POINT txtSize = Render::GetTextSize(Fonts::smallESP, "Hello");
	x += 8;

	std::vector<std::string> infos;

	auto weapon = (C_BaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(pEntity->GetActiveWeapon());

	if (Config::Visuals::ESP::PlayerInfo::Rank && I::PlayerResourcePtr && *I::PlayerResourcePtr)
	{
		int rank = *(*I::PlayerResourcePtr)->GetCompetitiveRanking(index);
		infos.push_back(ranks[rank]);
	}

	if (Config::Visuals::ESP::PlayerInfo::SteamID)
	{
		POINT idSize = Render::GetTextSize(Fonts::ESP, pInfo.guid);
		Render::Text(x + (w - idSize.x) / 2, y + h + idSize.y + 4, Colors::Menu::MENUTEXT, Fonts::ESP, pInfo.guid);
	}

	if (Config::Visuals::ESP::PlayerInfo::Health)
	{
		std::string buf = std::to_string(pEntity->GetHealth()) + " HP";
		infos.push_back(buf);
	}

	if (Config::Visuals::ESP::PlayerInfo::Armor)
	{
		std::string buf = std::to_string(pEntity->GetArmor()) + " Armor";
		infos.push_back(buf);
	}

	if (Config::Visuals::ESP::PlayerInfo::Weapon && weapon)
	{
		std::string modelName = ESP::FriendlyWeaponName((int)*weapon->GetItemDefinitionIndex());

		if (weapon->GetAmmo() > 0)
			modelName += " | " + std::to_string(weapon->GetAmmo()) + "/" + std::to_string(weapon->GetReserveAmmo());

		infos.push_back(modelName);
	}

	if (Config::Visuals::ESP::PlayerInfo::Scoped && pEntity->IsScoped())
		infos.push_back("Scoped");

	if (Config::Visuals::ESP::PlayerInfo::Reloading && weapon && weapon->GetInReload())
		infos.push_back("Reloading");

	if (Config::Visuals::ESP::PlayerInfo::Flashed && pEntity->GetFlashBangTime() - I::globalVars->curtime > 2.f)
		infos.push_back("Flashed");

	if (Config::Visuals::ESP::PlayerInfo::Defusing && pEntity->IsDefusing())
		infos.push_back("Defusing");

	// cs player resource -> getplayerc4 to check iff index == for bomb carrier ESP
	if (Config::Visuals::ESP::PlayerInfo::BombCarrier && (*I::PlayerResourcePtr)->GetPlayerC4() == index)
		infos.push_back("Bomb Carrier");

	if (Config::Visuals::ESP::PlayerInfo::DefuseKit && pEntity->HasDefuser())
		infos.push_back("Defuse Kit");

	if (Config::Visuals::ESP::PlayerInfo::HostageCarrier && pEntity->IsGrabbingHostage())
		infos.push_back("Hostage Carrier");

	if (Config::Visuals::ESP::PlayerInfo::Rescuing && pEntity->IsRescuing())
		infos.push_back("Rescuing");

	if (Config::Visuals::ESP::PlayerInfo::Location)
		infos.push_back(pEntity->GetLastPlaceName());

	for (int i = 0; i < infos.size(); i++)
		Render::Text(x + w + 4, y + i * txtSize.y, Colors::Menu::MENUTEXT, Fonts::smallESP, infos[i].c_str());
}

void DrawDLights(C_BasePlayer* pEntity)
{
	DLights::AllocDlight(Color(255, 15, 255), 10.f, 50.f, 10.f, pEntity->GetIndex() + 69, pEntity->GetVecOrigin(), pEntity->GetVecOrigin() + Vector(0, 0, 32));
}

Vector PlayerViewPoint(float Line, Vector Angle, Vector ClientPos)
{
	Vector Trail;

	Math::AngleVectors(Angle, Trail);
	//Trail[2] = -Trail[2]; // same as 0.0f

	Vector Pos;
	VectorCopy(ClientPos, Pos);

	Pos[0] += (Trail[0] * Line);
	Pos[1] += (Trail[1] * Line);
	Pos[2] += (Trail[2] * Line);

	return Pos;
}

void DrawBarrel(C_BasePlayer* pEntity, Color col)
{
	Vector origin = pEntity->GetVecOrigin();
	origin.z += 65.f;

	Vector end, head;

	if (!Render::WorldToScreen(PlayerViewPoint(Config::Visuals::ESP::BarrelESPLength, *pEntity->GetEyeAngles(), origin), end))
		return;

	if (!Render::WorldToScreen(origin, head))
		return;

	Render::Line(end.x, end.y, head.x, head.y, col);
	Render::Circle(end.x, end.y, 2, col);
}

void DrawName(C_BasePlayer* pEntity, std::string name, int x, int y, int w, int h, bool littleFont = false)
{
	POINT txtSize = Render::GetTextSize(Fonts::ESP, name.c_str());
	Render::Text(x + ((w - txtSize.x) / 2), y - txtSize.y - 4, Colors::ESP::ESPTEXT, littleFont ? Fonts::smallESP : Fonts::ESP, name.c_str());
}

void DrawSpreadCircle()
{
	if (!Config::Visuals::ESP::SpreadCircle)
		return;

	static int width = 0;
	static int height = 0;
	if (height == 0 || width == 0)
		I::Engine->GetScreenSize(width, height);

	if (G::LocalPlayer->GetAlive() && G::ActiveWeapon/* && ESP::GetNadeInfo(G::ActiveWeapon).name == ""*/)
	{
		float spreadDist = ((G::ActiveWeapon->GetInaccuracy() + G::ActiveWeapon->GetSpread()) * 320.f) / std::tan(DEG2RAD(Config::Visuals::OverrideView::CustomFOV ? Config::Visuals::OverrideView::FOV : 68.f) * 0.5f);
		float spreadRadius = spreadDist * (height / 480.f);

		Render::OutlineCircle(width / 2, height / 2, (int)spreadRadius, 24, Colors::ESP::SPREADCIRCLE);
	}
}

void DrawBonesWithIds(C_BasePlayer* local, C_BasePlayer* entity)
{
	if (!Entity::IsPlayerVisible(entity, Bone::BONE_HEAD))
		return;

	if (local == entity)
		return;

	QAngle viewAngles;
	I::Engine->GetViewAngles(viewAngles);

	Vector source = local->GetEyePosition();

	for (size_t i = 0; i < 128; i++)
	{
		Vector dest = entity->GetBonePosition(i);

		if (dest.Length() < 1.f)
			continue;

		Vector screen;
		if (!Render::WorldToScreen(dest, screen))
			continue;

		Render::Text(screen.x, screen.y, Colors::ESP::ESPTEXT, Fonts::MENU, std::to_string(i).c_str());
	}
}

void DrawAimSpot(C_BasePlayer* entity, Bone bone)
{
	Vector screen;
	Vector world = entity->GetBonePosition((int)bone);

	if (!Render::WorldToScreen(world, screen))
		return;

	Render::CircleOutlined(screen.x, screen.y, 3, Colors::ESP::AIMSPOT, Colors::BLACK40);
}

void DrawPlayer(C_BasePlayer* pLocal, C_BasePlayer* pEntity, IEngineClient::player_info_t pInfo, int index)
{
	if (pLocal == pEntity)
		return;

	if (pEntity->GetDormant())
		return;

	if (Config::Visuals::ESP::FilterTeam && pEntity->GetTeam() == pLocal->GetTeam())
		return;

	Bone targetBone = Aimbot::GetTargetBone(pLocal, pEntity, WeaponType_t::INVALID);

	if (Config::Visuals::ESP::VisibleCheck && !Entity::IsPlayerVisible(pEntity, targetBone))
		return;

	if (Config::Visuals::ESP::AimSpot)
		DrawAimSpot(pEntity, targetBone);

	if (!Config::Visuals::ESP::PlayerESP)
		return;

	int x, y, w, h;
	if (!GetBox(pEntity, x, y, w, h))
		return;

	if (Config::Visuals::ESP::DrawBoxes)
		DrawBox(pEntity, x, y, w, h, GetPlayerColor(pEntity));

	if (Config::Visuals::ESP::DrawHealthBar)
		DrawHealthBar(pEntity, x, y, w, h);

	if (Config::Visuals::ESP::PlayerInfo::Enable)
		DrawInfo(pEntity, pInfo, x, y, w, h, index);

	if (Config::Visuals::ESP::DrawNames)
		DrawName(pEntity, pInfo.name, x, y, w, h);

	if (Config::Visuals::ESP::DrawDlights)
		DrawDLights(pEntity);

	if (Config::Visuals::ESP::BarrelESP)
		DrawBarrel(pEntity, GetPlayerColor(pEntity));

#ifdef __cyandbg
	if (Config::Debug::DrawBoneIDs)
		DrawBonesWithIds(pLocal, pEntity);
#endif
}

void DrawThrowable(C_BaseEntity* throwable, ClientClass* client)
{
	if (!Config::Visuals::ESP::GrenadeESP)
		return;

	if (Config::Visuals::ESP::VisibleCheck && !Entity::IsVisible(throwable))
		return;

	model_t* nadeModel = throwable->GetModel();

	if (!nadeModel)
		return;

	studiohdr_t* hdr = I::ModelInfo->GetStudioModel(nadeModel);

	if (!hdr)
		return;

	if (!strstr(hdr->name, "thrown") && !strstr(hdr->name, "dropped"))
		return;

	NadeInfo_t ni = ESP::GetNadeInfo(throwable);

	int x, y, w, h;
	if (!GetBox(throwable, x, y, w, h))
		return;

	DrawBox(throwable, x, y, w, h, ni.color);
	/*Render::Outline(x - 1, y - 1, w + 2, h + 2, Colors::BLACK35);
	Render::Outline(x, y, w, h, ni.color);
	Render::Outline(x + 1, y + 1, w - 2, h - 2, Colors::BLACK35);*/

	POINT txtSize = Render::GetTextSize(Fonts::smallESP, ni.name.c_str());
	Render::Text(x + ((w - txtSize.x) / 2), y - txtSize.y - 4, ni.color, Fonts::smallESP, ni.name.c_str());
}

float CSGO_Armor(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue))
		{
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}

void DrawBombTimerAndHealth(Vector origin, float timer)
{
	if (!I::csGameRules->IsBombPlanted() || !EventTrackers::BombPlanted)
		return;

	if (!Config::Visuals::Bomb::BombTimer && !Config::Visuals::Bomb::PredictHealth)
		return;

	char tmp[64];
	static int w = -1;
	static int h = -1;
	if (w < 0 || h < 0)
		I::Engine->GetScreenSize(w, h);

	if (Config::Visuals::Bomb::PredictHealth)
	{
		C_BasePlayer* pPlayer(G::LocalPlayer);
		if (!pPlayer)
			return;

		// try to find observer target if pLocal is dead
		if (!pPlayer->GetAlive())
		{
			pPlayer = I::EntList->GetClientEntityFromHandle(pPlayer->GetObserverTarget());
			if (!pPlayer)
				return;
		}

		if (pPlayer->GetAlive())
		{
			float flDistance = pPlayer->GetVecOrigin().DistTo(origin);

			float a = 450.7f;
			float b = 75.68f;
			float c = 789.2f;
			float d = ((flDistance - b) / c);
			float flDamage = a*exp(-d * d);

			int damage = (int)ceilf(CSGO_Armor(flDamage, pPlayer->GetArmor()));
			int health = pPlayer->GetHealth() - damage;

			if (health < 1)
				sprintf_s(tmp, "%.2f DEAD!", timer);// charenc
			else
				sprintf_s(tmp, "%.2f Health: %i", timer, health);// charenc
		}
	}
	else if (Config::Visuals::Bomb::BombTimer)
		sprintf_s(tmp, "%.2f", timer);// charenc


	POINT size = Render::GetTextSize(Fonts::ESP, tmp);
	Render::Text((w - size.x) / 2, 35, Colors::ESP::ESPBOMBTEXT, Fonts::ESP, tmp);
}

void DrawPlantedC4(C_BasePlayer* pEntity)
{
	if (!Config::Visuals::ESP::Enable)
		return;

	if (!I::csGameRules->IsBombPlanted() || !EventTrackers::BombPlanted)
		return;

	ESP::bombCarrierIndex = -1;
	C_PlantedC4* bomb = reinterpret_cast<C_PlantedC4*>(pEntity);
	Vector origin = pEntity->GetVecOrigin();

	float timer = bomb->GetBombTime() - I::globalVars->curtime;
	if (0.f > timer || timer > 60.f)
		return;

	DrawBombTimerAndHealth(origin, timer);

	TeamID localTeam = G::LocalPlayer->GetTeam();
	if (localTeam == TeamID::TEAM_TERRORIST && !Config::Visuals::Bomb::ShowPlantedOnT)
		return;

	if (localTeam == TeamID::TEAM_COUNTER_TERRORIST && !Config::Visuals::Bomb::ShowPlantedOnCT)
		return;

	Vector screen;
	if (!Render::WorldToScreen(origin, screen))
		return;

	char tmp[32];
	sprintf_s(tmp, "Bomb\n%.2f", timer); // charenc
	Render::Text(screen.x, screen.y, Colors::ESP::ESPBOMBTEXT, Fonts::ESP, tmp);
}

void DrawC4(C_BasePlayer* pEntity)
{
	if (!Config::Visuals::ESP::Enable)
		return;

	TeamID localTeam = G::LocalPlayer->GetTeam();
	if (localTeam == TeamID::TEAM_TERRORIST && !Config::Visuals::Bomb::ShowWildOnT)
		return;

	if (localTeam == TeamID::TEAM_COUNTER_TERRORIST && !Config::Visuals::Bomb::ShowWildOnCT)
		return;

	Vector origin = pEntity->GetVecOrigin();
	if (origin.Length() < 0.1f)
		return;

	Vector screen;
	if (I::csGameRules->IsBombDropped() && Render::WorldToScreen(origin, screen))
		Render::Text(screen.x, screen.y, Colors::ESP::ESPBOMBTEXT, Fonts::ESP, "BOMB");
}

void DrawChicken(C_BasePlayer* pEntity)
{
	if (!Config::Visuals::ESP::ChickenESP)
		return;

	int x, y, w, h;
	if (!GetBox(pEntity, x, y, w, h))
		return;

	DrawBox(pEntity, x, y, w, h, Colors::ESP::CHICKEN);
	DrawName(pEntity, "Chicken", x, y, w, h);
}

void DrawDrop(C_BasePlayer* pEntity, ClientClass* pClass)
{
	if (!Config::Visuals::ESP::DropESP)
		return;

	C_BaseCombatWeapon* weapon = reinterpret_cast<C_BaseCombatWeapon*>(pEntity);

	if (!weapon)
		return;

	Vector origin = weapon->GetVecOrigin();
	std::string modelName = ESP::FriendlyWeaponName((int)*weapon->GetItemDefinitionIndex());
	//HANDLE owner = weapon->GetOwner();

	if (origin.Length() < 1)
		return;

	int x, y, w, h;
	if (!GetBox(pEntity, x, y, w, h))
		return;

	//std::string modelName = FriendlyWeaponName(*weapon->GetItemDefinitionIndex());

	if (weapon->GetAmmo() > 0)
		modelName += " | " + std::to_string(weapon->GetAmmo());

	DrawBox(pEntity, x, y, w, h, Colors::ESP::DROPS);
	DrawName(pEntity, modelName.c_str(), x, y, w, h, true);
}

void ESP::PaintTraverse()
{
	if (!I::Engine->IsInGame() || !G::LocalPlayer)
		return;

	DrawSounds();
	DrawSpreadCircle();

	if (Config::Visuals::ESP::HitMarkers)
	{
		static DamageListener* hits = new DamageListener();
		hits->PaintTraverse();
	}

	static GameEventListener* sounds = new GameEventListener();
	sounds->PaintTraverse();

	if (!Config::Visuals::ESP::Enable)
		return;

	for (int i = 0; i < I::EntList->GetHighestEntityIndex(); i++)
	{
		C_BasePlayer* pEntity = I::EntList->GetClientEntity(i);

		if (!pEntity)
			continue;

		IEngineClient::player_info_t pInfo;
		ClientClass* pClass = pEntity->GetClientClass();

		if (pEntity->GetAlive() && I::Engine->GetPlayerInfo(i, &pInfo))
			DrawPlayer(G::LocalPlayer, pEntity, pInfo, i);
		else if (pClass->m_ClassID == EClassIds::CPlantedC4)
			DrawPlantedC4(pEntity);
		else if (pClass->m_ClassID == EClassIds::CC4)
			DrawC4(pEntity);
		else if (pClass->m_ClassID == EClassIds::CChicken)
			DrawChicken(pEntity);
		else if (pClass->m_ClassID != EClassIds::CBaseWeaponWorldModel && (strstr(pClass->m_pNetworkName, "Weapon")) || pClass->m_ClassID == EClassIds::CDEagle || pClass->m_ClassID == EClassIds::CAK47)
			DrawDrop(pEntity, pClass);
		else if (strstr(pClass->m_pNetworkName, "Projectile"))
			DrawThrowable(pEntity, pClass);
	}
}