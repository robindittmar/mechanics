#include "Chams.h"
#include "Application.h"

CChams::CChams()
{
	m_bMaterialsInitialized = false;
}

CChams::~CChams()
{
}

void CChams::Setup()
{
	m_pApp = CApplication::Instance();
}

void CChams::Update(void* pParameters)
{
}

void CChams::ReloadMaterials()
{
	m_pFlatHiddenCT->DecrementReferenceCount();
	m_pFlatVisibleCT->DecrementReferenceCount();
	m_pFlatHiddenT->DecrementReferenceCount();
	m_pFlatVisibleT->DecrementReferenceCount();

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
		m_pFlatHiddenCT = m_pApp->ResourceManager()->CreateMaterial(true, false, true);
		m_pFlatVisibleCT = m_pApp->ResourceManager()->CreateMaterial(true);
		m_pFlatHiddenT = m_pApp->ResourceManager()->CreateMaterial(true, false, true);
		m_pFlatVisibleT = m_pApp->ResourceManager()->CreateMaterial(true);

		// Colors
		m_pFlatHiddenCT->ColorModulate(0.0f, 0.0f, 1.0f);
		m_pFlatVisibleCT->ColorModulate(0.0f, 1.0f, 0.0f);
		m_pFlatHiddenT->ColorModulate(1.0f, 0.0f, 0.0f);
		m_pFlatVisibleT->ColorModulate(1.0f, 1.0f, 0.0f);

		// Z-Flag
		/*m_pFlatHiddenCT->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
		m_pFlatHiddenT->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);*/

		// Flat
		/*m_pFlatHiddenCT->SetMaterialVarFlag(MATERIAL_VAR_FLAT, false);
		m_pFlatVisibleCT->SetMaterialVarFlag(MATERIAL_VAR_FLAT, false);
		m_pFlatHiddenT->SetMaterialVarFlag(MATERIAL_VAR_FLAT, false);
		m_pFlatVisibleT->SetMaterialVarFlag(MATERIAL_VAR_FLAT, false);*/

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
			m_pModelRender->ForcedMaterialOverride(m_pFlatHiddenT);
			m_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
			m_pModelRender->ForcedMaterialOverride(m_pFlatVisibleT);
		}
		else if(iModelTeamNum == TEAMNUM_CT)
		{
			m_pModelRender->ForcedMaterialOverride(m_pFlatHiddenCT);
			m_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
			m_pModelRender->ForcedMaterialOverride(m_pFlatVisibleCT);
		}
	}
}