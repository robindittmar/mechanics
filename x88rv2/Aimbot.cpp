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
	CUserCmd* pUserCmd = (CUserCmd*)pParameters;

	if (!pUserCmd)
		return;

	if (!pLocalEntity)
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

		int entityTeam = *(int*)((DWORD)pCurEntity + TEAM_OFFSET);
		if (entityTeam == localTeam || entityTeam != 2 && entityTeam != 3)
			continue;

		// Bone ID: 8 (maybe 7=neck)
		// 10 ca chest
		//studiohdr_t* pModel = pApp->ModelInfo()->GetStudioModel(pCurEntity->GetModel());

		// TODO
		int boneIdx = 12;
		mat3x4* boneMatrix = (mat3x4*)((*(DWORD*)((DWORD)pCurEntity + 0x2698)) + (0x30 * boneIdx));
		Vector headPos;
		headPos.x = boneMatrix->c[0][3];
		headPos.y = boneMatrix->c[1][3];
		headPos.z = boneMatrix->c[2][3];

		// IS VISIBLE?
		ray.Init(myHeadPos, headPos);
		pApp->EngineTrace()->TraceRay(ray, 0x4600400B, &traceFilter, &trace);

		if (!trace.IsVisible())
			continue;

		// IS VISIBLE?

		//Vector headPos = *(Vector*)((DWORD)pCurEntity + 0x134);
		headPos -= myHeadPos;

		float vecLen = sqrtf(headPos.x * headPos.x + headPos.y * headPos.y + headPos.z * headPos.z);

		float yaw = atan2f(headPos.y, headPos.x);
		float pitch = -asinf(headPos.z / vecLen);

		yaw = (yaw * 180.0f) / 3.1415f;
		pitch = (pitch * 180.0f) / 3.1415f;


		// Normalize angles
		while (yaw > 179.9999f)
		{
			yaw -= 360.0f;
		}
		while (yaw < -179.9999f)
		{
			yaw += 360.0f;
		}

		while (pitch > 89.0f)
		{
			pitch -= 178.0f;
		}
		while (pitch < -89.0f)
		{
			pitch += 178.0f;
		}

		//pEngineClient->SetViewAngles(ang);
		//pUserCmd->viewangles[0] = pitch;
		//if (pUserCmd->buttons & IN_ATTACK)
		//{
			pApp->m_bAimbotNoRecoil = true;

			pApp->EngineClient()->GetViewAngles(pApp->m_viewAngle);
			QAngle aimPunchAngle = *(QAngle*)((DWORD)pLocalEntity + (LOCAL_OFFSET + AIMPUNCHANGLE_OFFSET));

			pitch -= aimPunchAngle.x * RECOIL_COMPENSATION;
			yaw -= aimPunchAngle.y * RECOIL_COMPENSATION;

			pApp->m_oldAimPunchAngle.x = aimPunchAngle.x * RECOIL_COMPENSATION;
			pApp->m_oldAimPunchAngle.y = aimPunchAngle.y * RECOIL_COMPENSATION;

			pUserCmd->viewangles[0] = pitch;
			pUserCmd->viewangles[1] = yaw;

			pUserCmd->buttons |= IN_ATTACK;

			if (!ENABLE_SILENTAIM)
			{
				pApp->m_bSetClientViewAngles = true;
			}
		//}

		break;
	}
}
