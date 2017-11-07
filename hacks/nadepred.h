#pragma once

#include "interfaces.h"

namespace GrenadePrediction
{
	extern float CurrentThrowStrength;
	extern std::vector<Vector> path;
	extern C_BaseCSGrenade* throwable;
	//extern NadeInfo_t ni;

	void CreateMove();
	void PaintTraverse();

	bool IsGrenade(ItemDefinitionIndex id);
	void Setup(C_BasePlayer* localplayer, Vector& vecSource, Vector& vecThrow);
	void Simulate(C_BasePlayer* localplayer);
	int Step(Vector& vecSource, Vector& vecThrow, int tick, float interval);
	bool CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval);
	void TraceHull(const Vector& vecSource, const Vector& vecEnd, trace_t& tr);
	void AddGravityMove(Vector& vecMove, Vector& vecVelocity, float frametime, bool onGround);
	void PushEntity(Vector& vecSource, const Vector& vecMove, trace_t& tr);
	void ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval);
	int PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce);

}


