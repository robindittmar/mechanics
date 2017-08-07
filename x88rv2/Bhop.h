#ifndef __BHOP_H__
#define __BHOP_H__

#include "Feature.h"
#include "UserCmd.h"

#define FORCEJUMP_OFFSET 0x4F2379C
#define JUMP_FLAG_OFFSET 0x100

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