#ifndef __SKINCHANGER_H__
#define __SKINCHANGER_H__

// Std lib
#include <unordered_map>

// Source SDK
#include "ClientEntity.h"

// Custom
#include "IFeature.h"
#include "SkinMetadata.h"

class CApplication;

// Hint: also changes models :D
class CSkinChanger : public IFeature
{
public:
	CSkinChanger();
	~CSkinChanger();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);

	void ClearReplacements();

	// pNew won't be affected
	void AddModelReplacement(const char* pOld, const char* pNew);
	// After passing pSkin to this function the SkinChanger takes care of cleaning up the heap
	void AddSkinReplacement(int iWeaponId, CSkinMetadata* pSkin);

	bool ApplyCustomModel(IClientEntity* pLocal, CBaseAttributableItem* pItem);
	bool ApplyCustomSkin(CBaseAttributableItem* pWeapon, int iWeaponId);

	// ApplyCustomKillIcon
	// Der vom interface --> EventManagerVTable[9] = (DWORD)FireEventClientSideThink;
	//typedef bool(__thiscall *FireEventClientSide)(void*, IGameEvent*);
	//FireEventClientSide fnOriginalFireEventClientSide = NULL;
	// Perform kill icon replacements in here.
	//bool __fastcall FireEventClientSideThink(void* ecx, void* edx, IGameEvent* pEvent) {
private:
	// Delete's all items of the maps
	void DeleteModelNames();
	void DeleteSkinMetadata();

	std::unordered_map<int, CSkinMetadata*> m_mapSkinMetadata;
	std::unordered_map<int, const char*> m_mapModelMetadata;
};

#endif // __SKINCHANGER_H__