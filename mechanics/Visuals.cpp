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

	m_dwOverridePostProcessingDisable = (bool*)(*(DWORD**)(CPattern::FindPattern((BYTE*)m_pApp->ClientDll(), 0x50E5000, (BYTE*)"\x80\x3D\x00\x00\x00\x00\x00\x53\x56\x57\x0F\x85", "ag-----zrhli") + 0x2));

	m_iViewmodelFovValue = 70;
}

void CVisuals::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;
}

void CVisuals::TriggerHitmarker(float fTime)
{
	m_fDrawHitmarkerStartTime = fTime;
	m_fDrawHitmarkerTime = fTime;
}

void CVisuals::UpdateHitmarker(float fInputSampleTime)
{
	if (!m_bHitmarker || m_fDrawHitmarkerTime == 0.0f)
		return;

	m_fDrawHitmarkerTime -= fInputSampleTime;
	if (m_fDrawHitmarkerTime < 0.0f)
		m_fDrawHitmarkerTime = 0.0f;
}

void CVisuals::DrawCrosshair()
{
	if (!m_bIsEnabled)
		return;

	if (!m_bCrosshair)
		return;

	const int crosshair_size = 12;
	int iMidX, iMidY;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	if (!pLocalEntity)
		return;

	QAngle qAimPunchAngles = *pLocalEntity->GetAimPunchAngle() * m_pApp->GetRecoilCompensation();
	if (m_bCrosshairShowRecoil && (qAimPunchAngles.x != 0.0f || qAimPunchAngles.y != 0.0f || qAimPunchAngles.z != 0.0f) &&
		!m_pApp->Misc()->GetNoRecoil() && !m_pApp->Ragebot()->DidNoRecoil())
	{
		Vector vHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();
		qAimPunchAngles += m_pApp->GetClientViewAngles();

		Vector vForward, vScreenPos;
		AngleVectors(qAimPunchAngles, &vForward);

		CGui::Instance()->WorldToScreen(vHeadPos + (vForward * 8192.0f), vScreenPos);
		iMidX = vScreenPos.x;
		iMidY = vScreenPos.y;
	}
	else
	{
		iMidX = m_iSurfaceWidth / 2;
		iMidY = m_iSurfaceHeight / 2;
	}

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
	if (!m_bIsEnabled)
		return;

	if (!m_bHitmarker)
		return;

	const int hitmarker_gap = 4;
	const int hitmarker_size = 6;

	if (m_fDrawHitmarkerTime > 0.0f && m_bHitmarker)
	{
		int iMidX = m_iSurfaceWidth / 2;
		int iMidY = m_iSurfaceHeight / 2;

		ISurface* pSurface = m_pApp->Surface();
		pSurface->DrawSetColor((int)((m_fDrawHitmarkerTime / m_fDrawHitmarkerStartTime) * 255.0f), 255, 255, 255);
		pSurface->DrawLine(iMidX - (hitmarker_gap + hitmarker_size), iMidY - (hitmarker_gap + hitmarker_size), iMidX - hitmarker_gap, iMidY - hitmarker_gap); // Top left
		pSurface->DrawLine(iMidX - (hitmarker_gap + hitmarker_size), iMidY + (hitmarker_gap + hitmarker_size), iMidX - hitmarker_gap, iMidY + hitmarker_gap); // Bottom left
		pSurface->DrawLine(iMidX + hitmarker_gap, iMidY - hitmarker_gap, iMidX + (hitmarker_gap + hitmarker_size), iMidY - (hitmarker_gap + hitmarker_size)); // Top right
		pSurface->DrawLine(iMidX + hitmarker_gap, iMidY + hitmarker_gap, iMidX + (hitmarker_gap + hitmarker_size), iMidY + (hitmarker_gap + hitmarker_size)); // Bottom right
	}
}

void CVisuals::NoFlash(float fFlashPercentage)
{
	if (!m_bIsEnabled)
		return;

	m_fFlashPercentage = fFlashPercentage;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	static bool bDisabledNoFlash = false;
	if (!m_bNoFlash)
	{
		if (!bDisabledNoFlash)
		{
			*(float*)((DWORD)pLocalEntity + Offsets::m_flFlashMaxAlpha) = 255.0f;
			bDisabledNoFlash = true;
		}
	}
	else
	{
		*(float*)((DWORD)pLocalEntity + Offsets::m_flFlashMaxAlpha) = 255.0f - (255.0f * (1.0f - (m_fFlashPercentage / 100.0f)));
		bDisabledNoFlash = false;
	}
}

void CVisuals::NoSmoke(bool bNoSmoke)
{
	if (!m_bIsEnabled)
		return;

	static CXorString smoke_materials[] = {
		CXorString("gj÷¶~hé§8}ì±cjö¯x`à´&$ó«dä±zdî§a:Ú¤~yà"),
		CXorString("gj÷¶~hé§8}ì±cjö¯x`à´&$ó«dä±zdî§a:Ú±zdî§pyà¬voà"),
		CXorString("gj÷¶~hé§8}ì±cjö¯x`à´&$ó«dä±zdî§a:Ú§zdá±"),
		CXorString("gj÷¶~hé§8}ì±cjö¯x`à´&$ó«dä±zdî§a:Ú§zdá±Hbè²vhñ¦bxñ")
	};
	static CXorString pOtherTextures("Xí§e+ñ§oð°rx");

	m_bNoSmoke = bNoSmoke;

	static bool bNoSmokeDisabled = true;
	if (!m_bNoSmoke && bNoSmokeDisabled)
		return;

	for (int i = 0; i < sizeof(smoke_materials) / sizeof(*smoke_materials); i++)
	{
		IMaterial* pMat = this->m_pApp->MaterialSystem()->FindMaterial(smoke_materials[i].ToCharArray(), pOtherTextures.ToCharArray());
		
		pMat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, bNoSmokeDisabled);
	}
	bNoSmokeDisabled = !bNoSmokeDisabled;
}

IMaterial* CVisuals::HandsDrawStyle(const char* pszModelName, void* ecx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!m_bIsEnabled)
		return NULL;

	if (m_iHandsDrawStyle == HANDSDRAWSTYLE_NONE)
		return NULL;

	static CXorString pArms("vyè±");
	static CXorString pModelTextures("Zdá§{+ñ§oð°rx");

	IMaterial* pMat = NULL;

	if (strstr(pszModelName, pArms.ToCharArray()) != NULL)
	{
		pMat = this->m_pApp->MaterialSystem()->FindMaterial(pszModelName, pModelTextures.ToCharArray());

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

	return pMat;
}

void CVisuals::NoVisualRecoil(CViewSetup* pViewSetup)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bNoVisualRecoil)
		return;

	IClientEntity* pLocalEntity = this->m_pApp->EntityList()->GetClientEntity(this->m_pApp->EngineClient()->GetLocalPlayer());

	QAngle punchAngles = *pLocalEntity->GetAimPunchAngle();
	QAngle viewPunch = *pLocalEntity->GetViewPunchAngle();

	pViewSetup->angles.x -= (viewPunch.x + punchAngles.x * m_pApp->GetRecoilCompensation() * RECOIL_TRACKING);
	pViewSetup->angles.y -= (viewPunch.y + punchAngles.y * m_pApp->GetRecoilCompensation() * RECOIL_TRACKING);
}

void CVisuals::Thirdperson()
{
	static Vector vecAngles;
	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	bool localIsAlive = pLocalEntity->IsAlive();
	if (!m_bThirdperson || !m_bIsEnabled || !localIsAlive)
	{
		if (m_pApp->Input()->m_fCameraInThirdPerson || !localIsAlive)
		{
			m_pApp->Input()->m_fCameraInThirdPerson = false;
			m_pApp->Input()->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
		}
		return;
	}

	m_pApp->EngineClient()->GetViewAngles(vecAngles);
	if (!m_pApp->Input()->m_fCameraInThirdPerson || m_iThirdpersonDistance != m_pApp->Input()->m_vecCameraOffset.z)
	{
		m_pApp->Input()->m_fCameraInThirdPerson = true;
		m_pApp->Input()->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, m_iThirdpersonDistance);
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
		*(Vector*)((DWORD)pLocalEntity + Offsets::deadflag + 0x4) = m_pApp->LastTickAngles();
	}
}

void CVisuals::FovChange(CViewSetup* pViewSetup)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bFovChange)
		return;

	static bool bChangedZoomSensitivity = false;
	static ConVar* pZoomSensitivity = m_pApp->CVar()->FindVar(CXorString("mdê¯Hxà¬dbñ«abñ»Hyä¶~dÚ¯x~ö§").ToCharArray());
	IClientEntity* pLocalEntity = this->m_pApp->EntityList()->GetClientEntity(this->m_pApp->EngineClient()->GetLocalPlayer());
	bool bChangeScopedFov = m_bFovChangeScoped && pLocalEntity->IsScoped();
	if (bChangeScopedFov)
	{
		if (!bChangedZoomSensitivity)
		{
			pZoomSensitivity->SetValue(0.0f);
			bChangedZoomSensitivity = true;
		}
	}
	else if (bChangedZoomSensitivity)
	{
		pZoomSensitivity->SetValue(m_flZoomSensitivity);
		bChangedZoomSensitivity = false;

		return;
	}

	if (bChangeScopedFov || m_bFovChangeScoped && !pLocalEntity->IsScoped() || !m_bFovChangeScoped && !pLocalEntity->IsScoped())
	{
		pViewSetup->fov = m_iFovValue;
	}
}

void CVisuals::DisablePostProcessing(bool bDisablePostProcessing)
{
	m_bDisablePostProcessing = bDisablePostProcessing;

	*m_dwOverridePostProcessingDisable = m_bDisablePostProcessing;
}
