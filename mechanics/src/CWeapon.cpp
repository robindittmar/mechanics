#include "CWeapon.h"

int CWeapon::GetWeaponId()
{
	return *(int*)((unsigned long)this + Offsets::m_iItemDefinitionIndex);
}

bool CWeapon::IsWeaponX(int weaponId)
{
	return this->GetWeaponId() == weaponId;
}

bool CWeapon::IsKnife()
{
	int weaponId = this->GetWeaponId();
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
	int weaponId = this->GetWeaponId();
	if (weaponId == WEAPON_AWP ||
		weaponId == WEAPON_SSG08 ||
		weaponId == WEAPON_G3SG1 ||
		weaponId == WEAPON_SCAR20)
		return true;
	return false;
}

bool CWeapon::IsNade()
{
	int weaponId = this->GetWeaponId();
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
	return (this->GetWeaponId() == WEAPON_C4);
}

bool CWeapon::IsPistol()
{
	int weaponId = this->GetWeaponId();
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
	return (this->GetWeaponId() == WEAPON_TASER);
}

int CWeapon::GetOwnerId()
{
	return *(int*)((unsigned long)this + Offsets::m_hOwner);
}

int CWeapon::GetClip1()
{
	return *(int*)((unsigned long)this + Offsets::m_iClip1);
}

int CWeapon::GetClip2()
{
	return *(int*)((unsigned long)this + Offsets::m_iClip2);
}

float CWeapon::GetNextPrimaryAttack()
{
	return *(float*)((unsigned long)this + Offsets::m_flNextPrimaryAttack);
}

float CGrenade::GetThrowTime()
{
	return *(float*)((unsigned long)this + Offsets::m_fThrowTime);
}

CWeaponInfo* CWeapon::GetWeaponInfo()
{
	typedef CWeaponInfo*(__thiscall* GetWeaponData_t)(void*);
	return ((GetWeaponData_t)(*(void***)this)[446])(this);
}

float CWeapon::GetAccuracyPenalty()
{
	return *(float*)((unsigned long)this + Offsets::m_fAccuracyPenalty);
}

float CWeapon::GetPostPoneFireReady()
{
	return *(float*)((unsigned long)this + Offsets::m_flPostponeFireReadyTime);
}

Vector CWeapon::GetVecMin()
{
	return *(Vector*)((unsigned long)this + Offsets::m_vecMins);
}

Vector CWeapon::GetVecMax()
{
	return *(Vector*)((unsigned long)this + Offsets::m_vecMaxs);
}

void CWeapon::UpdateAccuracyPenalty()
{
	typedef void(__thiscall *UpdateAccuracyPenalty_t)(void*);
	((UpdateAccuracyPenalty_t)(*(void***)this)[470])(this);
}

float CWeapon::GetInaccuracy()
{
	typedef float(__thiscall *GetInaccuracy_t)(void*);
	return ((GetInaccuracy_t)(*(void***)this)[469])(this);
}

float CWeapon::GetSpread()
{
	typedef float(__thiscall* GetSpread_t)(void*);
	return ((GetSpread_t)(*(void***)this)[439])(this);
}