#include "Application.h"

DWORD ThreadProc(void*);

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadProc, (void*)hinstDLL, NULL, NULL);
	}
	return true;
}

DWORD ThreadProc(void* pParam)
{
	CApplication* pApp = CApplication::Instance();
	pApp->Run((HMODULE)pParam);

	return 0;
}