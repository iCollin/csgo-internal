#pragma once

#include "IControl.h"

typedef void Event_t(void);

class Button : public IControl
{
protected:
	Event_t* fn;

public:
	Button(const char* name, Event_t* func)
	{
		Name = name;

		Type = UIControlType::uiButton;
		Saveable = false;

		fn = func;
	}

	void Save(tinyxml2::XMLElement* element);
	void Load(const char* text);
	void Update(POINT* location, Menu* parent, bool skipCall = false);
};