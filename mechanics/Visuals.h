#ifndef __VISUALS_H__
#define __VISUALS_H__

#include "Offsets.h"
#include "IFeature.h"
#include "IVModelRender.h"
#include "ClientEntity.h"
#include "CViewSetup.h"

#define HITMARKER_DEFAULT_TIME		1.0f

#define HANDSDRAWSTYLE_NONE			0
#define HANDSDRAWSTYLE_NOHANDS		1
#define HANDSDRAWSTYLE_WIREFRAME	2

class IMatRenderContext;

class CVisuals : public IFeature
{
public:
	CVisuals();
	~CVisuals();

	void SetCrosshair(bool bCrosshair) { m_bCrosshair = bCrosshair; }
	bool GetCrosshair() { return m_bCrosshair; }

	void SetCrosshairShowRecoil(bool bCrosshairShowRecoil) { m_bCrosshairShowRecoil = bCrosshairShowRecoil; }
	bool GetCrosshairShowRecoil() { return m_bCrosshairShowRecoil; }

	void SetHitmarker(bool bHitmarker) { m_bHitmarker = bHitmarker; }
	bool GetHitmarker() { return m_bHitmarker; }

	void SetNoFlash(bool bNoFlash) { m_bNoFlash = bNoFlash; }
	bool GetNoFlash() { return m_bNoFlash; }
	void SetFlashPercentage(float fNoFlashPercentage) { m_fFlashPercentage = fNoFlashPercentage; }
	float GetFlashPercentage() { return m_fFlashPercentage; }

	void SetNoSmoke(bool bNoSmoke) { m_bNoSmoke = bNoSmoke; }
	bool GetNoSmoke() { return m_bNoSmoke; }

	void SetHandsDrawStyle(int iHandsDrawStyle) { m_iHandsDrawStyle = iHandsDrawStyle; }
	int GetHandsDrawStyle() { return m_iHandsDrawStyle; }

	void SetNoVisualRecoil(bool bNoVisualRecoil) { m_bNoVisualRecoil = bNoVisualRecoil; }
	bool GetNoVisualRecoil() { return m_bNoVisualRecoil; }

	void SetThirdperson(bool bThirdperson) { m_bThirdperson = bThirdperson; }
	bool GetThirdperson() { return m_bThirdperson; }
	void SetThirdpersonDistance(int iThirdpersonValue) { m_iThirdpersonValue = iThirdpersonValue; }
	int GetThirdpersonValue() { return m_iThirdpersonValue; }

	void SetFovChange(bool bFovChange) { m_bFovChange = bFovChange; }
	bool GetFovChange() { return m_bFovChange; }
	void SetFovChangeScoped(bool bFovChangeScoped) { m_bFovChangeScoped = bFovChangeScoped; }
	int GetFovChangeScoped() { return m_bFovChangeScoped; }
	void SetFovValue(int iFovValue) { m_iFovValue = iFovValue; }
	int GetFovValue() { return m_iFovValue; }

	void SetDrawLbyIndicator(bool bLbyIndicator) { m_bLbyIndicator = bLbyIndicator; }
	bool GetDrawLbyIndicator() { return m_bLbyIndicator; }

	void SetZoomSensitivity(float flZoomSensitivity) { m_flZoomSensitivity = flZoomSensitivity; }
	float GetZoomSensitivity() { return m_flZoomSensitivity; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);

	void TriggerHitmarker(float fTime = HITMARKER_DEFAULT_TIME);
	void UpdateHitmarker(float fInputSampleTime);

	void DrawCrosshair();
	void DrawHitmarker();
	void NoFlash();
	void NoSmoke();
	void HandsDrawStyle(const char*, void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	void NoVisualRecoil(CViewSetup*);
	void Thirdperson();
	void ThirdpersonAntiAim();
	void FovChange(CViewSetup*);
private:
	int m_iSurfaceWidth;
	int m_iSurfaceHeight;

	bool m_bCrosshair;
	bool m_bCrosshairShowRecoil;

	bool m_bHitmarker;
	float m_fDrawHitmarkerTime;
	float m_fDrawHitmarkerStartTime;

	bool m_bNoFlash;
	float m_fFlashPercentage;

	bool m_bNoSmoke;
	int m_iHandsDrawStyle;
	bool m_bNoVisualRecoil;

	bool m_bThirdperson;
	int m_iThirdpersonValue;

	bool m_bFovChange;
	bool m_bFovChangeScoped;
	int m_iFovValue;

	bool m_bLbyIndicator;

	float m_flZoomSensitivity;
};

#endif // __VISUALS_H__