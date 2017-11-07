#pragma once

#include "interfaces.h"

namespace Stickers
{
	enum class EStickerAttributeType
	{
		Index,
		Wear,
		Scale,
		Rotation
	};

	void ApplyStickerHooks(C_BaseAttributableItem* item);
}

// FuckFace32 stuff (haven't tried to implement)
/*
class CCStrike15ItemDefinition
{
public:
virtual int GetNumSupportedStickerSlots();
virtual const char* GetStickerSlotMaterialBySlotIndex();
};

class CMaterialReference
{
public:
	// constructor, destructor
	CMaterialReference(char const* pMaterialName = 0, const char *pTextureGroupName = 0, bool bComplain = true);
	~CMaterialReference();

	// Attach to a material
	void Init(const char* pMaterialName, const char *pTextureGroupName, bool bComplain = true);
	void Init(const char *pMaterialName, KeyValues *pVMTKeyValues);
	void Init(IMaterial* pMaterial);
	void Init(CMaterialReference& ref);
	void Init(const char *pMaterialName, const char *pTextureGroupName, KeyValues *pVMTKeyValues);

	// Detach from a material
	void Shutdown(bool bDeleteIfUnreferenced = false);
	bool IsValid() { return m_pMaterial != 0; }

	// Automatic casts to IMaterial
	operator IMaterial*() { return m_pMaterial; }
	operator IMaterial const*() const { return m_pMaterial; }
	IMaterial* operator->() { return m_pMaterial; }

	// Assignment operator
	const CMaterialReference& operator=(const CMaterialReference &ref);


private:
	CMaterialReference(CMaterialReference &ref) { }

	IMaterial* m_pMaterial;
};

class C_EconItemView
{
public:
	void GenerateStickerMaterials();
	
	CCStrike15ItemDefinition* GetItemDefinition();

private:
	struct stickerMaterialReference_t
	{
		CMaterialReference FirstPerson;
		CMaterialReference ThirdPerson;
		unsigned int iPosition;
	};

	CUtlVector<stickerMaterialReference_t> m_vStickerMaterialReferences;
};*/