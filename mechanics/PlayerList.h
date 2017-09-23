#ifndef __PLAYERLIST_H__
#define __PLAYERLIST_H__

// Std lib
#include <vector>
#include <unordered_map>

// Source SDK
#include "IVEngineClient.h"
#include "ClientEntityList.h"

// Custom
#include "Player.h"

class CApplication;

class CPlayerList
{
public:
	CPlayerList();
	~CPlayerList();
private:
	CApplication* m_pApp;
	IVEngineClient* m_pEngineClient;
	IClientEntityList* m_pEntityList;
};

#endif // __PLAYERLIST_H__