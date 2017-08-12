#include "GameEventListener.h"
#include "Application.h"

CGameEventListener::CGameEventListener() :
	m_xorUserId("bx�~o"),
	m_xorAttacker("v�t`�"),
	m_xorDmgHealth("sf�n�cc"),
	m_xorDmgArmor("sf�vy�e"),
	m_xorHitgroup("b�ed�"),
	m_xorHeadshot("n�dc�"),
	m_xorDominated("sd�yj�s"),
	m_xorRevenge("en�yl�"),
	m_xorPenetrated("gn�cy�ro"),
	m_xorTimelimit("h�rg�~"),
	m_xorFraglimit("qy�{b�c"),
	m_xorObjective("xi�t�r"),
	m_xorWinner("`b�ry"),
	m_xorReason("en�xe"),
	m_xorMessage("zn��vl�")
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

		switch(iNameHash)
		{
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

	// Hitmarker (TODO: It probably should go into the Visuals class,
	//			        but the hitmarker feature is depending on a few more hooks)
	// TODO IMPORTANT: Probably should try to call the sound engine directly
	if (pApp->Visuals()->Hitmarker())
	{
		int dmg_health = pEvent->GetInt(m_xorDmgHealth.ToCharArray());
		int hitgroup = pEvent->GetInt(m_xorHitgroup.ToCharArray());

		if (dmg_health > 100 && hitgroup == 1)
		{
			// say +1
			//pApp->EngineClient()->ExecuteClientCmd(CXorString("dj��<:").ToCharArray());
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

	bool sayTaunt = false;
	bool headshot = pEvent->GetBool(m_xorHeadshot.ToCharArray());
	if (headshot && sayTaunt)
	{
		pApp->EngineClient()->ClientCmd(CXorString("dj��DB��7C��[").ToCharArray());
	}
}

void CGameEventListener::round_start(IGameEvent* pEvent)
{
	CApplication* pApp = CApplication::Instance();

	/* Possible vars in this event:
	uint64 timelimit = pEvent->GetUint64(m_xorTimelimit.ToCharArray());
	uint64 fraglimit = pEvent->GetUint64(m_xorFraglimit.ToCharArray());
	const char* objective = pEvent->GetString(m_xorObjective.ToCharArray());*/

	bool roundSay = false;
	if (roundSay)
	{
		pApp->EngineClient()->ClientCmd(CXorString("dj��)+��`n��s+�7s��e}�").ToCharArray());
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