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
credit: Synraw @ unknowncheats.me
*/

#include "skinchanger.h"

#include "menu.h"
#include "path.h"
#include "globals.h"
#include "tinyxml.h"
#include "utilities.h"
#include "definitions.h"

#include "stickers.h"
#include "settings.h"

// skins knives config
bool Config::Skins::Enable = true;
bool Config::Skins::Knives::Enable = true;
int Config::Skins::Knives::TKnife = 4;
int Config::Skins::Knives::CTKnife = 5;
int Config::Skins::Knives::TKnifeSkin = 3;
int Config::Skins::Knives::CTKnifeSkin = 6;
std::vector<std::string> Config::Skins::Knives::knifeOptions = { "Default", "Flip", "Gut", "Bayonet", "M9 Bayonet", "Karambit", "Shadow Daggers", "Huntsman", "Bowie", "Falcion", "Butterfly" };
std::vector<std::string> Config::Skins::Knives::skinOptions = { "Default", "Fade", "Emerald", "Sapphire", "Ruby", "Slaughter", "Tiger Tooth", "Crimson Web", "Marble Fade", "Lore", "Forest DDPAT", "Night", "Blue Steel", "Stained", "Case Hardened", "Safari Mesh", "Boreal Forest", "Ultraviolet", "Urban Masked", "Damascus Steel", "Scorched", "Rust Coat" };

int Config::Skins::Weapons::SelectedSkin = 0;
int Config::Skins::Weapons::SelectedWeapon = 0;
float Config::Skins::Weapons::Float = 0.f;
bool Config::Skins::Weapons::StatTrak = false;

int Config::Skins::Weapons::sticker1 = 0;
int Config::Skins::Weapons::sticker2 = 0;
int Config::Skins::Weapons::sticker3 = 0;
int Config::Skins::Weapons::sticker4 = 0;


std::unordered_map<int, SkinObject_t> SkinChanger::skin_map;

SkinObject_t::SkinObject_t(int ind, int nSeed, int paint, float fl, bool stat, int trak, int quality, const char* n, int s1, int s2, int s3, int s4)
{
	index = ind;
	seed = nSeed;
	iPaintKit = paint;
	flFloat = fl;
	bStatTrak = stat;
	iStatTrak = trak;
	iEntityQuality = quality;
	name = n ? std::string(n) : "";

	iStickerOne = s1;
	iStickerTwo = s2;
	iStickerThree = s3;
	iStickerFour = s4;
}

void SkinObject_t::AddStickers(int one, int two, int three, int four)
{
	iStickerOne = one;
	iStickerTwo = two;
	iStickerThree = three;
	iStickerFour = four;
}

int SkinObject_t::GetStickerID(int slot)
{
	if (slot == 0)
		return iStickerOne;
	if (slot == 1)
		return iStickerTwo;
	if (slot == 2)
		return iStickerThree;
	if (slot == 3)
		return iStickerFour;

	return 0;
}

AttribItem_t SkinObject_t::ToAttribItem()
{
	AttribItem_t item = AttribItem_t();

	item.itemDefinitionIndex = ItemDefinitionIndex::GLOVE_SPECIALIST;
	item.fallbackPaintKit = iPaintKit;
	item.fallbackWear = flFloat;
	item.fallbackSeed = seed;
	item.fallbackStatTrak = bStatTrak ? iStatTrak : -1;
	item.entityQuality = iEntityQuality;
	item.customName = name;

	return item;
}

ItemDefinitionIndex getKnifeIndex()
{
	int index = G::LocalPlayer->GetTeam() == TeamID::TEAM_TERRORIST ? Config::Skins::Knives::TKnife : Config::Skins::Knives::CTKnife;

	switch (index)
	{
	case 0:
		return G::LocalPlayer->GetTeam() == TeamID::TEAM_TERRORIST ? ItemDefinitionIndex::WEAPON_KNIFE_T : ItemDefinitionIndex::WEAPON_KNIFE;
	case 1:
		return ItemDefinitionIndex::WEAPON_KNIFE_FLIP;
	case 2:
		return ItemDefinitionIndex::WEAPON_KNIFE_GUT;
	case 3:
		return ItemDefinitionIndex::WEAPON_KNIFE_BAYONET;
	case 4:
		return ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET;
	case 5:
		return ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT;
	case 6:
		return ItemDefinitionIndex::WEAPON_KNIFE_PUSH;
	case 7:
		return ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL;
	case 8:
		return ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE;
	case 9:
		return ItemDefinitionIndex::WEAPON_KNIFE_FALCHION;
	case 10:
		return ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY;
	}

	return ItemDefinitionIndex::INVALID;
}

const char* GetModelByItemIndex(ItemDefinitionIndex id)
{
	switch (id)
	{
	case ItemDefinitionIndex::WEAPON_KNIFE_GUT:
		return "models/weapons/v_knife_gut.mdl";
	case ItemDefinitionIndex::WEAPON_KNIFE_FLIP:
		return "models/weapons/v_knife_flip.mdl";
	case ItemDefinitionIndex::WEAPON_KNIFE_BAYONET:
		return "models/weapons/v_knife_bayonet.mdl";
	case ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:
		return "models/weapons/v_knife_m9_bay.mdl";
	case ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT:
		return "models/weapons/v_knife_karam.mdl";
	case ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL:
		return "models/weapons/v_knife_tactical.mdl";
	case ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY:
		return "models/weapons/v_knife_butterfly.mdl";
	case ItemDefinitionIndex::WEAPON_KNIFE_FALCHION:
		return "models/weapons/v_knife_falchion_advanced.mdl";
	case ItemDefinitionIndex::WEAPON_KNIFE_PUSH:
		return "models/weapons/v_knife_push.mdl";
	default:
		return NULL;
	}
}

int getKnifePaintKit()
{
	int selection = G::LocalPlayer->GetTeam() == TeamID::TEAM_TERRORIST ? Config::Skins::Knives::TKnifeSkin : Config::Skins::Knives::CTKnifeSkin;

	switch (selection)
	{
	case 0:
		break;
	case 1:
		return 38;
	case 2:
		return 568;
	case 3:
		return 416;
	case 4:
		return 415;
	case 5:
		return 59;
	case 6:
		return 409;
	case 7:
		return 12;
	case 8:
		return 413;
	case 9:
		switch (getKnifeIndex())
		{
		case ItemDefinitionIndex::WEAPON_KNIFE_BAYONET:
			return 558;
		case ItemDefinitionIndex::WEAPON_KNIFE_FLIP:
			return 559;
		case ItemDefinitionIndex::WEAPON_KNIFE_GUT:
			return 560;
		case ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT:
			return 561;
		case ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:
			return 562;
		}
		break;
	case 10:
		return 5;
	case 11:
		return 40;
	case 12:
		return 42;
	case 13:
		return 43;
	case 14:
		return 44;
	case 15:
		return 72;
	case 16:
		return 77;
	case 17:
		return 98;
	case 18:
		return 143;
	case 19:
		return 410;
	case 20:
		return 175;
	case 21:
		return 414;
	}
	return -1;
}

SkinObject_t GetKnifeObject()
{
	ItemDefinitionIndex idx = G::LocalPlayer->GetTeam() == TeamID::TEAM_TERRORIST ? ItemDefinitionIndex::WEAPON_KNIFE_T : ItemDefinitionIndex::WEAPON_KNIFE;
	SkinObject_t skin = *SkinChanger::GetSkinObjectByIndex(idx);

	skin.index = (int)getKnifeIndex();
	skin.iPaintKit = getKnifePaintKit();

	return skin;
}

void SkinChanger::FrameStageNotify()
{
	if (G::CurStage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		return;

	if (!G::LocalPlayer->GetAlive())
		return;

	IEngineClient::player_info_t local_info;
	if (!I::Engine->GetPlayerInfo(I::Engine->GetLocalPlayer(), &local_info))
		return;

	HANDLE* weapons = G::LocalPlayer->GetWeapons();

	for (size_t i = 0; weapons[i] != (HANDLE)0xFFFFFFFF; i++)
	{
		C_BaseCombatWeapon* weapon = (C_BaseCombatWeapon*)I::EntList->GetClientEntityFromHandle(weapons[i]);

		if (!weapon)
			continue;

		ItemDefinitionIndex idx = *weapon->GetItemDefinitionIndex();
		if (idx == ItemDefinitionIndex::WEAPON_KNIFE || idx == ItemDefinitionIndex::WEAPON_KNIFE_T || (int)idx > 499)
		{
			if (!Config::Skins::Knives::Enable)
				return;

			idx = G::LocalPlayer->GetTeam() == TeamID::TEAM_TERRORIST ? ItemDefinitionIndex::WEAPON_KNIFE_T : ItemDefinitionIndex::WEAPON_KNIFE;
			SkinObject_t* skin = GetSkinObjectByIndex(idx);

			if (const char* modelFilename = GetModelByItemIndex(getKnifeIndex()))
				*weapon->GetModelIndex() = I::ModelInfo->GetModelIndex(modelFilename);

			// apply custom model
			*weapon->GetItemDefinitionIndex() = getKnifeIndex();

			*weapon->GetFallbackPaintKit() = getKnifePaintKit();
			*weapon->GetEntityQuality() = skin->iEntityQuality;
			*weapon->GetFallbackSeed() = skin->seed;
			*weapon->GetFallbackStatTrak() = skin->bStatTrak ? skin->iStatTrak : -1;
			*weapon->GetFallbackWear() = skin->flFloat;

			if (skin->name.length() > 0)
				sprintf_s(weapon->GetCustomName(), 32, "%s", skin->name.c_str());

			*weapon->GetItemIDHigh() = -1;
		}
		else
		{
			if (!Config::Skins::Enable)
				return;

			SkinObject_t* skin = GetSkinObjectByIndex(idx);

			if (!skin)
				continue;

			//if (const char* modelFilename = GetModelByItemIndex(idx))
			//	*weapon->GetModelIndex() = I::ModelInfo->GetModelIndex(modelFilename);

			// apply custom skin
			*weapon->GetFallbackPaintKit() = skin->iPaintKit;
			*weapon->GetEntityQuality() = skin->iEntityQuality;
			*weapon->GetFallbackSeed() = skin->seed;
			*weapon->GetFallbackStatTrak() = skin->bStatTrak ? skin->iStatTrak : -1;
			*weapon->GetFallbackWear() = skin->flFloat;

			Stickers::ApplyStickerHooks(weapon);

			if (skin->name.length() > 0)
				sprintf_s(weapon->GetCustomName(), 32, "%s", skin->name.c_str());

			*weapon->GetItemIDHigh() = -1;

			*weapon->GetAccountID() = local_info.xuidlow;
		}
	}
}

SkinObject_t* SkinChanger::GetSkinObjectByIndex(ItemDefinitionIndex index)
{
	return &skin_map[(int)index];
}

std::string SkinChanger::GetSkinNameByIndex(int index)
{
	switch (index)
	{
	case (int)ItemDefinitionIndex::WEAPON_DEAGLE:
		return "WEAPON_DEAGLE";
	case (int)ItemDefinitionIndex::WEAPON_ELITE:
		return "WEAPON_DUAL";
	case (int)ItemDefinitionIndex::WEAPON_FIVESEVEN:
		return "WEAPON_FIVESEVEN";
	case (int)ItemDefinitionIndex::WEAPON_GLOCK:
		return "WEAPON_GLOCK";
	case (int)ItemDefinitionIndex::WEAPON_AK47:
		return "WEAPON_AK47";
	case (int)ItemDefinitionIndex::WEAPON_AUG:
		return "WEAPON_AUG";
	case (int)ItemDefinitionIndex::WEAPON_AWP:
		return "WEAPON_AWP";
	case (int)ItemDefinitionIndex::WEAPON_FAMAS:
		return "WEAPON_FAMAS";
	case (int)ItemDefinitionIndex::WEAPON_G3SG1:
		return "WEAPON_G3SG1";
	case (int)ItemDefinitionIndex::WEAPON_GALILAR:
		return "WEAPON_GALIL";
	case (int)ItemDefinitionIndex::WEAPON_M249:
		return "WEAPON_M249";
	case (int)ItemDefinitionIndex::WEAPON_M4A1:
		return "WEAPON_M4A4";
	case (int)ItemDefinitionIndex::WEAPON_MAC10:
		return "WEAPON_MAC10";
	case (int)ItemDefinitionIndex::WEAPON_P90:
		return "WEAPON_P90";
	case (int)ItemDefinitionIndex::WEAPON_UMP45:
		return "WEAPON_UMP";
	case (int)ItemDefinitionIndex::WEAPON_XM1014:
		return "WEAPON_XM1014";
	case (int)ItemDefinitionIndex::WEAPON_BIZON:
		return "WEAPON_BIZON";
	case (int)ItemDefinitionIndex::WEAPON_MAG7:
		return "WEAPON_MAG7";
	case (int)ItemDefinitionIndex::WEAPON_NEGEV:
		return "WEAPON_NEGEV";
	case (int)ItemDefinitionIndex::WEAPON_SAWEDOFF:
		return "WEAPON_SAWEDOFF";
	case (int)ItemDefinitionIndex::WEAPON_TEC9:
		return "WEAPON_TEC9";
	case (int)ItemDefinitionIndex::WEAPON_TASER:
		return "WEAPON_TASER";
	case (int)ItemDefinitionIndex::WEAPON_HKP2000:
		return "WEAPON_HKP2000";
	case (int)ItemDefinitionIndex::WEAPON_MP7:
		return "WEAPON_MP7";
	case (int)ItemDefinitionIndex::WEAPON_MP9:
		return "WEAPON_MP9";
	case (int)ItemDefinitionIndex::WEAPON_NOVA:
		return "WEAPON_NOVA";
	case (int)ItemDefinitionIndex::WEAPON_P250:
		return "WEAPON_P250";
	case (int)ItemDefinitionIndex::WEAPON_SCAR20:
		return "WEAPON_SCAR20";
	case (int)ItemDefinitionIndex::WEAPON_SG556:
		return "WEAPON_SG556";
	case (int)ItemDefinitionIndex::WEAPON_SSG08:
		return "WEAPON_SSG08";
	case (int)ItemDefinitionIndex::WEAPON_KNIFE:
		return "WEAPON_KNIFE_CT";
	case (int)ItemDefinitionIndex::WEAPON_FLASHBANG:
		return "WEAPON_FLASHBANG";
	case (int)ItemDefinitionIndex::WEAPON_HEGRENADE:
		return "WEAPON_HEGRENADE";
	case (int)ItemDefinitionIndex::WEAPON_SMOKEGRENADE:
		return "WEAPON_SMOKE";
	case (int)ItemDefinitionIndex::WEAPON_MOLOTOV:
		return "WEAPON_T_MOLOTOV";
	case (int)ItemDefinitionIndex::WEAPON_DECOY:
		return "WEAPON_DECOY";
	case (int)ItemDefinitionIndex::WEAPON_C4:
		return "WEAPON_C4";
	case (int)ItemDefinitionIndex::WEAPON_KNIFE_T:
		return "WEAPON_KNIFE_T";
	case (int)ItemDefinitionIndex::WEAPON_M4A1_SILENCER:
		return "WEAPON_M4A1";
	case (int)ItemDefinitionIndex::WEAPON_USP_SILENCER:
		return "WEAPON_USP";
	case (int)ItemDefinitionIndex::WEAPON_CZ75A:
		return "WEAPON_CZ75";
	case (int)ItemDefinitionIndex::WEAPON_KNIFE_BAYONET:
		return "WEAPON_KNIFE_BAYONET";
	case (int)ItemDefinitionIndex::WEAPON_KNIFE_FLIP:
		return "WEAPON_KNIFE_FLIP";
	case (int)ItemDefinitionIndex::WEAPON_KNIFE_GUT:
		return "WEAPON_KNIFE_GUT";
	case (int)ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT:
		return "WEAPON_KNIFE_KARAMBIT";
	case (int)ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:
		return "WEAPON_KNIFE_M9";
	case (int)ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL:
		return "WEAPON_KNIFE_HUNTSMAN";
	case (int)ItemDefinitionIndex::WEAPON_KNIFE_FALCHION:
		return "WEAPON_KNIFE_FALCHION";
	case (int)ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY:
		return "WEAPON_KNIFE_BUTTERFLY";
	case (int)ItemDefinitionIndex::WEAPON_KNIFE_PUSH:
		return "WEAPON_KNIFE_DAGGERS";
	case (int)ItemDefinitionIndex::GLOVE_CT_SIDE:
		return "CT_GLOVES";
	case (int)ItemDefinitionIndex::GLOVE_T_SIDE:
		return "T_GLOVES";
	}

	return "#UNDEF_" + std::to_string((int)index);
}

void SkinChanger::SaveConfig()
{
	std::string Filename = Path::GetXMLConfig("skins");

	// Create a whole new document and we'll just save over top of the old one
	tinyxml2::XMLDocument Doc;

	tinyxml2::XMLElement *Root = Doc.NewElement("SKINS");
	Doc.LinkEndChild(Root);

	// If the window has some tabs..
	if (!Root && skin_map.size() < 1)
	{
		Utilities::Log("Failed to save skin config: No skins in map");
		return;
	}

	for (auto i = skin_map.begin(); i != skin_map.end(); i++)
	{
		if (GetSkinNameByIndex(i->first).substr(1, 5) == "UNDEF")
			continue;

		// Add a new section for this gun
		tinyxml2::XMLElement *WeaponElement = Doc.NewElement(GetSkinNameByIndex(i->first).c_str());
		Root->LinkEndChild(WeaponElement);

		// Now we add all the Skin info
		if (WeaponElement && i->second.iPaintKit > 0)
		{
			tinyxml2::XMLElement *PaintKit = Doc.NewElement("PAINTKIT");
			PaintKit->SetText(i->second.iPaintKit);
			WeaponElement->LinkEndChild(PaintKit);

			tinyxml2::XMLElement *Seed = Doc.NewElement("SEED");
			Seed->SetText(i->second.seed);
			WeaponElement->LinkEndChild(Seed);

			tinyxml2::XMLElement *Float = Doc.NewElement("FLOAT");
			Float->SetText(i->second.flFloat);
			WeaponElement->LinkEndChild(Float);

			tinyxml2::XMLElement *bStatTrak = Doc.NewElement("bSTATTRAK");
			bStatTrak->SetText(i->second.bStatTrak);
			WeaponElement->LinkEndChild(bStatTrak);

			tinyxml2::XMLElement *iStatTrak = Doc.NewElement("iSTATTRAK");
			iStatTrak->SetText(i->second.iStatTrak);
			WeaponElement->LinkEndChild(iStatTrak);

			tinyxml2::XMLElement *EntityQuality = Doc.NewElement("ENTITYQUALITY");
			EntityQuality->SetText(i->second.iEntityQuality);
			WeaponElement->LinkEndChild(EntityQuality);

			tinyxml2::XMLElement *Name = Doc.NewElement("NAME");
			Name->SetText(i->second.name.c_str());
			WeaponElement->LinkEndChild(Name);

			tinyxml2::XMLElement *sticker1 = Doc.NewElement("sticker1");
			sticker1->SetText(i->second.iStickerOne);
			WeaponElement->LinkEndChild(sticker1);

			tinyxml2::XMLElement *sticker2 = Doc.NewElement("sticker2");
			sticker2->SetText(i->second.iStickerTwo);
			WeaponElement->LinkEndChild(sticker2);

			tinyxml2::XMLElement *sticker3 = Doc.NewElement("sticker3");
			sticker3->SetText(i->second.iStickerThree);
			WeaponElement->LinkEndChild(sticker3);

			tinyxml2::XMLElement *sticker4 = Doc.NewElement("sticker4");
			sticker4->SetText(i->second.iStickerFour);
			WeaponElement->LinkEndChild(sticker4);
		}
	}

	//Save the file
	if (!Doc.NoChildren() && !Root->NoChildren() && Doc.SaveFile(Filename.c_str()) != tinyxml2::XML_NO_ERROR)
		Utilities::Error("Failed to save skin config!");
	else
		Utilities::Log("Successfully saved skin config.");
}

void SkinChanger::LoadConfig()
{
	tinyxml2::XMLDocument Doc;

	if (Doc.LoadFile(Path::GetXMLConfig("skins").c_str()) != tinyxml2::XML_SUCCESS)
	{
		Utilities::Error("Failed to load skin config!");
		return;
	}

	tinyxml2::XMLElement *Root = Doc.RootElement();

	// The root "skins" element
	if (!Root)
		return;

	skin_map.clear();

	for (int i = 0; i < 64; i++)
	{
		tinyxml2::XMLElement *SkinElement = Root->FirstChildElement(GetSkinNameByIndex(i).c_str());

		if (!SkinElement)
			continue;

		tinyxml2::XMLElement *PaintElement = SkinElement->FirstChildElement("PAINTKIT");
		tinyxml2::XMLElement *SeedElement = SkinElement->FirstChildElement("SEED");
		tinyxml2::XMLElement *FloatElement = SkinElement->FirstChildElement("FLOAT");
		tinyxml2::XMLElement *bStatTrakElement = SkinElement->FirstChildElement("bSTATTRAK");
		tinyxml2::XMLElement *iStatTrakElement = SkinElement->FirstChildElement("iSTATTRAK");
		tinyxml2::XMLElement *QualityElement = SkinElement->FirstChildElement("ENTITYQUALITY");
		tinyxml2::XMLElement *NameElement = SkinElement->FirstChildElement("NAME");

		int paint = PaintElement ? atoi(PaintElement->GetText()) : 1;
		int seed = SeedElement ? atoi(SeedElement->GetText()) : 1;
		float fl = FloatElement ? atof(FloatElement->GetText()) : 0.0001f;
		bool bStat = bStatTrakElement ? bStatTrakElement->GetText()[0] == '1' : 0;
		int iStat = iStatTrakElement ? atoi(iStatTrakElement->GetText()) : 0;
		int qual = QualityElement ? atoi(QualityElement->GetText()) : 0;
		const char* name = NameElement ? NameElement->GetText() : "";

		tinyxml2::XMLElement* sticker1 = SkinElement->FirstChildElement("sticker1");
		tinyxml2::XMLElement* sticker2 = SkinElement->FirstChildElement("sticker2");
		tinyxml2::XMLElement* sticker3 = SkinElement->FirstChildElement("sticker3");
		tinyxml2::XMLElement* sticker4 = SkinElement->FirstChildElement("sticker4");

		int s1 = sticker1 ? atoi(sticker1->GetText()) : 0;
		int s2 = sticker2 ? atoi(sticker2->GetText()) : 0;
		int s3 = sticker3 ? atoi(sticker3->GetText()) : 0;
		int s4 = sticker4 ? atoi(sticker4->GetText()) : 0;

		skin_map[i] = SkinObject_t(i, seed, paint, fl, bStat, iStat, qual, name);
		skin_map[i].AddStickers(s1, s2, s3, s4);
	}
}

void SkinChanger::SaveMenuToMap()
{
	SkinObject_t temp = SkinObject_t(Config::Skins::Weapons::SelectedWeapon, 1, Config::Skins::Weapons::SelectedSkin, Config::Skins::Weapons::Float, Config::Skins::Weapons::StatTrak, 666, 0);
	temp.AddStickers(Config::Skins::Weapons::sticker1, Config::Skins::Weapons::sticker2, Config::Skins::Weapons::sticker3, Config::Skins::Weapons::sticker4);

	SkinChanger::skin_map[Config::Skins::Weapons::SelectedWeapon] = temp;

	Utilities::Log("Skin updated.");

	ForceRefresh();
}

void SkinChanger::ForceRefresh()
{
	if (!I::Engine->IsInGame())
		return;

	Utilities::Warn("Forcing full refresh!");
	I::ClientState->ForceFullUpdate();
}

void SkinChanger::Hooked_RecvProxy_Viewmodel(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	int iModel = pData->m_Value.m_Int;

	int default_t = I::ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int default_ct = I::ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");

	if (G::LocalPlayer && G::LocalPlayer->GetLifeState() == LIFE_ALIVE)
		if (iModel == default_t || iModel == default_ct)
			iModel = I::ModelInfo->GetModelIndex(GetModelByItemIndex(getKnifeIndex()));

	*(int*)pOut = iModel;
}

inline const int RandomSequence(int low, int high) {
	return (rand() % (high - low + 1) + low);
}

RecvVarProxyFn ogSequence;

void __cdecl hkSequenceProxyFn(const CRecvProxyData* proxy_data_const, void* entity, void* output) {
	// Get the original proxy function used by the game and store it in a static variable for later usage.
	static RecvVarProxyFn oSequenceProxyFn = ogSequence;

	// Don't go any further if we're not alive.
	if (!G::LocalPlayer || G::LocalPlayer->GetLifeState() != LIFE_ALIVE)
		return oSequenceProxyFn(proxy_data_const, entity, output);

	// check if we areent holding a knife and return orig if we are.
	if (!G::ActiveWeapon)
		return oSequenceProxyFn(proxy_data_const, entity, output);

	auto client = G::ActiveWeapon->GetClientClass();
	if (!client)
		return oSequenceProxyFn(proxy_data_const, entity, output);

	if (client->m_ClassID != EClassIds::CKnife)
		return oSequenceProxyFn(proxy_data_const, entity, output);

	// Remove the constness from the proxy data allowing us to make changes.
	CRecvProxyData* proxy_data = const_cast<CRecvProxyData*>(proxy_data_const);

	// Cast the view model entity to the correct class.
	C_BaseViewModel* viewmodel = static_cast<C_BaseViewModel*>(entity);

	if (viewmodel && viewmodel->GetOwner()) {
		// Ensure the owner handle is valid.
		if (viewmodel->GetOwner() == (HANDLE)0xFFFFFF)
			return oSequenceProxyFn(proxy_data_const, entity, output);

		// Convert the view model owner handle to a player pointer.
		C_BasePlayer* owner = static_cast<C_BasePlayer*>(I::EntList->GetClientEntityFromHandle(viewmodel->GetOwner()));

		// Compare the owner entity of this view model to the local player entity.
		if (owner == G::LocalPlayer) {
			// Get the filename of the current view model.
			//const model_t* knife_model = I::ModelInfo->GetModel(*viewmodel->GetModelIndex());
			const char* model_filename = GetModelByItemIndex(getKnifeIndex());

			// Set the current sequence.
			int m_nSequence = proxy_data->m_Value.m_Int;

			// Check the model filename to determine which fixes to use.
			if (!strcmp(model_filename, "models/weapons/v_knife_butterfly.mdl")) {
				// Fix animations for the Butterfly Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03); break;
				default:
					m_nSequence++;
				}
			}
			else if (!strcmp(model_filename, "models/weapons/v_knife_falchion_advanced.mdl")) {
				// Fix animations for the Falchion Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomSequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP); break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomSequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02); break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (!strcmp(model_filename, "models/weapons/v_knife_push.mdl")) {
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomSequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5); break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomSequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1); break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (!strcmp(model_filename, "models/weapons/v_knife_survival_bowie.mdl")) {
				// Fix animations for the Bowie Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1; break;
				default:
					m_nSequence--;
				}
			}

			// Set the fixed sequence.
			proxy_data->m_Value.m_Int = m_nSequence;
		}
	}

	// Call the original function with our edited data.
	oSequenceProxyFn(proxy_data, entity, output);
}

void SkinChanger::Initialize()
{
	NetVarManager::HookProp("DT_BaseViewModel", "m_nModelIndex", Hooked_RecvProxy_Viewmodel);

	// TODO fix this man its crashing unpredictably
	NetVarManager::HookProp("DT_BaseViewModel", "m_nSequence", hkSequenceProxyFn, ogSequence);

	LoadConfig();
}