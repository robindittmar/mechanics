#ifndef __ESPPLAYER_H__
#define __ESPPLAYER_H__

#include <Windows.h>
#include "Vector.h"

class CEspPlayer
{
public:
	CEspPlayer();
	~CEspPlayer();

	void SetTimestamp(ULONGLONG llTimestamp) { m_llTimestamp = llTimestamp; }
	ULONGLONG GetTimestamp() { return m_llTimestamp; }

	void SetIsDormant(bool bIsDormant) { m_bIsDormant = bIsDormant; }
	bool GetIsDormant() { return m_bIsDormant; }
private:
	ULONGLONG m_llTimestamp;
	bool m_bIsDormant;
};

#endif // __ESPPLAYER_H__