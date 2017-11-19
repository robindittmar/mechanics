#ifndef __SKINCHANGER_H__
#define __SKINCHANGER_H__

// Std lib
#include <unordered_map>

// Source SDK
#include "ClientEntity.h"
#include "IClientState.h"
#include "IGameEvent.h"

// Custom
#include "IFeature.h"
#include "SkinMetadata.h"
#include "murmurhash.h"

#define GLOVE_NONE				0
#define GLOVE_BLOODHOUND		1
#define GLOVE_DRIVER			2
#define GLOVE_HANDWRAPS			3
#define GLOVE_MOTO				4
#define GLOVE_SPECIALIST		5
#define GLOVE_SPORT				6

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1

#define RandomIntDef(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin)

class CApplication;

struct WeaponMetadata_t
{
	WeaponMetadata_t(int iId = 0, const char* pName = NULL)
	{
		id = iId;
		name = pName;
	}

	int id;
	const char* name;
};

// Hint: also changes models :D
class CSkinChanger : public IFeature
{
public:
	CSkinChanger();
	~CSkinChanger();

	std::unordered_map<uint32_t, WeaponMetadata_t>* GetWeaponsMap() { return &m_mapWeapons; }
	std::unordered_map<int, std::unordered_map<int, const char*>>* GetSkinsMap() { return &m_mapSkins; }

	virtual void Setup() override;
	virtual void Update(void* pParameters = 0) override;

	void ParseSkinFile();

	void SetForceFullUpdate(bool bForceFullUpdate = true) { m_bForceFullUpdate = bForceFullUpdate; }

	void ClearReplacements();

	void SetDesiredKnifeModelIndex(int iDesiredKnifeModelIndex) { m_iDesiredKnifeModelIndex = iDesiredKnifeModelIndex; }
	int GetDesiredKnifeModelIndex() { return m_iDesiredKnifeModelIndex; }

	void SetNewMap(bool bNewMap = true) { m_bNewMap = bNewMap; }

	// pNew won't be affected
	void AddModelReplacement(const char* pOld, const char* pNew);
	// After passing pSkin to this function the SkinChanger takes care of cleaning up the heap
	void AddSkinReplacement(int iWeaponId, CSkinMetadata* pSkin);
	// pNew won't be affected
	void AddKillIconReplacement(const char* pOld, const char* pNew);

	bool ApplyCustomModel(IClientEntity* pLocal, CBaseAttributableItem* pItem);
	bool ApplyCustomSkin(CBaseAttributableItem* pWeapon, int iWeaponId);
	bool ApplyCustomKillIcon(IGameEvent* pEvent);
	void ApplyDesiredKnife(int iDesiredKnifeModelIndex);
private:
	// Delete's all items of the maps
	void DeleteModelNames();
	void DeleteSkinMetadata();
	void DeleteKillIcons();

	bool m_bForceFullUpdate;
	bool m_bNewMap;

	int m_iDesiredKnifeModelIndex;
	std::unordered_map<int, const char*> m_mapKnives;
	std::unordered_map<uint32_t, WeaponMetadata_t> m_mapWeapons;

	std::unordered_map<int, CSkinMetadata*> m_mapSkinMetadata;
	std::unordered_map<int, const char*> m_mapModelMetadata;
	std::unordered_map<uint32_t, const char*> m_mapKillIcon;

	std::unordered_map<uint32_t, int> m_mapPaintKits;
	std::unordered_map<uint32_t, const char*> m_mapPaintKitDescription;
	std::unordered_map<uint32_t, std::vector<int>> m_mapWeaponSkins;
	std::unordered_map<int, std::unordered_map<int, const char*>> m_mapSkins;
};

#endif // __SKINCHANGER_H__