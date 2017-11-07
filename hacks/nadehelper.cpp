#include "nadehelper.h"

#include "esp.h"
#include "path.h"
#include "math.h"
#include "fonts.h"
#include "entity.h"
#include "render.h"
#include "dlights.h"
#include "tinyxml.h"
#include "settings.h"
#include "globals.h"

#include <unordered_map>

bool Config::Misc::GrenadeHelper::SmokeHelper = true;
bool Config::Misc::GrenadeHelper::MollyHelper = true;
bool Config::Misc::GrenadeHelper::FlashHelper = false;
bool Config::Misc::GrenadeHelper::DlightOnPosition = true;


// [MAP][NADE] -> vector of NadeHelper_t
NadeHelper::nadeArray NadeHelper::throws = {};

NadeInfo_t currentNadeInfo = { Color(0, 0, 0), "INVALID" };
std::string NadeHelper::currentMapName = "";

// pos (1148.2, -1184, -141.5) ang (-44.29, -165.28)
// TODO configs by map
// TODO configs for all this shit

void PrintEntryToConsole(Vector pos, QAngle angle)
{
	char tmp[180];
	sprintf_s(tmp, "Position: (%f, %f, %f) ViewAngles: (%f, %f, %f)", pos.x, pos.y, pos.z, angle.x, angle.y, angle.z);

	Utilities::Log(tmp);
}

void NadeHelper::UpdateMapName()
{
	currentMapName = I::Engine->GetLevelNameShort();
}
 
NadeInfo_t NadeInfoFromID(ItemDefinitionIndex index)
{
	switch (index)
	{
	case ItemDefinitionIndex::WEAPON_FLASHBANG:
		return { Colors::Grenades::FLASH, "FLASH" };
	case ItemDefinitionIndex::WEAPON_HEGRENADE:
		return { Colors::Grenades::FRAG, "FRAG" };
	case ItemDefinitionIndex::WEAPON_SMOKEGRENADE:
		return { Colors::Grenades::SMOKE, "SMOKE" };
	case ItemDefinitionIndex::WEAPON_DECOY:
		return { Colors::Grenades::DECOY, "DECOY" };
	case ItemDefinitionIndex::WEAPON_MOLOTOV:
	case ItemDefinitionIndex::WEAPON_INCGRENADE:
		return { Colors::Grenades::MOLLY, "MOLOTOV" };
	default:
		return { Color(0, 0, 0), "INVALID" };
	}
}

void NadeHelper::PaintTraverse()
{
	if (currentMapName == "")
		UpdateMapName();
	// maybe make it a try getmap name method, if returns false, return;
	if (currentMapName == "")
		return;

	if (!G::ActiveWeapon)
		return;

	ItemDefinitionIndex itemid = *G::ActiveWeapon->GetItemDefinitionIndex();
	currentNadeInfo = NadeInfoFromID(itemid);
	// todo this is a shitty way to filter them out.
	// should have each nadehelper type in a different vector,
	// only iterate through the enabled onees
	if (itemid == ItemDefinitionIndex::WEAPON_SMOKEGRENADE)
	{
		if (!Config::Misc::GrenadeHelper::SmokeHelper)
			return;
	}
	else if (itemid == ItemDefinitionIndex::WEAPON_MOLOTOV || itemid == ItemDefinitionIndex::WEAPON_INCGRENADE)
	{
		if (!Config::Misc::GrenadeHelper::MollyHelper)
			return;
	}
	else if (itemid == ItemDefinitionIndex::WEAPON_FLASHBANG)
	{
		if (!Config::Misc::GrenadeHelper::FlashHelper)
			return;
	}

	for (NadeHelper_t nh : throws[currentMapName][currentNadeInfo.name])
	{
		nh.visible = Entity::IsPositionVisible(G::LocalPlayer, nh.position);

		if (!nh.visible)
			continue;

		Vector vScreen;
		if (!Render::WorldToScreen(nh.position, vScreen))
			continue;

		Render::Text(vScreen.x, vScreen.y, currentNadeInfo.color, Fonts::ESP, nh.name.c_str());
		if (Config::Misc::GrenadeHelper::DlightOnPosition)
			DLights::AllocDlight(currentNadeInfo.color, 3.f, 40.f, 0.f, 1337 + rand() % 1000, Vector(), nh.position);
	}
}

void NadeHelper::CreateMove()
{
	if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000)
		PrintEntryToConsole(G::LocalPlayer->GetVecOrigin(), *G::LocalPlayer->GetEyeAngles());

	if (currentMapName == "" || currentNadeInfo.name == "INVALID")
		return;

	if (!G::ActiveWeapon)
		return;

	Vector myPos = G::LocalPlayer->GetVecOrigin(); // +localplayer->GetVecViewOffset();

	for (NadeHelper_t nh : throws[currentMapName][currentNadeInfo.name])
	{
		if (Vector(myPos - nh.position).Length() < 2.5f)
			if (G::UserCmd->buttons & IN_ATTACK || G::UserCmd->buttons & IN_ATTACK2)
				if (G::UserCmd->viewangles != nh.angle)
				{
					QAngle delta = (nh.angle - G::UserCmd->viewangles);

					Math::NormalizeAngles(delta);

					if (delta.Length() > 100.f)
						return;

					G::UserCmd->viewangles += delta / 6;
					I::Engine->SetViewAngles(G::UserCmd->viewangles);
				}
	}
}

Vector LoadVectorFromElement(tinyxml2::XMLElement* elem)
{
	float x = atof(elem->FirstChildElement("X")->GetText());
	float y = atof(elem->FirstChildElement("Y")->GetText());
	float z = atof(elem->FirstChildElement("Z")->GetText());

	return Vector(x, y, z);
}

void NadeHelper::LoadThrows()
{
	std::string filename = Path::GetXMLConfig("nadehelper");

	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	if (document->LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		Utilities::Error("Failed to load config from " + filename);
		return;
	}
	else
		Utilities::Log("Loading config file from " + filename);

	throws = {};
	// ITERATE THRU MAPS
	for (auto map = document->FirstChildElement(); map; map = map->NextSiblingElement())
	{
		throws[map->Name()] = {};
		// ITERATE THRU NADE TYPES
		for (auto nade = map->FirstChildElement(); nade; nade = nade->NextSiblingElement())
		{
			throws[map->Name()][nade->Name()] = {};
			// ITERATE THRU THROWS
			for (auto toss = nade->FirstChildElement(); toss; toss = toss->NextSiblingElement())
			{
				Vector pos = LoadVectorFromElement(toss->FirstChildElement("POSITION"));
				Vector ang = LoadVectorFromElement(toss->FirstChildElement("ANGLE"));
				ang.z = 0.f; // not necessary but better to do once at load than every tick (maybe lol)

				throws[map->Name()][nade->Name()].push_back(NadeHelper_t(toss->Name(), pos, ang));

				if (toss == nade->LastChildElement())
					break;
			}

			if (nade == map->LastChildElement())
				break;
		}

		if (map == document->LastChildElement())
			break;
	}
}

NadeHelper::NadeHelper_t::NadeHelper_t(std::string nombre, Vector pos, QAngle ang)
{
	name = nombre;
	position = pos;
	angle = ang;
}
