#include "CWeapon.h"

int CWeapon::WeaponId()
{
	return *(int*)((unsigned long)this + OFFSET_WEAPONID);
}

bool CWeapon::IsKnife()
{
	int weaponId = this->WeaponId();
	if (weaponId == WEAPON_KNIFE ||
		weaponId == WEAPON_KNIFE_BAYONET ||
		weaponId == WEAPON_KNIFE_BUTTERFLY ||
		weaponId == WEAPON_KNIFE_FALCHION ||
		weaponId == WEAPON_KNIFE_FLIP ||
		weaponId == WEAPON_KNIFE_GUT ||
		weaponId == WEAPON_KNIFE_KARAMBIT ||
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

int CWeapon::Clip1()
{
	return *(int*)((unsigned long)this + OFFSET_CLIP1);
}

int CWeapon::Clip2()
{
	return *(int*)((unsigned long)this + OFFSET_CLIP2);
}