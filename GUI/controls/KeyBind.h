#pragma once

#include "IControl.h"

class KeyBind : public IControl
{
public:
	bool WaitingForKey;
	int* Key;

	KeyBind(const char* name, int* key)
	{
		Name = name;

		Type = UIControlType::uiKeyBind;
		Saveable = true;

		Key = key;
		WaitingForKey = false;
	}

	void Save(tinyxml2::XMLElement* element);
	void Load(const char* text);
	void Update(POINT* location, Menu* parent, bool skipCall = false);
};