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
credit: Synraw @ unknowncheats.me
*/

/*

#include "menu.h"

#include "render.h"
#include "interfaces.h"
#include "utilities.h"

#include "controls.h"

cyanMenu Settings;

bool MenuIsOpen = false;
bool windowIsDragging = false;
char keys[256], oldKeys[256];
MPoint Mouse;
MPoint prevMouse;

int menu_pos_x = 240;
int menu_pos_y = 180;

int menu_width = 1080;
int menu_height = 680;

int menu_tab_width = 160;
int menu_tab_height = 40;

int close_button_size = 24;

int header_height = 10;
int tab_area_width = 180;

int tab_start_x = menu_pos_x + tab_area_width;
int tab_start_y = menu_pos_y + header_height;
int tab_width = menu_width - tab_area_width;
int tab_height = menu_height - header_height;

int selectedTabIndex = 0;
int highlightedTabIndex = -1;
std::vector<std::string> tabNames = { XOR("AIMBOT"), XOR("TRIGGER"), XOR("VISUALS"), XOR("SOUNDS"), XOR("MISC"), XOR("SKINS"), XOR("COLORS") };

int selectedConfigIndex = 0;
static std::vector<std::string> cfgNames = { XOR("Legit"), XOR("Legit2"), XOR("Rage"), XOR("HvH") };
bool configComboOpen = false;

bool MouseInArea(int x, int y, int w, int h)
{
	return (Mouse.x > x && Mouse.x < x + w) && (Mouse.y > y && Mouse.y < y + h);
}

void Menu2::Initialize()
{
	std::vector<CTab*> tabs = { &Settings.Aimbot, &Settings.Colors, &Settings.Misc, &Settings.Skins, &Settings.Sounds, &Settings.Trigger, &Settings.Visuals };

	for (auto tab : tabs)
		tab->Setup();
}

void Menu2::PaintTraverse()
{
	Update();
	Draw();
}

void Menu2::Update()
{
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
		keys[x] = static_cast<bool>(GetAsyncKeyState(x));

	RECT Screen = Render::GetViewport();

	if (GetKeyPress(VK_INSERT))
	{
		if (MenuIsOpen) I::Engine->ClientCmd_Unrestricted(XOR("cl_mouseenable 1"));
		else I::Engine->ClientCmd_Unrestricted(XOR("cl_mouseenable 0"));

		MenuIsOpen = !MenuIsOpen;
	}

	// page keys? (prior is up) (next is down)
	if (GetKeyPress(VK_END))
		Utilities::unload = true;

	if (!MenuIsOpen)
		return;

	// Mouse Location
	POINT mp; GetCursorPos(&mp);
	ScreenToClient(GetForegroundWindow(), &mp);
	Mouse.x = mp.x; Mouse.y = mp.y;  

	if (windowIsDragging)
	{
		menu_pos_x += Mouse.x - prevMouse.x;
		menu_pos_y += Mouse.y - prevMouse.y;
		windowIsDragging = false;
	}

	highlightedTabIndex = -1;
	if (MouseInArea(menu_pos_x + 5, menu_pos_y + 5, menu_tab_width, menu_tab_height * tabNames.size())) // tabs
	{
		highlightedTabIndex = CheckTabSelection();
		if (GetKeyPress(1))
			selectedTabIndex = highlightedTabIndex;
	}
	else if ((MouseInArea(menu_pos_x, menu_pos_y + menu_tab_height * tabNames.size() + 5, menu_tab_width, 220) || MouseInArea(menu_pos_x + tab_area_width, menu_pos_y, menu_width - tab_area_width, 15)) && GetAsyncKeyState(0x1) & 0x8000) // drag
	{
		windowIsDragging = true;
	}
	else if (MouseInArea(menu_pos_x, menu_pos_y + menu_tab_height * tabNames.size() + 185, tab_area_width, menu_height - (menu_tab_height * tabNames.size() + 185))) // config / DLL
	{
		if (GetKeyPress(1) && !configComboOpen)
		{
			if (MouseInArea(menu_pos_x + 30, menu_pos_y + 555, 40, 40)) // save
				Utilities::SaveConfig(cfgNames[selectedConfigIndex]);
			else if (MouseInArea(menu_pos_x + 90, menu_pos_y + 555, 40, 40)) // load
				Utilities::LoadConfig(cfgNames[selectedConfigIndex]);
			else if (MouseInArea(menu_pos_x + 30, menu_pos_y + 625, 40, 40)) // reload
				Utilities::GetLastCompetitiveMatchID();
			else if (MouseInArea(menu_pos_x + 90, menu_pos_y + 625, 40, 40)) // unload
				Utilities::unload = true;
		}
	}

	prevMouse.x = Mouse.x;
	prevMouse.y = Mouse.y;
}

void Menu2::Draw()
{
	if (!MenuIsOpen)
		return;

	DrawWindow();
	DrawCursor();
}

void Menu2::DrawCursor()
{
	Render::Circle(Mouse.x, Mouse.y, 4, oldColors::CURSOR_BACKGROUND);
	Render::Circle(Mouse.x, Mouse.y, 3, oldColors::CURSOR_FOREGROUND);
}

void DrawWindow()
{
	Render::Outline(menu_pos_x - 3, menu_pos_y - 3, 926, 486, oldColors::BLACK);
	Render::Outline(menu_pos_x - 2, menu_pos_y - 2, 924, 484, oldColors::BLACK);
	Render::Clear(menu_pos_x - 1, menu_pos_y - 1, 922, 482, oldColors::MENU_BORDER_LIGHT);

	Render::Clear(menu_pos_x + 2, menu_pos_y + 28, 916, 424, oldColors::MENU_BORDER);
	Render::Outline(menu_pos_x + 2, menu_pos_y + 28, 916, 424, oldColors::BLACK);

	Render::Outline(menu_pos_x + 2, menu_pos_y + 2, 916, 24, oldColors::BLACK);
	Render::Outline(menu_pos_x + 2, menu_pos_y + 455, 916, 24, oldColors::BLACK);

	Render::Text(menu_pos_x + 8, menu_pos_y + 6, oldColors::WHITE25, Render::Fonts::lucida18o, "CYAN");
	Render::Text(menu_pos_x + 38, menu_pos_y + 8, Color(48, 48, 48), Render::Fonts::lucida18o, "- collin");
}

void Menu2::DrawWindow()
{
	// menu outline
	Render::Outline(menu_pos_x - 7, menu_pos_y - 7, menu_width + 14, menu_height + 14, Colors::Menu::Border);
	Render::Outline(menu_pos_x - 6, menu_pos_y - 6, menu_width + 12, menu_height + 12, oldColors::MENU_BORDER_LIGHT);
	Render::Clear(menu_pos_x - 5, menu_pos_y - 5, 3, menu_height + 10, oldColors::MENU_BORDER);
	Render::Clear(menu_pos_x + menu_width + 2, menu_pos_y - 5, 3, menu_height + 10, oldColors::MENU_BORDER);
	Render::Clear(menu_pos_x - 5, menu_pos_y - 5, menu_width + 10, 3, oldColors::MENU_BORDER);
	Render::Clear(menu_pos_x - 5, menu_pos_y + menu_height + 2, menu_width + 10, 3, oldColors::MENU_BORDER);
	Render::Outline(menu_pos_x - 2, menu_pos_y - 2, menu_width + 4, menu_height + 4, oldColors::MENU_BORDER_LIGHT);
	Render::Outline(menu_pos_x - 1, menu_pos_y - 1, menu_width + 2, menu_height + 2, Colors::Menu::Border);

	// menu background
	Render::Clear(menu_pos_x, menu_pos_y, menu_width, menu_height, oldColors::MENU_BACKGROUND);

	// draw tab names
	for (size_t i = 0; i < tabNames.size(); i++)
		Render::Text(menu_pos_x + 10, menu_pos_y + 5 + i * menu_tab_height, i == selectedTabIndex ? oldColors::SELECTED_TAB_TEXT : i == highlightedTabIndex ? oldColors::HIGHLIGHTED_TAB_TEXT : oldColors::TAB_TEXT, Render::Fonts::tabFont, tabNames[i].c_str());

	// draw selected tab
	switch (selectedTabIndex)
	{
	case 0:
		Settings.Aimbot.Draw(menu_pos_x + tab_area_width, menu_pos_y);
		break;
	case 1:
		Settings.Trigger.Draw(menu_pos_x + tab_area_width, menu_pos_y);
		break;
	case 2:
		Settings.Visuals.Draw(menu_pos_x + tab_area_width, menu_pos_y);
		break;
	case 3:
		Settings.Sounds.Draw(menu_pos_x + tab_area_width, menu_pos_y);
		break;
	case 4:
		Settings.Misc.Draw(menu_pos_x + tab_area_width, menu_pos_y);
		break;
	case 5:
		Settings.Skins.Draw(menu_pos_x + tab_area_width, menu_pos_y);
		break;
	case 6:
		Settings.Colors.Draw(menu_pos_x + tab_area_width, menu_pos_y);
		break;
	}

	// draw config picker
	DrawConfigGroupBox(menu_pos_x, menu_pos_y + 500);

	// draw unload & reload buttons
	DrawDLLGroupBox(menu_pos_x, menu_pos_y + 610);
}

bool Menu2::GetKeyPress(unsigned int key)
{
	return keys[key] && !oldKeys[key];
}

void Watermark::PaintTraverse()
{
	if (I::Engine->IsInGame())
		return;

	RECT screen = Render::GetViewport();
	std::string tag(XOR("cyan ") + Utilities::VersionString);
	Render::Text(screen.left + 6, 3, oldColors::WATERMARK_BACKGROUND, Fonts::MENU, tag.c_str());
	Render::Text(screen.left + 5, 2, oldColors::WATERMARK_FOREGROUND, Fonts::MENU, tag.c_str());
}

void DrawFontTable(int x, int y)
{
	x -= 20;
	y -= 100;

	Render::Text(x + 40, y + 120, Colors::Menu::MENUTEXT, Render::Fonts::tahoma12, XOR("tahoma12"));
	Render::Text(x + 40, y + 144, Colors::Menu::MENUTEXT, Render::Fonts::tahoma14, XOR("tahoma14"));
	Render::Text(x + 40, y + 168, Colors::Menu::MENUTEXT, Render::Fonts::tahoma16, XOR("tahoma16"));
	Render::Text(x + 40, y + 192, Colors::Menu::MENUTEXT, Render::Fonts::tahoma18, XOR("tahoma18"));
	Render::Text(x + 40, y + 216, Colors::Menu::MENUTEXT, Render::Fonts::tahoma20, XOR("tahoma20"));

	Render::Text(x + 40, y + 240, Colors::Menu::MENUTEXT, Render::Fonts::tahoma12o, XOR("tahoma12o"));
	Render::Text(x + 40, y + 264, Colors::Menu::MENUTEXT, Render::Fonts::tahoma14o, XOR("tahoma14o"));
	Render::Text(x + 40, y + 288, Colors::Menu::MENUTEXT, Render::Fonts::tahoma16o, XOR("tahoma16o"));
	Render::Text(x + 40, y + 312, Colors::Menu::MENUTEXT, Render::Fonts::tahoma18o, XOR("tahoma18o"));
	Render::Text(x + 40, y + 336, Colors::Menu::MENUTEXT, Render::Fonts::tahoma20o, XOR("tahoma20o"));

	Render::Text(x + 240, y + 120, Colors::Menu::MENUTEXT, Render::Fonts::verdana12, XOR("verdana12"));
	Render::Text(x + 240, y + 144, Colors::Menu::MENUTEXT, Render::Fonts::verdana14, XOR("verdana14"));
	Render::Text(x + 240, y + 168, Colors::Menu::MENUTEXT, Render::Fonts::verdana16, XOR("verdana16"));
	Render::Text(x + 240, y + 192, Colors::Menu::MENUTEXT, Render::Fonts::verdana18, XOR("verdana18"));
	Render::Text(x + 240, y + 216, Colors::Menu::MENUTEXT, Render::Fonts::verdana20, XOR("verdana20"));
}

void DrawIcons(int x, int y)
{
	x -= 40;
	y -= 40;

	Render::Icons::Save(x + 40, y + 40, oldColors::WHITE);
	Render::Icons::Load(x + 80, y + 40, oldColors::WHITE);
	Render::Icons::Unload(x + 120, y + 40, oldColors::WHITE);
	Render::Icons::Reload(x + 160, y + 40, oldColors::WHITE);
	Render::Icons::Save2(x + 200, y + 40, oldColors::WHITE);
	Render::Icons::Load2(x + 240, y + 40, oldColors::WHITE);
	Render::Icons::Unload2(x + 280, y + 40, oldColors::WHITE);
	Render::Icons::Reload2(x + 320, y + 40, oldColors::WHITE);

	Render::Toggle(x + 40, y + 80, true);
	Render::Toggle(x + 40, y + 100, false);
	Render::BigToggle(x + 80, y + 80, true);
	Render::BigToggle(x + 140, y + 80, false);
}

int CheckTabSelection()
{
	for (size_t i = 0; i < tabNames.size(); i++)
		if (Mouse.y < menu_pos_y + 5 + menu_tab_height * (i + 1))
			return i;

	return 0;
}

void DrawConfigGroupBox(int x, int y)
{
	Render::Text(x + 30, y + 3, Colors::Menu::MENUTEXT, Fonts::MENU, XOR("Config"));
	Render::Line(x + 25, y + 8, x + 10, y + 8, oldColors::GRPBOX_OUTLINES);
	Render::Line(x + 10, y + 8, x + 10, y + 98, oldColors::GRPBOX_OUTLINES);
	Render::Line(x + 10, y + 98, x + 150, y + 98, oldColors::GRPBOX_OUTLINES);
	Render::Line(x + 150, y + 98, x + 150, y + 8, oldColors::GRPBOX_OUTLINES);
	Render::Line(x + 150, y + 8, x + 70, y + 8, oldColors::GRPBOX_OUTLINES);

	Render::ComboBox(x + 20, y + 20, cfgNames, &selectedConfigIndex, configComboOpen ? MouseInArea(x + 20, y + 25, 124, 18 * (cfgNames.size() + 1)) : MouseInArea(x + 20, y + 25, 124, 18), &configComboOpen, Menu2::GetKeyPress(1), Mouse.y);

	if (configComboOpen)
		return;

	if (MouseInArea(x + 30, y + 50, 40, 40)) Render::Icons::Save(x + 30, y + 50, oldColors::ICON);
	else Render::Icons::Save2(x + 30, y + 50, oldColors::ICON);
	if (MouseInArea(x + 90, y + 50, 40, 40)) Render::Icons::Load(x + 90, y + 50, oldColors::ICON);
	else Render::Icons::Load2(x + 90, y + 50, oldColors::ICON);
}

void DrawDLLGroupBox(int x, int y)
{
	Render::Text(x + 30, y + 3, Colors::Menu::MENUTEXT, Fonts::MENU, XOR("DLL"));
	Render::Line(x + 25, y + 8, x + 10, y + 8, oldColors::GRPBOX_OUTLINES);
	Render::Line(x + 10, y + 8, x + 10, y + 58, oldColors::GRPBOX_OUTLINES);
	Render::Line(x + 10, y + 58, x + 150, y + 58, oldColors::GRPBOX_OUTLINES);
	Render::Line(x + 150, y + 58, x + 150, y + 8, oldColors::GRPBOX_OUTLINES);
	Render::Line(x + 150, y + 8, x + 50, y + 8, oldColors::GRPBOX_OUTLINES);

	if (MouseInArea(x + 30, y + 15, 40, 40)) Render::Icons::Reload(x + 30, y + 15, oldColors::ICON);
	else Render::Icons::Reload2(x + 30, y + 15, oldColors::ICON);
	if (MouseInArea(x + 90, y + 15, 40, 40)) Render::Icons::Unload(x + 90, y + 15, oldColors::ICON);
	else Render::Icons::Unload2(x + 90, y + 15, oldColors::ICON);
}

void aimbotTab::Draw(int x, int y)
{
	bool* click = new bool(Menu2::GetKeyPress(1));
	Settings.Aimbot.conRifles->Update(x, y + 10, Mouse, click);
}

void triggerTab::Draw(int x, int y)
{
	bool* click = new bool(Menu2::GetKeyPress(1));
	Settings.Trigger.conTrigger->Update(x, y + 10, Mouse, click);
}

void visualsTab::Draw(int x, int y)
{
	bool* click = new bool(Menu2::GetKeyPress(1));
	Settings.Visuals.conOverrideView->Update(x, y + 10, Mouse, click);
	Settings.Visuals.conChams->Update(x, y + 230, Mouse, click);
	Settings.Visuals.conBomb->Update(x, y + 475, Mouse, click);

	Settings.Visuals.conESP->Update(x + 275, y + 10, Mouse, click);
}

void soundsTab::Draw(int x, int y)
{
	bool* click = new bool(Menu2::GetKeyPress(1));
	Settings.Sounds.conVisibleSounds->Update(x, y + 10, Mouse, click);
}

void miscTab::Draw(int x, int y)
{
	bool* click = new bool(Menu2::GetKeyPress(1));
	Settings.Misc.conAutoAccept->Update(x, y + 10, Mouse, click);
	Settings.Misc.conRCS->Update(x, y + 90, Mouse, click);
	Settings.Misc.conBhop->Update(x, y + 240, Mouse, click);
	Settings.Misc.conDefuse->Update(x, y + 320, Mouse, click);
}

void skinsTab::Draw(int x, int y)
{
	bool* click = new bool(Menu2::GetKeyPress(1));
	Settings.Skins.conKnife->Update(x, y + 10, Mouse, click);
	Settings.Skins.conSkins->Update(x, y + 120, Mouse, click);
	Settings.Skins.skinList->Update(x + 10, y + 160, Mouse, click);
}

void colorsTab::Draw(int x, int y)
{
	static int maxColorHeight = 33;
	bool* click = new bool(Menu2::GetKeyPress(1));

	for (size_t i = 0; i < Settings.Colors.colorPickers.size(); i++)
		if (Settings.Colors.colorPickers[i]->GetLabel() != Settings.Colors.selectedLabel)
			Settings.Colors.colorPickers[i]->Update((i > maxColorHeight) ? x + 450 : x + 5, (i > maxColorHeight) ? y + 10 + ((i - 26) * 20) : y + 10 + (i * 20), Mouse, click);

	for (size_t i = 0; i < Settings.Colors.colorPickers.size(); i++)
		if (Settings.Colors.colorPickers[i]->GetLabel() == Settings.Colors.selectedLabel)
		{
			Render::Clear((i > maxColorHeight) ? x + 450 : x + 5, (i > maxColorHeight) ? y + 10 + ((i - 26) * 20) : y + 9 + (i * 20), 338, 20, Colors::Menu::ENABLED);
			Settings.Colors.colorPickers[i]->Update((i > maxColorHeight) ? x + 450 : x + 5, (i > maxColorHeight) ? y + 10 + ((i - 26) * 20) : y + 10 + (i * 20), Mouse, click);
		}
}

void aimbotTab::Setup()
{
	conRifles->PlaceControl(enable, 0, 0);
	conRifles->PlaceControl(friendlyFire, 1, 0);
	conRifles->PlaceControl(fov, 2, 0);
	conRifles->PlaceControl(Selection, 3, 0);
	conRifles->PlaceControl(requireKey, 4, 0);
	conRifles->PlaceControl(key, 5, 0);
	conRifles->PlaceControl(bone, 6, 0);
	conRifles->PlaceControl(maxFov, 7, 0);
}

void colorsTab::Setup()
{
	selectedLabel = "";

	// add all color pickers
	colorPickers = { watermarkFG,
		watermarkBG,
		cursorFG,
		cursorBG,
		menuBorder,
		menuBorderLight,
		menuBG,
		menuOutlines,
		menuText,
		menuAccent1,
		menuAccent2,
		iconMain,
		chamsEnemyVisible,
		chamsEnemyHidden,
		chamsTeamVisible,
		chamsTeamHidden,
		chamsArms,
		chamsWeapons,
		crosshair1,
		crosshair2,
		consoleLog,
		consoleWarn,
		consoleError,
		footStepTeam,
		footStepEnemy
	};
}

void colorsTab::Select(std::string lbl)
{
	selectedLabel = lbl;
}

void skinsTab::Setup()
{
	// TODO: load config

	conKnife->PlaceControl(enableKnifeChanger, 0, 0);
	conKnife->PlaceControl(tKnife, 1, 0);
	conKnife->PlaceControl(ctKnife, 1, 1);
	conKnife->PlaceControl(tSkin, 2, 0);
	conKnife->PlaceControl(ctSkin, 2, 1);

	conSkins->PlaceControl(enableSkinChanger, 0, 0);
}

void miscTab::Setup()
{
	conAutoAccept->PlaceControl(enableAutoAccept, 0, 0);
	conAutoAccept->PlaceControl(delayAutoAccept, 1, 0);

	conRCS->PlaceControl(enableRCS, 0, 0);
	conRCS->PlaceControl(RCSxMin, 1, 0);
	conRCS->PlaceControl(RCSxMax, 2, 0);
	conRCS->PlaceControl(RCSyMin, 3, 0);
	conRCS->PlaceControl(RCSyMax, 4, 0);

	conBhop->PlaceControl(enableBhop, 0, 0);
	conBhop->PlaceControl(autoStrafe, 1, 0);

	conDefuse->PlaceControl(autoDefuse, 0, 0);
	conDefuse->PlaceControl(autoDefuseTime, 1, 0);
}

void visualsTab::Setup()
{
	conOverrideView->PlaceControl(customFOV, 0, 0);
	conOverrideView->PlaceControl(scopedFOV, 1, 0);
	conOverrideView->PlaceControl(extraFOV, 2, 0);
	conOverrideView->PlaceControl(thirdPerson, 3, 0);
	conOverrideView->PlaceControl(noVisRecoil, 4, 0);
	conOverrideView->PlaceControl(noSmoke, 5, 0);
	conOverrideView->PlaceControl(noFlash, 6, 0);
	conOverrideView->PlaceControl(flashMaxAlpha, 7, 0);

	conChams->PlaceControl(enableChams, 0, 0);
	conChams->PlaceControl(visCheck, 1, 0);
	conChams->PlaceControl(playerChams, 2, 0);
	conChams->PlaceControl(weaponChams, 3, 0);
	conChams->PlaceControl(armsChams, 4, 0);
	conChams->PlaceControl(armsTypes, 5, 0);
	conChams->PlaceControl(filterTeam, 6, 0);
	conChams->PlaceControl(flatChams, 7, 0);
	conChams->PlaceControl(wireframeSkins, 8, 0);

	conBomb->PlaceControl(plantedT, 0, 0);
	conBomb->PlaceControl(plantedCT, 1, 0);
	conBomb->PlaceControl(unplantedT, 2, 0);
	conBomb->PlaceControl(unplantedCT, 3, 0);
	conBomb->PlaceControl(showDefuser, 4, 0);
	conBomb->PlaceControl(bombTimer, 5, 0);
	conBomb->PlaceControl(predictHealth, 6, 0);

	conESP->PlaceControl(enableESP, 0, 0);
	conESP->PlaceControl(espVisCheck, 1, 0);
	conESP->PlaceControl(drawPlayers, 2, 0);
	conESP->PlaceControl(drawDrops, 3, 0);
	conESP->PlaceControl(drawChickens, 4, 0);
	conESP->PlaceControl(drawBox, 5, 0);
	conESP->PlaceControl(drawNames, 6, 0);
	conESP->PlaceControl(drawWeapon, 7, 0);
	conESP->PlaceControl(drawHealth, 8, 0);
}

void triggerTab::Setup()
{
	conTrigger->PlaceControl(enable, 0, 0);
	conTrigger->PlaceControl(requireKey, 1, 0);
	conTrigger->PlaceControl(triggerKey, 2, 0);
	conTrigger->PlaceControl(friendlyFire, 3, 0);
	conTrigger->PlaceControl(delay, 4, 0);
	conTrigger->PlaceControl(flashCheck, 5, 0);
}

void soundsTab::Setup()
{
	conVisibleSounds->PlaceControl(enable, 0, 0);
	conVisibleSounds->PlaceControl(showTeam, 1, 0);
	conVisibleSounds->PlaceControl(showOnRadar, 2, 0);
	conVisibleSounds->PlaceControl(minVolume, 3, 0);

	//auto VisibleSounds = CreateContainer("Sound ESP");

	//VisibleSounds->CreateToggle("Enable", "EnableVisibleSounds");
}


*/