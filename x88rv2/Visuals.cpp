#include "Visuals.h"
#include "Application.h"

CVisuals::CVisuals()
{
}

CVisuals::~CVisuals()
{
}

void CVisuals::Setup()
{
	m_pApp = CApplication::Instance();
}

void CVisuals::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;
}

void CVisuals::NoFlash()
{
	if (!m_bIsEnabled)
		return;

	if (!m_bNoFlash)
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	*(float*)((DWORD)pLocalEntity + NOFLASH_OFFSET) = 255.0f - (255.0f * (1.0f - m_iNoFlashPercentage));
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

void CVisuals::HandsDrawStyle(const char* pszModelName)
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
	if (!m_bIsEnabled)
		return;

	if (!m_bThirdperson)
		return;

	static Vector vecAngles;
	m_pApp->EngineClient()->GetViewAngles(vecAngles);
	if (!m_pApp->Input()->m_fCameraInThirdPerson)
	{
		m_pApp->Input()->m_fCameraInThirdPerson = true;
		m_pApp->Input()->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, m_iThirdpersonValue);
	}
	else
	{
		m_pApp->Input()->m_fCameraInThirdPerson = false;
		m_pApp->Input()->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
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