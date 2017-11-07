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

#include "color.h"

#include <vector>
#include "Vector.h"

//namespace oldColors
//
//
//{
//	Color WHITE = Color(255, 255, 255);
//	Color WHITE5 = Color(250, 250, 250);
//	Color WHITE10 = Color(245, 245, 245);
//	Color WHITE15 = Color(240, 240, 240);
//	Color WHITE20 = Color(235, 235, 235);
//	Color WHITE25 = Color(230, 230, 230);
//	Color WHITE30 = Color(225, 225, 225);
//	Color WHITE35 = Color(220, 220, 220);
//	Color WHITE40 = Color(215, 215, 215);
//
//	Color BLACK = Color(0, 0, 0);
//	Color BLACK5 = Color(5, 5, 5);
//	Color BLACK10 = Color(10, 10, 10);
//	Color BLACK15 = Color(15, 15, 15);
//	Color BLACK20 = Color(20, 20, 20);
//	Color BLACK25 = Color(25, 25, 25);
//	Color BLACK30 = Color(30, 30, 30);
//	Color BLACK35 = Color(35, 35, 35);
//	Color BLACK40 = Color(40, 40, 40);
//
//	Color RED = Color(218, 36, 36, 255);
//	Color ORANGE = Color(218, 127, 36);
//	Color YELLOW = Color(218, 218, 36);
//	Color LIME = Color(127, 218, 36);
//	Color GREEN = Color(36, 218, 36);
//	Color TURQUOISE = Color(36, 218, 127);
//	Color CYAN = Color(36, 218, 218);
//	Color BLUE = Color(36, 127, 218);
//	Color ROYAL = Color(36, 36, 218);
//	Color PURPLE = Color(127, 36, 218);
//	Color PINK = Color(218, 36, 218);
//	Color MAGENTA = Color(218, 36, 127);
//
//	Color MENU_BACKGROUND = Color(20, 20, 20);
//	Color MENU_OUTLINE = Color(62, 21, 66);
//	Color MENU_TITLE = CYAN;
//	Color MENU_BORDER = Color(0, 150, 255);
//	Color MENU_BORDER_LIGHT = Color(66, 66, 66, 254);
//	Color MENU_TEXT = WHITE40;
//
//	Color TAB_TEXT = Color(185, 185, 185);
//	Color SELECTED_TAB_TEXT = Color(95, 95, 95);
//	Color HIGHLIGHTED_TAB_TEXT = Color(135, 135, 135);
//
//	Color BUTTON_BG = BLACK25;
//	Color COMBO_BG = BLACK25;
//
//	Color ICON = WHITE40;
//	Color GRPBOX_OUTLINES = Color(62, 21, 66);
//
//	Color TOGGLE_ENABLED = CYAN;
//	Color TOGGLE_DISABLED = WHITE30;
//	Color TOGGLE_BUTTON = Color(200, 200, 200);
//
//	Color CHAMS_ENEMY_VISIBLE = Color(232, 42, 42);
//	Color CHAMS_ENEMY_HIDDEN = Color(232, 82, 12);
//	Color CHAMS_TEAM_VISIBLE = Color(12, 82, 232);
//	Color CHAMS_TEAM_HIDDEN = Color(12, 232, 82);
//	Color CHAMS_ARMS = Color(228, 232, 12);
//	Color CHAMS_WEAPONS = Color(232, 12, 192);
//
//	Color CROSSHAIR1 = Color(18, 211, 27);
//	Color CROSSHAIR2 = Color(0, 0, 0);
//
//	Color CONSOLE_LOG = oldColors::PURPLE;
//	Color CONSOLE_WARN = oldColors::ORANGE;
//	Color CONSOLE_ERROR = oldColors::RED;
//
//	Color FOOTSTEPS_TEAM = CHAMS_TEAM_HIDDEN;
//	Color FOOTSTEPS_ENEMY = CHAMS_ENEMY_HIDDEN;
//	
//	Color BOMB_TIMER_TEXT = RED;
//}

namespace Colors
{
	Color WHITE = Color(255, 255, 255);
	Color WHITE5 = Color(250, 250, 250);
	Color WHITE10 = Color(245, 245, 245);
	Color WHITE15 = Color(240, 240, 240);
	Color WHITE20 = Color(235, 235, 235);
	Color WHITE25 = Color(230, 230, 230);
	Color WHITE30 = Color(225, 225, 225);
	Color WHITE35 = Color(220, 220, 220);
	Color WHITE40 = Color(215, 215, 215);

	Color BLACK = Color(0, 0, 0);
	Color BLACK5 = Color(5, 5, 5);
	Color BLACK10 = Color(10, 10, 10);
	Color BLACK15 = Color(15, 15, 15);
	Color BLACK20 = Color(20, 20, 20);
	Color BLACK25 = Color(25, 25, 25);
	Color BLACK30 = Color(30, 30, 30);
	Color BLACK35 = Color(35, 35, 35);
	Color BLACK40 = Color(40, 40, 40);

	Color RED = Color(218, 36, 36, 255);
	Color ORANGE = Color(218, 127, 36);
	Color YELLOW = Color(218, 218, 36);
	Color LIME = Color(127, 218, 36);
	Color GREEN = Color(36, 218, 36);
	Color TURQUOISE = Color(36, 218, 127);
	Color CYAN = Color(36, 218, 218);
	Color BLUE = Color(36, 127, 218);
	Color ROYAL = Color(36, 36, 218);
	Color PURPLE = Color(127, 36, 218);
	Color PINK = Color(218, 36, 218);
	Color MAGENTA = Color(218, 36, 127);

	namespace Menu
	{
		Color BACKGROUND = Color(26, 26, 26);
		Color BORDER = Color(0, 122, 205);
		Color OUTLINES = Color(14, 14, 14);
		Color ENABLED = BORDER;
		Color DISABLED = Color(235, 235, 235);
		Color MENUTEXT = Color(230, 230, 230);
		Color HOVERTEXT = Color(255, 35, 255);
		Color MENUBAR = Color(62, 62, 62);

		Color WATERMARK_FOREGROUND = Color(36, 218, 218);;
		Color WATERMARK_BACKGROUND = Color(0, 0, 0);

		Color CURSOR_FOREGROUND = Color(218, 36, 127);
		Color CURSOR_BACKGROUND = Color(0, 0, 0);
	}

	namespace ESP
	{
		Color TCHAMS = Color(255, 100, 45);
		Color TCHAMSVISIBLE = Color(240, 150, 95);
		Color CTCHAMS = Color(15, 200, 215);
		Color CTCHAMSVISIBLE = Color(55, 95, 185);
		Color ESPTEXT = Color(220, 220, 220);
		Color ESPBOMBTEXT = Color(245, 55, 55);
		Color ARMSCHAMS = Color(228, 232, 12);
		Color WEAPONCHAMS = Color(232, 12, 192);

		Color FOOTSTEPS_TEAM = ESPTEXT;
		Color FOOTSTEPS_ENEMY = ESPBOMBTEXT;

		Color HITMARKERS = MAGENTA;
		Color SPREADCIRCLE = RED;

		Color ARMORBAR = WHITE35;
		Color HEALTHBAR = GREEN;

		Color AIMSPOT = RED;
		Color CHICKEN = YELLOW;
		Color DROPS = PURPLE;

		Color EVENTS = WHITE35;
		Color BULLETIMPACTS = MAGENTA.CustomAlpha(0.8);
	}

	namespace Grenades
	{
		Color FLASH = CYAN;
		Color FRAG = RED;
		Color DECOY = WHITE20;
		Color SMOKE = MAGENTA;
		Color MOLLY = ORANGE;
	}

	namespace Console
	{
		Color LOG = PURPLE;
		Color WARN = ORANGE;
		Color ERR = RED;
	}

	namespace SpotifyControls
	{
		Color CONTROLS = BLACK10;
		Color CONTROLSHOVER = RED;
		Color SONGTITLE = BLACK10;
	}
}

// CREDIT TO paradoxresolved (http://forums.codeguru.com/showthread.php?421328-RGB-to-HSL-and-vice-versa)

// This function extracts the hue, saturation, and luminance from "color" 
// and places these values in h, s, and l respectively.
Color RGBtoHSL(const Color& color, int& h, int& s, int& l)
{
	int r = color.r;
	int g = color.g;
	int b = color.b;

	double r_percent = ((double)r) / 255;
	double g_percent = ((double)g) / 255;
	double b_percent = ((double)b) / 255;

	double max_color = 0;
	if ((r_percent >= g_percent) && (r_percent >= b_percent))
		max_color = r_percent;
	if ((g_percent >= r_percent) && (g_percent >= b_percent))
		max_color = g_percent;
	if ((b_percent >= r_percent) && (b_percent >= g_percent))
		max_color = b_percent;

	double min_color = 0;
	if ((r_percent <= g_percent) && (r_percent <= b_percent))
		min_color = r_percent;
	if ((g_percent <= r_percent) && (g_percent <= b_percent))
		min_color = g_percent;
	if ((b_percent <= r_percent) && (b_percent <= g_percent))
		min_color = b_percent;

	double L = 0;
	double S = 0;
	double H = 0;

	L = (max_color + min_color) / 2;

	if (max_color == min_color)
	{
		S = 0;
		H = 0;
	}
	else
	{
		if (L < .50)
		{
			S = (max_color - min_color) / (max_color + min_color);
		}
		else
		{
			S = (max_color - min_color) / (2 - max_color - min_color);
		}
		if (max_color == r_percent)
		{
			H = (g_percent - b_percent) / (max_color - min_color);
		}
		if (max_color == g_percent)
		{
			H = 2 + (b_percent - r_percent) / (max_color - min_color);
		}
		if (max_color == b_percent)
		{
			H = 4 + (r_percent - g_percent) / (max_color - min_color);
		}
	}
	s = (int)(S * 100);
	l = (int)(L * 100);
	H = H * 60;
	if (H < 0)
		H += 360;
	h = (int)H;
	return Color(h, s, l);
}

// This function converts the "color" object to the equivalent RGB values of
// the hue, saturation, and luminance passed as h, s, and l respectively
Color HSLtoRGB(const int& h, const int& s, const int& l)
{
	int r = 0;
	int g = 0;
	int b = 0;

	double L = ((double)l) / 100;
	double S = ((double)s) / 100;
	double H = ((double)h) / 360;

	if (s == 0)
	{
		r = l;
		g = l;
		b = l;
	}
	else
	{
		double temp1 = 0;
		if (L < .50)
		{
			temp1 = L*(1 + S);
		}
		else
		{
			temp1 = L + S - (L*S);
		}

		double temp2 = 2 * L - temp1;

		double temp3 = 0;
		for (int i = 0; i < 3; i++)
		{
			switch (i)
			{
			case 0: // red
			{
				temp3 = H + .33333;
				if (temp3 > 1)
					temp3 -= 1;
				HSLtoRGB_Subfunction(r, temp1, temp2, temp3);
				break;
			}
			case 1: // green
			{
				temp3 = H;
				HSLtoRGB_Subfunction(g, temp1, temp2, temp3);
				break;
			}
			case 2: // blue
			{
				temp3 = H - .33333;
				if (temp3 < 0)
					temp3 += 1;
				HSLtoRGB_Subfunction(b, temp1, temp2, temp3);
				break;
			}
			default:
			{

			}
			}
		}
	}
	r = (int)((((double)r) / 100) * 255);
	g = (int)((((double)g) / 100) * 255);
	b = (int)((((double)b) / 100) * 255);
	return Color(r, g, b);
}

// This is a subfunction of HSLtoRGB
void HSLtoRGB_Subfunction(int& c, const double& temp1, const double& temp2, const double& temp3)
{
	if ((temp3 * 6) < 1)
		c = (int)((temp2 + (temp1 - temp2) * 6 * temp3) * 100);
	else
		if ((temp3 * 2) < 1)
			c = (int)(temp1 * 100);
		else
			if ((temp3 * 3) < 2)
				c = (int)((temp2 + (temp1 - temp2)*(.66666 - temp3) * 6) * 100);
			else
				c = (int)(temp2 * 100);
	return;
}