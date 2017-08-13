#ifndef __VISUALS_H__
#define __VISUALS_H__

#include "Feature.h"
#include "IVModelRender.h"

#define OFFSET_DEADFLAG 0x31C4

#define HITMARKER_DEFAULT_TIME	0.5f

enum HandsDrawStyles_t {
	HandsDrawStyleNone,
	HandsDrawStyleNoHands,
	HandsDrawStyleWireframe
};
class CViewSetup;
class IMatRenderContext;

class CVisuals : public IFeature
{
public:
	CVisuals();
	~CVisuals();

	void Crosshair(bool bCrosshair) { m_bCrosshair = bCrosshair; }
	bool Crosshair() { return m_bCrosshair; }

	void Hitmarker(bool bHitmarker) { m_bHitmarker = bHitmarker; }
	bool Hitmarker() { return m_bHitmarker; }

	void TriggerHitmarker(float fTime = HITMARKER_DEFAULT_TIME) { m_fDrawHitmarker = fTime; }
	void UpdateHitmarker(float fInputSampleTime) {
		m_fDrawHitmarker -= fInputSampleTime;
		if (m_fDrawHitmarker < 0.0f)
			m_fDrawHitmarker = 0.0f;
	}

	void IsNoFlash(bool bNoFlash) { m_bNoFlash = bNoFlash; }
	bool IsNoFlash() { return m_bNoFlash; }
	void NoFlashPercentage(int iNoFlashPercentage) { m_iNoFlashPercentage = iNoFlashPercentage; }
	int NoFlashPercentage() { return m_iNoFlashPercentage; }

	void IsNoSmoke(bool bNoSmoke) { m_bNoSmoke = bNoSmoke; }
	bool IsNoSmoke() { return m_bNoSmoke; }

	void HandsDrawStyle(HandsDrawStyles_t tHandsDrawStyle) { m_tHandsDrawStyle = tHandsDrawStyle; }
	HandsDrawStyles_t HandsDrawStyle() { return m_tHandsDrawStyle; }

	void IsNoVisualRecoil(bool bNoVisualRecoil) { m_bNoVisualRecoil = bNoVisualRecoil; }
	bool IsNoVisualRecoil() { return m_bNoVisualRecoil; }

	void IsThirdperson(bool bThirdperson) { m_bThirdperson = bThirdperson; }
	bool IsThirdperson() { return m_bThirdperson; }
	void ThirdpersonValue(int iThirdpersonValue) { m_iThirdpersonValue = iThirdpersonValue; }
	int ThirdpersonValue() { return m_iThirdpersonValue; }

	void IsFovChange(int bFovChange) { m_bFovChange = bFovChange; }
	int IsFovChange() { return m_bFovChange; }
	void ShouldFovChangeScoped(bool bFovChangeScoped) { m_bFovChangeScoped = bFovChangeScoped; }
	int ShouldFovChangeScoped() { return m_bFovChangeScoped; }
	void FovValue(int iFovValue) { m_iFovValue = iFovValue; }
	int FovValue() { return m_iFovValue; }

	void DrawChams(bool bChams) { m_bChams = bChams; }
	bool DrawChams() { return m_bChams; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);

	void DrawCrosshair();
	void DrawHitmarker();
	void NoFlash();
	void NoSmoke();
	void HandsDrawStyle(const char*, void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	void NoVisualRecoil(CViewSetup*);
	void Thirdperson();
	void ThirdpersonAntiAim();
	void FovChange(CViewSetup*);
	void Chams(const char*, void*, IMatRenderContext* , const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
private:
	int m_iSurfaceWidth;
	int m_iSurfaceHeight;

	bool m_bCrosshair;

	bool m_bHitmarker;
	float m_fDrawHitmarker;

	bool m_bNoFlash;
	float m_iNoFlashPercentage;

	bool m_bNoSmoke;
	HandsDrawStyles_t m_tHandsDrawStyle;
	bool m_bNoVisualRecoil;

	bool m_bThirdperson;
	int m_iThirdpersonValue;

	bool m_bFovChange;
	bool m_bFovChangeScoped;
	int m_iFovValue;

	bool m_bChams;
};

#endif // __VISUALS_H__