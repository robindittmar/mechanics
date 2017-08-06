#ifndef __MISC_H__
#define __MISC_H__

#include "Feature.h"

#define NOFLASH_OFFSET 0xA304

class CApplication;
class CUserCmd;

class CMisc : public IFeature
{
public:
	CMisc();
	~CMisc();

	virtual void Setup();
	virtual void Update(void* pParameters);
	virtual void NoRecoil(CUserCmd* pUserCmd);
private:
};


#endif // __MISC_H__