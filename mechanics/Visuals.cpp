#include "Visuals.h"
#include "Application.h"

CVisuals::CVisuals()
{
	m_bCrosshair = false;
	m_fDrawHitmarkerStartTime = 0.0f;
	m_flZoomSensitivity = 1.0f;
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

	sprintf(m_pHitmarkerSound, "%smedia\\%s", m_pApp->GetWorkingDirectory(), CXorString("b�x~�9|�").ToCharArray());
}

void CVisuals::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;
}

void CVisuals::TriggerHitmarker()
{
	m_fDrawHitmarkerStartTime = m_pApp->GlobalVars()->curtime;
	if (m_bHitmarkerSound)
	{
		PlaySoundA(m_pHitmarkerSound, NULL, SND_ASYNC | SND_FILENAME);
	}
}

void CVisuals::ResetHitmarker()
{
	m_fDrawHitmarkerStartTime = 0.0f;
}

void CVisuals::DrawCrosshair()
{
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

void CVisuals::DrawSpreadCone()
{
	if (!m_bSpreadCone)
		return;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	CWeapon* pActiveWeapon = pLocalEntity->GetActiveWeapon();
	if (!pActiveWeapon)
		return;

	int width = CGui::Instance()->GetScreenWidth();
	int height = CGui::Instance()->GetScreenHeight();

	float spread = ((pActiveWeapon->GetInaccuracy() + pActiveWeapon->GetSpread()) * 320.f) / tanf(DEG2RAD(this->m_iFovValue) * 0.5f) * height / 480.f;

	ISurface* pSurface = m_pApp->Surface();
	pSurface->DrawSetColor(255, 255, 0, 0);
	pSurface->DrawOutlinedCircle(width / 2, height / 2, spread, 64);
}

void CVisuals::DrawHitmarker(ISurface* pSurface)
{
	if (!m_bHitmarker)
		return;

	const int hitmarker_gap = 4;
	const int hitmarker_size = 6;

	int iMidX = m_iSurfaceWidth / 2;
	int iMidY = m_iSurfaceHeight / 2;

	float fTime = m_pApp->GlobalVars()->curtime - m_fDrawHitmarkerStartTime;
	if (fTime > HITMARKER_DEFAULT_TIME)
		return;

	int iAlpha = 255 - ((fTime / HITMARKER_DEFAULT_TIME) * 255.0f);
	if (iAlpha == 0)
		return;

	pSurface->DrawSetColor(iAlpha, 255, 255, 255);
	pSurface->DrawLine(iMidX - (hitmarker_gap + hitmarker_size), iMidY - (hitmarker_gap + hitmarker_size), iMidX - hitmarker_gap, iMidY - hitmarker_gap); // Top left
	pSurface->DrawLine(iMidX - (hitmarker_gap + hitmarker_size), iMidY + (hitmarker_gap + hitmarker_size), iMidX - hitmarker_gap, iMidY + hitmarker_gap); // Bottom left
	pSurface->DrawLine(iMidX + hitmarker_gap, iMidY - hitmarker_gap, iMidX + (hitmarker_gap + hitmarker_size), iMidY - (hitmarker_gap + hitmarker_size)); // Top right
	pSurface->DrawLine(iMidX + hitmarker_gap, iMidY + hitmarker_gap, iMidX + (hitmarker_gap + hitmarker_size), iMidY + (hitmarker_gap + hitmarker_size)); // Bottom right
}

void CVisuals::DrawHitmarkerHitpoint(ISurface* pSurface)
{
	float fCurtime = m_pApp->GlobalVars()->curtime;
	for (std::vector<HitmarkerEntry>::iterator it = m_pApp->m_pHitmarker.begin(); it != m_pApp->m_pHitmarker.end();)
	{
		Vector vScreen;
		if (m_pApp->Gui()->WorldToScreen(it->vHit, vScreen))
		{
			pSurface->DrawSetColor(255, 255, 255, 255);
			pSurface->DrawLine(vScreen.x - 4, vScreen.y - 4, vScreen.x + 4, vScreen.y + 4);
			pSurface->DrawLine(vScreen.x - 4, vScreen.y + 4, vScreen.x + 4, vScreen.y - 4);
		}

		if (it->fStarttime + 1.0 < fCurtime)
		{
			it = m_pApp->m_pHitmarker.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void CVisuals::NoFlash(float fFlashPercentage)
{
	if (!m_pApp->EngineClient()->IsInGame())
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
	static CXorString smoke_materials[] = {
		CXorString("gj��~h�8}�cj��x`�&$�d�zd�a:ڤ~y�"),
		CXorString("gj��~h�8}�cj��x`�&$�d�zd�a:ڱzd�py�vo�"),
		CXorString("gj��~h�8}�cj��x`�&$�d�zd�a:ڧzd�"),
		CXorString("gj��~h�8}�cj��x`�&$�d�zd�a:ڧzd�Hb�vh�bx�")
	};
	static CXorString pOtherTextures("X��e+�o�rx");

	m_bNoSmoke = bNoSmoke;

	static bool bNoSmokeDisabled = true;
	if (!m_bNoSmoke && bNoSmokeDisabled)
		return;

	for (int i = 0; i < sizeof(smoke_materials) / sizeof(*smoke_materials); i++)
	{
		IMaterial* pMat = this->m_pApp->MaterialSystem()->FindMaterial(smoke_materials[i].ToCharArray(), pOtherTextures.ToCharArray());
		if (!pMat)
			continue;

		pMat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, bNoSmokeDisabled);
	}
	bNoSmokeDisabled = !bNoSmokeDisabled;
}

IMaterial* CVisuals::HandsDrawStyle(const char* pszModelName, void* ecx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (m_iHandsDrawStyle == HANDSDRAWSTYLE_NONE)
		return NULL;

	static CXorString pArms("vy�");
	static CXorString pModelTextures("Zd�{+�o�rx");

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
	if (!m_bThirdperson || !localIsAlive)
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

		Ray_t ray;
		trace_t trace;
		CTraceFilterWorldAndPropsOnly filter;
		float fAdjDist = m_iThirdpersonDistance;

		Vector vMyHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();

		Vector camForward;
		AngleVectors(QAngle(vecAngles.x, vecAngles.y, vecAngles.z), &camForward, NULL, NULL);

		ray.Init(vMyHeadPos, vMyHeadPos - (camForward * m_iThirdpersonDistance));
		m_pApp->EngineTrace()->TraceRay(ray, MASK_SOLID, &filter, &trace);

		if (trace.fraction < 1.0) {
			fAdjDist = m_iThirdpersonDistance * (trace.fraction - 0.07f);
		}
		else {
			fAdjDist = m_iThirdpersonDistance;
		}

		//todo: TRANS DIST AND MIN MAYBE?
		//if (adjDist < START_TRANS_DIST) {
		//	localPlayer->SetRenderMode(kRenderTransColor); // make him translucent
		//	localPlayer->SetRenderColorA((byte)(adjDist * TRANS_DELTA)); // closer=less opacity
		//}

		//if (adjDist < CAM_MIN_DIST)
		//	adjDist = CAM_MIN_DIST;

		if (fAdjDist > CAM_MAX_DIST)
			fAdjDist = CAM_MAX_DIST;

		m_pApp->Input()->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, fAdjDist);
	}
}

void CVisuals::ThirdpersonAntiAim()
{
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
	if (!m_bFovChange)
		return;

	static bool bChangedZoomSensitivity = false;
	static ConVar* pZoomSensitivity = m_pApp->CVar()->FindVar(CXorString("md�Hx�db�ab�Hy�~dگx~��").ToCharArray());
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


void CVisuals::DrawNoScope()
{
	if (!m_bNoScope)
		return;

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer(true);
	if (!pLocalEntity)
		return;

	if (!pLocalEntity->IsScoped())
		return;

	int width, height;
	m_pApp->EngineClient()->GetScreenSize(width, height);
	m_pApp->Surface()->DrawSetColor(255, 0, 0, 0);
	m_pApp->Surface()->DrawLine(width / 2, 0, width / 2, height);
	m_pApp->Surface()->DrawLine(0, height / 2, width, height / 2);
}

bool CVisuals::NoScope(unsigned int vguiPanel)
{
	if (!m_bNoScope)
		return false;

	static CXorString hudZoom("_~�xd�");
	if (!strcmp(hudZoom.ToCharArray(), m_pApp->Panel()->GetName(vguiPanel)))
		return true;
	return false;
}

void CVisuals::Nightmode()
{
	static CXorString xorSkyname("d}ڱ|r�zn");
	static CXorString xorDrawSpecificStaticProp("eT��v|ֲrh�~hֶv�Gy�");
	static CXorString xorWorld("@d��s");
	static CXorString xorStaticProp("D�~hհx{");
	static CXorString xorSkyNight("d`��tx�He���");

	static bool bLastSetting = m_bNightmode;

	static ConVar* sv_skyname = m_pApp->CVar()->FindVar(xorSkyname.ToCharArray());
	//sv_skyname->nFlags &= ~FCVAR_CHEAT; // needed?
	static char oldSkyname[256];
	if (oldSkyname[0] == '\0' || m_bNightmodeMapChange)
	{
		int iLen = strlen(sv_skyname->value) + 1;
		memcpy(oldSkyname, sv_skyname->value, iLen < 256 ? iLen : 256);
		m_bNightmodeMapChange = false;
	}

	//static ConVar* r_drawspecificstaticprop = m_pApp->CVar()->FindVar(xorDrawSpecificStaticProp.ToCharArray());

	if (bLastSetting != m_bNightmode)
	{
		bLastSetting = m_bNightmode;
		m_bNightmodePerfomed = false;
	}

	if (!m_bNightmodePerfomed)
	{
		//r_drawspecificstaticprop->SetValue(0); // needed?
		for (auto i = m_pApp->MaterialSystem()->FirstMaterial(); i != m_pApp->MaterialSystem()->InvalidMaterial(); i = m_pApp->MaterialSystem()->NextMaterial(i))
		{
			IMaterial* pMaterial = m_pApp->MaterialSystem()->GetMaterial(i);

			if (!pMaterial || pMaterial->IsErrorMaterial())
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), xorWorld.ToCharArray()) || strstr(pMaterial->GetTextureGroupName(), xorStaticProp.ToCharArray()))
			{
				if (bLastSetting)
				{
					sv_skyname->SetValue(xorSkyNight.ToCharArray());
					pMaterial->ColorModulate(0.25, 0.25, 0.25);
				}
				else
				{
					sv_skyname->SetValue(oldSkyname);
					pMaterial->ColorModulate(1.00, 1.00, 1.00);
				}
			}
		}

		if (!bLastSetting)
		{
			oldSkyname[0] = '\0';
		}
		m_bNightmodePerfomed = true;
	}
}

void CVisuals::DrawBulletTracer()
{
	if (!m_bBulletTracer)
		return;

	for (std::vector<BulletTracerEntry>::iterator it = m_pApp->m_pBulletTracer.begin(); it != m_pApp->m_pBulletTracer.end(); it++)
	{
		BeamInfo_t beamInfo;
		beamInfo.m_nType = TE_BEAMPOINTS;
		beamInfo.m_pszModelName = "sprites/physbeam.vmt";
		beamInfo.m_nModelIndex = -1;

		// dont know why xD
		/*beamInfo.m_flHaloScale = 0.0f;
		beamInfo.m_flFadeLength = 0.0f;
		beamInfo.m_flSpeed = 0.2f;
		beamInfo.m_nStartFrame = 0;
		beamInfo.m_flFrameRate = 0.f;
		beamInfo.m_nFlags = 0;*/

		beamInfo.m_flAmplitude = 2.0f;
		beamInfo.m_flEndWidth = 2.0f;
		beamInfo.m_flLife = 2.0f; // time alive
		beamInfo.m_flWidth = 3.0f;

		beamInfo.m_flBrightness = 255.f;
		beamInfo.m_bRenderable = true;
		beamInfo.m_nSegments = 2;

		beamInfo.m_flRed = 255.f;
		beamInfo.m_flGreen = 255.0f;
		beamInfo.m_flBlue = 128.f;

		beamInfo.m_vecStart = it->vStart;
		beamInfo.m_vecEnd = it->vEnd;

		Beam_t* pBeam = m_pApp->ViewRenderBeams()->CreateBeamPoints(beamInfo);

		if (pBeam)
		{
			m_pApp->ViewRenderBeams()->DrawBeam(pBeam);
		}
	}
	m_pApp->m_pBulletTracer.clear();
}
