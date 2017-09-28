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

		GrenadeEsp(pCurEntity);

		//todo: func
		// Weapons
		CWeapon* pCurWeapon = (CWeapon*)pCurEntity;
		if (!pCurWeapon)
			continue;

		if (!pCurEntity->GetClientClass()->IsWeapon())
			continue;

		if (pCurWeapon->GetOwnerId() != -1) // check if dropped weapon
			continue;

		if (m_bDrawWeaponBoundingBox)
		{
			int x0, y0, x1, y1;
			if (ShouldDrawBox(pCurEntity, x0, y0, x1, y1))
			{
				m_pApp->Surface()->DrawSetColor(m_pApp->Esp()->GetColorSpotted());
				m_pApp->Surface()->DrawOutlinedRect(x0 + 1, y0 + 1, x1 - 1, y1 - 1);
			}
		}

		Vector vScreenOrigin;
		Vector vCurEntOrigin = *pCurEntity->GetOrigin();
		if (m_pApp->Gui()->WorldToScreen(vCurEntOrigin, vScreenOrigin))
		{
			DrawWeaponName(pCurWeapon, vScreenOrigin.x, vScreenOrigin.y);
		}
	}
}

void CWeaponEsp::GrenadeEsp(IClientEntity* pCurEntity)
{
	CWeapon* pCurWeapon = (CWeapon*)pCurEntity;
	if (!pCurWeapon)
		return;

	if (!pCurEntity->GetClientClass()->IsGrenade())
		return;

	if (m_bDrawGrenadeBoundingBox)
	{
		int x0, y0, x1, y1;
		if (ShouldDrawBox(pCurEntity, x0, y0, x1, y1))
		{
			m_pApp->Surface()->DrawSetColor(m_pApp->Esp()->GetColorSpotted());
			m_pApp->Surface()->DrawOutlinedRect(x0 + 1, y0 + 1, x1 - 1, y1 - 1);
		}
	}

	if (m_bDrawGrenadeName)
	{
		Vector vScreenOrigin;
		Vector vCurEntOrigin = *pCurEntity->GetOrigin();
		if (m_pApp->Gui()->WorldToScreen(vCurEntOrigin, vScreenOrigin))
		{
			static unsigned int iFont = g_pResourceManager->GetFont(RM_FONT_NORMAL);
			m_pApp->Surface()->DrawSetTextFont(iFont);

			m_pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);

			wchar_t* pGrenadeName;
			switch (pCurEntity->GetClientClass()->m_ClassID)
			{
			case CSmokeGrenadeProjectile:
				pGrenadeName = L"Smoke-Grenade";
				break;
			case CDecoyProjectile:
				pGrenadeName = L"Decoy";
				break;
			case CBaseCSGrenadeProjectile:
				//todo: check if flash or he
				pGrenadeName = L"HE/Flashbang";
				break;
			case CMolotovProjectile:
				pGrenadeName = L"Molotov";
				break;
			}
			int w, h;
			m_pApp->Surface()->GetTextSize(iFont, pGrenadeName, w, h);
			m_pApp->Surface()->DrawSetTextPos(vScreenOrigin.x - w / 2, vScreenOrigin.y);
			m_pApp->Surface()->DrawPrintText(pGrenadeName, lstrlenW(pGrenadeName));
		}
	}
}

void CWeaponEsp::DrawWeaponName(CWeapon* pCurEntity, int posX, int posY)
{
	if (!m_bDrawWeaponName)
		return;

	static unsigned int iFont = g_pResourceManager->GetFont(RM_FONT_NORMAL);
	m_pApp->Surface()->DrawSetTextFont(iFont);


	// TODO
	// remove weapon_
	// r8 = weapon_deagle
	wchar_t wcWeaponName[256];
	mbstowcs(wcWeaponName, pCurEntity->GetWeaponInfo()->szWeaponName, 256);
	int iWeaponNameLen = lstrlenW(wcWeaponName);

	int w, h;
	m_pApp->Surface()->GetTextSize(iFont, wcWeaponName, w, h);

	m_pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);
	m_pApp->Surface()->DrawSetTextPos(posX - w / 2, posY);
	m_pApp->Surface()->DrawPrintText(wcWeaponName, iWeaponNameLen);
}

bool CWeaponEsp::ShouldDrawBox(IClientEntity* pCurEntity, int &x0, int &y0, int &x1, int &y1)
{
	Vector edges[4] =
	{
		Vector(1.0f , 1.0f, 1.0f),
		Vector(-1.0f, 1.0f, 1.0f),
		Vector(1.0f ,-1.0f, 1.0f),
		Vector(-1.0f,-1.0f, 1.0f),
	};

	Vector vecOrigin = *pCurEntity->GetOrigin();
	QAngle angRotation = *pCurEntity->GetAngRotation();

	Vector mins = ((CWeapon*)pCurEntity)->GetVecMin();
	Vector maxs = ((CWeapon*)pCurEntity)->GetVecMax();

	x0 = maxof<int>();
	y0 = maxof<int>();
	x1 = minof<int>();
	y1 = minof<int>();

	for (int i = 0; i < 4; i++)
	{
		Vector mins2d, maxs2d;

		if (!m_pApp->Gui()->WorldToScreen(vecOrigin + (mins * VectorRotate(edges[i], angRotation)), mins2d))
			return false;

		if (!m_pApp->Gui()->WorldToScreen(vecOrigin + (maxs * VectorRotate(edges[i], angRotation)), maxs2d))
			return false;

		x0 = Min<int>(x0, Min(mins2d.x, maxs2d.x));
		y0 = Min<int>(y0, Min(mins2d.y, maxs2d.y));
		x1 = Max<int>(x1, Max(mins2d.x, maxs2d.x));
		y1 = Max<int>(y1, Max(mins2d.y, maxs2d.y));
	}

	return true;
}
