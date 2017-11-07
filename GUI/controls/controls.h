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

#pragma once

#ifndef CONTROLS_H
#define CONTROLS_H

#include <string>
#include <vector>
#include <unordered_map>

#include "color.h"

struct MPoint { int x; int y; };

enum class ControlType : int
{
	UI_Toggle = 1,
	UI_Slider,
	UI_ComboBox,
	UI_KeyBind
};

class CControl
{
public:
	inline void SetPos(int x, int y) { m_x = x; m_y = y; }
	inline void SetConfigName(std::string cfgName) { config_name = cfgName; }
	inline void SetLabel(std::string l) { label = l; }
	inline std::string GetLabel() { return label; }
	inline std::string GetConfigName() { return config_name; }
	bool skip;

	ControlType type;

protected:
	int m_x, m_y;
	int m_iWidth, m_iHeight;
	std::string config_name;
	std::string label;

public:
	virtual void Update(int x, int y, MPoint Mouse, bool* click) = 0;
};

class CContainer : public CControl
{
public:
	CContainer(std::string name);
	CContainer(int x, int y, std::string name);
	void PlaceControl(CControl* control, int r, int c);
	void SelectControl(std::string lbl);
	//CToggle* CreateToggle(std::string, std::string);

protected:
	int m_iC, m_iR;
	std::string selectedLabel;
	std::string title;
	std::unordered_map<int, std::unordered_map<int, CControl*>> controls;

public:
	void Update(int x, int y, MPoint Mouse, bool* click);
};

class CComboBox : public CControl
{
public:
	CComboBox();
	CComboBox(std::string lbl, std::string cfg_name, std::vector<std::string> items, int selectedIndex);
	void SelectIndex(int idx);
	int GetIndex();
	std::string GetItem();

protected:
	std::vector<std::string> Items;
	int SelectedIndex;

public:
	void Update(int x, int y, MPoint Mouse, bool* click);
};

class CToggle : public CControl
{
public:
	CToggle(std::string lbl, std::string cfg_name);
	bool GetBool();
	void SetBool(bool);

protected:
	bool enabled;

public:
	void Update(int x, int y, MPoint Mouse, bool* click);
};

class CSlider : public CControl
{
public:
	CSlider();
	CSlider(float minVal, float maxVal, float val, std::string lbl, std::string cfg_name);
	void SetBoundaries(float minVal, float maxVal);
	void SetValue(float val);
	inline float GetValue() { return value; }

protected:
	float min, max;
	int position;
	float value;

public:
	void Update(int x, int y, MPoint Mouse, bool* click);
};

class CKeyBind : public CControl
{
public:
	CKeyBind(std::string lbl, std::string cfg_name);
	int GetKey();
	void SetKey(int key);

protected:
	int Key;
	bool GettingKey;

public:
	void Update(int x, int y, MPoint Mouse, bool* click);
};

class CSearchBox : public CControl
{
public:
	CSearchBox(std::string lbl, std::string cfgName);
	void AddItem(int value, std::string text);
	inline int GetValue() { return SelectedValue; }

protected:
	int SelectedValue;
	std::vector<std::string> names;
	std::vector<int> values;
	int scrollPosition;

public:
	void Update(int x, int y, MPoint Mouse, bool* click);
};

class CColorPicker : public CControl
{
public:
	CColorPicker();
	CColorPicker(std::string lbl, std::string cfg_name, Color* clr = nullptr);
	inline Color GetColor() { return *color; }
	inline void SetColor(Color col) { *color = col; }

protected:
	int hue;
	int satuartion;
	int lightness;
	Color* color;
	int r;
	int g;
	int b;
	int a;

public:
	void Update(int x, int y, MPoint Mouse, bool* click);
};

class CTab
{
public:
	CTab() = default;
	virtual void Setup() = 0;
	virtual void Draw(int x, int y) = 0;

	CContainer* CreateContainer(std::string);

protected:
	std::vector<CContainer*> containers;
};

#endif