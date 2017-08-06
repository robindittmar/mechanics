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

void CVisuals::NoFlash(float fFlashPercentage)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bNoFlash)
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	*(float*)((DWORD)pLocalEntity + NOFLASH_OFFSET) = 255.0f - (255.0f * (1.0f - fFlashPercentage));
}

void CVisuals::NoSmoke()
{
	if (!m_bIsEnabled)
		return;

	if (!m_bNoSmoke)
		return;

	static CXorString smoke_materials[] = {
		CXorString("gj��~h�8}�cj��x`�&$�d�zd�a:ڤ~y�"),
		CXorString("gj��~h�8}�cj��x`�&$�d�zd�a:ڱzd�py�vo�"),
		CXorString("gj��~h�8}�cj��x`�&$�d�zd�a:ڧzd�"),
		CXorString("gj��~h�8}�cj��x`�&$�d�zd�a:ڧzd�Hb�vh�bx�")
	};
	static CXorString pOtherTextures("X�e+�o�rx");

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

	if (!m_tHandsDrawStyle == HandsDrawStyleNone)
		return;

	static CXorString pArms("vy�");
	static CXorString pModelTextures("Zd�{+�o�rx");

	if (strstr(pszModelName, pArms.ToCharArray()) != NULL)
	{
		IMaterial* pMat = this->m_pApp->MaterialSystem()->FindMaterial(pszModelName, pModelTextures.ToCharArray());

		pMat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);

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