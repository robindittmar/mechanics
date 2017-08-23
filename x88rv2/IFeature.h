#ifndef __FEATURE_H__
#define __FEATURE_H__

class CApplication;

class IFeature
{
public:
	IFeature();
	~IFeature();

	// TODO: Setup & Update will probably need parameters
	//		 sooner or later (config class, CApplication* or something)
	virtual void Setup() = 0;
	virtual void Update(void* pParameters = 0) = 0;

	void SetEnabled(bool bIsEnabled) { m_bIsEnabled = bIsEnabled; }
	bool GetEnabled()				{ return m_bIsEnabled; }
protected:
	CApplication* m_pApp;
	bool m_bIsEnabled;
};

#endif // __FEATURE_H__