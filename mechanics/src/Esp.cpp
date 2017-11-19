#include "Esp.h"
#include "Application.h"

CEsp::CEsp() :
	m_xorHeadZero("nä¦H;")
{
	m_iFadeoutTime = 1500;
	m_iWeaponStuffOffset = 0;

	m_clrCT = Color(0, 0, 255);
	m_clrT = Color(255, 0, 0);
	m_clrSpotted = Color(255, 128, 0);

	this->ResetHeadBones();
}

CEsp::~CEsp()
{
}

void CEsp::ResetHeadBones()
{
	m_iHeadBoneCT = -1;
	m_iHeadBoneT = -1;
}

void CEsp::Setup()
{
	IFeature::Setup();

	m_pGui = CGui::Instance();
	m_iFont = g_pResourceManager->GetFont(RM_FONT_NORMAL);
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

	matrix3x4_t pBoneMatrix[MAXSTUDIOBONES];

	// Grab LocalPlayer
	pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return;

	// Grab localplayer info
	iLocalTeam = pLocalEntity->GetTeamNum();
	vMyHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();

	ULONGLONG llTimestamp = GetTickCount64();

	m_iWeaponStuffOffset = 0;

	int iMaxClients = m_pApp->EngineClient()->GetMaxClients();
	for (int i = 0; i < iMaxClients; i++)
	{
		pCurEntity = m_pApp->EntityList()->GetClientEntity(i);
		if (!pCurEntity)
			continue;

		if (pCurEntity->IsDormant())
		{
			if (!m_bFadeoutEnabled)
				continue;

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

		if (m_iHeadBoneCT == -1)
		{
			if (iCurEntityTeam == TEAMNUM_CT)
			{
				m_iHeadBoneCT = pCurEntity->GetBoneByName(m_xorHeadZero.ToCharArray());
			}
		}

		if (m_iHeadBoneT == -1)
		{
			if (iCurEntityTeam == TEAMNUM_T)
			{
				m_iHeadBoneT = pCurEntity->GetBoneByName(m_xorHeadZero.ToCharArray());
			}
		}

		if (!pCurEntity->SetupBones(pBoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, m_pApp->EngineClient()->GetLastTimeStamp()))
			continue;

		Vector vScreenOrigin, vScreenHead, vScreenUpperOrigin;
		Vector vCurEntOrigin = *pCurEntity->GetOrigin();
		Vector vCurEntHeadPos;
		Vector vCurEntUpperOrigin;
		MatrixGetColumn(pBoneMatrix[iCurEntityTeam == TEAMNUM_CT ? m_iHeadBoneCT : m_iHeadBoneT], 3, vCurEntHeadPos);

		vCurEntUpperOrigin = vCurEntHeadPos;
		vCurEntUpperOrigin.z += 5.0f;

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

		if (m_pGui->WorldToScreen(vCurEntOrigin, vScreenOrigin) && m_pGui->WorldToScreen(vCurEntHeadPos, vScreenHead) && m_pGui->WorldToScreen(vCurEntUpperOrigin, vScreenUpperOrigin))
		{
			float height = abs(vScreenUpperOrigin.y - vScreenOrigin.y);
			float width = height * 0.65f;

			DrawBoundingBox(pSurface, vScreenOrigin.x, vScreenOrigin.y, height, width, color);
			DrawName(pSurface, pCurEntity, vScreenOrigin.x, vScreenOrigin.y, height, width, alpha);
			if (m_bDrawSkeleton)
				DrawSkeleton(pSurface, pCurEntity, pBoneMatrix, alpha);
			DrawHealthBar(pSurface, vScreenOrigin.x, vScreenOrigin.y, height, width, iHealth, alpha);
			DrawHealthNumber(pSurface, vScreenOrigin.x, vScreenOrigin.y, height, width, iHealth, alpha);
			DrawAmmoBar(pSurface, pCurEntity, vScreenOrigin.x, vScreenOrigin.y, height, width, alpha);
			DrawAmmoNumber(pSurface, pCurEntity, vScreenOrigin.x, vScreenOrigin.y, alpha);
			DrawActiveWeapon(pSurface, pCurEntity, vScreenOrigin.x, vScreenOrigin.y, alpha);
			DrawArmorBar(pSurface, vScreenOrigin.x, vScreenOrigin.y, height, width, armor, alpha);

			CResolverPlayer* pResolverPlayer = m_pApp->Resolver()->GetResolverPlayer(pCurEntity->EntIndex());
			if (pResolverPlayer)
			{
				pSurface->DrawSetTextColor(alpha, 255, 255, 255);
				pSurface->DrawSetTextFont(m_iFont);

				int w = 0, h = 0;
				if (pResolverPlayer->m_bFakeActive)
				{
					pSurface->GetTextSize(m_iFont, L"FAKE", w, h);

					pSurface->DrawSetTextPos(vScreenOrigin.x + width / 2 + 5, vScreenOrigin.y - height);
					pSurface->DrawPrintText(L"FAKE", strlen("FAKE"));
				}

				if (pResolverPlayer->m_bBreakingLby)
				{
					pSurface->DrawSetTextPos(vScreenOrigin.x + width / 2 + 5, vScreenOrigin.y - height + h);
					pSurface->DrawPrintText(L"BREAKING", strlen("BREAKING"));
				}
			}

			if (false && hasHelmet) //todo: check if hasHelmet
			{
				DrawHelmet(pSurface, vScreenOrigin.x, vScreenOrigin.y, height, width, alpha);
			}

			DrawViewangles(pSurface, vScreenHead.x, vScreenHead.y, vCurEntHeadPos, *pCurEntity->GetAngEyeAngles(), alpha);
		}
	}
}

void CEsp::DrawArmorBar(ISurface* pSurface, int posX, int posY, int height, int width, int armor, int alpha)
{
	if (!m_bDrawArmorBar)
		return;

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
			pSurface->DrawSetColor(color.a(), 0, 0, 0);
			pSurface->DrawOutlinedRect(x1 - 1, y1 - 1, x2 + 1, y2 + 1);
			pSurface->DrawOutlinedRect(x1 + 1, y1 + 1, x2 - 1, y2 - 1);
		}

		pSurface->DrawSetColor(color);
		pSurface->DrawOutlinedRect(x1, y1, x2, y2);
		break;
	case ESP_STYLE_EDGE:
		if (m_bDrawOutline)
		{
			// TODO: Alles auf x1, y1, x2, y2 umschreiben
			//left line top and bottom
			pSurface->DrawSetColor(color.a(), 0, 0, 0);
			pSurface->DrawFilledRect(
				posX - width / 2 - 2,
				posY - height - 6,
				posX - width / 2 + 2,
				posY - height - 4 + (width / 2 - width / 5));
			pSurface->DrawFilledRect(
				posX - width / 2 - 2,
				posY + 5 - (width / 2 - width / 5),
				posX - width / 2 + 2,
				posY + 7);

			// left line top and bottom
			pSurface->DrawFilledRect(
				posX + width / 2 - 2,
				posY - height - 6,
				posX + width / 2 + 2,
				posY - height - 4 + (width / 2 - width / 5));
			pSurface->DrawFilledRect(
				posX + width / 2 - 2,
				posY + 5 - (width / 2 - width / 5),
				posX + width / 2 + 2,
				posY + 7);

			// bottom line left and right
			pSurface->DrawFilledRect(
				posX - width / 2 - 2,
				posY + 3,
				posX - width / 5 + 1,
				posY + 7);
			pSurface->DrawFilledRect(
				posX + width / 5 - 1,
				posY + 3,
				posX + width / 2 + 2,
				posY + 7);

			// top line left and right
			pSurface->DrawFilledRect(
				posX - width / 2 - 2,
				posY - height - 6,
				posX - width / 5 + 1,
				posY - height - 2);
			pSurface->DrawFilledRect(
				posX + width / 5 - 1,
				posY - height - 6,
				posX + width / 2 + 2,
				posY - height - 2);
		}

		pSurface->DrawSetColor(color);
		//left line top and bottom
		pSurface->DrawFilledRect(
			posX - width / 2 - 1,
			posY - height - 5,
			posX - width / 2 + 1,
			posY - height - 5 + (width / 2 - width / 5));
		pSurface->DrawFilledRect(
			posX - width / 2 - 1,
			posY + 6 - (width / 2 - width / 5),
			posX - width / 2 + 1,
			posY + 6);

		// left line top and bottom
		pSurface->DrawFilledRect(
			posX + width / 2 - 1,
			posY - height - 5,
			posX + width / 2 + 1,
			posY - height - 5 + (width / 2 - width / 5));
		pSurface->DrawFilledRect(
			posX + width / 2 - 1,
			posY + 6 - (width / 2 - width / 5),
			posX + width / 2 + 1,
			posY + 6);

		// bottom line left and right
		pSurface->DrawFilledRect(
			posX - width / 2 - 1,
			posY + 4,
			posX - width / 5,
			posY + 6);
		pSurface->DrawFilledRect(
			posX + width / 5,
			posY + 4,
			posX + width / 2 + 1,
			posY + 6);

		//top line left and right
		pSurface->DrawFilledRect(
			posX - width / 2 - 1,
			posY - height - 5,
			posX - width / 5,
			posY - height - 3);
		pSurface->DrawFilledRect(
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

void CEsp::DrawSkeleton(ISurface* pSurface, IClientEntity* pEntity, matrix3x4_t* pBoneMatrix, int alpha)
{
	studiohdr_t* pStudioHdr = m_pApp->ModelInfo()->GetStudiomodel(pEntity->GetModel());
	if (!pStudioHdr)
		return;

	pSurface->DrawSetColor(alpha, 255, 255, 255);

	Vector vBonePos1, vBonePos2;
	Vector vPos1, vPos2;
	for (int i = 0; i < pStudioHdr->numbones; i++)
	{
		mstudiobone_t* pBone = pStudioHdr->pBone(i);
		if (!pBone)
			continue;

		if (!(pBone->flags & FL_CLIENT))
			continue;

		if (pBone->parent == -1)
			continue;

		MatrixGetColumn(pBoneMatrix[i], 3, vBonePos1);
		MatrixGetColumn(pBoneMatrix[pBone->parent], 3, vBonePos2);

		if (!m_pGui->WorldToScreen(vBonePos1, vPos1) || !m_pGui->WorldToScreen(vBonePos2, vPos2))
			continue;

		pSurface->DrawLine(vPos1.x, vPos1.y, vPos2.x, vPos2.y);
	}
}

void CEsp::DrawHealthBar(ISurface* pSurface, int posX, int posY, int height, int width, int health, int alpha)
{
	if (!m_bDrawHealthBar)
		return;

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
	if (!m_bDrawHealthNumber)
		return;

	if (health == 100)
		return;

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
		pSurface->SetFontGlyphSet(font, "Arial Black", 12, 255, 0, 0, 0x200);
	}
	pSurface->DrawSetTextFont(font);

	wchar_t sHealth[16];
	int iLen = swprintf(sHealth, 16, L"%d HP", health);

	int w, h;
	pSurface->GetTextSize(font, sHealth, w, h);

	pSurface->DrawSetTextColor(alpha, 255, 255, 255);
	pSurface->DrawSetTextPos(x1 - w, posY - (height - (height * healthpercentage)) - h);
	pSurface->DrawPrintText(sHealth, iLen);
}

void CEsp::DrawActiveWeapon(ISurface * pSurface, IClientEntity* pEntity, int posX, int posY, int alpha)
{
	if (!m_bDrawActiveWeapon)
		return;

	CWeapon* pActiveWeapon = pEntity->GetActiveWeapon();
	if (!pActiveWeapon)
		return;

	static int iWeaponUnderscoreLen = strlen(/*weapon_*/CXorString("`nä²xeÚ").ToCharArray());

	bool bIsFireableWeapon = !(pActiveWeapon->IsKnife() || pActiveWeapon->IsNade() || pActiveWeapon->IsTaser() || pActiveWeapon->IsC4());
	if (bIsFireableWeapon && (m_bDrawAmmoNumber || m_bDrawAmmoBar))
	{
		int w, h;
		pSurface->GetTextSize(m_iFont, L"", w, h);

		posY += h;
	}

	wchar_t wcWeaponName[256];
	mbstowcs(wcWeaponName, pActiveWeapon->GetWeaponInfo()->szWeaponName, 256);

	DrawWeaponText(pSurface, wcWeaponName + iWeaponUnderscoreLen, posX, posY, alpha);
}

void CEsp::DrawAmmoBar(ISurface* pSurface, IClientEntity* pEntity, int posX, int posY, int height, int width, int alpha)
{
	if (!m_bDrawAmmoBar)
		return;

	CWeapon* pActiveWeapon = pEntity->GetActiveWeapon();
	if (!pActiveWeapon)
		return;

	if (pActiveWeapon->IsKnife() ||
		pActiveWeapon->IsNade() ||
		pActiveWeapon->IsTaser() ||
		pActiveWeapon->IsC4())
		return;

	int iClip1 = pActiveWeapon->GetClip1();
	// Ammo reserver m_iPrimaryReserveAmmoCount
	CWeaponInfo* pWeaponInfo = pActiveWeapon->GetWeaponInfo();
	if (!pWeaponInfo)
		return;

	float ammopercentage = (pWeaponInfo->iMaxClip1 - iClip1) / (float)pWeaponInfo->iMaxClip1;
	int x1 = posX - width / 2;
	int x2 = posX + width / 2;

	int w, h;
	pSurface->GetTextSize(m_iFont, L"", w, h);

	//background
	pSurface->DrawSetColor(alpha, 0, 0, 0);
	pSurface->DrawFilledRect(
		x1,
		posY + h + 1,
		x2,
		posY + h + 6 + 3
	);

	// actual ammo
	pSurface->DrawSetColor(alpha, 0, 0, 255);
	pSurface->DrawFilledRect(
		x1,
		posY + h + 1,
		x2 - (width * ammopercentage),
		posY + h + 6 + 3
	);
}

void CEsp::DrawAmmoNumber(ISurface* pSurface, IClientEntity* pEntity, int posX, int posY, int alpha)
{
	if (!m_bDrawAmmoNumber)
		return;

	CWeapon* pActiveWeapon = pEntity->GetActiveWeapon();
	if (!pActiveWeapon)
		return;

	if (pActiveWeapon->IsKnife() ||
		pActiveWeapon->IsNade() ||
		pActiveWeapon->IsTaser() ||
		pActiveWeapon->IsC4())
		return;

	int iClip1 = pActiveWeapon->GetClip1();
	// Ammo reserver m_iPrimaryReserveAmmoCount
	CWeaponInfo* pWeaponInfo = pActiveWeapon->GetWeaponInfo();
	if (!pWeaponInfo)
		return;

	wchar_t wAmmoNumber[256];
	swprintf(wAmmoNumber, 16, L"%d / %d", iClip1, pWeaponInfo->iMaxClip1);

	DrawWeaponText(pSurface, wAmmoNumber, posX, posY, alpha);
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

void CEsp::DrawName(ISurface* pSurface, IClientEntity* pEntity, int posX, int posY, int height, int width, int alpha)
{
	if (!m_bDrawNames)
		return;

	pSurface->DrawSetTextFont(m_iFont);

	PlayerInfo pInfo;
	pEntity->GetPlayerInfo(&pInfo);

	wchar_t name[256];
	int iLen = pInfo.GetName(name, 256);

	int w, h;
	pSurface->GetTextSize(m_iFont, name, w, h);

	pSurface->DrawSetTextColor(alpha, 255, 255, 255);
	pSurface->DrawSetTextPos(posX - w / 2, posY - height - 17);
	pSurface->DrawPrintText(name, iLen);
}

void CEsp::DrawViewangles(ISurface* pSurface, int headX, int headY, Vector headPos, QAngle angles, int alpha)
{
	if (!m_bDrawViewangles)
		return;

	Vector vForward, vAimPos, vAimPosScreen;

	// Create forward vector & get aim point
	AngleVectors(angles, &vForward);
	vAimPos = headPos + (vForward * m_iViewanglesLength);

	if (m_pGui->WorldToScreen(vAimPos, vAimPosScreen))
	{
		pSurface->DrawSetColor(alpha, 255, 255, 255);
		pSurface->DrawLine(headX, headY, vAimPosScreen.x, vAimPosScreen.y);
	}
}

int CEsp::DrawWeaponText(ISurface* pSurface, wchar_t * pText, int posX, int posY, int alpha)
{
	pSurface->DrawSetTextColor(alpha, 255, 255, 255);
	pSurface->DrawSetTextFont(m_iFont);

	int iLen = lstrlenW(pText);

	int w, h;
	pSurface->GetTextSize(m_iFont, pText, w, h);

	pSurface->DrawSetTextColor(alpha, 255, 255, 255);
	pSurface->DrawSetTextPos(posX - w / 2, posY + h + m_iWeaponStuffOffset);
	pSurface->DrawPrintText(pText, iLen);

	return h;
}
