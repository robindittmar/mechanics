#ifndef __BHOP_H__
#define __BHOP_H__

#include "Feature.h"

#define JUMP_ADDRESS_OFFSET 0x4F236BC
#define JUMP_FLAG_OFFSET 0x100

class CApplication;

class CBhop : public IFeature
{
public:
	CBhop();
	~CBhop();

	virtual void Setup();
	virtual void Update();
private:
	CApplication* m_pApp;
};

#endif // __BHOP_H__