#include "Aimbot.h"
#include "Application.h"
#include "IEngineTrace.h"
#include "CGameTrace.h"
#include "ray.h"
#include "Console.h"

CAimbot::CAimbot()
{
	m_bSilentAim = false;
	m_bAutoshoot = true;
	m_bAutoscope = false;
	m_bDoNoRecoil = true;

	m_tTargetCriteria = TargetCriteriaUnspecified;
	m_fSpeed = 1.0f;
	m_fFov = 360.0f;
}

CAimbot::~CAimbot()
{
}

void CAimbot::Setup()
{
	m_pApp = CApplication::Instance();
}

int GetBoneByName(CApplication* pApp, IClientEntity* player, const char* bone)
{
	/*studiohdr_t pStudioModel = pApp->ModelInfo()->GetStudioModel(player->GetModel());
	if (!pStudioModel)
		return -1;

	*/
	return 0;
}

struct mat3x4
{
	float c[3][4];
};

// TODO: Still needs some reworking
void CAimbot::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	AimbotUpdateParam* pParam = (AimbotUpdateParam*)pParameters;

	// TODO
	//*m_pApp->m_bSendPackets = true;
	m_bHasTarget = false;
	m_bIsShooting = false;
	m_bDidNoRecoil = false;

	// Update code here
	IVEngineClient* pEngineClient = m_pApp->EngineClient();
	IClientEntityList* pEntityList = m_pApp->EntityList();

	int iLocalPlayerIdx = pEngineClient->GetLocalPlayer();
	IClientEntity* pLocalEntity = pEntityList->GetClientEntity(iLocalPlayerIdx);
	IClientEntity* pCurEntity;
	QAngle qAimAngles, qLocalViewAngles = m_pApp->ClientViewAngles();
	CUserCmd* pUserCmd = pParam->pUserCmd;

	if (!pUserCmd)
		return;

	if (!pLocalEntity)
		return;

	// TODO: This has moved below finding a target
	/*CWeapon* pActiveWeapon = (CWeapon*)pLocalEntity->ActiveWeapon();
	if (//pActiveWeapon->IsKnife() ||
		pActiveWeapon->IsNade() ||
		pActiveWeapon->IsC4() ||
		pActiveWeapon->Clip1() == 0)
		return;
	
	if (!this->m_bAutoshoot && !(pUserCmd->buttons & IN_ATTACK))
		return;*/

	// Get position + add relative eye position
	Vector myHeadPos = *pLocalEntity->Origin() + (*pLocalEntity->Velocity() * pParam->fInputSampleTime);
	myHeadPos += *pLocalEntity->EyeOffset();
	int localTeam = pLocalEntity->TeamNum();

	Vector targetPos;
	QAngle targetAngles;

	Ray_t ray;
	trace_t trace;
	CTraceFilterSkipEntity traceFilter(pLocalEntity);

	m_iSelectedTarget = -1;
	float fViewangleDist;
	float fOriginDist;
	float fLowestDist = 999999.0f;

	// Start at i=1 since 0 is usually (always as of now) the local player
	int iMaxEntities = m_pApp->EngineClient()->GetMaxClients();
	for (int i = 1; i < iMaxEntities; i++)
	{
		pCurEntity = pEntityList->GetClientEntity(i);

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
		int entityTeam = pCurEntity->TeamNum();
		if (entityTeam == localTeam || entityTeam == 0 || entityTeam == 1)
			continue;

		// Spawn protection
		if (pCurEntity->IsInvincible())
			continue;

		// Bone ID: 8 (maybe 7=neck)
		// 10 ca chest
		//studiohdr_t* pModel = m_pApp->ModelInfo()->GetStudioModel(pCurEntity->GetModel());

		// TODO
		// 8, 10, 72, 79
		bool isVisible = false;
		int boneIdx[] = { 8, 10, 72, 79 };
		int bone = 0;
		int boneCount = sizeof(boneIdx) / sizeof(int);

		mat3x4* boneMatrix;
		Vector headPos;

		for(bone = 0; bone < boneCount; bone++)
		{
			// Get matrix for current bone
			boneMatrix = (mat3x4*)((*(DWORD*)((DWORD)pCurEntity + 0x2698)) + (0x30 * boneIdx[bone]));

			// Get position
			headPos.x = boneMatrix->c[0][3];
			headPos.y = boneMatrix->c[1][3];
			headPos.z = boneMatrix->c[2][3];

			// Prediction
			headPos += (*pCurEntity->Velocity() * pParam->fInputSampleTime);

			ray.Init(myHeadPos, headPos);
			m_pApp->EngineTrace()->TraceRay(ray, 0x4600400B, &traceFilter, &trace);
			if (trace.IsVisible(pCurEntity))
			{
				isVisible = true;
				break;
			}
		}

		// Nothing visible :(
		if (!isVisible)
			continue;

		switch (m_tTargetCriteria)
		{
		case TargetCriteriaOrigin:
			fOriginDist = this->GetOriginDist(myHeadPos, headPos);
			if (fOriginDist < fLowestDist)
			{
				fLowestDist = fOriginDist;

				m_iSelectedTarget = i;
				targetPos = headPos;

				continue;
			}

			break;
		case TargetCriteriaViewangle:
			// Get Origin distance for "real" FOV (independent of distance)
			fOriginDist = this->GetOriginDist(myHeadPos, headPos);

			// Relative position
			headPos -= myHeadPos;
			// Calc angle
			m_qAimAngles = this->CalcAngle(headPos);

			// Calculate our fov to the enemy
			fViewangleDist = fabs(this->GetViewangleDist(qLocalViewAngles, m_qAimAngles, fOriginDist));
			// TODO: GetViewangleDist doesn't return a nice FOV, as it doesn't take fOriginDist into account (RIGHT NOW!)
			if (fViewangleDist > m_fFov)
			{
				continue;
			}
			else if (fViewangleDist < fLowestDist)
			{
				fLowestDist = fViewangleDist;

				m_iSelectedTarget = i;
				targetAngles = m_qAimAngles;

				continue;
			}
			break;
		case TargetCriteriaUnspecified:
			m_iSelectedTarget = i;
			targetPos = headPos;

			// Force the entityloop to exit out (break would only break switch)
			i = iMaxEntities;
			break;
		}
	}

	// If we found no target just exit
	if (m_iSelectedTarget == -1)
		return;

	// Save that we got a target :)
	m_bHasTarget = true;

	if (!this->m_bAutoshoot && !(pUserCmd->buttons & IN_ATTACK))
		return;

	CWeapon* pActiveWeapon = (CWeapon*)pLocalEntity->ActiveWeapon();
	if (pActiveWeapon->IsKnife() ||
		pActiveWeapon->IsNade() ||
		pActiveWeapon->IsC4() ||
		pActiveWeapon->Clip1() == 0)
		return;

	if (m_tTargetCriteria != TargetCriteriaViewangle)
	{
		// Get relative position to ourselves
		targetPos -= myHeadPos;

		// Calculate our viewangles to aim at the enemy
		m_qAimAngles = this->CalcAngle(targetPos);
	}
	else
	{
		m_qAimAngles = targetAngles;
	}

	// Set ClientViewAngles if we don't have silentaim activated
	if (!this->m_bSilentAim)
	{
		m_pApp->ClientViewAngles(m_qAimAngles);
	}

	// If we have no recoil activated in the aimbot, do it
	// (and remember that we did!)
	if(m_bDoNoRecoil)
	{
		QAngle aimPunchAngle = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));
		m_qAimAngles.x -= aimPunchAngle.x * RECOIL_COMPENSATION;
		m_qAimAngles.y -= aimPunchAngle.y * RECOIL_COMPENSATION;

		m_pApp->m_oldAimPunchAngle.x = aimPunchAngle.x * RECOIL_COMPENSATION;
		m_pApp->m_oldAimPunchAngle.y = aimPunchAngle.y * RECOIL_COMPENSATION;

		m_bDidNoRecoil = true;
	}

	pUserCmd->viewangles[0] = m_qAimAngles.x;
	pUserCmd->viewangles[1] = m_qAimAngles.y;

	float fNextattack = pActiveWeapon->NextPrimaryAttack();
	float fServertime = pLocalEntity->TickBase() * m_pApp->GlobalVars()->interval_per_tick;

	if (this->m_bAutoshoot && pActiveWeapon->IsSniper() && !pActiveWeapon->IsTaser())
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
	else if (this->m_bAutoshoot && !pActiveWeapon->IsTaser())
	{
		this->Shoot(pUserCmd, fNextattack, fServertime);
	}

	// TODO
	//if (this->m_bIsShooting)
	//	*m_pApp->m_bSendPackets = false;
}

void inline CAimbot::Shoot(CUserCmd* pUserCmd, float fNextPrimaryAttack, float fServerTime)
{
	if (fNextPrimaryAttack <= fServerTime)
	{
		pUserCmd->buttons |= IN_ATTACK;
		m_bIsShooting = true;
	}
}

QAngle CAimbot::CalcAngle(Vector& relativeDist)
{
	QAngle qAngle(
		RAD2DEG(-asinf(relativeDist.z / relativeDist.Length())),
		RAD2DEG(atan2f(relativeDist.y, relativeDist.x)),
		0.0f
	);

	return qAngle;
}

float CAimbot::GetOriginDist(Vector& a, Vector& b)
{
	return (b - a).Length();
}

float CAimbot::GetViewangleDist(QAngle& a, QAngle& b, float fOriginDistance)
{
	QAngle qDist = b - a;
	qDist.Normalize();
	float fAng = qDist.Length();

	// Causes weird angles to have giant "fovs"
	//return (sinf(DEG2RAD(fAng)) * fOriginDistance);
	return fAng;
}
