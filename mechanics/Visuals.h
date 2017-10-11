#ifndef __VISUALS_H__
#define __VISUALS_H__

// Std Lib
#include <Windows.h>

// Custom
#include "Offsets.h"
#include "IFeature.h"
#include "IVModelRender.h"
#include "ClientEntity.h"
#include "CViewSetup.h"

#pragma comment(lib, "winmm.lib")

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

	void SetSpreadCone(bool bSpreadCone) { m_bSpreadCone = bSpreadCone; }
	bool GetSpreadCone() { return m_bSpreadCone; }

	void SetHitmarker(bool bHitmarker) { m_bHitmarker = bHitmarker; }
	bool GetHitmarker() { return m_bHitmarker; }

	void SetNoFlash(bool bNoFlash) { m_bNoFlash = bNoFlash; NoFlash(m_fFlashPercentage); }
	bool GetNoFlash() { return m_bNoFlash; }
	void SetFlashPercentage(float fFlashPercentage) { m_fFlashPercentage = fFlashPercentage; }
	float GetFlashPercentage() { return m_fFlashPercentage; }

	void SetNoSmoke(bool bNoSmoke) { m_bNoSmoke = bNoSmoke; }
	bool GetNoSmoke() { return m_bNoSmoke; }

	void SetHandsDrawStyle(int iHandsDrawStyle) { m_iHandsDrawStyle = iHandsDrawStyle; }
	int GetHandsDrawStyle() { return m_iHandsDrawStyle; }

	void SetNoVisualRecoil(bool bNoVisualRecoil) { m_bNoVisualRecoil = bNoVisualRecoil; }
	bool GetNoVisualRecoil() { return m_bNoVisualRecoil; }

	void SetThirdperson(bool bThirdperson) { m_bThirdperson = bThirdperson; }
	bool GetThirdperson() { return m_bThirdperson; }
	void SetThirdpersonDistance(int iThirdpersonValue) { m_iThirdpersonDistance = iThirdpersonValue; }
	int GetThirdpersonDistance() { return m_iThirdpersonDistance; }

	void SetFovChange(bool bFovChange) { m_bFovChange = bFovChange; }
	bool GetFovChange() { return m_bFovChange; }
	void SetFovChangeScoped(bool bFovChangeScoped) { m_bFovChangeScoped = bFovChangeScoped; }
	int GetFovChangeScoped() { return m_bFovChangeScoped; }
	void SetFovValue(int iFovValue) { m_iFovValue = iFovValue; }
	int GetFovValue() { return m_iFovValue; }

	void SetViewmodelFov(bool bViewmodelFovChange) { m_bViewmodelFovChange = bViewmodelFovChange; }
	bool GetViewmodelFov() { return m_bViewmodelFovChange; }
	void SetViewmodelFovValue(int iViewmodelFovValue) { m_iViewmodelFovValue = iViewmodelFovValue; }
	int GetViewmodelFovValue() { return m_iViewmodelFovValue; }

	void SetZoomSensitivity(float flZoomSensitivity) { m_flZoomSensitivity = flZoomSensitivity; }
	float GetZoomSensitivity() { return m_flZoomSensitivity; }

	void SetDisablePostProcessing(bool bDisablePostProcessing) { m_bDisablePostProcessing = bDisablePostProcessing; };
	bool GetDisablePostProcessing() { return m_bDisablePostProcessing; };

	void SetNoScope(bool bNoScope) { m_bNoScope = bNoScope; };
	bool GetNoScope() { return m_bNoScope; };

	virtual void Setup();
	virtual void Update(void* pParameters = 0);

	void TriggerHitmarker(float fTime = HITMARKER_DEFAULT_TIME);
	void UpdateHitmarker(float fInputSampleTime);

	void DrawCrosshair();
	void DrawSpreadCone();
	void DrawHitmarker();
	void NoFlash(float fFlashPercentage);
	void NoSmoke(bool bNoSmoke);
	IMaterial* HandsDrawStyle(const char*, void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	void NoVisualRecoil(CViewSetup*);
	void Thirdperson();
	void ThirdpersonAntiAim();
	void FovChange(CViewSetup*);
	void DisablePostProcessing(bool bDisablePostProcessing);
	void DrawNoScope();
	bool NoScope(unsigned int vguiPanel);
private:
	int m_iSurfaceWidth;
	int m_iSurfaceHeight;

	bool m_bCrosshair;
	bool m_bCrosshairShowRecoil;

	bool m_bSpreadCone;

	char m_pHitmarkerSound[MAX_PATH];
	bool m_bHitmarker;
	float m_fDrawHitmarkerTime;
	float m_fDrawHitmarkerStartTime;

	bool m_bNoFlash;
	float m_fFlashPercentage;

	bool m_bNoSmoke;
	int m_iHandsDrawStyle;
	bool m_bNoVisualRecoil;

	bool m_bThirdperson;
	int m_iThirdpersonDistance;

	bool m_bFovChange;
	bool m_bFovChangeScoped;
	int m_iFovValue;

	bool m_bViewmodelFovChange;
	int m_iViewmodelFovValue;

	float m_flZoomSensitivity;

	bool m_bDisablePostProcessing;
	bool* m_dwOverridePostProcessingDisable;

	bool m_bNoScope;
};

#endif // __VISUALS_H__