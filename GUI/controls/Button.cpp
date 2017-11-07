#include "Button.h"
#include "fonts.h"

void Button::Update(POINT* location, Menu* parent, bool skipCall)
{
	//Render::Text(location->x, location->y, Colors::Menu::MENUTEXT, Fonts::MENU, Name);
	bool hover = parent->MouseInArea(location->x, location->y, 170, 16);

	Render::Outline(location->x, location->y + 2, 170, 16, Colors::Menu::OUTLINES);

	POINT txtSize = Render::GetTextSize(Fonts::MENU, Name);
	Render::Text(location->x + (170 - txtSize.x) / 2, location->y + 2 + (16 - txtSize.y) / 2, hover ? Colors::Menu::HOVERTEXT : Colors::Menu::MENUTEXT, Fonts::MENU, Name);

	if (hover && parent->KeyPressed(1)) fn();

	location->y += 20;
}

void Button::Load(const char* text)
{
	// not a thing bruh
}

void Button::Save(tinyxml2::XMLElement* element)
{
	// also not a thing
}