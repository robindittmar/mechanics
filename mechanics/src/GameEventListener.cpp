#include "GameEventListener.h"
#include "Application.h"

CGameEventListener::CGameEventListener() :
	m_xorUserId(/*userid*/"\x62\x78\xE0\xB0\x7E\x6F"),
	m_xorAttacker(/*attacker*/"\x76\x7F\xF1\xA3\x74\x60\xE0\xB0"),
	m_xorDmgHealth(/*dmg_health*/"\x73\x66\xE2\x9D\x7F\x6E\xE4\xAE\x63\x63"),
	m_xorDmgArmor(/*dmg_armor*/"\x73\x66\xE2\x9D\x76\x79\xE8\xAD\x65"),
	m_xorHitgroup(/*hitgroup*/"\x7F\x62\xF1\xA5\x65\x64\xF0\xB2"),
	m_xorHeadshot("n�dc�"), //TODO: xor gibt was falsches aus
	m_xorDominated(/*dominated*/"\x73\x64\xE8\xAB\x79\x6A\xF1\xA7\x73"),
	m_xorRevenge(/*revenge*/"\x65\x6E\xF3\xA7\x79\x6C\xE0"),
	m_xorPenetrated(/*penetrated*/"\x67\x6E\xEB\xA7\x63\x79\xE4\xB6\x72\x6F"),
	m_xorTimelimit("h�rg�~"), //TODO: xor gibt was falsches aus
	m_xorFraglimit(/*fraglimit*/"\x71\x79\xE4\xA5\x7B\x62\xE8\xAB\x63"),
	m_xorObjective(/*objective*/"\x78\x69\xEF\xA7\x74\x7F\xEC\xB4\x72"),
	m_xorWinner(/*winner*/"\x60\x62\xEB\xAC\x72\x79"),
	m_xorReason(/*reason*/"\x65\x6E\xE4\xB1\x78\x65"),
	m_xorMessage(/*message*/"\x7A\x6E\xF6\xB1\x76\x6C\xE0")
{
	m_bNewGame = true;
}

CGameEventListener::~CGameEventListener()
{
}

void CGameEventListener::FireGameEvent(IGameEvent *pEvent)
{
	if (pEvent)
	{
		uint32_t iNameHash = murmurhash(pEvent->GetName(), strlen(pEvent->GetName()), 0xB16B00B5);

		switch (iNameHash)
		{
		case 0xf65971fe: // game_newmap
			this->game_newmap(pEvent);
			break;
		case 0x85d0b7d7: // cs_game_disconnected
			this->cs_game_disconnected(pEvent);
			break;
		case 0x9f08124d: // switch_team
			this->switch_team(pEvent);
			break;
		case 0xf4563cc2: // player_spawned
			this->player_spawned(pEvent);
			break;
		case 0x5b325b2c: // player_hurt
			this->player_hurt(pEvent);
			break;
		case 0x9a7b9b30: // player_death
			this->player_death(pEvent);
			break;
		case 0xa41b1424: // round_start
			this->round_start(pEvent);
			break;
		case 0xf1a6fc33: // round_end
			this->round_end(pEvent);
			break;
		case 0x3b27ece4: // weapon_fire
			this->weapon_fire(pEvent);
			break;
		case 0xe856481f: // bullet_impact
			this->bullet_impact(pEvent);
			break;
		default:
			break;
		}
	}
}

int CGameEventListener::GetEventDebugID(void)
{
	return CEL_PROCEED_EVENT_HANDLING;
}

void CGameEventListener::game_newmap(IGameEvent* pEvent)
{
	CApplication* pApp = CApplication::Instance();

	static CXorString xorRecoilCompensation(/*weapon_recoil_scale*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xB0\x72\x68\xEA\xAB\x7B\x54\xF6\xA1\x76\x67\xE0");

	pApp->Gui()->Setup();
	pApp->SkinChanger()->ApplyDesiredKnife(TEAMNUM_CT, pApp->SkinChanger()->GetDesiredKnifeModelIndexCT());
	pApp->SkinChanger()->ApplyDesiredKnife(TEAMNUM_T, pApp->SkinChanger()->GetDesiredKnifeModelIndexT());
	pApp->GunAccuracy()->SetRecoilCompensation(atof(pApp->CVar()->FindVar(xorRecoilCompensation.ToCharArray())->value));
	pApp->Chams()->DestroyMaterials();
	pApp->Chams()->Setup();
	pApp->Esp()->ResetHeadBones();
	pApp->Visuals()->NoSmoke(pApp->Visuals()->GetNoSmoke());
	pApp->GunHud()->ResetHitmarker();

#ifdef _DEBUG
	INetChannelInfo* pNetChannelInfo = pApp->EngineClient()->GetNetChannelInfo();
	g_pConsole->Write(LOGLEVEL_INFO, "[game_newmap]: %s (name=%s)\n", pNetChannelInfo->GetAddress(), pNetChannelInfo->GetName());
#endif // _DEBUG

	m_bNewGame = true;
}

void CGameEventListener::cs_game_disconnected(IGameEvent* pEvent)
{
	CApplication* pApp = CApplication::Instance();
}

void CGameEventListener::switch_team(IGameEvent* pEvent)
{
	CApplication* pApp = CApplication::Instance();

	//pApp->Misc()->SetNoNameClanTag(pApp->Misc()->GetNoName());
	pApp->Misc()->ApplyClanTag();
	pApp->SkinChanger()->SetForceFullUpdate();
}

void CGameEventListener::player_spawned(IGameEvent* pEvent)
{
	CApplication* pApp = CApplication::Instance();

	int iLocalPlayerIndex = pApp->EngineClient()->GetLocalPlayer();
	int userid = pEvent->GetInt(m_xorUserId.ToCharArray());

	if (iLocalPlayerIndex != pApp->EngineClient()->GetPlayerForUserID(userid))
		return;

	if (m_bNewGame)
	{
		// ClanTag Things
		//pApp->Misc()->SetClanTag(".mechanics"); //todo: dynamic clantag!
		//pApp->Misc()->SetNoNameClanTag(pApp->Misc()->GetNoName());
		pApp->MaterialVisuals()->Nightmode(pApp->MaterialVisuals()->GetNightmodeValue());
		pApp->MaterialVisuals()->Asuswalls(pApp->MaterialVisuals()->GetAsuswallsValue());
		pApp->MaterialVisuals()->SetDefaultSky();
		pApp->MaterialVisuals()->Skychanger(pApp->MaterialVisuals()->GetSkychangerValue());
		//pApp->Visuals()->ResetHitmarker();
		pApp->Misc()->ApplyClanTag();
		// Name Things
		pApp->Misc()->SpamNameFix();

		m_bNewGame = false;
	}
}

void CGameEventListener::player_hurt(IGameEvent* pEvent)
{
	CApplication* pApp = CApplication::Instance();

	/* Possible vars in this event:
	int userid = pEvent->GetInt(m_xorUserId.ToCharArray());
	int attacker = pEvent->GetInt(m_xorAttacker.ToCharArray());
	int health = // No XorString yet
	int armor = // No XorString yet
	const char* weapon = // No XorString yet
	int dmg_health = pEvent->GetInt(m_xorDmgHealth.ToCharArray());
	int dmg_armor = pEvent->GetInt(m_xorDmgArmor.ToCharArray());
	int hitgroup = pEvent->GetBool(m_xorHitgroup.ToCharArray());*/

	int userid = pEvent->GetInt(m_xorUserId.ToCharArray());
	int attacker = pEvent->GetInt(m_xorAttacker.ToCharArray());
	if (pApp->EngineClient()->GetPlayerForUserID(attacker) != pApp->EngineClient()->GetLocalPlayer())
		return;

	pApp->GunHud()->SetPlayerHurtTime(pApp->GlobalVars()->curtime);

	int hitgroup = pEvent->GetInt(m_xorHitgroup.ToCharArray());
	CResolverPlayer* pResolverPlayer = pApp->Resolver()->GetResolverPlayer(pApp->EngineClient()->GetPlayerForUserID(userid));
	if (pResolverPlayer && hitgroup == 1)
	{
		if (pResolverPlayer->m_bBreakingLby)
		{
			pResolverPlayer->SetBreakingShotHit(pResolverPlayer->GetBreakingShotsFired() - 1);
		}
		else
		{
			pResolverPlayer->SetShotHit(pResolverPlayer->GetShotsFired() - 1);
		}
	}

	// TODO IMPORTANT: Probably should try to call the sound engine directly
	if (pApp->GunHud()->GetHitmarker())
	{
		int dmg_health = pEvent->GetInt(m_xorDmgHealth.ToCharArray());

		bool sayTaunt = false; // TODO

		if (dmg_health > 100 && hitgroup == 1)
		{
			if (sayTaunt) // TODO
			{
				// "say +1"
				pApp->EngineClient()->ExecuteClientCmd(CXorString(/*say + 1*/"\x64\x6A\xFC\xE2\x3C\x2B\xB4").ToCharArray());
			}
		}

		// Trigger hitmarker
		pApp->GunHud()->TriggerHitmarker();
	}
}

void CGameEventListener::player_death(IGameEvent* pEvent)
{
	CApplication* pApp = CApplication::Instance();

	/* Possible vars in this event:
	int userid = pEvent->GetInt(m_xorUserId.ToCharArray());
	int attacker = pEvent->GetInt(m_xorAttacker.ToCharArray());
	int assister = // No XorString yet
	bool headshot = pEvent->GetBool(m_xorHeadshot.ToCharArray());
	int dominated = pEvent->GetInt(m_xorDominated.ToCharArray());
	int revenge = pEvent->GetInt(m_xorRevenge.ToCharArray());
	int penetrated = pEvent->GetInt(m_xorPenetrated.ToCharArray());*/

	int iLocalPlayerIndex = pApp->EngineClient()->GetLocalPlayer();
	int userid = pEvent->GetInt(m_xorUserId.ToCharArray());
	int attacker = pEvent->GetInt(m_xorAttacker.ToCharArray());

	CResolverPlayer* pResolverPlayer = pApp->Resolver()->GetResolverPlayer(pApp->EngineClient()->GetPlayerForUserID(userid));
	if (pResolverPlayer)
	{
		pResolverPlayer->SetShotsFired(0);
		pResolverPlayer->SetShotHit(-1);
		pResolverPlayer->SetBreakingShotsFired(0);
		pResolverPlayer->SetBreakingShotHit(-1);
	}

	// Set here instead of player_spawned because of IsAlive check
	if (pApp->EngineClient()->GetPlayerForUserID(userid) == iLocalPlayerIndex)
		pApp->SkinChanger()->SetForceFullUpdate();

	if (pApp->EngineClient()->GetPlayerForUserID(attacker) != iLocalPlayerIndex)
		return;

	bool sayTaunt = false; // TODO
	bool headshot = pEvent->GetBool(m_xorHeadshot.ToCharArray());
	if (headshot && sayTaunt)
	{
		// "say SIEG HEIL"
		pApp->EngineClient()->ClientCmd(CXorString(/*say SIEG HEIL*/"\x64\x6A\xFC\xE2\x44\x42\xC0\x85\x37\x43\xC0\x8B\x5B").ToCharArray());
	}
}

void CGameEventListener::round_start(IGameEvent* pEvent)
{
	CApplication* pApp = CApplication::Instance();

	/* Possible vars in this event:
	uint64 timelimit = pEvent->GetUint64(m_xorTimelimit.ToCharArray());
	uint64 fraglimit = pEvent->GetUint64(m_xorFraglimit.ToCharArray());
	const char* objective = pEvent->GetString(m_xorObjective.ToCharArray());*/

	bool roundSay = false; // TODO
	if (roundSay)
	{
		// "say > powered by .mechanics"
		pApp->EngineClient()->ClientCmd(CXorString(/*say > powered by .mechanics*/"\x64\x6A\xFC\xE2\x29\x2B\xF5\xAD\x60\x6E\xF7\xA7\x73\x2B\xE7\xBB\x37\x25\xE8\xA7\x74\x63\xE4\xAC\x7E\x68\xF6").ToCharArray());
	}
}

void CGameEventListener::round_end(IGameEvent* pEvent)
{
	CApplication* pApp = CApplication::Instance();

	/* Possible vars in this event:
	int winner = pEvent->GetInt(m_xorWinner.ToCharArray());
	int reason = pEvent->GetInt(m_xorReason.ToCharArray());
	const char* message = pEvent->GetString(m_xorMessage.ToCharArray());*/

	// no fking clue what to do here :D
	// (maybe if localplayer->team == winner taunt enemies?)
}

void CGameEventListener::weapon_fire(IGameEvent* pEvent)
{
	CApplication* pApp = CApplication::Instance();

	int userid = pEvent->GetInt(m_xorUserId.ToCharArray());

	IClientEntity* pCurEnt = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetPlayerForUserID(userid));
	if (pCurEnt)
	{
		// Check if local shot
		if (pCurEnt->EntIndex() == pApp->GetLocalPlayer()->EntIndex())
		{
			// Check for valid weapon
			CWeapon* pActive = pCurEnt->GetActiveWeapon();
			if (pActive && !pActive->IsKnife() && !pActive->IsNade() && !pActive->IsC4())
			{
				pApp->GunHud()->SetWeaponFireTime(pApp->GlobalVars()->curtime);

				// Check if target exists
				CTarget* pTarget = pApp->Ragebot()->GetSelectedTarget();
				if (pTarget)
				{
					// Check if target ent exists
					IClientEntity* pEnt = pTarget->GetEntity();
					if (pEnt)
					{
						// Check if ResolverPlayer for target exists
						CResolverPlayer* pCur = pApp->Resolver()->GetResolverPlayer(pEnt->EntIndex());
						if (pCur)
						{
							if (!pCur->m_bLbyPredict)
							{
								// Setting shots fired + 1
								if (pCur->m_bBreakingLby)
								{
									pCur->SetBreakingShotsFired(pCur->GetBreakingShotsFired() + 1);

									if (pCur->GetBreakingShotsFired() > pCur->GetBreakingShotHit() + 2)
									{
										pCur->SetBreakingShotHit(-1);
									}
								}
								else
								{
									pCur->SetShotsFired(pCur->GetShotsFired() + 1);

									if (pCur->GetShotsFired() > pCur->GetShotHit() + 2)
									{
										pCur->SetShotHit(-1);
									}
								}
							}
							else
							{
								pCur->m_bLbyPredict = false;
							}
						}
					}
				}
			}
		}
	}
}

void CGameEventListener::bullet_impact(IGameEvent * pEvent)
{
	CApplication* pApp = CApplication::Instance();
	int userid = pEvent->GetInt(m_xorUserId.ToCharArray());

	IClientEntity* pCurEnt = pApp->EntityList()->GetClientEntity(pApp->EngineClient()->GetPlayerForUserID(userid));

	if (pCurEnt)
	{
		// Check for valid weapon
		CWeapon* pActiveWeapon = pCurEnt->GetActiveWeapon();
		if (pActiveWeapon && !pActiveWeapon->IsKnife() && !pActiveWeapon->IsNade() && !pActiveWeapon->IsC4())
		{
			if (pApp->GunHud()->GetHitmarkerHitpoint() && pApp->GetLocalPlayer()->EntIndex() == pCurEnt->EntIndex())
			{
				HitmarkerEntry hitmarkerentry;
				hitmarkerentry.vHit = Vector(pEvent->GetFloat("x"), pEvent->GetFloat("y"), pEvent->GetFloat("z"));
				hitmarkerentry.fStarttime = pApp->GlobalVars()->curtime;
				pApp->m_pHitmarker.push_back(hitmarkerentry);
			}

			if (pApp->Visuals()->GetBulletTracer())
			{
				int iLocalTeam = pApp->GetLocalPlayer()->GetTeamNum();

				if ((iLocalTeam != pCurEnt->GetTeamNum() || pApp->Visuals()->GetBulletTracerTeam()) ||
					(pApp->Visuals()->GetBulletTracerSelf() && pApp->GetLocalPlayer()->EntIndex() == pCurEnt->EntIndex()))
				{
					BulletTracerEntry btEntry;
					btEntry.qAngles = *pCurEnt->GetAngEyeAngles();
					btEntry.vStart = (*pCurEnt->GetOrigin() + *pCurEnt->GetEyeOffset());
					btEntry.vEnd = Vector(pEvent->GetFloat("x"), pEvent->GetFloat("y"), pEvent->GetFloat("z"));
					pApp->m_pBulletTracer.push_back(btEntry);
				}
			}
		}
	}
}
