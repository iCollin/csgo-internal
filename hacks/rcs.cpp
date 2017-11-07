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
*/


/*
#include "rcs.h"

#include "menu.h"
#include "utilities.h"
#include "interfaces.h"
#include "settings.h"

void RCS::CreateMove(void* pCmdVoid)
{
	CUserCmd* pCmd = reinterpret_cast<CUserCmd*>(pCmdVoid);

	// TODO complete refactor, move code into aimbot.cpp prolly
	if (!Config::Aimbot::Rifles::RCS::Enable)
		return;

	C_BasePlayer* pLocal = I::EntList->GetClientEntity(I::Engine->GetLocalPlayer());
	if (!pLocal)
		return;

	QAngle aimPunch = *pLocal->GetAimPunchAngle();
	vec_t punchLen = aimPunch.Length2D();

	static float curRCSx = (Config::Aimbot::RCS::xMin + Config::Aimbot::RCS::xMax) / 2.f;
	static float curRCSy = (Config::Aimbot::RCS::yMin + Config::Aimbot::RCS::yMax) / 2.f;

	if (punchLen < 0 || punchLen > 120)
		return;

	float xMin = Config::Aimbot::RCS::xMin;
	float xMax = Config::Aimbot::RCS::xMin;
	float yMin = Config::Aimbot::RCS::xMin;
	float yMax = Config::Aimbot::RCS::xMin;

	if (xMin > xMax)
	{
		Utilities::Warn(XOR("RCS x Min is greater than x Max, auto fixing\n"));
		//Settings.Misc.RCSxMin->SetValue(Settings.Misc.RCSxMax->GetValue() - 0.1f);
	}

	if (yMin > yMax)
	{
		Utilities::Warn(XOR("RCS y Min is greater than y Max, auto fixing\n"));
		//Settings.Misc.RCSyMin->SetValue(Settings.Misc.RCSyMax->GetValue() - 0.1f);
	}

	curRCSx += Utilities::RandomFloat(-0.02f, 0.02f);
	curRCSy += Utilities::RandomFloat(-0.02f, 0.02f);

	if (curRCSx < xMin) curRCSx = xMin;
	if (curRCSx > xMax) curRCSx = xMax;
	if (curRCSy < yMin) curRCSy = yMin;
	if (curRCSy > yMax) curRCSy = yMax;

	//if (!Settings.Aimbot.requireKey->GetBool() || GetAsyncKeyState(Settings.Aimbot.key->GetKey() & 0x8000))
	//{
	//	pCmd->viewangles.x -= aimPunch.x * 2.f;
	//	pCmd->viewangles.y -= aimPunch.y * 2.f;
	//}
	//else
	//{
		pCmd->viewangles.x -= aimPunch.x * curRCSx;
		pCmd->viewangles.y -= aimPunch.y * curRCSy;
	//}
}
*/