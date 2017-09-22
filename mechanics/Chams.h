#ifndef __CHAMS_H__
#define __CHAMS_H__

// SDK
#include "IMaterial.h"
#include "IVModelRender.h"

// Custom
#include "IFeature.h"

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

	void SetIgnoreZIndex(bool bIgnoreZ) { m_bIgnoreZIndex = !bIgnoreZ; } //todo GHETTOFIX
	bool GetIgnoreZIndex() { return !m_bIgnoreZIndex; }

	void SetFlatModels(bool bFlatModels);
	bool GetFlatModels() { return m_bFlatModels; }

	void ReloadMaterials();
	void Render(const char* pszModelName, void* ecx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
private:
	IVModelRender* m_pModelRender;
	DrawModelExecute_t m_pDrawModelExecute;

	bool m_bRenderTeam;
	bool m_bRenderLocalplayer;
	bool m_bIgnoreZIndex;
	bool m_bFlatModels;

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
};

#endif // __CHAMS_H__