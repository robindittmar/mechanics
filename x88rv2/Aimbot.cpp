#include "Aimbot.h"
#include "Application.h"

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

		// TODO
		int boneIdx = 8;
		mat3x4* boneMatrix = (mat3x4*)((*(DWORD*)((DWORD)pCurEntity + 0x2698)) + (0x30 * boneIdx));
		Vector headPos;
		headPos.x = boneMatrix->c[0][3];
		headPos.y = boneMatrix->c[1][3];
		headPos.z = boneMatrix->c[2][3];

		//Vector headPos = *(Vector*)((DWORD)pCurEntity + 0x134);
		headPos -= myHeadPos;

		float vecLen = sqrtf(headPos.x * headPos.x + headPos.y * headPos.y + headPos.z * headPos.z);

		float yaw = atan2f(headPos.y, headPos.x);
		float pitch = -asinf(headPos.z / vecLen);

		yaw = (yaw * 180.0f) / 3.1415f;
		pitch = (pitch * 180.0f) / 3.1415f;

		
		// Normalize angles
		while (yaw > 180.0f)
		{
			yaw -= 360.0f;
		}
		while (yaw < -180.0f)
		{
			yaw += 360.0f;
		}

		while (pitch > 90.0f)
		{
			pitch -= 180.0f;
		}
		while (pitch < -90.0f)
		{
			pitch += 180.0f;
		}

		//pEngineClient->SetViewAngles(ang);
		//pUserCmd->viewangles[0] = pitch;
		if (pUserCmd->buttons & IN_ATTACK)
		{
			pUserCmd->viewangles[0] = pitch;
			pUserCmd->viewangles[1] = yaw;
			if (false) //todo: check if silentaim
			{
				pApp->m_bSetClientViewAngles = true;
			}
		}
		
		break;
		// is dormant?
		// same team?
		// hp > 0?
		// is visible?
		// ----------
		// => Aim
	}
}
