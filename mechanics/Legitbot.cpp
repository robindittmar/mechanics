#include "Legitbot.h"
#include "Application.h"

//todo: LineGoesThroughSmoke Sig: 55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0

CLegitbot::CLegitbot()
	: m_bHasTarget(false), m_fStepSize(0.005f), m_bDrawTarget(true)
{
}

CLegitbot::~CLegitbot()
{
}

void CLegitbot::Setup()
{
	m_pApp = CApplication::Instance();
}

void CLegitbot::Update(void* pParameters)
{
	assert(pParameters != NULL);

	if (!m_bIsEnabled)
		return;

	CreateMoveParam* pParam = (CreateMoveParam*)pParameters;

	IClientEntity* pLocalEntity;
	IClientEntity* pCurEntity;

	int iLocalTeam, iCurEntityTeam;
	Vector vLocalHeadPos, vCurEntityHeadPos;
	QAngle qLocalAngles;

	pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	if (!pLocalEntity->IsAlive())
		return;

	Ray_t ray;
	trace_t trace;
	CTraceFilterSkipEntity traceFilter(pLocalEntity);

	iLocalTeam = pLocalEntity->GetTeamNum();
	vLocalHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();
	qLocalAngles = m_pApp->GetClientViewAngles();

	if (m_bHasTarget)
	{
		pCurEntity = m_pApp->EntityList()->GetClientEntity(m_iTarget);

		if (pCurEntity && !pCurEntity->IsDormant())
		{
			// Re-grab m_qEnd via hitbox of target
		}

		m_fAimProgress += m_fStepSize;
		QAngle qNewAngles = this->QuadraticBezier(m_qStart, m_qIntermediate, m_qEnd, m_fAimProgress);

		m_pApp->SetClientViewAngles(qNewAngles);

		if (m_fAimProgress >= 1.0f)
		{
			m_bHasTarget = false;
			m_fAimProgress = 0.0f;
		}
	}
	else
	{
		//
		// If we have no target yet, we get a target
		// (only to get a starting point & an end point)
		//

		// TODO
		bool bKeyDown = GetAsyncKeyState(VK_XBUTTON1) & 0x8000;

		if (!bKeyDown && !m_bDrawTarget)
			return;

		int iTarget = -1;
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

			pHitbox = pHitboxSet->pHitbox(HITBOX_HEAD);
			if (!pHitbox)
				continue;

			// TODO: PointScale
			Utils::GetHitBoxVectors(pHitbox, pBoneMatrix, vHitbox);

			bool bIsVisible = false;
			for (int i = 0; i < sizeof(vHitbox) / sizeof(Vector); i++)
			{
				ray.Init(vLocalHeadPos, vHitbox[i]);
				m_pApp->EngineTrace()->TraceRay(ray, (MASK_SHOT_HULL | CONTENTS_HITBOX), &traceFilter, &trace);
				if (trace.IsEntityVisible(pCurEntity))
				{
					vCurEntityHeadPos = vHitbox[i];
					bIsVisible = true;
					break;
				}
			}

			if (!bIsVisible)
				continue;

			QAngle qTargetAng = Utils::CalcAngle(vLocalHeadPos, vCurEntityHeadPos);
			QAngle qCurDist = (qTargetAng - qLocalAngles);
			qCurDist.NormalizeAngles();

			//PlayerInfo pi;
			//pCurEntity->GetPlayerInfo(&pi);

			float fCurDist = qCurDist.Length();
			if (fCurDist < fViewangleDist)
			{
				fViewangleDist = fCurDist;
				
				iTarget = i;
				qEnd = qTargetAng;
			}
		}

		if (iTarget != -1)
		{
			if (bKeyDown)
				m_bHasTarget = true;

			m_bHasDrawTarget = true;
			m_iTarget = iTarget;
			m_fAimProgress = 0.0f;

			m_qStart = m_pApp->GetClientViewAngles();
			m_qIntermediate.x = m_qStart.x;
			m_qIntermediate.y = qEnd.y;
			m_qEnd = qEnd;
		}
		else
		{
			m_bHasDrawTarget = false;
		}
	}
}

void CLegitbot::DrawTarget(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bDrawTarget)
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

	float fProgressPlusOne = m_fAimProgress + m_fStepSize;

	pSurface->DrawSetColor(255, 0, 255, 0);
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
	}
}

QAngle CLegitbot::QuadraticBezier(QAngle& p0, QAngle& p1, QAngle& p2, float t)
{
	QAngle qResult;

	qResult.x = ((1 - t) * (1 - t)) * p0.x + 2 * (1 - t) * t * p1.x + (t * t) * p2.x;
	qResult.y = ((1 - t) * (1 - t)) * p0.y + 2 * (1 - t) * t * p1.y + (t * t) * p2.y;

	return qResult;
}
