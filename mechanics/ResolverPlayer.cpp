#include "ResolverPlayer.h"

CResolverPlayer::CResolverPlayer() :
	m_fBalanceAdjustStart(-1)
{
	m_iShotsFired = 0;
	m_iShotHit = -1;
	m_fLbyProxyLastValue = 0.0f;
	m_fLbyUpdateTime = 0.0f;
	m_fLbyProxyUpdatedTime = 0.0f;
	m_fPredLbyUpdateTime = 0.0f;
}

CResolverPlayer::~CResolverPlayer()
{
}
