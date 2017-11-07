#include "fonts.h"

#include "interfaces.h"

namespace Fonts
{
	DWORD TITLE;
	DWORD MENU;
	DWORD MENUBIG;
	DWORD ESP;
	DWORD smallESP;
	DWORD SPOTIFY;

	DWORD tahoma12;
	DWORD tahoma14;
	DWORD tahoma16;
	DWORD tahoma12o;
	DWORD tahoma14o;
	DWORD verdana12;
	DWORD verdana14;
	DWORD verdana12o;
	DWORD verdana14o;
}

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

DWORD DefineFont(const char* name, int height, int weight, int blur, int scanlines, int flags)
{
	DWORD temp = I::Surface->FontCreate();

	I::Surface->SetFontGlyphSet(temp, name, height, weight, blur, scanlines, flags);

	return temp;
}

void Fonts::Initialize()
{
	TITLE = DefineFont("Verdana", 16, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE | FONTFLAG_DROPSHADOW);
	MENU = DefineFont("Tahoma", 12, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	MENUBIG = DefineFont("Verdana", 12, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	ESP = DefineFont("Verdana", 16, 600, 0, 0, FONTFLAG_OUTLINE);
	smallESP = DefineFont("Tahoma", 12, 400, 0, 0, FONTFLAG_OUTLINE);
	SPOTIFY = DefineFont("Verdana", 14, 800, 0, 0, 0);

	tahoma12 = DefineFont("Tahoma", 12, 400, 0, 0, FONTFLAG_ANTIALIAS);
	tahoma12o = DefineFont("Tahoma", 12, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	tahoma14 = DefineFont("Tahoma", 14, 400, 0, 0, FONTFLAG_ANTIALIAS);
	tahoma14o = DefineFont("Tahoma", 14, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	tahoma16 = DefineFont("Tahoma", 16, 400, 0, 0, FONTFLAG_ANTIALIAS);

	verdana12 = DefineFont("Verdana", 12, 400, 0, 0, FONTFLAG_ANTIALIAS);
	verdana12o = DefineFont("Verdana", 12, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	verdana14 = DefineFont("Verdana", 14, 400, 0, 0, FONTFLAG_ANTIALIAS);
	verdana14o = DefineFont("Verdana", 14, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
}