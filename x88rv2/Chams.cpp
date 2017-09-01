#include "Chams.h"
#include "Application.h"

CChams::CChams()
{
	m_pModelRender = NULL;
	m_pDrawModelExecute = NULL;

	m_bMaterialsInitialized = false;

	m_bRenderTeam = false;
	m_bRenderLocalplayer = false;
	m_bIgnoreZIndex = false;
	m_bFlatModels = false;

	m_pFlatHiddenCT = NULL;
	m_pFlatVisibleCT = NULL;
	m_pFlatHiddenT = NULL;
	m_pFlatVisibleT = NULL;

	m_pLitHiddenCT = NULL;
	m_pLitVisibleCT = NULL;
	m_pLitHiddenT = NULL;
	m_pLitVisibleT = NULL;

	m_pHiddenCT = NULL;
	m_pVisibleCT = NULL;
	m_pHiddenT = NULL;
	m_pVisibleT = NULL;
}

CChams::~CChams()
{
	// Only used to decrement reference count. shit won't be reinitialized,
	// since the destructor is called on getting destroyed
	this->ReloadMaterials();
}

void CChams::Setup()
{
	m_pApp = CApplication::Instance();
}

void CChams::Update(void* pParameters)
{
}

void CChams::SetFlatModels(bool bFlatModels)
{
	m_bFlatModels = bFlatModels;

	if(m_bFlatModels)
	{
		m_pHiddenCT = m_pFlatHiddenCT;
		m_pVisibleCT = m_pFlatVisibleCT;
		m_pHiddenT = m_pFlatHiddenT;
		m_pVisibleT = m_pFlatVisibleT;
	}
	else
	{
		m_pHiddenCT = m_pLitHiddenCT;
		m_pVisibleCT = m_pLitVisibleCT;
		m_pHiddenT = m_pLitHiddenT;
		m_pVisibleT = m_pLitVisibleT;
	}
}

void CChams::ReloadMaterials()
{
	if(m_pFlatHiddenCT)
		m_pFlatHiddenCT->DecrementReferenceCount();

	if (m_pFlatVisibleCT)
		m_pFlatVisibleCT->DecrementReferenceCount();

	if (m_pFlatHiddenT)
		m_pFlatHiddenT->DecrementReferenceCount();

	if (m_pFlatVisibleT)
		m_pFlatVisibleT->DecrementReferenceCount();

	if (m_pLitHiddenCT)
		m_pLitHiddenCT->DecrementReferenceCount();

	if (m_pLitVisibleCT)
		m_pLitVisibleCT->DecrementReferenceCount();

	if (m_pLitHiddenT)
		m_pLitHiddenT->DecrementReferenceCount();

	if (m_pLitVisibleT)
		m_pLitVisibleT->DecrementReferenceCount();

	m_bMaterialsInitialized = false;
}

void CChams::Render(const char* pszModelName, void* ecx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!m_bIsEnabled)
		return;

	static CXorString pModelTextures("Zdá§{+ñ§oð°rx");
	static CXorString pModelsSlashPlayers("zdá§{xª²{jü§e");

	// Create Materials
	if (!m_bMaterialsInitialized)
	{
		// Grab references
		m_pModelRender = m_pApp->ModelRender();
		m_pDrawModelExecute = m_pApp->DrawModelExecute();

		// Create materials
		m_pFlatHiddenCT = m_pApp->ResourceManager()->CreateMaterial(false, true, true);
		m_pFlatVisibleCT = m_pApp->ResourceManager()->CreateMaterial(false, true);
		m_pFlatHiddenT = m_pApp->ResourceManager()->CreateMaterial(false, true, true);
		m_pFlatVisibleT = m_pApp->ResourceManager()->CreateMaterial(false, true);

		m_pLitHiddenCT = m_pApp->ResourceManager()->CreateMaterial(true, false, true);
		m_pLitVisibleCT = m_pApp->ResourceManager()->CreateMaterial(true, false);
		m_pLitHiddenT = m_pApp->ResourceManager()->CreateMaterial(true, false, true);
		m_pLitVisibleT = m_pApp->ResourceManager()->CreateMaterial(true, false);

		// Colors
		m_pFlatHiddenCT->ColorModulate(0.0f, 0.0f, 1.0f);
		m_pFlatVisibleCT->ColorModulate(0.0f, 1.0f, 0.0f);
		m_pFlatHiddenT->ColorModulate(1.0f, 0.0f, 0.0f);
		m_pFlatVisibleT->ColorModulate(1.0f, 1.0f, 0.0f);

		m_pLitHiddenCT->ColorModulate(0.0, 0.0, 1.0f);
		m_pLitVisibleCT->ColorModulate(0.0f, 1.0f, 0.0f);
		m_pLitHiddenT->ColorModulate(1.0f, 0.0f, 0.0f);
		m_pLitVisibleT->ColorModulate(1.0f, 1.0f, 0.0f);

		// Force Chams to actually "load" into the pointers
		this->SetFlatModels(m_bFlatModels);

		// Don't do this again :)
		m_bMaterialsInitialized = true;
	}

	// models/player
	if (strstr(pszModelName, pModelsSlashPlayers.ToCharArray()) != NULL)
	{
		IClientEntity* pLocalEntity = (IClientEntity*)m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
		IClientEntity* pModelEntity = m_pApp->EntityList()->GetClientEntity(pInfo.entity_index);

		if (!pModelEntity)
			return;

		if (!pModelEntity->IsAlive())
			return;

		bool bIsLocalPlayer = pLocalEntity == pModelEntity;
		int iLocalTeam = pLocalEntity->GetTeamNum();
		int iModelTeamNum = pModelEntity->GetTeamNum();

		if (!(bIsLocalPlayer && m_pApp->Visuals()->GetThirdperson() && m_bRenderLocalplayer ||
			!bIsLocalPlayer && m_bRenderTeam && iModelTeamNum == iLocalTeam ||
			iModelTeamNum != iLocalTeam) ||
			iModelTeamNum == 0)
			return;

		if(iModelTeamNum == TEAMNUM_T)
		{
			if(m_bIgnoreZIndex)
			{
				m_pModelRender->ForcedMaterialOverride(m_pHiddenT);
				m_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
			}
			m_pModelRender->ForcedMaterialOverride(m_pVisibleT);
		}
		else if(iModelTeamNum == TEAMNUM_CT)
		{
			if(m_bIgnoreZIndex)
			{
				m_pModelRender->ForcedMaterialOverride(m_pHiddenCT);
				m_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
			}
			m_pModelRender->ForcedMaterialOverride(m_pVisibleCT);
		}
	}
}