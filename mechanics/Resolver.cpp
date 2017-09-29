#include "Resolver.h"
#include "Application.h"

CResolver::CResolver()
{
}

CResolver::~CResolver()
{
}

void CResolver::Setup()
{
	m_pApp = CApplication::Instance();
}

void CResolver::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	int iShotsCount = pLocalEntity->GetShotsFired();
	for (int i = 0; i < m_pApp->EngineClient()->GetMaxClients(); i++)
	{
		IClientEntity* pCurEntity = m_pApp->EntityList()->GetClientEntity(i);

		if (!pCurEntity)
			continue;

		if (pCurEntity == pLocalEntity)
			return;

		float fCurLby = pCurEntity->GetLowerBodyYaw();
		QAngle* qCurEyeAngles = pCurEntity->GetAngEyeAngles();
		bool bIsMoving = pCurEntity->GetVelocity()->Length() > 0.1f;

		switch (m_iResolverType) //todo: check if cur player got other resolver option
		{
		case RESOLVERTYPE_AUTOMATIC:
			// while moving LBY, while standing check if lby updated/bruteforce
			break;
		case RESOLVERTYPE_NOSPREAD:
			// change every shot | playerlist and save how many shots u shot on that target!
			switch (iShotsCount % 9)
			{
			case 8:
				qCurEyeAngles->y -= 90.0f;
				break;
			case 7:
				qCurEyeAngles->y -= 67.5f;
				break;
			case 6:
				qCurEyeAngles->y -= 45.0f;
				break;
			case 5:
				qCurEyeAngles->y -= 22.5f;
				break;
			case 4:
				qCurEyeAngles->y += 22.5f;
				break;
			case 3:
				qCurEyeAngles->y += 45.0f;
				break;
			case 2:
				qCurEyeAngles->y += 67.5f;
				break;
			case 1:
				qCurEyeAngles->y += 90.0f;
				break;
			case 0:
				break;
			}
			break;
		case RESOLVERTYPE_BRUTEFORCE:
			// start after second shot, save when hit
			break;
		case RESOLVERTYPE_LBY:
			if (qCurEyeAngles->y != fCurLby)
				qCurEyeAngles->y = fCurLby;
			break;
		case RESOLVERTYPE_NONE:
		default:
			break;
		}
	}
}