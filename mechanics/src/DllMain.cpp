#include "Application.h"

DWORD ThreadProc(void*);

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)ThreadProc, (void*)hinstDLL, 0, 0);
	}
	return true;
}

DWORD ThreadProc(void* pParam)
{
	CApplication* pApp = CApplication::Instance();
	pApp->Run((HMODULE)pParam);

	return 0;
}