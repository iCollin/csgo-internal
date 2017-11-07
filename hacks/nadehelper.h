#pragma once

#include "interfaces.h"

// TODO maybe make nade shared functions for both helper and prediction?

namespace NadeHelper
{
	struct NadeHelper_t
	{
		std::string name;
		Vector position;
		QAngle angle;
		bool visible;

		NadeHelper_t(std::string, Vector, QAngle);
	};

	using nadeArray = std::unordered_map<std::string, std::unordered_map<std::string, std::vector<NadeHelper::NadeHelper_t>>>;

	extern nadeArray throws;

	void UpdateMapName();
	extern std::string currentMapName;

	void PaintTraverse();
	void CreateMove();

	void LoadThrows();
}