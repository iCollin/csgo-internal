#include "Tab.h"

#include "GUI.h"
#include "render.h"
#include "fonts.h"

Button* Group::CreateButton(const char* name, Event_t func)
{
	auto temp = new Button(name, func);
	Controls.push_back(temp);
	return temp;
}

Checkbox* Group::CreateCheckbox(const char* name, bool saveable, bool* toggle)
{
	auto temp = new Checkbox(name, saveable, toggle);
	Controls.push_back(temp);
	return temp;
}

Slider* Group::CreateSlider(const char* name, float min, float max, float* value)
{
	auto temp = new Slider(name, min, max, value);
	Controls.push_back(temp);
	return temp;
}

Toggle* Group::CreateToggle(const char* name, bool saveable, bool* toggle)
{
	auto temp = new Toggle(name, saveable, toggle);
	Controls.push_back(temp);
	return temp;
}

ComboBox* Group::CreateComboBox(const char* name, bool saveable, int* idx, std::vector<std::string> strs)
{
	auto temp = new ComboBox(name, saveable, idx, strs);
	Controls.push_back(temp);
	return temp;
}

SearchBox* Group::CreateSearchBox(const char* name, std::vector<std::pair<int, std::string>> items, int* id)
{
	auto temp = new SearchBox(name, items, id);
	Controls.push_back(temp);
	return temp;
}

KeyBind* Group::CreateKeyBind(const char* name, int* key)
{
	auto temp = new KeyBind(name, key);
	Controls.push_back(temp);
	return temp;
}

ColorPicker* Group::CreateColorPicker(const char* name, Color* col)
{
	auto temp = new ColorPicker(name, col);
	Controls.push_back(temp);
	return temp;
}

void Group::Update(POINT* location, POINT mouse, Menu* parent)
{
	// hardcoded menu. if we ever use multiple menus this needs to change to look up to the parent
	if (Controls.size() * 20 > parent->height() - (location->y - parent->posY() + 10))
	{
		location->x += 180;
		location->y = parent->MenuBarHeight();
	}

	// GROUP WIDTH: 180
	Render::Text(location->x, location->y, Colors::Menu::MENUTEXT, Fonts::MENUBIG, Name);
	location->y += Render::GetTextSize(Fonts::MENUBIG, Name).y + 4;
	Render::Line(location->x - 5, location->y - 4, location->x + 165, location->y - 4, Colors::Menu::MENUBAR); // color in menu.cpp OUTLINES

	for (auto control : Controls)
		control->Update(location, parent);

	location->y += 20;
}