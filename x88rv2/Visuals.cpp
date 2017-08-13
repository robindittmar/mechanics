#include "Visuals.h"
#include "Application.h"

CVisuals::CVisuals()
{
	m_bCrosshair = false;
}

CVisuals::~CVisuals()
{
}

void CVisuals::Setup()
{
	m_pApp = CApplication::Instance();
	m_pApp->EngineClient()->GetScreenSize(m_iSurfaceWidth, m_iSurfaceHeight);
}

void CVisuals::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;
}

void CVisuals::DrawCrosshair()
{
	const int crosshair_size = 20;

	int iMidX = m_iSurfaceWidth / 2;
	int iMidY = m_iSurfaceHeight / 2;

	ISurface* pSurface = m_pApp->Surface();

	// Outer lines
	pSurface->DrawSetColor(255, 255, 255, 255);
	pSurface->DrawLine(iMidX - crosshair_size, iMidY, iMidX + crosshair_size, iMidY);
	pSurface->DrawLine(iMidX, iMidY - crosshair_size, iMidX, iMidY + crosshair_size);

	// Inner crosshair
	pSurface->DrawSetColor(255, 255, 0, 0);
	pSurface->DrawLine(iMidX - (crosshair_size / 2), iMidY, iMidX + (crosshair_size / 2), iMidY);
	pSurface->DrawLine(iMidX , iMidY - (crosshair_size / 2), iMidX, iMidY + (crosshair_size / 2));
}

void CVisuals::DrawHitmarker()
{
	const int hitmarker_gap = 10;
	const int hitmarker_size = 10;

	if(m_fDrawHitmarker > 0.0f && m_bHitmarker)
	{
		int iMidX = m_iSurfaceWidth / 2;
		int iMidY = m_iSurfaceHeight / 2;

		ISurface* pSurface = m_pApp->Surface();
		pSurface->DrawSetColor(255, 255, 255, 255);
		pSurface->DrawLine(iMidX - (hitmarker_gap + hitmarker_size), iMidY - (hitmarker_gap + hitmarker_size), iMidX - hitmarker_gap, iMidY - hitmarker_gap); // Top left
		pSurface->DrawLine(iMidX - (hitmarker_gap + hitmarker_size), iMidY + (hitmarker_gap + hitmarker_size), iMidX - hitmarker_gap, iMidY + hitmarker_gap); // Bottom left
		pSurface->DrawLine(iMidX + hitmarker_gap, iMidY - hitmarker_gap, iMidX + (hitmarker_gap + hitmarker_size), iMidY - (hitmarker_gap + hitmarker_size)); // Top right
		pSurface->DrawLine(iMidX + hitmarker_gap, iMidY + hitmarker_gap, iMidX + (hitmarker_gap + hitmarker_size), iMidY + (hitmarker_gap + hitmarker_size)); // Bottom right
	}
}

void CVisuals::NoFlash()
{
	if (!m_bIsEnabled)
		return;

	if (!m_bNoFlash)
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	*(float*)((DWORD)pLocalEntity + OFFSET_NOFLASH) = 255.0f - (255.0f * (1.0f - m_iNoFlashPercentage));
}

void CVisuals::NoSmoke()
{
	if (!m_bIsEnabled)
		return;

	if (!m_bNoSmoke)
		return;

	static CXorString smoke_materials[] = {
		CXorString("gj÷¶~hé§8}ì±cjö¯x`à´&$ó«dä±zdî§a:Ú¤~yà"),
		CXorString("gj÷¶~hé§8}ì±cjö¯x`à´&$ó«dä±zdî§a:Ú±zdî§pyà¬voà"),
		CXorString("gj÷¶~hé§8}ì±cjö¯x`à´&$ó«dä±zdî§a:Ú§zdá±"),
		CXorString("gj÷¶~hé§8}ì±cjö¯x`à´&$ó«dä±zdî§a:Ú§zdá±Hbè²vhñ¦bxñ")
	};
	static CXorString pOtherTextures("Xí§e+ñ§oð°rx");

	for (int i = 0; i < sizeof(smoke_materials) / sizeof(*smoke_materials); i++)
	{
		IMaterial* pMat = this->m_pApp->MaterialSystem()->FindMaterial(smoke_materials[i].ToCharArray(), pOtherTextures.ToCharArray());
		pMat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
	}
}

void CVisuals::HandsDrawStyle(const char* pszModelName, void* ecx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!m_bIsEnabled)
		return;

	if (m_tHandsDrawStyle == HandsDrawStyleNone)
		return;

	static CXorString pArms("vyè±");
	static CXorString pModelTextures("Zdá§{+ñ§oð°rx");

	if (strstr(pszModelName, pArms.ToCharArray()) != NULL)
	{
		IMaterial* pMat = this->m_pApp->MaterialSystem()->FindMaterial(pszModelName, pModelTextures.ToCharArray());

		switch (m_tHandsDrawStyle)
		{
		case HandsDrawStyleNoHands:
			pMat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			break;
		case HandsDrawStyleWireframe:
			pMat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				break;
		}

		this->m_pApp->ModelRender()->ForcedMaterialOverride(pMat);
		m_pApp->DrawModelExecute()(ecx, ctx, state, pInfo, pCustomBoneToWorld);
	}
}

void CVisuals::NoVisualRecoil(CViewSetup* pViewSetup)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bNoVisualRecoil)
		return;

	IClientEntity* pLocalEntity = this->m_pApp->EntityList()->GetClientEntity(this->m_pApp->EngineClient()->GetLocalPlayer());

	QAngle punchAngles = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));
	QAngle viewPunch = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_VIEWPUNCHANGLE));

	pViewSetup->angles.x -= (viewPunch.x + punchAngles.x * RECOIL_COMPENSATION * RECOIL_TRACKING);
	pViewSetup->angles.y -= (viewPunch.y + punchAngles.y * RECOIL_COMPENSATION * RECOIL_TRACKING);
}

void CVisuals::Thirdperson()
{
	static Vector vecAngles;

	if (!m_bThirdperson || !m_bIsEnabled)
	{
		if (m_pApp->Input()->m_fCameraInThirdPerson)
		{
			m_pApp->Input()->m_fCameraInThirdPerson = false;
			m_pApp->Input()->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
		}
		return;
	}

	m_pApp->EngineClient()->GetViewAngles(vecAngles);
	if (!m_pApp->Input()->m_fCameraInThirdPerson)
	{
		m_pApp->Input()->m_fCameraInThirdPerson = true;
		m_pApp->Input()->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, m_iThirdpersonValue);
	}
}

void CVisuals::ThirdpersonAntiAim()
{
	if (!m_bIsEnabled)
		return;

	if (!m_bThirdperson)
		return;

	if (m_pApp->Input()->m_fCameraInThirdPerson)
	{
		IClientEntity* pLocalEntity = this->m_pApp->EntityList()->GetClientEntity(this->m_pApp->EngineClient()->GetLocalPlayer());
		*(Vector*)((DWORD)pLocalEntity + OFFSET_DEADFLAG + 0x4) = m_pApp->LastTickAngles();
	}
}

void CVisuals::FovChange(CViewSetup* pViewSetup)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bFovChange)
		return;

	IClientEntity* pLocalEntity = this->m_pApp->EntityList()->GetClientEntity(this->m_pApp->EngineClient()->GetLocalPlayer());
	if (!m_bFovChangeScoped && pLocalEntity->IsScoped())
		return;

	pViewSetup->fov = 105;
}

void CVisuals::Chams(const char* pszModelName, void* ecx, IMatRenderContext * ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bChams)
		return;

	static CXorString pModelTextures("Zdá§{+ñ§oð°rx");

	if (strstr(pszModelName, "models/player") != NULL)
	{
		IMaterial *mats[32];

		IClientEntity* pLocalEntity = (IClientEntity*)m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
		IClientEntity* pModelEntity = m_pApp->EntityList()->GetClientEntity(pInfo.entity_index);
		studiohdr_t* hdr = (studiohdr_t*)((DWORD)pModelEntity + 0xD20);

		if (pModelEntity && pModelEntity->IsAlive() &&
			pLocalEntity != pModelEntity && 
			pLocalEntity->TeamNum() != pModelEntity->TeamNum())
		{
			m_pApp->ModelInfo()->GetModelMaterials(pInfo.pModel, hdr->numtextures, mats);
			for (int i = 0; i < hdr->numtextures; i++)
			{
				IMaterial* mat = mats[i];
				if (!mat)
					continue;

				if ((unsigned long)mat == 0xcccccccc)
					break;

				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
				mat->SetMaterialVarFlag(MATERIAL_VAR_FLAT, true);
				mat->SetMaterialVarFlag(MATERIAL_VAR_ALPHATEST, true);
				mat->AlphaModulate(1.0f); // don't need this, I just use it just cause.

				m_pApp->DrawModelExecute()(ecx, ctx, state, pInfo, pCustomBoneToWorld);

				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
				mat->SetMaterialVarFlag(MATERIAL_VAR_FLAT, false);
				mat->SetMaterialVarFlag(MATERIAL_VAR_ALPHATEST, false);
				mat->AlphaModulate(1.0f);

				m_pApp->DrawModelExecute()(ecx, ctx, state, pInfo, pCustomBoneToWorld);

				//TODO:
				//create material
				// set all for ignorez
				//m_pApp->DrawModelExecute()(ecx, ctx, state, pInfo, pCustomBoneToWorld);
				// set all for ignorez false
				//m_pApp->DrawModelExecute()(ecx, ctx, state, pInfo, pCustomBoneToWorld);
			}
		}
	}
}