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

typedef float matrix3x4_t[3][4];

class CNetChannel;

class IEngineClient
{
public:
	typedef struct player_info_s
	{
		int64_t __pad0;
		union {
			int64_t xuid;
			struct {
				int xuidlow;
				int xuidhigh;
			};
		};
		char name[128];
		int userid;
		char guid[33]; // steam id
		unsigned int friendsid;
		char friendsname[128];
		bool fakeplayer;
		bool ishltv;
		unsigned int customfiles[4];
		unsigned char filesdownloaded;
	} player_info_t;

	void GetScreenSize(int& width, int& height)
	{
		typedef void (__thiscall* oGetScreenSize)(void*, int&, int&);
		return getvfunc<oGetScreenSize>(this, 5)(this, width, height);
	}

	bool GetPlayerInfo(int iIndex, player_info_t *pInfo)
	{
		typedef bool(__thiscall* oGetPlayerInfo)(void*, int, player_info_t*);
		return getvfunc<oGetPlayerInfo>(this, 8)(this, iIndex, pInfo);
	}

	int GetPlayerForUserID(int UserID)
	{
		typedef int (__thiscall* oGetPlayerForUserID)(void*, int);
		return getvfunc<oGetPlayerForUserID>(this, 9)(this, UserID);
	}

	int GetLocalPlayer()
	{
		typedef int (__thiscall* oGetLocalPlayer)(void*);
		return getvfunc<oGetLocalPlayer>(this, 12)(this);
	}

	void GetViewAngles(QAngle& angle)
	{
		typedef void (__thiscall* oGetViewAngles)(void*, QAngle&);
		return getvfunc<oGetViewAngles>(this, 18)(this, angle);
	}

	void SetViewAngles(QAngle& angle)
	{
		typedef void (__thiscall* oSetViewAngles)(void*, QAngle&);
		return getvfunc<oSetViewAngles>(this, 19)(this, angle);
	}

	int GetMaxClients()
	{
		typedef int (__thiscall* oGetMaxClients)(void*);
		return getvfunc<oGetMaxClients>(this, 20)(this);
	}

	bool IsInGame()
	{
		typedef bool (__thiscall* oIsInGame)(void*);
		return getvfunc<oIsInGame>(this, 26)(this);
	}

	const matrix3x4_t& WorldToScreenMatrix()
	{
		typedef const matrix3x4_t& (__thiscall* oWorldToScreenMatrix)(PVOID);
		return getvfunc<oWorldToScreenMatrix>(this, 37)(this);
	}

	const char* GetLevelName()
	{
		typedef const char* (__thiscall* oGetLevelName)(void*);
		return getvfunc<oGetLevelName>(this, 52)(this);
	}

	const char* GetLevelNameShort()
	{
		typedef const char* (__thiscall* oGetLevelName)(void*);
		return getvfunc<oGetLevelName>(this, 53)(this);
	}

	bool IsTakingScreenshot()
	{
		typedef bool (__thiscall* oIsTakingScreenshot)(void*);
		return getvfunc<oIsTakingScreenshot>(this, 92)(this);
	}

	void ExecuteClientCmd(const char* szCmdString)
	{
		typedef void (__thiscall* oExecuteClientCmd)(void*, const char*);
		return getvfunc<oExecuteClientCmd>(this, 108)(this, szCmdString);
	}

	CNetChannel* GetNetChannelInfo()
	{
		typedef CNetChannel*(__thiscall*oGetNetChannelInfo)(void*);
		return getvfunc<oGetNetChannelInfo>(this, 102)(this);
	}

	void ClientCmd_Unrestricted(const char* szCmdString)
	{
		typedef void(__thiscall* oClientCmd_Unrestricted)(PVOID, const char*, char);
		return getvfunc<oClientCmd_Unrestricted>(this, 114)(this, szCmdString, 0);
	}
};
