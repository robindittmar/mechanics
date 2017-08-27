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

void CVisuals::TriggerHitmarker(float fTime)
{
	m_fDrawHitmarker = fTime;
}

void CVisuals::UpdateHitmarker(float fInputSampleTime)
{
	if (!m_bHitmarker)
		return;

	m_fDrawHitmarker -= fInputSampleTime;
	if (m_fDrawHitmarker < 0.0f)
		m_fDrawHitmarker = 0.0f;
}

void CVisuals::DrawCrosshair()
{
	if (!m_bIsEnabled)
		return;

	if (!m_bCrosshair)
		return;

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
	pSurface->DrawLine(iMidX, iMidY - (crosshair_size / 2), iMidX, iMidY + (crosshair_size / 2));
}

void CVisuals::DrawHitmarker()
{
	if (!m_bHitmarker)
		return;

	const int hitmarker_gap = 10;
	const int hitmarker_size = 10;

	if (m_fDrawHitmarker > 0.0f && m_bHitmarker)
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
	*(float*)((DWORD)pLocalEntity + Offsets::m_flFlashMaxAlpha) = 255.0f - (255.0f * (1.0f - m_fFlashPercentage));
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

	if (m_iHandsDrawStyle == HANDSDRAWSTYLE_NONE)
		return;

	static CXorString pArms("vyè±");
	static CXorString pModelTextures("Zdá§{+ñ§oð°rx");

	if (strstr(pszModelName, pArms.ToCharArray()) != NULL)
	{
		IMaterial* pMat = this->m_pApp->MaterialSystem()->FindMaterial(pszModelName, pModelTextures.ToCharArray());

		switch (m_iHandsDrawStyle)
		{
		case HANDSDRAWSTYLE_NOHANDS:
			pMat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			pMat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			break;
		case HANDSDRAWSTYLE_WIREFRAME:
			pMat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			pMat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
			break;
		case HANDSDRAWSTYLE_NONE:
		default:
			break;
		}

		this->m_pApp->ModelRender()->ForcedMaterialOverride(pMat);
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
		*(Vector*)((DWORD)pLocalEntity + Offsets::m_deadflag + 0x4) = m_pApp->LastTickAngles();
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

	pViewSetup->fov = m_iFovValue;
}
