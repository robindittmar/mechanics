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

void CAimbot::Update(void* pParameters)
{
	// Update code here
	CApplication* pApp = CApplication::Instance();

	IVEngineClient* pEngineClient = pApp->EngineClient();
	IClientEntityList* pEntityList = pApp->EntityList();

	IClientEntity* pLocalEntity = pEntityList->GetClientEntity(pEngineClient->GetLocalPlayer());
	IClientEntity* pCurEntity;

	// Start at i=1 since 0 is usually (always as of now) the local player
	int iMaxEntities = pEntityList->GetMaxEntities();
	for (int i = 1; i < iMaxEntities; i++)
	{
		pCurEntity = pEntityList->GetClientEntity(i);

		// Filter entites
		if (!pCurEntity)
			continue;

		// is dormant?
		// same team?
		// hp > 0?
		// is visible?
		// ----------
		// => Aim
	}
}
