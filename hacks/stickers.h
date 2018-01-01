#pragma once

// credit namazso @ unknowncheats.me

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
