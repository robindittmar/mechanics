#include "Esp.h"
#include "Application.h"

CEsp::CEsp()
{
	m_iFadeoutTime = 1500;

	m_clrCT = Color(0, 0, 255);
	m_clrT = Color(255, 0, 0);
	m_clrSpotted = Color(255, 128, 0);
}

CEsp::~CEsp()
{
}

void CEsp::Setup()
{
	m_pApp = CApplication::Instance();
	m_pGui = CGui::Instance();
}

void CEsp::Update(void* pParameters)
{
	assert(pParameters != NULL);

	if (!m_bIsEnabled)
		return;

	ISurface* pSurface = (ISurface*)pParameters;

	IClientEntity* pLocalEntity;
	IClientEntity* pCurEntity;
	int iLocalTeam;
	Vector vMyHeadPos;

	// Grab LocalPlayer
	pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	if (!pLocalEntity)
		return;

	// Grab localplayer info
	iLocalTeam = pLocalEntity->GetTeamNum();
	vMyHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();

	ULONGLONG llTimestamp = GetTickCount64();

	int iMaxClients = m_pApp->EngineClient()->GetMaxClients();
	for (int i = 0; i < iMaxClients; i++)
	{
		pCurEntity = m_pApp->EntityList()->GetClientEntity(i);
		if (!pCurEntity)
			continue;

		if (pCurEntity->IsDormant())
		{
			// If it was NOT dormant before (first tick player is dormant)
			if (!m_pPastPlayers[i].GetIsDormant())
			{
				// Remember timestamp and dormant
				m_pPastPlayers[i].SetTimestamp(llTimestamp);
				m_pPastPlayers[i].SetIsDormant(true);
			}
			// If our player is dormant for a longer time than we want it to fadeout
			else if (llTimestamp - m_pPastPlayers[i].GetTimestamp() > m_iFadeoutTime)
			{
				continue;
			}
		}
		else
		{
			m_pPastPlayers[i].SetIsDormant(false);
		}

		if (!pCurEntity->IsAlive())
			continue;

		bool bIsLocalPlayer = pLocalEntity == pCurEntity;
		int iCurEntityTeam = pCurEntity->GetTeamNum();

		if (!(bIsLocalPlayer && m_pApp->Visuals()->GetThirdperson() && m_bDrawOwnModel ||
			!bIsLocalPlayer && m_bDrawOwnTeam && iCurEntityTeam == iLocalTeam ||
			iCurEntityTeam != iLocalTeam) ||
			iCurEntityTeam == 0)
			continue;

		Vector vScreenOrigin, vScreenHead;
		Vector vCurEntOrigin = *pCurEntity->GetOrigin();
		Vector vCurEntHeadPos = vCurEntOrigin + *pCurEntity->GetEyeOffset();

		Ray_t ray;
		trace_t trace;
		CTraceFilterSkipEntity traceFilter(pLocalEntity);

		ray.Init(vMyHeadPos, vCurEntHeadPos);
		m_pApp->EngineTrace()->TraceRay(ray, MASK_VISIBLE, &traceFilter, &trace);

		bool bIsSpotted = pCurEntity->IsSpotted();
		if (!bIsSpotted && m_bDrawOnlySpotted)
		{
			if (!m_bDrawOnlyVisible || (!trace.IsEntityVisible(pCurEntity) && m_bDrawOnlyVisible))
				continue;
		}

		if (!trace.IsEntityVisible(pCurEntity) && m_bDrawOnlyVisible)
		{
			if (!m_bDrawOnlySpotted || (!bIsSpotted && m_bDrawOnlySpotted))
				continue;
		}

		int alpha = 255;
		Color color;
		if (m_pPastPlayers[i].GetIsDormant())
		{
			alpha = ((m_iFadeoutTime - (llTimestamp - m_pPastPlayers[i].GetTimestamp())) / (float)m_iFadeoutTime) * 255;
			color = Color(alpha, 100, 100, 100);
		}
		else if (iCurEntityTeam == TEAMNUM_CT)
		{
			color = m_clrCT;
		}
		else if (iCurEntityTeam == TEAMNUM_T)
		{
			color = m_clrT;
		}
		else
		{
			continue;
		}

		if (bIsSpotted)
		{
			color = m_clrSpotted;
		}

		//todo: both interesting for knifebot
		int iHealth = pCurEntity->GetHealth();
		int armor = pCurEntity->GetArmor();
		bool hasHelmet = pCurEntity->HasHelmet();

		if (m_pGui->WorldToScreen(vCurEntOrigin, vScreenOrigin) && m_pGui->WorldToScreen(vCurEntHeadPos, vScreenHead))
		{
			float height = abs(vScreenHead.y - vScreenOrigin.y);
			float width = height * 0.65f;

			DrawBoundingBox(pSurface, vScreenOrigin.x, vScreenOrigin.y, height, width, color);
			if (m_bDrawNames)
			{
				DrawName(pSurface, pCurEntity, vScreenOrigin.x, vScreenOrigin.y, height, width, alpha);
			}
			if (m_bDrawHealthBar)
			{
				DrawHealthBar(pSurface, vScreenOrigin.x, vScreenOrigin.y, height, width, iHealth, alpha);
			}
			if (m_bDrawHealthNumber && iHealth < 100)
			{
				DrawHealthNumber(pSurface, vScreenOrigin.x, vScreenOrigin.y, height, width, iHealth, alpha);
			}

			if (m_bDrawArmorBar)
			{
				DrawArmorBar(pSurface, vScreenOrigin.x, vScreenOrigin.y, height, width, armor, alpha);
			}
			if (false && hasHelmet) //todo: check if hasHelmet
			{
				DrawHelmet(pSurface, vScreenOrigin.x, vScreenOrigin.y, height, width, alpha);
			}
		}
	}
}

void CEsp::DrawArmorBar(ISurface* pSurface, int posX, int posY, int height, int width, int armor, int alpha)
{
	float armorpercentage = (100 - armor) / 100.0f;

	//background
	pSurface->DrawSetColor(alpha, 0, 0, 0);
	pSurface->DrawFilledRect(
		posX - width / 2 - 7,
		posY - height - 5,
		posX - width / 2 - 3,
		posY + 6);
	// actual armor
	pSurface->DrawSetColor(alpha, 128, 128, 128);
	pSurface->DrawFilledRect(
		posX - width / 2 - 7,
		posY - (height - (height * armorpercentage)) - 5,
		posX - width / 2 - 3,
		posY + 6);
}
void CEsp::DrawBoundingBox(ISurface* pSurface, int posX, int posY, int height, int width, Color color)
{
	int x1 = posX - width / 2 - 1;	// left
	int y1 = posY - height - 5;		// up
	int x2 = posX + width / 2 + 1;	// right
	int y2 = posY + 6;				// down

	if (m_bFillBoundingBox)
	{
		pSurface->DrawSetColor((color.a() / 255.0f) * 128, 50, 50, 50);
		pSurface->DrawFilledRect(x1, y1, x2, y2);
	}

	switch (m_iDrawBoundingBox)
	{
	case ESP_STYLE_FULL:
		if (m_bDrawOutline)
		{
			m_pApp->Surface()->DrawSetColor(color.a(), 0, 0, 0);
			m_pApp->Surface()->DrawOutlinedRect(x1 - 1, y1 - 1, x2 + 1, y2 + 1);
			m_pApp->Surface()->DrawOutlinedRect(x1 + 1, y1 + 1, x2 - 1, y2 - 1);
		}

		m_pApp->Surface()->DrawSetColor(color);
		m_pApp->Surface()->DrawOutlinedRect(x1, y1, x2, y2);
		break;
	case ESP_STYLE_EDGE:
		if (m_bDrawOutline)
		{
			// TODO: Alles auf x1, y1, x2, y2 umschreiben
			//left line top and bottom
			m_pApp->Surface()->DrawSetColor(color.a(), 0, 0, 0);
			m_pApp->Surface()->DrawFilledRect(
				posX - width / 2 - 2,
				posY - height - 6,
				posX - width / 2 + 2,
				posY - height - 4 + (width / 2 - width / 5));
			m_pApp->Surface()->DrawFilledRect(
				posX - width / 2 - 2,
				posY + 5 - (width / 2 - width / 5),
				posX - width / 2 + 2,
				posY + 7);

			// left line top and bottom
			m_pApp->Surface()->DrawFilledRect(
				posX + width / 2 - 2,
				posY - height - 6,
				posX + width / 2 + 2,
				posY - height - 4 + (width / 2 - width / 5));
			m_pApp->Surface()->DrawFilledRect(
				posX + width / 2 - 2,
				posY + 5 - (width / 2 - width / 5),
				posX + width / 2 + 2,
				posY + 7);

			// bottom line left and right
			m_pApp->Surface()->DrawFilledRect(
				posX - width / 2 - 2,
				posY + 3,
				posX - width / 5 + 1,
				posY + 7);
			m_pApp->Surface()->DrawFilledRect(
				posX + width / 5 - 1,
				posY + 3,
				posX + width / 2 + 2,
				posY + 7);

			// top line left and right
			m_pApp->Surface()->DrawFilledRect(
				posX - width / 2 - 2,
				posY - height - 6,
				posX - width / 5 + 1,
				posY - height - 2);
			m_pApp->Surface()->DrawFilledRect(
				posX + width / 5 - 1,
				posY - height - 6,
				posX + width / 2 + 2,
				posY - height - 2);
		}

		m_pApp->Surface()->DrawSetColor(color);
		//left line top and bottom
		m_pApp->Surface()->DrawFilledRect(
			posX - width / 2 - 1,
			posY - height - 5,
			posX - width / 2 + 1,
			posY - height - 5 + (width / 2 - width / 5));
		m_pApp->Surface()->DrawFilledRect(
			posX - width / 2 - 1,
			posY + 6 - (width / 2 - width / 5),
			posX - width / 2 + 1,
			posY + 6);

		// left line top and bottom
		m_pApp->Surface()->DrawFilledRect(
			posX + width / 2 - 1,
			posY - height - 5,
			posX + width / 2 + 1,
			posY - height - 5 + (width / 2 - width / 5));
		m_pApp->Surface()->DrawFilledRect(
			posX + width / 2 - 1,
			posY + 6 - (width / 2 - width / 5),
			posX + width / 2 + 1,
			posY + 6);

		// bottom line left and right
		m_pApp->Surface()->DrawFilledRect(
			posX - width / 2 - 1,
			posY + 4,
			posX - width / 5,
			posY + 6);
		m_pApp->Surface()->DrawFilledRect(
			posX + width / 5,
			posY + 4,
			posX + width / 2 + 1,
			posY + 6);

		//top line left and right
		m_pApp->Surface()->DrawFilledRect(
			posX - width / 2 - 1,
			posY - height - 5,
			posX - width / 5,
			posY - height - 3);
		m_pApp->Surface()->DrawFilledRect(
			posX + width / 5,
			posY - height - 5,
			posX + width / 2 + 1,
			posY - height - 3);
		break;
	case ESP_STYLE_NONE:
	default:
		break;
	}
}
void CEsp::DrawHealthBar(ISurface* pSurface, int posX, int posY, int height, int width, int health, int alpha)
{
	float healthpercentage = (100 - health) / 100.0f;
	int x1 = posX - width / 2 - 8;
	int x2 = posX - width / 2 - 4;
	if (m_bDrawArmorBar)
	{
		x1 -= 6;
		x2 -= 6;
	}

	//background
	pSurface->DrawSetColor(alpha, 0, 0, 0);
	pSurface->DrawFilledRect(
		x1,
		posY - height - 5,
		x2,
		posY + 6
	);

	// actual health
	pSurface->DrawSetColor(alpha, 0, 255, 0);
	pSurface->DrawFilledRect(
		x1,
		((posY - height) + (height * healthpercentage)) - 5,
		x2,
		posY + 6
	);
}
void CEsp::DrawHealthNumber(ISurface* pSurface, int posX, int posY, int height, int width, int health, int alpha)
{
	float healthpercentage = (100 - health) / 100.0f;
	int x1 = posX - width / 2;
	if (m_bDrawArmorBar)
	{
		x1 -= 6;
	}
	static unsigned long font = NULL;
	if (font == NULL)
	{
		font = m_pApp->Surface()->SCreateFont();
		m_pApp->Surface()->SetFontGlyphSet(font, "Arial", 12, 255, 0, 0, 0x200);
	}
	m_pApp->Surface()->DrawSetTextFont(font);

	wchar_t sHealth[16];
	int iLen = swprintf(sHealth, 16, L"%d", health);

	int w, h;
	m_pApp->Surface()->GetTextSize(font, sHealth, w, h);

	m_pApp->Surface()->DrawSetTextColor(alpha, 255, 255, 255);
	m_pApp->Surface()->DrawSetTextPos(x1 - w, posY - (height - (height * healthpercentage)) - h);
	m_pApp->Surface()->DrawPrintText(sHealth, iLen);
}
void CEsp::DrawHelmet(ISurface* pSurface, int posX, int posY, int height, int width, int alpha)
{
	//todo: iwie symbol zeichnen oder sonst etwas
	/*D3DRECT helmet = {
		posX + width / 2 + 4,
		posY - height - 5,
		posX + width / 2 + 8,
		posY - height - 1 };

	pDevice->Clear(1, &helmet, D3DCLEAR_TARGET, D3DCOLOR_ARGB(200, 83, 83, 83), 0, 0);*/
}
void CEsp::DrawName(ISurface* pSurface, IClientEntity* pEntity, int posX, int posY, int height, int width, int alpha) {
	// TODO
	static unsigned long font = NULL;
	if (font == NULL)
	{
		font = m_pApp->Surface()->SCreateFont();
		m_pApp->Surface()->SetFontGlyphSet(font, "Arial", 12, 255, 0, 0, 0x200);
	}
	m_pApp->Surface()->DrawSetTextFont(font);

	PlayerInfo pInfo;
	pEntity->GetPlayerInfo(&pInfo);

	wchar_t name[256];
	int iLen = pInfo.GetName(name, 256);

	int w, h;
	m_pApp->Surface()->GetTextSize(font, name, w, h);

	m_pApp->Surface()->DrawSetTextColor(alpha, 255, 255, 255);
	m_pApp->Surface()->DrawSetTextPos(posX - w / 2, posY - height - 17);
	m_pApp->Surface()->DrawPrintText(name, iLen);
}
