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
credit: Synraw @ unknowncheats.me
*/

#include "chams.h"

#include "menu.h"

#include "offsets.h"

#include "KeyValues.h"

#include "globals.h"
#include "settings.h"

// visuals chams config
bool Config::Visuals::Chams::Enable = true;
bool Config::Visuals::Chams::VisibleCheck = true;
bool Config::Visuals::Chams::Players = true;
bool Config::Visuals::Chams::Weapons = false;
bool Config::Visuals::Chams::Arms = true;
int Config::Visuals::Chams::ArmsType = 0;
std::vector<std::string> Config::Visuals::Chams::ArmsTypes = { "Default", "Wireframe", "Invisible", "Glass" };
bool Config::Visuals::Chams::FilterTeam = false;
bool Config::Visuals::Chams::FlatChams = false;
bool Config::Visuals::Chams::WireFrameSkins = false;

bool materialsCreated = false;
IMaterial* materialChams;
IMaterial* materialChamsIgnorez;
IMaterial* materialChamsFlat;
IMaterial* materialChamsFlatIgnorez;
IMaterial* materialChamsArms;
IMaterial* materialChamsWeapons;

std::vector<IMaterial*> createdMaterials = {};

void InitKeyValues(KeyValues* keyValues, char* name)
{
	static DWORD dwFunction = Offsets::KeyValues_Init + *reinterpret_cast<PDWORD_PTR>(Offsets::KeyValues_Init + 1) + 5;

	reinterpret_cast<void(__thiscall*)(void* thisptr, const char* name)>(dwFunction)(keyValues, name);
}

void LoadFromBuffer(KeyValues* keyValues, char const *resourceName, const char *pBuffer, void* fileSystem = 0, const char* pPathID = 0, void* fnEvaluateSymbolProc = 0)
{
	reinterpret_cast<void(__thiscall*)(void* thisptr, const char* resourceName, const char* pBuffer, void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc)>(Offsets::KeyValues_LoadFromBuffer)(keyValues, resourceName, pBuffer, fileSystem, pPathID, fnEvaluateSymbolProc);
}

IMaterial *CreateMaterial(bool shouldIgnoreZ, bool isLit, bool isWireframe) //credits to ph0ne
{
	static int created = 0;

	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$flat\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$nofog\" \"0\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
        \n}\n"
	};

	const char* baseType = isLit  ? "VertexLitGeneric" : "UnlitGeneric";
	char material[512];
	sprintf_s(material, sizeof(material), tmp, baseType, (shouldIgnoreZ) ? 1 : 0, (isWireframe) ? 1 : 0);

	std::string matName = "__cyan" + created++;

	KeyValues* keyValues = new KeyValues(matName.c_str());
	InitKeyValues(keyValues, (char*)baseType);
	LoadFromBuffer(keyValues, matName.c_str(), material);

	IMaterial *createdMaterial = I::MaterialSystem->CreateMaterial(matName.c_str(), keyValues);
	createdMaterial->IncrementReferenceCount();
	//createdMaterials.push_back(createdMaterial);

	return createdMaterial;
}

void DrawPlayer(void* thisPtr, void* context, void* state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld, DrawModelExecuteFn oDrawModelExecute)
{
	if (!Config::Visuals::Chams::Players)
		return;

	if (!G::LocalPlayer)
		return;

	C_BasePlayer* pEntity = (C_BasePlayer*)I::EntList->GetClientEntity(pInfo.entity_index);
	if (!pEntity || pEntity->GetDormant() || !pEntity->GetAlive())
		return;

	if (pEntity == G::LocalPlayer)
		return;

	if (Config::Visuals::Chams::FilterTeam && pEntity->GetTeam() == G::LocalPlayer->GetTeam())
		return;

	IMaterial* visible_material;
	IMaterial* hidden_material;

	if (Config::Visuals::Chams::FlatChams)
	{
		visible_material = materialChamsFlat;
		hidden_material = materialChamsFlatIgnorez;
	}
	else
	{
		visible_material = materialChams;
		hidden_material = materialChamsIgnorez;
	}

	visible_material->AlphaModulate(pEntity->GetImmune() ? 0.5f : 1.f);
	hidden_material->AlphaModulate(pEntity->GetImmune() ? 0.5f : 1.f);

	if (pEntity->GetTeam() == TeamID::TEAM_TERRORIST)
	{
		visible_material->ColorModulate(Colors::ESP::TCHAMSVISIBLE);
		hidden_material->ColorModulate(Colors::ESP::TCHAMS);
	}
	else if (pEntity->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST)
	{
		visible_material->ColorModulate(Colors::ESP::CTCHAMSVISIBLE);
		hidden_material->ColorModulate(Colors::ESP::CTCHAMS);
	}

	if (!Config::Visuals::Chams::VisibleCheck)
	{
		I::ModelRender->ForcedMaterialOverride(hidden_material);
		oDrawModelExecute(thisPtr, context, state, pInfo, pCustomBoneToWorld);
	}

	I::ModelRender->ForcedMaterialOverride(visible_material);
	oDrawModelExecute(thisPtr, context, state, pInfo, pCustomBoneToWorld);
}

void DrawArms(const ModelRenderInfo_t& pInfo, std::string modelName)
{
	if (!Config::Visuals::Chams::Arms)
		return;

	IMaterial* mat = materialChamsArms;

	switch (Config::Visuals::Chams::ArmsType)
	{
	case 0:
		break;
	case 1:
		mat->AlphaModulate(1.f);
		mat->ColorModulate(Colors::ESP::ARMSCHAMS);
		break;
	case 3:
		mat = I::MaterialSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
	}

	mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, Config::Visuals::Chams::ArmsType == 1);
	mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Config::Visuals::Chams::ArmsType == 2);
	I::ModelRender->ForcedMaterialOverride(mat);
}

void DrawWeapon(const ModelRenderInfo_t& pInfo, std::string modelName)
{
	if (!Config::Visuals::Chams::Weapons)
		return;

	IMaterial* mat = materialChamsWeapons;

	mat->ColorModulate(Colors::ESP::WEAPONCHAMS);
	mat->AlphaModulate(1.f);
	mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, Config::Visuals::Chams::WireFrameSkins);

	I::ModelRender->ForcedMaterialOverride(mat);
}

void Chams::DrawModelExecute(void* thisPtr, void* context, void* state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld, DrawModelExecuteFn oDrawModelExecute)
{
	if (!I::Engine->IsInGame())
		return;

	if (I::Engine->IsTakingScreenshot() || Config::Debug::StreamMode)
		return;

	if (!Config::Visuals::Chams::Enable)
		return;

	if (!pInfo.pModel)
		return;

	if (!materialsCreated)
	{
		materialChams = CreateMaterial(false, true, false); // Util::CreateMaterial("VertexLitGeneric", "VGUI/white_additive", false, true, true, true, true);
		materialChamsIgnorez = CreateMaterial(true, true, false); // Util::CreateMaterial("VertexLitGeneric", "VGUI/white_additive", true, true, true, true, true);
		materialChamsFlat = CreateMaterial(true, false, false); // Util::CreateMaterial("UnlitGeneric", "VGUI/white_additive", false, true, true, true, true);
		materialChamsFlatIgnorez = CreateMaterial(true, false, false); // Util::CreateMaterial("UnlitGeneric", "VGUI/white_additive", true, true, true, true, true);
		materialChamsArms = CreateMaterial(false, true, false); // Util::CreateMaterial("VertexLitGeneric", "VGUI/white_additive", false, true, true, true, true);
		materialChamsWeapons = CreateMaterial(false, true, false); // Util::CreateMaterial("VertexLitGeneric", "VGUI/white_additive", false, true, true, true, true);
		materialsCreated = true;
	}

	std::string modelName = I::ModelInfo->GetModelName(pInfo.pModel);

	if (modelName.find("models/player") != std::string::npos)
		DrawPlayer(thisPtr, context, state, pInfo, pCustomBoneToWorld, oDrawModelExecute);
	else if (modelName.find("arms") != std::string::npos)
		DrawArms(pInfo, modelName);
	else if (modelName.find("models/weapon") != std::string::npos)
		DrawWeapon(pInfo, modelName);
}

//void Chams::ShutDown()
//{
//	for (auto mat : createdMaterials)
//		mat->DecrementReferenceCount();
//}