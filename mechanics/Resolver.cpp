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
	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	for (int i = 1; i < m_pApp->EngineClient()->GetMaxClients(); i++)
	{
		IClientEntity* pCurEntity = m_pApp->EntityList()->GetClientEntity(i);
		CResolverPlayer* pCurResolverPlayer = this->GetResolverPlayer(i);

		pCurResolverPlayer->m_bHadFakeActive = pCurResolverPlayer->m_bFakeActive;
		pCurResolverPlayer->m_bFakeActive = false;
		pCurResolverPlayer->m_bBreakingLby = false;
		pCurResolverPlayer->m_bStartPredLbyBreaks = false;

		if (m_iResolverType == RESOLVERTYPE_NONE)
			continue;

		if (!m_pApp->Ragebot()->GetEnabled())
			continue;

		if (!pCurEntity)
			continue;

		if (pCurEntity->IsDormant())
		{
			// could be everything so reset!!
			pCurResolverPlayer->m_fBalanceAdjustStart = -1;
			pCurResolverPlayer->m_bAdjustedWithFake = false;
			pCurResolverPlayer->m_iFakeCount = 0;
			continue;
		}

		if (!(pCurEntity->GetFlags() & FL_CLIENT))
			continue;

		if (pCurEntity->GetTeamNum() == pLocalEntity->GetTeamNum()) //same team dont need to resolve
			continue;

		float fLastLbyProxyUpdate = pCurResolverPlayer->GetLbyProxyUpdatedTime();
		float fCurtime = m_pApp->GlobalVars()->curtime;
		float fLowerBodyYaw = pCurEntity->GetLowerBodyYaw();
		QAngle* qAngles = pCurEntity->GetAngEyeAngles();
		qAngles->NormalizeAngles();

		bool bIsMoving = (pCurEntity->GetVelocity()->Length() > 0.1f);
		bool bOnGround = (pCurEntity->GetFlags() & FL_ONGROUND);
		if (bIsMoving)
		{
			// Only check if on ground because lby won't update in air
			if (bOnGround)
			{
				// Check if was in air because lby wont update everytime landing!!
				if (!pCurResolverPlayer->m_bWasInAir)
				{
					// Time check maybe 0.2200001f
					pCurResolverPlayer->m_bFakeActive = (fabsf(qAngles->y - fLowerBodyYaw) > 35.0f);
				}
			}
			else
			{
				// Setting old value because if fake was active it should be still active
				pCurResolverPlayer->m_bFakeActive = pCurResolverPlayer->m_bHadFakeActive;
			}
		}
		else
		{
			// Check if was in air because lby wont update everytime landing!!
			if (!pCurResolverPlayer->m_bWasInAir)
			{
				pCurResolverPlayer->m_bFakeActive = (fabsf(qAngles->y - fLowerBodyYaw) > 35.0f && fCurtime - fLastLbyProxyUpdate > 1.1f);
			}

			//if (pCurResolverPlayer->m_bFakeActive)
				//g_pConsole->Write(LOGLEVEL_ERROR, "fake...\n");

			// Setting start of balanceadjust
			int iActivity = pCurEntity->GetSequenceActivity(pCurEntity->GetAnimationLayer(3).m_nSequence);
			if (pCurResolverPlayer->m_fBalanceAdjustStart == -1)
			{
				if (iActivity == ACT_CSGO_IDLE_TURN_BALANCEADJUST)
				{
					pCurResolverPlayer->m_fBalanceAdjustStart = fCurtime;
				}
			}

			// Resetting start of balanceadjust
			if (pCurResolverPlayer->m_fBalanceAdjustStart != -1 && iActivity != ACT_CSGO_IDLE_TURN_BALANCEADJUST)
			{
				pCurResolverPlayer->m_fBalanceAdjustStart = -1;
			}

			if (!pCurResolverPlayer->m_bFakeActive)
			{
				if (pCurResolverPlayer->m_bAdjustedWithFake)
				{
					//g_pConsole->Write(LOGLEVEL_INFO, "setting fake to adjustedwithfake..\n");

					pCurResolverPlayer->m_bFakeActive = pCurResolverPlayer->m_bAdjustedWithFake;
				}
				else
				{
					pCurResolverPlayer->m_iFakeCount = 0;
				}
			}
			else
			{
				pCurResolverPlayer->m_iFakeCount++;
			}

			// Has or had fake active
			if (pCurResolverPlayer->m_bFakeActive || pCurResolverPlayer->m_bHadFakeActive)
			{
				// is balance adjusting
				if (pCurResolverPlayer->m_fBalanceAdjustStart != -1 && pCurResolverPlayer->m_iFakeCount > 10)
				{
					pCurResolverPlayer->m_bAdjustedWithFake = true;
					pCurResolverPlayer->m_bBreakingLby = true;
					//g_pConsole->Write(LOGLEVEL_ERROR, "adjusting with fake...\n");
				}
				else
				{
					pCurResolverPlayer->m_bAdjustedWithFake = false;
				}
			}
			else
			{
				if (pCurResolverPlayer->m_fBalanceAdjustStart != -1)
				{
					// check if balanceadjust and should lby updated
					if (fCurtime + 1.1f > pCurResolverPlayer->m_fBalanceAdjustStart)
					{
						// Check if last lby update is older than balanceadjust
						if (fLastLbyProxyUpdate != 0 && fLastLbyProxyUpdate < pCurResolverPlayer->m_fBalanceAdjustStart)
						{
							// probably breaking lby
							//g_pConsole->Write(LOGLEVEL_INFO, "hack adjust..\n");
							pCurResolverPlayer->m_bBreakingLby = true;
						}
						else
						{
							//g_pConsole->Write(LOGLEVEL_INFO, "legit adjust..\n");
						}
					}
				}
			}
		}
		pCurResolverPlayer->m_bWasInAir = !bOnGround;

		// onground nicht weil kp was wir dann machen sollen :D
		if (bIsMoving /*&& bOnGround*/ ||
			pCurResolverPlayer->m_bFakeActive ||
			pCurResolverPlayer->m_bBreakingLby)
		{
			// Setting lby because moving and onground
			qAngles->y = fLowerBodyYaw;
		}

		switch (m_iResolverType) //todo: check if cur player got other resolver option
		{
		case RESOLVERTYPE_AUTOMATIC:
			// if moving
			//  - lby resolver
			// else
			//  - bruteforce
			//  - if lby breaker (after moving lby - lby after 1.1s <= 30)
			//  - - predictlbybreak

			if (!bIsMoving)
			{
				BruteforcePlayer(pCurResolverPlayer, pCurEntity);
			}
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
	if (!pCurResolverPlayer->m_bFakeActive)
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