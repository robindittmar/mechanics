#ifndef __FEATURE_H__
#define __FEATURE_H__

class IFeature
{
public:
	IFeature();
	~IFeature();

	// TODO: Setup & Update will probably need parameters
	//		 sooner or later (config class, CApplication* or something)
	virtual void Setup() = 0;
	virtual void Update(void* pParameters = 0) = 0;

	void IsEnabled(bool bIsEnabled) { m_bIsEnabled = bIsEnabled; }
	bool IsEnabled()				{ return m_bIsEnabled; }
private:
	bool m_bIsEnabled;
};

#endif // __FEATURE_H__