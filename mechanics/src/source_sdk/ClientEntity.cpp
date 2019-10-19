#include "ClientEntity.h"
#include "../Application.h"

void IClientEntity::SetModelIndex(int idx)
{
	*(int*)((DWORD)this + Offsets::m_nModelIndex) = idx;
}

CAnimationLayer& IClientEntity::GetAnimationLayer(int iIndex)
{
	return (*(CAnimationLayer**)((DWORD)this + 0x2970))[iIndex];
}

int IClientEntity::GetSequenceActivity(int nSequence)
{
	studiohdr_t* hdr = CApplication::Instance()->ModelInfo()->GetStudiomodel(this->GetModel());

	if (!hdr)
		return -1;

	// c_csplayer vfunc 242, follow calls to find the function.
	static auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>((CPattern::FindPattern(
		(BYTE*)CApplication::Instance()->ClientDll(),
		CLIENTDLL_SIZE,
		(BYTE*)"\x55\x8B\xEC\x83\x7D\x08\xFF\x56\x8B\xF1\x74\x3D",
		"fgkhlrecgnef"
		/*(BYTE*)"\x55\x8B\xEC\x57\x8B\xF9\x8B\x97\x00\x00\x00\x00\x85\xD2",
		"fgtujher----ft"*/
	)));

	return GetSequenceActivity(this, hdr, nSequence);
}

bool IClientEntity::IsAlive()
{
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

float IClientEntity::GetSimulationTime()
{
	return *(float*)((DWORD)this + Offsets::m_flSimulationTime);
}

float IClientEntity::GetOldSimulationTime()
{
	return *(float*)((DWORD)this + Offsets::m_flSimulationTime + 0x4);
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

void IClientEntity::SetSpotted(bool bSpotted)
{
	*(bool*)((DWORD)this + Offsets::m_bSpotted) = bSpotted;
}

int IClientEntity::GetArmor()
{
	return *(int*)((DWORD)this + Offsets::m_ArmorValue);
}

bool IClientEntity::HasHeavyArmor()
{
	return *(bool*)((DWORD)this + Offsets::m_bHasHeavyArmor);
}

bool IClientEntity::HasHelmet()
{
	return *(bool*)((DWORD)this + Offsets::m_bHasHelmet);
}

int IClientEntity::GetMoveType()
{
	return *(int*)((DWORD)this + OFFSET_MOVETYPE);
}

int IClientEntity::GetTickBase()
{
	return *(int*)((DWORD)this + Offsets::m_nTickBase);
}

bool IClientEntity::IsDefusing()
{
	return *(bool*)((DWORD)this + Offsets::m_bIsDefusing);
}

float IClientEntity::GetC4Blow()
{
	return *(float*)((DWORD)this + Offsets::m_flC4Blow);
}

float IClientEntity::GetDefuseCountDown()
{
	return *(float*)((DWORD)this + Offsets::m_flDefuseCountDown);
}

HANDLE IClientEntity::GetDefuser()
{
	return *(HANDLE*)((DWORD)this + Offsets::m_hBombDefuser);
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

QAngle* IClientEntity::GetAngRotation()
{
	return (QAngle*)((DWORD)this + Offsets::m_angRotation);
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

int IClientEntity::GetSequence()
{
	return *(int*)((DWORD)this + Offsets::m_nSequence);
}

float* IClientEntity::GetPoseParameter()
{
	return (float*)((DWORD)this + Offsets::m_flPoseParameter);
}

float IClientEntity::GetPoseParameter(int index)
{
	return *(float*)((DWORD)this + Offsets::m_flPoseParameter + sizeof(float) * index);
}

float IClientEntity::GetCycle()
{
	return *(float*)((DWORD)this + Offsets::m_flCycle);
}

void IClientEntity::UpdateClientSiteAnimation()
{
	typedef void(__thiscall *UpdateClientSiteAnimation_t)(void*);
	((UpdateClientSiteAnimation_t)(*(void***)this)[223])(this);
}

CAnimstate* IClientEntity::GetAnimState()
{
    return *reinterpret_cast<CAnimstate**>(uintptr_t(this) + 0x3900); //TODO: offset iwie anders kriegen?
}

float IClientEntity::GetMaxDesyncDelta() {
    auto animstate = uintptr_t(this->GetAnimState());

    float duckamount = *(float *) (animstate + 0xA4);

    float speedfraction = max(0, min(*reinterpret_cast<float *>(animstate + 0xF8), 1));
    float speedfactor = max(0, min(1, *reinterpret_cast<float *> (animstate + 0xFC)));

    float unk1 = ((*reinterpret_cast<float *> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
    float unk2 = unk1 + 1.f;
    float unk3;

    if (duckamount > 0)
        unk2 += ((duckamount * speedfactor) * (0.5f - unk2));

    unk3 = *(float *) (animstate + 0x334) * unk2;

    return unk3;
}

float IClientEntity::GetDuckSpeed()
{
    return *(float*)((DWORD)this + Offsets::m_flDuckSpeed);
}

float IClientEntity::GetDuckAmount()
{
    return *(float*)((DWORD)this + Offsets::m_flDuckAmount);
}

int IClientEntity::GetBoneByName(const char* pBoneName)
{
	CApplication* pApp = CApplication::Instance();

	studiohdr_t* pStudioHdr = pApp->ModelInfo()->GetStudiomodel(this->GetModel());
	for (int i = 0; i < pStudioHdr->numbones; i++)
	{
		mstudiobone_t* pBone = pStudioHdr->pBone(i);
		if (!pBone)
			continue;
		
		if (pBone->pszName() && !strcmp(pBone->pszName(), pBoneName))
			return i;
	}

	return -1;
}