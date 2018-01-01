// credit namazso @ unknowncheats.me

#include "stickers.h"

#include "skinchanger.h"

typedef float(__fastcall *AttributeFunction)(void*, void*, int, Stickers::EStickerAttributeType, float);
typedef unsigned int(__fastcall *IndexFunction)(void*, void*, int, Stickers::EStickerAttributeType, unsigned fl);

AttributeFunction oAttribFn;
IndexFunction oIndexFn;

float __fastcall AttributeFnHooked(void* thisptr, void*, int slot, Stickers::EStickerAttributeType attribute, float fl)
{
	C_BaseCombatWeapon* weapon = reinterpret_cast<C_BaseCombatWeapon*>((uintptr_t)thisptr - NetVars::CEconEntity::m_Item);

	switch (attribute)
	{
	case Stickers::EStickerAttributeType::Wear:
		return 0.000005f;
	case Stickers::EStickerAttributeType::Scale:
		return 1.f;
	case Stickers::EStickerAttributeType::Rotation:
		return 0.f;
	}

	return oAttribFn(thisptr, nullptr, slot, attribute, fl);
}

unsigned int __fastcall IndexFnHooked(void* thisptr, void*, int slot, Stickers::EStickerAttributeType attribute, unsigned fl)
{
	C_BaseCombatWeapon* weapon = reinterpret_cast<C_BaseCombatWeapon*>((uintptr_t)thisptr - NetVars::CEconEntity::m_Item);

	if (weapon && attribute == Stickers::EStickerAttributeType::Index)
	{
		int id = SkinChanger::GetSkinObjectByIndex(*weapon->GetItemDefinitionIndex())->GetStickerID(slot);

		if (id != 0)
			return id;
	}

	return oIndexFn(thisptr, nullptr, slot, attribute, fl);
}

void Stickers::ApplyStickerHooks(C_BaseAttributableItem* item)
{
	if (NetVars::CEconEntity::m_Item == 0xC)
		return;

	void**& stickerInfo = *reinterpret_cast<void***>((uintptr_t)item + NetVars::CEconEntity::m_Item);

	static void** stickerInfoHook = nullptr;

	if (!stickerInfoHook)
	{
		size_t length = 0;

		for (; stickerInfo[length]; ++length);

		stickerInfoHook = new void*[length];
		memcpy(stickerInfoHook, stickerInfo, length * sizeof(void*));

		oAttribFn = reinterpret_cast<AttributeFunction>(stickerInfoHook[4]);
		stickerInfoHook[4] = &AttributeFnHooked;

		oIndexFn = reinterpret_cast<IndexFunction>(stickerInfoHook[5]);
		stickerInfoHook[5] = &IndexFnHooked;
	}

	stickerInfo = stickerInfoHook;
}
