#include "Visuals.h"
#include "Application.h"

CVisuals::CVisuals()
	: m_flZoomSensitivity(1.0f), m_bNoFlash(false), m_fFlashPercentage(0.0f),
	m_bNoSmoke(false), m_iHandsDrawStyle(HANDSDRAWSTYLE_NONE), m_bNoVisualRecoil(false),
	m_bThirdperson(false), m_iThirdpersonDistance(150), m_bFovChange(false),
	m_bFovChangeScoped(false), m_iFovValue(90), m_bViewmodelFovChange(false),
	m_iViewmodelFovValue(70), m_bDisablePostProcessing(false), m_bNoScope(false),
	m_bBulletTracer(false), m_bBulletTracerSelf(false), m_bBulletTracerTeam(false)
{
}

CVisuals::~CVisuals()
{
}

void CVisuals::Setup()
{
	IFeature::Setup();

	m_pApp->EngineClient()->GetScreenSize(m_iSurfaceWidth, m_iSurfaceHeight);
	m_dwOverridePostProcessingDisable = (bool*)(*(DWORD * *)(CPattern::FindPattern((BYTE*)m_pApp->ClientDll(), 0x50E5000, (BYTE*)"\x80\x3D\x00\x00\x00\x00\x00\x53\x56\x57\x0F\x85", "ag-----zrhli") + 0x2));
}

void CVisuals::Think(void* pParameters)
{
	if (!m_bIsEnabled)
		return;
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
		//CXorString(/*particle/vistasmokev1/vistasmokev1_fire*/"\x67\x6A\xF7\xB6\x7E\x68\xE9\xA7\x38\x7D\xEC\xB1\x63\x6A\xF6\xAF\x78\x60\xE0\xB4\x26\x24\xF3\xAB\x64\x7F\xE4\xB1\x7A\x64\xEE\xA7\x61\x3A\xDA\xA4\x7E\x79\xE0"), // this creates awesome little circle smoke effect
		CXorString(/*particle/vistasmokev1/vistasmokev1_smokegrenade*/"\x67\x6A\xF7\xB6\x7E\x68\xE9\xA7\x38\x7D\xEC\xB1\x63\x6A\xF6\xAF\x78\x60\xE0\xB4\x26\x24\xF3\xAB\x64\x7F\xE4\xB1\x7A\x64\xEE\xA7\x61\x3A\xDA\xB1\x7A\x64\xEE\xA7\x70\x79\xE0\xAC\x76\x6F\xE0"),
		CXorString(/*particle/vistasmokev1/vistasmokev1_emods*/"\x67\x6A\xF7\xB6\x7E\x68\xE9\xA7\x38\x7D\xEC\xB1\x63\x6A\xF6\xAF\x78\x60\xE0\xB4\x26\x24\xF3\xAB\x64\x7F\xE4\xB1\x7A\x64\xEE\xA7\x61\x3A\xDA\xA7\x7A\x64\xE1\xB1"),
		CXorString(/*particle/vistasmokev1/vistasmokev1_emods_impactdust*/"\x67\x6A\xF7\xB6\x7E\x68\xE9\xA7\x38\x7D\xEC\xB1\x63\x6A\xF6\xAF\x78\x60\xE0\xB4\x26\x24\xF3\xAB\x64\x7F\xE4\xB1\x7A\x64\xEE\xA7\x61\x3A\xDA\xA7\x7A\x64\xE1\xB1\x48\x62\xE8\xB2\x76\x68\xF1\xA6\x62\x78\xF1")
	};
	static CXorString pOtherTextures(/*Other textures*/"\x58\x7F\xED\xA7\x65\x2B\xF1\xA7\x6F\x7F\xF0\xB0\x72\x78");

	m_bNoSmoke = bNoSmoke;

	static bool bNoSmokeDisabled = true;
	if (!m_bNoSmoke && bNoSmokeDisabled)
		return;

	for (int i = 0; i < sizeof(smoke_materials) / sizeof(*smoke_materials); i++)
	{
		IMaterial* pMat = this->m_pApp->MaterialSystem()->FindMaterial(smoke_materials[i].ToCharArray(), pOtherTextures.ToCharArray());
		if (!pMat)
			continue;

		if (pMat->IsErrorMaterial())
			continue;

		pMat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, bNoSmokeDisabled);
	}
	bNoSmokeDisabled = !bNoSmokeDisabled;
}

IMaterial* CVisuals::HandsDrawStyle(const char* pszModelName, void* ecx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (m_iHandsDrawStyle == HANDSDRAWSTYLE_NONE)
		return NULL;

	static CXorString pArms(/*arms*/"\x76\x79\xE8\xB1");
	static CXorString pModelTextures(/*Model textures*/"\x5A\x64\xE1\xA7\x7B\x2B\xF1\xA7\x6F\x7F\xF0\xB0\x72\x78");

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

	pViewSetup->angles.x -= (viewPunch.x + punchAngles.x * m_pApp->GunAccuracy()->GetRecoilCompensation() * RECOIL_TRACKING);
	pViewSetup->angles.y -= (viewPunch.y + punchAngles.y * m_pApp->GunAccuracy()->GetRecoilCompensation() * RECOIL_TRACKING);
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
	static float fAdjDist = m_iThirdpersonDistance;
	if (!m_pApp->Input()->m_fCameraInThirdPerson || m_iThirdpersonDistance != m_pApp->Input()->m_vecCameraOffset.z)
	{
		m_pApp->Input()->m_fCameraInThirdPerson = true;

		Ray_t ray;
		trace_t trace;
		CTraceFilterWorldAndPropsOnly filter;

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
	else
	{
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
	static ConVar* pZoomSensitivity = m_pApp->CVar()->FindVar(CXorString(/*zoom_sensitivity_ratio_mouse*/"\x6D\x64\xEA\xAF\x48\x78\xE0\xAC\x64\x62\xF1\xAB\x61\x62\xF1\xBB\x48\x79\xE4\xB6\x7E\x64\xDA\xAF\x78\x7E\xF6\xA7").ToCharArray());
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

	static CXorString hudZoom(/*HudZoom*/"\x5F\x7E\xE1\x98\x78\x64\xE8");
	const char* a = m_pApp->Panel()->GetName(vguiPanel);
	if (!strcmp(hudZoom.ToCharArray(), m_pApp->Panel()->GetName(vguiPanel)))
		return true;
	return false;
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
