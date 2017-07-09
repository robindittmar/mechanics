#ifndef __BHOP_H__
#define __BHOP_H__

#include "Feature.h"

class CBhop : public IFeature
{
public:
	CBhop();
	~CBhop();

	virtual void Setup();
	virtual void Update();
private:
};

#endif // __BHOP_H__