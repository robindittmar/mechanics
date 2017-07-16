#ifndef __BHOP_H__
#define __BHOP_H__

#include "Feature.h"

#define FORCEJUMP_OFFSET 0x4F236BC
#define JUMP_FLAG_OFFSET 0x100
#define VELOCITY_OFFSET 0x110

class CApplication;

class CBhop : public IFeature
{
public:
	CBhop();
	~CBhop();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	CApplication* m_pApp;
};

#endif // __BHOP_H__