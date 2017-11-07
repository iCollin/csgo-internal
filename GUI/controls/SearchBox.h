#pragma once

#include "IControl.h"

class SearchBox : public IControl
{
protected:
	int* m_Int;
	std::vector<std::pair<int, std::string>> m_Items;

	bool m_EditingFilter;
	std::string m_Filter;
	std::vector<std::pair<int, std::string>> m_FilteredItems;
	int m_ItemHeight, m_Offset;

public:
	SearchBox(const char* name, std::vector<std::pair<int, std::string>> Items, int* SelectedInt, int itemHeight = 9)
	{
		Name = name;
		m_Int = SelectedInt;

		Saveable = false;
		Type = UIControlType::uiSearchBox;

		SetItems(Items);

		m_EditingFilter = false;
		m_Filter = "";
		m_FilteredItems = m_Items;
		m_ItemHeight = itemHeight;
		m_Offset = 0;
	}

	void AddItem(int i, std::string str) { m_Items.push_back(std::pair<int, std::string>(i, str)); }
	void SetItems(std::vector<std::pair<int, std::string>> Items) { m_Items = Items; }

	void UpdateFilteredItems(std::string newFilter);

	void Save(tinyxml2::XMLElement* element);
	void Load(const char* text);
	void Update(POINT* location, Menu* parent, bool skipCall = false);
};