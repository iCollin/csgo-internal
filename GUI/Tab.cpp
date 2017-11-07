#include "Tab.h"

#include "settings.h"

#include "skinparser.h"

Group* Tab::CreateGroup(const char* name)
{
	auto temp = new Group(name);
	Groups.push_back(temp);
	return temp;
}

void Tab::Update(POINT* location, POINT mouse)
{
	for (auto group : Groups)
		group->Update(location, mouse, Parent);

	CustomRender();
}

//std::vector<std::pair<int, std::string>> GenWeaponDict(std::vector<std::string> weaponNames)
//{
//	std::vector<std::pair<int, std::string>> temp = {};
//
//	for (size_t i = 0; i < weaponNames.size(); i++)
//		temp.push_back({ i, weaponNames[i] });
//
//	return temp;
//}
//
//std::vector<std::pair<int, std::string>> GenPaintKitDict(SkinParser::mapSkins skinz)
//{
//	std::vector<std::pair<int, std::string>> temp = {};
//
//	for (auto it : skinz)
//		for (auto elem : it.second)
//			temp.push_back({ elem.paintkit, Utilities::ToString(elem.tagName) });
//
//	return temp;
//}
//
//void SkinTab::Update(POINT * location, POINT mouse)
//{
//	for (auto group : Groups)
//		group->Update(location, mouse, Parent);
//
//	location->x += 180;
//	static SearchBox* weapon = new SearchBox("Weapons", GenWeaponDict(SkinParser::weaponNames), &Config::Skins::Weapons::SelectedWeapon);
//	weapon->Update(location, Parent);
//
//	location->x += 180;
//	static SearchBox* paintkit = new SearchBox("Paint Kits", GenPaintKitDict(SkinParser::skinz), &Config::Skins::Weapons::SelectedWeapon);
//	paintkit->Update(location, Parent);
//}

void ColorsTab::CustomRender()
{
	int x = Parent->posX() + Parent->width() + 10;//- 380;
	int y = Parent->MenuBarHeight() + 8;

	Render::Outline(x - 10, Parent->MenuBarHeight() - 5, 391, 142, Colors::Menu::BORDER);
	Render::Clear(x - 11, Parent->MenuBarHeight() - 4, 391, 140, Colors::Menu::BACKGROUND);

	static int hue = 0;
	static int saturation = 0;
	static int lightness = 0;
	static bool draggingHue = false;
	static bool draggingSL = false;
	static Color curColor = HSLtoRGB(hue, saturation, lightness);
	//static std::string selectedLabel = colorList[0].label;

	Render::Clear(x - 2, y - 1, 364, 10, Colors::BLACK20);
	for (size_t h = 0; h < 360; h++)
		Render::Line(x + h, y, x + h, y + 8, HSLtoRGB(h, 85, 50));

	if (Parent->KeyPressed(1) && Parent->MouseInArea(x, y, 360, 8))
		draggingHue = true;

	if (draggingHue)
	{
		float ratio = (float)(Parent->m_Cursor.x - x) / (float)(360);
		if (ratio < 0.f) ratio = 0.f;
		if (ratio > 1.f) ratio = 1.f;
		hue = ratio * 360;

		curColor = HSLtoRGB(hue, saturation, lightness);

		if (!Parent->KeyHeld(1))
			draggingHue = false;
	}

	Render::TriangleOutline(Vector2D(x + hue - 4, y - 6), Vector2D(x + hue, y + 2), Vector2D(x + hue + 4, y - 6), Colors::WHITE35, Colors::BLACK40);
	Render::TriangleOutline(Vector2D(x + hue - 4, y + 14), Vector2D(x + hue, y + 6), Vector2D(x + hue + 4, y + 14), Colors::WHITE35, Colors::BLACK40);

	y += 20;

	for (int x2 = x; x2 < x + 100; x2++)
		for (int y2 = y; y2 < y + 100; y2++)
			Render::Line(x2, y2, x2 + 1, y2, HSLtoRGB(hue, x2 - x, 100 - (y2 - y)));

	Render::Outline(x - 1, y, 102, 100, Colors::BLACK);

	if (Parent->KeyPressed(1) && Parent->MouseInArea(x, y, 100, 100))
		draggingSL = true;

	if (draggingSL)
	{
		int dx = Parent->m_Cursor.x - x;
		int dy = Parent->m_Cursor.y - y;

		if (dx < 0) dx = 0;
		if (dx > 100) dx = 100;
		if (dy < 0) dy = 0;
		if (dy > 100) dy = 100;

		saturation = dx;
		lightness = 100 - dy;

		curColor = HSLtoRGB(hue, saturation, lightness);

		if (!Parent->KeyHeld(1))
			draggingSL = false;
	}

	int dx = saturation;
	int dy = 100 - lightness;

	// saturation and lightness xhair
	Render::Line(x + dx, y + dy - 3, x + dx, y + dy - 1, Colors::BLACK);
	Render::Line(x + dx, y + dy + 3, x + dx, y + dy + 1, Colors::BLACK);
	Render::Line(x + dx - 3, y + dy, x + dx - 1, y + dy, Colors::BLACK);
	Render::Line(x + dx + 3, y + dy, x + dx + 1, y + dy, Colors::BLACK);

	x += 116;

	Render::Clear(x, y, 16, 100, curColor);
	Render::Outline(x, y, 16, 100, Colors::BLACK);

	x += 32;

	// if click save, send *selectedColorPicker->color = HSLtoRGB(cur);

	bool saveHover = Parent->MouseInArea(x, y, 64, 24);

	Render::Outline(x, y, 64, 24, Colors::Menu::OUTLINES);
	POINT txtSize = Render::GetTextSize(Fonts::MENU, "Save");
	Render::Text(x + (64 - txtSize.x) / 2, y + (24 - txtSize.y) / 2, Colors::Menu::MENUTEXT, Fonts::MENU, "Save");

	if (Parent->KeyPressed(1) && saveHover)
		Parent->selectedColorPicker->SetColor(curColor);
}

void SkinsTab::CustomRender()
{
	int x = Parent->posX() + 10;
	int y = Parent->posY() + Parent->height() - 120;

	static POINT* location = new POINT(); //tagPOINT[2]{ m_x + 190, m_y + m_MenuBarHeight + 12 };
	location->x = Parent->posX() + 190;
	location->y = Parent->MenuBarHeight() + 6;

	static SearchBox* weapon = new SearchBox("Weapons", Parent->GenWeaponDict(), &Config::Skins::Weapons::SelectedWeapon, 24);
	weapon->Update(location, Parent);

	location->y = Parent->MenuBarHeight() + 6;
	location->x += 180;
	static SearchBox* paintkit = new SearchBox("Paint Kits", SkinParser::k_skins, &Config::Skins::Weapons::SelectedSkin, 24);
	paintkit->Update(location, Parent);

	location->y = Parent->MenuBarHeight() + 6;
	location->x += 180;

	static SearchBox* sticker1 = new SearchBox("Sticker One", SkinParser::k_stickers, &Config::Skins::Weapons::sticker1, 4);
	static SearchBox* sticker2 = new SearchBox("Sticker Two", SkinParser::k_stickers, &Config::Skins::Weapons::sticker2, 4);
	static SearchBox* sticker3 = new SearchBox("Sticker Three", SkinParser::k_stickers, &Config::Skins::Weapons::sticker3, 4);
	static SearchBox* sticker4 = new SearchBox("Sticker Four", SkinParser::k_stickers, &Config::Skins::Weapons::sticker4, 4);

	sticker1->Update(location, Parent);
	location->y += 25;
	sticker2->Update(location, Parent);
	location->y += 25;
	sticker3->Update(location, Parent);
	location->y += 25;
	sticker4->Update(location, Parent);
}