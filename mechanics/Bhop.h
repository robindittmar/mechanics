#ifndef __BHOP_H__
#define __BHOP_H__

#include "IFeature.h"
#include "UserCmd.h"

class CApplication;

class CBhop : public IFeature
{
public:
	CBhop();
	~CBhop();

	virtual void Update(void* pParameters = 0) override;
private:
};

#endif // __BHOP_H__