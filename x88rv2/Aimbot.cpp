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

	// TODO
	*m_pApp->m_bSendPackets = true;
	m_bIsShooting = false;
	m_bDidNoRecoil = false;

	// Update code here
	IVEngineClient* pEngineClient = m_pApp->EngineClient();
	IClientEntityList* pEntityList = m_pApp->EntityList();

	int iLocalPlayerIdx = pEngineClient->GetLocalPlayer();
	IClientEntity* pLocalEntity = pEntityList->GetClientEntity(iLocalPlayerIdx);
	IClientEntity* pCurEntity;
	QAngle qAimAngles, qLocalViewAngles = m_pApp->ClientViewAngles();
	CUserCmd* pUserCmd = (CUserCmd*)pParameters;

	if (!pUserCmd)
		return;

	if (!pLocalEntity)
		return;

	CWeapon* pActiveWeapon = (CWeapon*)pLocalEntity->ActiveWeapon();
	if (pActiveWeapon->IsKnife() ||
		pActiveWeapon->IsNade() ||
		pActiveWeapon->IsC4() ||
		pActiveWeapon->Clip1() == 0)
		return;

	if (!this->m_bAutoshoot && !(pUserCmd->buttons & IN_ATTACK))
		return;

	// Get position + add relative eye position
	Vector myHeadPos = *pLocalEntity->Origin();
	myHeadPos += *pLocalEntity->EyeOffset();
	int localTeam = pLocalEntity->TeamNum();

	Vector targetPos;
	QAngle targetAngles;

	QAngle aimAngles;
	Ray_t ray;
	trace_t trace;
	CTraceFilterSkipEntity traceFilter(pLocalEntity);

	int iSelectedEntity = -1;
	float fViewangleDist;
	float fOriginDist;
	float fLowestDist = 999999.0f;

	// Start at i=1 since 0 is usually (always as of now) the local player
	int iMaxEntities = pEntityList->GetMaxEntities();
	for (int i = 1; i < iMaxEntities; i++)
	{
		pCurEntity = pEntityList->GetClientEntity(i);

		// TODO: Check if entity invincible (after spawn in warmup for example)

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
		if (entityTeam == localTeam || entityTeam != 2 && entityTeam != 3)
			continue;

		// Spawn protection
		if (pCurEntity->IsInvincible())
			continue;

		// Bone ID: 8 (maybe 7=neck)
		// 10 ca chest
		//studiohdr_t* pModel = m_pApp->ModelInfo()->GetStudioModel(pCurEntity->GetModel());

		// TODO
		int boneIdx = 8;
		mat3x4* boneMatrix = (mat3x4*)((*(DWORD*)((DWORD)pCurEntity + 0x2698)) + (0x30 * boneIdx));
		Vector headPos;
		headPos.x = boneMatrix->c[0][3];
		headPos.y = boneMatrix->c[1][3];
		headPos.z = boneMatrix->c[2][3];

		// IsVisible check
		ray.Init(myHeadPos, headPos);
		m_pApp->EngineTrace()->TraceRay(ray, 0x4600400B, &traceFilter, &trace);
		if (!trace.IsVisible())
			continue;

		switch (m_tTargetCriteria)
		{
		case TargetCriteriaOrigin:
			fOriginDist = this->GetOriginDist(myHeadPos, headPos);
			if (fOriginDist < fLowestDist)
			{
				fLowestDist = fOriginDist;

				iSelectedEntity = i;
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
			aimAngles = this->CalcAngle(headPos);

			// Calculate our fov to the enemy
			fViewangleDist = fabs(this->GetViewangleDist(qLocalViewAngles, aimAngles, fOriginDist));
			// Max FOV
			if (fViewangleDist > m_fFov)
			{
				continue;
			}
			else if (fViewangleDist < fLowestDist)
			{
				fLowestDist = fViewangleDist;

				iSelectedEntity = i;
				targetAngles = aimAngles;

				continue;
			}
			break;
		case TargetCriteriaUnspecified:
			iSelectedEntity = i;
			targetPos = headPos;

			// Force the entityloop to exit out (break would only break switch)
			i = iMaxEntities;
			break;
		}
	}

	// If we found no target just exit
	if (iSelectedEntity == -1)
		return;

	if (m_tTargetCriteria != TargetCriteriaViewangle)
	{
		// Get relative position to ourselves
		targetPos -= myHeadPos;

		// Calculate our viewangles to aim at the enemy
		aimAngles = this->CalcAngle(targetPos);
	}
	else
	{
		aimAngles = targetAngles;
	}

	// Set ClientViewAngles if we don't have silentaim activated
	if (!this->m_bSilentAim)
	{
		m_pApp->ClientViewAngles(aimAngles);
	}

	// If we have no recoil activated in the aimbot, do it
	// (and remember that we did!)
	if(m_bDoNoRecoil)
	{
		QAngle aimPunchAngle = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));
		aimAngles.x -= aimPunchAngle.x * RECOIL_COMPENSATION;
		aimAngles.y -= aimPunchAngle.y * RECOIL_COMPENSATION;

		m_pApp->m_oldAimPunchAngle.x = aimPunchAngle.x * RECOIL_COMPENSATION;
		m_pApp->m_oldAimPunchAngle.y = aimPunchAngle.y * RECOIL_COMPENSATION;

		m_bDidNoRecoil = true;
	}

	pUserCmd->viewangles[0] = aimAngles.x;
	pUserCmd->viewangles[1] = aimAngles.y;

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
	if (this->m_bIsShooting)
		*m_pApp->m_bSendPackets = false;
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

	return (sinf(DEG2RAD(fAng)) * fOriginDistance);
}
