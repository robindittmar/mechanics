#include "Radar.h"
#include "Application.h"

CRadar::CRadar()
	: m_iType(RADAR_TYPE_INGAME)
{
}

CRadar::~CRadar()
{
}

void CRadar::Think(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	int iMaxPlayers = m_pApp->EngineClient()->GetMaxClients();
	for (int i = 0; i < iMaxPlayers; i++)
	{
		IClientEntity* pCurEntity = m_pApp->EntityList()->GetClientEntity(i);
		if (!pCurEntity)
			continue;

		if (pCurEntity->IsDormant())
			continue;

		if (!pCurEntity->IsAlive())
			continue;

		switch (m_iType)
		{
		case RADAR_TYPE_INGAME:
			pCurEntity->SetSpotted();
			break;
		case RADAR_TYPE_CUSTOM:
			break;
		}
	}
}
