#ifndef __PLAYERLIST_H__
#define __PLAYERLIST_H__

// Std lib
#include <vector>
#include <unordered_map>

// Source SDK
#include "IVEngineClient.h"
#include "ClientEntityList.h"

// Custom
#include "Player.h"

#define PLAYERLIST_TIME_REMOVE_INVALID_PLAYERS	5000

class CApplication;

class CPlayerList
{
public:
	CPlayerList();
	~CPlayerList();

	void Init(CApplication* pApp);
	void UpdateList();

	int GetPlayerCount() { return m_vPlayerList.size(); }
	CPlayer* GetPlayerByIndex(int i) { return m_vPlayerList[i]; }

	CPlayer* GetLocalPlayer() { return m_vPlayerList[m_iLocalPlayerIdx]; }
private:
	void ClearList();

	int m_iLocalPlayerIdx;
	std::vector<CPlayer*> m_vPlayerList;
	std::unordered_map<int, CPlayer*> m_mapInvalidPlayers;

	CApplication* m_pApp;
	IVEngineClient* m_pEngineClient;
	IClientEntityList* m_pEntityList;
};

#endif // __PLAYERLIST_H__