#ifndef __ANTIAIM_H__
#define __ANTIAIM_H__

#include "Feature.h"

class CApplication;

class CAntiaim : public IFeature
{
public:
	CAntiaim();
	~CAntiaim();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	CApplication* m_pApp;
};

#endif // __ANTIAIM_H__