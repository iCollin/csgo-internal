#include "IControl.h"

bool MouseInArea(POINT m_Cursor, int x, int y, int w, int h)
{
		return m_Cursor.x > x && m_Cursor.x < x + w && m_Cursor.y > y && m_Cursor.y < y + h;
}
