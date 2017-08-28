#ifndef __SKINCHANGER_H__
#define __SKINCHANGER_H__

// Std lib
#include <unordered_map>

// Source SDK
#include "ClientEntity.h"

// Custom
#include "IFeature.h"
#include "SkinMetadata.h"

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

class IClientState {
public:
	char		pad[0x9C];
	void*       net_channel;
	char		pad1[0x10];
	uint32_t    retry_number;
	char	    pad2[0x54];
	int         sign_on_state;
	char		pad3[0x8];
	float       next_cmd_time;
	int         server_count;
	int         curseq;
	char		pad4[0x4B];
	int         server_tick;
	int         client_tick;
	int         delta_tick;
	char		pad5[0x4];
	int         view_entity;
	char		pad6[0x8];
	char        level_name[260];
	char        level_name_short[40];
	char		pad7[0x49FD];
	int         last_out_cmd;
	int         choked_cmds;
	int         last_cmd;
	int         cmd;
	int         sound_sequence;
	char		pad8[0x8];
	bool        is_hltv;

	void ForceFullUpdate()
	{
		if (delta_tick != -1)
			delta_tick = -1;
	}
};

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
	// TODO: ApplyCustomKillIcon map & aufräum zeugs, dann @ Application aus dem Hook hier nen PRE-eventhandler aufrufen, der bei player_kill die weapon ändert (m_mapDeathNote[weapon])
	std::unordered_map<int, CSkinMetadata*> m_mapSkinMetadata;
	std::unordered_map<int, const char*> m_mapModelMetadata;
};

#endif // __SKINCHANGER_H__