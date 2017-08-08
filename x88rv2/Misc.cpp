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
	if (!m_bIsEnabled)
		return;

	if (!m_bNoRecoil)
		return;

	if (m_pApp->Aimbot()->DidNoRecoil())
		return;

	if (!(pUserCmd->buttons & IN_ATTACK))
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	int shotsFired = *(int*)((DWORD)pLocalEntity + OFFSET_SHOTSFIRED);
	if (m_pApp->Visuals()->IsNoVisualRecoil())
	{
		if (shotsFired > 1)
		{
			QAngle aimPunch = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));
			pUserCmd->viewangles[0] -= aimPunch.x * RECOIL_COMPENSATION;
			pUserCmd->viewangles[1] -= aimPunch.y * RECOIL_COMPENSATION;
		}
	}
	else {
		if (shotsFired > 1) {
			m_pApp->EngineClient()->GetViewAngles(m_pApp->m_viewAngle);
			QAngle aimPunchAngle = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));

			m_pApp->m_viewAngle.x += (m_pApp->m_oldAimPunchAngle.x - aimPunchAngle.x * RECOIL_COMPENSATION);
			m_pApp->m_viewAngle.y += (m_pApp->m_oldAimPunchAngle.y - aimPunchAngle.y * RECOIL_COMPENSATION);

			m_pApp->EngineClient()->SetViewAngles(m_pApp->m_viewAngle);

			m_pApp->m_oldAimPunchAngle.x = aimPunchAngle.x * RECOIL_COMPENSATION;
			m_pApp->m_oldAimPunchAngle.y = aimPunchAngle.y * RECOIL_COMPENSATION;
		}
		else {
			m_pApp->m_oldAimPunchAngle.x = 0;
			m_pApp->m_oldAimPunchAngle.y = 0;
		}
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
	if (chokedPackets >= MAXPACKETSCHOKED || pLocalEntity->GetVelocity() <= 0.f) //todo: MAXPACKETCHOKED -/+ FakelagValue!
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
	if (!(pLocalEntity->Flags() & FL_ONGROUND))
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