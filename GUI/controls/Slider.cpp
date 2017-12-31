#include "Slider.h"
#include "fonts.h"

void Slider::Update(POINT* location, Menu* parent, bool skipCall)
{
	Render::Text(location->x, location->y, Colors::Menu::MENUTEXT, Fonts::MENU, Name);
	char buffer[24];
	sprintf_s(buffer, "%.3f", *Value);
	Render::Text(location->x + 165 - Render::GetTextSize(Fonts::MENU, buffer).x, location->y, Colors::Menu::MENUTEXT, Fonts::MENU, buffer);

	bool hover = parent->MouseInArea(location->x, location->y, 175, 28);

	location->x += 15;
	location->y += 20;

	if (m_Dragging)
	{
		float ratio = (float)(parent->m_Cursor.x - location->x) / 140.f;
		if (ratio < 0.f) ratio = 0.f;
		if (ratio > 1.f) ratio = 1.f;
		*Value = Min + ((Max - Min) * ratio);
		m_Dragging = parent->KeyHeld(1);
	}

	int pos = ((*Value - Min) / (Max - Min)) * 140.f;

	Render::Circle(location->x, location->y, 3, Colors::Menu::ENABLED);
	Render::Clear(location->x, location->y - 3, pos, 6, Colors::Menu::ENABLED);

	Render::Clear(location->x + pos, location->y - 3, 140 - pos, 6, Colors::Menu::DISABLED);
	Render::Circle(location->x + 140, location->y, 3, Colors::Menu::DISABLED);

	Render::Circle(location->x + pos, location->y, 3, Colors::Menu::ENABLED);

	if (hover) Render::Circle(location->x + pos, location->y, 5, Colors::Menu::ENABLED);

	if (hover && parent->KeyPressed(1)) m_Dragging = true;

	location->x -= 15;

	location->y += 8;
}

void Slider::Load(const char* text)
{
	if (!text) return;

	if (!Saveable) return;

	*Value = std::atof(text);
}

void Slider::Save(tinyxml2::XMLElement* element)
{
	if (!element) return;

	if (!Saveable) return;

	element->SetText(*Value);
}