#ifndef __MATERIALVISUALS_H__
#define __MATERIALVISUALS_H__

#include "IFeature.h"
#include "UserCmd.h"

class CApplication;

class CMaterialVisuals : public IFeature
{
public:
	CMaterialVisuals();
	~CMaterialVisuals();

	void Nightmode(float fValue);
	void Asuswalls(float fValue);

	virtual void Update(void* pParameters = 0) override;
private:
	float m_fNightmodeValue;

	float m_fAsuswallsValue;
};

#endif // __MATERIALVISUALS_H__