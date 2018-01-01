#pragma once

// credit namazso @ unknowncheats.me

#include <vector>

#include "utilities.h"

namespace SkinParser
{
	extern std::vector<std::pair<int, std::string>> k_skins;
	extern std::vector<std::pair<int, std::string>> k_gloves;
	extern std::vector<std::pair<int, std::string>> k_stickers;

	extern void InitializeKits();

}
