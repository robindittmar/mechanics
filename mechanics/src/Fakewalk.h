#ifndef __FAKEWALK_H__
#define __FAKEWALK_H__

#include "IFeature.h"
#include "UserCmd.h"

class CApplication;

class CFakewalk : public IFeature
{
public:
	CFakewalk();
	virtual ~CFakewalk();

	virtual void Think(void* pParameters = nullptr) override;
private:
};

#endif // __FAKEWALK_H__