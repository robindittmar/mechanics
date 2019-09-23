#include "Fakewalk.h"
#include "Application.h"

CFakewalk::CFakewalk()
{
}

CFakewalk::~CFakewalk()
{
}

// TODO not working
void CFakewalk::Think(void* pParameters)
{
	/*if (!m_bIsEnabled)
		return;*/

	CUserCmd* pUserCmd = (CUserCmd*)pParameters;
	if (!pUserCmd)
		return;

	static int choked = 0;
	choked = choked > 7 ? 0 : choked + 1;
	pUserCmd->forwardmove = choked < 2 || choked > 5 ? 0 : pUserCmd->forwardmove;
	pUserCmd->sidemove = choked < 2 || choked > 5 ? 0 : pUserCmd->sidemove;
	*m_pApp->m_bSendPackets = choked < 1;
}