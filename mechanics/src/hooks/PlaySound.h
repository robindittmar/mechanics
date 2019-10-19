#ifndef __PLAYSOUND_H__
#define __PLAYSOUND_H__

struct PlaySoundParam
{
	const char* pFilename;
};

typedef void(__thiscall *PlaySound_t)(void*, const char*);
extern PlaySound_t g_pPlaySound;

void __fastcall hk_PlaySound(void* ecx, void* edx, const char* fileName);

#endif // __PLAYSOUND_H__