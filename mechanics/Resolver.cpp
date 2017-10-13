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

		pCurResolverPlayer->m_bHasFakeActive = false;
		pCurResolverPlayer->m_bStartPredLbyBreaks = false;

		if (!pCurEntity)
			continue;

		if (pCurEntity->IsDormant())
			continue;

		if (pCurEntity == pLocalEntity) // dont need to resolve myself
			continue;

		if (pCurEntity->GetTeamNum() == pLocalEntity->GetTeamNum()) // same team dont need to resolve
			continue;

		if (!(pCurEntity->GetFlags() & FL_CLIENT))
			continue;

		float fCurtime = m_pApp->GlobalVars()->curtime;
		float fCurLby = pCurEntity->GetLowerBodyYaw();
		QAngle* qCurEyeAngles = pCurEntity->GetAngEyeAngles();
		qCurEyeAngles->NormalizeAngles();
		bool bIsMoving = pCurEntity->GetVelocity()->Length() > 0.1f;
		bool bLbyBreakPredicted = false;

		if (bIsMoving)
			pCurResolverPlayer->SetLastMovingTime(fCurtime);

		switch (m_iResolverType) //todo: check if cur player got other resolver option
		{
		case RESOLVERTYPE_AUTOMATIC:
			// if moving
			// - lby resolver
			// else
			// - if lby breaker (after moving lby - lby after 1.1s <= 30)
			// - - predictlbybreak
			// bruteforce

			// Checks if Fake Angles active
			pCurResolverPlayer->m_bHasFakeActive = fabsf(qCurEyeAngles->y - fCurLby) > 35.0f;

			if (bIsMoving)
			{
				qCurEyeAngles->y = fCurLby;

				/*pCurResolverPlayer->m_fFirstLbySinceStanding = -1;
				pCurResolverPlayer->m_fPossibleLbyBreakerStart = -1;
				pCurResolverPlayer->m_bStartPredLbyBreaks = false;*/
			}
			else
			{
				BruteforcePlayer(pCurResolverPlayer, pCurEntity);
				//if (pCurResolverPlayer->m_fFirstLbySinceStanding == -1)
					//{
					//	// Setting first LBY after moving
					//	pCurResolverPlayer->m_fFirstLbySinceStanding = fCurLby;
					//}

					//// Predicting LBY-Break
					//if (!PredictLbyBreak(pCurResolverPlayer, pCurEntity))
					//{
					//	// Bruteforcing Player's Real
					//	if (m_pApp->Ragebot()->IsShooting())
					//	{
					//		// Setting Bruteforced Real
					//		BruteforcePlayer(pCurResolverPlayer, pCurEntity);
					//	}
					//	else
					//	{
					//		// Setting PossibleReal
					//		qCurEyeAngles->y = pCurResolverPlayer->m_fPossibleLbyBreakerReal;
					//	}
					//}
					//else
					//{
					//	// Setting to PossibleFake because he needs to break
					//	qCurEyeAngles->y = pCurResolverPlayer->m_fPossibleLbyBreakerFake;
					//}
			}
			break;
		case RESOLVERTYPE_BRUTEFORCE:
			BruteforcePlayer(pCurResolverPlayer, pCurEntity);
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

bool CResolver::PredictLbyBreak(CResolverPlayer * pCurResolverPlayer, IClientEntity* pCurEntity)
{
	// No Fake = No LBY Breaker = No LbyBreak = No Pred needed
	if (!pCurResolverPlayer->m_bHasFakeActive)
		return false;

	float fCurLby = pCurEntity->GetLowerBodyYaw();
	float fCurtime = m_pApp->GlobalVars()->curtime;

	// Checks if LBY-Breaker could've startet
	if (pCurResolverPlayer->m_fPossibleLbyBreakerStart == -1 &&
		fabsf(pCurResolverPlayer->m_fFirstLbySinceStanding - fCurLby) > 35.0f) // todo: check if 35.0f is right value
	{
		// Possible start of LBY-Breaker
		pCurResolverPlayer->m_fPossibleLbyBreakerReal = pCurResolverPlayer->m_fFirstLbySinceStanding;
		pCurResolverPlayer->m_fPossibleLbyBreakerFake = fCurLby;
		pCurResolverPlayer->m_fPossibleLbyBreakerStart = fCurtime;
	}

	// Check if LBY-Breaker active
	pCurResolverPlayer->m_bStartPredLbyBreaks =
		((pCurResolverPlayer->m_fPossibleLbyBreakerStart != -1) && // checks if possible lby-breaker start time is valid
		(fCurtime - pCurResolverPlayer->m_fPossibleLbyBreakerStart > 1.1f)); // checks if lby should've updated

	// Checks if LBY-Breaker failed or is disabled
	if (pCurResolverPlayer->m_bStartPredLbyBreaks &&
		fCurtime - pCurResolverPlayer->GetLastMovingTime() > 2.0f &&
		fCurtime - pCurResolverPlayer->GetLbyProxyUpdatedTime() <= 1.1f)
	{
		pCurResolverPlayer->m_bStartPredLbyBreaks = false;
		pCurResolverPlayer->m_fPossibleLbyBreakerReal = pCurResolverPlayer->m_fFirstLbySinceStanding = fCurLby;
		pCurResolverPlayer->m_fPossibleLbyBreakerStart = m_pApp->GlobalVars()->curtime;
	}

	// Predicts LBY-Breaks
	if (pCurResolverPlayer->m_bStartPredLbyBreaks)
	{
		// Prediction LBY-Breaks
		return m_pApp->AntiAim()->NextLBYUpdate(pCurResolverPlayer);
	}

	return false;
}

void CResolver::BruteforcePlayer(CResolverPlayer* pCurResolverPlayer, IClientEntity* pCurEntity)
{
	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	QAngle* qCurEyeAngle = pCurEntity->GetAngEyeAngles();
	qCurEyeAngle->NormalizeAngles();

	int iModuloShots = pCurResolverPlayer->GetShotsFired() % 13;
	int iShotHit = pCurResolverPlayer->GetShotHit();

	if (iShotHit != -1)
	{
		iModuloShots = iShotHit;
	}
	// + links, - rechts
	switch (iModuloShots)
	{
	case 12:
		qCurEyeAngle->y = pCurResolverPlayer->GetOriginalYaw() - 150.0f;
		break;
	case 11:
		qCurEyeAngle->y = pCurResolverPlayer->GetOriginalYaw() + 150.0f;
		break;
	case 10:
		qCurEyeAngle->y = pCurResolverPlayer->GetOriginalYaw() - 120.0f;
		break;
	case 9:
		qCurEyeAngle->y = pCurResolverPlayer->GetOriginalYaw() + 120.0f;
		break;
	case 8:
		qCurEyeAngle->y = pCurResolverPlayer->GetOriginalYaw() - 90.0f;
		break;
	case 7:
		qCurEyeAngle->y = pCurResolverPlayer->GetOriginalYaw() + 90.0f;
		break;
	case 6:
		qCurEyeAngle->y = pCurResolverPlayer->GetOriginalYaw() - 60.0f;
		break;
	case 5:
		qCurEyeAngle->y = pCurResolverPlayer->GetOriginalYaw() + 60.0f;
		break;
	case 4:
		qCurEyeAngle->y = pCurResolverPlayer->GetOriginalYaw() - 30.0f;
		break;
	case 3:
		qCurEyeAngle->y = pCurResolverPlayer->GetOriginalYaw() + 30.0f;
		break;
	case 2:
		qCurEyeAngle->y = pCurResolverPlayer->GetOriginalYaw() - 180.0f;
		break;
	case 1:
	case 0:
		pCurResolverPlayer->SetOriginalYaw(qCurEyeAngle->y);
		break;
	}

	qCurEyeAngle->NormalizeAngles();
}