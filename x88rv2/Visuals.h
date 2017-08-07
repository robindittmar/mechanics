#ifndef __VISUALS_H__
#define __VISUALS_H__

#include "Feature.h"

#define RECOIL_COMPENSATION 2
#define RECOIL_TRACKING 0.4499999f

enum HandsDrawStyles_t {
	HandsDrawStyleNone,
	HandsDrawStyleNoHands,
	HandsDrawStyleWireframe
};
class CViewSetup;

class CVisuals : public IFeature
{
public:
	CVisuals();
	~CVisuals();

	void IsNoFlash(bool bNoFlash) { m_bNoFlash = bNoFlash; };
	bool IsNoFlash() { return m_bNoFlash; };

	void IsNoSmoke(bool bNoSmoke) { m_bNoSmoke = bNoSmoke; };
	bool IsNoSmoke() { return m_bNoSmoke; };

	void HandsDrawStyle(HandsDrawStyles_t tHandsDrawStyle) { m_tHandsDrawStyle = tHandsDrawStyle; };
	HandsDrawStyles_t HandsDrawStyle() { return m_tHandsDrawStyle; };

	void IsNoVisualRecoil(bool bNoVisualRecoil) { m_bNoVisualRecoil = bNoVisualRecoil; };
	bool IsNoVisualRecoil() { return m_bNoVisualRecoil; };

	virtual void Setup();
	virtual void Update(void* pParameters = 0);

	void NoFlash(float fFlashPercentage);
	void NoSmoke();
	void HandsDrawStyle(const char*);
	void NoVisualRecoil(CViewSetup*);
private:
	bool m_bNoFlash;
	bool m_bNoSmoke;
	HandsDrawStyles_t m_tHandsDrawStyle;
	bool m_bNoVisualRecoil;
};

#endif // __VISUALS_H__