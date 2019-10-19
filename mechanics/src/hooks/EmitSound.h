#ifndef __EMITSOUND_H__
#define __EMITSOUND_H__

#include "../source_sdk/IEngineSound.h"

struct EmitSound1Param
{
	IRecipientFilter& filter;
	int iEntIndex;
	int iChannel;
	const char* pSoundEntry;
	unsigned int nSoundEntryHash;
	const char* pSample;
	float flVolume;
	soundlevel_t iSoundlevel;
	int nSeed;
	int iFlags;
	int iPitch;
	const Vector* pOrigin;
	const Vector* pDirection;
	CUtlVector<Vector>* pUtlVecOrigins;
	bool bUpdatePositions;
	float soundtime;
	int speakerentity;
};

struct EmitSound2Param
{
	IRecipientFilter& filter;
	int iEntIndex;
	int iChannel;
	const char* pSoundEntry;
	unsigned int nSoundEntryHash;
	const char* pSample;
	float flVolume;
	float flAttenuation;
	int nSeed;
	int iFlags;
	int iPitch;
	const Vector* pOrigin;
	const Vector* pDirection;
	CUtlVector<Vector>* pUtlVecOrigins;
	bool bUpdatePositions;
	float soundtime;
	int speakerentity;
};

typedef int(__thiscall *EmitSound1_t)(void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, soundlevel_t, int, int, int, const Vector*, const Vector*, CUtlVector<Vector>*, bool, float, int, int);
typedef int(__thiscall *EmitSound2_t)(void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, float, int, int, int, const Vector*, const Vector*, CUtlVector<Vector>*, bool, float, int);

extern EmitSound1_t g_pEmitSound1;
extern EmitSound2_t g_pEmitSound2;

int __fastcall hk_EmitSound1(void* ecx, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
	float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM, const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector<Vector>* pUtlVecOrigins = NULL,
	bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1, int unknown = 0);
int __fastcall hk_EmitSound2(void* ecx, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
	float flVolume, float flAttenuation, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM, const Vector *pOrigin = NULL, const Vector *pDirection = NULL, CUtlVector<Vector>* pUtlVecOrigins = NULL,
	bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1);

#endif // __EMITSOUND_H__