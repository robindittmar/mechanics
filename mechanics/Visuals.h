#ifndef __VISUALS_H__
#define __VISUALS_H__

// Std Lib
#include <Windows.h>

// Source SDK
#include "IVModelRender.h"
#include "ClientEntity.h"
#include "CViewSetup.h"
#include "ISurface.h"

// Custom
#include "Offsets.h"
#include "IFeature.h"

#define HANDSDRAWSTYLE_NONE			0
#define HANDSDRAWSTYLE_NOHANDS		1
#define HANDSDRAWSTYLE_WIREFRAME	2

#define CAM_MIN_DIST				30.0
#define CAM_MAX_DIST				300.0
#define CAM_HULL_OFFSET				9.0    // the size of the bounding hull used for collision checking
static Vector CAM_HULL_MIN(-CAM_HULL_OFFSET, -CAM_HULL_OFFSET, -CAM_HULL_OFFSET);
static Vector CAM_HULL_MAX(CAM_HULL_OFFSET, CAM_HULL_OFFSET, CAM_HULL_OFFSET);

class IMatRenderContext;

class CVisuals : public IFeature
{
public:
	CVisuals();
	~CVisuals();

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

	// TODO: Wäre SetFovEnabled nicht aussagekräftiger?
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

	float GetZoomSensitivity() { return m_flZoomSensitivity; }

	void SetDisablePostProcessing(bool bDisablePostProcessing) { m_bDisablePostProcessing = bDisablePostProcessing; }
	bool GetDisablePostProcessing() { return m_bDisablePostProcessing; }

	void SetNoScope(bool bNoScope) { m_bNoScope = bNoScope; }
	bool GetNoScope() { return m_bNoScope; }

	void SetNightmode(bool bNightmode) { m_bNightmode = bNightmode; }
	bool GetNightmode() { return m_bNightmode; }

	void SetNightmodePerfomed(bool bNightmodePerfomed) { m_bNightmodePerfomed = bNightmodePerfomed; }
	void SetNightmodeMapChanged(bool bNightmodeMapChange) { m_bNightmodeMapChange = bNightmodeMapChange; }

	void SetAsusWalls(bool bAsusWalls) { m_bAsusWalls = bAsusWalls; }
	bool GetAsusWalls() { return m_bAsusWalls; }

	void SetAsusWallsPerformed(bool bAsusWallsPerformed) { m_bAsusWallsPerformed = bAsusWallsPerformed; }

	void SetBulletTracer(bool bBulletTracer) { m_bBulletTracer = bBulletTracer; }
	bool GetBulletTracer() { return m_bBulletTracer; }

	void SetBulletTracerSelf(bool bBulletTracerSelf) { m_bBulletTracerSelf = bBulletTracerSelf; }
	bool GetBulletTracerSelf() { return m_bBulletTracerSelf; }

	void SetBulletTracerTeam(bool bBulletTracerTeam) { m_bBulletTracerTeam = bBulletTracerTeam; };
	bool GetBulletTracerTeam() { return m_bBulletTracerTeam; }

	virtual void Setup() override;
	virtual void Update(void* pParameters = 0) override;

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
	void Nightmode();
	void AsusWalls();
	void DrawBulletTracer();
private:
	int m_iSurfaceWidth;
	int m_iSurfaceHeight;

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

	bool m_bNightmode;
	bool m_bNightmodePerfomed;
	bool m_bNightmodeMapChange;

	bool m_bAsusWalls;
	bool m_bAsusWallsPerformed;

	bool m_bBulletTracer;
	bool m_bBulletTracerSelf;
	bool m_bBulletTracerTeam;
};

#endif // __VISUALS_H__