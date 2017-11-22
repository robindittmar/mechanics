#ifndef __CROSSHAIR_H__
#define __CROSSHAIR_H__

// Std Lib
#include <assert.h>

// Source SDK
#include "ISurface.h"

// Custom
#include "IFeature.h"
#include "Gui.h"

// Used for PlaySound
#pragma comment(lib, "winmm.lib")

#define SPREADCONE_DEFAULT_COLOR	Color(255, 255, 0, 0)
#define HITMARKER_DEFAULT_TIME		1.0f

#define SPREADCONE_STYLE_OUTLINE	0
#define SPREADCONE_STYLE_FILLED		1

class CGunHud : public IFeature
{
public:
	CGunHud();
	virtual ~CGunHud();

	void SetSurface(ISurface* pSurface) { m_pSurface = pSurface; }

	void SetPlayerHurtTime(float fPlayerHurtTime) { m_fPlayerHurtTime = fPlayerHurtTime; }
	void SetWeaponFireTime(float fWeaponFireTime) { m_fWeaponFireTime = fWeaponFireTime; }

	void SetCrosshair(bool bCrosshair) { m_bCrosshair = bCrosshair; }
	bool GetCrosshair() { return m_bCrosshair; }

	void SetCrosshairShowRecoil(bool bCrosshairShowRecoil) { m_bCrosshairShowRecoil = bCrosshairShowRecoil; }
	bool GetCrosshairShowRecoil() { return m_bCrosshairShowRecoil; }

	void SetSpreadCone(bool bSpreadCone) { m_bSpreadCone = bSpreadCone; }
	bool GetSpreadCone() { return m_bSpreadCone; }

	void SetSpreadConeStyle(int iSpreadConeStyle) { m_iSpreadConeStyle = iSpreadConeStyle; }
	int GetSpreadConeStyle() { return m_iSpreadConeStyle; }

	void SetSpreadConeShowRecoil(bool bSpreadConeShowRecoil) { m_bSpreadConeShowRecoil = bSpreadConeShowRecoil; }
	bool GetSpreadConeShowRecoil() { return m_bSpreadConeShowRecoil; }

	void SetSpreadConeColor(Color clrSpreadCone) { m_clrSpreadCone = clrSpreadCone; }
	Color GetSpreadConeColor() { return m_clrSpreadCone; }

	void SetHitmarker(bool bHitmarker) { m_bHitmarker = bHitmarker; }
	bool GetHitmarker() { return m_bHitmarker; }

	void SetHitmarkerShowTime(float fHitmarkerShowTime) { m_fHitmarkerShowTime = fHitmarkerShowTime; }
	float GetHitmarkerShowTime() { return m_fHitmarkerShowTime; }

	void SetHitmarkerSound(bool bHitmarkerSound) { m_bHitmarkerSound = bHitmarkerSound; }
	bool GetHitmarkerSound() { return m_bHitmarkerSound; }

	void SetHitmarkerHitpoint(bool bHitmarkerHitpoint) { m_bHitmarkerHitpoint = bHitmarkerHitpoint; }
	bool GetHitmarkerHitpoint() { return m_bHitmarkerHitpoint; }

	virtual void Setup() override;
	virtual void Think(void* pParameters = nullptr) override;

	void TriggerHitmarker();
	void ResetHitmarker();

	void DrawCrosshair();
	void DrawSpreadCone();
	void DrawHitmarker();
	void DrawHitmarkerHitpoint();
private:
	ISurface* m_pSurface;

	float m_fPlayerHurtTime;
	float m_fWeaponFireTime;

	int m_iMiddleX, m_iMiddleY;
	int m_iRcMiddleX, m_iRcMiddleY;
	bool m_bRecoilIsNonZero;

	bool m_bCrosshair;
	bool m_bCrosshairShowRecoil;

	bool m_bSpreadCone;
	int m_iSpreadConeStyle;
	bool m_bSpreadConeShowRecoil;
	Color m_clrSpreadCone;

	bool m_bHitmarker;
	bool m_bHitmarkerSound;
	bool m_bHitmarkerHitpoint;

	float m_fHitmarkerShowTime;
	float m_fDrawHitmarkerStartTime;
	char m_pHitmarkerSound[MAX_PATH];

	CGui* m_pGui;
};

#endif // __CROSSHAIR_H__