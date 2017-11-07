#pragma once

#include <vector>
#include "Windows.h"
#include "color.h"

class Tab;
class ColorPicker;
class SkinTab;
typedef bool KeyArray[256];

class Menu
{
protected:
	int m_x;
	int m_y;
	int m_w;
	int m_h;

	int m_MenuBarHeight;
	int m_TabHeight;
	int m_TabSpacing;

	bool m_Open;
	bool m_Dragging;

	char* SelectedTabName;

	int m_ConfigIndex;
	const std::vector<std::string> configNames = { "LEGIT", "DEV", "RAGE", "HVH" };

	void* SkippedControl;
	POINT SkippedLocation;
	bool ClickOnSkipped;

	//void DrawColorsTab();
	//void UpdateSkinsTab();
public:
	Menu()
	{
		m_x = 420;
		m_y = 280;
		m_w = 720;
		m_h = 480;
		SelectedTabName = "";

		m_ConfigIndex = 0;
		m_TabHeight = 18;
		m_MenuBarHeight = 22;
		m_TabSpacing = 12;
		selectedColorPicker = nullptr;
	}

	std::vector<Tab*> Tabs;
	Tab* RegisterTab(const char* name);

	template <class T, typename = typename std::enable_if<std::is_base_of<Tab, T>::value, T>::type>
	T* Menu::RegisterCustomTab(const char * name)
	{
		T* temp = new T(name);
		temp->Parent = this;
		Tabs.push_back(temp);
		return temp;
	}

	POINT m_Cursor;
	ColorPicker* selectedColorPicker;

	void Draw();
	void DrawWatermark();

	void Skip(void* thisptr, POINT* location);
	bool HasSkippedControl();
	bool MouseInArea(int x, int y, int w, int h);
	bool KeyPressed(unsigned int key);
	bool KeyHeld(unsigned int key);
	int MouseWheelDelta();

	inline int posX() { return m_x; }
	inline int posY() { return m_y; }
	inline int width() { return m_w; }
	inline int height() { return m_h; }
	inline int MenuBarHeight() { return m_y + 4 + m_MenuBarHeight; }
	inline std::string GetConfigName() { return configNames[m_ConfigIndex]; }
	bool isSkinTab() { return SelectedTabName == "SKINS"; }
	std::vector<std::pair<int, std::string>> GenWeaponDict();
};