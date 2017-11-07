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

#pragma once

#ifndef RENDER_H
#define RENDER_H

#include <string>

#include "color.h"
#include "interfaces.h"

namespace Render
{
	// Normal Drawing functions
	void Clear(int x, int y, int w, int h, Color color);
	void Outline(int x, int y, int w, int h, Color color);
	void Line(int x, int y, int x2, int y2, Color color);
	void Line(POINT start, POINT end, Color color);
	void LineFadeH(int x, int y, int w, Color color);
	void PolyLine(int *x, int *y, int count, Color color);
	void Polygon(int count, Vertex_t* Vertexs, Color color);
	void PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine);
	void PolyLine(int count, Vertex_t* Vertexs, Color colorLine);
	void Circle(int x, int y, int r, Color color);
	void CircleOutlined(int x, int y, int r, Color fill, Color outline);
	void OutlineCircle(int x, int y, int r, int seg, Color color);
	void OutlineCircleSmooth(int x, int y, int r, Color color);
	void OutlinePartialCircle(int x, int y, int r, Color color, float minTheta, float maxTheta);
	void Triangle(Vector2D one, Vector2D two, Vector2D three, Color fill);
	void TriangleOutline(Vector2D one, Vector2D two, Vector2D three, Color fill, Color outline);

	// Drawing Helpers
	void PointArray(size_t count, POINT* ary, Color color);

	// Gradient Functions
	void GradientV(int x, int y, int w, int h, Color c1, Color c2);
	void GradientH(int x, int y, int w, int h, Color c1, Color c2);
	void GradientFadeV(int x, int y, int w, int h, Color c);
	void GradientFadeH(int x, int y, int w, int h, Color c);

	// Text functions
	//namespace Fonts
	//{
	//	extern DWORD Impact;
	//	extern DWORD Verdana;
	//	extern DWORD bigVerdana;
	//	extern DWORD titleFont;
	//	extern DWORD tabFont;

	//	extern DWORD tahoma12;
	//	extern DWORD tahoma14;
	//	extern DWORD tahoma16;
	//	extern DWORD tahoma18;
	//	extern DWORD tahoma20;

	//	extern DWORD tahoma12o;
	//	extern DWORD tahoma14o;
	//	extern DWORD tahoma16o;
	//	extern DWORD tahoma18o;
	//	extern DWORD tahoma20o;

	//	extern DWORD verdana12;
	//	extern DWORD verdana14;
	//	extern DWORD verdana16;
	//	extern DWORD verdana18;
	//	extern DWORD verdana20;

	//	extern DWORD verdana12o;
	//	extern DWORD verdana14o;
	//	extern DWORD verdana16o;
	//	extern DWORD verdana18o;
	//	extern DWORD verdana20o;

	//	extern DWORD MENUTEXTBIG;
	//	extern DWORD MENUTEXT;
	//	extern DWORD lucida10o;

	//	extern DWORD lucida18o;
	//};

	void Text(int x, int y, Color color, DWORD font, const char* text);
	void Text(int x, int y, Color color, DWORD font, const wchar_t* text);
	void Textf(int x, int y, Color color, DWORD font, const char* fmt, ...);
	POINT GetTextSize(DWORD font, const char* text);

	// Other rendering functions
	bool WorldToScreen(Vector &in, Vector &out);
	RECT GetViewport();

	/*void Button(std::string label, int x, int y, int w, int h);
	void Toggle(int x, int y, bool enabled);
	void BigToggle(int x, int y, bool enabled);
	void ComboBox(int x, int y, std::vector<std::string> items, int* selectedIndex, bool hover, bool* open, bool click, int yMouse);
	void GroupBox(int x, int y, int w, int h, std::string title);
	void ColorPicker(int x, int y, Color& curColor);

	namespace Icons
	{
		void Save(int x, int y, Color color);
		void Save2(int x, int y, Color color);
		void Load(int x, int y, Color color);
		void Load2(int x, int y, Color color);

		void Reload(int x, int y, Color color);
		void Reload2(int x, int y, Color color);
		void Unload(int x, int y, Color color);
		void Unload2(int x, int y, Color color);
	}*/
};

#endif // !RENDER_H