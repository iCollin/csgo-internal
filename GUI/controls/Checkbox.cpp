#include "Checkbox.h"
#include "fonts.h"

void Checkbox::Update(POINT* location, Menu* parent, bool skipCall)
{
	Render::Text(location->x, location->y, Colors::Menu::MENUTEXT, Fonts::MENU, Name);
	location->y += 16;
}

void Checkbox::Load(const char* text)
{
	if (!text) return;

	if (!Saveable) return;

	*Enable = text[0] == '1';
}

void Checkbox::Save(tinyxml2::XMLElement* element)
{
	if (!element) return;

	if (!Saveable) return;

	element->SetText(*Enable);
}