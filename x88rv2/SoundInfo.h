#ifndef __SOUNDINFO_H__
#define __SOUNDINFO_H__

#include <Windows.h>
#include "Vector.h"

class CSoundInfo
{
public:
	CSoundInfo(Vector vOrigin, const char* pSample);
	~CSoundInfo();

	Vector GetOrigin() { return m_vOrigin; }
	const char* GetSample() { return m_pSample; }

	bool IsOutdated(ULONGLONG timestamp);
private:
	ULONGLONG m_llTimestamp;
	Vector m_vOrigin;
	const char* m_pSample;
};

#endif // __SOUNDINFO_H__