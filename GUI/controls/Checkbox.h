#pragma once

#include "IControl.h"

class Checkbox : public IControl
{
public:
	bool* Enable;

	Checkbox(const char* name, bool saveable, bool* toggle)
	{
		Name = name;
		Saveable = saveable;

		Type = UIControlType::uiToggle;

		Enable = toggle;
	}

	inline void Flip() { *Enable = !*Enable; }

	void Save(tinyxml2::XMLElement* element);
	void Load(const char* text);
	void Update(POINT* location, Menu* parent, bool skipCall = false);
};