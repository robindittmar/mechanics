#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "ClientEntity.h"
#include "PlayerInfo.h"
#include "Vector.h"

typedef unsigned long long ULONGLONG;

class CPlayer
{
public:
	CPlayer(int iEntityIndex = -1, IClientEntity* pHandle = NULL);
	~CPlayer();

	void Reset();

	void SetValid(bool bIsValid) { m_bIsValid = bIsValid; }
	bool GetValid() { return m_bIsValid; }

	void SetTimestamp(ULONGLONG llTimestamp) { m_llTimestamp = llTimestamp; }
	ULONGLONG GetTimestamp() { return m_llTimestamp; }

	void SetEntityIndex(int i) { m_iEntityIndex = i; }
	int GetEntityIndex() { return m_iEntityIndex; }

	void SetHandle(IClientEntity* h) { m_pHandle = h; }
	IClientEntity* GetHandle() { return m_pHandle; }

	PlayerInfo* GetPlayerInfo();
private:
	bool m_bIsValid;
	ULONGLONG m_llTimestamp;

	int m_iEntityIndex;
	IClientEntity* m_pHandle;
	
	bool m_bPlayerInfoValid;
	PlayerInfo m_playerInfo;
};

#endif // __PLAYER_H__