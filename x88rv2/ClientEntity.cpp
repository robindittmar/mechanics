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

float IClientEntity::GetVelocity()
{
	return *(float*)((DWORD)this + OFFSET_VELOCITY);
}