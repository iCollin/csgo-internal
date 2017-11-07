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

#include "vector.h"
#include "netvar.h"
#include "IVModelRender.h"

#include "utilities.h"

#define MAX_SHOOT_SOUNDS 16
#define MAX_WEAPON_STRING 80
#define MAX_WEAPON_PREFIX 16
#define MAX_WEAPON_AMMO_NAME 32

enum WeaponSound_t
{
	EMPTY,
	SINGLE,
	SINGLE_NPC,
	WPN_DOUBLE, // Can't be "DOUBLE" because windows.h uses it.
	DOUBLE_NPC,
	BURST,
	RELOAD,
	RELOAD_NPC,
	MELEE_MISS,
	MELEE_HIT,
	MELEE_HIT_WORLD,
	SPECIAL1,
	SPECIAL2,
	SPECIAL3,
	TAUNT,
	FAST_RELOAD,

	// Add new shoot sound types here
	REVERSE_THE_NEW_SOUND,

	NUM_SHOOT_SOUND_TYPES,
};

enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};

enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

enum WeaponType_t : int
{
	INVALID = -1,
	RIFLE,
	SNIPER,
	PISTOL,
	SMG
};

WeaponType_t GetWeaponTypeFromClassID(EClassIds id);

class ICollideable
{
public:
	virtual void pad0();
	virtual const Vector& OBBMins() const;
	virtual const Vector& OBBMaxs() const;
};

class IHandleEntity
{
public:
	virtual ~IHandleEntity() {};
};

class IClientUnknown : public IHandleEntity {};
class IClientRenderable
{
public:
	virtual ~IClientRenderable() {};

	model_t* GetModel()
	{
		typedef model_t* (__thiscall* oGetModel)(void*);
		return getvfunc<oGetModel>(this, 8)(this);
	}

	bool SetupBones(matrix3x4_t* pBoneMatrix, int nMaxBones, int nBoneMask, float flCurTime = 0)
	{
		typedef bool (__thiscall* oSetupBones)(void*, matrix3x4_t*, int, int, float);
		return getvfunc<oSetupBones>(this, 13)(this, pBoneMatrix, nMaxBones, nBoneMask, flCurTime);
	}
};

class IClientNetworkable
{
public:
	virtual ~IClientNetworkable() {};

	void Release()
	{
		typedef void (__thiscall* oRelease)(void*);
		return getvfunc<oRelease>(this, 1)(this);
	}

	ClientClass* GetClientClass()
	{
		typedef ClientClass* (__thiscall* oGetClientClass)(void*);
		return getvfunc<oGetClientClass>(this, 2)(this);
	}

	void PreDataUpdate(DataUpdateType_t updateType)
	{
		typedef void (__thiscall* oPreDataUpdate)(void*, DataUpdateType_t);
		return getvfunc<oPreDataUpdate>(this, 6)(this, updateType);
	}

	bool GetDormant()
	{
		typedef bool (__thiscall* oGetDormant)(void*);
		return getvfunc<oGetDormant>(this, 9)(this);
	}

	int GetIndex()
	{
		typedef int (__thiscall* oGetIndex)(void*);
		return getvfunc<oGetIndex>(this, 10)(this);
	}

	void SetDestroyedOnRecreateEntities()
	{
		typedef void (__thiscall* oSetDestroyedOnRecreateEntities)(void*);
		return getvfunc<oSetDestroyedOnRecreateEntities>(this, 13)(this);
	}
};

class IClientThinkable
{
public:
	virtual ~IClientThinkable() {};
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual ~IClientEntity() {};
};

class C_BaseEntity : public IClientEntity
{
public:

	IClientNetworkable* GetNetworkable()
	{
		return (IClientNetworkable*)((uintptr_t)this + 0x08);
	}

	void SetModelIndex(int index)
	{
		typedef void (__thiscall* oSetModelIndex)(void*, int);
		return getvfunc<oSetModelIndex>(this, 111)(this, index);
	}

	void SetModelIndex2(int index)
	{
		typedef void(__thiscall* oSetModelIndex)(void*, int);
		return getvfunc<oSetModelIndex>(this, 75)(this, index);
	}

	int* GetModelIndex()
	{
		return (int*)((uintptr_t)this + NetVars::DT_BaseViewModel::m_nModelIndex);
	}

	float GetAnimTime()
	{
		return *(float*)((uintptr_t)this + NetVars::DT_BaseEntity::m_flAnimTime);
	}

	float GetSimulationTime()
	{
		return *(float*)((uintptr_t)this + NetVars::DT_BaseEntity::m_flSimulationTime);
	}

	TeamID GetTeam()
	{
		return *(TeamID*)((uintptr_t)this + NetVars::DT_BaseEntity::m_iTeamNum);
	}

	Vector GetVecOrigin()
	{
		return *(Vector*)((uintptr_t)this + NetVars::DT_BaseEntity::m_vecOrigin);
	}

	MoveType_t GetMoveType()
	{
		return *(MoveType_t*)((uintptr_t)this + NetVars::DT_BaseEntity::m_MoveType);
	}

	ICollideable* GetCollideable()
	{
		return (ICollideable*)((uintptr_t)this + NetVars::DT_BaseEntity::m_Collision);
	}

	bool* GetSpotted()
	{
		return (bool*)((uintptr_t)this + NetVars::DT_BaseEntity::m_bSpotted);
	}
};

/* generic game classes */
class C_BasePlayer : public C_BaseEntity
{
public:
	QAngle* GetViewPunchAngle()
	{
		return (QAngle*)((uintptr_t)this + NetVars::DT_BasePlayer::m_viewPunchAngle);
	}

	QAngle* GetAimPunchAngle()
	{
		return (QAngle*)((uintptr_t)this + NetVars::DT_BasePlayer::m_aimPunchAngle);
	}

	Vector GetVecViewOffset()
	{
		return *(Vector*)((uintptr_t)this + NetVars::DT_BasePlayer::m_vecViewOffset);
	}

	unsigned int GetTickBase()
	{
		return *(unsigned int*)((uintptr_t)this + NetVars::DT_BasePlayer::m_nTickBase);
	}

	Vector GetVelocity()
	{
		return *(Vector*)((uintptr_t)this + NetVars::DT_BasePlayer::m_vecVelocity);
	}

	int GetHealth()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_BasePlayer::m_iHealth);
	}

	unsigned char GetLifeState()
	{
		return *(unsigned char*)((uintptr_t)this + NetVars::DT_BasePlayer::m_lifeState);
	}

	int GetFlags()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_BasePlayer::m_fFlags);
	}

	void SetFlags(int iFlags)
	{
		*(int*)((uintptr_t)this + NetVars::DT_BasePlayer::m_fFlags) = iFlags;
	}

	ObserverMode_t* GetObserverMode()
	{
		return (ObserverMode_t*)((uintptr_t)this + NetVars::DT_BasePlayer::m_iObserverMode);
	}

	HANDLE GetObserverTarget()
	{
		return *(HANDLE*)((uintptr_t)this + NetVars::DT_BasePlayer::m_hObserverTarget);
	}

	HANDLE* GetViewModel()
	{
		return (HANDLE*)((uintptr_t)this + NetVars::DT_BasePlayer::m_hViewModel);
	}

	const char* GetLastPlaceName()
	{
		return (const char*)((uintptr_t)this + NetVars::DT_BasePlayer::m_szLastPlaceName);
	}

	int GetShotsFired()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSPlayer::m_iShotsFired);
	}

	QAngle* GetEyeAngles()
	{
		return (QAngle*)((uintptr_t)this + NetVars::DT_CSPlayer::m_angEyeAngles[0]);
	}

	int GetMoney()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSPlayer::m_iAccount);
	}

	int GetHits()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSPlayer::m_totalHitsOnServer);
	}

	int GetArmor()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSPlayer::m_ArmorValue);
	}

	int HasDefuser()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSPlayer::m_bHasDefuser);
	}

	bool IsDefusing()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSPlayer::m_bIsDefusing);
	}

	bool IsGrabbingHostage()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSPlayer::m_bIsGrabbingHostage);
	}

	bool IsScoped()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSPlayer::m_bIsScoped);
	}

	bool GetImmune()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSPlayer::m_bGunGameImmunity);
	}

	bool IsRescuing()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_CSPlayer::m_bIsRescuing);
	}

	int HasHelmet()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_CSPlayer::m_bHasHelmet);
	}

	float GetFlashBangTime()
	{
		return *(float*)((uintptr_t)this + 0xABE4);
	}

	float GetFlashDuration()
	{
		return *(float*)((uintptr_t)this + NetVars::DT_CSPlayer::m_flFlashDuration);
	}

	float* GetFlashMaxAlpha()
	{
		return (float*)((uintptr_t)this + NetVars::DT_CSPlayer::m_flFlashMaxAlpha);
	}

	float* GetLowerBodyYawTarget()
	{
		return (float*)((uintptr_t)this + NetVars::DT_CSPlayer::m_flLowerBodyYawTarget);
	}

	HANDLE GetActiveWeapon()
	{
		return *(HANDLE*)((uintptr_t)this + NetVars::DT_BaseCombatCharacter::m_hActiveWeapon);
	}

	HANDLE* GetWeapons()
	{
		return (HANDLE*)((uintptr_t)this + NetVars::DT_BaseCombatCharacter::m_hMyWeapons);
	}

	int* GetWearables()
	{
		return (int*)((uintptr_t)this + NetVars::DT_BaseCombatCharacter::m_hMyWearables);
	}

	bool GetAlive()
	{
		return this->GetHealth() > 0 && this->GetLifeState() == LIFE_ALIVE;
	}

	Vector GetEyePosition()
	{
		return this->GetVecOrigin() + this->GetVecViewOffset();
	}

	inline Vector GetBonePosition(int boneIndex)
	{
		matrix3x4_t BoneMatrix[MAXSTUDIOBONES];

		if (!this->SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
			return this->GetVecOrigin();

		return Vector(BoneMatrix[boneIndex][0][3], BoneMatrix[boneIndex][1][3], BoneMatrix[boneIndex][2][3]);
	}

	QAngle* GetVAngles()
	{
		return (QAngle*)((uintptr_t)this + NetVars::DT_BasePlayer::deadflag + 0x4);
	}
};

class C_PlantedC4 : public C_BaseEntity
{
public:
	bool IsBombTicking()
	{
		return (bool)((uintptr_t)this + NetVars::DT_PlantedC4::m_bBombTicking);
	}

	float GetBombTime()
	{
		return *(float*)((uintptr_t)this + NetVars::DT_PlantedC4::m_flC4Blow);
	}

	bool IsBombDefused()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_PlantedC4::m_bBombDefused);
	}

	int GetBombDefuser()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_PlantedC4::m_hBombDefuser);
	}
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
	ItemDefinitionIndex* GetItemDefinitionIndex()
	{
		return (ItemDefinitionIndex*)((uintptr_t)this + NetVars::DT_BaseAttributableItem::m_iItemDefinitionIndex);
	}

	int* GetItemIDHigh()
	{
		return (int*)((uintptr_t)this + NetVars::DT_BaseAttributableItem::m_iItemIDHigh);
	}

	int* GetEntityQuality()
	{
		return (int*)((uintptr_t)this + NetVars::DT_BaseAttributableItem::m_iEntityQuality);
	}

	char* GetCustomName()
	{
		return (char*)((uintptr_t)this + NetVars::DT_BaseAttributableItem::m_szCustomName);
	}

	int* GetFallbackPaintKit()
	{
		return (int*)((uintptr_t)this + NetVars::DT_BaseAttributableItem::m_nFallbackPaintKit);
	}

	int* GetFallbackSeed()
	{
		return (int*)((uintptr_t)this + NetVars::DT_BaseAttributableItem::m_nFallbackSeed);
	}

	float* GetFallbackWear()
	{
		return (float*)((uintptr_t)this + NetVars::DT_BaseAttributableItem::m_flFallbackWear);
	}

	int* GetFallbackStatTrak()
	{
		return (int*)((uintptr_t)this + NetVars::DT_BaseAttributableItem::m_nFallbackStatTrak);
	}

	int* GetAccountID()
	{
		return (int*)((uintptr_t)this + NetVars::DT_BaseAttributableItem::m_iAccountID);
	}
};

class C_BaseViewModel: public C_BaseEntity
{
public:
	int GetWeapon()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_BaseViewModel::m_hWeapon);
	}

	HANDLE GetOwner()
	{
		return *(HANDLE*)((uintptr_t)this + NetVars::DT_BaseViewModel::m_hOwner);
	}
};

class CHudTexture;

class FileWeaponInfo_t
{
public:
	FileWeaponInfo_t();

	// Each game can override this to get whatever values it wants from the script.
	virtual void Parse(KeyValues *pKeyValuesData, const char *szWeaponName);

	bool bParsedScript;
	bool bLoadedHudElements;

	char szClassName[MAX_WEAPON_STRING];
	char szPrintName[MAX_WEAPON_STRING];

	char szViewModel[MAX_WEAPON_STRING];
	char szWorldModel[MAX_WEAPON_STRING];
	char szAmmo1[MAX_WEAPON_AMMO_NAME];
	char szWorldDroppedModel[MAX_WEAPON_STRING];
	char szAnimationPrefix[MAX_WEAPON_PREFIX];
	int iSlot;
	int iPosition;
	int iMaxClip1;
	int iMaxClip2;
	int iDefaultClip1;
	int iDefaultClip2;
	int iWeight;
	int iRumbleEffect;
	bool bAutoSwitchTo;
	bool bAutoSwitchFrom;
	int iFlags;
	char szAmmo2[MAX_WEAPON_AMMO_NAME];
	char szAIAddOn[MAX_WEAPON_STRING];

	// Sound blocks
	char aShootSounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING];

	int iAmmoType;
	int iAmmo2Type;
	bool m_bMeleeWeapon;

	// This tells if the weapon was built right-handed (defaults to true).
	// This helps cl_righthand make the decision about whether to flip the model or not.
	bool m_bBuiltRightHanded;
	bool m_bAllowFlipping;

	// Sprite data, read from the data file
	int iSpriteCount;
	CHudTexture* iconActive;
	CHudTexture* iconInactive;
	CHudTexture* iconAmmo;
	CHudTexture* iconAmmo2;
	CHudTexture* iconCrosshair;
	CHudTexture* iconAutoaim;
	CHudTexture* iconZoomedCrosshair;
	CHudTexture* iconZoomedAutoaim;
	CHudTexture* iconSmall;
};

struct CCSWeaponInfo
{
	float GetRangeModifier()
	{
		return flRangeModifier;
	}

	float GetPenetration()
	{
		return flPenetration;
	}

	int GetDamage()
	{
		return iDamage;
	}

	float GetRange()
	{
		return flRange;
	}

	float GetWeaponArmorRatio()
	{
		return flArmorRatio;
	}

	CSWeaponType GetWeaponType()
	{
		return (CSWeaponType)iWeaponType;
	}

	virtual ~CCSWeaponInfo() {};
	/*Parse(KeyValues *, char const*)
	RefreshDynamicParameters(void)
	GetPrimaryClipSize(C_EconItemView const*, int, float)const
	GetSecondaryClipSize(C_EconItemView const*, int, float)const
	GetDefaultPrimaryClipSize(C_EconItemView const*, int, float)const
	GetDefaultSecondaryClipSize(C_EconItemView const*, int, float)const
	GetPrimaryReserveAmmoMax(C_EconItemView const*, int, float)const
	GetSecondaryReserveAmmoMax(C_EconItemView const*, int, float)const*/

	char*		consoleName;			// 0x0004
	char		pad_0008[12];			// 0x0008
	int			iMaxClip1;				// 0x0014
	int			iMaxClip2;				// 0x0018
	int			iDefaultClip1;			// 0x001C
	int			iDefaultClip2;			// 0x0020
	char		pad_0024[8];			// 0x0024
	char*		szWorldModel;			// 0x002C
	char*		szViewModel;			// 0x0030
	char*		szDroppedModel;			// 0x0034
	char		pad_0038[4];			// 0x0038
	char*		N0000023E;				// 0x003C
	char		pad_0040[56];			// 0x0040
	char*		szEmptySound;			// 0x0078
	char		pad_007C[4];			// 0x007C
	char*		szBulletType;			// 0x0080
	char		pad_0084[4];			// 0x0084
	char*		szHudName;				// 0x0088
	char*		szWeaponName;			// 0x008C
	char		pad_0090[56];			// 0x0090
	int 		iWeaponType;			// 0x00C8
	int			iWeaponPrice;			// 0x00CC
	int			iKillAward;				// 0x00D0
	char*		szAnimationPrefix;		// 0x00D4
	float		flCycleTime;			// 0x00D8
	float		flCycleTimeAlt;			// 0x00DC
	float		flTimeToIdle;			// 0x00E0
	float		flIdleInterval;			// 0x00E4
	bool		bFullAuto;				// 0x00E8
	char		pad_0x00E5[3];			// 0x00E9
	int			iDamage;				// 0x00EC
	float		flArmorRatio;			// 0x00F0
	int			iBullets;				// 0x00F4
	float		flPenetration;			// 0x00F8
	float		flFlinchVelocityModifierLarge;	// 0x00FC
	float		flFlinchVelocityModifierSmall;	// 0x0100
	float		flRange;				// 0x0104
	float		flRangeModifier;		// 0x0108
	float		flThrowVelocity;		// 0x010C
	char		pad_0x010C[12];			// 0x0110
	bool		bHasSilencer;			// 0x011C
	char		pad_0x0119[3];			// 0x011D
	char*		pSilencerModel;			// 0x0120
	int			iCrosshairMinDistance;	// 0x0124
	int			iCrosshairDeltaDistance;// 0x0128 - iTeam?
	float		flMaxPlayerSpeed;		// 0x012C
	float		flMaxPlayerSpeedAlt;	// 0x0130
	float		flSpread;				// 0x0134
	float		flSpreadAlt;			// 0x0138
	float		flInaccuracyCrouch;		// 0x013C
	float		flInaccuracyCrouchAlt;	// 0x0140
	float		flInaccuracyStand;		// 0x0144
	float		flInaccuracyStandAlt;	// 0x0148
	float		flInaccuracyJumpInitial;// 0x014C
	float		flInaccuracyJump;		// 0x0150
	float		flInaccuracyJumpAlt;	// 0x0154
	float		flInaccuracyLand;		// 0x0158
	float		flInaccuracyLandAlt;	// 0x015C
	float		flInaccuracyLadder;		// 0x0160
	float		flInaccuracyLadderAlt;	// 0x0164
	float		flInaccuracyFire;		// 0x0168
	float		flInaccuracyFireAlt;	// 0x016C
	float		flInaccuracyMove;		// 0x0170
	float		flInaccuracyMoveAlt;	// 0x0174
	float		flInaccuracyReload;		// 0x0178
	int			iRecoilSeed;			// 0x017C
	float		flRecoilAngle;			// 0x0180
	float		flRecoilAngleAlt;		// 0x0184
	float		flRecoilAngleVariance;	// 0x0188
	float		flRecoilAngleVarianceAlt;	// 0x018C
	float		flRecoilMagnitude;		// 0x0190
	float		flRecoilMagnitudeAlt;	// 0x0194
	float		flRecoilMagnitudeVariance;	// 0x0198
	float		flRecoilMagnitudeVarianceAlt;	// 0x019C
	float		flRecoveryTimeCrouch;	// 0x01A0
	float		flRecoveryTimeStand;	// 0x01A4
	float		flRecoveryTimeCrouchFinal;	// 0x01A8
	float		flRecoveryTimeStandFinal;	// 0x01AC
	int			iRecoveryTransitionStartBullet;// 0x01B0 
	int			iRecoveryTransitionEndBullet;	// 0x01B4
	bool		bUnzoomAfterShot;		// 0x01B8
	bool		bHideViewModelZoomed;	// 0x01B9
	char		pad_0x01B5[2];			// 0x01BA
	char		iZoomLevels[3];			// 0x01BC
	int			iZoomFOV[2];			// 0x01C0
	float		fZoomTime[3];			// 0x01C4
	char*		szWeaponClass;			// 0x01D4
	float		flAddonScale;			// 0x01D8
	char		pad_0x01DC[4];			// 0x01DC
	char*		szEjectBrassEffect;		// 0x01E0
	char*		szTracerEffect;			// 0x01E4
	int			iTracerFrequency;		// 0x01E8
	int			iTracerFrequencyAlt;	// 0x01EC
	char*		szMuzzleFlashEffect_1stPerson; // 0x01F0
	char		pad_0x01F4[4];			 // 0x01F4
	char*		szMuzzleFlashEffect_3rdPerson; // 0x01F8
	char		pad_0x01FC[4];			// 0x01FC
	char*		szMuzzleSmokeEffect;	// 0x0200
	float		flHeatPerShot;			// 0x0204
	char*		szZoomInSound;			// 0x0208
	char*		szZoomOutSound;			// 0x020C
	float		flInaccuracyPitchShift;	// 0x0210
	float		flInaccuracySoundThreshold;	// 0x0214
	float		flBotAudibleRange;		// 0x0218
	char		pad_0x0218[8];			// 0x0220
	char*		pWrongTeamMsg;			// 0x0224
	bool		bHasBurstMode;			// 0x0228
	char		pad_0x0225[3];			// 0x0229
	bool		bIsRevolver;			// 0x022C
	bool		bCannotShootUnderwater;	// 0x0230
};

class C_BaseCombatWeapon: public C_BaseAttributableItem
{
public:
	HANDLE GetOwner()
	{
		return *(HANDLE*)((uintptr_t)this + NetVars::DT_BaseCombatWeapon::m_hOwner);
	}

	int GetAmmo()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_BaseCombatWeapon::m_iClip1);
	}

	float GetNextPrimaryAttack()
	{
		return *(float*)((uintptr_t)this + NetVars::DT_BaseCombatWeapon::m_flNextPrimaryAttack);
	}

	bool GetInReload()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_BaseCombatWeapon::m_bInReload);
	}

	int GetReserveAmmo()
	{
		return *(int*)((uintptr_t)this + NetVars::DT_BaseCombatWeapon::m_iPrimaryReserveAmmoCount);
	}

	float GetAccuracyPenalty()
	{
		return *(float*)((uintptr_t)this + NetVars::DT_WeaponCSBase::m_fAccuracyPenalty);
	}

	CCSWeaponInfo* GetCSWpnData()
	{
		// crashes in wrong thread. update 9/1/2017
		typedef CCSWeaponInfo*(__thiscall* ogFN)(void*);
		return getvfunc<ogFN>(this, 446)(this);
	}

	CCSWeaponInfo* GetCSWpnData2()
	{
		typedef CCSWeaponInfo*(__thiscall* ogFN)(void*);
		return getvfunc<ogFN>(this, 446)(this);
	}

	float GetInaccuracy()
	{
		typedef float (__thiscall* oGetInaccuracy)(void*);
		return getvfunc<oGetInaccuracy>(this, 469)(this);  // 552
	}

	float GetSpread()
	{
		typedef float (__thiscall* oGetSpread)(void*);
		return getvfunc<oGetSpread>(this, 439)(this); // 553
	}

	void UpdateAccuracyPenalty()
	{
		typedef void (__thiscall* oUpdateAccuracyPenalty)(void*);
		return getvfunc<oUpdateAccuracyPenalty>(this, 470)(this); // 554
	}
};

class C_WeaponC4 : public C_BaseCombatWeapon
{
public:
	bool GetStartedArming()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_WeaponC4::m_bStartedArming);
	}
};

class C_Chicken : public C_BaseEntity
{
public:
	bool* GetShouldGlow()
	{
		return (bool*)((uintptr_t)this + NetVars::DT_DynamicProp::m_bShouldGlow);
	}
};

class C_BaseCSGrenade : public C_BaseCombatWeapon
{
public:
	bool IsHeldByPlayer()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_BaseCSGrenade::m_bIsHeldByPlayer);
	}

	bool GetPinPulled()
	{
		return *(bool*)((uintptr_t)this + NetVars::DT_BaseCSGrenade::m_bPinPulled);
	}

	float GetThrowTime()
	{
		return *(float*)((uintptr_t)this + NetVars::DT_BaseCSGrenade::m_fThrowTime);
	}

	float GetThrowStrength()
	{
		return *(float*)((uintptr_t)this + NetVars::DT_BaseCSGrenade::m_flThrowStrength);
	}
};
