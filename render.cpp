/*
      ___           ___           ___           ___     
     /\  \         |\__\         /\  \         /\__\    
    /::\  \        |:|  |       /::\  \       /::|  |   
   /:/\:\  \       |:|  |      /:/\:\  \     /:|:|  |   
  /:/  \:\  \      |:|__|__   /::\~\:\  \   /:/|:|  |__ 
 /:/__/ \:\__\     /::::\__\ /:/\:\ \:\__\ /:/ |:| /\__\
 \:\  \  \/__/    /:/~~/~~   \/__\:\/:/  / \/__|:|/:/  /
  \:\  \         /:/  /           \::/  /      |:/:/  / 
   \:\  \        \/__/            /:/  /       |::/  /  
    \:\__\                       /:/  /        /:/  /   
     \/__/                       \/__/         \/__/    

revolt (4/2017)
credit: Synraw @ unknowncheats.me
*/

#include "render.h"

#include "utilities.h"
#include <math.h>

#include "fonts.h"

#define MAX_SEGMENTS 180

#define M_PI 3.14159265358979323846264338327950288419716939937510582f
#define D2R(d) (d / 180.f) * M_PI

RECT Render::GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w = 1920, h = 1080;
	Viewport.right = w; Viewport.bottom = h;
	return Viewport;
}

void Render::Clear(int x, int y, int w, int h, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawFilledRect(x, y, x + w, y + h);
}

void Render::Outline(int x, int y, int w, int h, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Render::Line(int x, int y, int x2, int y2, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawLine(x, y, x2, y2);
}

void Render::Line(POINT start, POINT end, Color color)
{
	Line(start.x, start.y, end.x, end.y, color);
}

void Render::LineFadeH(int x, int y, int w, Color color)
{
	int r = color.r;
	int g = color.g;
	int b = color.b;
	for (int i = 0; i < w; i++) {
		Clear(x, y + i, 1, 1, Color(r, g, b, 255 * (i / w)));
	}
}

void Render::PolyLine(int *x, int *y, int count, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawPolyLine(x, y, count);
}

void Render::OutlineCircle(int x, int y, int r, int seg, Color color)
{
	I::Surface->DrawSetColor(color);
	I::Surface->DrawOutlinedCircle(x, y, r, seg);
}

void Render::OutlineCircleSmooth(int x, int y, int r, Color color)
{
	float circum = M_PI * 2.f * r;
	const int seg = std::ceilf(circum);

	if (seg > MAX_SEGMENTS)
	{
		Utilities::Error("Drawing Circle would be too slow. Increase MAX_SEGMENTS or remove this drawing.");
		return;
	}

	float theta = 0.f;
	float thetaStep = 360.0f / seg;
	POINT points[MAX_SEGMENTS];
	//points[0] = { x, y + r };

	for (size_t i = 0; i < seg; i++)
	{
		theta = i * thetaStep;
		points[i] = { x + (long)std::round(r * sin(D2R(theta))), y + (long)std::round(r * cos(D2R(theta))) };
	}

	PointArray(seg, points, color);
}

void Render::OutlinePartialCircle(int x, int y, int r, Color color, float minTheta = 0.f, float maxTheta = 360.f)
{
	float circum = M_PI * 2.f * r;
	int seg = std::ceilf(circum) / 2;

	if (seg > MAX_SEGMENTS)
	{
		Utilities::Error("Drawing Circle would be too slow. Increase MAX_SEGMENTS or remove this drawing.");
		return;
	}

	float theta = 0.f;
	float thetaStep = 360.0f / seg;
	POINT points[MAX_SEGMENTS];
	//points[0] = { x, y + r };

	int firstSeg = std::ceilf(minTheta / thetaStep);
	int lastSeg = std::floorf(maxTheta / thetaStep);
	seg = lastSeg - firstSeg;

	for (size_t i = 0; i < seg; i++)
	{
		theta = (i + firstSeg) * thetaStep;
		points[i] = { x + (long)std::round(r * sin(D2R(theta))), y + (long)std::round(r * cos(D2R(theta))) };
	}

	PointArray(seg, points, color);
}

void Render::Circle(int x, int y, int r, Color color)
{
	float circum = 6 * r; // about lol
	int seg = 3 * r;

	float theta = 0.f;
	float thetaStep = 360.0f / seg;

	static Vertex_t CircleVt[MAX_SEGMENTS];

	for (size_t i = 0; i < seg; i++)
	{
		theta = i * thetaStep;
		CircleVt[i].Init(Vector2D(x + (long)std::round(r * sin(D2R(theta))), y + (long)std::round(r * cos(D2R(theta)))));
	}

	Polygon(seg, CircleVt, color);
}

void Render::CircleOutlined(int x, int y, int r, Color fill, Color outline)
{
	float circum = 6 * r; // about lol
	int seg = 3 * r;

	float theta = 0.f;
	float thetaStep = 360.0f / seg;

	static Vertex_t CircleVt[MAX_SEGMENTS];

	for (size_t i = 0; i < seg; i++)
	{
		theta = i * thetaStep;
		CircleVt[i].Init(Vector2D(x + (long)std::round(r * sin(D2R(theta))), y + (long)std::round(r * cos(D2R(theta)))));
	}

	PolygonOutline(seg, CircleVt, fill, outline);
}

void Render::Triangle(Vector2D one, Vector2D two, Vector2D three, Color fill)
{
	static Vertex_t MouseVt[3];

	MouseVt[0].Init(one);
	MouseVt[1].Init(two);
	MouseVt[2].Init(three);

	Polygon(3, MouseVt, fill);
}

void Render::TriangleOutline(Vector2D one, Vector2D two, Vector2D three, Color fill, Color outline)
{
	static Vertex_t MouseVt[3];

	MouseVt[0].Init(one);
	MouseVt[1].Init(two);
	MouseVt[2].Init(three);

	Render::PolygonOutline(3, MouseVt, fill, outline);
}

void Render::PointArray(size_t count, POINT* ary, Color color)
{
	for (size_t i = 0; i < count - 1; i++)
		Line(ary[i].x, ary[i].y, ary[i+1].x, ary[i+1].y, color);
}

bool Render::WorldToScreen(Vector &in, Vector &out)
{
	const matrix3x4_t& worldToScreen = I::Engine->WorldToScreenMatrix(); //Grab the world to screen matrix from CEngineClient::WorldToScreenMatrix

	float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3]; //Calculate the angle in compareson to the player's camera.
	out.z = 0; //Screen doesn't have a 3rd dimension.

	if (w > 0.001) //If the object is within view.
	{
		RECT ScreenSize = GetViewport();
		float fl1DBw = 1 / w;
		out.x = (ScreenSize.right / 2) + (0.5f * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * fl1DBw) * ScreenSize.right + 0.5f); //Get the X dimension and push it in to the Vector.
		out.y = (ScreenSize.bottom / 2) - (0.5f * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * fl1DBw) * ScreenSize.bottom + 0.5f); //Get the Y dimension and push it in to the Vector.
		return true;
	}

	return false;
}

void Render::Text(int x, int y, Color color, DWORD font, const char* text)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	I::Surface->DrawSetTextFont(font);

	I::Surface->DrawSetTextColor(color);
	I::Surface->DrawSetTextPos(x, y);
	I::Surface->DrawPrintText(wcstring, wcslen(wcstring));
	return;
}

void Render::Text(int x, int y, Color color, DWORD font, const wchar_t* text)
{
	I::Surface->DrawSetTextFont(font);
	I::Surface->DrawSetTextColor(color);
	I::Surface->DrawSetTextPos(x, y);
	I::Surface->DrawPrintText(text, wcslen(text));
}

void Render::Textf(int x, int y, Color color, DWORD font, const char* fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf_s(logBuf + strlen(logBuf), 256 - strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	Text(x, y, color, font, logBuf);
}

POINT Render::GetTextSize(DWORD font, const char* text)
{
	if (!text)
		return { 0, 0 };

	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	POINT p; int x, y;
	I::Surface->GetTextSize(font, wcstring, x, y);
	p.x = x;
	p.y = y;
	return p;
}

//
// Ghetto gradients, replace with some real color blending later on
//
void Render::GradientV(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r;
	BYTE second = c2.g;
	BYTE third = c2.b;
	for (int i = 0; i < h; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		Clear(x, y + i, w, 1, Color(first, second, third, ia));
	}
}

void Render::GradientFadeV(int x, int y, int w, int h, Color c)
{
	BYTE first = c.r;
	BYTE second = c.g;
	BYTE third = c.b;
	for (int i = 0; i < h; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255.f;
		Clear(x, y + i, w, 1, Color(first, second, third, 255 - ia));
	}
}

void Render::GradientH(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r;
	BYTE second = c2.g;
	BYTE third = c2.b;
	float fw = w;
	for (int i = 0; i < w; i++)
	{
		float fi = i;
		float a = fi / fw;
		DWORD ia = a * 255;
		Clear(x + i, y, 1, h, Color(first, second, third, ia));
	}
}

void Render::GradientFadeH(int x, int y, int w, int h, Color c)
{
	BYTE first = c.r;
	BYTE second = c.g;
	BYTE third = c.b;
	float fw = w;
	for (int i = 0; i < w; i++)
	{
		float fi = i;
		float a = fi / fw;
		DWORD ia = a * 255;
		Clear(x + i, y, 1, h, Color(first, second, third, 255 - ia));
	}
}

//
// Thanks sl0w or w/e you wanna be called
//
void Render::Polygon(int count, Vertex_t* Vertexs, Color color)
{
	static int Texture = I::Surface->CreateNewTextureID(true); //need to make a texture with procedural true
	unsigned char buffer[4] = { 255, 255, 255, 255 };//{ color.r(), color.g(), color.b(), color.a() };

	if (!I::Surface->IsTextureIDValid(Texture))
	{
		Texture = I::Surface->CreateNewTextureID(true);
		Utilities::Warn("TextureID to draw polygon was invalid. Creating a new texture ID.\n");
	}

	//I::Surface->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
	I::Surface->DrawSetColor(color); // keep this full color and opacity use the RGBA @top to set values.
	I::Surface->DrawSetTexture(Texture); // bind texture

	I::Surface->DrawTexturedPolygon(count, Vertexs);
}

void Render::PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine)
{
	static int x[128];
	static int y[128];

	Render::Polygon(count, Vertexs, color);

	for (int i = 0; i < count; i++)
	{
		x[i] = Vertexs[i].m_Position.x;
		y[i] = Vertexs[i].m_Position.y;
	}

	Render::PolyLine(x, y, count, colorLine);
}

void Render::PolyLine(int count, Vertex_t* Vertexs, Color colorLine)
{
	static int x[128];
	static int y[128];

	for (int i = 0; i < count; i++)
	{
		x[i] = Vertexs[i].m_Position.x;
		y[i] = Vertexs[i].m_Position.y;
	}

	Render::PolyLine(x, y, count, colorLine);
}
//
//void Render::Button(std::string label, int x, int y, int w, int h)
//{
//	Render::Outline(x, y, w, h, Colors::Menu::BORDER);
//	Render::Outline(x + 1, y + 1, w - 2, h - 2, Colors::Menu::BORDER);
//	Render::Clear(x + 2, y + 2, w - 4, h - 4, Colors::Menu::MENUBAR);
//
//	POINT txtSize = Render::GetTextSize(Fonts::MENU, label.c_str());
//	int txtW = txtSize.x, txtH = txtSize.y;
//
//	Render::Text(x + ((w - txtW) / 2), y + ((h - txtH) / 2) + 1, Colors::Menu::MENUTEXT, Fonts::MENU, label.c_str());
//}
//
//void Render::Toggle(int x, int y, bool enabled)
//{
//	Circle(x + 10, y + 10, 8, enabled ? Colors::Menu::ENABLED : Colors::Menu::DISABLED);
//	Circle(x + 22, y + 10, 8, enabled ? Colors::Menu::ENABLED : Colors::Menu::DISABLED);
//	Clear(x + 10, y + 2, 12, 16, enabled ? Colors::Menu::ENABLED : Colors::Menu::DISABLED);
//
//	if (enabled)
//	{
//		Circle(x + 22, y + 10, 7, Colors::WHITE10);
//		OutlineCircle(x + 22, y + 10, 7, 45, Colors::BLACK15);
//		Line(x + 10, y + 7, x + 10, y + 13, Colors::BLACK40);
//	}
//	else
//	{
//		Circle(x + 10, y + 10, 7, Colors::WHITE10);
//		OutlineCircle(x + 10, y + 10, 7, 45, Colors::BLACK15);
//		OutlineCircle(x + 23, y + 10, 3, 25, Colors::BLACK40);
//	}
//}
//
//void Render::BigToggle(int x, int y, bool enabled)
//{
//	Circle(x + 20, y + 16, 12, enabled ? Colors::Menu::ENABLED : Colors::Menu::DISABLED);
//	Circle(x + 40, y + 16, 12, enabled ? Colors::Menu::ENABLED : Colors::Menu::DISABLED);
//	Clear(x + 20, y + 4, 20, 24, enabled ? Colors::Menu::ENABLED : Colors::Menu::DISABLED);
//
//	if (enabled)
//	{
//		CircleOutlined(x + 39, y + 16, 10, oldColors::TOGGLE_BUTTON, Colors::BLACK15);
//		Line(x + 20, y + 11, x + 20, y + 21, Colors::BLACK35);
//	}
//	else
//	{
//		CircleOutlined(x + 20, y + 16, 10, oldColors::TOGGLE_BUTTON, Colors::BLACK15);
//		CircleOutlined(x + 41, y + 16, 4, Color(1, 1, 1, 0), Colors::BLACK);
//	}
//}
//
//void Render::ComboBox(int x, int y, std::vector<std::string> items, int* selectedIndex, bool hover, bool* skip, bool click, int yMouse)
//{
//	Render::Clear(x, y + 2, 122, 18, oldColors::COMBO_BG);
//	Render::Outline(x, y + 2, 122, 18, Colors::Menu::OUTLINES);
//
//	//Render::Line(x + 100, y + 2, x + 100, y + 20, oldColors::GRPBOX_OUTLINES);
//	//Render::TriangleOutline(Vector2D(x + 105, y + 7), Vector2D(x + 119, y + 7), Vector2D(x + 112, y + 15), oldColors::WHITE40, oldColors::MENU_BORDER);
//
//	//POINT txtSize = GetTextSize(Fonts::MENU, items[selectedIndex].c_str());
//	Render::Text(x + 6, y + 6, Colors::Menu::MENUTEXT, Fonts::MENU, items[*selectedIndex].c_str());
//
//	if (hover && click)
//	{
//		if (*skip)
//			*selectedIndex = (yMouse - (y + 20)) / 18;
//
//		*skip = !*skip;
//	}
//
//	if (*skip)
//	{
//		for (size_t i = 0; i < items.size(); i++)
//		{
//			Render::Clear(x, y + 20 + i * 18, 122, 18, (hover && yMouse > (y + 22 + i * 18) && (y + 22 + i * 18 + 18) > yMouse) ? oldColors::BLACK40 : oldColors::COMBO_BG);
//			Render::Text(x + 6, y + 24 + (18 * i), Colors::Menu::MENUTEXT, Fonts::MENU, items[i].c_str());
//		}
//
//		Render::Outline(x, y + 19, 122, 1 + 18 * items.size(), oldColors::GRPBOX_OUTLINES);
//	}
//}
//
//void Render::GroupBox(int x, int y, int w, int h, std::string title)
//{
//	POINT txtSize = GetTextSize(Fonts::MENU, title.c_str());
//	Text(x + 18, y, Colors::Menu::MENUTEXT, Fonts::MENU, title.c_str());
//
//	y += txtSize.y / 2;
//
//	Line(x, y, x + 15, y, oldColors::GRPBOX_OUTLINES);
//	Line(x, y, x, y + h, oldColors::GRPBOX_OUTLINES);
//	Line(x, y + h, x + w, y + h, oldColors::GRPBOX_OUTLINES);
//	Line(x + w, y + h, x + w, y, oldColors::GRPBOX_OUTLINES);
//	Line(x + w, y, x + 20 + txtSize.x, y, oldColors::GRPBOX_OUTLINES);
//}
//
//void Render::ColorPicker(int x, int y, Color& curColor)
//{
//	
//}
//
//// arrow down into tray (40x40)
//void Render::Icons::Save(int x, int y, Color color)
//{
//	Clear(x + 18, y + 5, 4, 15, color);
//	Triangle(Vector2D(x + 16, y + 20), Vector2D(x + 24, y + 20), Vector2D(x + 20, y + 25), color);
//
//
//	Clear(x + 5, y + 28, 3, 4, color);
//	Clear(x + 32, y + 28, 3, 4, color);
//	Clear(x + 5, y + 32, 30, 3, color);
//}
//
//// floppy disk save icon (40x40)
//void Render::Icons::Save2(int x, int y, Color color)
//{
//	Line(x + 5, y + 5, x + 5, y + 35, color);
//	Line(x + 5, y + 35, x + 35, y + 35, color);
//	Line(x + 35, y + 35, x + 35, y + 12, color);
//	Line(x + 35, y + 12, x + 28, y + 5, color);
//	Line(x + 28, y + 5, x + 5, y + 5, color);
//
//	Line(x + 10, y + 25, x + 30, y + 25, color);
//	Line(x + 10, y + 28, x + 30, y + 28, color);
//	Line(x + 10, y + 31, x + 30, y + 31, color);
//
//	Clear(x + 10, y + 10, 20, 8, color);
//}
//
//// manilla folder icon (40x40)
//void Render::Icons::Load(int x, int y, Color color)
//{
//	const size_t count = 13;
//	static POINT points[count];
//
//	if (points[0].x != x + 5 || points[0].y != y + 35)
//	{
//		points[0] = { x + 5, y + 35 };
//		points[1] = { x + 5, y + 11 };
//		points[2] = { x + 8, y + 11 };
//		points[3] = { x + 10, y + 8 };
//		points[4] = { x + 16, y + 8 };
//		points[5] = { x + 18, y + 11 };
//		points[6] = { x + 30, y + 11 };
//		points[7] = { x + 30, y + 15 }; // intersection
//		points[8] = { x + 35, y + 15 };
//		points[9] = { x + 30, y + 35 };
//		points[10] = { x + 5, y + 35 };
//		points[11] = { x + 12, y + 15 };
//		points[12] = { x + 30, y + 15 };
//	}
//
//	PointArray(count, points, color);
//}
//
//// manilla folder icon (40x40)
//void Render::Icons::Load2(int x, int y, Color color)
//{
//	const size_t count = 16;
//	static POINT points[count];
//
//	if (points[0].x != x + 5 || points[0].y != y + 35)
//	{
//		points[0] = { x + 5, y + 35 }; // replacing last point of Load1 icon ?? (learn c++)
//		points[1] = { x + 5, y + 12 };
//		points[2] = { x + 7, y + 12 };
//		points[3] = { x + 10, y + 9 };
//		points[4] = { x + 15, y + 9 };
//		points[5] = { x + 17, y + 12 };
//		points[6] = { x + 30, y + 12 };
//		points[7] = { x + 30, y + 14 }; // top folder goes behind front folder
//		points[8] = { x + 32, y + 17 };
//		points[9] = { x + 35, y + 17 };
//		points[10] = { x + 30, y + 35 };
//		points[11] = { x + 5, y + 35 };
//		points[12] = { x + 10, y + 17 };
//		points[13] = { x + 23, y + 17 };
//		points[14] = { x + 25, y + 14 };
//		points[15] = { x + 30, y + 14 };
//	}
//
//	PointArray(count, points, color);
//}
//
//// eject icon (40x40)
//void Render::Icons::Unload(int x, int y, Color color)
//{
//	Triangle(Vector2D(x + 10, y + 25), Vector2D(x + 30, y + 25), Vector2D(x + 20, y + 10), color);
//	Clear(x + 10, y + 30, 20, 5, color);
//}
//
//// eject icon (40x40) (outline)
//void Render::Icons::Unload2(int x, int y, Color color)
//{
//	TriangleOutline(Vector2D(x + 10, y + 25), Vector2D(x + 30, y + 25), Vector2D(x + 20, y + 10), Color(0, 0, 0, 0), color);
//	Outline(x + 10, y + 30, 20, 5, color);
//}
//
//// refresh icon (40x40) (skinny)
//void Render::Icons::Reload(int x, int y, Color color)
//{
//	OutlinePartialCircle(x + 20, y + 20, 14, color, 0.f, 270.f);
//	Triangle(Vector2D(x + 20, y + 31), Vector2D(x + 20, y + 37), Vector2D(x + 13, y + 34), color);
//}
//
//// refresh icon (40x40) (thicc)
//void Render::Icons::Reload2(int x, int y, Color color)
//{
//	OutlinePartialCircle(x + 20, y + 20, 11, color, 0.f, 270.f);
//	OutlinePartialCircle(x + 20, y + 20, 12, color, 0.f, 270.f);
//	Triangle(Vector2D(x + 20, y + 26), Vector2D(x + 20, y + 36), Vector2D(x + 13, y + 31), color);
//}
//
