#pragma once

struct CCSWeaponInfo;

class IWeaponSystem
{
	virtual void unused0() = 0;
	virtual void unused1() = 0;
public:
	virtual const CCSWeaponInfo* GetWpnData(ItemDefinitionIndex) = 0;
};