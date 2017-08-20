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
}

void CMisc::Update(void* pParameters)
{
}

void CMisc::NoRecoil(CUserCmd* pUserCmd)
{
	if (!m_bIsEnabled ||
		!m_bNoRecoil ||
		m_pApp->Aimbot()->DidNoRecoil() ||
		!(pUserCmd->buttons & IN_ATTACK))
	{
		m_pApp->m_oldAimPunchAngle.x = 0;
		m_pApp->m_oldAimPunchAngle.y = 0;
		return;
	}

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	CWeapon* activeWeapon = (CWeapon*)pLocalEntity->ActiveWeapon();
	if (activeWeapon->IsNade() ||
		activeWeapon->IsPistol() && !m_pApp->Visuals()->GetNoVisualRecoil()) //todo: maybe norecoil while pistol
		return;

	if (m_pApp->Aimbot()->DidNoRecoil())
		return;

	int shotsFired = pLocalEntity->ShotsFired();
	if (m_pApp->Visuals()->GetNoVisualRecoil())
	{
		QAngle aimPunch = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));
		pUserCmd->viewangles[0] -= aimPunch.x * RECOIL_COMPENSATION;
		pUserCmd->viewangles[1] -= aimPunch.y * RECOIL_COMPENSATION;
	}
	else {
		m_pApp->EngineClient()->GetViewAngles(m_pApp->m_viewAngle);
		QAngle aimPunchAngle = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));

		m_pApp->m_viewAngle.x += (m_pApp->m_oldAimPunchAngle.x - aimPunchAngle.x * RECOIL_COMPENSATION);
		m_pApp->m_viewAngle.y += (m_pApp->m_oldAimPunchAngle.y - aimPunchAngle.y * RECOIL_COMPENSATION);

		m_pApp->ClientViewAngles(m_pApp->m_viewAngle);

		m_pApp->m_oldAimPunchAngle.x = aimPunchAngle.x * RECOIL_COMPENSATION;
		m_pApp->m_oldAimPunchAngle.y = aimPunchAngle.y * RECOIL_COMPENSATION;
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
	DWORD moveType = pLocalEntity->MoveType();
	if (!(pLocalEntity->Flags() & FL_ONGROUND) &&
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

	static CXorString hudZoom("_~á˜xdè");
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
	CWeapon* activeWeapon = (CWeapon*)pLocalEntity->ActiveWeapon();

	if (!activeWeapon->IsPistol())
		return;

	if (activeWeapon->WeaponId() == WEAPON_REVOLVER)
		return;

	float nextattack = activeWeapon->NextPrimaryAttack();
	float servertime = pLocalEntity->TickBase() * m_pApp->GlobalVars()->interval_per_tick;
	if (nextattack > servertime)
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
	PlayerInfo pLocalInfo = pLocalEntity->GetPlayerInfo();
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

		if (m_bOnlyMyTeamSpectators &&	pLocalEntity->TeamNum() != pEntity->TeamNum() && pEntity->TeamNum() != 1)
			continue;

		PlayerInfo pEntityInfo = pEntity->GetPlayerInfo();
		if (pEntityInfo.ishltv)
			continue;

		IClientEntity* pObserverTarget = pEntity->ObserverTarget(); //todo: crashes

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
	m_pApp->Surface()->DrawPrintText(L"Spectator List:", lstrlenW(L"Spectator List"));

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
			PlayerInfo infoObserver = observer->GetPlayerInfo();
			PlayerInfo infoObserving = observing->GetPlayerInfo();

			wchar_t observerName[256];
			int lenObserver = infoObserver.GetName(observerName, 256);
			wchar_t observingName[256];
			int lenObserving = infoObserving.GetName(observingName, 256);

			wchar_t text[5] = L" -> ";
			wchar_t end[256];

			m_pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);
			if (!m_bOnlyMySpectators)
			{
				wcscpy(end, observerName);
				wcscat(end, text);
				wcscat(end, observingName);
				if (wcscmp(observingName, pLocalName) == 0)
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
	m_pSetClanTag(tag, "");
}

void CMisc::AutoRevolver(CUserCmd* pUserCmd)
{
	if (!m_bIsEnabled)
		return;

	if (!m_pApp->Aimbot()->IsEnabled())
		return;

	IClientEntity* pLocalEntity = (IClientEntity*)m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	CWeapon* activeWeapon = (CWeapon*)pLocalEntity->ActiveWeapon();
	if (activeWeapon->WeaponId() != WEAPON_REVOLVER)
		return;

	if (activeWeapon->Clip1() == 0)
		return;

	pUserCmd->buttons |= IN_ATTACK;
	float flPostponeFireReady = activeWeapon->PostPoneFireReady();
	if (flPostponeFireReady > 0 && flPostponeFireReady - .1f < m_pApp->GlobalVars()->curtime)
	{
		pUserCmd->buttons &= ~IN_ATTACK;
	}
}