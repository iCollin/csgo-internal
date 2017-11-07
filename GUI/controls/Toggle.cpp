#include "Toggle.h"
#include "fonts.h"

void Toggle::Update(POINT* location, Menu* parent, bool skipCall)
{
	Render::Text(location->x, location->y, Colors::Menu::MENUTEXT, Fonts::MENU, Name);

	bool hover = parent->MouseInArea(location->x, location->y, 180, 16);

	// TODO if hover showTooltip();

	if (hover && parent->KeyPressed(1)) Flip();

	Render::Circle(location->x + 150, location->y + 8, 6, *Enable ? Colors::Menu::ENABLED : Colors::Menu::DISABLED);
	Render::Circle(location->x + 158, location->y + 8, 6, *Enable ? Colors::Menu::ENABLED : Colors::Menu::DISABLED);
	Render::Clear(location->x + 150, location->y + 2, 8, 12, *Enable ? Colors::Menu::ENABLED : Colors::Menu::DISABLED);

	if (*Enable)
	{
		Render::Circle(location->x + 158, location->y + 8, 5, hover ? Colors::WHITE40 : Colors::WHITE10);
		Render::OutlineCircle(location->x + 158, location->y + 8, 5, 45, Colors::BLACK15);
		Render::Line(location->x + 149, location->y + 6, location->x + 149, location->y + 10, Colors::BLACK40);
	}
	else
	{
		Render::Circle(location->x + 150, location->y + 8, 5, hover ? Colors::WHITE40 : Colors::WHITE10);
		Render::OutlineCircle(location->x + 150, location->y + 8, 5, 45, Colors::BLACK15);
		Render::OutlineCircle(location->x + 159, location->y + 8, 2, 25, Colors::BLACK40);
	}

	location->y += 16;
}

void Toggle::Load(const char* text)
{
	if (!text) return;

	if (!Saveable) return;

	*Enable = text[0] == '1';
}

void Toggle::Save(tinyxml2::XMLElement* element)
{
	if (!element) return;

	if (!Saveable) return;

	element->SetText(*Enable);
}