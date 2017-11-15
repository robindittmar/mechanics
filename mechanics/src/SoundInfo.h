#ifndef __SOUNDINFO_H__
#define __SOUNDINFO_H__

#include <Windows.h>
#include "Vector.h"

class CSoundInfo
{
public:
	CSoundInfo(int iEntityIndex, Vector vOrigin, const char* pSample);
	~CSoundInfo();

	int GetEntityIndex() { return m_iEntityIndex; }
	Vector GetOrigin() { return m_vOrigin; }

	void SetSample(const char* p);
	const char* GetSample() { return (const char*)m_pSample; }

	ULONGLONG GetTimeSinceCreation(ULONGLONG timestamp);
private:
	ULONGLONG m_llTimestamp;

	int m_iEntityIndex;
	Vector m_vOrigin;
	char* m_pSample;
};

#endif // __SOUNDINFO_H__