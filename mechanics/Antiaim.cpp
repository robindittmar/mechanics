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

	if(bIsLocalPlayer)
		pApp->m_bLbyUpdate = false;

	bool bPlayerMoving = pLocalEntity->GetVelocity()->Length2D() > 0.1f;
	if (bPlayerMoving)
		return false;

	float value = abs(pResolverPlayer->GetLbyUpdateTime() - pResolverPlayer->GetPredictedLbyUpdateTime() - GetOutgoingLatency());
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

	if (pActiveWeapon->IsKnife() && pUserCmd->buttons & IN_ATTACK2)
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

	// Applying Pitch Anti Aim
	ApplyPitchAntiAim(&angles);

	// Applying Yaw Anti Aims
	ApplyYawAntiAim(&angles);

	// LBY indicator check
	m_pApp->m_bLBY = !m_bIsMoving && pLocalResolverPlayer->GetRealLbyUpdateTime() + 1.1 < m_pApp->GlobalVars()->curtime;

	// Setting calculated angles to player angles
	pUserCmd->viewangles[0] = angles.x;
	pUserCmd->viewangles[1] = angles.y;
}

void CAntiAim::ApplyPitchAntiAim(QAngle* angles)
{
	float fRealPitchAngle = 0.0f + (m_bIsMoving ? m_fPitchOffsetMoving : m_fPitchOffsetStanding);

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
	if (IsFakeYaw() && m_bWasMoving && !m_bNextLbyUpdate)
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
	static bool bFakeAngleSwitch = true;
	float fFakeYaw = 0.0f + (m_bIsMoving ? m_fYawFakeOffsetMoving : m_fYawFakeOffsetStanding);

	switch ((m_bIsMoving ? m_iYawFakeSettingMoving : m_iYawFakeSettingStanding))
	{
	case FAKEYAWANTIAIM_STATIC:
		break;
	case FAKEYAWANTIAIM_NONE:
	default:
		fFakeYaw = 0.0f;
		break;
	}

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

	if (m_bWasMoving && m_bNextLbyUpdate && !m_bIsMoving)
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
	return m_bIsMoving ? m_iYawFakeSettingMoving == FAKEYAWANTIAIM_STATIC : m_iYawFakeSettingStanding == FAKEYAWANTIAIM_STATIC;
}