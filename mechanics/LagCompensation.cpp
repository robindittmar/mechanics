#include "LagCompensation.h"
#include "Application.h"

CLagCompensationPlayerEntry::CLagCompensationPlayerEntry()
{
	m_bIsEndOfList = true;
}


//float CLagCompensationPlayerEntry::LerpTime()
//{
//	CApplication* pApp = CApplication::Instance();
//	static ConVar* cl_update_rate = pApp->CVar()->FindVar("cl_updaterate");
//	static ConVar* min_ud_rate = pApp->CVar()->FindVar("sv_minupdaterate");
//	static ConVar* max_ud_rate = pApp->CVar()->FindVar("sv_maxupdaterate");
//
//	static ConVar* cl_interp_ratio = pApp->CVar()->FindVar("cl_interp_ratio");
//
//	static ConVar* cl_interp = pApp->CVar()->FindVar("cl_interp");
//	static ConVar* c_min_ratio = pApp->CVar()->FindVar("sv_client_min_interp_ratio");
//	static ConVar* c_max_ratio = pApp->CVar()->FindVar("sv_client_max_interp_ratio");
//
//	int ud_rate = atoi(cl_update_rate->value);
//
//
//	if (min_ud_rate && max_ud_rate)
//		ud_rate = atoi(max_ud_rate->value);
//
//	float ratio = atof(cl_interp_ratio->value);
//
//	if (ratio == 0)
//		ratio = 1.0f;
//
//	float lerp = atof(cl_interp->value);
//
//	if (c_min_ratio && c_max_ratio && atof(c_min_ratio->value) != 1)
//		ratio = clamp(ratio, atof(c_min_ratio->value), atof(c_max_ratio->value));
//
//	return fmax(lerp, (ratio / ud_rate));
//}
//
//int CLagCompensationPlayerEntry::FixedTickcount(IClientEntity* pCur)
//{
//	if (m_fSimulationTime == 0.0f)
//		return TIME_TO_TICKS(pCur->GetSimulationTime() + LerpTime()) + 1;
//
//	int iLerpTicks = TIME_TO_TICKS(LerpTime());
//	int iTargetTickCount = TIME_TO_TICKS(m_fSimulationTime) + iLerpTicks;
//
//	return iTargetTickCount + 1;
//}

void LagCompensationList::RemoveInvalidPlayerEntries()
{
	ULONGLONG llCurTime = GetTickCount64();

	int iTempCount = 0;
	CLagCompensationPlayerEntry pTempList[LC_MAXSAVEDTICKS];

	for (int i = 0; i < min(m_iEntryCount, LC_MAXSAVEDTICKS - 1); i++)
	{
		// End of list -> should never be the case!!
		if (m_pPlayerEntries[i].m_bIsEndOfList)
			break;

		// Check for valid entries
		if (llCurTime - m_pPlayerEntries[i].m_llAddTime < 200)
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

void LagCompensationList::AddPlayerEntry(IClientEntity* pCurEnt, int tickcount)
{
	static CXorString pHeadZero("nä¦H;");

	float intervalPerTick = CApplication::Instance()->GlobalVars()->interval_per_tick;

	matrix3x4_t pBoneMatrix[MAXSTUDIOBONES];
	if (!pCurEnt->SetupBones(pBoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, CApplication::Instance()->EngineClient()->GetLastTimeStamp()))
		return;

	m_pPlayerEntries[m_iEntryCount].m_iTickCount = tickcount;

	for (int i = 0; i < MAXSTUDIOBONES; i++)
		m_pPlayerEntries[m_iEntryCount].m_pBoneMatrix[i] = pBoneMatrix[i];

	m_pPlayerEntries[m_iEntryCount].m_vOrigin = *pCurEnt->GetOrigin();
	m_pPlayerEntries[m_iEntryCount].m_llAddTime = GetTickCount64();
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

void LagCompensationList::RestorePlayerEntry(IClientEntity* pCurEnt, int iEntryIndex)
{
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

CLagCompensation::CLagCompensation()
{
	m_iDrawStyle = LC_DRAWSTYLE_NONE;
	m_iDrawFrequency = 1;
}

CLagCompensation::~CLagCompensation()
{
}

void CLagCompensation::Setup()
{
	m_pApp = CApplication::Instance();
}

void CLagCompensation::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	IClientEntity* pLocalEnt = m_pApp->GetLocalPlayer();
	if (!pLocalEnt)
		return;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		IClientEntity* pCurEnt = m_pApp->EntityList()->GetClientEntity(i);
		if (!pCurEnt)
			continue;

		if (pCurEnt == pLocalEnt)
			continue;

		if (pCurEnt->IsDormant())
			continue;

		if (!(pCurEnt->GetFlags() & FL_CLIENT))
			continue;

		if (!pCurEnt->IsAlive())
			continue;

		if (pCurEnt->GetTeamNum() == pLocalEnt->GetTeamNum())
			continue;

		m_pPlayerList[i].RemoveInvalidPlayerEntries();
		m_pPlayerList[i].AddPlayerEntry(pCurEnt, (int)pParameters);
	}
}

int CLagCompensation::RestorePlayerClosestToCrosshair()
{
	if (!m_bIsEnabled)
		return -1;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return -1;

	Vector vLocalHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();
	QAngle qLocalAngles = m_pApp->GetClientViewAngles();
	
	QAngle aimPunchAngle = *pLocalEntity->GetAimPunchAngle();
	qLocalAngles.x -= aimPunchAngle.x * m_pApp->GetRecoilCompensation();
	qLocalAngles.y -= aimPunchAngle.y * m_pApp->GetRecoilCompensation();

	float fViewangleDist = 99999.0f;
	int iCurTickcount = -1;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		IClientEntity* pCurEnt = m_pApp->EntityList()->GetClientEntity(i);
		if (!pCurEnt)
			continue;

		if (pCurEnt == pLocalEntity)
			continue;

		if (pCurEnt->IsDormant())
			continue;

		if (!(pCurEnt->GetFlags() & FL_CLIENT))
			continue;

		if (!pCurEnt->IsAlive())
			continue;

		if (pCurEnt->GetTeamNum() == pLocalEntity->GetTeamNum())
			continue;

		LagCompensationList lcCurList = m_pPlayerList[i];
		//lcCurList.RemoveInvalidPlayerEntries();
		for (int x = 0; x < min(lcCurList.m_iEntryCount, LC_MAXSAVEDTICKS); x++)
		{
			// End of list -> should never be the case!!
			if (lcCurList.m_pPlayerEntries[x].m_bIsEndOfList)
				break;

			QAngle qCurAngle = Utils::CalcAngle(vLocalHeadPos, lcCurList.m_pPlayerEntries[x].m_vHeadPos);
			float fCurDist = (qLocalAngles - qCurAngle).Length();

			if (fCurDist < fViewangleDist)
			{
				fViewangleDist = fCurDist;
				iCurTickcount = lcCurList.m_pPlayerEntries[x].m_iTickCount;
			}
		}
	}

	return iCurTickcount;
}

void CLagCompensation::DrawLagCompensationEntries()
{
	if (m_iDrawStyle == LC_DRAWSTYLE_NONE)
		return;

	IClientEntity* pLocalEnt = m_pApp->GetLocalPlayer();
	if (!pLocalEnt)
		return;

	Vector vScreenOrigin;

	m_pApp->Surface()->DrawSetColor(255, 255, 255, 255);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		IClientEntity* pCurEnt = m_pApp->EntityList()->GetClientEntity(i);
		if (!pCurEnt)
			continue;

		if (pCurEnt == pLocalEnt)
			continue;

		if (pCurEnt->IsDormant())
			continue;

		if (!(pCurEnt->GetFlags() & FL_CLIENT))
			continue;

		if (!pCurEnt->IsAlive())
			continue;

		if (pCurEnt->GetTeamNum() == pLocalEnt->GetTeamNum())
			continue;

		LagCompensationList lcCurList = m_pPlayerList[i];
		lcCurList.RemoveInvalidPlayerEntries();
		for (int x = 0; x < min(lcCurList.m_iEntryCount, LC_MAXSAVEDTICKS); x += m_iDrawFrequency)
		{
			// End of list -> should never be the case!!
			if (lcCurList.m_pPlayerEntries[x].m_bIsEndOfList)
				break;

			switch (m_iDrawStyle)
			{
			case LC_DRAWSTYLE_CROSS:
				if (m_pApp->Gui()->WorldToScreen(lcCurList.m_pPlayerEntries[x].m_vHeadPos, vScreenOrigin))
				{
					m_pApp->Surface()->DrawLine(vScreenOrigin.x - 2, vScreenOrigin.y - 2, vScreenOrigin.x + 2, vScreenOrigin.y + 2);
					m_pApp->Surface()->DrawLine(vScreenOrigin.x - 2, vScreenOrigin.y + 2, vScreenOrigin.x + 2, vScreenOrigin.y - 2);
				}
				break;
			case LC_DRAWSTYLE_BONES:
				m_pApp->Esp()->DrawSkeleton(m_pApp->Surface(), pCurEnt, lcCurList.m_pPlayerEntries[x].m_pBoneMatrix, 255);
			default:
				break;
			}
		}
	}
}

LagCompensationList* CLagCompensation::GetLCList(int index)
{
	return &m_pPlayerList[index];
}
