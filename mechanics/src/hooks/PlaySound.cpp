#include "PlaySound.h"
#include "../Application.h"

PlaySound_t g_pPlaySound;

void __fastcall hk_PlaySound(void* ecx, void* edx, const char* fileName)
{
	CApplication* pApp = CApplication::Instance();
	pApp->Misc()->AutoAccept(fileName);

	g_pPlaySound(ecx, fileName);
}
