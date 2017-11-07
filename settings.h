#pragma once

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <vector>

#include "utilities.h"

namespace Config
{
	namespace Aimbot
	{
		extern bool Enable;
		extern bool FriendlyFire;
		extern int SelectionStyle;
		extern std::vector<std::string> SelectionStyles;
		extern bool RequiresKey;
		extern int Key;
		extern std::vector<std::string> Bones;
		extern float SaltAmount;
		extern float SmoothingAmount;
		extern bool Prediction;
		extern bool AutoCrouch;
		extern bool AutoScope;
		extern bool IgnoreJump;
		extern bool VisCheck;
		extern bool OneTarget;
		extern float HitChance;

		namespace AutoWall
		{
			extern bool Enable;
			extern float MinDamage;
		}

		namespace RCS
		{
			extern int Mode;
			extern std::vector<std::string> Modes;
			extern float xMin;
			extern float xMax;
			extern float yMin;
			extern float yMax;
		}

		namespace Rifles
		{
			extern bool Enable;
			extern bool MaxFOV;
			extern float minFOV;
			extern float FOV;
			extern float maxFOV;
			extern int Bone;
			extern bool Smooth;
			extern float SmoothingRatio;
			extern bool Salt;

			namespace RCS
			{
				extern bool Enable;
			}
		}

		namespace Snipers
		{
			extern bool Enable;
			extern bool MaxFOV;
			extern float minFOV;
			extern float FOV;
			extern float maxFOV;
			extern int Bone;
			extern bool Smooth;
			extern float SmoothingRatio;
			extern bool Salt;

			namespace RCS
			{
				extern bool Enable;
			}
		}

		namespace Pistols
		{
			extern bool Enable;
			extern bool MaxFOV;
			extern float minFOV;
			extern float FOV;
			extern float maxFOV;
			extern int Bone;
			extern bool Smooth;
			extern float SmoothingRatio;
			extern bool Salt;

			namespace RCS
			{
				extern bool Enable;
			}
		}
	}

	namespace Triggerbot
	{
		extern bool Enable;
		extern bool RequiresKey;
		extern int Key;

		namespace Rifles
		{
			extern bool Enable;
			extern float WaitTime;
			extern bool FlashCheck;
			extern bool SmokeCheck;
		}

		namespace Snipers
		{
			extern bool Enable;
			extern float WaitTime;
			extern bool FlashCheck;
			extern bool SmokeCheck;
		}

		namespace Pistols
		{
			extern bool Enable;
			extern float WaitTime;
			extern bool FlashCheck;
			extern bool SmokeCheck;
		}
	}

	namespace Visuals
	{
		namespace OverrideView
		{
			extern bool CustomFOV;
			extern bool CustomScopedFOV;
			extern float minFOV;
			extern float FOV;
			extern float maxFOV;
			extern bool Thirdperson;
			extern float ThirdpersonDistance;
			extern bool VisibleRecoil;
			extern bool NoSmoke;
			extern bool NoFlash;
			extern float FlashMaxAlpha;
		}

		namespace Chams
		{
			extern bool Enable;
			extern bool VisibleCheck;
			extern bool Players;
			extern bool Weapons;
			extern bool Arms;
			extern int ArmsType;
			extern std::vector<std::string> ArmsTypes;
			extern bool FilterTeam;
			extern bool FlatChams;
			extern bool WireFrameSkins;
		}

		namespace Bomb
		{
			extern bool ShowPlantedOnT;
			extern bool ShowPlantedOnCT;
			extern bool ShowWildOnT;
			extern bool ShowWildOnCT;
			extern bool ShowDefusingCT;
			extern bool BombTimer;
			extern bool PredictHealth;
			extern bool AnnouncePlant;
		}

		namespace ESP
		{
			extern bool Enable;
			extern bool FilterTeam;
			extern bool VisibleCheck;
			extern bool PlayerESP;
			extern bool DropESP;
			extern bool ChickenESP;
			extern bool DrawBoxes;
			extern bool DrawNames;
			extern bool DrawHealthBar;
			extern bool DrawDlights;
			extern bool SpreadCircle;
			extern bool HitMarkers;
			extern bool GrenadeESP;
			extern bool BarrelESP;
			extern float BarrelESPLength;
			extern bool AimSpot;
			//extern bool NadePred;

			namespace PlayerInfo
			{
				extern bool Enable;
				extern bool Rank;
				extern bool SteamID;
				extern bool Weapon;
				extern bool Health;
				extern bool Armor;
				extern bool Reloading;
				extern bool Flashed;
				extern bool Scoped;
				extern bool BombCarrier;
				extern bool DefuseKit;
				extern bool Defusing;
				extern bool HostageCarrier;
				extern bool Rescuing;
				extern bool Location;
			}
		}

		namespace Sounds
		{
			extern bool Enable;
			extern bool FilterTeam;
			extern bool ShowOnRadar;
			extern float MinimumVolume;
			extern bool MuteAmbient;

			namespace Events
			{
				extern bool item_pickup;
				extern bool player_footstep;
				extern bool player_falldamage;
				extern bool door_moving;
				extern bool silencer_on;
				extern bool silencer_off;
				extern bool weapon_zoom;
				extern bool weapon_reload;
				extern bool weapon_fire;
				extern bool bomb_actions;
				//extern bool bomb_begindefuse; // all included in plant & defuse toggle
				//extern bool bomb_abortdefuse;
				//extern bool bomb_beginplant;
				//extern bool bomb_abortplant;
				extern bool bomb_dropped;
				//extern bool bomb_pickup;    // both should be captured with item pickup, test that.
				//extern bool defuser_pickup;
				extern bool ShowEventType;
				extern float expireTime;
				extern float MaximumDistance;
			}
		}

		namespace HitMarkers
		{
			extern bool Enable;
			extern float ExpireTime;
			extern float MarkerSize;
			extern float TravelDistance;
			extern bool ShowImpacts;
		}
	}

	namespace Misc
	{
		namespace AutoAccept
		{
			extern bool Enable;
			extern float WaitTime;
		}

		namespace Bunnyhop
		{
			extern bool Enable;
			extern bool AutoStrafe;
			extern bool SMACSafe;
		}

		namespace AutoDefuse
		{
			extern bool Enable;
			extern bool AimAtBomb;
			extern float ExtraTime;
		}

		namespace Spotify
		{
			extern bool ShowControls;
			extern bool ShowCurrentlyPlaying;
		}

		namespace SpectatorList
		{
			extern bool Enable;
		}

		namespace SkyChanger
		{
			extern int SkySelection;
			extern std::vector<std::string> SkyOptions;
		}

		namespace GrenadeHelper
		{
			namespace Prediction
			{
				extern bool Enable;
				extern bool DlightAtEnd;
			}

			extern bool SmokeHelper;
			extern bool MollyHelper;
			extern bool FlashHelper;
			extern bool DlightOnPosition;
		}

		namespace Stream
		{
			extern bool HideNames;
		}

		namespace RenderConVars
		{
			extern bool mat_drawgray;
			extern bool mat_fullbright;
			extern bool mat_showlowresimage;
			extern bool mat_showmiplevels;
		}

		namespace ViewModelFOV
		{
			extern bool Enable;
			extern float Modifier;
		}
	}

	namespace Skins
	{
		namespace Knives
		{
			extern bool Enable;
			extern int TKnife;
			extern int TKnifeSkin;
			extern int CTKnife;
			extern int CTKnifeSkin;
			extern std::vector<std::string> knifeOptions;
			extern std::vector<std::string> skinOptions;
		}

		namespace Gloves
		{
			extern bool Enable;
			extern int TGlove;
			extern int CTGlove;
			extern std::vector<std::string> GloveOptions;
			extern std::vector<std::vector<std::string>> SkinsByGlove;
			extern int TSkin;
			extern std::vector<std::string> TSkinOptions; 
			extern int CTSkin;
			extern std::vector<std::string> CTSkinOptions;
		}

		namespace Weapons
		{
			extern int SelectedWeapon;
			extern int SelectedSkin;
			extern bool StatTrak;
			extern float Float;

			extern int sticker1;
			extern int sticker2;
			extern int sticker3;
			extern int sticker4;
		}

		extern bool Enable;
		//extern float Wear;
		//extern int PaintKit;
	}

	namespace Debug
	{
		extern bool DrawBoneIDs;
		extern bool StreamMode;
	}
}

#endif // !SETTINGS_H