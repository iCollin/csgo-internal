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

#pragma once
 
#include "Windows.h"

#include "controls.h"

#include <string>

// TODO: Xor these strings... seriously liability if ever string encryption is needed.
/*
namespace Watermark
{
	void PaintTraverse();
}

namespace Menu2
{
	void Initialize();

	void PaintTraverse();

	void Update();

	void Draw();
	void DrawCursor();
	void DrawWindow();

	bool GetKeyPress(unsigned int key);
}

class aimbotTab : public CTab
{
public:
	aimbotTab() = default;
	void Setup();
	void Draw(int x, int y);

	CContainer* conRifles = new CContainer(20, 1, "Rifles");

	CToggle* enable = new CToggle("Enable", "Rifles.Enable");
	CToggle* friendlyFire = new CToggle("Friendly Fire", "Rifles.FriendlyFire");
	CSlider* fov = new CSlider(0.f, 10.f, 3.5f, "FOV", "Rifles.FOV");
	CComboBox* Selection = new CComboBox("Select By", "Rifles.Selection", { "Crosshair", "Distance", "Health" }, 0);
	CToggle* requireKey = new CToggle("RequireKey", "Rifles.RequiresKey");
	CKeyBind* key = new CKeyBind("Key", "Rifles.AimKey");
	CComboBox* bone = new CComboBox("Bone", "TargetBone", { "Head", "Neck", "Chest", "Stomach", "Random", "Best" }, 0);
	CToggle* maxFov = new CToggle("Max FOV?", "maxFOV");

	std::vector<CControl*> controls = { enable, friendlyFire, fov, Selection, requireKey, key, bone, maxFov };
};

class triggerTab : public CTab
{
public:
	void Setup();
	void Draw(int x, int y);

	CContainer* conTrigger = new CContainer(12, 1, "Settings");

	CToggle* enable = new CToggle("Enable", "Enable");
	CToggle* requireKey = new CToggle("Require Key?", "requireKey");
	CKeyBind* triggerKey = new CKeyBind("Key", "TriggerKey");
	CToggle* friendlyFire = new CToggle("Friendly Fire", "FriendlyFire");
	CSlider* delay = new CSlider(0.f, 2.f, 0.08f, "Wait", "Delay");
	CToggle* flashCheck = new CToggle("Flash Check?", "FlashCheck");

	std::vector<CControl*> controls = { enable, requireKey, triggerKey, friendlyFire, delay, flashCheck };
};

class visualsTab : public CTab
{
public:
	void Setup();
	void Draw(int x, int y);

	CContainer* conOverrideView = new CContainer(8, 1, "FrameStageNotify");

	CToggle* customFOV = new CToggle("Custom FOV", "CustomFOVEnable");
	CToggle* scopedFOV = new CToggle("When Scoped?", "CustomScopedFOV");
	CSlider* extraFOV = new CSlider(0.f, 50.f, 37.f, "+FOV", "ExtraFOV");
	CToggle* thirdPerson = new CToggle("Thirdperson", "ThirdPerson");
	CToggle* noVisRecoil = new CToggle("No Visible Recoil", "NoVisRecoil");
	CToggle* noSmoke = new CToggle("No Smoke", "NoSmoke");
	CToggle* noFlash = new CToggle("No Flash", "NoFlash");
	CSlider* flashMaxAlpha = new CSlider(0.f, 255.f, 155.f, "Alpha", "FlashMaxAlpha");

	CContainer* conChams = new CContainer(9, 1, "Chams");

	CToggle* enableChams = new CToggle("Enable", "ChamsEnable");
	CToggle* visCheck = new CToggle("Vis Check", "ChamsVisibleOnly");
	CToggle* playerChams = new CToggle("Players", "PlayerChams");
	CToggle* weaponChams = new CToggle("Weapons", "WeaponChams");
	CToggle* armsChams = new CToggle("Arms", "ArmsChams");
	CComboBox* armsTypes = new CComboBox("Arms Type", "ChamsArmsType", {"Default", "Colored", "Wire Frame", "None"}, 0);
	CToggle* filterTeam = new CToggle("Filter Team", "TeamFilter");
	CToggle* flatChams = new CToggle("Flat Chams", "FlatChams");
	CToggle* wireframeSkins = new CToggle("Wireframe Skins", "WireframeSkins");

	CContainer* conBomb = new CContainer(7, 1, "Bomb");

	CToggle* plantedT = new CToggle("Show Planted Bomb on T?", "PlantedBombT");
	CToggle* plantedCT = new CToggle("Show Planted Bomb on CT?", "PlantedBombCT");
	CToggle* unplantedT = new CToggle("Show Wild Bomb on T?", "WildBombT");
	CToggle* unplantedCT = new CToggle("Show Wild Bomb on CT?", "WildBombCT");
	CToggle* showDefuser = new CToggle("Show Defuer?", "DefuseESP");
	CToggle* bombTimer = new CToggle("Bomb Timer", "BombTimer");
	CToggle* predictHealth = new CToggle("Predict Explosion Health", "BombPred");

	CContainer* conESP = new CContainer(10, 1, "ESP");

	CToggle* enableESP = new CToggle("Enable", "ESPEnable");
	CToggle* espVisCheck = new CToggle("Visible Check?", "ESPVisCheck");
	CToggle* drawPlayers = new CToggle("Draw Players", "DrawPlayers");
	CToggle* drawDrops = new CToggle("Draw Dropped Weapons", "DrawDrops");
	CToggle* drawChickens = new CToggle("Draw Chickens", "DrawChickens");
	CToggle* drawBox = new CToggle("Draw Box", "DrawBox");
	CToggle* drawNames = new CToggle("Draw Names", "DrawNames");
	CToggle* drawWeapon = new CToggle("Draw Weapon", "DrawWeapon");
	CToggle* drawHealth = new CToggle("Draw Health", "DrawHealth");

	std::vector<CControl*> controls = { customFOV, scopedFOV, extraFOV, thirdPerson, noVisRecoil, noSmoke, noFlash, flashMaxAlpha, enableChams, visCheck, playerChams, weaponChams, armsChams, armsTypes, filterTeam, flatChams, plantedT, plantedCT, unplantedT, unplantedCT, showDefuser, bombTimer,
							enableESP, espVisCheck, drawPlayers, drawDrops, drawChickens, drawBox, drawNames, drawWeapon, drawHealth };
};

class soundsTab : public CTab
{
public:
	void Setup();
	void Draw(int x, int y);

	CContainer* conVisibleSounds = new CContainer(8, 1, "Visible Sounds");

	CToggle* enable = new CToggle("Enable", "EnableVisibleSounds");
	CToggle* showTeam = new CToggle("Filter Team?", "FilterTeamSounds");
	CToggle* showOnRadar = new CToggle("Show on Radar", "SpotLoudEnemies");
	CSlider* minVolume = new CSlider(0.f, 1.f, 0.35f, "Vol", "MinimumSoundESPVolume");

	std::vector<CControl*> controls = { enable, showTeam, showOnRadar, minVolume };
};

class miscTab : public CTab
{
public:
	void Setup();
	void Draw(int x, int y);

	CContainer* conAutoAccept = new CContainer(2, 1, "Auto Accept");

	CToggle* enableAutoAccept = new CToggle("Enable", "AutoAcceptEnable");
	CSlider* delayAutoAccept = new CSlider(0.f, 19.9f, 3.4f, "Wait", "AutoAcceptDelay");

	CContainer* conRCS = new CContainer(5, 1, "RCS");

	CToggle* enableRCS = new CToggle("Enable", "RCSEnable");
	CSlider* RCSxMin = new CSlider(0.f, 2.f, 1.3f, "Xmin", "RCSxMin");
	CSlider* RCSxMax = new CSlider(0.f, 2.f, 2.f, "Xmax", "RCSxMax");
	CSlider* RCSyMin = new CSlider(0.f, 2.f, 1.3f, "Ymin", "RCSyMin");
	CSlider* RCSyMax = new CSlider(0.f, 2.f, 2.f, "Ymax", "RCSyMax");

	CContainer* conBhop = new CContainer(2, 1, "Bunnyhop");

	CToggle* enableBhop = new CToggle("Enable", "BHOPEnable");
	CToggle* autoStrafe = new CToggle("Auto Strafe", "BHOPAutoStrafe");

	CContainer* conDefuse = new CContainer(2, 1, "Auto Defuse");

	CToggle* autoDefuse = new CToggle("Enable", "EnableAutoDefuse");
	CSlider* autoDefuseTime = new CSlider(0.f, 1.f, 0.3f, "Time", "AutoDefuseTime");

	std::vector<CControl*> controls = { enableAutoAccept, delayAutoAccept, enableRCS, RCSxMax, RCSxMin, RCSyMax, RCSyMin, enableBhop, autoStrafe, autoDefuse, autoDefuseTime };
};

class skinsTab : public CTab
{
public:
	void Setup();
	void Draw(int x, int y);

	CContainer* conKnife = new CContainer(3, 2, "Knives");

	CToggle* enableKnifeChanger = new CToggle("Enable", "KnifeChangerEnabled");
	std::vector<std::string> knives = { "Default", "Flip", "Gut", "Bayonet", "M9 Bayonet", "Karambit", "Shadow Daggers", "Huntsman", "Bowie", "Falchion", "Butterfly" };
	CComboBox* tKnife = new CComboBox("T Knife", "TerroristKnife", knives, 0);
	CComboBox* ctKnife = new CComboBox("CT Knife", "CounterTerroristKnife", knives, 0);
	std::vector<std::string> skins = { "Vanilla", "Sapphire", "Ruby", "Black Pearl", "Emerald", "Lore", "Fade", "Fire & Ice", "Blue Gem" };
	CComboBox* tSkin = new CComboBox("T Skin", "TerroristKnifeSkin", skins, 0);
	CComboBox* ctSkin = new CComboBox("CT Skin", "CounterTerroristKnifeSkin", skins, 0);

	CContainer* conSkins = new CContainer(20, 2, "Skins");

	CToggle* enableSkinChanger = new CToggle("Enable", "SkinChangerEnable");
	CSearchBox* skinList = new CSearchBox("Search Skins:", ""); // SKIN CONFIG IS DIFFERENT, DONT ADD TO CONTROLS VECTOR HERE

	std::vector<CControl*> controls = { enableKnifeChanger, tKnife, ctKnife, tSkin, ctSkin, enableSkinChanger };
};

class colorsTab : public CTab
{
public:
	void Setup();
	void Draw(int x, int y);
	void Select(std::string lbl);

	std::string selectedLabel;
	std::vector<CColorPicker*> colorPickers;

	CColorPicker* watermarkFG = new CColorPicker("Watermark FG", "WatermarkFG", &oldColors::WATERMARK_FOREGROUND);
	CColorPicker* watermarkBG = new CColorPicker("Watermark BG", "WatermarkBG", &oldColors::WATERMARK_BACKGROUND);

	CColorPicker* cursorFG = new CColorPicker("Cursor FG", "CursorFG", &oldColors::CURSOR_FOREGROUND);
	CColorPicker* cursorBG = new CColorPicker("Cursor BG", "CursorBG", &oldColors::CURSOR_BACKGROUND);

	CColorPicker* menuBorder = new CColorPicker("Menu Border", "MenuBorder", &oldColors::MENU_BORDER);
	CColorPicker* menuBorderLight = new CColorPicker("Menu Border Light", "MenuBorderLight", &oldColors::MENU_BORDER_LIGHT);
	CColorPicker* menuBG = new CColorPicker("Menu BG", "MenuBG", &oldColors::MENU_BACKGROUND);
	CColorPicker* menuOutlines = new CColorPicker("Menu Outlines", "MenuOutlines", &oldColors::GRPBOX_OUTLINES);
	CColorPicker* menuText = new CColorPicker("Menu Text", "MenuText", &oldColors::MENU_TEXT);
	CColorPicker* menuAccent1 = new CColorPicker("Menu Accent #1", "MenuAccent1", &Colors::Menu::ENABLED);
	CColorPicker* menuAccent2 = new CColorPicker("Menu Accent #2", "MenuAccent2", &Colors::Menu::DISABLED);

	CColorPicker* iconMain = new CColorPicker("Icons", "Icons", &oldColors::ICON);

	CColorPicker* chamsEnemyVisible = new CColorPicker("Chams Enemy Visible", "ChamsEnemyVisible", &oldColors::CHAMS_ENEMY_VISIBLE);
	CColorPicker* chamsEnemyHidden = new CColorPicker("Chams Enemy Hidden", "ChamsEnemyHidden", &oldColors::CHAMS_ENEMY_HIDDEN);
	CColorPicker* chamsTeamVisible = new CColorPicker("Chams Team Visible", "ChamsTeamVisible", &oldColors::CHAMS_TEAM_VISIBLE);
	CColorPicker* chamsTeamHidden = new CColorPicker("Chams Team Hidden", "ChamsTeamHidden", &oldColors::CHAMS_TEAM_HIDDEN);
	CColorPicker* chamsArms = new CColorPicker("Chams Arms", "ChamsArms", &oldColors::CHAMS_ARMS);
	CColorPicker* chamsWeapons = new CColorPicker("Chams Weapons", "ChamsWeapons", &oldColors::CHAMS_WEAPONS);

	CColorPicker* crosshair1 = new CColorPicker("Crosshair #1", "Crosshair1", &oldColors::CROSSHAIR1);
	CColorPicker* crosshair2 = new CColorPicker("Crosshair #2", "Crosshair2", &oldColors::CROSSHAIR2);

	CColorPicker* consoleLog = new CColorPicker("Console Log", "ConsoleLog", &oldColors::CONSOLE_LOG);
	CColorPicker* consoleWarn = new CColorPicker("Console Warn", "ConsoleWarn", &oldColors::CONSOLE_WARN);
	CColorPicker* consoleError = new CColorPicker("Console Error", "ConsoleError", &oldColors::CONSOLE_ERROR);

	CColorPicker* footStepTeam = new CColorPicker("Team Footsteps", "TeamFootsteps", &oldColors::FOOTSTEPS_TEAM);
	CColorPicker* footStepEnemy = new CColorPicker("Enemy Footstep", "EnemyFootstep", &oldColors::FOOTSTEPS_ENEMY);
	CColorPicker* bombTimerText = new CColorPicker("Bomb Timer Text", "BombText", &oldColors::BOMB_TIMER_TEXT);
};

class cyanMenu
{
public:
	cyanMenu() = default;

	aimbotTab Aimbot;
	triggerTab Trigger;
	soundsTab Sounds;
	visualsTab Visuals;
	miscTab Misc;
	skinsTab Skins;
	colorsTab Colors;
};

extern cyanMenu Settings;

extern HINSTANCE hModule;

int CheckTabSelection();

void DrawConfigGroupBox(int x, int y);
void DrawDLLGroupBox(int x, int y);

*/