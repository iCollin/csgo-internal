#include "SkyChanger.h"

#include "settings.h"
#include "offsets.h"
#include "interfaces.h"

int Config::Misc::SkyChanger::SkySelection = 0;
std::vector<std::string> Config::Misc::SkyChanger::SkyOptions = { "Off", "cs_baggage_skybox_", "cs_tibet", "embassy", "italy","jungle","nukeblank", "office","sky_cs15_daylight01_hdr","sky_cs15_daylight02_hdr", "sky_cs15_daylight03_hdr","sky_cs15_daylight04_hdr","sky_csgo_cloudy01","sky_csgo_night02","sky_csgo_night02b","sky_dust","sky_venice","vertigo","vietnam" };

namespace SkyChanger
{
	bool SkyNeedsUpdate = true;

	void PaintTraverse()
	{
		if (!SkyNeedsUpdate)
			return;

		if (!I::Engine->IsInGame())
			return;

		if (!Config::Misc::SkyChanger::SkySelection)
			return;

		if (I::CVar->FindVar("sv_skyname")->strString != Config::Misc::SkyChanger::SkyOptions[Config::Misc::SkyChanger::SkySelection])
		{
			static auto LoadSky = reinterpret_cast< void(__fastcall*)(const char*) >(Offsets::LoadSky);
			if (!LoadSky) Utilities::Warn("LoadSky signature unsuccessful");
			else LoadSky(Config::Misc::SkyChanger::SkyOptions[Config::Misc::SkyChanger::SkySelection].c_str());
		}

		SkyNeedsUpdate = false;
	}
}