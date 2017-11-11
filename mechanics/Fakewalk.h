#ifndef __FAKEWALK_H__
#define __FAKEWALK_H__

#include "IFeature.h"
#include "UserCmd.h"

class CApplication;

class CFakewalk : public IFeature
{
public:
	CFakewalk();
	~CFakewalk();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
};

#endif // __FAKEWALK_H__