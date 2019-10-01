#ifndef __FAKEDUCK_H__
#define __FAKEDUCK_H__

#include "IFeature.h"
#include "UserCmd.h"

class CApplication;

class CFakeduck : public IFeature
{
public:
    CFakeduck();
    virtual ~CFakeduck();

    virtual void Think(void* pParameters = nullptr) override;
private:
};

#endif // __FAKEDUCK_H__