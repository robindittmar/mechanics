#include "LagCompensationPlayerList.h"
#include "Application.h"
#include "XorString.h"

void CLagCompensationPlayerList::RemoveInvalidPlayerEntries()
{
	CApplication* pApp = CApplication::Instance();
	float fStoreTime = pApp->Ragebot()->GetEnabled() ? 200 : pApp->LagCompensation()->GetLegitLagCompensationDuration();

	float fCurTime = CApplication::Instance()->GlobalVars()->curtime * 1000;

	int iTempCount = 0;
	CLagCompensationPlayerEntry pTempList[LC_MAXSAVEDTICKS];

	for (int i = 0; i < min(m_iEntryCount, LC_MAXSAVEDTICKS - 1); i++)
	{
		// End of list -> should never be the case!!
		if (m_pPlayerEntries[i].m_bIsEndOfList)
			break;

		// Check for valid entries
		if (fCurTime - m_pPlayerEntries[i].m_llAddTime < fStoreTime)
		{
			pTempList[iTempCount++] = m_pPlayerEntries[i];
		}
	}

	m_iEntryCount = iTempCount;
	for (int i = 0; i < iTempCount; i++)
	{
		// End of list -> should never be the case!!
		if (pTempList[i].m_bIsEndOfList)
			break;

		m_pPlayerEntries[i].m_iTickCount = pTempList[i].m_iTickCount;

		for (int x = 0; x < MAXSTUDIOBONES; x++)
			m_pPlayerEntries[i].m_pBoneMatrix[x] = pTempList[i].m_pBoneMatrix[x];

		m_pPlayerEntries[i].m_vOrigin = pTempList[i].m_vOrigin;
		m_pPlayerEntries[i].m_llAddTime = pTempList[i].m_llAddTime;
		m_pPlayerEntries[i].m_vHeadPos = pTempList[i].m_vHeadPos;
		m_pPlayerEntries[i].m_vVelocity = pTempList[i].m_vVelocity;
		m_pPlayerEntries[i].m_angEyeAngles = pTempList[i].m_angEyeAngles;
		m_pPlayerEntries[i].m_fSimulationTime = pTempList[i].m_fSimulationTime;
		m_pPlayerEntries[i].m_iSequenceNumber = pTempList[i].m_iSequenceNumber;
		m_pPlayerEntries[i].m_fFlags = pTempList[i].m_fFlags;
		m_pPlayerEntries[i].m_fCycle = pTempList[i].m_fCycle;
		m_pPlayerEntries[i].m_fLowerBodyYaw = pTempList[i].m_fLowerBodyYaw;
		for (int x = 0; x < MAXSTUDIOPOSEPARAM; x++)
		{
			m_pPlayerEntries[i].m_fPoseParameters[x] = pTempList[i].m_fPoseParameters[x];
		}
		m_pPlayerEntries[i].m_bIsEndOfList = false;
	}

	// Setting end of list
	m_pPlayerEntries[iTempCount + 1].m_bIsEndOfList = true;
}

void CLagCompensationPlayerList::AddPlayerEntry(IClientEntity* pCurEnt, int tickcount)
{
	CApplication* pApp = CApplication::Instance();
	if ((!pApp->Ragebot()->GetEnabled() || !pApp->LagCompensation()->GetRageLagCompensationEnabled()) && !pApp->LagCompensation()->GetLegitLagCompensationEnabled())
		return;

	static CXorString pHeadZero("nä¦H;");

	float intervalPerTick = CApplication::Instance()->GlobalVars()->interval_per_tick;

	matrix3x4_t pBoneMatrix[MAXSTUDIOBONES];
	if (!pCurEnt->SetupBones(pBoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, CApplication::Instance()->EngineClient()->GetLastTimeStamp()))
		return;

	m_pPlayerEntries[m_iEntryCount].m_iTickCount = tickcount;

	for (int i = 0; i < MAXSTUDIOBONES; i++)
		m_pPlayerEntries[m_iEntryCount].m_pBoneMatrix[i] = pBoneMatrix[i];

	m_pPlayerEntries[m_iEntryCount].m_vOrigin = *pCurEnt->GetOrigin();
	m_pPlayerEntries[m_iEntryCount].m_llAddTime = CApplication::Instance()->GlobalVars()->curtime * 1000;
	MatrixGetColumn(pBoneMatrix[pCurEnt->GetBoneByName(pHeadZero.ToCharArray())], 3, m_pPlayerEntries[m_iEntryCount].m_vHeadPos);
	m_pPlayerEntries[m_iEntryCount].m_vHeadPos += *pCurEnt->GetVelocity() * intervalPerTick;
	m_pPlayerEntries[m_iEntryCount].m_vVelocity = *pCurEnt->GetVelocity();
	m_pPlayerEntries[m_iEntryCount].m_angEyeAngles = *pCurEnt->GetAngEyeAngles();
	m_pPlayerEntries[m_iEntryCount].m_fSimulationTime = pCurEnt->GetSimulationTime();
	m_pPlayerEntries[m_iEntryCount].m_iSequenceNumber = pCurEnt->GetSequence();
	m_pPlayerEntries[m_iEntryCount].m_fFlags = pCurEnt->GetFlags();
	m_pPlayerEntries[m_iEntryCount].m_fCycle = pCurEnt->GetCycle();
	m_pPlayerEntries[m_iEntryCount].m_fLowerBodyYaw = pCurEnt->GetLowerBodyYaw();
	for (int i = 0; i < MAXSTUDIOPOSEPARAM; i++)
	{
		m_pPlayerEntries[m_iEntryCount].m_fPoseParameters[i] = pCurEnt->GetPoseParameter(i);
	}
	m_pPlayerEntries[m_iEntryCount].m_bIsEndOfList = false;

	// Setting end of list
	m_pPlayerEntries[++m_iEntryCount].m_bIsEndOfList = true;
}

void CLagCompensationPlayerList::RestorePlayerEntry(IClientEntity* pCurEnt, int iEntryIndex)
{
	CApplication* pApp = CApplication::Instance();
	if (!pApp->LagCompensation()->GetRageLagCompensationEnabled() && !pApp->LagCompensation()->GetLegitLagCompensationEnabled())
		return;

	CLagCompensationPlayerEntry pEntry = m_pPlayerEntries[iEntryIndex];

	*(Vector*)((DWORD)pCurEnt + Offsets::m_vecOrigin) = pEntry.m_vOrigin;
	*(Vector*)((DWORD)pCurEnt + Offsets::m_vecVelocity) = pEntry.m_vVelocity;
	*(float*)((DWORD)pCurEnt + Offsets::m_flSimulationTime) = pEntry.m_fSimulationTime;
	*(QAngle*)((DWORD)pCurEnt + Offsets::m_angEyeAngles) = pEntry.m_angEyeAngles;
	*(float*)((DWORD)pCurEnt + Offsets::m_flCycle) = pEntry.m_fCycle;
	*(int*)((DWORD)pCurEnt + Offsets::m_nSequence) = pEntry.m_iSequenceNumber;
	*(DWORD*)((DWORD)pCurEnt + Offsets::m_fFlags) = pEntry.m_fFlags;
	*(float*)((DWORD)pCurEnt + Offsets::m_flLowerBodyYawTarget) = pEntry.m_fLowerBodyYaw;

	for (int i = 0; i < MAXSTUDIOPOSEPARAM; i++)
	{
		*(float*)((DWORD)pCurEnt + Offsets::m_flPoseParameter + sizeof(float) * i) = pEntry.m_fPoseParameters[i];
	}
}
