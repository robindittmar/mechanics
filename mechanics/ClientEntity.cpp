#include "ClientEntity.h"
#include "Application.h"

void IClientEntity::SetModelIndex(int idx)
{
	*(int*)((DWORD)this + Offsets::m_nModelIndex) = idx;
}

bool IClientEntity::IsAlive()
{
	//return (this->GetHealth() > 0);
	return (this->GetLifestate() == LIFE_ALIVE);
}

int IClientEntity::GetLifestate()
{
	return  *(int*)((DWORD)this + Offsets::m_lifeState);
}

int IClientEntity::GetHealth()
{
	return *(int*)((DWORD)this + Offsets::m_iHealth);
}

int IClientEntity::GetTeamNum()
{
	return *(int*)((DWORD)this + Offsets::m_iTeamNum);
}

unsigned long IClientEntity::GetFlags()
{
	return *(unsigned long*)((DWORD)this + Offsets::m_fFlags);
}

Vector* IClientEntity::GetOrigin()
{
	return (Vector*)((DWORD)this + Offsets::m_vecOrigin);
}

Vector* IClientEntity::GetEyeOffset()
{
	return (Vector*)((DWORD)this + Offsets::m_vecViewOffset);
}

bool IClientEntity::IsScoped()
{
	return *(bool*)((DWORD)this + Offsets::m_bIsScoped);
}

CWeapon* IClientEntity::GetActiveWeapon()
{
	HANDLE hActiveWeapon = *(HANDLE*)((UCHAR*)this + Offsets::m_hActiveWeapon);
	return (CWeapon*)CApplication::Instance()->EntityList()->GetClientEntityFromHandle(hActiveWeapon);
}

void** IClientEntity::GetWeapons()
{
	return (HANDLE*)((DWORD)this + Offsets::m_hMyWeapons);
}

Vector* IClientEntity::GetVelocity()
{
	return (Vector*)((DWORD)this + Offsets::m_vecVelocity);
}

bool IClientEntity::IsSpotted()
{
	return *(bool*)((DWORD)this + Offsets::m_bSpotted);
}

int IClientEntity::GetArmor()
{
	return *(int*)((DWORD)this + Offsets::m_ArmorValue);
}

bool IClientEntity::HasHelmet()
{
	return *(bool*)((DWORD)this + Offsets::m_bHasHelmet);
}

DWORD IClientEntity::GetMoveType()
{
	return *(DWORD*)((DWORD)this + OFFSET_MOVETYPE);
}

int IClientEntity::GetTickBase()
{
	return *(int*)((DWORD)this + Offsets::m_nTickBase);
}

int IClientEntity::GetShotsFired()
{
	return *(int*)((DWORD)this + Offsets::m_iShotsFired);
}

void IClientEntity::GetPlayerInfo(PlayerInfo* p)
{
	CApplication::Instance()->EngineClient()->GetPlayerInfo(this->EntIndex(), p);
}

bool IClientEntity::IsInvincible()
{
	return *(bool*)((DWORD)this + Offsets::m_bGunGameImmunity);
}

IClientEntity* IClientEntity::GetObserverTarget()
{
	return CApplication::Instance()->EntityList()->GetClientEntityFromHandle(*(HANDLE*)((DWORD)this + Offsets::m_hObserverTarget)); // todo: crashes...
}

QAngle* IClientEntity::GetAngEyeAngles()
{
	return (QAngle*)((DWORD)this + Offsets::m_angEyeAngles);
}

float IClientEntity::GetLowerBodyYaw()
{
	return *(float*)((DWORD)this + Offsets::m_flLowerBodyYawTarget);
}

int IClientEntity::GetModelIndex()
{
	return *(int*)((DWORD)this + Offsets::m_nModelIndex);
}

QAngle* IClientEntity::GetViewPunchAngle()
{
	return (QAngle*)((DWORD)this + Offsets::m_viewPunchAngle);
}

QAngle* IClientEntity::GetAimPunchAngle()
{
	return (QAngle*)((DWORD)this + Offsets::m_aimPunchAngle);
}

CBaseViewModel* IClientEntity::GetViewModel()
{
	void* pViewModel = *(void**)((DWORD)this + Offsets::m_hViewModel);
	return (CBaseViewModel*)CApplication::Instance()->EntityList()->GetClientEntityFromHandle(pViewModel);
}