#pragma once

#include "IControl.h"

class ComboBox : public IControl
{
protected:
	int* m_Index;
	std::vector<std::string> m_Items;
	bool m_Open;

public:
	ComboBox(const char* name, bool saveable, int* idx, std::vector<std::string> strs)
	{
		Name = name;
		m_Index = idx;
		m_Open = false;

		Saveable = saveable;
		Type = UIControlType::uiComboBox;
		
		m_Items = strs;
	}

	inline void AddItems(std::vector<std::string> strs) { for (auto str : strs) { m_Items.push_back(str); } }

	inline void SetItems(std::vector<std::string> strs) 
	{ 
		if (*m_Index > strs.size())
			*m_Index = 0;
		m_Items = strs; 
	}

	void Save(tinyxml2::XMLElement* element);
	void Load(const char* text);
	void Update(POINT* location, Menu* parent, bool skipCall = false);
	void ConfigUpdate(POINT* location, Menu* parent);
};