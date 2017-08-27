#ifndef __SKINCHANGER_H__
#define __SKINCHANGER_H__

#include "IFeature.h"
#include "ClientEntity.h"

#include <unordered_map>

struct SkinItemCfg {
	int m_iItemDefinitionIndex = -1;
	int m_iFallbackPaintKit = 0;
	int m_iFallbackSeed = 0;
	int m_iFallbackStatTrak = -1;
	int m_iEntityQuality = 4;
	char* m_szCustomName = nullptr;
	float m_flFallbackWear = 0.00001f;
};

class CApplication;

class CSkinChanger : public IFeature
{
public:
	CSkinChanger();
	~CSkinChanger();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);

	void SetKnifeModel(const char* pModel);

	bool ApplyCustomModel(IClientEntity* pLocal, CBaseAttributableItem* pWeapon, int nWeaponIndex);
	bool ApplyCustomSkin(CBaseAttributableItem* pWeapon, int nWeaponIndex);

	void AddSkinConfig(int wepId, SkinItemCfg& cfg) { m_mapSkinChangerCfg[wepId] = cfg; }
private:
	char* m_pKnifeModel;

	std::unordered_map<int, SkinItemCfg> m_mapSkinChangerCfg;
	std::unordered_map<int, const char*> m_mapModelCfg;
	std::unordered_map<int, int> m_mapWeaponDict;
};

#endif // __SKINCHANGER_H__