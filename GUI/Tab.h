#pragma once

#include "Group.h"

#include "Button.h"
#include "Checkbox.h"
#include "ComboBox.h"
#include "SearchBox.h"
#include "Slider.h"
#include "Toggle.h"
#include "KeyBind.h"

class Tab
{

public:
	Tab(const char* name) { Name = name; }
	//virtual ~Tab() {}

	Menu* Parent;
	
	const char* Name;

	std::vector<Group*> Groups;
	Group* CreateGroup(const char*);

	virtual void CustomRender() { }
	void Update(POINT* location, POINT mouse);
};

class ColorsTab : public Tab
{
public:
	ColorsTab(const char* name) : Tab(name) {}

	void CustomRender();
};

class SkinsTab : public Tab
{
public:
	SkinsTab(const char* name) : Tab(name) {}

	void CustomRender();
};