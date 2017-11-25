#include "WeaponEsp.h"
#include "Application.h"

CWeaponEsp::CWeaponEsp()
	: m_bDrawWeaponName(false), m_bDrawWeaponBoundingBox(false), m_bDrawGrenadeName(false),
	m_bDrawGrenadeBoundingBox(false), m_bDrawBombName(false), m_bDrawBombBoundingBox(false),
	m_bDrawBombTimer(false), m_bDrawBombDefuseTimer(false), m_bDrawBombDamageIndicator(false)
{
}

CWeaponEsp::~CWeaponEsp()
{
}

void CWeaponEsp::Setup()
{
	IFeature::Setup();

	m_iFont = g_pResourceManager->GetFont(RM_FONT_NORMAL);

	mbstowcs(m_pSmokeGrenade, CXorString("Dfê©r&Â°reä¦r").ToCharArray(), 64);
	mbstowcs(m_pDecoy, CXorString("Snæ­n").ToCharArray(), 64);
	mbstowcs(m_pFlashbang, CXorString("Qgä±iä¬p").ToCharArray(), 64);
	mbstowcs(m_pHeGrenade, CXorString("_N¨…enë£sn").ToCharArray(), 64);
	mbstowcs(m_pMolotov, CXorString("Zdé­cdó").ToCharArray(), 64);

	mbstowcs(m_pC4Planted, CXorString("Ggä¬cnáâT?").ToCharArray(), 64);
	mbstowcs(m_pC4Time, CXorString("Cbè§-+ ì%mö").ToCharArray(), 64);
	mbstowcs(m_pC4DefuseCountDown, CXorString("Snã·dnáâ~e¿â2%·¤").ToCharArray(), 64);
	mbstowcs(m_pC4DamageIndicator, CXorString("Sjè£pn¿âi.ì").ToCharArray(), 64);
}

void CWeaponEsp::Think(void* pParameters)
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
		WeaponEsp(pCurEntity);
		BombEsp(pCurEntity);
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
			m_pApp->Surface()->DrawSetColor(/*m_pApp->Esp()->GetColorSpotted()*/Color(255, 255, 128, 0));
			m_pApp->Surface()->DrawOutlinedRect(x0 + 1, y0 + 1, x1 - 1, y1 - 1);
		}
	}

	if (m_bDrawGrenadeName)
	{
		Vector vScreenOrigin;
		Vector vCurEntOrigin = *pCurEntity->GetOrigin();
		if (m_pApp->Gui()->WorldToScreen(vCurEntOrigin, vScreenOrigin))
		{
			wchar_t* pGrenadeName;
			const char* pModelName;

			static CXorString xorFlashbang("qgä±iä¬p");
			switch (pCurEntity->GetClientClass()->m_ClassID)
			{
			case CSmokeGrenadeProjectile:
				pGrenadeName = m_pSmokeGrenade;
				break;
			case CDecoyProjectile:
				pGrenadeName = m_pDecoy;
				break;
			case CBaseCSGrenadeProjectile:
				pModelName = m_pApp->ModelInfo()->GetModelName(pCurEntity->GetModel());
				if (strstr(pModelName, xorFlashbang.ToCharArray()) != NULL)
				{
					pGrenadeName = m_pFlashbang;
				}
				else
				{
					pGrenadeName = m_pHeGrenade;
				}
				break;
			case CMolotovProjectile:
				pGrenadeName = m_pMolotov;
				break;
			}
			DrawWeaponName(pGrenadeName, vScreenOrigin.x, vScreenOrigin.y);
		}
	}
}

void CWeaponEsp::WeaponEsp(IClientEntity* pCurEntity)
{
	CWeapon* pCurWeapon = (CWeapon*)pCurEntity;
	if (!pCurWeapon)
		return;

	if (!pCurEntity->GetClientClass()->IsWeapon())
		return;

	if (pCurWeapon->GetOwnerId() != -1) // check if dropped weapon
		return;

	if (m_bDrawWeaponBoundingBox)
	{
		int x0, y0, x1, y1;
		if (ShouldDrawBox(pCurEntity, x0, y0, x1, y1))
		{
			m_pApp->Surface()->DrawSetColor(/*m_pApp->Esp()->GetColorSpotted()*/Color(255, 255, 128, 0));
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

void CWeaponEsp::BombEsp(IClientEntity* pCurEntity)
{
	if (pCurEntity->GetClientClass()->m_ClassID != CPlantedC4)
		return;

	CWeapon* pCurWeapon = (CWeapon*)pCurEntity;
	if (!pCurWeapon)
		return;

	if (m_bDrawBombBoundingBox)
	{
		int x0, y0, x1, y1;
		if (ShouldDrawBox(pCurEntity, x0, y0, x1, y1))
		{
			m_pApp->Surface()->DrawSetColor(/*m_pApp->Esp()->GetColorSpotted()*/Color(255, 255, 128, 0));
			m_pApp->Surface()->DrawOutlinedRect(x0 + 1, y0 + 1, x1 - 1, y1 - 1);
		}
	}

	bool bGotWorldToScreen = false;
	Vector vScreenOrigin;
	Vector vCurEntOrigin = *pCurEntity->GetOrigin();
	int iPlantedTextHeight = 0;
	if (m_bDrawBombName)
	{
		if (bGotWorldToScreen = m_pApp->Gui()->WorldToScreen(vCurEntOrigin, vScreenOrigin))
		{
			iPlantedTextHeight = DrawWeaponName(m_pC4Planted, vScreenOrigin.x, vScreenOrigin.y);
		}
	}

	if (m_bDrawBombTimer)
	{
		if (!bGotWorldToScreen)
		{
			bGotWorldToScreen = m_pApp->Gui()->WorldToScreen(vCurEntOrigin, vScreenOrigin);
		}

		if (bGotWorldToScreen)
		{
			float fTimeToExplode = fmax(pCurEntity->GetC4Blow() - m_pApp->GlobalVars()->curtime, 0.0f);
			if (fTimeToExplode != m_pApp->GlobalVars()->curtime)
			{
				wchar_t pBombTimeText[256];
				swprintf(pBombTimeText, m_pC4Time, fTimeToExplode);
				iPlantedTextHeight += DrawWeaponName(pBombTimeText, vScreenOrigin.x, vScreenOrigin.y + iPlantedTextHeight);
			}
		}
	}

	if (m_bDrawBombDefuseTimer)
	{
		if (!bGotWorldToScreen)
		{
			bGotWorldToScreen = m_pApp->Gui()->WorldToScreen(vCurEntOrigin, vScreenOrigin);
		}

		if (bGotWorldToScreen)
		{
			IClientEntity* pDefuser = m_pApp->EntityList()->GetClientEntityFromHandle(pCurEntity->GetDefuser());
			if (pDefuser && pDefuser->IsDefusing())
			{
				float fTimeToDefused = fmax(pCurEntity->GetDefuseCountDown() - m_pApp->GlobalVars()->curtime, 0.0f);
				if (fTimeToDefused != m_pApp->GlobalVars()->curtime)
				{
					wchar_t pBombTimeText[256];
					swprintf(pBombTimeText, m_pC4DefuseCountDown, fTimeToDefused);
					iPlantedTextHeight += DrawWeaponName(pBombTimeText, vScreenOrigin.x, vScreenOrigin.y + iPlantedTextHeight);
				}
			}
		}
	}


	if (m_bDrawBombDamageIndicator)
	{
		IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
		float flDistance = (*pLocalEntity->GetOrigin() - *pCurEntity->GetOrigin()).Length();

		static float a = 450.7f;
		static float b = 75.68f;
		static float c = 789.2f;

		float d = ((flDistance - b) / c);
		float flDamage = a*exp(-d * d);
		int damage = fmax(((int)DamageIndicatorArmor(flDamage, pLocalEntity->GetArmor()) + 0.5f), 0.0f);

		wchar_t pBombTimeText[256];
		swprintf(pBombTimeText, m_pC4DamageIndicator, damage);
		DrawWeaponName(pBombTimeText, vScreenOrigin.x, vScreenOrigin.y + iPlantedTextHeight);
	}

}

float CWeaponEsp::DamageIndicatorArmor(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue)) {
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}

void CWeaponEsp::DrawWeaponName(CWeapon* pCurEntity, int posX, int posY)
{
	if (!m_bDrawWeaponName)
		return;

	static unsigned int iFont = g_pResourceManager->GetFont(RM_FONT_NORMAL);
	m_pApp->Surface()->DrawSetTextFont(iFont);


	static int iWeaponUnderscoreLen = strlen("weapon_");

	wchar_t wcWeaponName[256];
	mbstowcs(wcWeaponName, pCurEntity->GetWeaponInfo()->szWeaponName, 256);
	DrawWeaponName(wcWeaponName + iWeaponUnderscoreLen, posX, posY);
}

int CWeaponEsp::DrawWeaponName(wchar_t* pWeaponName, int posX, int posY)
{
	static unsigned int iFont = g_pResourceManager->GetFont(RM_FONT_NORMAL);
	m_pApp->Surface()->DrawSetTextFont(iFont);

	int iWeaponNameLen = lstrlenW(pWeaponName);

	int w, h;
	m_pApp->Surface()->GetTextSize(iFont, pWeaponName, w, h);

	m_pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);
	m_pApp->Surface()->DrawSetTextPos(posX - w / 2, posY);
	m_pApp->Surface()->DrawPrintText(pWeaponName, iWeaponNameLen);

	return h;
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
