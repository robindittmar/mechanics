#include "Ragebot.h"
#include "Application.h"

CRagebot::CRagebot()
	: m_bAutoshoot(false), m_bAutoscope(false), m_bSilentAim(false),
	m_bCalculateHitchance(false), m_fHitchance(0.0f), m_fMinDamage(1.0f),
	m_bAutoReload(false), m_bAutoZeus(false), m_bAutoRevolver(false),
	m_iTargetCriteria(TARGETCRITERIA_VIEWANGLES), m_iVisibleMode(VISIBLEMODE_CANHIT), m_bMultipoint(false),
	m_fMultipointScale(0.5f)
{
	m_bCheckHitbox[0] = true; m_iHitboxes[0] = HITBOX_HEAD;
	m_bCheckHitbox[1] = false; m_iHitboxes[1] = HITBOX_CHEST;
	m_bCheckHitbox[2] = false; m_iHitboxes[2] = HITBOX_PELVIS;
	m_bCheckHitbox[3] = false; m_iHitboxes[3] = HITBOX_RIGHT_FOREARM;
	m_bCheckHitbox[4] = false; m_iHitboxes[4] = HITBOX_LEFT_FOREARM;
	m_bCheckHitbox[5] = false; m_iHitboxes[5] = HITBOX_RIGHT_CALF;
	m_bCheckHitbox[6] = false; m_iHitboxes[6] = HITBOX_LEFT_CALF;
}

CRagebot::~CRagebot()
{
}

void CRagebot::Setup()
{
	IFeature::Setup();

	m_pEngineClient = m_pApp->EngineClient();
	m_pEntityList = m_pApp->EntityList();
}

void CRagebot::Think(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	// Grab IsShooting before reseting for new tick
	bool bDidShootLastTick = m_bIsShooting;

	// Reset
	this->ResetTickVariables();

	// Local vars
	CreateMoveParam* pParam;
	CUserCmd* pUserCmd;
	float fInputSampleTime;

	IClientEntity* pLocalEntity;
	CWeapon* pMyActiveWeapon;

	// Initialize local variables
	pParam = (CreateMoveParam*)pParameters;
	pUserCmd = pParam->pUserCmd;
	fInputSampleTime = pParam->fInputSampleTime;

	pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	pMyActiveWeapon = pLocalEntity->GetActiveWeapon();
	if (!pMyActiveWeapon)
		return;

	// Aimbot invalid weapons
	if (pMyActiveWeapon->IsKnife() ||
		pMyActiveWeapon->IsNade() ||
		pMyActiveWeapon->IsC4())
		return;

	// Clip empty?
	if (pMyActiveWeapon->GetClip1() == 0)
	{
		if (m_bAutoReload)
			pUserCmd->buttons |= IN_RELOAD;

		return;
	}

	// TODO: Ghettofix for sniper not shooting after 1st shot
	if (pMyActiveWeapon->IsSniper() && bDidShootLastTick)
	{
		pUserCmd->buttons &= ~IN_ATTACK;
		return;
	}

	//CBenchmark targetBenchmark(true);
	// Choose target
	this->SelectTarget(fInputSampleTime);
	//targetBenchmark.FinishBenchmark();
	//targetBenchmark.PrintBenchmark("Target selection");

	if (!m_target.GetValid())
		return;

	// Copy target angles
	m_qAimAngles = *m_target.GetAimAngles();

	//CBenchmark hitchanceBenchmark(true);
	// Hitchance
	bool bHitchance = this->CalculateHitchance(pLocalEntity, pMyActiveWeapon, m_target.GetEntity());
	//hitchanceBenchmark.FinishBenchmark();
	//hitchanceBenchmark.PrintBenchmark("Calc Hitchance");

	if (!bHitchance)
		return;

	// AutoZeus stuff
	// Ehrlich gesagt versteh ich das nicht, vorher ist er ne for-schleife durchgegangen
	// und hat alle targets angeguckt; aber wtf? (macht ja keinen sinn an der stelle nochmal das target zu wechseln; falls das hinter ner
	// wand steht oder so
	bool bAbleToHit = !(pMyActiveWeapon->IsTaser() && m_bAutoZeus);
	if (!bAbleToHit)
	{
		float fOriginDist = m_target.GetOriginDist();
		int iTargetHealth = m_target.GetEntity()->GetHealth();

		// Check if enough damage is made to kill
		if (fOriginDist <= 185.0f)
			bAbleToHit = true;
		else if (fOriginDist < 225.0f && iTargetHealth < 72)
			bAbleToHit = true;
		else if (fOriginDist < 230.0f && iTargetHealth < 60)
			bAbleToHit = true;

		if (bAbleToHit)
			m_qAimAngles = *m_target.GetAimAngles();
	}

	// Set ClientViewAngles if we don't have silentaim activated
	// (we do so before applying NoRecoil :D)
	if (!this->m_bSilentAim)
	{
		m_pApp->SetClientViewAngles(m_qAimAngles);
	}

	// Apply viewangles & shoot if necessary
	this->ApplyViewanglesAndShoot(pUserCmd, pLocalEntity, bAbleToHit);
}

void inline CRagebot::ResetTickVariables()
{
	m_bIsShooting = false;
	m_fDamage = 0.0f;

	m_target.Invalidate();
}

void CRagebot::SelectTarget(float fInputSampleTime)
{
	int iLocalPlayerIdx;
	IClientEntity* pLocalEntity;
	IClientEntity* pCurEntity;

	// My values
	int iMyTeamNum;
	Vector vMyHeadPos;
	CWeapon* pMyActiveWeapon;

	// Angle stuff
	QAngle qLocalViewAngles;
	QAngle qAimAngles;

	// Position stuff
	Vector vEnemyPos;

	// BoneMatrix & Hitboxes
	matrix3x4_t pBoneMatrix[MAXSTUDIOBONES];
	studiohdr_t* pStudioModel;
	mstudiohitboxset_t* pHitboxSet;
	mstudiobbox_t* pHitbox;

	// Hitbox vectors
	Vector vHitpoint;
	Vector vHitbox[9];

	float fDamage;
	float fHighestDamage = FLT_MIN;

	float fOriginDist;
	float fViewangleDist;
	float fLowestDist = FLT_MAX;

	qLocalViewAngles = m_pApp->GetClientViewAngles();

	iLocalPlayerIdx = m_pEngineClient->GetLocalPlayer();
	pLocalEntity = m_pEntityList->GetClientEntity(iLocalPlayerIdx);
	if (!pLocalEntity)
		return;

	Ray_t ray;
	trace_t trace;
	CTraceFilterSkipEntity traceFilter(pLocalEntity);
	CTraceFilterWorldAndPropsOnly filter;

	iMyTeamNum = pLocalEntity->GetTeamNum();
	vMyHeadPos = *pLocalEntity->GetOrigin() + (*pLocalEntity->GetVelocity() * fInputSampleTime);
	vMyHeadPos += *pLocalEntity->GetEyeOffset();
	pMyActiveWeapon = pLocalEntity->GetActiveWeapon();

	int iMaxClients = m_pApp->GlobalVars()->maxClients;
	for (int i = 0; i < iMaxClients; i++)
	{
		pCurEntity = m_pEntityList->GetClientEntity(i);
		if (!pCurEntity)
			continue;

		// Skip dormant entities
		if (pCurEntity->IsDormant())
			continue;

		// If the possible target isn't alive
		if (!pCurEntity->IsAlive())
			continue;

		// Can't shoot ourself
		if (i == iLocalPlayerIdx)
			continue;
		
		// Only from enemy team & we don't want spectators or something
		int entityTeam = pCurEntity->GetTeamNum();
		if (entityTeam == iMyTeamNum || entityTeam != TEAMNUM_T && entityTeam != TEAMNUM_CT)
			continue;

		// Spawn protection
		if (pCurEntity->IsInvincible())
			continue;

		if (!pCurEntity->SetupBones(pBoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, m_pApp->EngineClient()->GetLastTimeStamp()))
			continue;

		const model_t* pModel = pCurEntity->GetModel();
		if (!pModel)
			continue;

		pStudioModel = m_pApp->ModelInfo()->GetStudiomodel(pModel);
		if (!pStudioModel)
			continue;

		pHitboxSet = pStudioModel->pHitboxSet(0);
		if (!pHitboxSet)
			continue;

		//
		// = TODO =
		// -> GetCenterOfHitbox
		// =============
		// -> FixHitbox
		// -> Multipoint scan variable enable/disable
		//

		bool bIsHittable = false;
		IEngineTrace* pEngineTrace = m_pApp->EngineTrace();
		Vector vCurVelocity = (*pCurEntity->GetVelocity() * fInputSampleTime);
		for (int i = 0; i < TARGET_HITBOX_COUNT; i++)
		{
			if (!m_bCheckHitbox[i])
				continue;

			pHitbox = pHitboxSet->pHitbox(m_iHitboxes[i]);
			if (!pHitbox)
				continue;

			// If we shouldn't check visibility, get hitbox pos and go on
			if (m_iVisibleMode == VISIBLEMODE_IGNORE)
			{
				Utils::GetHitBoxCenter(pHitbox, pBoneMatrix, vEnemyPos);
				vEnemyPos += vCurVelocity;

				bIsHittable = true;
				break;
			}

			if (m_bMultipoint)
			{
				Utils::GetHitBoxVectors(pHitbox, pBoneMatrix, vHitbox, m_fMultipointScale);
				// TODO: FixHitbox?

				for (int i = 0; i < sizeof(vHitbox) / sizeof(Vector); i++)
				{
					vHitbox[i] += vCurVelocity;

					if (m_iVisibleMode == VISIBLEMODE_FULLVISIBLE)
					{
						ray.Init(vMyHeadPos, vHitbox[i]);
						pEngineTrace->TraceRay(ray, (MASK_SHOT_HULL | CONTENTS_HITBOX), &traceFilter, &trace);
						if (trace.IsEntityVisible(pCurEntity))
						{
							vEnemyPos = vHitbox[i];
							bIsHittable = true;
							break;
						}
					}
					else if (m_iVisibleMode == VISIBLEMODE_CANHIT)
					{
						// TODO:
						if (Autowall::CanHit(vHitbox[i], &fDamage))
						{
							if (fDamage > fHighestDamage && fDamage > m_fMinDamage)
							{
								vEnemyPos = vHitbox[i];

								fHighestDamage = fDamage;
								bIsHittable = true;
							}
						}
					}

				}
			}
			else
			{
				Utils::GetHitBoxCenter(pHitbox, pBoneMatrix, vHitpoint);
				vHitpoint += vCurVelocity;

				if (m_iVisibleMode == VISIBLEMODE_FULLVISIBLE)
				{
					ray.Init(vMyHeadPos, vHitpoint);
					pEngineTrace->TraceRay(ray, (MASK_SHOT_HULL | CONTENTS_HITBOX), &traceFilter, &trace);
					if (trace.IsEntityVisible(pCurEntity))
					{
						vEnemyPos = vHitpoint;
						bIsHittable = true;
					}
				}
				else if (m_iVisibleMode == VISIBLEMODE_CANHIT)
				{
					if (Autowall::CanHit(vHitpoint, &fDamage))
					{
						if (fDamage > fHighestDamage && fDamage > m_fMinDamage)
						{
							vEnemyPos = vHitpoint;

							fHighestDamage = fDamage;
							bIsHittable = true;
						}
					}
				}
			}

			if (bIsHittable)
				break;
		}

		CLagCompensationPlayerList pCurLCList = *m_pApp->LagCompensation()->GetLCList(i);
		pCurLCList.RemoveInvalidPlayerEntries();
		int iBacktracked = -1;

		CResolverPlayer* pCurResolverPlayer = m_pApp->Resolver()->GetResolverPlayer(i);

		Ray_t lcRay;
		trace_t lcTrace;
		Vector vCurHeadPos;
		for (int x = pCurLCList.m_iEntryCount - 1; x >= 0; x--)
		{
			vCurHeadPos = pCurLCList.m_pPlayerEntries[x].m_vHeadPos;

			ray.Init(vMyHeadPos, vCurHeadPos);
			m_pApp->EngineTrace()->TraceRay(ray, 0x200400B, &filter, &trace);
			if (!trace.IsVisible())
				continue;

			// TODO: maybe better or use AWall code not a trace <3
			if (pCurLCList.m_pPlayerEntries[x].m_iTickCount == pCurResolverPlayer->m_iLbyUpdateTickCount)
			{
				iBacktracked = x;
				break;
			}

			if (iBacktracked == -1)
			{
				iBacktracked = x;
			}
		}

		// Nothing visible :(
		if (!bIsHittable)
		{
			if (iBacktracked == -1)
				continue;
		}

		// Calculate values
		if (iBacktracked == -1)
		{
			qAimAngles = Utils::CalcAngle(vMyHeadPos, vEnemyPos);
			fOriginDist = Utils::GetOriginDist(vMyHeadPos, vEnemyPos);
		}
		else
		{
			qAimAngles = Utils::CalcAngle(vMyHeadPos, pCurLCList.m_pPlayerEntries[iBacktracked].m_vHeadPos);
			fOriginDist = Utils::GetOriginDist(vMyHeadPos, pCurLCList.m_pPlayerEntries[iBacktracked].m_vHeadPos);
		}
		fViewangleDist = fabs(Utils::GetViewangleDist(qLocalViewAngles, qAimAngles/*, fOriginDist*/));

		switch (m_iTargetCriteria)
		{
		case TARGETCRITERIA_ORIGIN:
			if (fOriginDist < fLowestDist)
			{
				fLowestDist = fOriginDist;

				m_target.SetTarget(
					vEnemyPos,
					qAimAngles,
					pCurEntity,
					fViewangleDist,
					fOriginDist,
					iBacktracked,
					fHighestDamage
				);
			}
			break;
		case TARGETCRITERIA_VIEWANGLES:
			// Viewangles
			if (fViewangleDist < fLowestDist)
			{
				fLowestDist = fViewangleDist;

				m_target.SetTarget(
					vEnemyPos,
					qAimAngles,
					pCurEntity,
					fViewangleDist,
					fOriginDist,
					iBacktracked,
					fHighestDamage
				);
			}
			break;
		case TARGETCRITERIA_UNSPECIFIED:
		default:
			m_target.SetTarget(
				vEnemyPos,
				qAimAngles,
				pCurEntity,
				fViewangleDist,
				fOriginDist,
				iBacktracked,
				fHighestDamage
			);
			break;
		}
	}
}

void CRagebot::ApplyViewanglesAndShoot(CUserCmd* pUserCmd, IClientEntity* pLocalEntity, bool bAbleToHit)
{
	CWeapon* pActiveWeapon = (CWeapon*)pLocalEntity->GetActiveWeapon();
	if (!pActiveWeapon)
		return;

	float fNextattack = pActiveWeapon->GetNextPrimaryAttack();
	float fServertime = pLocalEntity->GetTickBase() * m_pApp->GlobalVars()->interval_per_tick;

	if (this->m_bAutoshoot && pActiveWeapon->IsSniper())
	{
		if (this->m_bAutoscope)
		{
			if (pLocalEntity->IsScoped())
			{
				this->Shoot(pUserCmd, fNextattack, fServertime);
			}
			else
			{
				pUserCmd->buttons |= IN_ATTACK2;
			}
		}
		else
		{
			this->Shoot(pUserCmd, fNextattack, fServertime);
		}
	}
	else if (this->m_bAutoshoot && bAbleToHit)
	{
		this->Shoot(pUserCmd, fNextattack, fServertime);
	}
	else if (!this->m_bAutoshoot && bAbleToHit)
	{
		this->Aim(pUserCmd);
	}
}

void inline CRagebot::Shoot(CUserCmd* pUserCmd, float fNextPrimaryAttack, float fServerTime)
{
	if (fNextPrimaryAttack <= fServerTime)
	{
		this->Aim(pUserCmd);

		pUserCmd->buttons |= IN_ATTACK;
		m_bIsShooting = true;
	}
}

void inline CRagebot::Aim(CUserCmd* pUserCmd)
{
	// Write viewangles
	pUserCmd->viewangles[0] = m_qAimAngles.x;
	pUserCmd->viewangles[1] = m_qAimAngles.y;
}

bool CRagebot::IsAbleToApplyNoSpread()
{
	static ConVar* pCustomRandomSeed = m_pApp->CVar()->FindVar(/*sv_usercmd_custom_random_seed*/CXorString("\x64\x7D\xDA\xB7\x64\x6E\xF7\xA1\x7A\x6F\xDA\xA1\x62\x78\xF1\xAD\x7A\x54\xF7\xA3\x79\x6F\xEA\xAF\x48\x78\xE0\xA7\x73").ToCharArray());
	return atoi(pCustomRandomSeed->value) == 0;
}
bool CRagebot::IsNoSpread()
{
	static ConVar* pWeaponAccuracity = m_pApp->CVar()->FindVar(/*weapon_accuracy_nospread*/CXorString("\x60\x6E\xE4\xB2\x78\x65\xDA\xA3\x74\x68\xF0\xB0\x76\x68\xFC\x9D\x79\x64\xF6\xB2\x65\x6E\xE4\xA6").ToCharArray());
	return atoi(pWeaponAccuracity->value) == 1;
}


void CRagebot::AutoRevolver(CUserCmd* pUserCmd)
{
	if (!m_bIsEnabled)
		return;

	m_bDoingAutoRevolver = false;

	if (!m_bAutoRevolver)
		return;

	if (m_bIsShooting)
		return;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	CWeapon* activeWeapon = (CWeapon*)pLocalEntity->GetActiveWeapon();
	if (!activeWeapon)
		return;

	if (activeWeapon->GetWeaponId() != WEAPON_REVOLVER)
		return;

	if (activeWeapon->GetClip1() == 0)
		return;

	m_bDoingAutoRevolver = true;
	pUserCmd->buttons |= IN_ATTACK;
	float flPostponeFireReady = activeWeapon->GetPostPoneFireReady();
	if (flPostponeFireReady > 0 && flPostponeFireReady <= (pLocalEntity->GetTickBase() * m_pApp->GlobalVars()->interval_per_tick))
	{
		pUserCmd->buttons &= ~IN_ATTACK;
	}
}

bool CRagebot::CalculateHitchance(IClientEntity* pLocalEntity, CWeapon* pActiveWeapon, IClientEntity* pTarget)
{
	if (!m_bCalculateHitchance)
		return true;

	int iHits = 0;
	int iHitsRequired = m_fHitchance * 256;

	int iMisses = 0;
	int iMissesRquired = 256 - iHitsRequired;

	CWeaponInfo* pWeaponInfo = pActiveWeapon->GetWeaponInfo();
	if (!pWeaponInfo)
		return false;

	Vector vHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();
	Vector vForward;
	QAngle qCurAngles;

	Ray_t ray;
	trace_t trace;
	CTraceFilterOnlyPlayers traceFilter(pLocalEntity);

	pActiveWeapon->UpdateAccuracyPenalty();
	for (int i = 1; i <= 256; i++)
	{
		qCurAngles = m_qAimAngles;
		m_pApp->GunAccuracy()->ApplySpreadToAngles(qCurAngles, pLocalEntity, pActiveWeapon, i);

		AngleVectors(qCurAngles, &vForward);

		ray.Init(vHeadPos, vHeadPos + (vForward * pWeaponInfo->m_WeaponRange));
		m_pApp->EngineTrace()->TraceRay(ray, (MASK_SHOT_HULL | CONTENTS_HITBOX), &traceFilter, &trace);
		if (trace.DidHitEntity(pTarget))
		{
			if (++iHits >= iHitsRequired)
				return true;
		}
		else
		{
			if (++iMisses >= iMissesRquired)
				return false;
		}
	}

	return false;
}