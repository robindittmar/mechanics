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

	void SetOrigin(Vector vOrigin) { m_vOrigin = vOrigin; }
	Vector GetOrigin() { return m_vOrigin; }
	
	void SetHeadOrigin(Vector vHeadOrigin) { m_vHeadOrigin = vHeadOrigin; }
	Vector GetHeadOrigin() { return m_vHeadOrigin; }
private:
	ULONGLONG m_llTimestamp;
	bool m_bIsDormant;
	
	Vector m_vOrigin;
	Vector m_vHeadOrigin;
};

#endif // __ESPPLAYER_H__