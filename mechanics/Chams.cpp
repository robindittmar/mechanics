#include "Chams.h"
#include "Application.h"

CChams::CChams()
{
	m_pModelRender = NULL;
	m_pDrawModelExecute = NULL;

	m_bMaterialsInitialized = false;
	m_bFakeAngleMaterialsInitialized = false;

	m_bRenderTeam = false;
	m_bRenderLocalplayer = false;
	m_bOnlyVisible = false;
	m_iPlayerChamsStyle = PLAYER_CHAMSSTYLE_NONE;
	m_bRenderFakeAngle = false;

	m_iWeaponChamsStyle = WEAPON_CHAMSSTYLE_NONE;

	m_pFlatHiddenCT = NULL;
	m_pFlatVisibleCT = NULL;
	m_pFlatHiddenT = NULL;
	m_pFlatVisibleT = NULL;
	m_pFlatFakeAngle = NULL;

	m_pLitHiddenCT = NULL;
	m_pLitVisibleCT = NULL;
	m_pLitHiddenT = NULL;
	m_pLitVisibleT = NULL;
	m_pLitFakeAngle = NULL;

	m_pHiddenCT = NULL;
	m_pVisibleCT = NULL;
	m_pHiddenT = NULL;
	m_pVisibleT = NULL;
	m_pFakeAngle = NULL;
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

void CChams::SetFlatModels(int iPlayerChamsStyle)
{
	m_iPlayerChamsStyle = iPlayerChamsStyle;

	switch (m_iPlayerChamsStyle)
	{
	case PLAYER_CHAMSSTYLE_FLAT:
		m_pHiddenCT = m_pFlatHiddenCT;
		m_pVisibleCT = m_pFlatVisibleCT;
		m_pHiddenT = m_pFlatHiddenT;
		m_pVisibleT = m_pFlatVisibleT;

		m_pFakeAngle = m_pFlatFakeAngle;
		break;
	case PLAYER_CHAMSSTYLE_LIT:
		m_pHiddenCT = m_pLitHiddenCT;
		m_pVisibleCT = m_pLitVisibleCT;
		m_pHiddenT = m_pLitHiddenT;
		m_pVisibleT = m_pLitVisibleT;

		m_pFakeAngle = m_pLitFakeAngle;
		break;
	case PLAYER_CHAMSSTYLE_NONE:
	default:
		break;
	}
}

void CChams::ReloadMaterials()
{
	if (m_pFlatHiddenCT)
		m_pFlatHiddenCT->DecrementReferenceCount();

	if (m_pFlatVisibleCT)
		m_pFlatVisibleCT->DecrementReferenceCount();

	if (m_pFlatHiddenT)
		m_pFlatHiddenT->DecrementReferenceCount();

	if (m_pFlatVisibleT)
		m_pFlatVisibleT->DecrementReferenceCount();

	if (m_pFlatFakeAngle)
		m_pFlatFakeAngle->DecrementReferenceCount();

	if (m_pLitHiddenCT)
		m_pLitHiddenCT->DecrementReferenceCount();

	if (m_pLitVisibleCT)
		m_pLitVisibleCT->DecrementReferenceCount();

	if (m_pLitHiddenT)
		m_pLitHiddenT->DecrementReferenceCount();

	if (m_pLitVisibleT)
		m_pLitVisibleT->DecrementReferenceCount();

	if (m_pLitFakeAngle)
		m_pLitFakeAngle->DecrementReferenceCount();

	m_bMaterialsInitialized = false;
	m_bFakeAngleMaterialsInitialized = false;
}

void CChams::DrawFakeAngle(void* ecx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bRenderFakeAngle)
		return;

	if (!m_pApp->Visuals()->GetThirdperson())
		return;

	if (pInfo.entity_index < 0 || pInfo.entity_index > m_pApp->GlobalVars()->maxClients)
		return;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	IClientEntity* pRenderEntity = m_pApp->EntityList()->GetClientEntity(pInfo.entity_index);

	if (!pLocalEntity || !pRenderEntity)
		return;

	if (pLocalEntity != pRenderEntity)
		return;

	/*if (!m_pApp->Visuals()->GetThirdperson())
		return;*/

	if (!m_pApp->AntiAim()->IsFakeYaw())
		return;

	if (m_pApp->AntiAim()->IsLbyUpdate())
		return;

	if (!m_bFakeAngleMaterialsInitialized)
	{
		// Grab references
		m_pModelRender = m_pApp->ModelRender();
		m_pDrawModelExecute = m_pApp->DrawModelExecute();

		// Create materials
		m_pFlatFakeAngle = m_pApp->ResourceManager()->CreateMaterial(false, true);
		m_pLitFakeAngle = m_pApp->ResourceManager()->CreateMaterial(true, false);

		// Colors
		m_pFlatFakeAngle->AlphaModulate(0.7f);
		m_pFlatFakeAngle->ColorModulate(1.0f, 1.0f, 1.0f);
		m_pLitFakeAngle->AlphaModulate(0.7f);
		m_pLitFakeAngle->ColorModulate(1.0f, 1.0f, 1.0f);

		// Force Chams to actually "load" into the pointers
		this->SetFlatModels(m_iPlayerChamsStyle);

		m_bFakeAngleMaterialsInitialized = true;
	}

	matrix3x4_t mRotation;
	matrix3x4_t mCurBone;
	matrix3x4_t mCurBoneModified;
	matrix3x4_t pBoneMatrix[MAXSTUDIOBONES];
	Vector vCurPos, vCurPosRotated;

	QAngle qRotationAngles = QAngle(0, m_pApp->AntiAim()->GetFakeYaw() - m_pApp->AntiAim()->GetRealYaw(), 0);
	QAngle qAngles;

	// Vector2Matrix
	AngleMatrix(qRotationAngles, mRotation);
	for (int i = 0; i < MAXSTUDIOBONES; i++)
	{
		// Copy original matrix
		MatrixCopy(pCustomBoneToWorld[i], mCurBone);

		// Get Position and subtract current origin
		MatrixGetColumn(mCurBone, 3, vCurPos);
		vCurPos -= pInfo.origin;

		// Transform vector
		VectorTransform(vCurPos, mRotation, vCurPosRotated);

		// Add origin back to rotated vector
		vCurPosRotated += pInfo.origin;

		// Set position in saved matrix to 0
		MatrixSetColumn(Vector(0, 0, 0), 3, mCurBone);

		// Multiply Matrix
		ConcatTransforms(mRotation, mCurBone, mCurBoneModified);

		// Matrix2Vector
		MatrixAngles(mCurBoneModified, (float*)&qAngles);

		// Vector2Matrix
		AngleMatrix(qAngles, vCurPosRotated, pBoneMatrix[i]);
	}

	m_pModelRender->ForcedMaterialOverride(m_pFakeAngle);
	m_pDrawModelExecute(ecx, ctx, state, pInfo, pBoneMatrix);
	m_pModelRender->ForcedMaterialOverride(NULL);
}

void CChams::RenderPlayerChams(const char* pszModelName, void* ecx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!m_bIsEnabled)
		return;

	if (m_iPlayerChamsStyle == PLAYER_CHAMSSTYLE_NONE)
		return;

	static CXorString pModelTextures("Zd�{+�o�rx");
	static CXorString pModelsSlashPlayers("zd�{x��{j��e");

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
		m_pFlatHiddenCT->ColorModulate(m_clrHiddenCT.r() / 255.0f, m_clrHiddenCT.g() / 255.0f, m_clrHiddenCT.b() / 255.0f);
		m_pFlatVisibleCT->ColorModulate(m_clrVisibleCT.r() / 255.0f, m_clrVisibleCT.g() / 255.0f, m_clrVisibleCT.b() / 255.0f);
		m_pFlatHiddenT->ColorModulate(m_clrHiddenT.r() / 255.0f, m_clrHiddenT.g() / 255.0f, m_clrHiddenT.b() / 255.0f);
		m_pFlatVisibleT->ColorModulate(m_clrVisibleT.r() / 255.0f, m_clrVisibleT.g() / 255.0f, m_clrVisibleT.b() / 255.0f);

		m_pLitHiddenCT->ColorModulate(m_clrHiddenCT.r() / 255.0f, m_clrHiddenCT.g() / 255.0f, m_clrHiddenCT.b() / 255.0f);
		m_pLitVisibleCT->ColorModulate(m_clrVisibleCT.r() / 255.0f, m_clrVisibleCT.g() / 255.0f, m_clrVisibleCT.b() / 255.0f);
		m_pLitHiddenT->ColorModulate(m_clrHiddenT.r() / 255.0f, m_clrHiddenT.g() / 255.0f, m_clrHiddenT.b() / 255.0f);
		m_pLitVisibleT->ColorModulate(m_clrVisibleT.r() / 255.0f, m_clrVisibleT.g() / 255.0f, m_clrVisibleT.b() / 255.0f);

		// Force Chams to actually "load" into the pointers
		this->SetFlatModels(m_iPlayerChamsStyle);

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

		if (iModelTeamNum == TEAMNUM_T)
		{
			if (!m_bOnlyVisible)
			{
				m_pModelRender->ForcedMaterialOverride(m_pHiddenT);
				m_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
			}
			m_pModelRender->ForcedMaterialOverride(m_pVisibleT);
		}
		else if (iModelTeamNum == TEAMNUM_CT)
		{
			if (!m_bOnlyVisible)
			{
				m_pModelRender->ForcedMaterialOverride(m_pHiddenCT);
				m_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
			}
			m_pModelRender->ForcedMaterialOverride(m_pVisibleCT);
		}
	}
}

void CChams::RenderWeaponChams(const char* pszModelName, void* ecx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!m_bIsEnabled)
		return;

	if (m_iWeaponChamsStyle == WEAPON_CHAMSSTYLE_NONE)
		return;

	static CXorString xorWeaponsV("`n�xe��a");
	static CXorString xorWeaponsVModels("zd�{x��rj��yx��Hf�rg��");
	static CXorString xorOtherTextures("X�e+�o�rx");
	static CXorString xorGoldMat("zd�{x��y}�cd��Hb�zx��ed��nT�}d��8l�s");
	static CXorString xorCrystalMat("zd�{x��y}�cd��Hb�zx��ed��nT�}d��8h��d�Hi�r");
	static CXorString xorPlatMat("zd�{x��{j��e$�Hm�8h�qi�pg�d");
	static CXorString xorGlassMat("zd�{x��y}�cd��Hb�zx��ed��nT�}d��8h��d�Hh�vy");


	if (strstr(pszModelName, xorWeaponsV.ToCharArray()) != NULL &&
		strstr(pszModelName, xorWeaponsVModels.ToCharArray()) == NULL)
	{
		IMaterial* pMat;
		switch (m_iWeaponChamsStyle)
		{
		case WEAPON_CHAMSSTYLE_GOLD:
			pMat = m_pApp->MaterialSystem()->FindMaterial(xorGoldMat.ToCharArray(), xorOtherTextures.ToCharArray());
			break;
		case WEAPON_CHAMSSTYLE_CRYSTAL:
			pMat = m_pApp->MaterialSystem()->FindMaterial(xorCrystalMat.ToCharArray(), xorOtherTextures.ToCharArray());
			break;
		case WEAPON_CHAMSSTYLE_PLATINUM:
			pMat = m_pApp->MaterialSystem()->FindMaterial(xorPlatMat.ToCharArray(), xorOtherTextures.ToCharArray());
			break;
		case WEAPON_CHAMSSTYLE_GLASS:
			pMat = m_pApp->MaterialSystem()->FindMaterial(xorGlassMat.ToCharArray(), xorOtherTextures.ToCharArray());
			break;
		case WEAPON_CHAMSSTYLE_NONE:
		default:
			break;
		}

		if (pMat)
		{
			m_pApp->ModelRender()->ForcedMaterialOverride(pMat);
			m_pApp->DrawModelExecute()(ecx, ctx, state, pInfo, pCustomBoneToWorld);
		}
	}
}
