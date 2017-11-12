#include "Misc.h"
#include "Application.h"

CMisc::CMisc()
	: m_xorName("yjËß")
{
	m_bIsCustomClanTag = false;
}

CMisc::~CMisc()
{
}

void CMisc::Setup()
{
	IFeature::Setup();

	m_pSetClanTag = (SetClanTag_t)CPattern::FindPattern((BYTE*)m_pApp->EngineDll(), 0x8C7000, (BYTE*)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", "adhgezvel");
}

void CMisc::Update(void* pParameters)
{
}

void CMisc::NoRecoil(CUserCmd* pUserCmd)
{
	if (!m_bNoRecoil ||
		m_pApp->Ragebot()->DidNoRecoil() ||
		!(pUserCmd->buttons & IN_ATTACK))
	{
		m_pApp->m_oldAimPunchAngle.x = 0;
		m_pApp->m_oldAimPunchAngle.y = 0;
		return;
	}

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	if (!pLocalEntity)
		return;

	CWeapon* pActiveWeapon = (CWeapon*)pLocalEntity->GetActiveWeapon();
	if (!pActiveWeapon)
		return;

	if (pActiveWeapon->IsNade() ||
		pActiveWeapon->IsPistol() && !m_pApp->Visuals()->GetNoVisualRecoil()) //todo: maybe norecoil while pistol
		return;

	if (m_pApp->Ragebot()->DidNoRecoil())
		return;

	int shotsFired = pLocalEntity->GetShotsFired();
	if (m_pApp->Visuals()->GetNoVisualRecoil())
	{
		QAngle aimPunch = *pLocalEntity->GetAimPunchAngle();
		pUserCmd->viewangles[0] -= aimPunch.x * m_pApp->GetRecoilCompensation();
		pUserCmd->viewangles[1] -= aimPunch.y * m_pApp->GetRecoilCompensation();
	}
	else
	{
		m_pApp->EngineClient()->GetViewAngles(m_pApp->m_viewAngle);
		QAngle aimPunchAngle = *pLocalEntity->GetAimPunchAngle();

		m_pApp->m_viewAngle.x += (m_pApp->m_oldAimPunchAngle.x - aimPunchAngle.x * m_pApp->GetRecoilCompensation());
		m_pApp->m_viewAngle.y += (m_pApp->m_oldAimPunchAngle.y - aimPunchAngle.y * m_pApp->GetRecoilCompensation());

		m_pApp->SetClientViewAngles(m_pApp->m_viewAngle);

		m_pApp->m_oldAimPunchAngle.x = aimPunchAngle.x * m_pApp->GetRecoilCompensation();
		m_pApp->m_oldAimPunchAngle.y = aimPunchAngle.y * m_pApp->GetRecoilCompensation();
	}
}

//void CMisc::Fakelag(CUserCmd* pUserCmd)
//{
//	if (!m_bFakelag)
//		return;
//
//	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
//	if (pLocalEntity->GetVelocity()->Length() <= 0.1f)
//	{
//		m_iFakelagChokedAmount = 0;
//		return;
//	}
//
//	if (pUserCmd->buttons == IN_ATTACK ||
//		pUserCmd->buttons == IN_ATTACK2 ||
//		pUserCmd->buttons == IN_USE)
//	{
//		*m_pApp->m_bSendPackets = true;
//		return;
//	}
//
//	if (m_bFakelagOnlyInAir && ((pLocalEntity->GetFlags() & FL_ONGROUND) || (pLocalEntity->GetFlags() & FL_INWATER)))
//		return;
//
//	if (m_iFakelagChokedAmount >= m_iFakelagChokeAmount)
//	{
//		*m_pApp->m_bSendPackets = true;
//		m_iFakelagChokedAmount = 0;
//	}
//	else
//	{
//		m_iFakelagChokedAmount++;
//		*m_pApp->m_bSendPackets = false;
//	}
//}

void CMisc::AutoStrafe(CUserCmd* pUserCmd)
{
	if (m_iAutoStrafeMode == 0)
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	DWORD moveType = pLocalEntity->GetMoveType();
	if (!(pLocalEntity->GetFlags() & FL_ONGROUND) &&
		!(moveType & MOVETYPE_NOCLIP) &&
		!(moveType & MOVETYPE_LADDER))
	{
		switch (m_iAutoStrafeMode)
		{
		case AUTOSTRAFEMODE_RAGE:
			pUserCmd->forwardmove = (1800.f * 4.f) / pLocalEntity->GetVelocity()->Length();
			if (pUserCmd->mousedx > 1 || pUserCmd->mousedx < -1) {
				pUserCmd->sidemove = pUserCmd->mousedx < 0.f ? -450.f : 450.f;
			}
			else {
				pUserCmd->sidemove = (pUserCmd->command_number % 2) == 0 ? -450.f : 450.f;
			}
			break;
		case AUTOSTRAFEMODE_LEGIT:
			if (pUserCmd->mousedx > 0)
			{
				pUserCmd->sidemove = 450;
			}
			else if (pUserCmd->mousedx < 0)
			{
				pUserCmd->sidemove = -450;
			}
			break;
		case AUTOSTRAFEMODE_NONE:
		default:
			break;
		}
	}
}

//todo: Robin wohin damit ?
float GetTraceFractionWorldProps(Vector start, Vector end)
{
	Ray_t ray;
	trace_t tr;
	CTraceFilterWorldAndPropsOnly filter;

	ray.Init(start, end);
	CApplication::Instance()->EngineTrace()->TraceRay(ray, MASK_SOLID, &filter, &tr);

	return tr.fraction;
}

void CMisc::CircleStrafe(IClientEntity * pLocalEntity, CUserCmd * pUserCmd)
{
	if (!m_bCircleStrafe)
		return;

	//todo: which key? maybe better method?
	if (!GetAsyncKeyState(VK_MENU))
		return;

	static float fStrafeAngle = 0.0f;

	Vector vOrigin = *pLocalEntity->GetOrigin();

	Vector Velocity = *pLocalEntity->GetVelocity();
	Velocity.z = 0;

	float Speed = Velocity.Length();
	if (Speed < 45)
		Speed = 45;

	if (Speed > 750)
		Speed = 750;

	float FinalPath = GetTraceFractionWorldProps(vOrigin + Vector(0, 0, 10), vOrigin + Vector(0, 0, 10) + Velocity / 2.0f);
	float DeltaAngle = m_fCircleStrafeStartDirection * fmax((275.0f / Speed) * (2.0f / FinalPath) * (128.0f / (1.7f / m_pApp->GlobalVars()->interval_per_tick)) * 4.20f, 2.0f);
	fStrafeAngle += DeltaAngle;

	// Autojump
	if (!(pLocalEntity->GetFlags() & FL_ONGROUND))
		pUserCmd->buttons &= ~IN_JUMP;
	else
		pUserCmd->buttons |= IN_JUMP;

	if (fabs(fStrafeAngle) >= 360.0f)
	{
		fStrafeAngle = 0.0f;
	}
	else
	{
		pUserCmd->forwardmove = cos((fStrafeAngle + 90 * m_fCircleStrafeStartDirection) * (PI_F / 180.0f)) * 450.0f;
		pUserCmd->sidemove = sin((fStrafeAngle + 90 * m_fCircleStrafeStartDirection) * (PI_F / 180.0f)) * 450.0f;
	}
}

void CMisc::AutoPistol(CUserCmd* pUserCmd)
{
	if (!m_bAutoPistol)
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	CWeapon* pActiveWeapon = (CWeapon*)pLocalEntity->GetActiveWeapon();
	if (!pActiveWeapon)
		return;

	if (!pActiveWeapon->IsPistol())
		return;

	if (pActiveWeapon->GetWeaponId() == WEAPON_REVOLVER)
		return;

	if (pActiveWeapon->GetClip1() == 0)
		return;

	float fNextattack = pActiveWeapon->GetNextPrimaryAttack();
	float fServertime = pLocalEntity->GetTickBase() * m_pApp->GlobalVars()->interval_per_tick;
	if (fNextattack > fServertime)
	{
		pUserCmd->buttons &= ~IN_ATTACK;
		return;
	}
}

// TODO ‹BERARBEITEN!!!!
void CMisc::SpectatorList()
{
	if (!m_bSpectators)
		return;

	IClientEntity* pLocalEntity = (IClientEntity*)m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	PlayerInfo pLocalInfo, pEntityInfo;
	pLocalEntity->GetPlayerInfo(&pLocalInfo);

	wchar_t pLocalName[256];
	int iLocalNameLen = pLocalInfo.GetName(pLocalName, 256);

	int count = 0;
	Observers m_Observers[64];

	for (int i = 1; i < m_pApp->EngineClient()->GetMaxClients(); i++)
	{
		IClientEntity* pEntity = m_pApp->EntityList()->GetClientEntity(i);

		if (!pEntity)
			continue;

		if (pEntity == pLocalEntity)
			continue;

		if (pEntity->IsAlive())
			continue;

		if (pEntity->IsDormant())
			continue;

		if (m_bOnlyMyTeamSpectators &&	pLocalEntity->GetTeamNum() != pEntity->GetTeamNum() && pEntity->GetTeamNum() != 1)
			continue;

		pEntity->GetPlayerInfo(&pEntityInfo);
		if (pEntityInfo.ishltv)
			continue;

		IClientEntity* pObserverTarget = pEntity->GetObserverTarget(); //todo: crashes

		if (!pObserverTarget)
			continue;

		if (m_bOnlyMySpectators && pObserverTarget != pLocalEntity)
			continue;

		m_Observers[count++] = Observers(pEntity->EntIndex(), pObserverTarget->EntIndex());
	}


	static int width, height;
	m_pApp->EngineClient()->GetScreenSize(width, height);
	m_pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);

	static unsigned long fontHeader = NULL;
	if (fontHeader == NULL)
	{
		fontHeader = m_pApp->Surface()->SCreateFont();
		m_pApp->Surface()->SetFontGlyphSet(fontHeader, "Arial Black", 13, 255, 0, 0, 0x200);
	}
	m_pApp->Surface()->DrawSetTextFont(fontHeader);
	int wHeader, hHeader;
	m_pApp->Surface()->GetTextSize(fontHeader, L"Spectator List:", wHeader, hHeader);
	m_pApp->Surface()->DrawSetTextPos(10, height / 2 - hHeader + 3);
	m_pApp->Surface()->DrawPrintText(L"Spectator List:", wcslen(L"Spectator List"));

	for (int i = 0; i < (sizeof(m_Observers) / sizeof(m_Observers[0])); i++)
	{
		if (m_Observers[i].Observer == NULL)
			break;

		// Drawing list
		static unsigned long fontSpecList = NULL;
		if (fontSpecList == NULL)
		{
			fontSpecList = m_pApp->Surface()->SCreateFont();
			m_pApp->Surface()->SetFontGlyphSet(fontSpecList, "Arial", 12, 255, 0, 0, 0x200);
		}
		m_pApp->Surface()->DrawSetTextFont(fontSpecList);

		IClientEntity* observer = m_pApp->EntityList()->GetClientEntity(m_Observers[i].Observer);
		IClientEntity* observing = m_pApp->EntityList()->GetClientEntity(m_Observers[i].Observing);

		if (observer && observing)
		{
			PlayerInfo infoObserver;
			PlayerInfo infoObserving;

			observer->GetPlayerInfo(&infoObserver);
			observing->GetPlayerInfo(&infoObserving);

			wchar_t observerName[256];
			wchar_t observingName[256];

			int lenObserver = infoObserver.GetName(observerName, 256);
			int lenObserving = infoObserving.GetName(observingName, 256);

			wchar_t text[5] = L" -> ";
			wchar_t end[256];

			m_pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);
			if (!m_bOnlyMySpectators)
			{
				wcscpy(end, observerName);
				wcscat(end, text);
				wcscat(end, observingName);
				// @Nico: Performance? :D (strings dauern immer lange, weil mind. strlen(k¸rzererString) vergleiche
				//		  einfach observing auf LocalEntity checken :) (ist auch besser wegen namechanger oder so)
				//if (wcscmp(observingName, pLocalName) == 0)
				if (observing == pLocalEntity)
				{
					m_pApp->Surface()->DrawSetTextColor(255, 255, 0, 0);
				}
			}
			else
			{
				wcscpy(end, observerName);
			}
			int len = lstrlenW(end);


			int w, h;
			m_pApp->Surface()->GetTextSize(fontSpecList, observerName, w, h);

			m_pApp->Surface()->DrawSetTextPos(10, height / 2 + i * h);
			m_pApp->Surface()->DrawPrintText(end, len);
		}
	}
}

void CMisc::SetClanTag(const char* tag)
{
	if (tag == NULL)
	{
		SetClanTag("");
		return;
	}

	if (tag[0] == '\0')
		m_bIsCustomClanTag = false;
	else
		m_bIsCustomClanTag = true;

	int iLen = strlen(tag) + 1;
	memcpy(m_pClanTag, tag, iLen < 128 ? iLen : 128);

	//m_pSetClanTag(tag, "");
}

/*void CMisc::SetNoNameClanTag(bool bSetNoName)
{
	m_bNoName = bSetNoName;

	char tempBuffer[128];
	int iLen = strlen(m_pClanTag) + 1;
	memcpy(tempBuffer, m_pClanTag, iLen < 128 ? iLen : 128);

	if (bSetNoName)
	{
		if (tempBuffer[0] == '\0')
			memcpy(tempBuffer, ".mechanics", 11);

		strcat(tempBuffer, "\n");
	}

	m_pSetClanTag(tempBuffer, "");
}*/

void CMisc::ApplyClanTag()
{
	if (!m_bIsCustomClanTag)
		return;

	char pBuffer[128];
	strcpy(pBuffer, m_pClanTag);

	if (m_bNoName)
	{
		strcat(pBuffer, "\n");
	}

	m_pSetClanTag(pBuffer, "");
}

void CMisc::JumpScout(CUserCmd* pUserCmd)
{
	if (!m_bJumpScout)
		return;

	IClientEntity* pLocalEntity = (IClientEntity*)m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	CWeapon* pActiveWeapon = pLocalEntity->GetActiveWeapon();
	if (!pActiveWeapon)
		return;

	if (pActiveWeapon->GetWeaponId() == WEAPON_SSG08)
	{
		if (!(pLocalEntity->GetFlags() & FL_ONGROUND))
		{
			bool isAtPeakOfJump = fabs(pLocalEntity->GetVelocity()->z) <= 5.0f;
			if (!isAtPeakOfJump)
			{
				pUserCmd->buttons &= ~IN_ATTACK;
			}
		}
		/*else
		{
			g_pConsole->Write("%f\n", pActiveWeapon->GetAccuracyPenalty());
		}*/
	}
}

void CMisc::SetName(const char* newName)
{
	ConVar* pName = m_pApp->CVar()->FindVar(m_xorName.ToCharArray());
	int callbackNum = pName->callback;
	pName->callback = NULL;
	pName->SetValue(newName);
	pName->callback = callbackNum;
}

void CMisc::SpamNameFix()
{
	if (!m_bSpamName)
		return;

	ConVar* pName = m_pApp->CVar()->FindVar(m_xorName.ToCharArray());
	int callbackNum = pName->callback;
	pName->callback = NULL;
	pName->SetValue("\n\xAD\xAD\xAD");
	pName->callback = callbackNum;
}

void CMisc::AutoAccept(const char* filename)
{
	if (!m_bAutoAccept)
		return;

	if (m_pApp->EngineClient()->IsInGame())
		return;
	static int count = 0;
	static CXorString acceptBeep("6^ÃÌtdË≤rÏ∂~}‡ùvhÊßg⁄†rnıÏ`jÛ");
	if (strcmp(filename, acceptBeep.ToCharArray()) != 0)
		return;
	else
		count++;

	if (count == 3)
	{
		m_IsReadyCallback();
		count = 0;
	}
}