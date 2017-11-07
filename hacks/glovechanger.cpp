#include "settings.h"
#include "globals.h"
#include "glovechanger.h"

#include "skinchanger.h"

bool Config::Skins::Gloves::Enable = true;
int Config::Skins::Gloves::TGlove = 0;
int Config::Skins::Gloves::CTGlove = 0;
std::vector<std::string> Config::Skins::Gloves::GloveOptions = { "Bloodhound", "Sporty", "Slick", "Leather", "Motorcycle", "Specialist" };
std::vector<int> GloveIndices = { 5027, 5030, 5031, 5032, 5033, 5034 };
std::vector<std::vector<std::string>> Config::Skins::Gloves::SkinsByGlove = { { "Black Silver", "Snakeskin Brass", "Metallic", "Guerrilla" }, { "Light Blue", "Military", "Purple", "Green" }, { "Black", "Military", "Red", "Snakeskin Yellow" }, { "Red Slaughter", "Orange Camo" }, { "Basic Black", "Mint Triangle", "Mono Boom", "Triangle Blue" }, { "Green Camo", "Red Kimono", "Emerald Web", "Orange White" } };
std::vector<std::vector<size_t>> PaintByGlove = { { 10006, 10007, 10008, 10039 }, {10018, 10019, 10037, 10038 }, { 10013, 10015, 10016, 10040 }, { 10021, 10036 }, { 10024, 10026, 10027, 10028 }, { 10030, 10033, 10034, 10035 } };
int Config::Skins::Gloves::TSkin = 0;
std::vector<std::string> Config::Skins::Gloves::TSkinOptions = Config::Skins::Gloves::SkinsByGlove[0];
int Config::Skins::Gloves::CTSkin = 0;
std::vector<std::string> Config::Skins::Gloves::CTSkinOptions = Config::Skins::Gloves::SkinsByGlove[0];

TeamID currentTeam = TeamID::TEAM_UNASSIGNED;

namespace GloveChanger
{
	bool NeedsUpdate = false;

	WearableObject_t::WearableObject_t(int index, int paint, int seed, int stattrak, int quality, float wear, char* fileName, bool updateStatus)
	{
		Index = index;
		Paintkit = paint;
		Seed = seed;
		Stattrak = stattrak;
		EntityQuality = quality;
		Wear = wear;
		ModelFileName = fileName;
		UpdateStatus = updateStatus;
	}

	GloveSkin_t GetGloveSkin(bool TSide)
	{
		size_t glove = TSide ? Config::Skins::Gloves::TGlove : Config::Skins::Gloves::CTGlove;
		size_t skin = TSide ? Config::Skins::Gloves::TSkin : Config::Skins::Gloves::CTSkin;

		return { (ItemDefinitionIndex)GloveIndices[glove], PaintByGlove[glove][skin] };
	}

	void FrameStageNotify()
	{
		if (G::CurStage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
			return;

		if (!I::Engine->IsInGame())
			return;

		if (!Config::Skins::Gloves::Enable)
			return;

		if (!G::LocalPlayer)
			return;

		IEngineClient::player_info_t localplayer_info;
		if (!I::Engine->GetPlayerInfo(I::Engine->GetLocalPlayer(), &localplayer_info))
			return;

		if (!G::LocalPlayer->GetAlive() || G::LocalPlayer->GetTeam() != currentTeam)
		{
			if (G::LocalPlayer->GetTeam() != currentTeam)
				currentTeam = G::LocalPlayer->GetTeam();

			C_BaseAttributableItem* glove = (C_BaseAttributableItem*)I::EntList->GetClientEntity(G::LocalPlayer->GetWearables()[0] & 0xFFF);
			if (!glove)
				return;

			glove->GetNetworkable()->SetDestroyedOnRecreateEntities();
			glove->GetNetworkable()->Release();

			return;
		}

		if (!I::EntList->GetClientEntity(G::LocalPlayer->GetWearables()[0] & 0xFFF))
		{
			for (ClientClass *pClass = I::Client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
			{
				if (pClass->m_ClassID != EClassIds::CEconWearable)
					continue;

				int entry = (I::EntList->GetHighestEntityIndex() + 1);
				int serial = Utilities::RandomInt(0x0, 0xFFF);

				pClass->m_pCreateFn(entry, serial);
				G::LocalPlayer->GetWearables()[0] = entry | (serial << 16);

				//SkinChanger::forceFullUpdate = true;
				NeedsUpdate = true;

				break;
			}
		}

		C_BaseAttributableItem* glove = (C_BaseAttributableItem*)I::EntList->GetClientEntity(G::LocalPlayer->GetWearables()[0] & 0xFFF);
		if (!glove)
			return;

		GloveSkin_t skin = GetGloveSkin(G::LocalPlayer->GetTeam() == TeamID::TEAM_TERRORIST);

		if (*glove->GetItemDefinitionIndex() != skin.index)
		{

			*glove->GetItemDefinitionIndex() = skin.index;

			*glove->GetFallbackSeed() = 0;
			*glove->GetFallbackStatTrak() = -1;
			*glove->GetEntityQuality() = 4;
			*glove->GetItemIDHigh() = -1;

			*glove->GetFallbackPaintKit() = skin.paint;
			*glove->GetFallbackWear() = 0.000001f;

			if (NeedsUpdate)
			{
				glove->GetNetworkable()->PreDataUpdate(DATA_UPDATE_CREATED);
				*glove->GetAccountID() = localplayer_info.xuidlow;
				glove->SetModelIndex2(I::ModelInfo->GetModelIndex(ItemDefinitionIndexMap.at(skin.index).entityModel));
				NeedsUpdate = false;
			}
		}
	}
}