#include "CWeapon.h"


int CWeapon::WeaponId()
{
	return *(int*)((unsigned long)this + OFFSET_ATRIBUTEMANAGER + OFFSET_ITEM + OFFSET_ITEMDEFINITIONINDEX);
}

bool CWeapon::IsKnife()
{
	int weaponId = this->WeaponId();
	if (weaponId == WEAPON_KNIFE ||
		weaponId == WEAPON_KNIFE_BAYONET ||
		weaponId == WEAPON_KNIFE_FLIP ||
		weaponId == WEAPON_KNIFE_GUT ||
		weaponId == WEAPON_KNIFE_KARAMBIT ||
		weaponId == WEAPON_KNIFE_M9_BAYONET ||
		weaponId == WEAPON_KNIFE_BUTTERFLY ||
		weaponId == WEAPON_KNIFE_FALCHION ||
		weaponId == WEAPON_KNIFE_PUSH ||
		weaponId == WEAPON_KNIFE_SURVIVAL_BOWIE ||
		weaponId == WEAPON_KNIFE_T ||
		weaponId == WEAPON_KNIFE_TACTICAL)
		return true;
	return false;
}

bool CWeapon::IsSniper()
{
	int weaponId = this->WeaponId();
	if (weaponId == WEAPON_AWP ||
		weaponId == WEAPON_SSG08 ||
		weaponId == WEAPON_G3SG1 ||
		weaponId == WEAPON_SCAR20)
		return true;
	return false;
}

bool CWeapon::IsNade()
{
	int weaponId = this->WeaponId();
	if (weaponId == WEAPON_HEGRENADE ||
		weaponId == WEAPON_INCGRENADE ||
		weaponId == WEAPON_DECOY ||
		weaponId == WEAPON_FLASHBANG ||
		weaponId == WEAPON_MOLOTOV ||
		weaponId == WEAPON_SMOKEGRENADE)
		return true;
	return false;
}

bool CWeapon::IsC4()
{
	return (this->WeaponId() == WEAPON_C4);
}

bool CWeapon::IsPistol()
{
	int weaponId = this->WeaponId();
	if (weaponId == WEAPON_DEAGLE ||
		weaponId == WEAPON_REVOLVER ||
		weaponId == WEAPON_FIVESEVEN ||
		weaponId == WEAPON_USP_SILENCER ||
		weaponId == WEAPON_GLOCK ||
		weaponId == WEAPON_TEC9 ||
		weaponId == WEAPON_CZ75A ||
		weaponId == WEAPON_P250 ||
		weaponId == WEAPON_HKP2000 ||
		weaponId == WEAPON_ELITE)
		return true;
	return false;
}

bool CWeapon::IsTaser()
{
	return (this->WeaponId() == WEAPON_TASER);
}

int CWeapon::Clip1()
{
	return *(int*)((unsigned long)this + OFFSET_CLIP1);
}

int CWeapon::Clip2()
{
	return *(int*)((unsigned long)this + OFFSET_CLIP2);
}

float CWeapon::NextPrimaryAttack()
{
	return *(float*)((unsigned long)this + OFFSET_NEXTPRIMARYATTACK);
}

float CGrenade::ThrowTime()
{
	return *(float*)((unsigned long)this + OFFSET_THROWTIME);
}

CWeaponInfo* CWeapon::GetWeaponInfo()
{
	typedef CWeaponInfo*(__thiscall* GetWeaponData_t)(void*);
	return ((GetWeaponData_t)(*(void***)this)[446])(this);
}

float CWeapon::PostPoneFireReady()
{
	return *(float*)((unsigned long)this + OFFSET_POSTPONEFIREREADY);
}