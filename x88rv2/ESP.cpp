#include "ESP.h"
#include "Application.h"

CEsp::CEsp()
{
}

CEsp::~CEsp()
{
}

void CEsp::Setup()
{
	m_pApp = CApplication::Instance();
}

void CEsp::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	IClientEntity* localEntity = (IClientEntity*)m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	int localTeam = localEntity->TeamNum();

	for (int i = 1; i < m_pApp->EntityList()->GetMaxEntities(); i++)
	{
		IClientEntity* pEntity = m_pApp->EntityList()->GetClientEntity(i);

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;

		bool isLocalPlayer = m_pApp->EngineClient()->GetLocalPlayer() == i;
		int entityTeam = pEntity->TeamNum();

		if (!(isLocalPlayer && m_pApp->Visuals()->IsThirdperson() && m_bDrawOwnModel ||
			!isLocalPlayer && m_bDrawOwnTeam && entityTeam == localTeam ||
			entityTeam != localTeam) ||
			entityTeam == 0)
			continue;

		bool isSpotted = pEntity->IsSpotted();
		if (!isSpotted && m_bDrawOnlySpotted)
			continue;

		Vector screenOrigin, screenHead;
		Vector headPos = *(Vector*)((DWORD)pEntity + 0x134);
		Vector origin = headPos;

		Color color;
		if (entityTeam == CT_TEAMID)
		{
			color = Color(0, 0, 255);
			headPos.z += 71;
		}
		else if (entityTeam == T_TEAMID)
		{
			color = Color(255, 0, 0);
			headPos.z += 72;
		}
		else
		{
			continue;
		}

		if (isSpotted)
		{
			color = Color(255, 51, 255);
		}

		int health = pEntity->Health();
		if (health == 0)
			continue;

		DWORD flags = pEntity->Flags();
		if (flags & IN_DUCK)
		{
			headPos.z -= 17;
		}

		//todo: both interesting for knifebot
		int armor = pEntity->Armor();
		bool hasHelmet = pEntity->HasHelmet();

		if (WorldToScreen(origin, screenOrigin) && WorldToScreen(headPos, screenHead))
		{
			float height = abs(screenHead.y - screenOrigin.y);
			float width = height * 0.65f;

			if (m_bDrawBoundingBox)
			{
				DrawBoundingBox(screenOrigin.x, screenOrigin.y, height, width, color);
			}
			if (m_bDrawNames)
			{
				DrawName(pEntity, screenOrigin.x, screenOrigin.y, height, width);
			}
			if (m_bDrawHealthBar)
			{
				DrawHealthBar(screenOrigin.x, screenOrigin.y, height, width, health);
			}
			if (m_bDrawHealthNumber && health < 100)
			{
				DrawHealthNumber(screenOrigin.x, screenOrigin.y, height, width, health);
			}

			if (m_bDrawArmorBar)
			{
				DrawArmorBar(screenOrigin.x, screenOrigin.y, height, width, armor);
			}
			if (false && hasHelmet) //todo: check if hasHelmet
			{
				DrawHelmet(screenOrigin.x, screenOrigin.y, height, width);
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
void CEsp::DrawHealthBar(int posX, int posY, int height, int width, int health)
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
	m_pApp->Surface()->DrawSetColor(255, 0, 0, 0);
	m_pApp->Surface()->DrawFilledRect(
		x1,
		posY - height - 5,
		x2,
		posY + 6);

	// actual health
	m_pApp->Surface()->DrawSetColor(255, 0, 255, 0);
	m_pApp->Surface()->DrawFilledRect(
		x1,
		posY - (height - (height * healthpercentage)) - 5,
		x2,
		posY + 6);
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
	static unsigned long font = NULL;
	if (font == NULL)
	{
		font = m_pApp->Surface()->SCreateFont();
		m_pApp->Surface()->SetFontGlyphSet(font, "Arial", 12, 255, 0, 0, 0x200);
	}
	m_pApp->Surface()->DrawSetTextFont(font);

	PlayerInfo pInfo = pEntity->GetPlayerInfo();
	wchar_t name[256];
	int iLen = pInfo.GetName(name, 256);

	int w, h;
	m_pApp->Surface()->GetTextSize(font, name, w, h);

	m_pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);
	m_pApp->Surface()->DrawSetTextPos(posX - w / 2, posY - height - 17);
	m_pApp->Surface()->DrawPrintText(name, iLen);
}



bool CEsp::ScreenTransform(const Vector& point, Vector& screen) {
	const VMatrix& w2sMatrix = m_pApp->EngineClient()->WorldToScreenMatrix();
	screen.x = w2sMatrix.m[0][0] * point.x + w2sMatrix.m[0][1] * point.y + w2sMatrix.m[0][2] * point.z + w2sMatrix.m[0][3];
	screen.y = w2sMatrix.m[1][0] * point.x + w2sMatrix.m[1][1] * point.y + w2sMatrix.m[1][2] * point.z + w2sMatrix.m[1][3];
	screen.z = 0.0f;

	float w = w2sMatrix.m[3][0] * point.x + w2sMatrix.m[3][1] * point.y + w2sMatrix.m[3][2] * point.z + w2sMatrix.m[3][3];

	if (w < 0.001f) {
		screen.x *= 100000;
		screen.y *= 100000;
		return true;
	}

	float invw = 1.0f / w;
	screen.x *= invw;
	screen.y *= invw;

	return false;
}
bool CEsp::WorldToScreen(const Vector &origin, Vector &screen) {
	if (!ScreenTransform(origin, screen)) {
		int iScreenWidth, iScreenHeight;
		m_pApp->EngineClient()->GetScreenSize(iScreenWidth, iScreenHeight);

		screen.x = (iScreenWidth / 2.0f) + (screen.x * iScreenWidth) / 2;
		screen.y = (iScreenHeight / 2.0f) - (screen.y * iScreenHeight) / 2;

		return true;
	}
	return false;
}