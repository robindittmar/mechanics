#include "GameEventListener.h"
#include "Application.h"

CGameEventListener::CGameEventListener() :
	m_xorUserId("bxà°~o"),
	m_xorAttacker("vñ£t`à°"),
	m_xorDmgHealth("sfânä®cc"),
	m_xorDmgArmor("sfâvyè­e"),
	m_xorHitgroup("bñ¥edğ²"),
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

	/*if (!pApp->GetHooked() && pApp->GetInitialHookDone())
	{
		g_pConsole->Write("Rehooking...\n");
		pApp->Rehook();
	}*/

	pApp->SetRecoilCompensation(atof(pApp->CVar()->FindVar(/*weapon_recoil_scale*/CXorString("`nä²xeÚ°rhê«{Tö¡vgà").ToCharArray())->value));
	pApp->Gui()->Setup();
	pApp->Misc()->SpamNameFix();
	pApp->Chams()->ReloadMaterials();
}

void CGameEventListener::cs_game_disconnected(IGameEvent* pEvent)
{
	CApplication* pApp = CApplication::Instance();
	
	/*if(pApp->GetHooked())
	{
		g_pConsole->Write("Unhooking...\n");
		pApp->Unhook();
	}*/
}

void CGameEventListener::switch_team(IGameEvent* pEvent)
{

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

	int attacker = pEvent->GetInt(m_xorAttacker.ToCharArray());
	if (pApp->EngineClient()->GetPlayerForUserID(attacker) != pApp->EngineClient()->GetLocalPlayer())
		return;

	// TODO IMPORTANT: Probably should try to call the sound engine directly
	if (pApp->Visuals()->GetHitmarker())
	{
		int dmg_health = pEvent->GetInt(m_xorDmgHealth.ToCharArray());
		int hitgroup = pEvent->GetInt(m_xorHitgroup.ToCharArray());
		bool sayTaunt = false; // TODO

		if (dmg_health > 100 && hitgroup == 1 && sayTaunt)
		{
			// "say +1"
			pApp->EngineClient()->ExecuteClientCmd(CXorString("djüâ<:").ToCharArray());
			pApp->EngineClient()->ExecuteClientCmd("play buttons/blip2.wav"); // TODO: Xor
		}
		else
		{
			pApp->EngineClient()->ExecuteClientCmd("play buttons/blip1.wav"); // TODO: Xor
		}

		// Draw hitmarker
		pApp->Visuals()->TriggerHitmarker();
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

	int attacker = pEvent->GetInt(m_xorAttacker.ToCharArray());
	if (pApp->EngineClient()->GetPlayerForUserID(attacker) != pApp->EngineClient()->GetLocalPlayer())
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
