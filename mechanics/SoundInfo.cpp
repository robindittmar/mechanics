#include "SoundInfo.h"

CSoundInfo::CSoundInfo(Vector vOrigin, const char* pSample)
{
	m_llTimestamp = GetTickCount64();
	m_vOrigin = vOrigin;
	m_pSample = pSample;
}

CSoundInfo::~CSoundInfo()
{
}

bool CSoundInfo::IsOutdated(ULONGLONG timestamp)
{
	return (timestamp > m_llTimestamp);
}
