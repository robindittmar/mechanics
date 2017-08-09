//#include "ClientEntity.h"
#include "Application.h"

bool IClientEntity::IsAlive()
{
	return (this->Health() > 0);
}

int IClientEntity::Health()
{
	return *(int*)((unsigned long)this + OFFSET_HEALTH);
}

int IClientEntity::TeamNum()
{
	return *(int*)((unsigned long)this + OFFSET_TEAM);
}

unsigned long IClientEntity::Flags()
{
	return *(unsigned long*)((unsigned long)this + OFFSET_FLAGS);
}

Vector* IClientEntity::Origin()
{
	return (Vector*)((unsigned long)this + OFFSET_ORIGIN);
}

Vector* IClientEntity::EyeOffset()
{
	return (Vector*)((unsigned long)this + OFFSET_EYEPOS);
}

bool IClientEntity::IsScoped()
{
	return *(bool*)((unsigned long)this + OFFSET_ISSCOPED);
}

void* IClientEntity::ActiveWeapon()
{
	CApplication* pApp = CApplication::Instance();

	HANDLE hActiveWeapon = *(PHANDLE)((PUCHAR)this + OFFSET_ACTIVEWEAPON);
	return (void*)pApp->EntityList()->GetClientEntityFromHandle(hActiveWeapon);
}

Vector* IClientEntity::Velocity()
{
	return (Vector*)((DWORD)this + OFFSET_VELOCITY);
}

bool IClientEntity::IsSpotted()
{
	return *(bool*)((DWORD)this + OFFSET_SPOTTED);
}

int IClientEntity::Armor()
{
	return *(int*)((DWORD)this + OFFSET_ARMOR);
}

bool IClientEntity::HasHelmet()
{
	return *(bool*)((DWORD)this + OFFSET_HELMET);
}

DWORD IClientEntity::MoveType()
{
	return *(DWORD*)((DWORD)this + OFFSET_MOVETYPE);
}

int IClientEntity::TickBase()
{
	return *(int*)((DWORD)this + OFFSET_TICKBASE);
}

int IClientEntity::ShotsFired()
{
	return *(int*)((DWORD)this + OFFSET_SHOTSFIRED);
}

PlayerInfo IClientEntity::GetPlayerInfo()
{
	CApplication* pApp = CApplication::Instance();

	PlayerInfo pInfo;
	pApp->EngineClient()->GetPlayerInfo(this->EntIndex(), &pInfo);

	return pInfo;
}

bool IClientEntity::IsInvincible()
{
	return *(bool*)((DWORD)this + OFFSET_SPAWNPROTECTION);
}