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

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin)

class CApplication;

// Hint: also changes models :D
class CSkinChanger : public IFeature
{
public:
	CSkinChanger();
	~CSkinChanger();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);

	void SetForceFullUpdate(bool bForceFullUpdate = true) { m_bForceFullUpdate = bForceFullUpdate; }

	void ClearReplacements();

	// pNew won't be affected
	void AddModelReplacement(const char* pOld, const char* pNew);
	// After passing pSkin to this function the SkinChanger takes care of cleaning up the heap
	void AddSkinReplacement(int iWeaponId, CSkinMetadata* pSkin);
	// pNew won't be affected
	void AddKillIconReplacement(const char* pOld, const char* pNew);

	bool ApplyCustomModel(IClientEntity* pLocal, CBaseAttributableItem* pItem);
	bool ApplyCustomSkin(CBaseAttributableItem* pWeapon, int iWeaponId);
	bool ApplyCustomKillIcon(IGameEvent* pEvent);
private:
	// Delete's all items of the maps
	void DeleteModelNames();
	void DeleteSkinMetadata();
	void DeleteKillIcons();

	bool m_bForceFullUpdate;

	std::unordered_map<int, CSkinMetadata*> m_mapSkinMetadata;
	std::unordered_map<int, const char*> m_mapModelMetadata;
	std::unordered_map<uint32_t, const char*> m_mapKillIcon;};

#endif // __SKINCHANGER_H__