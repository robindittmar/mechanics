#ifndef __SKINCHANGER_H__
#define __SKINCHANGER_H__

// Std lib
#include <algorithm>
#include <vector>
#include <unordered_map>

// Source SDK
#include "../source_sdk/ClientEntity.h"
#include "../source_sdk/IClientState.h"
#include "../source_sdk/IGameEvent.h"

// Custom
#include "IFeature.h"
#include "../core/SkinMetadata.h"
#include "../third_party/murmurhash/murmurhash.h"

#define SKINCHANGER_CONFIG_FOLDER_FULL_RELATIVE		CXorString(/*cfg\\skins\\*/"\x74\x6D\xE2\x9E\x4B\x78\xEE\xAB\x79\x78\xD9\x9E").ToCharArray()
#define SKINCHANGER_CONFIG_FOLDER					CXorString(/*skins\\*/"\x64\x60\xEC\xAC\x64\x57\xD9").ToCharArray()

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
	WeaponMetadata_t(int iId = 0, const char* pName = nullptr, const char* pReadableName = nullptr)
	{
		id = iId;
		name = pName;
		readableName = pReadableName;
	}

	int id;
	const char* name;
	const char* readableName;
};

struct SkinMetadata_t
{
	SkinMetadata_t(int iId = 0, const wchar_t* pName = nullptr)
	{
		id = iId;
		name = pName;
	}

	int id;
	const wchar_t* name;
};

// Hint: also changes models :D
class CSkinChanger : public IFeature
{
public:
	CSkinChanger();
	virtual ~CSkinChanger();

	std::unordered_map<uint32_t, WeaponMetadata_t>* GetWeaponsMap() { return &m_mapWeapons; }
	std::unordered_map<int, std::unordered_map<int, const wchar_t*>>* GetSkinsMap() { return &m_mapSkins; }

	std::vector<WeaponMetadata_t>* GetWeapons() { return &m_vWeapons; }
	std::unordered_map<int, std::vector<SkinMetadata_t>>* GetSkins() { return &m_vSkins; }

	virtual void Setup() override;
	virtual void Think(void* pParameters = nullptr) override;

	void ParseSkinFile();
	void SortWeaponsAndSkinsByName();

	void SetForceFullUpdate(bool bForceFullUpdate = true) { m_bForceFullUpdate = bForceFullUpdate; }

	void ClearReplacements();

	int GetDesiredKnifeModelIndexCT() { return m_iDesiredKnifeModelIndexCT; }
	int GetDesiredKnifeModelIndexT() { return m_iDesiredKnifeModelIndexT; }

	void SetOnlyMyWeapons(bool bOnlyMyWeapons) { m_bOnlyMyWeapons = bOnlyMyWeapons; }
	bool GetOnlyMyWeapons() { return m_bOnlyMyWeapons; }

	/// <summary>
	/// Returns the CSkinMetadata for the WeaponId
	/// </summary>
	/// <param name="iWeaponId">WeaponId of weapon to check</param>
	/// <remarks>Can only be called if ingame, since it's relying on LocalPlayer teamnum</remarks>
	/// <returns>CSkinMetadata for changed skin for WeaponId</returns>
	CSkinMetadata* GetSkinMetadataForWeapon(int iWeaponId);

	// pNew won't be affected
	void AddModelReplacement(int iTeamNum, const char* pOld, const char* pNew);
	// After passing pSkin to this function the SkinChanger takes care of cleaning up the heap
	void AddSkinReplacement(int iTeamNum, int iWeaponId, CSkinMetadata* pSkin);
	// pNew won't be affected
	void AddKillIconReplacement(int iTeamNum, const char* pOld, const char* pNew);

	bool ApplyCustomModel(IClientEntity* pLocal, CBaseAttributableItem* pItem);
	bool ApplyCustomSkin(CBaseAttributableItem* pWeapon, int iWeaponId);
	bool ApplyCustomKillIcon(IGameEvent* pEvent);
	void ApplyDesiredKnife(int iTeamNum, int iDesiredKnifeModelIndex, bool bApplySkin = false, int iPaintKit = 0, int iSeed = 0, int iStatTrak = -1, const char* pName = nullptr, float fWear = 0.01f);

	bool WriteToConfig(const char* pFilename);
	bool LoadFromConfig(const char* pFilename);
private:
	// Delete's all items of the maps
	void DeleteSkinMetadata();
	void DeleteModelNames(int iTeamNum);
	void DeleteKillIcons(int iTeamNum);

	bool m_bForceFullUpdate;

	bool m_bOnlyMyWeapons;

	int m_iDesiredKnifeModelIndexCT;
	int m_iDesiredKnifeModelIndexT;
	std::unordered_map<int, const char*> m_mapKnives;
	std::unordered_map<uint32_t, WeaponMetadata_t> m_mapWeapons;
	std::unordered_map<uint32_t, WeaponMetadata_t> m_mapKnifeWeapons;

	// Sorted
	std::vector<WeaponMetadata_t> m_vWeapons;
	std::unordered_map<int, std::vector<SkinMetadata_t>> m_vSkins;
	// /Sorted

	std::unordered_map<int, CSkinMetadata*> m_mapSkinMetadataCT;
	std::unordered_map<int, const char*> m_mapModelMetadataCT;
	std::unordered_map<uint32_t, const char*> m_mapKillIconCT;

	std::unordered_map<int, CSkinMetadata*> m_mapSkinMetadataT;
	std::unordered_map<int, const char*> m_mapModelMetadataT;
	std::unordered_map<uint32_t, const char*> m_mapKillIconT;

	std::unordered_map<uint32_t, int> m_mapPaintKits;
	std::unordered_map<uint32_t, const wchar_t*> m_mapPaintKitDescription;
	std::unordered_map<uint32_t, std::vector<int>> m_mapWeaponSkins;
	std::unordered_map<int, std::unordered_map<int, const wchar_t*>> m_mapSkins;
};

#endif // __SKINCHANGER_H__