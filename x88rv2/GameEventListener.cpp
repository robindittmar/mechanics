#include "GameEventListener.h"
#include "Application.h"

CXorString g_pUserId("bx�~o");
CXorString g_pAttackerId("v�t`�");

//
// Event player_hurt
//
CPlayerHurtEventListener::CPlayerHurtEventListener()
{
}

CPlayerHurtEventListener::~CPlayerHurtEventListener()
{
}

void CPlayerHurtEventListener::FireGameEvent(IGameEvent *pEvent)
{
	static CXorString strDmgHealth("sf�n�cc");
	static CXorString strDmgArmor("sf�vy�e");
	static CXorString strHitgroup("b�ed�");

	if (pEvent)
	{
		CApplication* pApp = CApplication::Instance();

		int userid = pEvent->GetInt(g_pUserId.ToCharArray());
		int attacker = pEvent->GetInt(g_pAttackerId.ToCharArray());
		int dmg_health = pEvent->GetInt(strDmgHealth.ToCharArray());
		int dmg_armor = pEvent->GetInt(strDmgArmor.ToCharArray());
		int hitgroup = pEvent->GetBool(strHitgroup.ToCharArray());

		if (pApp->EngineClient()->GetPlayerForUserID(attacker) != pApp->EngineClient()->GetLocalPlayer())
			return;

		// Hitmarker (TODO: It probably should go into the Visuals class,
		//			        but the hitmarker feature is depending on a few more hooks)
		// TODO IMPORTANT: Probably should try to call the sound engine directly
		if(pApp->Visuals()->Hitmarker())
		{
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
}

int CPlayerHurtEventListener::GetEventDebugID(void)
{
	return CEL_PROCEED_EVENT_HANDLING;
}

//
// Event player_death
//
CPlayerDeathEventListener::CPlayerDeathEventListener()
{
}

CPlayerDeathEventListener::~CPlayerDeathEventListener()
{
}

void CPlayerDeathEventListener::FireGameEvent(IGameEvent *pEvent)
{
	static CXorString strHeadshot("n�dc�");
	static CXorString strDominated("sd�yj�s");
	static CXorString strRevenge("en�yl�");
	static CXorString strPenetrated("gn�cy�ro");

	if (pEvent)
	{
		CApplication* pApp = CApplication::Instance();

		int userid = pEvent->GetInt(g_pUserId.ToCharArray());
		int attacker = pEvent->GetInt(g_pAttackerId.ToCharArray());
		bool headshot = pEvent->GetBool(strHeadshot.ToCharArray());
		int dominated = pEvent->GetInt(strDominated.ToCharArray());
		int revenge = pEvent->GetInt(strRevenge.ToCharArray());
		int penetrated = pEvent->GetInt(strPenetrated.ToCharArray());

		if (pApp->EngineClient()->GetPlayerForUserID(attacker) != pApp->EngineClient()->GetLocalPlayer())
			return;

		bool sayTaunt = false;
		if (headshot && sayTaunt)
		{
			pApp->EngineClient()->ClientCmd(CXorString("dj��DB��7C��[").ToCharArray());
		}
	}
}

int CPlayerDeathEventListener::GetEventDebugID(void)
{
	return CEL_PROCEED_EVENT_HANDLING;
}

//
// Event round_start
//
CRoundStartEventListener::CRoundStartEventListener()
{
}

CRoundStartEventListener::~CRoundStartEventListener()
{
}

void CRoundStartEventListener::FireGameEvent(IGameEvent *pEvent)
{
	static CXorString strTimelimit("h�rg�~");
	static CXorString strFraglimit("qy�{b�c");
	static CXorString strObjective("xi�t�r");

	if (pEvent)
	{
		CApplication* pApp = CApplication::Instance();

		uint64 timelimit = pEvent->GetUint64(strTimelimit.ToCharArray());
		uint64 fraglimit = pEvent->GetUint64(strFraglimit.ToCharArray());
		const char* objective = pEvent->GetString(strObjective.ToCharArray());

		bool roundSay = false;
		if (roundSay)
		{
			pApp->EngineClient()->ClientCmd(CXorString("dj��)+��`n��s+�7s��e}�").ToCharArray());
		}
	}
}

int CRoundStartEventListener::GetEventDebugID(void)
{
	return CEL_PROCEED_EVENT_HANDLING;
}

//
// Event round_end
//
CRoundEndEventListener::CRoundEndEventListener()
{
}

CRoundEndEventListener::~CRoundEndEventListener()
{
}

void CRoundEndEventListener::FireGameEvent(IGameEvent *pEvent)
{
	static CXorString strWinner("`b�ry");
	static CXorString strReason("en�xe");
	static CXorString strMessage("zn��vl�");

	if (pEvent)
	{
		CApplication* pApp = CApplication::Instance();

		int winner = pEvent->GetInt(strWinner.ToCharArray());
		int reason = pEvent->GetInt(strReason.ToCharArray());
		const char* message = pEvent->GetString(strMessage.ToCharArray());

		// not sure
	}
}

int CRoundEndEventListener::GetEventDebugID(void)
{
	return CEL_PROCEED_EVENT_HANDLING;
}