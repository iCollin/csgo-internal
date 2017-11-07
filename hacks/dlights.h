#pragma once

#include "interfaces.h"

namespace DLights
{
	void AllocDlight(Color col, float exponent, float radius, float decay, int key, Vector direction, Vector origin, float die = 0.f);
	void AllocElight(Color col, float exponent, float radius, float decay, int key, Vector direction, Vector origin, float die = 0.f);
}