#ifndef __ANTIAIM_H__
#define __ANTIAIM_H__

#include "Feature.h"

class CApplication;

class CAntiAim : public IFeature
{
public:
	CAntiAim();
	~CAntiAim();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	CApplication* m_pApp;
};

#endif // __ANTIAIM_H__