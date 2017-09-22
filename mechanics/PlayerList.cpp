#include "PlayerList.h"
#include "Application.h"

CPlayerList::CPlayerList()
{
}

CPlayerList::~CPlayerList()
{
	this->ClearList();
}

void CPlayerList::Init(CApplication* pApp)
{
	m_pApp = pApp;

	m_pEngineClient = m_pApp->EngineClient();
	m_pEntityList = m_pApp->EntityList();
}

void CPlayerList::UpdateList()
{
	this->ClearList();

	int iLocalPlayerIdx = m_pEngineClient->GetLocalPlayer();
	IClientEntity* pCurEntity;
	
	int iPlayerCount = 0;
	CPlayer* pCurrent;

	ULONGLONG llTimestamp = GetTickCount64();
	
	int iMaxClients = m_pEngineClient->GetMaxClients();
	for (int i = 0; i < iMaxClients; i++)
	{
		pCurEntity = m_pEntityList->GetClientEntity(i);
		if (!pCurEntity)
			continue;

		if (pCurEntity->IsDormant())
		{
			if (!m_mapInvalidPlayers[i] && pCurEntity->IsAlive())
			{
				m_mapInvalidPlayers[i] = new CPlayer(i, pCurEntity);
				m_mapInvalidPlayers[i]->SetValid(false);
				m_mapInvalidPlayers[i]->SetTimestamp(llTimestamp);
			}

			continue;
		}
		else
		{
			if (m_mapInvalidPlayers[i])
			{
				delete m_mapInvalidPlayers[i];
				m_mapInvalidPlayers.erase(i);
			}
		}

		if (!pCurEntity->IsAlive())
			continue;

		// TODO: Once we're dead (check above)
		//		 our localplayer is not in the list anymore
		//       and will 100% cause a crash
		if (iLocalPlayerIdx == i)
			m_iLocalPlayerIdx = iPlayerCount;

		pCurrent = new CPlayer(i, pCurEntity);
		m_vPlayerList.push_back(pCurrent);
		iPlayerCount++;
	}

	CPlayer* pCur;
	for (std::unordered_map<int, CPlayer*>::iterator it = m_mapInvalidPlayers.begin(); it != m_mapInvalidPlayers.end();)
	{
		pCur = it->second;
		if (!pCur)
		{
			it++;
			continue;
		}

		if (pCur->GetTimestamp() + PLAYERLIST_TIME_REMOVE_INVALID_PLAYERS < llTimestamp)
		{
			delete pCur;
			it = m_mapInvalidPlayers.erase(it);
			continue;
		}

		//m_vPlayerList.push_back(pCur);
		it++;
	}
}

void CPlayerList::ClearList()
{
	CPlayer* pCur;
	for (std::vector<CPlayer*>::iterator it = m_vPlayerList.begin(); it != m_vPlayerList.end(); it++)
	{
		pCur = *it;

		if (pCur)
			delete pCur;
	}

	m_vPlayerList.clear();
}