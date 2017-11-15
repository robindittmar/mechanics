#include "SoundInfo.h"

CSoundInfo::CSoundInfo(int iEntityIndex, Vector vOrigin, const char* pSample)
{
	m_llTimestamp = GetTickCount64();

	m_iEntityIndex = iEntityIndex;
	m_vOrigin = vOrigin;

	m_pSample = NULL;
	this->SetSample(pSample);
}

CSoundInfo::~CSoundInfo()
{
	if (m_pSample)
		delete[] m_pSample;
}

void CSoundInfo::SetSample(const char* p)
{
	if (m_pSample)
		delete[] m_pSample;

	int iLen = strlen(p) + 1;
	m_pSample = new char[iLen];
	memcpy(m_pSample, p, iLen);
}

ULONGLONG CSoundInfo::GetTimeSinceCreation(ULONGLONG timestamp)
{
	return (timestamp - m_llTimestamp);
}
