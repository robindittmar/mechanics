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

	for (int i = 1; i < m_pApp->EngineClient()->GetMaxClients(); i++)
	{
		IClientEntity* pCurEntity = m_pApp->EntityList()->GetClientEntity(i);
		CResolverPlayer* pCurResolverPlayer = this->GetResolverPlayer(i);
		int iShotsFired = pCurResolverPlayer->GetShotsFired();

		if (!pCurEntity)
			continue;

		if (pCurEntity->IsDormant())
			continue;

		if (pCurEntity == pLocalEntity)
			continue;

		float fCurLby = pCurEntity->GetLowerBodyYaw();
		QAngle* qCurEyeAngles = pCurEntity->GetAngEyeAngles();
		bool bIsMoving = pCurEntity->GetVelocity()->Length() > 0.1f;

		Vector vRelativeDist = (*pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset()) - (*pCurEntity->GetOrigin() + *pCurEntity->GetEyeOffset());
		QAngle qAngle(
			RAD2DEG(-asinf(vRelativeDist.z / vRelativeDist.Length())),
			RAD2DEG(atan2f(vRelativeDist.y, vRelativeDist.x)),
			0.0f
		);

		switch (m_iResolverType) //todo: check if cur player got other resolver option
		{
		case RESOLVERTYPE_AUTOMATIC:
			//todo: not working
			// while moving LBY, while standing check if lby updated/bruteforce
			if (m_pApp->GlobalVars()->curtime - pCurResolverPlayer->GetRealLbyUpdateTime() > 1.1f)
			{
				if (!pCurResolverPlayer->m_bDidSet)
				{
					pCurResolverPlayer->SetAngles(*qCurEyeAngles);
					pCurResolverPlayer->m_bDidSet = true;
				}

				*qCurEyeAngles = pCurResolverPlayer->GetAngles();
				if (!m_pApp->AntiAim()->NextLBYUpdate(pCurResolverPlayer))
				{
					qCurEyeAngles->y += 180.0f;
				}
				qCurEyeAngles->x = 89;
				qCurEyeAngles->NormalizeAngles();
			}
			break;
		case RESOLVERTYPE_NOSPREAD:
			/**qCurEyeAngles = qAngle;
			if (iShotsFired > pCurResolverPlayer->GetShotsBruteforced())
			{
				switch (iShotsFired % 9)
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
				pCurResolverPlayer->SetShotsBruteforced(pCurResolverPlayer->GetShotsBruteforced() + 1);
			}*/
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