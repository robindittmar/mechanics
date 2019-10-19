#include "Fakeduck.h"
#include "../Application.h"

CFakeduck::CFakeduck()
{
}

CFakeduck::~CFakeduck()
{
}

void CFakeduck::Think(void* pParameters)
{
    /*if (!m_bIsEnabled)
        return;*/

    CUserCmd* pUserCmd = (CUserCmd*)pParameters;
    if (!pUserCmd)
        return;

    INetChannel* nc = m_pApp->EngineClient()->GetNetChannel();
    if (!nc)
        return;

    pUserCmd->buttons |= IN_BULLRUSH;
    if (m_pApp->InputSystem()->IsButtonDown(KEY_C))
    {
        // TODO: change key + change how to get maxchokeamount
        nc->choked_packets >= m_pApp->Fakelag()->GetChokeAmount() / 2 ? pUserCmd->buttons |= IN_DUCK : pUserCmd->buttons &= ~IN_DUCK;
    }
}