#pragma once

#include "IControl.h"

class Slider : public IControl
{
public:
	float Min;
	float Max;
	float* Value;
	bool m_Dragging;

	Slider(const char* name, float min, float max, float* value)
	{
		Name = name;

		Min = min;
		Max = max;
		Value = value;
		m_Dragging = false;

		Saveable = true;
		Type = UIControlType::uiSlider;
	}

	void Save(tinyxml2::XMLElement* element);
	void Load(const char* text);
	void Update(POINT* location, Menu* parent, bool skipCall = false);
};