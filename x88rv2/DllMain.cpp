#include "Application.h"

void ThreadProc(void);

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadProc, NULL, NULL, NULL);
	}
	return true;
}

void ThreadProc()
{
	CApplication* pApp = CApplication::Instance();
	pApp->Run();
}