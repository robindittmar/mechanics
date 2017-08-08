#include "Misc.h"
#include "Application.h"

CMisc::CMisc()
{
}

CMisc::~CMisc()
{
}

void CMisc::Setup()
{
	m_pApp = CApplication::Instance();
}

void CMisc::Update(void* pParameters)
{
}

void CMisc::NoRecoil(CUserCmd* pUserCmd)
{
	if (!m_bIsEnabled ||
		!m_bNoRecoil ||
		m_pApp->m_bAimbotNoRecoil ||
		!(pUserCmd->buttons & IN_ATTACK))
	{
		m_pApp->m_oldAimPunchAngle.x = 0;
		m_pApp->m_oldAimPunchAngle.y = 0;
		return;
	}

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	CWeapon* activeWeapon = (CWeapon*)pLocalEntity->ActiveWeapon();
	if (activeWeapon->IsPistol() && !m_pApp->Visuals()->IsNoVisualRecoil()) //todo: maybe norecoil while pistol
		return;

	int shotsFired = pLocalEntity->ShotsFired();
	if (m_pApp->Visuals()->IsNoVisualRecoil())
	{
		QAngle aimPunch = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));
		pUserCmd->viewangles[0] -= aimPunch.x * RECOIL_COMPENSATION;
		pUserCmd->viewangles[1] -= aimPunch.y * RECOIL_COMPENSATION;
	}
	else {
		m_pApp->EngineClient()->GetViewAngles(m_pApp->m_viewAngle);
		QAngle aimPunchAngle = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));

		m_pApp->m_viewAngle.x += (m_pApp->m_oldAimPunchAngle.x - aimPunchAngle.x * RECOIL_COMPENSATION);
		m_pApp->m_viewAngle.y += (m_pApp->m_oldAimPunchAngle.y - aimPunchAngle.y * RECOIL_COMPENSATION);

		m_pApp->ClientViewAngles(m_pApp->m_viewAngle);

		m_pApp->m_oldAimPunchAngle.x = aimPunchAngle.x * RECOIL_COMPENSATION;
		m_pApp->m_oldAimPunchAngle.y = aimPunchAngle.y * RECOIL_COMPENSATION;
	}

	if (!m_pApp->Misc()->IsAutoPistol() && activeWeapon->IsPistol()) //todo: maybe norecoil while pistol
		return;

	float nextattack = activeWeapon->NextPrimaryAttack();
	float servertime = pLocalEntity->TickBase() * m_pApp->GlobalVars()->interval_per_tick;
	if (nextattack > servertime)
	{
		pUserCmd->buttons &= ~IN_ATTACK;
		return;
	}
}

//todo: check if works properly and not to many packets choked
void CMisc::Fakelag(CUserCmd* pUserCmd)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bFakelag)
		return;

	if (!m_pApp->m_bGotSendPackets)
		return;

	if (pUserCmd->buttons == IN_ATTACK ||
		pUserCmd->buttons == IN_ATTACK2)
	{
		*m_pApp->m_bSendPackets = true;
		return;
	}

	static int chokedPackets = 0;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	if (chokedPackets >= MAXPACKETSCHOKED) //todo: MAXPACKETCHOKED -/+ FakelagValue!
	{
		*m_pApp->m_bSendPackets = true;
		chokedPackets = 0;
	}
	else
	{
		chokedPackets++;
		*m_pApp->m_bSendPackets = false;
	}
}

void CMisc::AutoStrafe(CUserCmd* pUserCmd)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bAutoStrafe)
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	DWORD moveType = pLocalEntity->MoveType();
	if (!(pLocalEntity->Flags() & FL_ONGROUND) &&
		!(moveType & MOVETYPE_NOCLIP) &&
		!(moveType & MOVETYPE_LADDER))
	{
		if (pUserCmd->mousedx > 0)
		{
			pUserCmd->sidemove = 450;
		}
		if (pUserCmd->mousedx < 0)
		{
			pUserCmd->sidemove = -450;
		}
	}
}

void CMisc::DrawNoScope()
{
	if (!m_bIsEnabled)
		return;

	if (!m_bNoScope)
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	if (!pLocalEntity->IsScoped())
		return;

	int width, height;
	m_pApp->EngineClient()->GetScreenSize(width, height);
	m_pApp->Surface()->DrawSetColor(255, 0, 0, 0);
	m_pApp->Surface()->DrawLine(width / 2, 0, width / 2, height);
	m_pApp->Surface()->DrawLine(0, height / 2, width, height / 2);
}

bool CMisc::NoScope(unsigned int vguiPanel)
{
	if (!m_bIsEnabled)
		return false;

	if (!m_bNoScope)
		return false;

	static CXorString hudZoom("_~á˜xdè");
	if (!strcmp(hudZoom.ToCharArray(), m_pApp->Panel()->GetName(vguiPanel)))
		return true;
	return false;
}