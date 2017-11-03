#include "Fakelag.h"
#include "Application.h"

CFakelag::CFakelag()
	: m_bIsChoking(false), m_iPacketsChoked(0), m_iChokeAmount(0), m_bOnlyInAir(false), m_iLagType(FAKELAG_TYPE_FACTOR)
{
}

CFakelag::~CFakelag()
{
}

int CFakelag::GetChokeAmount()
{
	if (m_iLagType == FAKELAG_TYPE_ADAPTIVE)
		return m_iAdaptiveChokeAmount;

	return m_iChokeAmount;
}

void CFakelag::CalcAdaptiveChokeAmount()
{
	if (m_iLagType != FAKELAG_TYPE_ADAPTIVE)
		return;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	float fIntervalPerTickSqr;
	float fVelocitySqr;
	int iUnclampedTicks;

	fIntervalPerTickSqr = powf(m_pApp->GlobalVars()->interval_per_tick, 2);
	fVelocitySqr = pLocalEntity->GetVelocity()->Length2DSqr();

	iUnclampedTicks = min(m_iChokeAmount, (int)(ceilf(sqrt(4096.0f / (fVelocitySqr * fIntervalPerTickSqr)))));
	m_iAdaptiveChokeAmount = (max(0, min(iUnclampedTicks, m_iChokeAmount - 1)) ? iUnclampedTicks : m_iChokeAmount - 1);
}

void CFakelag::Setup()
{
	m_pApp = CApplication::Instance();
}

void CFakelag::Update(void* pParameters)
{
	assert(pParameters != NULL);

	if (!m_bIsEnabled)
	{
		m_bIsChoking = false;
		return;
	}

	CUserCmd* pUserCmd = (CUserCmd*)pParameters;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	if (pLocalEntity->GetVelocity()->Length() <= 0.1f)
	{
		m_bIsChoking = false;
		m_iPacketsChoked = 0;
		return;
	}

	if (pUserCmd->buttons == IN_ATTACK ||
		pUserCmd->buttons == IN_ATTACK2 ||
		pUserCmd->buttons == IN_USE)
	{
		m_bIsChoking = false;
		*m_pApp->m_bSendPackets = true;
		return;
	}

	if (m_bOnlyInAir && ((pLocalEntity->GetFlags() & FL_ONGROUND) || (pLocalEntity->GetFlags() & FL_INWATER)))
	{
		m_bIsChoking = false;
		return;
	}

	switch (m_iLagType)
	{
	case FAKELAG_TYPE_FACTOR:
		if (m_iPacketsChoked >= m_iChokeAmount)
		{
			m_iPacketsChoked = 0;
			m_bIsChoking = false;
		}
		else
		{
			m_iPacketsChoked++;
			m_bIsChoking = true;
		}
		break;
	case FAKELAG_TYPE_ADAPTIVE:
		if (m_iPacketsChoked < m_iAdaptiveChokeAmount)
		{
			m_iPacketsChoked++;
			m_bIsChoking = true;
		}
		else
		{
			m_iPacketsChoked = 0;
			m_bIsChoking = false;
		}
		break;
	default:
		break;
	}

	*m_pApp->m_bSendPackets = !m_bIsChoking;
}
