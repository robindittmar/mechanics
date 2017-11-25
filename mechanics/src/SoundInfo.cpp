#include "SoundInfo.h"
#include "Application.h"

CSoundInfo::CSoundInfo(int iEntityIndex, Vector vOrigin, const char* pSample)
	: m_fTimestamp(CApplication::Instance()->GlobalVars()->curtime), m_iEntityIndex(iEntityIndex), m_vOrigin(vOrigin),
	m_pSample(nullptr)
{
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

float CSoundInfo::GetTimeSinceCreation(float fTimestamp)
{
	return (fTimestamp - m_fTimestamp);
}
