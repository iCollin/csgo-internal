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
credit: AimTux (https://github.com/AimTuxOfficial/AimTux)
*/

#pragma once

#include "Vector2D.h"
#include "IAppSystem.h"

struct Vertex_t
{
	Vector2D m_Position;
	Vector2D m_TexCoord;

	Vertex_t() {}

	Vertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}

	void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
};

typedef Vertex_t FontVertex_t;

class ISurface
{
public:
	void DrawSetColor(Color col)
	{
		//DrawSetColor(col.r, col.g, col.b, col.a);
		typedef void(__thiscall* oDrawSetColor)(void*, Color);
		return getvfunc< oDrawSetColor >(this, 14)(this, col);
	}

	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc< OriginalFn >(this, 15)(this, r, g, b, a);
	}

	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc< OriginalFn >(this, 16)(this, x0, y0, x1, y1);
	}

	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc< OriginalFn >(this, 18)(this, x0, y0, x1, y1);
	}

	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc< OriginalFn >(this, 19)(this, x0, y0, x1, y1);
	}

	void DrawPolyLine(int *x, int *y, int count)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int *, int *, int);
		getvfunc< OriginalFn >(this, 20)(this, x, y, count);
	}

	void DrawSetTextFont(unsigned long font)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long);
		getvfunc< OriginalFn >(this, 23)(this, font);
	}

	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc< OriginalFn >(this, 25)(this, r, g, b, a);
	}

	void DrawSetTextColor(Color col)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, Color);
		return getvfunc< oDrawSetTextColor >(this, 24)(this, col);
	}

	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int);
		getvfunc< OriginalFn >(this, 26)(this, x, y);
	}

	void DrawPrintText(const wchar_t* text, int textLen)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const wchar_t*, int, int);
		getvfunc< OriginalFn >(this, 28)(this, text, textLen, 0);
	}

	int DrawGetTextureId(const char* filename)
	{
		typedef int(__thiscall* oDrawGetTextureId)(void*, const char*);
		return getvfunc<oDrawGetTextureId>(this, 34)(this, filename);
	}

	void DrawSetTextureFile(int id, const char* fileName, int hardwareFilter, bool forceReload)
	{
		typedef void(__thiscall* oDrawSetTextureFile)(void*, int, const char*, int, bool);
		getvfunc<oDrawSetTextureFile>(this, 36)(this, id, fileName, hardwareFilter, forceReload);
	}

	void DrawSetTextureRGBA(int id, const unsigned char *rgba, int wide, int tall)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, const unsigned char*, int, int);
		return getvfunc<OriginalFn>(this, 37)(this, id, rgba, wide, tall);
	}

	void DrawSetTexture(int textureID)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, int);
		return getvfunc< oDrawSetTextColor >(this, 38)(this, textureID); // --
	}

	void DrawTexturedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* oDrawTexturedRect)(void*, int, int, int, int);
		return getvfunc<oDrawTexturedRect>(this, 41)(this, x0, y0, x1, y1); // --
	}

	bool IsTextureIDValid(int id)
	{
		typedef bool(__thiscall* oIsTextureIDValid)(void*, int);
		return getvfunc<oIsTextureIDValid>(this, 42)(this, id); // --
	}

	int CreateNewTextureID(bool procedural)
	{
		typedef int(__thiscall* oDrawSetTextColor)(PVOID, bool);
		return getvfunc< oDrawSetTextColor >(this, 43)(this, procedural); // --
	}

	void GetScreenSize(int& width, int& height)
	{
		typedef void(__thiscall* oGetScreenSize)(void*, int&, int&);
		return getvfunc<oGetScreenSize>(this, 44)(this, width, height);
	}

	void DrawTexturedPolygon(int vtxCount, FontVertex_t *vtx, bool bClipVertices = true)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, int, FontVertex_t*, bool);
		return getvfunc< oDrawSetTextColor >(this, 106)(this, vtxCount, vtx, bClipVertices);
	}

	unsigned long FontCreate()
	{
		typedef unsigned int(__thiscall* OriginalFn)(PVOID);
		return getvfunc< OriginalFn >(this, 71)(this);
	}

	void SetFontGlyphSet(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		getvfunc< OriginalFn >(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}

	bool AddCustomFontFile(const char* fontFileName)
	{
		typedef bool(__thiscall* oAddCustomFontFile)(void*, const char*);
		return getvfunc<oAddCustomFontFile>(this, 73)(this, fontFileName);
	}

	void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, unsigned long, const wchar_t*, int&, int&);
		getvfunc< OriginalFn >(this, 79)(this, font, text, wide, tall);
	}

	void DrawOutlinedCircle(int x, int y, int r, int seg)
	{
		typedef void(__thiscall* oDrawOutlinedCircle)(PVOID, int, int, int, int);
		return getvfunc< oDrawOutlinedCircle >(this, 103)(this, x, y, r, seg);
	}

	void SurfaceGetCursorPos(int &x, int &y)
	{
		typedef void(__thiscall* oSurfaceGetCursorPos)(PVOID, int&, int&);
		return getvfunc< oSurfaceGetCursorPos >(this, 66)(this, x, y);
	}
};
