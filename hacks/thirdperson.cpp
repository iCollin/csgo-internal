#include "thirdperson.h"

#include "globals.h"
#include "settings.h"

void ThirdPerson::FrameStageNotify()
{
	if (!I::Engine->IsInGame())
		return;

	if (I::Engine->IsTakingScreenshot() || Config::Debug::StreamMode)
		return;

	static QAngle vecAngles;
	I::Engine->GetViewAngles(vecAngles);

	if (Config::Visuals::OverrideView::Thirdperson && G::LocalPlayer->GetAlive())
	{
		if (!I::Input->m_fCameraInThirdPerson)
		{
			I::Input->m_fCameraInThirdPerson = true;
			I::Input->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, Config::Visuals::OverrideView::ThirdpersonDistance);
		}
	}
	else
	{
		I::Input->m_fCameraInThirdPerson = false;
		I::Input->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
	}
}
