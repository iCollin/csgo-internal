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
credit: paradoxresolved (http://forums.codeguru.com/showthread.php?421328-RGB-to-HSL-and-vice-versa)
*/

#pragma once

class Color
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	Color()
	{
		this->r = 0;
		this->g = 0;
		this->b = 0;
		this->a = 255;
	}

	Color(int r, int g, int b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 255;
	}

	Color(int r, int g, int b, int a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Color operator / (float div)
	{
		Color color = *this;
		color.r = color.r / div;
		color.g = color.g / div;
		color.b = color.b / div;
		return color;
	}

	Color& operator /= (float div)
	{
		Color& color = *this;
		color.r /= div;
		color.g /= div;
		color.b /= div;
		return color;
	}

	Color& operator *= (float coeff)
	{
		Color& color = *this;
		color.r *= coeff;
		color.g *= coeff;
		color.b *= coeff;
		return color;
	}

	Color CustomAlpha(float factor)
	{
		if (factor < 0.f) factor = 0.f;
		else if (factor > 1.f) factor = 1.f;
		return Color(r, g, b, 255.f * factor);
	}

	float fr()
	{
		return r / 255.f;
	}

	float fg()
	{
		return g / 255.f;
	}

	float fb()
	{
		return b / 255.f;
	}

	float fa()
	{
		return a / 255.f;
	}
};

//namespace oldColors
//{
//	extern Color WHITE;
//	extern Color WHITE5;
//	extern Color WHITE10;
//	extern Color WHITE15;
//	extern Color WHITE20;
//	extern Color WHITE25;
//	extern Color WHITE30;
//	extern Color WHITE35;
//	extern Color WHITE40;
//
//	extern Color BLACK;
//	extern Color BLACK5;
//	extern Color BLACK10;
//	extern Color BLACK15;
//	extern Color BLACK20;
//	extern Color BLACK25;
//	extern Color BLACK30;
//	extern Color BLACK35;
//	extern Color BLACK40;
//
//	extern Color RED;
//	extern Color ORANGE;
//	extern Color YELLOW;
//	extern Color LIME;
//	extern Color GREEN;
//	extern Color TURQUOISE;
//	extern Color CYAN;
//	extern Color BLUE;
//	extern Color ROYAL;
//	extern Color PURPLE;
//	extern Color PINK;
//	extern Color MAGENTA;
//
//	extern Color MENU_BACKGROUND;
//	extern Color MENU_OUTLINE;
//	extern Color MENU_TITLE;
//	extern Color MENU_BORDER;
//	extern Color MENU_BORDER_LIGHT;
//	extern Color MENU_TEXT;
//
//	extern Color TAB_TEXT;
//	extern Color SELECTED_TAB_TEXT;
//	extern Color HIGHLIGHTED_TAB_TEXT;
//
//	extern Color BUTTON_BG;
//	extern Color COMBO_BG;
//
//	extern Color ICON;
//	extern Color GRPBOX_OUTLINES;
//
//	extern Color TOGGLE_ENABLED;
//	extern Color TOGGLE_DISABLED;
//	extern Color TOGGLE_BUTTON;
//
//	extern Color CHAMS_ENEMY_VISIBLE;
//	extern Color CHAMS_ENEMY_HIDDEN;
//	extern Color CHAMS_TEAM_VISIBLE;
//	extern Color CHAMS_TEAM_HIDDEN;
//	extern Color CHAMS_ARMS;
//	extern Color CHAMS_WEAPONS;
//
//	extern Color CROSSHAIR1;
//	extern Color CROSSHAIR2;
//
//	extern Color CONSOLE_LOG;
//	extern Color CONSOLE_WARN;
//	extern Color CONSOLE_ERROR;
//
//	extern Color FOOTSTEPS_TEAM;
//	extern Color FOOTSTEPS_ENEMY;
//
//	extern Color BOMB_TIMER_TEXT;
//}

namespace Colors
{
	namespace Menu
	{
		extern Color BACKGROUND;
		extern Color BORDER;
		extern Color OUTLINES;
		extern Color ENABLED;
		extern Color DISABLED;
		extern Color MENUTEXT;
		extern Color HOVERTEXT;
		extern Color MENUBAR;

		extern Color WATERMARK_FOREGROUND;
		extern Color WATERMARK_BACKGROUND;

		extern Color CURSOR_FOREGROUND;
		extern Color CURSOR_BACKGROUND;
	}

	namespace Grenades
	{
		extern Color FLASH;
		extern Color FRAG;
		extern Color DECOY;
		extern Color SMOKE;
		extern Color MOLLY;
	}

	namespace ESP
	{
		extern Color TCHAMS;
		extern Color TCHAMSVISIBLE;
		extern Color CTCHAMS;
		extern Color CTCHAMSVISIBLE;

		extern Color ESPTEXT;
		extern Color ESPBOMBTEXT;

		extern Color ARMSCHAMS;
		extern Color WEAPONCHAMS;

		extern Color FOOTSTEPS_TEAM;
		extern Color FOOTSTEPS_ENEMY;

		extern Color HITMARKERS;
		extern Color SPREADCIRCLE;

		extern Color ARMORBAR;
		extern Color HEALTHBAR;

		extern Color AIMSPOT;
		extern Color CHICKEN;
		extern Color DROPS;

		// TODO color dif events dif?
		extern Color EVENTS;
		extern Color BULLETIMPACTS;
	}

	namespace Console
	{
		extern Color LOG;
		extern Color WARN;
		extern Color ERR;
	}

	namespace SpotifyControls
	{
		extern Color CONTROLS;
		extern Color CONTROLSHOVER;
		extern Color SONGTITLE;
	}

	extern Color WHITE;
	extern Color WHITE5;
	extern Color WHITE10;
	extern Color WHITE15;
	extern Color WHITE20;
	extern Color WHITE25;
	extern Color WHITE30;
	extern Color WHITE35;
	extern Color WHITE40;

	extern Color BLACK;
	extern Color BLACK5;
	extern Color BLACK10;
	extern Color BLACK15;
	extern Color BLACK20;
	extern Color BLACK25;
	extern Color BLACK30;
	extern Color BLACK35;
	extern Color BLACK40;

	extern Color RED;
	extern Color ORANGE;
	extern Color YELLOW;
	extern Color LIME;
	extern Color GREEN;
	extern Color TURQUOISE;
	extern Color CYAN;
	extern Color BLUE;
	extern Color ROYAL;
	extern Color PURPLE;
	extern Color PINK;
	extern Color MAGENTA;
}

// PURE COPY PASTA, ONLY CHANGED RETURN TYPES.
// CREDIT TO paradoxresolved (http://forums.codeguru.com/showthread.php?421328-RGB-to-HSL-and-vice-versa)
Color RGBtoHSL(const Color& color, int& h, int& s, int& l); // broken doesn't work properly
Color HSLtoRGB(const int& h, const int& s, const int& l);
void HSLtoRGB_Subfunction(int& c, const double& temp1, const double& temp2, const double& temp3);