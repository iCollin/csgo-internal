#include "IMenu.h"
#include "Tab.h"
#include "GUI.h"

#include "esp.h"
#include "skinchanger.h"
#include "SpotifyControls.h"

bool Config::Misc::Spotify::ShowControls = true;
bool Config::Misc::Spotify::ShowCurrentlyPlaying = true;

Tab* Menu::RegisterTab(const char * name)
{
	auto temp = new Tab(name);
	temp->Parent = this;
	Tabs.push_back(temp);
	return temp;
}

void Menu::Skip(void* thisptr, POINT * location)
{
	SkippedControl = thisptr;
	SkippedLocation = *location;
	//ClickOnSkipped = false;

	if (GUI::KeyHeld(1))
	{
		ClickOnSkipped = true;
		GUI::LiftKey(1);
	}
}

bool Menu::HasSkippedControl()
{
	return SkippedControl != nullptr;
}

bool Menu::MouseInArea(int x, int y, int w, int h)
{
	return m_Cursor.x > x && m_Cursor.x < x + w && m_Cursor.y > y && m_Cursor.y < y + h;
}

void Menu::DrawWatermark()
{
	if (I::Engine->IsInGame())
		return;

	static RECT screen = Render::GetViewport();
	static std::string tag("cyan " + Utilities::VersionString);
	Render::Text(screen.left + 6, 3, Colors::Menu::WATERMARK_BACKGROUND, Fonts::MENU, tag.c_str());
	Render::Text(screen.left + 5, 2, Colors::Menu::WATERMARK_FOREGROUND, Fonts::MENU, tag.c_str());
}

bool Menu::KeyHeld(unsigned int key)
{
	return GUI::KeyHeld(key);
}

bool Menu::KeyPressed(unsigned int key)
{
	return GUI::KeyPressed(key);
}

int Menu::MouseWheelDelta()
{
	return GUI::MouseWheelDelta();
}

// TODO move tf outta here dude
std::vector<std::pair<int, std::string>> Menu::GenWeaponDict()
{
	std::vector<std::pair<int, std::string>> temp = {};

	for (size_t i = 0; i < 41; i++)
	{
		std::string s = ESP::FriendlyWeaponName(i);
		if (s.at(0) == '#') continue;

		temp.push_back({ i, s });
	}

	for (size_t i = 60; i < 71; i++)
	{
		std::string s = ESP::FriendlyWeaponName(i);
		if (s.at(0) == '#') continue;

		temp.push_back({ i, s });
	}

	return temp;
}

void Menu::Draw()
{
	if (SelectedTabName == "" && Tabs.size() > 0) SelectedTabName = (char*)Tabs[0]->Name;

	if (KeyPressed(VK_INSERT))
	{
		I::Engine->ClientCmd_Unrestricted(m_Open ? "cl_mouseenable 1" : "cl_mouseenable 0");
		I::InputSystem->EnableInput(m_Open);

		m_Open = !m_Open;
	}

	if (!m_Open) return;

	//  BEGIN DRAWING

	static POINT newCursor;
	GetCursorPos(&newCursor);

	if (m_Dragging)
	{
		m_x += newCursor.x - m_Cursor.x;
		m_y += newCursor.y - m_Cursor.y;
		if (!GUI::KeyHeld(1)) m_Dragging = false;
	}

	m_Cursor = newCursor;

	POINT location = { m_x, m_y };

	// outlines
	Render::Outline(m_x - 1, m_y - 1, m_w + 2, m_h + 2, Colors::Menu::BORDER.CustomAlpha(0.5f));
	Render::Outline(m_x - 2, m_y - 2, m_w + 4, m_h + 4, Colors::Menu::BORDER.CustomAlpha(0.1f));
	Render::Outline(m_x - 3, m_y - 3, m_w + 6, m_h + 6, Colors::Menu::BORDER.CustomAlpha(0.025f));

	// menu background
	Render::Clear(m_x, m_y, m_w, m_h, Colors::Menu::BACKGROUND);
	Render::Clear(m_x, m_y, m_w, m_MenuBarHeight, Colors::Menu::MENUBAR); // menu bar
	Render::Outline(m_x, m_y, m_w, m_h, Colors::Menu::OUTLINES);

	location.y += m_MenuBarHeight - m_TabHeight;

	// tab headers
	POINT cyanSize = Render::GetTextSize(Fonts::MENUBIG, "cyan ");
	Render::Text(location.x + 7, location.y + m_TabHeight - cyanSize.y - 3, Color(255, 5, 255), Fonts::MENUBIG, "cyan");
	location.x += cyanSize.x + 10;
	Render::Line(m_x, location.y + m_TabHeight, m_x + m_w, location.y + m_TabHeight, Colors::Menu::OUTLINES);

	for (auto tab : Tabs)
	{
		POINT txtSize = Render::GetTextSize(Fonts::MENUBIG, tab->Name);
		bool hover = MouseInArea(location.x, location.y, txtSize.x + m_TabSpacing, m_TabHeight);
		bool selected = tab->Name == SelectedTabName;
		if (selected)
		{
			Render::Clear(location.x, location.y + 1, txtSize.x + m_TabSpacing, m_TabHeight, Colors::Menu::BACKGROUND);
			Render::Line(location.x, location.y, location.x, location.y + m_TabHeight, Colors::Menu::OUTLINES);
			Render::Line(location.x, location.y, location.x + txtSize.x + m_TabSpacing, location.y, Colors::Menu::OUTLINES);
			Render::Line(location.x + txtSize.x + m_TabSpacing, location.y, location.x + txtSize.x + m_TabSpacing, location.y + m_TabHeight, Colors::Menu::OUTLINES);
		}
		Render::Text(location.x + m_TabSpacing / 2, location.y + 3, hover ? Color(255, 5, 255) : selected ? Colors::Menu::BORDER : Colors::Menu::MENUTEXT, Fonts::MENUBIG, tab->Name);
		if (KeyPressed(1) && hover) SelectedTabName = (char*)tab->Name;
		location.x += m_TabSpacing + txtSize.x;
	}

	if (MouseInArea(location.x, m_y, m_x + m_w - location.x, m_MenuBarHeight) && GUI::KeyPressed(1)) m_Dragging = true;

	// draw bottom bar with config changer
	Render::Clear(m_x + 1, m_y + m_h - m_TabHeight - 1, m_w - 2, m_TabHeight, Color(0, 151, 255));

	static ComboBox* selectedConfig = new ComboBox("", false, &m_ConfigIndex, configNames);
	POINT temp = { m_x + m_w - 65, m_y + m_h - m_TabHeight + 2 };
	selectedConfig->ConfigUpdate(&temp, this);

	bool saveHover = MouseInArea(m_x + m_w - 145, m_y + m_h - m_TabHeight, 40, m_TabHeight);
	Render::Text(m_x + m_w - 145, m_y + m_h - m_TabHeight + 2, saveHover ? Color(255, 15, 255) : Colors::Menu::MENUTEXT, Fonts::MENUBIG, "SAVE");
	if (saveHover && KeyPressed(1)) GUI::Save();
	bool loadHover = MouseInArea(m_x + m_w - 105, m_y + m_h - m_TabHeight, 40, m_TabHeight);
	Render::Text(m_x + m_w - 105, m_y + m_h - m_TabHeight + 2, loadHover ? Color(255, 15, 255) : Colors::Menu::MENUTEXT, Fonts::MENUBIG, "LOAD");
	if (loadHover && KeyPressed(1)) GUI::Load();

	// draw spotify controls
	POINT ref = { m_x, m_y + m_h };
	static Spotify* spotify = new Spotify();
	if (spotify->_IsAttached())
	{
		std::wstring curSong = spotify->GetCurrentSong();

		bool backHover = MouseInArea(ref.x + 3, ref.y - 20, 14, 20);
		if (backHover && KeyPressed(1))
			spotify->Previous();
		Vertex_t backArrow[] = { Vertex_t(Vector2D(ref.x + 5, ref.y - 10)), Vertex_t(Vector2D(ref.x + 9, ref.y - 15)), Vertex_t(Vector2D(ref.x + 9, ref.y - 5)) };
		Render::PolygonOutline(3, backArrow, backHover ? Colors::SpotifyControls::CONTROLSHOVER : Colors::SpotifyControls::CONTROLS, Colors::SpotifyControls::CONTROLS);
		Render::Clear(ref.x + 12, ref.y - 15, 2, 11, Colors::SpotifyControls::CONTROLS);

		ref.x += 4;

		bool pauseHover = MouseInArea(ref.x + 16, ref.y - 20, 14, 20);
		if (pauseHover && KeyPressed(1))
			spotify->Pause();

		if (curSong.find(L"Paused") == std::wstring::npos)
		{
			Render::Clear(ref.x + 18, ref.y - 15, 4, 11, pauseHover ? Colors::SpotifyControls::CONTROLSHOVER : Colors::SpotifyControls::CONTROLS);
			Render::Clear(ref.x + 24, ref.y - 15, 4, 11, pauseHover ? Colors::SpotifyControls::CONTROLSHOVER : Colors::SpotifyControls::CONTROLS);
		}
		else
		{
			Vertex_t playButton[] = { Vertex_t(Vector2D(ref.x + 20, ref.y - 15)), Vertex_t(Vector2D(ref.x + 20, ref.y - 5)), Vertex_t(Vector2D(ref.x + 26, ref.y - 10)) };
			Render::PolygonOutline(3, playButton, pauseHover ? Colors::SpotifyControls::CONTROLSHOVER : Colors::SpotifyControls::CONTROLS, Colors::SpotifyControls::CONTROLS);
		}

		ref.x += 4;

		bool nextHover = MouseInArea(ref.x + 29, ref.y - 20, 14, 20);
		if (nextHover && KeyPressed(1))
			spotify->Next();
		Vertex_t skipArrow[] = { Vertex_t(Vector2D(ref.x + 41, ref.y - 10)), Vertex_t(Vector2D(ref.x + 37, ref.y - 15)), Vertex_t(Vector2D(ref.x + 37, ref.y - 5)) };
		Render::PolygonOutline(3, skipArrow, nextHover ? Colors::SpotifyControls::CONTROLSHOVER : Colors::SpotifyControls::CONTROLS, Colors::SpotifyControls::CONTROLS);
		Render::Clear(ref.x + 33, ref.y - 15, 2, 11, Colors::SpotifyControls::CONTROLS);

		Render::Text(ref.x + 50, ref.y - 17, Colors::SpotifyControls::SONGTITLE, Fonts::SPOTIFY, curSong.c_str());
	}

	// draw the actual tab
	location.x = m_x + 10;
	location.y = m_y + m_MenuBarHeight + 4;
	for (auto tab : Tabs)
		if (tab->Name == SelectedTabName)
			tab->Update(&location, m_Cursor);

	location.x = m_x + 10;
	location.y = m_y + m_MenuBarHeight + 4;

	if (SkippedControl)
	{
		if (ClickOnSkipped) GUI::PressKey(1);
		reinterpret_cast<IControl*>(SkippedControl)->Update(&SkippedLocation, this, true);
		ClickOnSkipped = false;
		SkippedControl = nullptr;
	}

	// draw cursor
	Render::Circle(m_Cursor.x, m_Cursor.y, 4, Colors::Menu::CURSOR_BACKGROUND);
	Render::Circle(m_Cursor.x, m_Cursor.y, 3, Colors::Menu::CURSOR_FOREGROUND);
}