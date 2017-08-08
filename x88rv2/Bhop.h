#ifndef __BHOP_H__
#define __BHOP_H__

#include "Feature.h"
#include "UserCmd.h"

class CApplication;

class CBhop : public IFeature
{
public:
	CBhop();
	~CBhop();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
};

#endif // __BHOP_H__