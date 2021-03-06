#ifndef __RADAR_H__
#define __RADAR_H__

// Std Lib

// Source SDK
#include "../source_sdk/ClientEntity.h"

// Custom
#include "IFeature.h"

#define	RADAR_TYPE_INGAME			0
#define RADAR_TYPE_CUSTOM			1

class CRadar : public IFeature
{
public:
	CRadar();
	virtual ~CRadar();

	void SetType(int iType) { m_iType = iType; }
	int GetType() { return m_iType; }

	virtual void Think(void* pParameters = nullptr) override;
private:
	int m_iType;
};

#endif // __RADAR_H__