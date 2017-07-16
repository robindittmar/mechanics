#ifndef __MISC_H__
#define __MISC_H__

#include "Feature.h"

#define NOFLASH_OFFSET 0xA304

class CApplication;

class CMisc : public IFeature
{
public:
	CMisc();
	~CMisc();

	virtual void Setup();
	virtual void Update(void* pParameters);
	virtual void NoFlash(int flashPercentage);
private:
	CApplication* m_pApp;
};


#endif // __MISC_H__