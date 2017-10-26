#ifndef __GAMEEVENTLISTENER_H__
#define __GAMEEVENTLISTENER_H__

#include "murmurhash.h"
#include "XorString.h"
#include "IGameEventManager.h"

class CGameEventListener : public IGameEventListener2
{
public:
	CGameEventListener();
	virtual ~CGameEventListener();

	virtual void FireGameEvent(IGameEvent* pEvent);
	virtual int GetEventDebugID(void);
private:
	bool m_bNewGame;
	// I hate to break conventions for function names,
	// but naming/labeling these after the events just seems a little nicer
	// (as they shouldn't be called by anyone else anyway)
	//
	// Also pEvent will never be NULL when they get called
	void game_newmap(IGameEvent* pEvent);
	void cs_game_disconnected(IGameEvent* pEvent);
	void switch_team(IGameEvent* pEvent);
	void player_spawned(IGameEvent* pEvent);
	void player_hurt(IGameEvent* pEvent);
	void player_death(IGameEvent* pEvent);
	void round_start(IGameEvent* pEvent);
	void round_end(IGameEvent* pEvent);
	void weapon_fire(IGameEvent* pEvent);
	void bullet_impact(IGameEvent* pEvent);

	// player_hurt & player_death
	CXorString m_xorUserId;
	CXorString m_xorAttacker;

	// player_hurt
	CXorString m_xorDmgHealth;
	CXorString m_xorDmgArmor;
	CXorString m_xorHitgroup;

	// player_daeth
	CXorString m_xorHeadshot;
	CXorString m_xorDominated;
	CXorString m_xorRevenge;
	CXorString m_xorPenetrated;

	// round_start
	CXorString m_xorTimelimit;
	CXorString m_xorFraglimit;
	CXorString m_xorObjective;

	// round_end
	CXorString m_xorWinner;
	CXorString m_xorReason;
	CXorString m_xorMessage;
};

#endif // __GAMEEVENTLISTENER_H__