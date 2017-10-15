#ifndef __CHAMS_H__
#define __CHAMS_H__

// SDK
#include "IMaterial.h"
#include "IVModelRender.h"
#include "ISurface.h" // class Color

// Custom
#include "IFeature.h"

#define PLAYER_CHAMSSTYLE_NONE			0
#define PLAYER_CHAMSSTYLE_LIT			1
#define PLAYER_CHAMSSTYLE_FLAT			2

#define WEAPON_CHAMSSTYLE_NONE			0
#define WEAPON_CHAMSSTYLE_GLASS			1
#define WEAPON_CHAMSSTYLE_PLATINUM		2
#define WEAPON_CHAMSSTYLE_CRYSTAL		3
#define WEAPON_CHAMSSTYLE_GOLD			4

class IMatRenderContext;
typedef void(__thiscall *DrawModelExecute_t)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);

class CChams : public IFeature
{
public:
	CChams();
	~CChams();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);

	void SetRenderTeam(bool bRender) { m_bRenderTeam = bRender; }
	bool GetRenderTeam() { return m_bRenderTeam; }

	void SetRenderLocalplayer(bool bRender) { m_bRenderLocalplayer = bRender; }
	bool GetRenderLocalplayer() { return m_bRenderLocalplayer; }

	void SetOnlyVisible(bool bOnlyVisible) { m_bOnlyVisible = bOnlyVisible; }
	bool GetOnlyVisible() { return m_bOnlyVisible; }

	void SetFlatModels(int iPlayerChamsStyle);
	int GetFlatModels() { return m_iPlayerChamsStyle; }

	void SetRenderFakeAngle(bool bRenderFakeAngle) { m_bRenderFakeAngle = bRenderFakeAngle; }
	bool GetRenderFakeAngle() { return m_bRenderFakeAngle; }

	void SetColorHiddenCT(Color clrHiddenCT) { m_clrHiddenCT = clrHiddenCT; }
	Color GetColorHiddenCT() { return m_clrHiddenCT; }

	void SetColorVisibleCT(Color clrVisibleCT) { m_clrVisibleCT = clrVisibleCT; }
	Color GetColorVisibleCT() { return m_clrVisibleCT; }

	void SetColorHiddenT(Color clrHiddenT) { m_clrHiddenT = clrHiddenT; }
	Color GetColorHiddenT() { return m_clrHiddenT; }

	void SetColorVisibleT(Color clrVisibleT) { m_clrVisibleT = clrVisibleT; }
	Color GetColorVisibleT() { return m_clrVisibleT; }

	void SetWeaponChamsStyle(int iWeaponChamStyle) { m_iWeaponChamsStyle = iWeaponChamStyle; }
	int GetWeaponChamsStyle() { return m_iWeaponChamsStyle; }

	void ReloadMaterials();
	void DrawFakeAngle(void* ecx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	void RenderPlayerChams(const char* pszModelName, void* ecx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	void RenderWeaponChams(const char* pszModelName, void* ecx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
private:
	IVModelRender* m_pModelRender;
	DrawModelExecute_t m_pDrawModelExecute;

	bool m_bRenderTeam;
	bool m_bRenderLocalplayer;
	bool m_bOnlyVisible;
	int m_iPlayerChamsStyle;
	bool m_bRenderFakeAngle;

	Color m_clrHiddenCT;
	Color m_clrVisibleCT;
	Color m_clrHiddenT;
	Color m_clrVisibleT;

	int m_iWeaponChamsStyle;

	bool m_bMaterialsInitialized;
	IMaterial* m_pFlatHiddenCT;
	IMaterial* m_pFlatVisibleCT;
	IMaterial* m_pFlatHiddenT;
	IMaterial* m_pFlatVisibleT;

	IMaterial* m_pLitHiddenCT;
	IMaterial* m_pLitVisibleCT;
	IMaterial* m_pLitHiddenT;
	IMaterial* m_pLitVisibleT;

	IMaterial* m_pHiddenCT;
	IMaterial* m_pVisibleCT;
	IMaterial* m_pHiddenT;
	IMaterial* m_pVisibleT;

	bool m_bFakeAngleMaterialsInitialized;
	IMaterial* m_pFakeAngle;
	IMaterial* m_pFlatFakeAngle;
	IMaterial* m_pLitFakeAngle;
};

#endif // __CHAMS_H__