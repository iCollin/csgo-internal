#include "ComboBox.h"
#include "fonts.h"

void ComboBox::Update(POINT* location, Menu* parent, bool skipCall)
{
	Render::Text(location->x, location->y, Colors::Menu::MENUTEXT, Fonts::MENU, Name);

	bool hover = parent->MouseInArea(location->x, location->y, 170, 16);

	Render::Text(location->x + 165 - Render::GetTextSize(Fonts::MENU, m_Items[*m_Index].c_str()).x, location->y, Colors::Menu::MENUTEXT, Fonts::MENU, m_Items[*m_Index].c_str());
	Render::Outline(location->x + 70, location->y, 100, 16,Colors::Menu::MENUTEXT);

	if (m_Open)
	{
		if (!skipCall) parent->Skip(this, location);

		location->x += 80;

		for (auto item : m_Items)
		{
			location->y += 16;
			Render::Clear(location->x, location->y, 90, 16, Colors::Menu::BACKGROUND);
			Render::Text(location->x + 85 - Render::GetTextSize(Fonts::MENU, item.c_str()).x, location->y, Colors::Menu::MENUTEXT, Fonts::MENU, item.c_str());
			Render::Outline(location->x, location->y, 90, 16, Colors::Menu::OUTLINES);
		}

		location->y -= 16 * m_Items.size();
		if (parent->KeyPressed(1))
		{
			if (parent->MouseInArea(location->x, location->y, 90, 16))
				m_Open = false;
			else if (parent->MouseInArea(location->x, location->y + 16, 90, m_Items.size() * 16))
			{
				m_Open = false;
				*m_Index = ((float)(parent->m_Cursor.y - location->y) / (float)(16 * m_Items.size())) * m_Items.size() - 1; // float ratio
			}
		}

		location->x -= 80;
	}
	else if (hover && parent->KeyPressed(1))
		m_Open = true;

	location->y += 16;
}

void ComboBox::Load(const char* text)
{
	if (!text) return;

	if (!Saveable) return;

	*m_Index = std::atoi(text);
}

void ComboBox::Save(tinyxml2::XMLElement* element)
{
	if (!element) return;

	if (!Saveable) return;

	element->SetText(*m_Index);
}

void ComboBox::ConfigUpdate(POINT* location, Menu* parent)
{
	//Render::Text(location->x, location->y, Colors::Menu::MENUTEXT, Fonts::MENUBIG, Name);

	//bool hover = parent->MouseInArea(location->x, location->y, 60, 16);
	//Render::Outline(location->x - 2, location->y - 1, 90, 14, Colors::OUTLINES);

	Render::Text(location->x + 55 - Render::GetTextSize(Fonts::MENUBIG, m_Items[*m_Index].c_str()).x, location->y, Colors::Menu::MENUTEXT, Fonts::MENUBIG, m_Items[*m_Index].c_str());

	location->y -= 4;
	location->x -= 4;

	bool upHover = parent->MouseInArea(location->x, location->y, 10, 24);
	if (upHover && parent->KeyPressed(1) && *m_Index < (m_Items.size() - 1))
		*m_Index += 1;

	Vertex_t upArrow[] = { Vertex_t(Vector2D(location->x + 1, location->y + 14)), Vertex_t(Vector2D(location->x + 5, location->y + 6)), Vertex_t(Vector2D(location->x + 9, location->y + 14)) };
	Render::PolygonOutline(3, upArrow, upHover ? Colors::RED : Colors::BLACK40, Colors::BLACK10);

	bool downHover = parent->MouseInArea(location->x + 10, location->y, 10, 24);
	if (downHover && parent->KeyPressed(1) && *m_Index > 0)
		*m_Index -= 1;

	Vertex_t downArrow[] = { Vertex_t(Vector2D(location->x + 11, location->y + 6)), Vertex_t(Vector2D(location->x + 15, location->y + 14)), Vertex_t(Vector2D(location->x + 19, location->y + 6)) };
	Render::PolygonOutline(3, downArrow, downHover ? Colors::RED : Colors::BLACK40, Colors::BLACK10);
}