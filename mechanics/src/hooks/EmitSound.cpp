#include "EmitSound.h"
#include "../Application.h"

EmitSound1_t g_pEmitSound1;
EmitSound2_t g_pEmitSound2;

int __fastcall hk_EmitSound1(void* ecx, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
	float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, CUtlVector<Vector>* pUtlVecOrigins,
	bool bUpdatePositions, float soundtime, int speakerentity, int unknown)
{
	CApplication* pApp = CApplication::Instance();

	if (pOrigin)
		pApp->SoundEsp()->AddSound(new CSoundInfo(iEntIndex, *pOrigin, pSample));

	return g_pEmitSound1(ecx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, iSoundlevel, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, 0);
}

int __fastcall hk_EmitSound2(void* ecx, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
	float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, CUtlVector<Vector>* pUtlVecOrigins,
	bool bUpdatePositions, float soundtime, int speakerentity)
{
	CApplication* pApp = CApplication::Instance();

	if (pOrigin)
		pApp->SoundEsp()->AddSound(new CSoundInfo(iEntIndex, *pOrigin, pSample));

	return g_pEmitSound2(ecx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity);
}