#ifndef __MATERIALVISUALS_H__
#define __MATERIALVISUALS_H__

#include "IFeature.h"
#include "UserCmd.h"

#define SKY_NOCHANGE		0
#define SKY_BAGGAGE			1
#define SKY_TIBET			2
#define SKY_EMBASSY			3
#define SKY_ITALY			4
#define SKY_JUNGLE			5
#define SKY_OFFICE			6
#define SKY_DAYLIGHT1		7
#define SKY_DAYLIGHT2		8
#define SKY_DAYLIGHT3		9
#define SKY_DAYLIGHT4		10
#define SKY_NUKEBLANK		11
#define SKY_VENICE			12
#define SKY_CLOUDY			13
#define SKY_NIGHT			14
#define SKY_NIGHTB			15
#define SKY_VERTIGO			16
#define SKY_VERTIGOBLUE		17
#define SKY_DUST			18
#define SKY_VIETNAM			19


class CApplication;
typedef void(__fastcall *LoadSky_t)(const char*);

class CMaterialVisuals : public IFeature
{
public:
	CMaterialVisuals();
	virtual ~CMaterialVisuals();

	void SetNightmodeValue(float fNightmodeValue) { m_fNightmodeValue = fNightmodeValue; }
	float GetNightmodeValue() { return m_fNightmodeValue; }

	void SetAsuswallsValue(float fAsuswallsValue) { m_fAsuswallsValue = fAsuswallsValue; }
	float GetAsuswallsValue() { return m_fAsuswallsValue; }

	void SetSkychangerValue(int iSkychangerValue) { m_iSkychangerValue = iSkychangerValue; }
	int GetSkychangerValue() { return m_iSkychangerValue; }

	void SetDefaultSky();

	void Nightmode(float fValue);
	void Asuswalls(float fValue);
	void Skychanger(int iValue);

	virtual void Setup() override;
	virtual void Think(void* pParameters = nullptr) override;
private:
	float m_fNightmodeValue;
	float m_fAsuswallsValue;

	LoadSky_t m_pLoadSky;
	int m_iSkychangerValue;
	char m_pDefaultSky[128];
};

#endif // __MATERIALVISUALS_H__