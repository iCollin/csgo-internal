#include "speclist.h"

#include "fonts.h"
#include "render.h"
#include "settings.h"
#include "globals.h"

bool Config::Misc::SpectatorList::Enable = true;

std::vector<std::string> spectators = {};
bool m_Dragging = false;
int m_x = 10, m_y = 420, m_w = 120, m_h = 40;
int m_MenuBarHeight = 22;
POINT m_Cursor;

bool MouseInArea(int x, int y, int w, int h)
{
	return m_Cursor.x > x && m_Cursor.x < x + w && m_Cursor.y > y && m_Cursor.y < y + h;
}

void SpectatorList::PaintTraverse()
{
	if (!Config::Misc::SpectatorList::Enable)
		return;

	if (!G::LocalPlayer || !G::LocalPlayer->GetAlive())
		return;

	spectators.clear();
	for (size_t i = 0; i < 64/*I::EntList->GetHighestEntityIndex()*/; i++)
	{
		C_BasePlayer* entity = I::EntList->GetClientEntity(i);

		if (!entity || entity->GetAlive())
			continue;

		C_BasePlayer* specTarget = I::EntList->GetClientEntityFromHandle(entity->GetObserverTarget());

		if (specTarget != G::LocalPlayer)
			continue;

		IEngineClient::player_info_t info;
		if (!I::Engine->GetPlayerInfo(i, &info))
			continue;

		spectators.push_back(info.name);
	}

	POINT newCursor;
	GetCursorPos(&newCursor);

	if (m_Dragging)
	{
		m_x += newCursor.x - m_Cursor.x;
		m_y += newCursor.y - m_Cursor.y;
		m_Dragging = false;
	}

	m_Cursor = newCursor;

	if (MouseInArea(m_x, m_y, m_w, m_MenuBarHeight) && (GetAsyncKeyState(1) & 0x8000)) m_Dragging = true;

	m_h = 22 + spectators.size() * 24;

	// outlines
	Render::Outline(m_x - 1, m_y - 1, m_w + 2, m_h + 2, Colors::Menu::BORDER.CustomAlpha(0.5f));
	Render::Outline(m_x - 2, m_y - 2, m_w + 4, m_h + 4, Colors::Menu::BORDER.CustomAlpha(0.1f));
	Render::Outline(m_x - 3, m_y - 3, m_w + 6, m_h + 6, Colors::Menu::BORDER.CustomAlpha(0.025f));

	// menu background
	Render::Clear(m_x, m_y, m_w, m_h, Colors::Menu::BACKGROUND);
	Render::Clear(m_x, m_y, m_w, m_MenuBarHeight, Colors::Menu::MENUBAR); // menu bar
	Render::Outline(m_x, m_y, m_w, m_h, Colors::Menu::OUTLINES);

	static POINT titleSize = Render::GetTextSize(Fonts::MENU, "Spectator List");
	Render::Text(m_x + 4, m_y + (m_MenuBarHeight - (int)titleSize.y) / 2, Color(255, 0, 255), Fonts::MENU, "Spectator List");

	if (spectators.size() == 0)
		return;

	POINT txtSize = Render::GetTextSize(Fonts::MENU, "HELLO");

	for (size_t i = 0; i < spectators.size(); i++)
		Render::Text(m_x + 4, m_y + 22 + 24 * i + (24 - txtSize.y) / 2, Colors::ESP::ESPTEXT, Fonts::MENU, spectators[i].c_str());
}
