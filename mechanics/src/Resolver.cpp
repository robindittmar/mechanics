#include "Resolver.h"
#include "Application.h"

CResolver::CResolver()
{
}

CResolver::~CResolver()
{
}

void CResolver::Think(void* pParameters)
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

		pCurResolverPlayer->m_bWasBreakingLby = pCurResolverPlayer->m_bBreakingLby;
		pCurResolverPlayer->m_bBreakingLby = false;

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
			continue;
		}

		if (!(pCurEntity->GetFlags() & FL_CLIENT))
			continue;

		if (pCurEntity->GetTeamNum() == pLocalEntity->GetTeamNum()) //same team dont need to resolve
			continue;

		switch (m_iResolverType) //todo: check if cur player got other resolver option
		{
		case RESOLVERTYPE_AUTOMATIC:
			CheckPlayer(pCurResolverPlayer, pCurEntity);

			if (pCurResolverPlayer->m_bBreakingLby)
			{
				//TODO: maybe formula exists to get the ~delta otherwise bruteforce xD
				//TODO: check if edge AA if so flip 180°

				BruteforceBreakingPlayer(pCurResolverPlayer, pCurEntity);
			}
			else if (pCurResolverPlayer->m_bMoving && pCurResolverPlayer->m_bOnGround)
			{
				// Setting lby because moving and onground
				pCurEntity->GetAngEyeAngles()->y = pCurEntity->GetLowerBodyYaw();
			}
			else if (pCurResolverPlayer->m_bMoving && !pCurResolverPlayer->m_bOnGround)
			{
				//TODO: kinda bruteforce because can't know?

				//temp!!
				pCurEntity->GetAngEyeAngles()->y = pCurEntity->GetLowerBodyYaw();
			}
			else if (pCurResolverPlayer->m_bFakeActive)
			{
				// Backup if fake is active set lby
				// Shouldn't happen tho
				pCurEntity->GetAngEyeAngles()->y = pCurEntity->GetLowerBodyYaw();
			}
			break;
		case RESOLVERTYPE_LBY:
			pCurEntity->GetAngEyeAngles()->y = pCurEntity->GetLowerBodyYaw();
			break;
		case RESOLVERTYPE_NONE:
		default:
			break;
		}
	}
}

void CResolver::CheckPlayer(CResolverPlayer* pCurResolverPlayer, IClientEntity* pCurEntity)
{
	float fLastLbyProxyUpdate = pCurResolverPlayer->GetLbyProxyUpdatedTime();
	float fCurtime = m_pApp->GlobalVars()->curtime;
	float fLowerBodyYaw = pCurEntity->GetLowerBodyYaw();
	QAngle qAngles = pCurResolverPlayer->GetAngles();

	pCurResolverPlayer->m_bFakewalking = (pCurEntity->GetVelocity()->Length2D() > 0.1f && pCurEntity->GetVelocity()->Length2D() < 35.0f);

	pCurResolverPlayer->m_bMoving = (pCurEntity->GetVelocity()->Length() > 0.1f);
	pCurResolverPlayer->m_bOnGround = (pCurEntity->GetFlags() & FL_ONGROUND);
	if (pCurResolverPlayer->m_bMoving)
	{
		// Only check if on ground because lby won't update in air
		if (pCurResolverPlayer->m_bOnGround)
		{
			// Check if was in air because lby wont update everytime landing!!
			if (!pCurResolverPlayer->m_bWasInAir)
			{
				// Time check maybe 0.2200001f
				pCurResolverPlayer->m_bFakeActive = (fabsf(qAngles.y - fLowerBodyYaw) > 35.0f);
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
			pCurResolverPlayer->m_bFakeActive = (fabsf(qAngles.y - fLowerBodyYaw) > 35.0f && fCurtime - fLastLbyProxyUpdate > 1.1f);
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

		// TODO: check if this detects fakewalk!!!

		CAnimationLayer breakingAnimLayer = pCurEntity->GetAnimationLayer(3);
		int iAct = pCurEntity->GetSequenceActivity(breakingAnimLayer.m_nSequence);

		pCurResolverPlayer->m_bBreakingLby = (iAct == ACT_CSGO_IDLE_TURN_BALANCEADJUST && breakingAnimLayer.m_flWeight > 0.0f &&
			(pCurResolverPlayer->m_fBalanceAdjustStart != -1 && pCurResolverPlayer->m_fBalanceAdjustStart + 1.1f < fCurtime));

		if (pCurResolverPlayer->m_bBreakingLby)
		{
			pCurResolverPlayer->m_iBreakingLbyInteruptCount = 0;
		}
		else
		{
			if (pCurResolverPlayer->m_bWasBreakingLby && pCurResolverPlayer->m_iBreakingLbyInteruptCount < 3)
			{
				pCurResolverPlayer->m_bBreakingLby = true;
				pCurResolverPlayer->m_iBreakingLbyInteruptCount++;
			}
		}
	}
	pCurResolverPlayer->m_bWasInAir = !pCurResolverPlayer->m_bOnGround;
}

void CResolver::BruteforcePlayer(CResolverPlayer* pCurResolverPlayer, IClientEntity* pCurEntity)
{
	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	QAngle* qCurEyeAngle = pCurEntity->GetAngEyeAngles();
	qCurEyeAngle->NormalizeAngles();

	int iModuloShots = pCurResolverPlayer->GetShotsFired() % 12;
	int iShotHit = pCurResolverPlayer->GetShotHit();

	// If a shot hitted head use that bruteforceposition
	if (iShotHit != -1)
	{
		iModuloShots = iShotHit;
	}

	// + links, - rechts
	switch (iModuloShots)
	{
	case 11:
		qCurEyeAngle->y = pCurResolverPlayer->GetAngles().y - 150.0f;
		break;
	case 10:
		qCurEyeAngle->y = pCurResolverPlayer->GetAngles().y + 150.0f;
		break;
	case 9:
		qCurEyeAngle->y = pCurResolverPlayer->GetAngles().y - 120.0f;
		break;
	case 8:
		qCurEyeAngle->y = pCurResolverPlayer->GetAngles().y + 120.0f;
		break;
	case 7:
		qCurEyeAngle->y = pCurResolverPlayer->GetAngles().y - 90.0f;
		break;
	case 6:
		qCurEyeAngle->y = pCurResolverPlayer->GetAngles().y + 90.0f;
		break;
	case 5:
		qCurEyeAngle->y = pCurResolverPlayer->GetAngles().y - 60.0f;
		break;
	case 4:
		qCurEyeAngle->y = pCurResolverPlayer->GetAngles().y + 60.0f;
		break;
	case 3:
		qCurEyeAngle->y = pCurResolverPlayer->GetAngles().y - 30.0f;
		break;
	case 2:
		qCurEyeAngle->y = pCurResolverPlayer->GetAngles().y + 30.0f;
		break;
	case 1:
		qCurEyeAngle->y = pCurResolverPlayer->GetAngles().y - 180.0f;
		break;
	case 0:
		break;
	}

	qCurEyeAngle->NormalizeAngles();
}

void CResolver::BruteforceBreakingPlayer(CResolverPlayer* pCurResolverPlayer, IClientEntity* pCurEntity)
{
	float fLowerBodyYaw = pCurEntity->GetLowerBodyYaw();
	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	QAngle* qCurEyeAngle = pCurEntity->GetAngEyeAngles();
	qCurEyeAngle->NormalizeAngles();

	int iModuloShots = pCurResolverPlayer->GetBreakingShotsFired() % 5;
	int iShotHit = pCurResolverPlayer->GetBreakingShotHit();

	// If a shot hitted head use that bruteforceposition
	if (iShotHit != -1)
	{
		iModuloShots = iShotHit;
	}

	// + links, - rechts
	switch (iModuloShots)
	{
	case 4:
		qCurEyeAngle->y = fLowerBodyYaw - 120.0f;
		break;
	case 3:
		qCurEyeAngle->y = fLowerBodyYaw + 120.0f;
		break;
	case 2:
		qCurEyeAngle->y = fLowerBodyYaw - 150.0f;
		break;
	case 1:
		qCurEyeAngle->y = fLowerBodyYaw + 150.0f;
		break;
	case 0:
		qCurEyeAngle->y = fLowerBodyYaw - 180.0f;
		break;
	}

	// Predicting LBY update
	if (m_pApp->AntiAim()->NextLBYUpdate(pCurResolverPlayer, false, false) && iModuloShots != 0)
	{
		qCurEyeAngle->y = fLowerBodyYaw;
		pCurResolverPlayer->m_bLbyPredict = true;
	}

	qCurEyeAngle->NormalizeAngles();
}
