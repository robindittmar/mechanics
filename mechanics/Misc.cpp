#include "Misc.h"
#include "Application.h"

CMisc::CMisc()
{
}

CMisc::~CMisc()
{
}

void CMisc::Setup()
{
	m_pApp = CApplication::Instance();
	m_pSetClanTag = (SetClanTag_t)CPattern::FindPattern((BYTE*)m_pApp->EngineDll(), 0x8C7000, (BYTE*)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", "adhgezvel");

	m_xorName.String("yjËß");
	m_xorName.Xor();

	m_dwOverridePostProcessingDisable = *(DWORD**)(CPattern::FindPattern((BYTE*)m_pApp->ClientDll(), 0x50E5000, (BYTE*)"\x80\x3D\x00\x00\x00\x00\x00\x53\x56\x57\x0F\x85", "ag-----zrhli") + 0x2);
}

void CMisc::Update(void* pParameters)
{
}

void CMisc::NoRecoil(CUserCmd* pUserCmd)
{
	if (!m_bIsEnabled ||
		!m_bNoRecoil ||
		m_pApp->Ragebot()->DidNoRecoil() ||
		!(pUserCmd->buttons & IN_ATTACK))
	{
		m_pApp->m_oldAimPunchAngle.x = 0;
		m_pApp->m_oldAimPunchAngle.y = 0;
		return;
	}

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	CWeapon* activeWeapon = (CWeapon*)pLocalEntity->GetActiveWeapon();
	if (activeWeapon->IsNade() ||
		activeWeapon->IsPistol() && !m_pApp->Visuals()->GetNoVisualRecoil()) //todo: maybe norecoil while pistol
		return;

	if (m_pApp->Ragebot()->DidNoRecoil())
		return;

	int shotsFired = pLocalEntity->GetShotsFired();
	if (m_pApp->Visuals()->GetNoVisualRecoil())
	{
		QAngle aimPunch = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));
		pUserCmd->viewangles[0] -= aimPunch.x * m_pApp->GetRecoilCompensation();
		pUserCmd->viewangles[1] -= aimPunch.y * m_pApp->GetRecoilCompensation();
	}
	else {
		m_pApp->EngineClient()->GetViewAngles(m_pApp->m_viewAngle);
		QAngle aimPunchAngle = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));

		m_pApp->m_viewAngle.x += (m_pApp->m_oldAimPunchAngle.x - aimPunchAngle.x * m_pApp->GetRecoilCompensation());
		m_pApp->m_viewAngle.y += (m_pApp->m_oldAimPunchAngle.y - aimPunchAngle.y * m_pApp->GetRecoilCompensation());

		m_pApp->SetClientViewAngles(m_pApp->m_viewAngle);

		m_pApp->m_oldAimPunchAngle.x = aimPunchAngle.x * m_pApp->GetRecoilCompensation();
		m_pApp->m_oldAimPunchAngle.y = aimPunchAngle.y * m_pApp->GetRecoilCompensation();
	}
}

//todo: check if works properly and not to many packets choked
void CMisc::Fakelag(CUserCmd* pUserCmd)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bFakelag)
		return;

	if (!m_pApp->m_bGotSendPackets)
		return;

	if (pUserCmd->buttons == IN_ATTACK ||
		pUserCmd->buttons == IN_ATTACK2 ||
		pUserCmd->buttons == IN_JUMP)
	{
		*m_pApp->m_bSendPackets = true;
		return;
	}

	static int chokedPackets = 0;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	if (chokedPackets >= MAXPACKETSCHOKED) //todo: MAXPACKETCHOKED -/+ FakelagValue!
	{
		*m_pApp->m_bSendPackets = true;
		chokedPackets = 0;
	}
	else
	{
		chokedPackets++;
		*m_pApp->m_bSendPackets = false;
	}
}

void CMisc::AutoStrafe(CUserCmd* pUserCmd)
{
	if (!m_bIsEnabled)
		return;

	if (!m_bAutoStrafe)
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	DWORD moveType = pLocalEntity->GetMoveType();
	if (!(pLocalEntity->GetFlags() & FL_ONGROUND) &&
		!(moveType & MOVETYPE_NOCLIP) &&
		!(moveType & MOVETYPE_LADDER))
	{
		if (pUserCmd->mousedx > 0)
		{
			pUserCmd->sidemove = 450;
		}
		else if (pUserCmd->mousedx < 0)
		{
			pUserCmd->sidemove = -450;
		}
	}
}

void CMisc::DrawNoScope()
{
	if (!m_bIsEnabled)
		return;

	if (!m_bNoScope)
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	if (!pLocalEntity->IsScoped())
		return;

	int width, height;
	m_pApp->EngineClient()->GetScreenSize(width, height);
	m_pApp->Surface()->DrawSetColor(255, 0, 0, 0);
	m_pApp->Surface()->DrawLine(width / 2, 0, width / 2, height);
	m_pApp->Surface()->DrawLine(0, height / 2, width, height / 2);
}

bool CMisc::NoScope(unsigned int vguiPanel)
{
	if (!m_bIsEnabled)
		return false;

	if (!m_bNoScope)
		return false;

	static CXorString hudZoom("_~·òxdË");
	if (!strcmp(hudZoom.ToCharArray(), m_pApp->Panel()->GetName(vguiPanel)))
		return true;
	return false;
}

void CMisc::AutoPistol(CUserCmd* pUserCmd)
{
	if (!m_bIsEnabled)
		return;

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

	float fNextattack = pActiveWeapon->GetNextPrimaryAttack();
	float fServertime = pLocalEntity->GetTickBase() * m_pApp->GlobalVars()->interval_per_tick;
	if (fNextattack > fServertime)
	{
		pUserCmd->buttons &= ~IN_ATTACK;
		return;
	}
}

void CMisc::SpectatorList()
{
	if (!m_bIsEnabled)
		return;

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

	m_pSetClanTag(tag, "");
}

void CMisc::SetNoNameClanTag(bool bSetNoName)
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
}

void CMisc::AutoRevolver(CUserCmd* pUserCmd)
{
	if (!m_bIsEnabled)
		return;

	if (!m_pApp->Ragebot()->GetEnabled())
		return;

	if (m_pApp->Ragebot()->IsShooting())
		return;

	IClientEntity* pLocalEntity = (IClientEntity*)m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	CWeapon* activeWeapon = (CWeapon*)pLocalEntity->GetActiveWeapon();
	if (!activeWeapon)
		return;

	if (activeWeapon->GetWeaponId() != WEAPON_REVOLVER)
		return;

	if (activeWeapon->GetClip1() == 0)
		return;

	pUserCmd->buttons |= IN_ATTACK;
	float flPostponeFireReady = activeWeapon->GetPostPoneFireReady();
	if (flPostponeFireReady > 0 && flPostponeFireReady - .1f < m_pApp->GlobalVars()->curtime)
	{
		pUserCmd->buttons &= ~IN_ATTACK;
	}
}

void CMisc::DisablePostProcessing()
{
	bool* bOverridePostProcessingDisable = (bool*)(m_dwOverridePostProcessingDisable);
	IClientEntity* pLocalEntity = (IClientEntity*)m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	if (m_bDisablePostProcessing && pLocalEntity->IsScoped())
	{
		*bOverridePostProcessingDisable = true;
	}
	else
	{
		*bOverridePostProcessingDisable = false;
	}
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