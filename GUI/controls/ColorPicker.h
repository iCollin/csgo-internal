#pragma once

#include "IControl.h"

class ColorPicker : public IControl
{
protected:
	Color* color;

public:
	ColorPicker(const char* name, Color* col)
	{
		Name = name;

		Type = UIControlType::uiColorPicker;
		Saveable = true;

		color = col;
	}

	void SetColor(Color newColor);

	void Save(tinyxml2::XMLElement* element);
	void Load(const char* text);
	void Update(POINT* location, Menu* parent, bool skipCall = false);
};