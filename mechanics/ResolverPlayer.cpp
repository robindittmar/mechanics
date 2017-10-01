#include "ResolverPlayer.h"

CResolverPlayer::CResolverPlayer()
{
	m_iShotsFired = 0;
	m_fLastLby = 0.0f;
	m_fLbyUpdateTime = 0.0f;
	m_fRealLbyUpdateTime = 0.0f;
	m_fPredictedLbyUpdateTime = 0.0f;
}

CResolverPlayer::~CResolverPlayer()
{
}
