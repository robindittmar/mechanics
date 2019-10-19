#ifndef __LAGCOMPENSATIONPLAYERLIST_H__
#define __LAGCOMPENSATIONPLAYERLIST_H__

#include "LagCompensationPlayerEntry.h"
#include "../source_sdk/ClientEntity.h"

#define LC_MAXSAVEDTICKS	26

class CLagCompensationPlayerList
{
public:
	int m_iEntryCount = 0;
	CLagCompensationPlayerEntry m_pPlayerEntries[LC_MAXSAVEDTICKS];

	void RemoveInvalidPlayerEntries();
	void AddPlayerEntry(IClientEntity* pCurEnt, int tickcount);
	void RestorePlayerEntry(IClientEntity* pCurEnt, int iEntryIndex);
};

#endif // __LAGCOMPENSATIONPLAYERLIST_H__