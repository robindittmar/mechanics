#include "Legitbot.h"
#include "Application.h"

//todo: LineGoesThroughSmoke Sig: 55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0

CLegitbot::CLegitbot()
	: m_bHasTarget(false), m_bHasDrawTarget(false), m_bAutoshoot(false),
	m_fTimeToAim(0.2f), m_fCurve(0.0f), m_fFieldOfView(90.0f),
	m_bDrawFieldOfView(false), m_clrFieldOfView(Color(0, 255, 0)), m_bDrawPath(true),
	m_clrPath(Color(255, 128, 0)), m_bShoot(false), m_fPointScale(0.5f)
{
	m_bCheckHitbox[0] = false; m_iHitboxes[0] = HITBOX_HEAD;
	m_bCheckHitbox[1] = false; m_iHitboxes[1] = HITBOX_CHEST;
	m_bCheckHitbox[2] = false; m_iHitboxes[2] = HITBOX_PELVIS;
	m_bCheckHitbox[3] = false; m_iHitboxes[3] = HITBOX_RIGHT_FOREARM;
	m_bCheckHitbox[4] = false; m_iHitboxes[4] = HITBOX_LEFT_FOREARM;
	m_bCheckHitbox[5] = false; m_iHitboxes[5] = HITBOX_RIGHT_CALF;
	m_bCheckHitbox[6] = false; m_iHitboxes[6] = HITBOX_LEFT_CALF;
}

CLegitbot::~CLegitbot()
{
}

void CLegitbot::Think(void* pParameters)
{
	assert(pParameters != NULL);

	if (!m_bIsEnabled)
		return;

	CreateMoveParam* pParam = (CreateMoveParam*)pParameters;

	if (pParam->pUserCmd->mousedx != 0.0f ||
		pParam->pUserCmd->mousedy != 0.0f)
	{
		m_bHasTarget = false;
		m_bHasDrawTarget = false;
		return;
	}

	IClientEntity* pLocalEntity;
	IClientEntity* pCurEntity;

	int iShotsFired;
	int iLocalTeam, iCurEntityTeam;
	Vector vLocalHeadPos, vCurEntityHeadPos;
	QAngle qLocalAngles;

	// Grab localplayer & validate
	pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	if (!pLocalEntity->IsAlive())
		return;

	// For TraceRay
	Ray_t ray;
	trace_t trace;
	CTraceFilterSkipEntity traceFilter(pLocalEntity);

	// Grab local info
	iShotsFired = pLocalEntity->GetShotsFired();
	iLocalTeam = pLocalEntity->GetTeamNum();
	vLocalHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();
	qLocalAngles = m_pApp->GetClientViewAngles();
	if (iShotsFired > 1)
		qLocalAngles += (*pLocalEntity->GetAimPunchAngle() * m_pApp->GetRecoilCompensation());

	if (m_bShoot)
	{
		m_bShoot = false;
		pParam->pUserCmd->buttons |= IN_ATTACK;
	}

	// Get weapon and check if valid weapon
	CWeapon* pActiveWeapon = pLocalEntity->GetActiveWeapon();
	if (!pActiveWeapon)
		return;

	if (pActiveWeapon->IsKnife() ||
		pActiveWeapon->IsNade() ||
		pActiveWeapon->IsC4())
		return;

	// Get weapon info
	CWeaponInfo* pWeaponInfo = pActiveWeapon->GetWeaponInfo();
	if (!pWeaponInfo)
		return;

	if (m_bHasTarget)
	{
		pCurEntity = m_pApp->EntityList()->GetClientEntity(m_iTarget);

		if (pCurEntity && !pCurEntity->IsDormant())
		{
			// Re-grab m_qEnd via hitbox of target
		}

		m_fAimProgress += m_fStepSize;
		if (m_fAimProgress > 1.0f)
			m_fAimProgress = 1.0f;

		QAngle qNewAngles = this->QuadraticBezier(m_qStart, m_qIntermediate, m_qEnd, m_fAimProgress);
		qNewAngles.NormalizeAngles();

		m_pApp->SetClientViewAngles(qNewAngles);

		if (m_fAimProgress >= 1.0f)
		{
			if (m_bAutoshoot && iShotsFired < 1)
				m_bShoot = true;

			m_bHasTarget = false;
			m_bHasDrawTarget = false;
			m_fAimProgress = 0.0f;
		}
		else if (m_bAutoshoot && iShotsFired < 1)
		{
			pParam->pUserCmd->buttons &= ~IN_ATTACK;
		}
	}
	else
	{
		//
		// If we have no target yet, we get a target
		// (only to get a starting point & an end point)
		//

		// Grab current headpos and trace forward (+recoil if shotsfired > 1) by weapon range
		// to check if we already aim on an enemy (if so, do nothing pls)
		Vector vForward, vTraceEnd;
		AngleVectors(qLocalAngles, &vForward);
		vTraceEnd = vLocalHeadPos + vForward * pWeaponInfo->flRange;

		ray.Init(vLocalHeadPos, vTraceEnd);
		m_pApp->EngineTrace()->TraceRay(ray, (MASK_SHOT_HULL | CONTENTS_HITBOX), &traceFilter, &trace);
		if (trace.hit_entity)
		{
			if (trace.hit_entity->GetClientClass()->m_ClassID == CCSPlayer &&
				trace.hit_entity->GetTeamNum() != iLocalTeam)
			{
				m_bHasDrawTarget = false;
				return;
			}
		}

		// Only appears on automatic guns
		if (iShotsFired > 1)
		{
			// If we don't want to help out
			if (iShotsFired % m_iHelpAfterShots != 0)
			{
				m_bHasDrawTarget = false;
				return;
			}
		}

		bool bKeyDown = GetAsyncKeyState(m_iTriggerKey) & 0x8000;
		if (!bKeyDown && !m_bDrawPath)
			return;

		int iTarget = -1;
		QAngle qTargetAng;
		QAngle qCurDist;
		QAngle qEnd;
		float fViewangleDist = 99999.9f;
		Vector vHitbox[9];

		matrix3x4_t pBoneMatrix[MAXSTUDIOBONES];
		studiohdr_t* pStudioModel;
		mstudiohitboxset_t* pHitboxSet;
		mstudiobbox_t* pHitbox;

		int iMaxClients = m_pApp->EngineClient()->GetMaxClients();
		for (int i = 0; i < iMaxClients; i++)
		{
			pCurEntity = m_pApp->EntityList()->GetClientEntity(i);
			if (!pCurEntity)
				continue;

			if (pCurEntity->IsDormant())
				continue;

			if (pLocalEntity == pCurEntity)
				continue;

			if (!(pCurEntity->GetFlags() & FL_CLIENT))
				continue;

			if (!pCurEntity->IsAlive())
				continue;

			iCurEntityTeam = pCurEntity->GetTeamNum();
			if (iCurEntityTeam != TEAMNUM_CT &&
				iCurEntityTeam != TEAMNUM_T ||
				iCurEntityTeam == iLocalTeam)
				continue;

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

			for (int i = 0; i < sizeof(m_iHitboxes) / sizeof(*m_iHitboxes); i++)
			{
				if (!m_bCheckHitbox[i])
					continue;

				pHitbox = pHitboxSet->pHitbox(m_iHitboxes[i]);
				if (!pHitbox)
					continue;

				Utils::GetHitBoxVectors(pHitbox, pBoneMatrix, vHitbox, m_fPointScale);

				for (int i = 0; i < sizeof(vHitbox) / sizeof(*vHitbox); i++)
				{
					ray.Init(vLocalHeadPos, vHitbox[i]);
					m_pApp->EngineTrace()->TraceRay(ray, (MASK_SHOT_HULL | CONTENTS_HITBOX), &traceFilter, &trace);
					if (trace.IsEntityVisible(pCurEntity))
					{
						vCurEntityHeadPos = vHitbox[i];

						qTargetAng = Utils::CalcAngle(vLocalHeadPos, vCurEntityHeadPos);
						qCurDist = (qTargetAng - qLocalAngles);
						qCurDist.NormalizeAngles();

						float fCurDist = qCurDist.Length2D();
						if (fCurDist < fViewangleDist && fCurDist < m_fFieldOfView)
						{
							fViewangleDist = fCurDist;

							iTarget = i;
							qEnd = qTargetAng;
						}
					}
				}
			}
		}

		if (iTarget != -1)
		{
			if (bKeyDown)
			{
				m_bHasTarget = true;

				if (m_bAutoshoot && iShotsFired < 1)
					pParam->pUserCmd->buttons &= ~IN_ATTACK;
			}

			m_bHasDrawTarget = true;
			m_iTarget = iTarget;
			m_fAimProgress = 0.0f;

			m_fStepSize = 1.0f / (m_fTimeToAim / m_pApp->GlobalVars()->interval_per_tick);

			m_qStart = m_pApp->GetClientViewAngles();
			m_qEnd = qEnd;
			if (iShotsFired > 1)
				m_qEnd -= (*pLocalEntity->GetAimPunchAngle() * m_pApp->GetRecoilCompensation());

			// Get interpolation point for bezier curve
			m_qIntermediate = m_qStart;
			m_qIntermediate.x += (m_qEnd.x - m_qStart.x) * m_fCurve;
		}
		else
		{
			m_bHasDrawTarget = false;
		}
	}
}

void CLegitbot::DrawFieldOfView(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bDrawFieldOfView)
		return;

	CGui* pGui = CGui::Instance();
	int iMidX = pGui->GetScreenWidth() / 2;
	int iMidY = pGui->GetScreenHeight() / 2;
	int iRadius = iMidX * (tanf(DEG2RAD(m_fFieldOfView * 0.5f)) / tanf(DEG2RAD(m_pApp->GetRenderFieldOfView() * 0.5f)));

	pSurface->DrawSetColor(m_clrFieldOfView);
	pSurface->DrawOutlinedCircle(iMidX, iMidY, iRadius, 64);
}

void CLegitbot::DrawPath(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bDrawPath)
		return;

	if (!m_bHasDrawTarget)
		return;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	QAngle qCurAngles;
	Vector vLocalPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();
	Vector vCurForward;
	Vector vCurScreen, vLastScreen;

	pSurface->DrawSetColor(m_clrPath);
	for (float fProgress = m_fAimProgress; fProgress <= 1.0f; fProgress += m_fStepSize)
	{
		qCurAngles = this->QuadraticBezier(m_qStart, m_qIntermediate, m_qEnd, fProgress);
		AngleVectors(qCurAngles, &vCurForward);

		vCurForward *= 8192.0f;
		vCurForward += vLocalPos;
		if (m_pApp->Gui()->WorldToScreen(vCurForward, vCurScreen))
		{
			if (vLastScreen.x != 0.0f && vLastScreen.y != 0.0f)
			{
				pSurface->DrawLine(vLastScreen.x, vLastScreen.y, vCurScreen.x, vCurScreen.y);
			}

			vLastScreen = vCurScreen;
		}

		if (fProgress < 1.0f && fProgress + m_fStepSize > 1.0f)
		{
			fProgress = 1.0f - m_fStepSize;
		}
	}
}

QAngle CLegitbot::QuadraticBezier(QAngle& p0, QAngle& p1, QAngle& p2, float t)
{
	QAngle qResult;

	qResult.x = ((1.0f - t) * (1.0f - t)) * p0.x + 2.0f * (1.0f - t) * t * p1.x + (t * t) * p2.x;
	qResult.y = ((1.0f - t) * (1.0f - t)) * p0.y + 2.0f * (1.0f - t) * t * p1.y + (t * t) * p2.y;

	return qResult;
}
