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

void CMisc::NoFlash(int flashPercentage)
{
	if (!m_bIsEnabled)
		return;

	if (false) //todo: check if noflash
		return;

	//todo: check if noflash enabled
	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());

	*(float*)((DWORD)pLocalEntity + NOFLASH_OFFSET) = 255.0f - (255.0f * (1.0f - ((float)flashPercentage / 100.0f)));
}

void CMisc::NoRecoil(CUserCmd* pUserCmd)
{
	if (!m_bIsEnabled)
		return;

	if (false) //todo: check if norecoil
		return;

	if (m_pApp->m_bAimbotNoRecoil)
		return;

	if (!(pUserCmd->buttons & IN_ATTACK))
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	int shotsFired = *(int*)((DWORD)pLocalEntity + SHOTSFIRED_OFFSET);
	if (ENABLE_NOVISRECOIL)
	{
		if (shotsFired > 1)
		{
			QAngle aimPunch = *(QAngle*)((DWORD)pLocalEntity + (LOCAL_OFFSET + AIMPUNCHANGLE_OFFSET));
			pUserCmd->viewangles[0] -= aimPunch.x * RECOIL_COMPENSATION;
			pUserCmd->viewangles[1] -= aimPunch.y * RECOIL_COMPENSATION;
		}
	}
	else {
		if (shotsFired > 1) {
			m_pApp->EngineClient()->GetViewAngles(m_pApp->m_viewAngle);
			QAngle aimPunchAngle = *(QAngle*)((DWORD)pLocalEntity + (LOCAL_OFFSET + AIMPUNCHANGLE_OFFSET));

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

void CMisc::Update(void* pParameters)
{
	// Update code (once per in-game loop)
}