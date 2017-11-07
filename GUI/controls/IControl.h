#pragma once

#include <Windows.h>

#include <vector>
#include <string>

#include "tinyxml.h"
#include "render.h"

#include "IMenu.h"
#include "fonts.h"

enum UIControlType : int
{
	uiButton,
	uiCheckBox,
	uiSlider,
	uiToggle,
	uiKeyBind,
	uiComboBox,
	uiSearchBox,
	uiColorPicker
};

/*struct POINT
{
	int x;
	int y;
};*/

typedef bool KeyArray[256];

class IControl
{
public:
	IControl() {}
	virtual ~IControl() {}

	const char* Name;
	UIControlType Type;
	bool Saveable;

	inline std::string SafeName() 
	{ 
		std::string s = std::string(Name);
		size_t idx = s.find(' ');

		while (idx != std::string::npos)
		{
			s = s.substr(0, idx) + '_' + s.substr(idx + 1);
			idx = s.find(' ');
		}

		return s;
	}

	virtual void Load(const char* text) = 0;
	virtual void Save(tinyxml2::XMLElement* element) = 0;
	// TODO: mouse is unnecessary, pass optional bool isSkipCall to prevent double drawing of skipped controls
	virtual void Update(POINT* location, Menu* parent, bool skipCall = false) = 0;
};

bool MouseInArea(POINT, int, int, int, int);