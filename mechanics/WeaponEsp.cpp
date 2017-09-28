#include "WeaponEsp.h"
#include "Application.h"

CWeaponEsp::CWeaponEsp()
{
}

CWeaponEsp::~CWeaponEsp()
{
}

void CWeaponEsp::Setup()
{
	m_pApp = CApplication::Instance();
}

void CWeaponEsp::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	for (int i = m_pApp->EngineClient()->GetMaxClients(); i < m_pApp->EntityList()->GetHighestEntityIndex(); i++)
	{
		IClientEntity* pCurEntity = m_pApp->EntityList()->GetClientEntity(i);
		if (!pCurEntity)
			continue;

		if (!pCurEntity->GetBaseEntity())
			continue;

		if (!pCurEntity->GetModel())
			continue;

		if (pCurEntity->IsDormant())
			continue;

		CWeapon* pCurWeapon = (CWeapon*)pCurEntity;
		if (!pCurWeapon)
			continue;

		// Checks if it is a weapon
		const char* pClientClassName = pCurEntity->GetClientClass()->GetName();
		if (strstr(pClientClassName, "CWeapon") == NULL &&
			strstr(pClientClassName, "CAK47") == NULL && // Ak47
			strstr(pClientClassName, "CDEagle") == NULL && // R8 Revolver
			strstr(pClientClassName, "CC4") == NULL) // C4
			continue;

		if (pCurWeapon->GetOwnerId() != -1) // check if dropped weapon
			continue;

		Vector vScreenOrigin;
		Vector vCurEntOrigin = *pCurEntity->GetOrigin();

		if (m_pApp->Gui()->WorldToScreen(vCurEntOrigin, vScreenOrigin))
		{
			// todo: get size somehow ?
			/*pApp->Surface()->DrawSetColor(pApp->Esp()->GetColorSpotted());
			pApp->Surface()->DrawOutlinedRect(vScreenOrigin.x - 10, vScreenOrigin.y - 10, vScreenOrigin.x + 10, vScreenOrigin.y + 10);*/


			DrawWeaponName(pCurWeapon, vScreenOrigin.x, vScreenOrigin.y);
		}
	}
}

void CWeaponEsp::DrawWeaponName(CWeapon* pCurEntity, int posX, int posY)
{
	if (!m_bDrawWeaponName)
		return;

	static unsigned int iFont = g_pResourceManager->GetFont(RM_FONT_NORMAL);
	m_pApp->Surface()->DrawSetTextFont(iFont);

	wchar_t wcWeaponName[256];
	
	// TODO
	// remove weapon_
	// r8 = weapon_deagle
	mbstowcs(wcWeaponName, pCurEntity->GetWeaponInfo()->szWeaponName, 256);
	int iWeaponNameLen = lstrlenW(wcWeaponName);

	int w, h;
	m_pApp->Surface()->GetTextSize(iFont, wcWeaponName, w, h);

	m_pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);
	m_pApp->Surface()->DrawSetTextPos(posX - w / 2, posY + h / 2);
	m_pApp->Surface()->DrawPrintText(wcWeaponName, iWeaponNameLen);
}