#include "GameEventListener.h"
#include "Application.h"

CGameEventListener::CGameEventListener() :
	m_xorUserId("bxà°~o"),
	m_xorAttacker("vñ£t`à°"),
	m_xorDmgHealth("sfânä®cc"),
	m_xorDmgArmor("sfâvyè­e"),
	m_xorHitgroup("bñ¥edð²"),
	m_xorHeadshot("nä¦dcê¶"),
	m_xorDominated("sdè«yjñ§s"),
	m_xorRevenge("enó§ylà"),
	m_xorPenetrated("gnë§cyä¶ro"),
	m_xorTimelimit("hì¯rgì¯~"),
	m_xorFraglimit("qyä¥{bè«c"),
	m_xorObjective("xiï§tì´r"),
	m_xorWinner("`bë¬ry"),
	m_xorReason("enä±xe"),
	m_xorMessage("znö±vlà")
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

	static CXorString xorRecoilCompensation("`nä²xeÚ°rhê«{Tö¡vgà");

	pApp->Gui()->Setup();
	pApp->SkinChanger()->SetNewMap();
	pApp->SkinChanger()->ApplyDesiredKnife(pApp->SkinChanger()->GetDesiredKnifeModelIndex());
	pApp->SkinChanger()->SetForceFullUpdate();
	pApp->SetRecoilCompensation(atof(pApp->CVar()->FindVar(xorRecoilCompensation.ToCharArray())->value));
	pApp->Chams()->ReloadMaterials();
	pApp->Esp()->ResetHeadBones();
	pApp->Visuals()->NoSmoke(pApp->Visuals()->GetNoSmoke());
	pApp->GunHud()->ResetHitmarker();

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
				pApp->EngineClient()->ExecuteClientCmd(CXorString("djüâ<:").ToCharArray());
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
		pApp->EngineClient()->ClientCmd(CXorString("djüâDBÀ…7CÀ‹[").ToCharArray());
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
		pApp->EngineClient()->ClientCmd(CXorString("djüâ)+õ­`n÷§s+ç»7%è§tcä¬~hö").ToCharArray());
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
				CTarget* pTarget = pApp->TargetSelector()->GetTarget(pApp->Ragebot()->GetTargetCriteria());
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
