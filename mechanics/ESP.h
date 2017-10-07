#ifndef __ESP_H__
#define __ESP_H__

// Std lib
#include <assert.h>
#include <unordered_map>

// Source SDK
#include "Vector.h"
#include "ISurface.h"
#include "ClientEntity.h"

// Custom
#include "IFeature.h"
#include "Gui.h"
#include "EspPlayer.h"

#define MAX_PLAYERS	64

#define ESP_STYLE_NONE			0
#define ESP_STYLE_EDGE			1
#define ESP_STYLE_FULL			2

class CEsp : public IFeature
{
public:
	CEsp();
	~CEsp();

	void SetFillBoundingBox(bool bFillBoundingBox) { m_bFillBoundingBox = bFillBoundingBox; }
	bool GetFillBoundingBox() { return m_bFillBoundingBox; }

	void SetDrawBoundingBox(int iDrawBoundingBox) { m_iDrawBoundingBox = iDrawBoundingBox; }
	int GetDrawBoundingBox() { return m_iDrawBoundingBox; }

	void SetDrawSkeleton(bool bDrawSkeleton) { m_bDrawSkeleton = bDrawSkeleton; }
	bool GetDrawSkeleton() { return m_bDrawSkeleton; }

	void SetDrawHealthBar(bool bDrawHealthBar) { m_bDrawHealthBar = bDrawHealthBar; }
	bool GetDrawHealthBar() { return m_bDrawHealthBar; }

	void SetDrawHealthNumber(bool bDrawHealthNumber) { m_bDrawHealthNumber = bDrawHealthNumber; }
	bool GetDrawHealthNumber() { return m_bDrawHealthNumber; }

	void SetDrawArmorBar(bool bDrawArmorBar) { m_bDrawArmorBar = bDrawArmorBar; }
	bool GetDrawArmorBar() { return m_bDrawArmorBar; }

	void SetDrawActiveWeapon(bool bDrawActiveWeapon) { m_bDrawActiveWeapon = bDrawActiveWeapon; }
	bool GetDrawActiveWeapon() { return m_bDrawActiveWeapon; }

	void SetDrawAmmoBar(bool bDrawAmmoBar) { m_bDrawAmmoBar = bDrawAmmoBar; }
	bool GetDrawAmmoBar() { return m_bDrawAmmoBar; }

	void SetDrawAmmoNumber(bool bDrawAmmoNumber) { m_bDrawAmmoNumber = bDrawAmmoNumber; }
	bool GetDrawAmmoNumber() { return m_bDrawAmmoNumber; }

	void SetDrawOwnTeam(bool bDrawOwnTeam) { m_bDrawOwnTeam = bDrawOwnTeam; }
	bool GetDrawOwnTeam() { return m_bDrawOwnTeam; }

	void SetDrawOwnModel(bool bDrawOwnModel) { m_bDrawOwnModel = bDrawOwnModel; }
	bool GetDrawOwnModel() { return m_bDrawOwnModel; }

	void SetDrawOnlyVisible(bool bDrawOnlyVisible) { m_bDrawOnlyVisible = bDrawOnlyVisible; }
	bool GetDrawOnlyVisible() { return m_bDrawOnlyVisible; }

	void SetDrawOnlySpotted(bool bDrawOnlySpotted) { m_bDrawOnlySpotted = bDrawOnlySpotted; }
	bool GetDrawOnlySpotted() { return m_bDrawOnlySpotted; }

	void SetDrawOutline(bool bDrawOutlineEsp) { m_bDrawOutline = bDrawOutlineEsp; }
	bool GetDrawOutline() { return m_bDrawOutline; }

	void SetDrawNames(bool bDrawNames) { m_bDrawNames = bDrawNames; }
	bool GetDrawNames() { return m_bDrawNames; }

	void SetDrawViewangles(bool bDrawViewangles) { m_bDrawViewangles = bDrawViewangles; }
	bool GetDrawViewangles() { return m_bDrawViewangles; }

	void SetViewanglesLength(int iViewanglesLength) { m_iViewanglesLength = iViewanglesLength; }
	int GetViewanglesLength() { return m_iViewanglesLength; }

	void SetFadeoutEnabled(bool bFadeoutEnabled) { m_bFadeoutEnabled = bFadeoutEnabled; }
	bool GetFadeoutEnabled() { return m_bFadeoutEnabled; }

	void SetFadeoutTime(float fFadeoutTime) { m_iFadeoutTime = fFadeoutTime * 1000; }
	float GetFadeoutTime() { return m_iFadeoutTime / 1000.0f; }

	void SetColorCT(Color clrCT) { m_clrCT = clrCT; }
	Color GetColorCT() { return m_clrCT; }

	void SetColorT(Color clrT) { m_clrT = clrT; }
	Color GetColorT() { return m_clrT; }

	void SetColorSpotted(Color clrSpotted) { m_clrSpotted = clrSpotted; }
	Color GetColorSpotted() { return m_clrSpotted; }

	void ResetHeadBones();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);

	void DrawSkeleton(ISurface* pSurface, IClientEntity* pEntity, matrix3x4_t* pBoneMatrix, int alpha);
private:
	void DrawArmorBar(ISurface* pSurface, int posX, int posY, int height, int width, int armor, int alpha);
	void DrawBoundingBox(ISurface* pSurface, int posX, int posY, int height, int width, Color color);
	/*void DrawSkeleton(ISurface* pSurface, IClientEntity* pEntity, matrix3x4_t* pBoneMatrix, int alpha);*/
	void DrawHealthBar(ISurface* pSurface, int posX, int posY, int height, int width, int health, int alpha);
	void DrawHealthNumber(ISurface* pSurface, int posX, int posY, int height, int width, int health, int alpha);
	void DrawActiveWeapon(ISurface* pSurface, IClientEntity* pEntity, int posX, int posY, int alpha);
	void DrawAmmoBar(ISurface* pSurface, IClientEntity* pEntity, int posX, int posY, int height, int width, int alpha);
	void DrawAmmoNumber(ISurface* pSurface, IClientEntity* pEntity, int posX, int posY, int alpha);
	void DrawHelmet(ISurface* pSurface, int posX, int posY, int height, int width, int alpha);
	void DrawName(ISurface* pSurface, IClientEntity* pEntity, int posX, int posY, int height, int width, int alpha);
	void DrawViewangles(ISurface* pSurface, int headX, int headY, Vector headPos, QAngle angles, int alpha);

	int DrawWeaponText(ISurface* pSurface, wchar_t* pText, int posX, int posY, int alpha);

	bool m_bFillBoundingBox;
	int m_iDrawBoundingBox;
	bool m_bDrawSkeleton;

	bool m_bDrawHealthBar;
	bool m_bDrawHealthNumber;
	bool m_bDrawArmorBar;
	bool m_bDrawActiveWeapon;
	bool m_bDrawAmmoBar;
	bool m_bDrawAmmoNumber;
	bool m_bDrawOwnTeam;
	bool m_bDrawOwnModel;
	bool m_bDrawOnlyVisible;
	bool m_bDrawOnlySpotted;
	bool m_bDrawOutline;
	bool m_bDrawNames;
	bool m_bDrawViewangles;
	int m_iViewanglesLength;

	bool m_bFadeoutEnabled;
	int m_iFadeoutTime;

	int m_iHeadBoneCT;
	int m_iHeadBoneT;

	int m_iFont;
	int m_iWeaponStuffOffset;

	Color m_clrCT;
	Color m_clrT;
	Color m_clrSpotted;

	CEspPlayer m_pPastPlayers[MAX_PLAYERS];

	CGui* m_pGui;
};

#endif // __ESP_H__