#ifndef __RESOLVER_H__
#define __RESOLVER_H__

#include "IFeature.h"

class CApplication;

#define RESOLVERTYPE_NONE						0
#define RESOLVERTYPE_LBY						1
#define RESOLVERTYPE_BRUTEFORCE					2
#define RESOLVERTYPE_NOSPREAD					3
#define RESOLVERTYPE_AUTOMATIC					4

class CResolver : public IFeature
{
public:
	CResolver();
	~CResolver();

	void SetResolverType(int iResolverType) { m_iResolverType = iResolverType; }
	int GetResolverType() { return m_iResolverType; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	int m_iResolverType;
};

#endif // __RESOLVER_H__