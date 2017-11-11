#include "Fakewalk.h"
#include "Application.h"

CFakewalk::CFakewalk()
{
}

CFakewalk::~CFakewalk()
{
}

void CFakewalk::Setup()
{
	m_pApp = CApplication::Instance();
}

// TODO not working
void CFakewalk::Update(void* pParameters)
{
	/*if (!m_bIsEnabled)
		return;*/

	CUserCmd* pUserCmd = (CUserCmd*)pParameters;
	if (!pUserCmd)
		return;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	static int iChoked = 0;
	bool bIsLbyUpdateTick = m_pApp->AntiAim()->NextLBYUpdate(m_pApp->Resolver()->GetResolverPlayer(pLocalEntity->EntIndex()), true, true);

	// Choke packets and setting real
	*m_pApp->m_bSendPackets = false;
	pUserCmd->viewangles[1] += 90.0f; // todo: real

	if (iChoked > 2 || iChoked == 0)
	{
		// Start movement suppression
		pUserCmd->forwardmove = pUserCmd->sidemove = 0.0f;

		// Check if standing
		if (pLocalEntity->GetVelocity()->Length2D() == 0.0f && iChoked > 1)
		{
			// Sending standing packet and fake
			*m_pApp->m_bSendPackets = true;
			pUserCmd->viewangles[1] += -90.0f; // todo: fake

			// -1 because of iChoked++ below
			iChoked = -1;
		}
	}

	if (bIsLbyUpdateTick && !m_pApp->m_bSendPackets)
	{
		// Sending standing packet and fake
		*m_pApp->m_bSendPackets = true;
		pUserCmd->viewangles[1] += -90.0f; // todo: fake

		// -1 because of iChoked++ below
		iChoked = -1;
	}

	iChoked++;
}