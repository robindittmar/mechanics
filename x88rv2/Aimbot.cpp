#include "Aimbot.h"
#include "Application.h"
#include "Vector.h"
#include "IEngineTrace.h"
#include "CGameTrace.h"
#include "ray.h"

CAimbot::CAimbot()
{
	// Just test values
	m_tTargetCriteria = TargetCriteriaOrigin;
	m_fSpeed = 1.0f;
}

CAimbot::~CAimbot()
{
}

void CAimbot::Setup()
{
	// Setup code here
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

// TODO: Still needs alot of reworking ;)
void CAimbot::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	// Update code here
	CApplication* pApp = CApplication::Instance();
	IVEngineClient* pEngineClient = pApp->EngineClient();
	IClientEntityList* pEntityList = pApp->EntityList();

	int iLocalPlayerIdx = pEngineClient->GetLocalPlayer();
	IClientEntity* pLocalEntity = pEntityList->GetClientEntity(iLocalPlayerIdx);
	IClientEntity* pCurEntity;
	QAngle qAimAngles, qLocalViewAngles = pApp->ClientViewAngles();
	CUserCmd* pUserCmd = (CUserCmd*)pParameters;

	if (!pUserCmd)
		return;

	if (!pLocalEntity)
		return;

	if (!ENABLE_AUTOSHOOT && !(pUserCmd->buttons & IN_ATTACK))
		return;

	// Get position + add relative eye position
	Vector myHeadPos = *(Vector*)((DWORD)pLocalEntity + 0x134);
	myHeadPos += *(Vector*)((DWORD)pLocalEntity + 0x104);

	int localTeam = *(int*)((DWORD)pLocalEntity + TEAM_OFFSET);

	Ray_t ray;
	trace_t trace;
	CTraceFilterSkipEntity traceFilter(pLocalEntity);

	// Start at i=1 since 0 is usually (always as of now) the local player
	int iMaxEntities = pEntityList->GetMaxEntities();
	for (int i = 1; i < iMaxEntities; i++)
	{
		pCurEntity = pEntityList->GetClientEntity(i);

		// TODO: Check if entity invincible (after spawn in warmup for example)

		// Filter entites
		if (!pCurEntity)
			continue;

		// Self-killing aimbot :D
		if (i == iLocalPlayerIdx)
			continue;

		// Skip dormant entities
		if (pCurEntity->IsDormant())
			continue;

		int health = *(int*)((DWORD)pCurEntity + HEALTH_OFFSET);
		if (health == 0)
			continue;

		//todo: check if knife or nades

		int entityTeam = *(int*)((DWORD)pCurEntity + TEAM_OFFSET);
		if (entityTeam == localTeam || entityTeam != 2 && entityTeam != 3)
			continue;

		// Bone ID: 8 (maybe 7=neck)
		// 10 ca chest
		//studiohdr_t* pModel = pApp->ModelInfo()->GetStudioModel(pCurEntity->GetModel());

		// TODO
		int boneIdx = 8;
		mat3x4* boneMatrix = (mat3x4*)((*(DWORD*)((DWORD)pCurEntity + 0x2698)) + (0x30 * boneIdx));
		Vector headPos;
		headPos.x = boneMatrix->c[0][3];
		headPos.y = boneMatrix->c[1][3];
		headPos.z = boneMatrix->c[2][3];

		// IsVisible check
		ray.Init(myHeadPos, headPos);
		pApp->EngineTrace()->TraceRay(ray, 0x4600400B, &traceFilter, &trace);
		if (!trace.IsVisible())
			continue;

		// Get relative position to ourselves
		headPos -= myHeadPos;
		float vecLen = sqrtf(headPos.x * headPos.x + headPos.y * headPos.y + headPos.z * headPos.z);

		// Calculate our viewangles to aim at the enemy
		QAngle aimAngles(
			-asinf(headPos.z / vecLen),
			atan2f(headPos.y, headPos.x),
			0.0f
		);

		// Get the angles in degrees (for the game)
		aimAngles.x = RAD2DEG(aimAngles.x);
		aimAngles.y = RAD2DEG(aimAngles.y);


		pApp->m_bAimbotNoRecoil = true;

		QAngle aimPunchAngle = *(QAngle*)((DWORD)pLocalEntity + (LOCAL_OFFSET + AIMPUNCHANGLE_OFFSET));

		if (!ENABLE_SILENTAIM)
		{
			pApp->ClientViewAngles(aimAngles);
		}

		aimAngles.x -= aimPunchAngle.x * RECOIL_COMPENSATION;
		aimAngles.y -= aimPunchAngle.y * RECOIL_COMPENSATION;

		pApp->m_oldAimPunchAngle.x = aimPunchAngle.x * RECOIL_COMPENSATION;
		pApp->m_oldAimPunchAngle.y = aimPunchAngle.y * RECOIL_COMPENSATION;

		pUserCmd->viewangles[0] = aimAngles.x;
		pUserCmd->viewangles[1] = aimAngles.y;


		if (ENABLE_AUTOSHOOT && false) //todo: check if issniper
		{
			if (ENABLE_AUTOSCOPE)
			{
				if (*(bool*)((DWORD)pLocalEntity + ISSCOPED_OFFSET))
				{
					pUserCmd->buttons |= IN_ATTACK;
				}
				else
				{
					pUserCmd->buttons |= IN_ATTACK2;
				}
			}
		}
		else if (ENABLE_AUTOSHOOT) // todo check if !issniper
		{
			pUserCmd->buttons |= IN_ATTACK;
		}

		break;
	}
}
