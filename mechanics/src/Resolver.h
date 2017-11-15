#ifndef __RESOLVER_H__
#define __RESOLVER_H__

#include "ResolverPlayer.h"
#include "ClientEntity.h"
#include "IFeature.h"

#define MAX_PLAYERS 64

class CApplication;

#define RESOLVERTYPE_NONE						0
#define RESOLVERTYPE_LBY						1
#define RESOLVERTYPE_AUTOMATIC					2

class CResolver : public IFeature
{
public:
	CResolver();
	~CResolver();

	void SetResolverType(int iResolverType) { m_iResolverType = iResolverType; }
	int GetResolverType() { return m_iResolverType; }

	CResolverPlayer* GetResolverPlayer(int index) { return &m_pResolverPlayers[index]; }

	virtual void Update(void* pParameters = 0) override;
private:
	int m_iResolverType;

	void CheckPlayer(CResolverPlayer* pCurResolverPlayer, IClientEntity* pCurEntity);
	void BruteforcePlayer(CResolverPlayer* pCurResolverPlayer, IClientEntity* pCurEntity);
	void BruteforceBreakingPlayer(CResolverPlayer* pCurResolverPlayer, IClientEntity* pCurEntity);

	CResolverPlayer m_pResolverPlayers[MAX_PLAYERS];
};

#endif // __RESOLVER_H__