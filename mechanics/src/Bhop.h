#ifndef __BHOP_H__
#define __BHOP_H__

// Std Lib

// Source SDK
#include "UserCmd.h"

// Custom
#include "IFeature.h"


class CBhop : public IFeature
{
public:
	CBhop();
	~CBhop();

	virtual void Update(void* pParameters = 0) override;
private:
};

#endif // __BHOP_H__