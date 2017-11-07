#include "GUI.h"

#include "tinyxml.h"

#include "Path.h"
#include "utilities.h"

#include "nadehelper.h"
//#include "skinparser.h"

#include "skinchanger.h"

Menu* GUI::cyan;

ComboBox* GUI::TGloveSkinOptions;
ComboBox* GUI::CTGloveSkinOptions;
ComboBox* GUI::SkinOptions;

bool m_keys[256], m_oldKeys[256];
int m_curMouseWheelValue = 0;

LRESULT CALLBACK mouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	// Hook chain
	if (nCode < 0)
		return CallNextHookEx(0, nCode, wParam, lParam);

	// Check if the mouse wheel is used
	if (wParam == WM_MOUSEWHEEL)
	{
		MSLLHOOKSTRUCT *hookStruct = (MSLLHOOKSTRUCT*)lParam;
		
		m_curMouseWheelValue = (short)HIWORD(hookStruct->mouseData);
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}

HHOOK mHook = NULL;

bool __declspec(dllexport) __stdcall GUI::SetHook()
{
	if (IsHookSet())
		return false;

	mHook = SetWindowsHookExA(WH_MOUSE_LL, mouseHookCallback, Utilities::hModule, 0);

	int errorID = GetLastError();
#ifdef __cyandbg
	if (errorID != 0)
		MessageBoxA(NULL, std::to_string(errorID).c_str(), "Hooking Error ID", 0);
#endif
	return !errorID;
}

bool __declspec(dllexport) __stdcall GUI::RemoveHook()
{
	if (!IsHookSet())
		return false;

	if (UnhookWindowsHookEx(mHook))
	{
		mHook = NULL;
		return true;
	}

	int errorID = GetLastError();
#ifdef __cyandbg
	if (errorID != 0)
		MessageBoxA(NULL, std::to_string(errorID).c_str(), "Unhooking Error ID", 0);
#endif
	return !errorID;
}

bool GUI::IsHookSet()
{
	return mHook != NULL;
}

void GUI::UpdateHook()
{
	// must be called from game thread
	static HWND csgo = FindWindow(NULL, "Counter-Strike: Global Offensive");

	if (GetActiveWindow() == csgo && cyan->isSkinTab()) GUI::SetHook();
	else GUI::RemoveHook();

	if (cyan->isSkinTab())
	{
		GUI::TGloveSkinOptions->SetItems(Config::Skins::Gloves::SkinsByGlove[Config::Skins::Gloves::TGlove]);
		GUI::CTGloveSkinOptions->SetItems(Config::Skins::Gloves::SkinsByGlove[Config::Skins::Gloves::CTGlove]);
	}
}

void GUI::Initialize()
{
	cyan = new Menu();

	auto aimbot = cyan->RegisterTab("AIMBOT");
	auto visuals = cyan->RegisterTab("VISUALS");
	auto misc = cyan->RegisterTab("MISC");
	auto skins = cyan->RegisterCustomTab<SkinsTab>("SKINS");
	auto colors = cyan->RegisterCustomTab<ColorsTab>("COLORS");

	// general aimbot controls
	auto aimbotGeneral = aimbot->CreateGroup("General");
	aimbotGeneral->CreateToggle("Enable", true, &Config::Aimbot::Enable);
	aimbotGeneral->CreateToggle("Friendly Fire", true, &Config::Aimbot::FriendlyFire);
	aimbotGeneral->CreateComboBox("Selection Style", true, &Config::Aimbot::SelectionStyle, Config::Aimbot::SelectionStyles);
	aimbotGeneral->CreateToggle("Aimbot Requires Key", true, &Config::Aimbot::RequiresKey);
	aimbotGeneral->CreateKeyBind("Aim Key", &Config::Aimbot::Key);
	aimbotGeneral->CreateToggle("Trigger Requires Key", true, &Config::Triggerbot::RequiresKey);
	aimbotGeneral->CreateKeyBind("Trigger Key", &Config::Triggerbot::Key);
	aimbotGeneral->CreateComboBox("RCS Mode", true, &Config::Aimbot::RCS::Mode, Config::Aimbot::RCS::Modes);
	aimbotGeneral->CreateToggle("Prediction", true, &Config::Aimbot::Prediction);
	aimbotGeneral->CreateToggle("Auto Scope", true, &Config::Aimbot::AutoScope);
	aimbotGeneral->CreateToggle("Auto Crouch", true, &Config::Aimbot::AutoCrouch);
	aimbotGeneral->CreateToggle("Only On Ground", true, &Config::Aimbot::IgnoreJump);
	aimbotGeneral->CreateToggle("Visible Check", true, &Config::Aimbot::VisCheck);
	aimbotGeneral->CreateToggle("One Target", true, &Config::Aimbot::OneTarget);
	aimbotGeneral->CreateToggle("Autowall", true, &Config::Aimbot::AutoWall::Enable);
	aimbotGeneral->CreateSlider("Autowall Min Damage", 0.f, 100.f, &Config::Aimbot::AutoWall::MinDamage);
	aimbotGeneral->CreateSlider("Min Hitchance", 0.f, 100.f, &Config::Aimbot::HitChance);

	// aimbot rifles controls
	auto aimbotRifles = aimbot->CreateGroup("Rifles");
	aimbotRifles->CreateToggle("Enable", true, &Config::Aimbot::Rifles::Enable);
	aimbotRifles->CreateToggle("Max FOV", true, &Config::Aimbot::Rifles::MaxFOV);
	aimbotRifles->CreateSlider("FOV", Config::Aimbot::Rifles::minFOV, Config::Aimbot::Rifles::maxFOV, &Config::Aimbot::Rifles::FOV);
	aimbotRifles->CreateComboBox("Bone", true, &Config::Aimbot::Rifles::Bone, Config::Aimbot::Bones);
	aimbotRifles->CreateToggle("Enable Trigger", true, &Config::Triggerbot::Rifles::Enable);
	aimbotRifles->CreateSlider("Trigger Wait", 0.f, 0.5f, &Config::Triggerbot::Rifles::WaitTime);
	aimbotRifles->CreateToggle("Flash Check", true, &Config::Triggerbot::Rifles::FlashCheck);
	aimbotRifles->CreateToggle("Smoke Check", true, &Config::Triggerbot::Rifles::SmokeCheck);
	aimbotRifles->CreateToggle("Enable RCS", true, &Config::Aimbot::Rifles::RCS::Enable);
	aimbotRifles->CreateToggle("Salt", true, &Config::Aimbot::Rifles::Salt);
	aimbotRifles->CreateToggle("Smooth Aimbot", true, &Config::Aimbot::Rifles::Smooth);
	aimbotRifles->CreateSlider("Smoothing Ratio", 1.f, 32.f, &Config::Aimbot::Rifles::SmoothingRatio);

	// aimbot sniper controls
	auto aimbotSnipers = aimbot->CreateGroup("Snipers");
	aimbotSnipers->CreateToggle("Enable", true, &Config::Aimbot::Snipers::Enable);
	aimbotSnipers->CreateToggle("Max FOV", true, &Config::Aimbot::Snipers::MaxFOV);
	aimbotSnipers->CreateSlider("FOV", Config::Aimbot::Snipers::minFOV, Config::Aimbot::Snipers::maxFOV, &Config::Aimbot::Snipers::FOV);
	aimbotSnipers->CreateComboBox("Bone", true, &Config::Aimbot::Snipers::Bone, Config::Aimbot::Bones);
	aimbotSnipers->CreateToggle("Enable Trigger", true, &Config::Triggerbot::Snipers::Enable);
	aimbotSnipers->CreateSlider("Trigger Wait", 0.f, 0.5f, &Config::Triggerbot::Snipers::WaitTime);
	aimbotSnipers->CreateToggle("Flash Check", true, &Config::Triggerbot::Snipers::FlashCheck);
	aimbotSnipers->CreateToggle("Smoke Check", true, &Config::Triggerbot::Snipers::SmokeCheck);
	aimbotSnipers->CreateToggle("Enable RCS", true, &Config::Aimbot::Snipers::RCS::Enable);
	aimbotSnipers->CreateToggle("Salt", true, &Config::Aimbot::Snipers::Salt);
	aimbotSnipers->CreateToggle("Smooth Aimbot", true, &Config::Aimbot::Snipers::Smooth);
	aimbotSnipers->CreateSlider("Smoothing Ratio", 1.f, 32.f, &Config::Aimbot::Snipers::SmoothingRatio);

	// aimbot pistol controls
	auto aimbotPistols = aimbot->CreateGroup("Pistols");
	aimbotPistols->CreateToggle("Enable", true, &Config::Aimbot::Pistols::Enable);
	aimbotPistols->CreateToggle("Max FOV", true, &Config::Aimbot::Pistols::MaxFOV);
	aimbotPistols->CreateSlider("FOV", Config::Aimbot::Pistols::minFOV, Config::Aimbot::Pistols::maxFOV, &Config::Aimbot::Pistols::FOV);
	aimbotPistols->CreateComboBox("Bone", true, &Config::Aimbot::Pistols::Bone, Config::Aimbot::Bones);
	aimbotPistols->CreateToggle("Enable Trigger", true, &Config::Triggerbot::Pistols::Enable);
	aimbotPistols->CreateSlider("Trigger Wait", 0.f, 0.5f, &Config::Triggerbot::Pistols::WaitTime);
	aimbotPistols->CreateToggle("Flash Check", true, &Config::Triggerbot::Pistols::FlashCheck);
	aimbotPistols->CreateToggle("Smoke Check", true, &Config::Triggerbot::Pistols::SmokeCheck);
	aimbotPistols->CreateToggle("Enable RCS", true, &Config::Aimbot::Pistols::RCS::Enable);
	aimbotPistols->CreateToggle("Salt", true, &Config::Aimbot::Pistols::Salt);
	aimbotPistols->CreateToggle("Smooth Aimbot", true, &Config::Aimbot::Pistols::Smooth);
	aimbotPistols->CreateSlider("Smoothing Ratio", 1.f, 32.f, &Config::Aimbot::Pistols::SmoothingRatio);

	// misc visuals controls (override view)
	auto visualsMisc = visuals->CreateGroup("Misc");
	visualsMisc->CreateToggle("Custom FOV", true, &Config::Visuals::OverrideView::CustomFOV);
	visualsMisc->CreateToggle("Custom Scoped FOV", true, &Config::Visuals::OverrideView::CustomScopedFOV);
	visualsMisc->CreateSlider("FOV", Config::Visuals::OverrideView::minFOV, Config::Visuals::OverrideView::maxFOV, &Config::Visuals::OverrideView::FOV);
	visualsMisc->CreateToggle("Thirdperson", true, &Config::Visuals::OverrideView::Thirdperson);
	visualsMisc->CreateSlider("Thirdperson Distance", 50.f, 500.f, &Config::Visuals::OverrideView::ThirdpersonDistance);
	visualsMisc->CreateToggle("Visible Recoil", true, &Config::Visuals::OverrideView::VisibleRecoil);
	visualsMisc->CreateToggle("No Smoke", true, &Config::Visuals::OverrideView::NoSmoke);
	visualsMisc->CreateToggle("No Flash", true, &Config::Visuals::OverrideView::NoFlash);
	visualsMisc->CreateSlider("Flash Alpha", 0.f, 255.f, &Config::Visuals::OverrideView::FlashMaxAlpha);

	// visuals chams controls
	auto visualsChams = visuals->CreateGroup("Chams");
	visualsChams->CreateToggle("Enable", true, &Config::Visuals::Chams::Enable);
	visualsChams->CreateToggle("Vis Check", true, &Config::Visuals::Chams::VisibleCheck);
	visualsChams->CreateToggle("Players", true, &Config::Visuals::Chams::Players);
	visualsChams->CreateToggle("Weapons", true, &Config::Visuals::Chams::Weapons);
	visualsChams->CreateToggle("Arms", true, &Config::Visuals::Chams::Arms);
	visualsChams->CreateComboBox("Arms Type", true, &Config::Visuals::Chams::ArmsType, Config::Visuals::Chams::ArmsTypes);
	visualsChams->CreateToggle("Filter Team", true, &Config::Visuals::Chams::FilterTeam);
	visualsChams->CreateToggle("Flat Chams", true, &Config::Visuals::Chams::FlatChams);
	visualsChams->CreateToggle("Wireframe Skins", true, &Config::Visuals::Chams::WireFrameSkins);

	// visuals bomb controls
	auto visualsBomb = visuals->CreateGroup("Bomb");
	visualsBomb->CreateToggle("T Show Planted", true, &Config::Visuals::Bomb::ShowPlantedOnT);
	visualsBomb->CreateToggle("T Show Wild", true, &Config::Visuals::Bomb::ShowWildOnT);
	visualsBomb->CreateToggle("CT Show Planted", true, &Config::Visuals::Bomb::ShowPlantedOnCT);
	visualsBomb->CreateToggle("CT Show Wild", true, &Config::Visuals::Bomb::ShowWildOnCT);
	visualsBomb->CreateToggle("CT Show Defuser", true, &Config::Visuals::Bomb::ShowDefusingCT);
	visualsBomb->CreateToggle("Bomb Timer", true, &Config::Visuals::Bomb::BombTimer);
	visualsBomb->CreateToggle("Predict Health", true, &Config::Visuals::Bomb::PredictHealth);

	// visuals ESP player info controls
	auto infoESP = visuals->CreateGroup("PlayerInfo");
	infoESP->CreateToggle("Enable", true, &Config::Visuals::ESP::PlayerInfo::Enable);
	infoESP->CreateToggle("Weapon", true, &Config::Visuals::ESP::PlayerInfo::Weapon);
	infoESP->CreateToggle("Health", true, &Config::Visuals::ESP::PlayerInfo::Health);
	infoESP->CreateToggle("Armor", true, &Config::Visuals::ESP::PlayerInfo::Armor);
	infoESP->CreateToggle("Reloading", true, &Config::Visuals::ESP::PlayerInfo::Reloading);
	infoESP->CreateToggle("Flashed", true, &Config::Visuals::ESP::PlayerInfo::Flashed);
	infoESP->CreateToggle("Scoped", true, &Config::Visuals::ESP::PlayerInfo::Scoped);
	infoESP->CreateToggle("Bomb Carrier", true, &Config::Visuals::ESP::PlayerInfo::BombCarrier);
	infoESP->CreateToggle("Defuse Kit", true, &Config::Visuals::ESP::PlayerInfo::DefuseKit);
	infoESP->CreateToggle("Defusing", true, &Config::Visuals::ESP::PlayerInfo::Defusing);
	infoESP->CreateToggle("Hostage Carrier", true, &Config::Visuals::ESP::PlayerInfo::HostageCarrier);
	infoESP->CreateToggle("Rescuing", true, &Config::Visuals::ESP::PlayerInfo::Rescuing);
	infoESP->CreateToggle("Location", true, &Config::Visuals::ESP::PlayerInfo::Location);

	// visuals ESP controls
	auto visualsESP = visuals->CreateGroup("ESP");
	visualsESP->CreateToggle("Visible Check", true, &Config::Visuals::ESP::VisibleCheck);
	visualsESP->CreateToggle("Filter Team", true, &Config::Visuals::ESP::FilterTeam);
	visualsESP->CreateToggle("Player ESP", true, &Config::Visuals::ESP::PlayerESP);
	visualsESP->CreateToggle("Drop ESP", true, &Config::Visuals::ESP::DropESP);
	visualsESP->CreateToggle("Chicken ESP", true, &Config::Visuals::ESP::ChickenESP);
	visualsESP->CreateToggle("Grenade ESP", true, &Config::Visuals::ESP::GrenadeESP);
	visualsESP->CreateToggle("Draw Boxes", true, &Config::Visuals::ESP::DrawBoxes);
	visualsESP->CreateToggle("Draw Names", true, &Config::Visuals::ESP::DrawNames);
	visualsESP->CreateToggle("Draw Health", true, &Config::Visuals::ESP::DrawHealthBar);
	visualsESP->CreateToggle("Draw DLights", true, &Config::Visuals::ESP::DrawDlights);
	visualsESP->CreateToggle("Draw Barrel", true, &Config::Visuals::ESP::BarrelESP);
	visualsESP->CreateSlider("Barrel ESP Length", 0.f, 500.f, &Config::Visuals::ESP::BarrelESPLength);
	visualsESP->CreateToggle("Spread Circle", true, &Config::Visuals::ESP::SpreadCircle);
	visualsESP->CreateToggle("Hit Markers", true, &Config::Visuals::ESP::HitMarkers);
	visualsESP->CreateToggle("Aim Spot", true, &Config::Visuals::ESP::AimSpot);

	// visuals sounds controls
	auto visualSound = visuals->CreateGroup("Sound");
	visualSound->CreateToggle("Enable", true, &Config::Visuals::Sounds::Enable);
	visualSound->CreateToggle("Filter Team", true, &Config::Visuals::Sounds::FilterTeam);
	visualSound->CreateToggle("Show on Radar", true, &Config::Visuals::Sounds::ShowOnRadar);
	visualSound->CreateSlider("Minimum Volume", 0.f, 1.f, &Config::Visuals::Sounds::MinimumVolume);
	visualSound->CreateToggle("Mute Ambient", true, &Config::Visuals::Sounds::MuteAmbient);

	// visuals Events controls
	auto visualEvents = visuals->CreateGroup("EventESP");
	visualEvents->CreateToggle("Item Pickup", true, &Config::Visuals::Sounds::Events::item_pickup);
	visualEvents->CreateToggle("Footsteps", true, &Config::Visuals::Sounds::Events::player_footstep);
	visualEvents->CreateToggle("Fall Damage", true, &Config::Visuals::Sounds::Events::player_falldamage);
	visualEvents->CreateToggle("Squeaky", true, &Config::Visuals::Sounds::Events::door_moving);
	visualEvents->CreateToggle("Silencer Attach", true, &Config::Visuals::Sounds::Events::silencer_on);
	visualEvents->CreateToggle("Silencer Detach", true, &Config::Visuals::Sounds::Events::silencer_off);
	visualEvents->CreateToggle("Weeapon Zoom", true, &Config::Visuals::Sounds::Events::weapon_zoom);
	visualEvents->CreateToggle("Weapon Reload", true, &Config::Visuals::Sounds::Events::weapon_reload);
	visualEvents->CreateToggle("Weapon Fire", true, &Config::Visuals::Sounds::Events::weapon_fire);
	visualEvents->CreateToggle("Planting and Defusing", true, &Config::Visuals::Sounds::Events::bomb_actions);
	visualEvents->CreateToggle("Bomb Dropped", true, &Config::Visuals::Sounds::Events::bomb_dropped);
	visualEvents->CreateToggle("Show Event Name", true, &Config::Visuals::Sounds::Events::ShowEventType);
	visualEvents->CreateSlider("Expire Time", 0.1f, 5.f, &Config::Visuals::Sounds::Events::expireTime);
	visualEvents->CreateSlider("Maximum Distance", 0.f, 3000.f, &Config::Visuals::Sounds::Events::MaximumDistance);

	// visuals hitmarkers controls
	auto visualsHitMarkers = visuals->CreateGroup("HitMarkers");
	visualsHitMarkers->CreateToggle("Enable", true, &Config::Visuals::HitMarkers::Enable);
	visualsHitMarkers->CreateSlider("Expire Time", 0.f, 1.f, &Config::Visuals::HitMarkers::ExpireTime);
	visualsHitMarkers->CreateSlider("Marker Size", 1.f, 16.f, &Config::Visuals::HitMarkers::MarkerSize);
	visualsHitMarkers->CreateSlider("Move Distance", 0.f, 48.f, &Config::Visuals::HitMarkers::TravelDistance);
	visualsHitMarkers->CreateToggle("Show Impacts", true, &Config::Visuals::HitMarkers::ShowImpacts);

	// misc autoaccept controls
	auto miscAutoAccept = misc->CreateGroup("AutoAccept");
	miscAutoAccept->CreateToggle("Enable", true, &Config::Misc::AutoAccept::Enable);
	miscAutoAccept->CreateSlider("Delay", 0.f, 19.9f, &Config::Misc::AutoAccept::WaitTime);

	// misc bunnyhop controls
	auto miscBhop = misc->CreateGroup("Bunnyhop");
	miscBhop->CreateToggle("Enable", true, &Config::Misc::Bunnyhop::Enable);
	miscBhop->CreateToggle("SMAC Safe", true, &Config::Misc::Bunnyhop::SMACSafe);
	miscBhop->CreateToggle("AutoStrafe", true, &Config::Misc::Bunnyhop::AutoStrafe);

	// misc autodefuse controls
	auto miscDefuse = misc->CreateGroup("AutoDefuse");
	miscDefuse->CreateToggle("Enable", true, &Config::Misc::AutoDefuse::Enable);
	miscDefuse->CreateToggle("Aim at Bomb", true, &Config::Misc::AutoDefuse::AimAtBomb);
	miscDefuse->CreateSlider("Time Remaining", 0.1f, 1.f, &Config::Misc::AutoDefuse::ExtraTime);

	// misc spotify controls
	auto miscSpotify = misc->CreateGroup("Spotify");
	miscSpotify->CreateToggle("Show Controls", true, &Config::Misc::Spotify::ShowControls);
	miscSpotify->CreateToggle("Show Currently Playing", true, &Config::Misc::Spotify::ShowCurrentlyPlaying);

	// misc spectator list controls
	auto miscSpecList = misc->CreateGroup("SpectatorList");
	miscSpecList->CreateToggle("Enable", true, &Config::Misc::SpectatorList::Enable);

	// misc sky changer controls
	auto miscSky = misc->CreateGroup("Sky");
	miscSky->CreateComboBox("Custom Sky", true, &Config::Misc::SkyChanger::SkySelection, Config::Misc::SkyChanger::SkyOptions);

	// misc grenade controls
	auto miscNades = misc->CreateGroup("Grenades");
	miscNades->CreateToggle("Enable Prediction", true, &Config::Misc::GrenadeHelper::Prediction::Enable);
	miscNades->CreateToggle("Prediction DLight", true, &Config::Misc::GrenadeHelper::Prediction::DlightAtEnd);
	miscNades->CreateToggle("Smoke Helper", true, &Config::Misc::GrenadeHelper::SmokeHelper);
	miscNades->CreateToggle("Molly Helper", true, &Config::Misc::GrenadeHelper::MollyHelper);
	miscNades->CreateToggle("Flash Helper", true, &Config::Misc::GrenadeHelper::FlashHelper);
	miscNades->CreateToggle("DLight On Helper", true, &Config::Misc::GrenadeHelper::DlightOnPosition);

	// misc convar controls
	auto miscConvars = misc->CreateGroup("Convars");
	miscConvars->CreateToggle("mat_drawgray", true, &Config::Misc::RenderConVars::mat_drawgray);
	miscConvars->CreateToggle("mat_fullbright", true, &Config::Misc::RenderConVars::mat_fullbright);
	miscConvars->CreateToggle("mat_showlowresimage", true, &Config::Misc::RenderConVars::mat_showlowresimage);
	miscConvars->CreateToggle("mat_showmiplevels", true, &Config::Misc::RenderConVars::mat_showmiplevels);

	// misc viewmodel fov controls
	auto miscViewModelFOV = misc->CreateGroup("ViewmodelFOV");
	miscViewModelFOV->CreateToggle("Enable", true, &Config::Misc::ViewModelFOV::Enable);
	miscViewModelFOV->CreateSlider("Modifier", -100.f, 100.f, &Config::Misc::ViewModelFOV::Modifier);

	// skinchanger controls
	auto skinsKnife = skins->CreateGroup("Knives");
	skinsKnife->CreateToggle("Enable", true, &Config::Skins::Knives::Enable);
	skinsKnife->CreateComboBox("T Knife", true, &Config::Skins::Knives::TKnife, Config::Skins::Knives::knifeOptions);
	skinsKnife->CreateComboBox("T Skin", true, &Config::Skins::Knives::TKnifeSkin, Config::Skins::Knives::skinOptions);
	skinsKnife->CreateComboBox("CT Knife", true, &Config::Skins::Knives::CTKnife, Config::Skins::Knives::knifeOptions);
	skinsKnife->CreateComboBox("CT Skin", true, &Config::Skins::Knives::CTKnifeSkin, Config::Skins::Knives::skinOptions);

	// glove changer controls
	auto skinsGloves = skins->CreateGroup("Gloves");
	skinsGloves->CreateToggle("Enable", true, &Config::Skins::Gloves::Enable);
	skinsGloves->CreateComboBox("T Glove", true, &Config::Skins::Gloves::TGlove, Config::Skins::Gloves::GloveOptions);
	skinsGloves->CreateComboBox("CT Glove", true, &Config::Skins::Gloves::CTGlove, Config::Skins::Gloves::GloveOptions);
	TGloveSkinOptions = skinsGloves->CreateComboBox("T Skin", true, &Config::Skins::Gloves::TSkin, Config::Skins::Gloves::TSkinOptions);
	CTGloveSkinOptions = skinsGloves->CreateComboBox("CT Skin", true, &Config::Skins::Gloves::CTSkin, Config::Skins::Gloves::CTSkinOptions);

	// weapon skins controls
	auto skinsWeapons = skins->CreateGroup("Weapons");
	skinsWeapons->CreateToggle("Enable", true, &Config::Skins::Enable);
	skinsWeapons->CreateToggle("StatTrak", false, &Config::Skins::Weapons::StatTrak);
	skinsWeapons->CreateSlider("Float", 0.000005f, 1.f, &Config::Skins::Weapons::Float);
	skinsWeapons->CreateButton("Save Skin", &SkinChanger::SaveMenuToMap);
	skinsWeapons->CreateButton("Force Refresh", &SkinChanger::ForceRefresh);

	// menu colors
	auto menuColors = colors->CreateGroup("MenuColors");
	menuColors->CreateColorPicker("Background", &Colors::Menu::BACKGROUND);
	menuColors->CreateColorPicker("Border", &Colors::Menu::BORDER);
	menuColors->CreateColorPicker("Outlines", &Colors::Menu::OUTLINES);
	menuColors->CreateColorPicker("Enabled", &Colors::Menu::ENABLED);
	menuColors->CreateColorPicker("Disabled", &Colors::Menu::DISABLED);
	menuColors->CreateColorPicker("Text", &Colors::Menu::MENUTEXT);
	menuColors->CreateColorPicker("Hover Text", &Colors::Menu::HOVERTEXT);
	menuColors->CreateColorPicker("Menu Bar", &Colors::Menu::MENUBAR);
	menuColors->CreateColorPicker("Watermark FG", &Colors::Menu::WATERMARK_FOREGROUND);
	menuColors->CreateColorPicker("Watermark BG", &Colors::Menu::WATERMARK_BACKGROUND);
	menuColors->CreateColorPicker("Cursor FG", &Colors::Menu::CURSOR_FOREGROUND);
	menuColors->CreateColorPicker("Cursor BG", &Colors::Menu::CURSOR_BACKGROUND);

	// grenade colors
	auto nadeColors = colors->CreateGroup("Grenades");
	nadeColors->CreateColorPicker("Flashbang", &Colors::Grenades::FLASH);
	nadeColors->CreateColorPicker("HE", &Colors::Grenades::FRAG);
	nadeColors->CreateColorPicker("Decoy", &Colors::Grenades::DECOY);
	nadeColors->CreateColorPicker("Smoke", &Colors::Grenades::SMOKE);
	nadeColors->CreateColorPicker("Molotov", &Colors::Grenades::MOLLY);

	// console colors
	auto consoleColors = colors->CreateGroup("CONSOLE");
	consoleColors->CreateColorPicker("Log", &Colors::Console::LOG);
	consoleColors->CreateColorPicker("Warn", &Colors::Console::WARN);
	consoleColors->CreateColorPicker("Error", &Colors::Console::ERR);

	// esp colors
	auto espColors = colors->CreateGroup("ESP");
	espColors->CreateColorPicker("Text", &Colors::ESP::ESPTEXT);
	espColors->CreateColorPicker("Bomb Text", &Colors::ESP::ESPBOMBTEXT);
	espColors->CreateColorPicker("T Chams", &Colors::ESP::TCHAMS);
	espColors->CreateColorPicker("T Chams Visible", &Colors::ESP::TCHAMSVISIBLE);
	espColors->CreateColorPicker("CT Chams", &Colors::ESP::CTCHAMS);
	espColors->CreateColorPicker("CT Chams Visible", &Colors::ESP::CTCHAMSVISIBLE);
	espColors->CreateColorPicker("Arms Chams", &Colors::ESP::ARMSCHAMS);
	espColors->CreateColorPicker("Weapon Chams", &Colors::ESP::WEAPONCHAMS);
	espColors->CreateColorPicker("Team Footsteps", &Colors::ESP::FOOTSTEPS_TEAM);
	espColors->CreateColorPicker("Enemy Footsteps", &Colors::ESP::FOOTSTEPS_ENEMY);
	espColors->CreateColorPicker("Hitmarkers", &Colors::ESP::HITMARKERS);
	espColors->CreateColorPicker("Spread Circle", &Colors::ESP::SPREADCIRCLE);
	espColors->CreateColorPicker("Armor Bar", &Colors::ESP::ARMORBAR);
	espColors->CreateColorPicker("Health Bar", &Colors::ESP::HEALTHBAR);
	espColors->CreateColorPicker("Aim Spot", &Colors::ESP::AIMSPOT);
	espColors->CreateColorPicker("Chickens", &Colors::ESP::CHICKEN);
	espColors->CreateColorPicker("Drops", &Colors::ESP::DROPS);
	espColors->CreateColorPicker("Events", &Colors::ESP::EVENTS);

	// spotify controls colors
	auto spotifyColors = colors->CreateGroup("Spotify");
	spotifyColors->CreateColorPicker("Controls", &Colors::SpotifyControls::CONTROLS);
	spotifyColors->CreateColorPicker("Controls Hover", &Colors::SpotifyControls::CONTROLSHOVER);
	spotifyColors->CreateColorPicker("Song Title", &Colors::SpotifyControls::SONGTITLE);

#ifdef __cyandbg
	auto debug = cyan->RegisterTab("DEBUG");

	auto dreams = debug->CreateGroup("memes");
	dreams->CreateToggle("Draw Bones and IDs", true, &Config::Debug::DrawBoneIDs);
	dreams->CreateToggle("Stream Mode", true, &Config::Debug::StreamMode);
#endif

	Load();
}

void GUI::Save(std::string cfgName)
{
	tinyxml2::XMLDocument document;

	for (auto tab : cyan->Tabs)
	{
		auto elem = document.NewElement(tab->Name);
		document.LinkEndChild(elem);

		for (auto group : tab->Groups)
		{
			auto elem2 = document.NewElement(group->Name);
			elem->LinkEndChild(elem2);

			for (auto control : group->Controls)
			{
				if (!control->Saveable) continue;

				auto elem3 = document.NewElement(control->SafeName().c_str());
				elem2->LinkEndChild(elem3);

				control->Save(elem3);
			}
		}
	}

	std::string filename = Path::GetXMLConfig(cfgName);

	if (document.SaveFile(filename.c_str()) != tinyxml2::XML_NO_ERROR)
		Utilities::Error("Failed to save config file to " + filename);
	else
		Utilities::Log("Saved config file to " + filename);

	SkinChanger::SaveConfig();
}

void GUI::Load(std::string cfgName)
{
	NadeHelper::LoadThrows();

	std::string filename = Path::GetXMLConfig(cfgName);

	tinyxml2::XMLDocument document;
	if (document.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		Utilities::Error("Failed to load config from " + filename);
		return;
	}
	else
		Utilities::Log("Loading config file from " + filename);

	for (auto tab : cyan->Tabs)
	{
		auto elem = document.FirstChildElement(tab->Name);

		if (!elem)
		{
			Utilities::Warn("Didn't find " + std::string(tab->Name) + " in the config. Could not load its' children. Save to fix this issue.");
			continue;
		}

		for (auto group : tab->Groups)
		{
			auto elem2 = elem->FirstChildElement(group->Name);

			if (!elem2)
			{
				Utilities::Warn("Didn't find " + std::string(group->Name) + " in the config. Could not load its' children. Save to fix this issue.");
				continue;
			}

			for (auto control : group->Controls)
			{
				if (!control->Saveable) continue;

				auto elem3 = elem2->FirstChildElement(control->SafeName().c_str());

				if (!elem3)
				{
					Utilities::Warn("Didn't find " + control->SafeName() + " in the config. Save to update it.");
					continue;
				}

				control->Load(elem3->GetText());
			}
		}
	}
}

bool GUI::KeyPressed(unsigned int key)
{
	return m_keys[key] && !m_oldKeys[key];
}

bool GUI::KeyHeld(unsigned int key)
{
	return m_keys[key];
}

void GUI::PressKey(unsigned int key)
{
	m_keys[key] = true;
}

void GUI::LiftKey(unsigned int key)
{
	m_keys[key] = false;
}

int GUI::MouseWheelDelta()
{
	return m_curMouseWheelValue;
}

void GUI::PaintTraverse()
{
	std::copy(m_keys, m_keys + 255, m_oldKeys);
	for (int x = 0; x < 255; x++)
		m_keys[x] = static_cast<bool>(GetAsyncKeyState(x));

#ifdef __cyandbg
	if (KeyPressed(VK_END))
		Utilities::unload = true;
#endif

	cyan->Draw();
	cyan->DrawWatermark();

	m_curMouseWheelValue = 0;
}