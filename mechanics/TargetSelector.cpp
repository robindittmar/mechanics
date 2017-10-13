#include "TargetSelector.h"
#include "Application.h"

CTargetSelector::CTargetSelector()
{
	m_bCheckHitbox[0] = true; m_iHitboxes[0] = HITBOX_HEAD;
	m_bCheckHitbox[1] = false; m_iHitboxes[1] = HITBOX_CHEST;
	m_bCheckHitbox[2] = false; m_iHitboxes[2] = HITBOX_PELVIS;
	m_bCheckHitbox[3] = false; m_iHitboxes[3] = HITBOX_RIGHT_FOREARM;
	m_bCheckHitbox[4] = false; m_iHitboxes[4] = HITBOX_LEFT_FOREARM;
	m_bCheckHitbox[5] = false; m_iHitboxes[5] = HITBOX_RIGHT_CALF;
	m_bCheckHitbox[6] = false; m_iHitboxes[6] = HITBOX_LEFT_CALF;
}

CTargetSelector::~CTargetSelector()
{
}

void CTargetSelector::Setup(CApplication* pApp)
{
	m_pApp = pApp;

	m_pEngineClient = m_pApp->EngineClient();
	m_pEntityList = m_pApp->EntityList();
}

void CTargetSelector::SelectTargets(float fInputSampleTime)
{
	this->ResetTargets();
	
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

	// Max values
	float fViewangleDist;
	float fOriginDist;
	float fDamage;
	float fLowestViewangleDist = 999999.0f;
	float fLowestOriginDist = 999999.0f;
	float fHighestDamage = -1.0f;

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

	int iMaxEntities = m_pApp->EngineClient()->GetMaxClients();
	for (int i = 1; i < iMaxEntities; i++)
	{
		pCurEntity = m_pEntityList->GetClientEntity(i);

		// Filter entites
		if (!pCurEntity)
			continue;

		// Skip dormant entities
		if (pCurEntity->IsDormant())
			continue;

		// Can't shoot ourself
		if (i == iLocalPlayerIdx)
			continue;

		// If the possible target isn't alive
		if (!pCurEntity->IsAlive())
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
				this->GetHitBoxCenter(pHitbox, pBoneMatrix, vEnemyPos);
				vEnemyPos += vCurVelocity;

				bIsHittable = true;
				break;
			}

			if (m_bMultipoint)
			{
				this->GetHitBoxVectors(pHitbox, pBoneMatrix, vHitbox);
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
							if (fDamage > fHighestDamage)
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
				this->GetHitBoxCenter(pHitbox, pBoneMatrix, vHitpoint);
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
						if (fDamage > fHighestDamage)
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

		// Nothing visible :(
		if (!bIsHittable)
		{
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

				iBacktracked = x;
				break;
			}
			
			if(iBacktracked == -1)
				continue;
		}


		// Calculate a few values
		if (iBacktracked == -1)
		{
			qAimAngles = Utils::CalcAngle(vMyHeadPos, vEnemyPos);
		}
		else
		{
			qAimAngles = Utils::CalcAngle(vMyHeadPos, pCurLCList.m_pPlayerEntries[iBacktracked].m_vHeadPos);
		}
		fOriginDist = this->GetOriginDist(vMyHeadPos, vEnemyPos);
		fViewangleDist = fabs(this->GetViewangleDist(qLocalViewAngles, qAimAngles/*, fOriginDist*/));

		// Unspecified
		if (!m_pTargets[TARGETCRITERIA_UNSPECIFIED].GetValid())
		{
			m_pTargets[TARGETCRITERIA_UNSPECIFIED].SetTarget(
				vEnemyPos,
				qAimAngles,
				pCurEntity,
				fViewangleDist,
				fOriginDist,
				iBacktracked
			);
		}

		// Origin
		if (fOriginDist < fLowestOriginDist)
		{
			fLowestOriginDist = fOriginDist;
			m_pTargets[TARGETCRITERIA_ORIGIN].SetTarget(
				vEnemyPos,
				qAimAngles,
				pCurEntity,
				fViewangleDist,
				fOriginDist,
				iBacktracked
			);
		}

		// Viewangles
		if (fViewangleDist < fLowestViewangleDist)
		{
			fLowestViewangleDist = fViewangleDist;
			m_pTargets[TARGETCRITERIA_VIEWANGLES].SetTarget(
				vEnemyPos,
				qAimAngles,
				pCurEntity,
				fViewangleDist,
				fOriginDist,
				iBacktracked
			);
		}
	}
}

void CTargetSelector::GetHitBoxCenter(mstudiobbox_t* hitBox, matrix3x4_t* boneMatrix, Vector& hitBoxVector)
{
	Vector bbmin;
	Vector bbmax;

	float fMod = hitBox->m_flRadius != -1.0f ? hitBox->m_flRadius : 0.0f;
	bbmin = (hitBox->m_vecBBMin - fMod);
	bbmax = (hitBox->m_vecBBMax + fMod);

	Vector vPoint = (bbmin + bbmax) * 0.5f;
	VectorTransform(vPoint, boneMatrix[hitBox->m_iBone], hitBoxVector);
}

void CTargetSelector::GetHitBoxVectors(mstudiobbox_t* hitBox, matrix3x4_t* boneMatrix, Vector* hitBoxVectors)
{
	Vector bbmin;
	Vector bbmax;

	// TODO: Try FixHitbox instead (trace from edge points to center and use trace.endpos as new edge)
	float fMod = hitBox->m_flRadius != -1.0f ? hitBox->m_flRadius : 0.0f;
	bbmin = (hitBox->m_vecBBMin - fMod) * 0.65f;
	bbmax = (hitBox->m_vecBBMax + fMod) * 0.65f;

	Vector vPoints[] = {
		(bbmin + bbmax) * 0.5f,
		Vector(bbmin.x, bbmin.y, bbmin.z),
		Vector(bbmin.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmin.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmin.y, bbmax.z),
		Vector(bbmax.x, bbmin.y, bbmax.z)
	};

	for (int i = 0; i < sizeof(vPoints) / sizeof(Vector); i++)
	{
		/*if (i != 0)
		vPoints[i] = ((((vPoints[i] + vPoints[0]) * 0.5f) + vPoints[i]) * 0.5f);*/

		VectorTransform(vPoints[i], boneMatrix[hitBox->m_iBone], hitBoxVectors[i]);
	}
}

float CTargetSelector::GetOriginDist(Vector& vSource, Vector& vTarget)
{
	return (vTarget - vSource).Length();
}

float CTargetSelector::GetViewangleDist(QAngle& qSource, QAngle& qTarget/*, float fOriginDistance*/)
{
	QAngle qDist = qTarget - qSource;
	qDist.NormalizeAngles();
	float fAng = qDist.Length();

	// Causes weird angles to have giant "fovs"
	//return (sinf(DEG2RAD(fAng)) * fOriginDistance);
	return fAng;
}

void CTargetSelector::ResetTargets()
{
	for (int i = 0; i < TARGETCRITERIA_COUNT; i++)
	{
		m_pTargets[i].Invalidate();
	}
}