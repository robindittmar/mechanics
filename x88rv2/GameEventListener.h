#ifndef __GAMEEVENTLISTENER_H__
#define __GAMEEVENTLISTENER_H__

#include "IGameEventManager.h"

// player_hurt
class CPlayerHurtEventListener : public IGameEventListener2
{
public:
	CPlayerHurtEventListener();
	virtual ~CPlayerHurtEventListener();

	virtual void FireGameEvent(IGameEvent *pEvent);
	virtual int GetEventDebugID(void);
private:
};

// player_death
class CPlayerDeathEventListener : public IGameEventListener2
{
public:
	CPlayerDeathEventListener();
	virtual ~CPlayerDeathEventListener();

	virtual void FireGameEvent(IGameEvent *pEvent);
	virtual int GetEventDebugID(void);
};

// round_start
class CRoundStartEventListener : public IGameEventListener2
{
public:
	CRoundStartEventListener();
	virtual ~CRoundStartEventListener();

	virtual void FireGameEvent(IGameEvent *pEvent);
	virtual int GetEventDebugID(void);
};

// round_end
class CRoundEndEventListener : public IGameEventListener2
{
public:
	CRoundEndEventListener();
	virtual ~CRoundEndEventListener();

	virtual void FireGameEvent(IGameEvent *pEvent);
	virtual int GetEventDebugID(void);
};

#endif // __GAMEEVENTLISTENER_H__