#pragma once

#include <unordered_map>

struct GloveSkin_t
{
	ItemDefinitionIndex index;
	size_t paint;
};

namespace GloveChanger
{
	struct WearableObject_t
	{
		WearableObject_t(int, int, int, int, int, float, char* = "", bool = false);

		int Index;
		int Paintkit;
		int Seed;
		int Stattrak;
		int EntityQuality;
		float Wear;
		char* ModelFileName;
		bool UpdateStatus;
	};

	// to keep track of when new gloves need to be made. when team switch :)
	extern bool NeedsUpdate;

	// takes team == Terrorist
	GloveSkin_t GetGloveSkin(bool);

	void FrameStageNotify();
}