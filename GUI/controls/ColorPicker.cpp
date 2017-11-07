#include "ColorPicker.h"

void ColorPicker::SetColor(Color newColor)
{
	*color = newColor;
}

void ColorPicker::Save(tinyxml2::XMLElement * element)
{
	std::string rgba = std::to_string(color->r) + "," + std::to_string(color->g) + "," + std::to_string(color->b) + "," + std::to_string(color->a);
	element->SetText(rgba.c_str());
}

void ColorPicker::Load(const char * text)
{
	std::string rgba(text);

	color->r = atoi(rgba.substr(0, rgba.find(',')).c_str());
	rgba = rgba.substr(rgba.find(',') + 1);

	color->g = atoi(rgba.substr(0, rgba.find(',')).c_str());
	rgba = rgba.substr(rgba.find(',') + 1);

	color->b = atoi(rgba.substr(0, rgba.find(',')).c_str());
	rgba = rgba.substr(rgba.find(',') + 1);

	color->a = atoi(rgba.c_str());
}

void ColorPicker::Update(POINT * location, Menu * parent, bool skipCall)
{
	if (parent->selectedColorPicker == this)
	{
		Render::Clear(location->x, location->y + 2, 8, 8, Colors::Menu::ENABLED);
		Render::Text(location->x + 14, location->y, Colors::Menu::ENABLED, Fonts::MENU, Name);
	}
	else
		Render::Text(location->x + 14, location->y, Colors::Menu::MENUTEXT, Fonts::MENU, Name);

	Render::Outline(location->x - 2, location->y, 12, 12, Colors::Menu::OUTLINES);
	Render::Clear(location->x + 100, location->y, 60, 12, *color);
	Render::Outline(location->x + 100, location->y, 60, 12, Colors::Menu::OUTLINES);

	if (parent->KeyPressed(1) && parent->MouseInArea(location->x - 4, location->y, 180, 16))
		parent->selectedColorPicker = this;

	location->y += 16;
}
