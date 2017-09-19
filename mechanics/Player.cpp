#include "Player.h"
#include "Application.h"

CPlayer::CPlayer(int iEntityIndex, IClientEntity* pHandle)
{
	m_iEntityIndex = iEntityIndex;
	m_pHandle = pHandle;
	m_bPlayerInfoValid = false;

	m_llTimestamp = 0;
	m_bIsValid = true;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Reset()
{
	m_iEntityIndex = -1;
	m_pHandle = NULL;
	m_bPlayerInfoValid = false;
}

PlayerInfo* CPlayer::GetPlayerInfo()
{
	if (!m_bPlayerInfoValid)
	{
		CApplication::Instance()->EngineClient()->GetPlayerInfo(m_iEntityIndex, &m_playerInfo);
		m_bPlayerInfoValid = true;
	}

	return &m_playerInfo;
}
