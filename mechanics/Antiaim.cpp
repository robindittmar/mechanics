#include "Antiaim.h"
#include "Application.h"
#include "Console.h"

CAntiAim::CAntiAim()
{
	m_iPitchSettingStanding = PITCHANTIAIM_NONE;
	m_iYawSettingStanding = YAWANTIAIM_NONE;
}

CAntiAim::~CAntiAim()
{
}

void CAntiAim::DrawEdgeAntiAimPoints()
{
	if (!m_pApp->Visuals()->GetThirdperson())
		return;

	CTarget* pTarget = this->GetTarget(m_pApp->Ragebot()->GetTargetCriteria());
	if (!pTarget || (pTarget && !pTarget->GetValid()))
		return;

	Vector vScreenRight[EDGEANTIAIM_RANGE];
	Vector vScreenLeft[EDGEANTIAIM_RANGE];

	for (int i = 0; i < EDGEANTIAIM_RANGE; i++)
	{
		if (m_pApp->Gui()->WorldToScreen(EdgeAntiAimPointsRight[i].vPoint, vScreenRight[i]) && m_pApp->Gui()->WorldToScreen(EdgeAntiAimPointsLeft[i].vPoint, vScreenLeft[i]))
		{
			if (EdgeAntiAimPointsRight[i].bIsHidden)
			{
				m_pApp->Surface()->DrawSetColor(255, 0, 255, 0);
			}
			else
			{
				m_pApp->Surface()->DrawSetColor(255, 255, 0, 0);
			}
			m_pApp->Surface()->DrawOutlinedCircle(vScreenRight[i].x, vScreenRight[i].y, 5, 16);

			if (EdgeAntiAimPointsLeft[i].bIsHidden)
			{
				m_pApp->Surface()->DrawSetColor(255, 0, 255, 0);
			}
			else
			{
				m_pApp->Surface()->DrawSetColor(255, 255, 0, 0);
			}
			m_pApp->Surface()->DrawOutlinedCircle(vScreenLeft[i].x, vScreenLeft[i].y, 5, 16);
		}
	}
}

void CAntiAim::Setup()
{
	m_pApp = CApplication::Instance();
}


float GetOutgoingLatency()
{
	INetChannelInfo *nci = CApplication::Instance()->EngineClient()->GetNetChannelInfo();
	if (nci)
	{
		float OutgoingLatency = nci->GetLatency(FLOW_OUTGOING);
		return OutgoingLatency;
	}
	else
	{
		return 0.0f;
	}
}

bool CAntiAim::NextLBYUpdate(CResolverPlayer* pResolverPlayer, bool bIsLocalPlayer)
{
	CApplication* pApp = CApplication::Instance();
	IClientEntity* pLocalEntity = pApp->GetLocalPlayer();

	if (!pApp->AntiAim()->GetLbyBreaker())
		return false;

	if (bIsLocalPlayer)
		pApp->m_bLbyUpdate = false;

	bool bPlayerMoving = pLocalEntity->GetVelocity()->Length2D() > 0.1f;
	if (bPlayerMoving)
		return false;

	float value = abs(pResolverPlayer->GetLbyUpdateTime() - pResolverPlayer->GetPredLbyUpdateTime() - GetOutgoingLatency());
	if (value > 1.0f)
	{
		if (value > 1.1f)
		{
			pResolverPlayer->SetLbyUpdateTime(pResolverPlayer->GetLbyUpdateTime() + 1.125f);
			return !bIsLocalPlayer;
		}

		pApp->m_bLbyUpdate = true;
		return true;
	}

	return false;
}


void CAntiAim::Update(void* pParameters)
{
	m_fCurRealYaw = 0.0f;
	m_fCurFakeYaw = 0.0f;

	if (!m_bIsEnabled)
		return;

	CUserCmd* pUserCmd = (CUserCmd*)pParameters;

	if (!pUserCmd)
		return;

	// Allow us to use (open doors, defuse bomb) and shoot normally
	if (pUserCmd->buttons & IN_USE || pUserCmd->buttons & IN_ATTACK && !m_pApp->Ragebot()->DoingAutoRevolver())
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	CWeapon* pActiveWeapon = (CWeapon*)pLocalEntity->GetActiveWeapon();

	if (!pActiveWeapon)
		return;

	if (pActiveWeapon->IsKnife() && (!pActiveWeapon->IsSniper() && pUserCmd->buttons & IN_ATTACK2))
		return;

	if (pActiveWeapon->IsNade())
	{
		CGrenade* activeGrenade = (CGrenade*)pActiveWeapon;
		if (activeGrenade->GetThrowTime() > 0.f)
			return;
	}

	if (pLocalEntity->GetMoveType() & MOVETYPE_LADDER)
		return;

	m_bIsMoving = pLocalEntity->GetVelocity()->Length() > 0.1;
	if (m_bIsMoving)
		m_bWasMoving = true;

	QAngle angles;
	angles = m_pApp->GetClientViewAngles();

	CResolverPlayer* pLocalResolverPlayer = m_pApp->Resolver()->GetResolverPlayer(pLocalEntity->EntIndex());
	m_bNextLbyUpdate = NextLBYUpdate(pLocalResolverPlayer, true);

	// Applying Pitch Anti Aim and setting calculated angles
	ApplyPitchAntiAim(&angles);
	pUserCmd->viewangles[0] = angles.x;

	// LBY indicator check
	m_pApp->m_bLBY = !m_bIsMoving && pLocalResolverPlayer->GetLbyProxyUpdatedTime() + 1.1 < m_pApp->GlobalVars()->curtime;

	m_bIsEdgeAntiAim = EdgeAntiAim(pLocalEntity, pUserCmd);
	if (m_bIsEdgeAntiAim)
		return;

	// Applying Yaw Anti Aims
	ApplyYawAntiAim(&angles);

	// Setting calculated angles to player angles
	pUserCmd->viewangles[1] = angles.y;
}


void CAntiAim::HideHead(CUserCmd* pUserCmd, QAngle qAimAngle, int iHideDirection)
{
	if (iHideDirection == HIDEDIRECTION_INVALID)
		return;

	if (m_bEdgeAntiAimFakeSwitch)
	{
		// real
		if (iHideDirection == HIDEDIRECTION_LEFT)
		{
			pUserCmd->viewangles[1] = qAimAngle.y - 90.0f;
		}
		else
		{
			pUserCmd->viewangles[1] = qAimAngle.y + 90.0f;
		}
	}
	else
	{
		// fake
		if (iHideDirection == HIDEDIRECTION_LEFT)
		{
			pUserCmd->viewangles[1] = qAimAngle.y + 90.0f;
		}
		else
		{
			pUserCmd->viewangles[1] = qAimAngle.y - 90.0f;
		}
	}
}

int CAntiAim::CheckHeadPoint(Vector vEnemyHeadPos, EdgeAntiAimPoint* vRight, EdgeAntiAimPoint* vLeft)
{
	//todo: use autowall instead of traceray

	Ray_t rayRight, rayLeft;
	trace_t traceRight, traceLeft;
	CTraceFilterWorldAndPropsOnly filter;

	rayRight.Init(vEnemyHeadPos, vRight->vPoint);
	m_pApp->EngineTrace()->TraceRay(rayRight, (MASK_SHOT_HULL || CONTENTS_HITBOX), &filter, &traceRight);
	rayLeft.Init(vEnemyHeadPos, vLeft->vPoint);
	m_pApp->EngineTrace()->TraceRay(rayLeft, (MASK_SHOT_HULL || CONTENTS_HITBOX), &filter, &traceLeft);

	if (traceRight.fraction < 1.0f)
	{
		vRight->bIsHidden = true;
	}
	else
	{
		vRight->bIsHidden = false;
	}

	if (traceLeft.fraction < 1.0f)
	{
		vLeft->bIsHidden = true;
	}
	else
	{
		vLeft->bIsHidden = false;
	}

	if (traceRight.fraction == 1.0f && traceLeft.fraction == 1.0)
	{
		return HIDEDIRECTION_INVALID;
	}
	else if (traceRight.fraction < 1.0f && traceLeft.fraction == 1.0f)
	{
		return HIDEDIRECTION_RIGHT;
	}
	else if (traceRight.fraction == 1.0f && traceLeft.fraction < 1.0f)
	{
		return HIDEDIRECTION_LEFT;
	}
	else
	{
		return HIDEDIRECTION_NEXT;
	}
}

bool CAntiAim::EdgeAntiAim(IClientEntity* pLocalEntity, CUserCmd* pUserCmd)
{
	if (!m_bDoEdgeAntiAim)
		return false;

	bool bDoesEdgeAntiAim = false;
	Vector vLocalHeadpos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();

	this->GetAntiAimTargets();
	CTarget* pTarget = this->GetTarget(m_pApp->Ragebot()->GetTargetCriteria());
	if (!pTarget || (pTarget && !pTarget->GetValid()))
		return false;

	IClientEntity* pEnemyEntity = pTarget->GetEntity();
	if (!pEnemyEntity)
		return false;

	CWeapon* pEnemyActiveWeapon = pEnemyEntity->GetActiveWeapon();
	if (!pEnemyActiveWeapon)
		return false;

	QAngle qAimAngle = *pTarget->GetAimAngles();

	Vector right;
	AngleVectors(qAimAngle, NULL, &right);

	// Initialize checkpoints
	for (int i = 0; i < EDGEANTIAIM_RANGE; i++)
	{
		EdgeAntiAimPointsRight[i].vPoint = vLocalHeadpos + (right * (15 + (i * 10)));
		EdgeAntiAimPointsLeft[i].vPoint = vLocalHeadpos - (right * (15 + (i * 10)));
	}

	Vector vEnemyHeadPos = *pEnemyEntity->GetOrigin() + *pEnemyEntity->GetEyeOffset();

	bool bWasInvalid = false;
	// Check if edge antiaim
	for (int i = 0; i < EDGEANTIAIM_RANGE; i++)
	{
		int iHideDirection = CheckHeadPoint(vEnemyHeadPos, &EdgeAntiAimPointsRight[i], &EdgeAntiAimPointsLeft[i]);
		if (iHideDirection == HIDEDIRECTION_INVALID)
			bWasInvalid = true;

		if (iHideDirection != HIDEDIRECTION_NEXT && iHideDirection != HIDEDIRECTION_INVALID)
		{
			if (!bDoesEdgeAntiAim && !bWasInvalid)
			{
				HideHead(pUserCmd, qAimAngle, iHideDirection);
				bDoesEdgeAntiAim = true;
			}
		}
	}

	// No edge antiaim so no need to break/fake
	if (!bDoesEdgeAntiAim)
		return false;

	Vector vTempYaw = Vector(0.0f, pUserCmd->viewangles[1] + 180.0f, 0.0f);
	vTempYaw.NormalizeAngles();
	if (m_bEdgeAntiAimFakeSwitch)
	{
		m_fCurRealYaw = vTempYaw.y;
		m_fCurFakeYaw = pUserCmd->viewangles[1];
	}
	else
	{
		m_fCurRealYaw = pUserCmd->viewangles[1];
		m_fCurFakeYaw = vTempYaw.y;
	}

	*m_pApp->m_bSendPackets = m_bEdgeAntiAimFakeSwitch;
	m_bEdgeAntiAimFakeSwitch = !m_bEdgeAntiAimFakeSwitch;

	if (!m_bIsMoving && m_bLbyBreaker && m_bNextLbyUpdate && !*m_pApp->m_bSendPackets)
	{
		pUserCmd->viewangles[1] = vTempYaw.y;
	}

	return bDoesEdgeAntiAim;
}


void CAntiAim::GetAntiAimTargets()
{
	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	int iLocalTeamNum = pLocalEntity->GetTeamNum();
	Vector vMyHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();

	// Angle stuff
	QAngle qLocalViewAngles = m_pApp->GetClientViewAngles();
	QAngle qAimAngles;

	// Position stuff
	Vector vEnemyPos;

	float fViewangleDist;
	float fOriginDist;
	float fLowestViewangleDist = 999999.0f;
	float fLowestOriginDist = 999999.0f;

	int iTargetCount = 0;
	for (int i = 0; i < m_pApp->EngineClient()->GetMaxClients(); i++)
	{
		IClientEntity* pCurEntity = m_pApp->EntityList()->GetClientEntity(i);

		// Filter entites
		if (!pCurEntity)
			continue;

		// Skip dormant entities
		if (pCurEntity->IsDormant())
			continue;

		// If the possible target isn't alive
		if (!pCurEntity->IsAlive())
			continue;

		// Only from enemy team & we don't want spectators or something
		int entityTeam = pCurEntity->GetTeamNum();
		if (entityTeam == iLocalTeamNum || entityTeam != TEAMNUM_T && entityTeam != TEAMNUM_CT)
			continue;

		iTargetCount++;

		vEnemyPos = *pCurEntity->GetOrigin() + *pCurEntity->GetEyeOffset();

		// Calculate a values
		qAimAngles = Utils::CalcAngle(vMyHeadPos, vEnemyPos);
		fOriginDist = this->GetOriginDist(vMyHeadPos, vEnemyPos);
		fViewangleDist = fabs(this->GetViewangleDist(qLocalViewAngles, qAimAngles));

		// Unspecified
		if (!m_pTargets[TARGETCRITERIA_UNSPECIFIED].GetValid())
		{
			m_pTargets[TARGETCRITERIA_UNSPECIFIED].SetTarget(
				vEnemyPos,
				qAimAngles,
				pCurEntity,
				fViewangleDist,
				fOriginDist,
				-1
			);
		}

		// Origin
		if (fOriginDist < fLowestOriginDist)
		{
			fLowestOriginDist = fOriginDist;
			m_pTargets[TARGETCRITERIA_ORIGIN].SetTarget(
				vEnemyPos,
				qAimAngles,
				pCurEntity,
				fViewangleDist,
				fOriginDist,
				-1
			);
		}

		// Viewangles
		if (fViewangleDist < fLowestViewangleDist)
		{
			fLowestViewangleDist = fViewangleDist;
			m_pTargets[TARGETCRITERIA_VIEWANGLES].SetTarget(
				vEnemyPos,
				qAimAngles,
				pCurEntity,
				fViewangleDist,
				fOriginDist,
				-1
			);
		}
	}

	if (iTargetCount == 0)
	{
		m_pTargets[TARGETCRITERIA_UNSPECIFIED].SetValid(false);
		m_pTargets[TARGETCRITERIA_ORIGIN].SetValid(false);
		m_pTargets[TARGETCRITERIA_VIEWANGLES].SetValid(false);
	}
}

float CAntiAim::GetOriginDist(Vector& vSource, Vector& vTarget)
{
	return (vTarget - vSource).Length();
}

float CAntiAim::GetViewangleDist(QAngle& qSource, QAngle& qTarget)
{
	QAngle qDist = qTarget - qSource;
	qDist.NormalizeAngles();
	float fAng = qDist.Length();

	return fAng;
}


void CAntiAim::ApplyPitchAntiAim(QAngle* angles)
{
	float fRealPitchAngle = angles->x;

	switch ((m_bIsMoving ? m_iPitchSettingMoving : m_iPitchSettingStanding))
	{
	case PITCHANTIAIM_UP:
		fRealPitchAngle = -89.0f;
		break;
	case PITCHANTIAIM_DOWN:
		fRealPitchAngle = 89.0f;
		break;
	case PITCHANTIAIM_NONE:
	default:
		break;
	}

	angles->x = fRealPitchAngle;
}

void CAntiAim::ApplyYawAntiAim(QAngle* angles)
{
	static float trigger = 0.0f;

	bool bSetMovingSettings = false;
	if (m_bWasMoving && !m_bNextLbyUpdate)
	{
		bSetMovingSettings = true;
	}

	float fRealYawAngle = 0.0f + (bSetMovingSettings ? m_fYawOffsetMoving : m_fYawOffsetStanding);

	// Getting anti aim angles
	switch ((bSetMovingSettings ? m_iYawSettingMoving : m_iYawSettingStanding))
	{
	case YAWANTIAIM_STATIC:
		break;
	case YAWANTIAIM_BACKWARDS:
		fRealYawAngle += 180.0f;
		break;
	case YAWANTIAIM_STATICJITTERBACKWARDS:
		trigger += 15.0f;
		fRealYawAngle -= trigger > 50.0f ? -145.0f : 145.0f;

		if (trigger > 100.0f)
		{
			trigger = 0.0f;
		}
		break;
	case YAWANTIAIM_NONE:
	default:
		fRealYawAngle = 0.0f;
		break;
	}

	// Applying Fake Yaw
	ApplyYawFakeAntiAim(angles, fRealYawAngle);
}

void CAntiAim::ApplyYawFakeAntiAim(QAngle* angles, float fRealYaw)
{
	static float trigger = 0.0f;
	static bool bFakeAngleSwitch = true;
	float fFakeYaw = 0.0f + (m_bIsMoving ? m_fYawFakeOffsetMoving : m_fYawFakeOffsetStanding);

	switch ((m_bIsMoving ? m_iYawFakeSettingMoving : m_iYawFakeSettingStanding))
	{
	case FAKEYAWANTIAIM_STATICJITTER:
		trigger += 15.0f;
		fFakeYaw -= trigger > 80.0f ? -35.0f : 35.0f;

		if (trigger > 160.0f)
		{
			trigger = 0.0f;
		}
		break;
	case FAKEYAWANTIAIM_STATIC:
		break;
	case FAKEYAWANTIAIM_NONE:
	default:
		fFakeYaw = 0.0f;
		break;
	}

	m_fCurRealYaw = angles->y + fRealYaw;
	m_fCurFakeYaw = angles->y + fFakeYaw;

	if (IsFakeYaw())
	{
		if (m_pApp->Misc()->GetFakelag() && m_pApp->Misc()->GetFakelagChokedAmount() + 1 <= m_pApp->Misc()->GetFakelagChokeAmount() && m_bIsMoving ||
			!bFakeAngleSwitch)
		{
			angles->y += fRealYaw;
			bFakeAngleSwitch = false;
		}
		else
		{
			angles->y += fFakeYaw;
		}
		*m_pApp->m_bSendPackets = bFakeAngleSwitch;
		bFakeAngleSwitch = !bFakeAngleSwitch;
	}
	else
	{
		angles->y += fRealYaw;
	}

	if (!m_bIsMoving && m_bLbyBreaker && IsFakeYaw() && m_bNextLbyUpdate && !*m_pApp->m_bSendPackets) //todo check if !bSendPackets is needed
	{
		angles->y += -fRealYaw + fFakeYaw;
	}

	if (m_bWasMoving && (!m_bLbyBreaker || m_bNextLbyUpdate) && !m_bIsMoving)
		m_bWasMoving = false;
}

void CAntiAim::DrawLBYIndicator()
{
	if (!m_bIsEnabled)
		return;

	if (!m_bDrawLbyIndicator)
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());

	// Draw "LBY"
	m_pApp->Surface()->DrawSetTextFont(g_pResourceManager->GetFont(RM_FONT_HEADER));
	if (m_pApp->m_bLBY)
		m_pApp->Surface()->DrawSetTextColor(255, 0, 255, 0);
	else
		m_pApp->Surface()->DrawSetTextColor(255, 255, 0, 0);

	int w, h;
	m_pApp->EngineClient()->GetScreenSize(w, h);
	m_pApp->Surface()->DrawSetTextPos(25, h - 55);
	m_pApp->Surface()->DrawPrintText(L"LBY", 4);


	// Draw LBY and Y Angle
	m_pApp->Surface()->DrawSetTextFont(g_pResourceManager->GetFont(RM_FONT_NORMAL));
	m_pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);

	char pBuffer[16];
	snprintf(pBuffer, 16, "%i", (int)pLocalEntity->GetLowerBodyYaw());
	wchar_t pBuffW[16];
	mbstowcs(pBuffW, pBuffer, 16);
	int len = lstrlenW(pBuffW);

	m_pApp->Surface()->DrawSetTextPos(50, h - 58);
	m_pApp->Surface()->DrawPrintText(pBuffW, len);

	char pBuffer1[16];

	snprintf(pBuffer1, 16, "%i", (int)m_pApp->LastTickAngles().y);
	wchar_t pBuffW1[16];
	mbstowcs(pBuffW1, pBuffer1, 16);
	int len1 = lstrlenW(pBuffW1);

	m_pApp->Surface()->DrawSetTextPos(50, h - 47);
	m_pApp->Surface()->DrawPrintText(pBuffW1, len1);
}

bool CAntiAim::IsFakeYaw()
{
	return (m_bIsMoving ? m_iYawFakeSettingMoving != FAKEYAWANTIAIM_NONE : m_iYawFakeSettingStanding != FAKEYAWANTIAIM_NONE) || (m_bIsEdgeAntiAim && !m_bEdgeAntiAimFakeSwitch) && m_bIsEnabled;
}