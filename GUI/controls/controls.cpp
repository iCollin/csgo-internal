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

#include "controls.h"

#include "render.h"
#include "color.h"
#include "menu.h"

#include "fonts.h"

bool MouseInArea(MPoint Mouse, int x, int y, int w, int h)
{
	return (Mouse.x > x && Mouse.x < x + w) && (Mouse.y > y && Mouse.y < y + h);
}

#pragma region CToggle Control Functions
CToggle::CToggle(std::string lbl, std::string cfg_name)
{
	enabled = false;
	m_iWidth = 32;
	m_iHeight = 20;

	label = lbl;
	config_name = cfg_name;
	skip = false;

	type = ControlType::UI_Toggle;
}

bool CToggle::GetBool()
{
	return enabled;
}

void CToggle::SetBool(bool b)
{
	enabled = b;
}

void CToggle::Update(int x, int y, MPoint Mouse, bool* click)
{
	Render::Text(x + 8, y + 4, Colors::Menu::MENUTEXT, Fonts::MENU, label.c_str());

	x += 164;
	bool hover = MouseInArea(Mouse, x, y, 32, 20);

	Render::Circle(x + 10, y + 10, 8, enabled ? Colors::Menu::ENABLED : Colors::Menu::DISABLED);
	Render::Circle(x + 22, y + 10, 8, enabled ? Colors::Menu::ENABLED : Colors::Menu::DISABLED);
	Render::Clear(x + 10, y + 2, 12, 16, enabled ? Colors::Menu::ENABLED : Colors::Menu::DISABLED);

	if (enabled)
	{
		Render::Circle(x + 22, y + 10, 7, hover ? Colors::WHITE40 : Colors::WHITE10);
		Render::OutlineCircle(x + 22, y + 10, 7, 45, Colors::BLACK15);
		Render::Line(x + 10, y + 7, x + 10, y + 13, Colors::BLACK40);
	}
	else
	{
		Render::Circle(x + 10, y + 10, 7, hover ? Colors::WHITE40 : Colors::WHITE10);
		Render::OutlineCircle(x + 10, y + 10, 7, 45, Colors::BLACK15);
		Render::OutlineCircle(x + 23, y + 10, 3, 25, Colors::BLACK40);
	}

	if (hover && *click)
		enabled = !enabled;
}
#pragma endregion

#pragma region Container Control Funcions
#define CONTAINER_ROW_HEIGHT 24
#define CONTAINER_ROW_WIDTH 200

CContainer::CContainer(std::string name)
{
	m_iR = 0;
	m_iC = 0;
	title = name;
	selectedLabel = "";

	controls.clear();
}

CContainer::CContainer(int x, int y, std::string name)
{
	m_iR = x;
	m_iC = y;
	title = name;
	selectedLabel = "";

	controls.clear();
}

void CContainer::PlaceControl(CControl* control, int r, int c)
{
	controls[r][c] = control;
}

void CContainer::SelectControl(std::string lbl)
{
	selectedLabel = lbl;
}

//CToggle* CContainer::CreateToggle(std::string name, std::string configName)
//{
//	CToggle* toggle = new CToggle(name, configName);
//
//	return toggle;
//}

void CContainer::Update(int x, int y, MPoint Mouse, bool* click)
{
	int w = m_iC * CONTAINER_ROW_WIDTH;
	int h = m_iR * CONTAINER_ROW_HEIGHT + 10;

	POINT txtSize = Render::GetTextSize(Fonts::MENU, title.c_str());
	Render::Text(x + 18, y, Colors::Menu::MENUTEXT, Fonts::MENU, title.c_str());

	y += txtSize.y / 2;

	Render::Line(x, y, x + 15, y, Colors::Menu::OUTLINES);
	Render::Line(x, y, x, y + h, Colors::Menu::OUTLINES);
	Render::Line(x, y + h, x + w, y + h, Colors::Menu::OUTLINES);
	Render::Line(x + w, y + h, x + w, y, Colors::Menu::OUTLINES);
	Render::Line(x + w, y, x + 18 + txtSize.x, y, Colors::Menu::OUTLINES);

	for (auto i = controls.begin(); i != controls.end(); i++)
		for (auto j = controls[i->first].begin(); j != controls[i->first].end(); j++)
			if (!j->second->skip)
				j->second->Update(x + j->first * CONTAINER_ROW_WIDTH, y + i->first * CONTAINER_ROW_HEIGHT + 8, Mouse, click); // TODO send hover down stream

	for (auto i = controls.begin(); i != controls.end(); i++)
		for (auto j = controls[i->first].begin(); j != controls[i->first].end(); j++)
			if (j->second->skip)
				j->second->Update(x + j->first * CONTAINER_ROW_WIDTH, y + i->first * CONTAINER_ROW_HEIGHT + 8, Mouse, click);
}
#pragma endregion

#pragma region Slider Functions
CSlider::CSlider(float minVal, float maxVal, float val, std::string lbl, std::string cfg_name)
{
	min = minVal;
	max = maxVal;
	value = val;
	label = lbl;
	config_name = cfg_name;
	skip = false;

	type = ControlType::UI_Slider;
}

void CSlider::SetBoundaries(float minVal, float maxVal)
{
	min = minVal;
	max = maxVal;
}

void CSlider::SetValue(float val)
{
	value = val < min ? min : val > max ? max : val;
}

void CSlider::Update(int x, int y, MPoint Mouse, bool* click)
{
	//static bool isDragging = false;

	char buffer[24];
	sprintf_s(buffer, value < 10.f ? "%.2f" : value < 100.f ? "%.1f" : "%.0f", value);
	std::string txt = label + ": " + buffer;

	Render::Text(x + 8, y + 4, Colors::Menu::MENUTEXT, Fonts::MENU, txt.c_str());

	x += 64;
	y += 2;

	float ratio = (value - min) / (max - min);
	int pos = ratio * 120;

	Render::Circle(x + 11, y + 7, 3, Colors::Menu::ENABLED);
	Render::Clear(x + 11, y + 4, pos + 3, 6, Colors::Menu::ENABLED);
	Render::Clear(x + 11 + pos, y + 4, 120 - (pos + 3), 6, Colors::Menu::DISABLED);
	Render::Circle(x + 11 + 120 - 3, y + 7, 3, Colors::Menu::DISABLED);

	Render::Circle(x + 10 + pos, y + 7, 5, Colors::Menu::ENABLED);

	if (GetAsyncKeyState(1) & 0x8000 && MouseInArea(Mouse, x + 8, y + 4, 124, 8))
	{
		float ratio = (float)(Mouse.x - (x + 10)) / (float)(120);
		if (ratio < 0.f) ratio = 0.f;
		if (ratio > 1.f) ratio = 1.f;
		float potVal = ratio * (max - min) + min;
		if (potVal >= min && potVal <= max)
			SetValue(potVal);
	}

}
#pragma endregion

#pragma region Combo Box Functions
CComboBox::CComboBox(std::string lbl, std::string cfg_name, std::vector<std::string> items, int selectedIndex)
{
	label = lbl;
	config_name = cfg_name;
	Items = items;
	SelectedIndex = selectedIndex;
	skip = false;

	type = ControlType::UI_ComboBox;
}

void CComboBox::SelectIndex(int idx)
{
	SelectedIndex = idx;
}

int CComboBox::GetIndex()
{
	return SelectedIndex;
}

std::string CComboBox::GetItem()
{
	return Items[SelectedIndex];
}

void CComboBox::Update(int x, int y, MPoint Mouse, bool* click)
{
	Render::Text(x + 8, y + 6, Colors::Menu::MENUTEXT, Fonts::MENU, label.c_str());

	x += 74;
	bool hover = skip ? MouseInArea(Mouse, x, y, 124, 18 * (Items.size() + 1)) : MouseInArea(Mouse, x, y, 124, 18);

	Render::Clear(x, y + 2, 122, 18, Colors::Menu::MENUBAR);
	Render::Outline(x, y + 2, 122, 18, Colors::Menu::OUTLINES);

	Render::Text(x + 6, y + 6, Colors::Menu::MENUTEXT, Fonts::MENU, GetItem().c_str());

	if (hover && *click)
	{
		if (skip)
			SelectedIndex = (Mouse.y - (y + 20)) / 18;

		skip = !skip;
		*click = false;
	}

	if (skip)
	{
		for (size_t i = 0; i < Items.size(); i++)
		{
			Render::Clear(x, y + 20 + i * 18, 122, 18, MouseInArea(Mouse, x, y + 22 + i * 18, 122, 18) ? Colors::BLACK40 : Colors::Menu::MENUBAR);
			Render::Text(x + 6, y + 24 + (18 * i), Colors::Menu::MENUTEXT, Fonts::MENU, Items[i].c_str());
		}

		Render::Outline(x, y + 19, 122, 1 + 18 * Items.size(), Colors::Menu::OUTLINES);
	}
}
#pragma endregion

#pragma region CColorPicker Functions
CColorPicker::CColorPicker(std::string lbl, std::string cfg_name, Color* clr)
{
	color = clr;
	label = lbl;
	config_name = cfg_name;
	a = clr->a;
	skip = false;

	RGBtoHSL(*clr, hue, satuartion, lightness);
}

void CColorPicker::Update(int x, int y, MPoint Mouse, bool* click)
{
	x += 5;

	Render::Text(x, y + 4, Colors::Menu::MENUTEXT, Fonts::MENU, label.c_str());
	Render::Clear(x + 240, y, 80, 18, *color);
	Render::Outline(x + 240, y, 80, 18, Colors::BLACK30);

	if (label == "#undef")
	{
		// box title
		//POINT txtSize = Render::GetTextSize(Fonts::MENU, label.c_str());
		//Render::Text(x + 18, y, Colors::Menu::MENUTEXT, Fonts::MENU, label.c_str());

		//y += txtSize.y / 2;

		int h = 128;
		int w = 373;

		x += w - 40;

		y -= 1;

		// box outline
		Render::Clear(x, y, w, h, Colors::Menu::BACKGROUND);
		Render::Outline(x, y, w, h, Colors::Menu::MENUBAR);

		x += 5;
		y += 8;

		// hue slider
		Render::Clear(x, y, 364, 10, Colors::BLACK);

		// TODO: make this a texture, theyre all identical
		for (size_t h = 0; h < 360; h++)
			Render::Line(x + 2 + h, y + 1, x + 2 + h, y + 9, HSLtoRGB(h, 85, 50));

		x += 2;

		// check for hue changes
		if (GetAsyncKeyState(1) & 0x8000 && MouseInArea(Mouse, x, y, 364, 10))
		{
			float ratio = (float)(Mouse.x - (x + 2)) / (float)(360);
			if (ratio < 0.f) ratio = 0.f;
			if (ratio > 1.f) ratio = 1.f;
			hue = ratio * 360;

			*color = HSLtoRGB(hue, satuartion, lightness);
		}

		// selection bar on hue slider
		Render::Clear(x + hue - 2, y - 1, 4, 14, Colors::WHITE20);
		Render::Clear(x + hue - 1, y, 2, 12, Colors::BLACK);

		y += 16;

		// check for Saturation or lightness changes
		if (GetAsyncKeyState(1) & 0x8000)
		{
			int dx = Mouse.x - x;
			int dy = Mouse.y - y;

			if (0 < dx && dx < 100 && 0 < dy && dy < 100)
			{
				satuartion = dx;
				lightness = 100 - dy;

				*color = HSLtoRGB(hue, satuartion, lightness);
			}
		}

		// satuaration and lightness table
		// TODO: render this once per change as a texture and use a texture if it exists.
		for (int x2 = x; x2 < x + 100; x2++)
			for (int y2 = y; y2 < y + 100; y2++)
				Render::Line(x2, y2, x2 + 1, y2, HSLtoRGB(hue, x2 - x, 100 - (y2 - y)));

		int dx = satuartion;
		int dy = 100 - lightness;

		// saturation and lightness xhair
		Render::Line(x + dx, y + dy - 3, x + dx, y + dy - 1, Colors::BLACK);
		Render::Line(x + dx, y + dy + 3, x + dx, y + dy + 1, Colors::BLACK);
		Render::Line(x + dx - 3, y + dy, x + dx - 1, y + dy, Colors::BLACK);
		Render::Line(x + dx + 3, y + dy, x + dx + 1, y + dy, Colors::BLACK);

		Render::Outline(x - 1, y - 1, 102, 102, Colors::BLACK10);

		//x += 108;
		y -= 1;

		// selected color
		Render::Clear(x + 108, y, 8, 100, *color);
		Render::Outline(x + 108, y, 8, 100, Colors::BLACK);

		// alpha slider
		for (size_t x2 = x + 108 + 16; x2 < x + 108 + 16 + 235; x2++)
			Render::Line(x2, y + 1, x2, y + 13, Color(255, 255, 255, (x2 - (x + 108 + 16)) * 255 / 235));

		// check alpha changes
		if (GetAsyncKeyState(1) & 0x8000 && MouseInArea(Mouse, x + 108 + 16, y + 1, 235, 12))
		{
			float ratio = (float)(Mouse.x - (x + 108 + 16 + 1)) / (float)(230);
			if (ratio < 0.f) ratio = 0.f;
			if (ratio > 1.f) ratio = 1.f;
			a = ratio * 255;
			color->a = a;
		}

		// alpha knob
		Render::Clear(x + 108 + 13 + (a * 235 / 255), y - 1, 4, 16, Colors::BLACK35);
		Render::Clear(x + 108 + 13 + (a * 235 / 255) + 1, y + 1, 2, 12, Colors::WHITE40);

		//  Add Entry Fields for RGB, HSL (maybe sliders)




		x -= 9;
		y -= 25;

		// Close / Save Button
		bool hover = MouseInArea(Mouse, x + w - 91, y + h - 31, 82, 22);
		Render::Outline(x + w - 91, y + h - 31, 82, 22, Colors::Menu::OUTLINES);
		Render::Clear(x + w - 90, y + h - 30, 80, 20, Colors::Menu::BORDER);
		Render::Text(x + w - 63, y + h - 25, hover ? Colors::Menu::ENABLED : Colors::Menu::MENUTEXT, Fonts::MENU, "Save");

		//if (hover && *click)
			//Settings.Colors.Select("");
	}
	//else
		//if (MouseInArea(Mouse, x + 240, y, 80, 18) && *click)
			//Settings.Colors.Select(label);
}
#pragma endregion

#pragma region KeyBind Functions
char* KeyStrings2[254] = { nullptr, "Left Mouse", "Right Mouse", "Control+Break", "Middle Mouse", "Mouse 4", "Mouse 5",
nullptr, "Backspace", "TAB", nullptr, nullptr, nullptr, "ENTER", nullptr, nullptr, "SHIFT", "CTRL", "ALT", "PAUSE",
"CAPS LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "ESC", nullptr, nullptr, nullptr, nullptr, "SPACEBAR",
"PG UP", "PG DOWN", "END", "HOME", "Left", "Up", "Right", "Down", nullptr, "Print", nullptr, "Print Screen", "Insert",
"Delete", nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", "Left Windows", "Right Windows", nullptr, nullptr, nullptr, "NUM 0", "NUM 1", "NUM 2", "NUM 3", "NUM 4", "NUM 5", "NUM 6",
"NUM 7", "NUM 8", "NUM 9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
"F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, "NUM LOCK", "SCROLL LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, "LSHIFT", "RSHIFT", "LCONTROL", "RCONTROL", "LMENU", "RMENU", nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "Next Track", "Previous Track", "Stop", "Play/Pause", nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[{", "\\|", "}]", "'\"", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

CKeyBind::CKeyBind(std::string lbl, std::string cfg_name)
{
	label = lbl;
	config_name = cfg_name;
	Key = -1;
	GettingKey = false;
	skip = false;

	type = ControlType::UI_KeyBind;
}

int CKeyBind::GetKey()
{
	return Key;
}

void CKeyBind::SetKey(int key)
{
	Key = key;
}

void CKeyBind::Update(int x, int y, MPoint Mouse, bool* click)
{
	Render::Text(x + 8, y + 6, Colors::Menu::MENUTEXT, Fonts::MENU, label.c_str());

	x += 74;
	bool hover = MouseInArea(Mouse, x, y, 124, 18);

	Render::Clear(x, y + 2, 122, 18, Colors::Menu::MENUBAR);
	Render::Outline(x, y + 2, 122, 18,Colors::Menu::OUTLINES);

	char* txt = GettingKey ? "?" : Key >= 0 ? KeyStrings2[Key] : "#undef";
	if (!txt) txt = "#undef";
	POINT txtSize = Render::GetTextSize(Fonts::MENU, txt);
	Render::Text(x + ((122 - txtSize.x) / 2), y + 2 + ((18 - txtSize.y) / 2), Colors::Menu::MENUTEXT, Fonts::MENU, txt);

	if (GettingKey)
	{
		for (size_t i = 0; i < 255; i++)
		{
			if (false)
			{
				Key = i == VK_ESCAPE ? -1 : i;

				GettingKey = false;
				return;
			}
		}
	}

	if (hover && *click)
		GettingKey = true;
}
#pragma endregion

CContainer* CTab::CreateContainer(std::string name)
{
	CContainer* con = new CContainer(name);

	return nullptr;
}

CSearchBox::CSearchBox(std::string lbl, std::string cfgName)
{
	label = lbl;
	config_name = cfgName;

	names = std::vector<std::string>();
	values = std::vector<int>();

	SelectedValue = 0;
	scrollPosition = 0;

	AddItem(666, "FIRE SERPENT");
	AddItem(69, "DLORE");
}

void CSearchBox::AddItem(int value, std::string text)
{
	names.push_back(text);
	values.push_back(value);
}

void CSearchBox::Update(int x, int y, MPoint Mouse, bool * click)
{
	Render::Text(x, y, Colors::Menu::MENUTEXT, Fonts::MENU, label.c_str());

	y += 20;

	for (int i = 0; i < ((values.size()) > 10 ? 10 : values.size()); i++)
	{
		Render::Text(x + 5, y + 20 * i, Colors::Menu::MENUTEXT, Fonts::verdana14o, names[scrollPosition + i].c_str());
	}
}
