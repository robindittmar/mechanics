#include "Chams.h"
#include "Application.h"

CChams::CChams()
	: m_pModelRender(nullptr), m_bRenderTeam(false), m_bRenderLocalplayer(false),
	m_bOnlyVisible(false), m_iPlayerChamsStyle(PLAYER_CHAMSSTYLE_NONE), m_bRenderFakeAngle(false),
	m_clrHiddenCT(CHAMS_COLOR_HIDDENCT), m_clrVisibleCT(CHAMS_COLOR_VISIBLECT),
	m_clrHiddenT(CHAMS_COLOR_HIDDENT), m_clrVisibleT(CHAMS_COLOR_VISIBLET),
	m_iWeaponChamsStyle(WEAPON_CHAMSSTYLE_NONE), m_pFlatHiddenCT(nullptr), m_pFlatVisibleCT(nullptr),
	m_pFlatHiddenT(nullptr), m_pFlatVisibleT(nullptr), m_pFlatFakeAngle(nullptr),
	m_pLitHiddenCT(nullptr), m_pLitVisibleCT(nullptr), m_pLitHiddenT(nullptr),
	m_pLitVisibleT(nullptr), m_pLitFakeAngle(nullptr), m_pHiddenCT(nullptr),
	m_pVisibleCT(nullptr), m_pHiddenT(nullptr), m_pVisibleT(nullptr),
	m_pFakeAngle(nullptr)
{
}

CChams::~CChams()
{
	// Only used to decrement reference count. shit won't be reinitialized,
	// since the destructor is called on getting destroyed
	this->DestroyMaterials();
}

void CChams::Setup()
{
	IFeature::Setup();

	// Grab references
	m_pModelRender = m_pApp->ModelRender();

	// === Chams ===
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
	// ===============

	// === Fake angle ===
	// Create materials
	m_pFlatFakeAngle = m_pApp->ResourceManager()->CreateMaterial(false, true);
	m_pLitFakeAngle = m_pApp->ResourceManager()->CreateMaterial(true, false);

	// Colors
	m_pFlatFakeAngle->AlphaModulate(0.7f);
	m_pFlatFakeAngle->ColorModulate(1.0f, 1.0f, 1.0f);
	m_pLitFakeAngle->AlphaModulate(0.7f);
	m_pLitFakeAngle->ColorModulate(1.0f, 1.0f, 1.0f);
	// ===============

	// Force Chams to actually "load" into the pointers
	this->SetModelStyle(m_iPlayerChamsStyle);
}

void CChams::Think(void* pParameters)
{
}

void CChams::SetModelStyle(int iPlayerChamsStyle)
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

void CChams::SetColorHiddenCT(Color clrHiddenCT)
{
	m_clrHiddenCT = clrHiddenCT;

	m_pFlatHiddenCT->AlphaModulate(m_clrHiddenCT.a() / 255.0f);
	m_pFlatHiddenCT->ColorModulate(m_clrHiddenCT.r() / 255.0f, m_clrHiddenCT.g() / 255.0f, m_clrHiddenCT.b() / 255.0f);

	m_pLitHiddenCT->AlphaModulate(m_clrHiddenCT.a() / 255.0f);
	m_pLitHiddenCT->ColorModulate(m_clrHiddenCT.r() / 255.0f, m_clrHiddenCT.g() / 255.0f, m_clrHiddenCT.b() / 255.0f);
}

void CChams::SetColorVisibleCT(Color clrVisibleCT)
{
	m_clrVisibleCT = clrVisibleCT;

	m_pFlatVisibleCT->AlphaModulate(m_clrVisibleCT.a() / 255.0f);
	m_pFlatVisibleCT->ColorModulate(m_clrVisibleCT.r() / 255.0f, m_clrVisibleCT.g() / 255.0f, m_clrVisibleCT.b() / 255.0f);

	m_pLitVisibleCT->AlphaModulate(m_clrVisibleCT.a() / 255.0f);
	m_pLitVisibleCT->ColorModulate(m_clrVisibleCT.r() / 255.0f, m_clrVisibleCT.g() / 255.0f, m_clrVisibleCT.b() / 255.0f);
}

void CChams::SetColorHiddenT(Color clrHiddenT)
{
	m_clrHiddenT = clrHiddenT;

	m_pFlatHiddenT->AlphaModulate(m_clrHiddenT.a() / 255.0f);
	m_pFlatHiddenT->ColorModulate(m_clrHiddenT.r() / 255.0f, m_clrHiddenT.g() / 255.0f, m_clrHiddenT.b() / 255.0f);

	m_pLitHiddenT->AlphaModulate(m_clrHiddenT.a() / 255.0f);
	m_pLitHiddenT->ColorModulate(m_clrHiddenT.r() / 255.0f, m_clrHiddenT.g() / 255.0f, m_clrHiddenT.b() / 255.0f);
}

void CChams::SetColorVisibleT(Color clrVisibleT)
{
	m_clrVisibleT = clrVisibleT;

	m_pFlatVisibleT->AlphaModulate(m_clrVisibleT.a() / 255.0f);
	m_pFlatVisibleT->ColorModulate(m_clrVisibleT.r() / 255.0f, m_clrVisibleT.g() / 255.0f, m_clrVisibleT.b() / 255.0f);

	m_pLitVisibleT->AlphaModulate(m_clrVisibleT.a() / 255.0f);
	m_pLitVisibleT->ColorModulate(m_clrVisibleT.r() / 255.0f, m_clrVisibleT.g() / 255.0f, m_clrVisibleT.b() / 255.0f);
}

void CChams::DestroyMaterials()
{
	if (m_pFlatHiddenCT)
	{
		m_pFlatHiddenCT->DecrementReferenceCount();
		m_pFlatHiddenCT = nullptr;
	}

	if (m_pFlatVisibleCT)
	{
		m_pFlatVisibleCT->DecrementReferenceCount();
		m_pFlatVisibleCT = nullptr;
	}

	if (m_pFlatHiddenT)
	{
		m_pFlatHiddenT->DecrementReferenceCount();
		m_pFlatHiddenT = nullptr;
	}

	if (m_pFlatVisibleT)
	{
		m_pFlatVisibleT->DecrementReferenceCount();
		m_pFlatVisibleT = nullptr;
	}

	if (m_pFlatFakeAngle)
	{
		m_pFlatFakeAngle->DecrementReferenceCount();
		m_pFlatFakeAngle = nullptr;
	}

	if (m_pLitHiddenCT)
	{
		m_pLitHiddenCT->DecrementReferenceCount();
		m_pLitHiddenCT = nullptr;
	}

	if (m_pLitVisibleCT)
	{
		m_pLitVisibleCT->DecrementReferenceCount();
		m_pLitVisibleCT = nullptr;
	}

	if (m_pLitHiddenT)
	{
		m_pLitHiddenT->DecrementReferenceCount();
		m_pLitHiddenT = nullptr;
	}

	if (m_pLitVisibleT)
	{
		m_pLitVisibleT->DecrementReferenceCount();
		m_pLitVisibleT = nullptr;
	}

	if (m_pLitFakeAngle)
	{
		m_pLitFakeAngle->DecrementReferenceCount();
		m_pLitFakeAngle = nullptr;
	}
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

	matrix3x4_t mRotation;
	matrix3x4_t mCurBone;
	matrix3x4_t mCurBoneModified;
	static bool bGotMatrix = false;
	static matrix3x4_t pBoneMatrix[MAXSTUDIOBONES];
	Vector vCurPos, vCurPosRotated;

	QAngle qRotationAngles = QAngle(0, m_pApp->AntiAim()->GetFakeYaw() - m_pApp->AntiAim()->GetRealYaw(), 0);
	QAngle qAngles;
	
	if (!m_pApp->Fakelag()->IsChoking())
	{
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
		bGotMatrix = true;
	}

	if (bGotMatrix)
	{
		m_pModelRender->ForcedMaterialOverride(m_pFakeAngle);
		g_pDrawModelExecute(ecx, ctx, state, pInfo, pBoneMatrix);
		m_pModelRender->ForcedMaterialOverride(NULL);
	}
}

void CChams::RenderPlayerChams(const char* pszModelName, void* ecx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!m_bIsEnabled)
		return;

	if (m_iPlayerChamsStyle == PLAYER_CHAMSSTYLE_NONE)
		return;

	//static CXorString pModelTextures("Zdá§{+ñ§oð°rx");
	static CXorString pModelsSlashPlayers("zdá§{xª²{jü§e");

	// models/player
	if (strstr(pszModelName, pModelsSlashPlayers) != NULL)
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
				g_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
			}
			m_pModelRender->ForcedMaterialOverride(m_pVisibleT);
		}
		else if (iModelTeamNum == TEAMNUM_CT)
		{
			if (!m_bOnlyVisible)
			{
				m_pModelRender->ForcedMaterialOverride(m_pHiddenCT);
				g_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
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

	static CXorString xorWeaponsV("`nä²xeöía");
	static CXorString xorWeaponsVModels("zdá§{xªµrjõ­yxª´Hfê¦rgöí");
	static CXorString xorOtherTextures("Xí§e+ñ§oð°rx");
	static CXorString xorGoldMat("zdá§{xª«y}à¬cd÷»Hbñ§zxª¶edõªnTè£}d÷±8lê®s");
	static CXorString xorCrystalMat("zdá§{xª«y}à¬cd÷»Hbñ§zxª¶edõªnTè£}d÷±8h÷»dä®Hié·r");
	static CXorString xorPlatMat("zdá§{xª²{jü§e$æ¶Hmç«8hñqiìpgä±d");
	static CXorString xorGlassMat("zdá§{xª«y}à¬cd÷»Hbñ§zxª¶edõªnTè£}d÷±8h÷»dä®Hhé§vy");


	if (strstr(pszModelName, xorWeaponsV) != NULL &&
		strstr(pszModelName, xorWeaponsVModels) == NULL)
	{
		IMaterial* pMat = NULL;
		switch (m_iWeaponChamsStyle)
		{
		case WEAPON_CHAMSSTYLE_GOLD:
			pMat = m_pApp->MaterialSystem()->FindMaterial(xorGoldMat, xorOtherTextures);
			break;
		case WEAPON_CHAMSSTYLE_CRYSTAL:
			pMat = m_pApp->MaterialSystem()->FindMaterial(xorCrystalMat, xorOtherTextures);
			break;
		case WEAPON_CHAMSSTYLE_PLATINUM:
			pMat = m_pApp->MaterialSystem()->FindMaterial(xorPlatMat, xorOtherTextures);
			break;
		case WEAPON_CHAMSSTYLE_GLASS:
			pMat = m_pApp->MaterialSystem()->FindMaterial(xorGlassMat, xorOtherTextures);
			break;
		case WEAPON_CHAMSSTYLE_NONE:
		default:
			break;
		}

		if (pMat)
		{
			m_pModelRender->ForcedMaterialOverride(pMat);
			g_pDrawModelExecute(ecx, ctx, state, pInfo, pCustomBoneToWorld);
		}
	}
}
