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
	int localTeam = *(int*)((DWORD)localEntity + TEAM_OFFSET);

	for (int i = 1; i < m_pApp->EntityList()->GetMaxEntities(); i++)
	{
		IClientEntity* pEntity = m_pApp->EntityList()->GetClientEntity(i);

		if (!pEntity)
			continue;
		if (pEntity->IsDormant())
			continue;

		if (m_pApp->EngineClient()->GetLocalPlayer() == i)
			continue;

		int entityTeam = *(int*)((DWORD)pEntity + TEAM_OFFSET);
		if (true && entityTeam == localTeam) //todo: same team
			continue;

		bool isSpotted = *(bool*)((DWORD)pEntity + SPOTTED_OFFSET);
		if (false && entityTeam == localTeam && !isSpotted) //todo: isSpotted
			continue;

		Vector screenOrigin, screenHead;
		Vector headPos = *(Vector*)((DWORD)pEntity + 0x134);
		Vector origin = headPos;

		D3DCOLOR color;
		if (entityTeam == CT_TEAMID)
		{
			color = D3DCOLOR_ARGB(200, 0, 0, 255);
			headPos.z += 71;
		}
		else if (entityTeam == T_TEAMID)
		{
			color = D3DCOLOR_ARGB(200, 255, 0, 0);
			headPos.z += 72;
		}

		if (isSpotted)
		{
			color = D3DCOLOR_ARGB(200, 255, 255, 255);
		}

		int health = *(int*)((DWORD)pEntity + HEALTH_OFFSET);
		if (health == 0)
			continue;

		DWORD flag = *(DWORD*)((DWORD)pEntity + JUMP_FLAG_OFFSET);
		if (flag & IN_DUCK)
		{
			headPos.z -= 17;
		}

		//todo: both interesting for knifebot
		int armor = *(int*)((DWORD)pEntity + ARMOR_OFFSET);
		bool hasHelmet = *(bool*)((DWORD)pEntity + HELMET_OFFSET);

		if (WorldToScreen(origin, screenOrigin) && WorldToScreen(headPos, screenHead))
		{
			float height = abs(screenHead.y - screenOrigin.y);
			float width = height * 0.65f;

			if (EnableArmorbar && armor > 0) //todo: check if armorbar
			{
				DrawArmorBar((IDirect3DDevice9*)pParameters, screenOrigin.x, screenOrigin.y, height, width, armor);
				m_DrawArmorbar = true;
			}
			else
				m_DrawArmorbar = false;
			if (true) //todo: check if bounding box
				DrawBoundingBox((IDirect3DDevice9*)pParameters, screenOrigin.x, screenOrigin.y, height, width, color);
			if (EnableHealthbar) //todo: check if healthbar
				DrawHealthBar((IDirect3DDevice9*)pParameters, screenOrigin.x, screenOrigin.y, height, width, health);
			if (false && hasHelmet) //todo: check if hasHelmet
				DrawHelmet((IDirect3DDevice9*)pParameters, screenOrigin.x, screenOrigin.y, height, width);
		}
	}
}

void CEsp::DrawArmorBar(IDirect3DDevice9* pDevice, int posX, int posY, int height, int width, int armor)
{
	float armorpercentage = (100 - armor) / 100.0f;
	D3DRECT armorbackground = {
		posX - width / 2 - 7,
		posY - height - 5,
		posX - width / 2 - 3,
		posY + 6 };
	D3DRECT armorbar = {
		posX - width / 2 - 7,
		posY - (height - (height * armorpercentage)) - 5,
		posX - width / 2 - 3,
		posY + 6 };

	pDevice->Clear(1, &armorbackground, D3DCLEAR_TARGET, D3DCOLOR_ARGB(200, 0, 0, 0), 0, 0);
	pDevice->Clear(1, &armorbar, D3DCLEAR_TARGET, D3DCOLOR_ARGB(200, 128, 128, 128), 0, 0);
}
void CEsp::DrawBoundingBox(IDirect3DDevice9* pDevice, int posX, int posY, int height, int width, D3DCOLOR color)
{
	D3DRECT rightLineTop = {
		posX - width / 2 - 1,
		posY - height - 5,
		posX - width / 2 + 1,
		posY - height - 5 + (width / 2 - width / 5) };
	D3DRECT rightLineBottom = {
		posX - width / 2 - 1,
		posY + 6 - (width / 2 - width / 5),
		posX - width / 2 + 1,
		posY + 6 };

	D3DRECT leftLineTop = {
		posX + width / 2 - 1,
		posY - height - 5,
		posX + width / 2 + 1,
		posY - height - 5 + (width / 2 - width / 5) };
	D3DRECT leftLineBottom = {
		posX + width / 2 - 1,
		posY + 6 - (width / 2 - width / 5),
		posX + width / 2 + 1,
		posY + 6 };

	D3DRECT bottomLineLeft = {
		posX - width / 2 - 1,
		posY + 4,
		posX - width / 5,
		posY + 6 };
	D3DRECT bottomLineRight = {
		posX + width / 5,
		posY + 4,
		posX + width / 2 + 1,
		posY + 6 };

	D3DRECT topLineLeft = {
		posX - width / 2 - 1,
		posY - height - 5,
		posX - width / 5,
		posY - height - 3 };
	D3DRECT topLineRight = {
		posX + width / 5,
		posY - height - 5,
		posX + width / 2 + 1,
		posY - height - 3 };

	pDevice->Clear(1, &rightLineTop, D3DCLEAR_TARGET, color, 0, 0);
	pDevice->Clear(1, &rightLineBottom, D3DCLEAR_TARGET, color, 0, 0);

	pDevice->Clear(1, &leftLineTop, D3DCLEAR_TARGET, color, 0, 0);
	pDevice->Clear(1, &leftLineBottom, D3DCLEAR_TARGET, color, 0, 0);

	pDevice->Clear(1, &bottomLineLeft, D3DCLEAR_TARGET, color, 0, 0);
	pDevice->Clear(1, &bottomLineRight, D3DCLEAR_TARGET, color, 0, 0);

	pDevice->Clear(1, &topLineLeft, D3DCLEAR_TARGET, color, 0, 0);
	pDevice->Clear(1, &topLineRight, D3DCLEAR_TARGET, color, 0, 0);
}
void CEsp::DrawHealthBar(IDirect3DDevice9* pDevice, int posX, int posY, int height, int width, int health)
{
	float healthpercentage = (100 - health) / 100.0f;
	int x1 = posX - width / 2 - 7;
	int x2 = posX - width / 2 - 3;
	if (m_DrawArmorbar)
	{
		x1 -= 6;
		x2 -= 6;
	}
	D3DRECT healthbackground = {
		x1,
		posY - height - 5,
		x2,
		posY + 6 };
	D3DRECT healthbar = {
		x1,
		posY - (height - (height * healthpercentage)) - 5,
		x2,
		posY + 6 };

	pDevice->Clear(1, &healthbackground, D3DCLEAR_TARGET, D3DCOLOR_ARGB(200, 0, 0, 0), 0, 0);
	pDevice->Clear(1, &healthbar, D3DCLEAR_TARGET, D3DCOLOR_ARGB(200, 0, 255, 0), 0, 0);
}
void CEsp::DrawHelmet(IDirect3DDevice9* pDevice, int posX, int posY, int height, int width)
{
	//todo: iwie symbol zeichnen oder sonst etwas
	/*D3DRECT helmet = {
		posX + width / 2 + 4,
		posY - height - 5,
		posX + width / 2 + 8,
		posY - height - 1 };

	pDevice->Clear(1, &helmet, D3DCLEAR_TARGET, D3DCOLOR_ARGB(200, 83, 83, 83), 0, 0);*/
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