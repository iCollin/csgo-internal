#include "dlights.h"

void DLights::AllocDlight(Color col, float exponent, float radius, float decay, int key, Vector direction, Vector origin, float die)
{
	dlight_t* temp = I::Effects->CL_AllocDlight(key);
	temp->color.r = col.r;
	temp->color.g = col.g;
	temp->color.b = col.b;
	temp->color.exponent = exponent; // 0, 133, 255, 161 <= ideal col
	temp->radius = radius;
	temp->decay = decay;
	temp->m_Direction = direction;
	temp->origin = origin;
	temp->die = die < 0.1f ? I::globalVars->curtime + 0.2f : die;
}

void DLights::AllocElight(Color col, float exponent, float radius, float decay, int key, Vector direction, Vector origin, float die)
{
	dlight_t* temp = I::Effects->CL_AllocElight(key);
	temp->color.r = col.r;
	temp->color.g = col.g;
	temp->color.b = col.b;
	temp->color.exponent = exponent;
	temp->radius = radius;
	temp->decay = decay;
	temp->m_Direction = direction;
	temp->origin = origin;
	temp->die = die < 0.1f ? I::globalVars->curtime + 0.2f : die;
}
