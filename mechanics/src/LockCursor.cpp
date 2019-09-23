#include "LockCursor.h"
#include "Application.h"

LockCursor_t g_pLockCursor;

void __fastcall hk_LockCursor(void* ecx, void* edx)
{
	CApplication* pApp = CApplication::Instance();

	if (pApp->Gui()->GetEnableGameInput())
	{
		g_pLockCursor(ecx);
	}
	else
	{
		pApp->Surface()->UnlockCursor();
	}
}
