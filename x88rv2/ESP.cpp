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
		if (false && entityTeam == localTeam) //todo: same team
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

		//todo: check if crouched

		int health = *(DWORD*)((DWORD)pEntity + HEALTH_OFFSET);
		if (health == 0)
			continue;

		if (WorldToScreen(origin, screenOrigin) && WorldToScreen(headPos, screenHead))
		{
			float height = abs(screenHead.y - screenOrigin.y);
			float width = height * 0.65f;

			DrawBoundingBox((IDirect3DDevice9*)pParameters, screenOrigin.x, screenOrigin.y, height, width, color);
		}
	}
}

void CEsp::DrawBoundingBox(IDirect3DDevice9* pDevice, int posX, int posY, int height, int width, D3DCOLOR color)
{
	D3DRECT rightLineTop = {
		posX - width / 2 - 1,
		posY - height - 5,
		posX - width / 2 + 1,
		posY - height - 5 + (width / 2 - width / 5)};
	D3DRECT rightLineBottom = {
		posX - width / 2 - 1,
		posY + 6 - (width / 2 - width / 5),
		posX - width / 2 + 1,
		posY + 6};

	D3DRECT leftLineTop = {
		posX + width / 2 - 1,
		posY - height - 5,
		posX + width / 2 + 1,
		posY - height - 5 + (width / 2 - width / 5)};
	D3DRECT leftLineBottom = {
		posX + width / 2 - 1,
		posY + 6 - (width / 2 - width / 5),
		posX + width / 2 + 1,
		posY + 6};

	D3DRECT bottomLineLeft = {
		posX - width / 2 - 1,
		posY + 4,
		posX - width / 5,
		posY + 6};
	D3DRECT bottomLineRight = {
		posX + width / 5,
		posY + 4,
		posX + width / 2 + 1,
		posY + 6};

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