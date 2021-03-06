#include "LagCompensation.h"
#include "../Application.h"

CLagCompensation::CLagCompensation()
	: m_iTickcount(-1), m_iDrawStyle(LC_DRAWSTYLE_NONE), m_iDrawFrequency(1),
	m_bDrawOnlyVisible(false), m_bRageLagCompensationEnabled(false),
	m_bLegitLagCompensationEnabled(false), m_bLegitLagCompensationDuration(200)
{
}

CLagCompensation::~CLagCompensation()
{
}

void CLagCompensation::Think(void* pParameters)
{
	IClientEntity* pLocalEnt = m_pApp->GetLocalPlayer();
	if (!pLocalEnt)
		return;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		IClientEntity* pCurEnt = m_pApp->EntityList()->GetClientEntity(i);
		if (!pCurEnt)
			continue;

		m_pPlayerList[i].RemoveInvalidPlayerEntries();

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

		m_pPlayerList[i].AddPlayerEntry(pCurEnt, m_iTickcount);
	}
}

int CLagCompensation::RestorePlayerClosestToCrosshair()
{
	if (!m_bLegitLagCompensationEnabled)
		return -1;

	if (m_pApp->Ragebot()->IsShooting())
		return -1;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return -1;

	Vector vLocalHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();
	QAngle qLocalAngles = m_pApp->GetClientViewAngles() + (*pLocalEntity->GetAimPunchAngle() * m_pApp->GunAccuracy()->GetRecoilCompensation());

	float fViewangleDist = 99999.0f;
	int iCurTickcount = -1;

	int iPlayerCount = m_pApp->GlobalVars()->maxClients;
	for (int i = 0; i < iPlayerCount; i++)
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

		CLagCompensationPlayerList lcCurList = m_pPlayerList[i];
		//lcCurList.RemoveInvalidPlayerEntries();
#ifdef __MINGW32__
		for (int x = 0; x < std::min(lcCurList.m_iEntryCount, LC_MAXSAVEDTICKS); x++)
#else
        for (int x = 0; x < min(lcCurList.m_iEntryCount, LC_MAXSAVEDTICKS); x++)
#endif
		{
			// End of list -> should never be the case!!
			if (lcCurList.m_pPlayerEntries[x].m_bIsEndOfList)
				break;

			QAngle qCurAngle = Utils::CalcAngle(vLocalHeadPos, lcCurList.m_pPlayerEntries[x].m_vHeadPos);
			float fCurDist = (qLocalAngles - qCurAngle).Length();
			if (fCurDist > 15.0f) // TODO: Find good min angle
				continue;

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

	if ((!m_pApp->Ragebot()->GetEnabled() || !m_bRageLagCompensationEnabled) && !m_bLegitLagCompensationEnabled)
		return;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	Vector vLocalHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();
	Vector vCurHeadPos;
	Vector vScreenOrigin;

	Ray_t ray;
	trace_t trace;
	CTraceFilterWorldAndPropsOnly filter;

	m_pApp->Surface()->DrawSetColor(255, 255, 255, 255);
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

		CLagCompensationPlayerList lcCurList = m_pPlayerList[i];
		lcCurList.RemoveInvalidPlayerEntries();

#ifdef __MINGW32__
        for (int x = 0; x < std::min(lcCurList.m_iEntryCount, LC_MAXSAVEDTICKS); x += m_iDrawFrequency)
#else
        for (int x = 0; x < min(lcCurList.m_iEntryCount, LC_MAXSAVEDTICKS); x += m_iDrawFrequency)
#endif
		{
			// End of list -> should never be the case!!
			if (lcCurList.m_pPlayerEntries[x].m_bIsEndOfList)
				break;

			if (*pCurEnt->GetOrigin() == lcCurList.m_pPlayerEntries[x].m_vOrigin && pCurEnt->GetFlags() == lcCurList.m_pPlayerEntries[x].m_fFlags)
				continue;

			if (m_bDrawOnlyVisible)
			{
				vCurHeadPos = lcCurList.m_pPlayerEntries[x].m_vHeadPos;
				ray.Init(vLocalHeadPos, vCurHeadPos);
				m_pApp->EngineTrace()->TraceRay(ray, 0x200400B, &filter, &trace);
				if (!trace.IsVisible())
					continue;
			}

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

void CLagCompensation::DrawLagCompensationIndicator()
{
	if (m_bRageLagCompensationEnabled && m_pApp->Ragebot()->GetEnabled())
	{
		if (m_pApp->Ragebot()->GetSelectedTargetIdx() == -1)
			return;

		m_pApp->Surface()->DrawSetTextFont(g_pResourceManager->GetFont(RM_FONT_HEADER));
		CTarget* pTarget = m_pApp->Ragebot()->GetSelectedTarget();

		static IClientEntity* pTargetEntity = nullptr;
		if (!pTargetEntity)
		{
			pTargetEntity = pTarget->GetEntity();
		}

		// Check if got LC target
		bool bGotLCEntries = pTargetEntity;
		if (pTargetEntity)
		{
			if (pTargetEntity->IsDormant())
			{
				bGotLCEntries = false;
			}
			else
			{
				CLagCompensationPlayerList* lcList = m_pApp->LagCompensation()->GetLCList(pTargetEntity->EntIndex());
				lcList->RemoveInvalidPlayerEntries();
				bGotLCEntries = lcList->m_iEntryCount > 0;
			}
		}

		// Draw LC Indicator
		if (pTargetEntity && bGotLCEntries)
		{
			m_pApp->Surface()->DrawSetTextColor(255, 0, 255, 0);
		}
		else
		{
			if (!bGotLCEntries)
			{
				pTargetEntity = nullptr;
			}
			m_pApp->Surface()->DrawSetTextColor(255, 255, 0, 0);
		}

		int w, h;
		m_pApp->EngineClient()->GetScreenSize(w, h);
		m_pApp->Surface()->DrawSetTextPos(70, h - 55);
		m_pApp->Surface()->DrawPrintText(L"LC", 2);
	}
	else if (m_bLegitLagCompensationEnabled)
	{
		//todo
	}
}

CLagCompensationPlayerList* CLagCompensation::GetLCList(int index)
{
	return &m_pPlayerList[index];
}
