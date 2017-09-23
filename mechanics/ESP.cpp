#include "Esp.h"
#include "Application.h"

CEsp::CEsp()
{
	m_iFadeoutTime = 1500;
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

	// Grab LocalPlayer
	pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	if (!pLocalEntity)
		return;

	// Grab localplayer info
	iLocalTeam = pLocalEntity->GetTeamNum();

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

		bool bIsSpotted = pCurEntity->IsSpotted();
		if (!bIsSpotted && m_bDrawOnlySpotted)
			continue;

		Vector vScreenOrigin, vScreenHead;
		Vector vOrigin = *pCurEntity->GetOrigin();
		Vector vHeadOrigin = vOrigin + *pCurEntity->GetEyeOffset();

		Color color;
		if (m_pPastPlayers[i].GetIsDormant())
		{
			int alpha = ((m_iFadeoutTime - (llTimestamp - m_pPastPlayers[i].GetTimestamp())) / (float)m_iFadeoutTime) * 255;
			g_pConsole->Write("%d\n", alpha);
			color = Color(alpha, 100, 100, 100);
		}
		else if (iCurEntityTeam == TEAMNUM_CT)
		{
			color = Color(0, 0, 255);
			//headPos.z += 71;
		}
		else if (iCurEntityTeam == TEAMNUM_T)
		{
			color = Color(255, 0, 0);
			//headPos.z += 72;
		}
		else
		{
			continue;
		}

		if (bIsSpotted)
		{
			color = Color(255, 51, 255);
		}

		DWORD dwFlags = pCurEntity->GetFlags();
		//if (dwFlags & IN_DUCK)
		//{
		//	headPos.z -= 17; // TODO
		//}

		//todo: both interesting for knifebot
		int iHealth = pCurEntity->GetHealth();
		int armor = pCurEntity->GetArmor();
		bool hasHelmet = pCurEntity->HasHelmet();

		if (m_pGui->WorldToScreen(vOrigin, vScreenOrigin) && m_pGui->WorldToScreen(vHeadOrigin, vScreenHead))
		{
			float height = abs(vScreenHead.y - vScreenOrigin.y);
			float width = height * 0.65f;

			if (m_bDrawBoundingBox)
			{
				DrawBoundingBox(vScreenOrigin.x, vScreenOrigin.y, height, width, color);
			}
			if (m_bDrawNames)
			{
				DrawName(pCurEntity, vScreenOrigin.x, vScreenOrigin.y, height, width);
			}
			if (m_bDrawHealthBar)
			{
				DrawHealthBar(pSurface, vScreenOrigin.x, vScreenOrigin.y, height, width, iHealth);
			}
			if (m_bDrawHealthNumber && iHealth < 100)
			{
				DrawHealthNumber(vScreenOrigin.x, vScreenOrigin.y, height, width, iHealth);
			}

			if (m_bDrawArmorBar)
			{
				DrawArmorBar(vScreenOrigin.x, vScreenOrigin.y, height, width, armor);
			}
			if (false && hasHelmet) //todo: check if hasHelmet
			{
				DrawHelmet(vScreenOrigin.x, vScreenOrigin.y, height, width);
			}
		}
	}
}

void CEsp::DrawArmorBar(int posX, int posY, int height, int width, int armor)
{
	float armorpercentage = (100 - armor) / 100.0f;

	//background
	m_pApp->Surface()->DrawSetColor(255, 0, 0, 0);
	m_pApp->Surface()->DrawFilledRect(
		posX - width / 2 - 7,
		posY - height - 5,
		posX - width / 2 - 3,
		posY + 6);
	// actual armor
	m_pApp->Surface()->DrawSetColor(255, 128, 128, 128);
	m_pApp->Surface()->DrawFilledRect(
		posX - width / 2 - 7,
		posY - (height - (height * armorpercentage)) - 5,
		posX - width / 2 - 3,
		posY + 6);
}
void CEsp::DrawBoundingBox(int posX, int posY, int height, int width, Color color)
{
	if (this->m_bDrawOutline)
	{
		//left line top and bottom
		m_pApp->Surface()->DrawSetColor(255, 0, 0, 0);
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
}
void CEsp::DrawHealthBar(ISurface* pSurface, int posX, int posY, int height, int width, int health)
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
	pSurface->DrawSetColor(255, 0, 0, 0);
	pSurface->DrawFilledRect(
		x1,
		posY - height - 5,
		x2,
		posY + 6
	);

	// actual health
	pSurface->DrawSetColor(255, 0, 255, 0);
	pSurface->DrawFilledRect(
		x1,
		((posY - height) + (height * healthpercentage)) - 5,
		x2,
		posY + 6
	);
}
void CEsp::DrawHealthNumber(int posX, int posY, int height, int width, int health)
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

	m_pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);
	m_pApp->Surface()->DrawSetTextPos(x1 - w, posY - (height - (height * healthpercentage)) - h);
	m_pApp->Surface()->DrawPrintText(sHealth, iLen);
}
void CEsp::DrawHelmet(int posX, int posY, int height, int width)
{
	//todo: iwie symbol zeichnen oder sonst etwas
	/*D3DRECT helmet = {
		posX + width / 2 + 4,
		posY - height - 5,
		posX + width / 2 + 8,
		posY - height - 1 };

	pDevice->Clear(1, &helmet, D3DCLEAR_TARGET, D3DCOLOR_ARGB(200, 83, 83, 83), 0, 0);*/
}
void CEsp::DrawName(IClientEntity* pEntity, int posX, int posY, int height, int width) {
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

	m_pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);
	m_pApp->Surface()->DrawSetTextPos(posX - w / 2, posY - height - 17);
	m_pApp->Surface()->DrawPrintText(name, iLen);
}
