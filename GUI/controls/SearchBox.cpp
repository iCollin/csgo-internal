#include "SearchBox.h"
#include "fonts.h"

#include <algorithm>

#define WIDTH 160

void SearchBox::Update(POINT* location, Menu* parent, bool skipCall)
{
	Render::Text(location->x + 2, location->y, Colors::Menu::MENUTEXT, Fonts::MENU, Name);
	location->y += 16;
	Render::Clear(location->x, location->y, WIDTH, 16, Colors::Menu::MENUBAR);
	bool hasFilter = m_Filter.size() > 0;
	Render::Text(location->x + 4, location->y + 2, hasFilter ? Colors::Menu::MENUTEXT : Colors::Menu::MENUTEXT.CustomAlpha(0.6f), Fonts::MENU, hasFilter ? m_Filter.c_str() : m_EditingFilter ? "|" : "Search...");
	Render::Line(location->x, location->y + 16, location->x + WIDTH, location->y + 16, Colors::Menu::OUTLINES);
	Render::Outline(location->x, location->y, WIDTH, (m_ItemHeight + 1) * 16 + 4, Colors::Menu::OUTLINES);
	bool downHover = parent->MouseInArea(location->x, location->y + +(m_ItemHeight + 1) * 16, WIDTH, 4);
	if (downHover) Render::Clear(location->x + 1, location->y + +(m_ItemHeight + 1) * 16, WIDTH - 2, 4, Colors::Menu::MENUBAR);
	if (downHover && parent->KeyPressed(1)) m_Offset++;

	POINT filterSize = Render::GetTextSize(Fonts::MENU, m_Filter.c_str());
	if (filterSize.x > 0)
		Render::Line(location->x + filterSize.x + 6, location->y + 2, location->x + filterSize.x + 6, location->y + 14, Colors::Menu::OUTLINES);

	bool InEditArea = parent->MouseInArea(location->x, location->y, WIDTH, 16);
	if (parent->KeyPressed(1)) m_EditingFilter = InEditArea;

	if (m_EditingFilter)
	{
		if (parent->KeyPressed(8))
			UpdateFilteredItems(m_Filter.substr(0, m_Filter.size() - 1));

		for (size_t i = 47; i < 58; i++)
			if (parent->KeyPressed(i))
				UpdateFilteredItems(m_Filter + (char)i);

		for (size_t i = 65; i < 91; i++)
			if (parent->KeyPressed(i))
				UpdateFilteredItems(m_Filter + (char)i);
	}

	bool hover = parent->MouseInArea(location->x, location->y + 16, WIDTH, m_ItemHeight * 16);
	if (hover && parent->MouseWheelDelta() != 0)
	{ 
		if (parent->MouseWheelDelta() > 0)
		{
			if (m_Offset > 0) m_Offset--;
		}
		else
		{
			if (m_Offset < (m_FilteredItems.size() - m_ItemHeight)) m_Offset++;
		}
	}

	for (size_t i = 0; i < m_ItemHeight; i++)
	{
		location->y += 16;

		if (i + m_Offset >= m_FilteredItems.size())
			continue;

		bool hover = parent->MouseInArea(location->x, location->y, WIDTH, 16);
		if (hover) Render::Clear(location->x + 1, location->y, WIDTH - 2, 16, Colors::Menu::MENUBAR.CustomAlpha(0.3f));
		else if (*m_Int == m_FilteredItems[i + m_Offset].first) Render::Clear(location->x + 1, location->y, WIDTH - 2, 16, Colors::Menu::MENUBAR.CustomAlpha(0.2f));
		Render::Text(location->x + 4, location->y + 2, Colors::Menu::MENUTEXT, Fonts::MENU, m_FilteredItems[i + m_Offset].second.c_str());

		if (hover && parent->KeyPressed(1)) *m_Int = m_FilteredItems[i + m_Offset].first;
	}
}

void SearchBox::Load(const char* text)
{
	// dont know about this one lol
}

std::string str_toupper(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); } );
	return s;
}

void SearchBox::UpdateFilteredItems(std::string newFilter)
{
	if (newFilter == m_Filter)
		return;

	m_Offset = 0;

	if (newFilter.length() < m_Filter.length())
		m_FilteredItems = m_Items;

	for (size_t i = 0; i < m_FilteredItems.size(); i++)
		if (str_toupper(m_FilteredItems.at(i).second).find(newFilter) == std::string::npos)
			m_FilteredItems.erase(m_FilteredItems.begin() + i--); // HACK

	m_Filter = newFilter;
}

void SearchBox::Save(tinyxml2::XMLElement* element)
{
	// nope
}