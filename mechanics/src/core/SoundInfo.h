#ifndef __SOUNDINFO_H__
#define __SOUNDINFO_H__

#include "../source_sdk/Vector.h"

class CSoundInfo
{
public:
	CSoundInfo(int iEntityIndex, Vector vOrigin, const char* pSample);
	~CSoundInfo();

	int GetEntityIndex() { return m_iEntityIndex; }
	Vector GetOrigin() { return m_vOrigin; }

	void SetSample(const char* p);
	const char* GetSample() { return (const char*)m_pSample; }

	float GetTimeSinceCreation(float fTimestamp);
private:
	float m_fTimestamp;

	int m_iEntityIndex;
	Vector m_vOrigin;
	char* m_pSample;
};

#endif // __SOUNDINFO_H__