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
	CApplication app;
	app.Run();
	

	/*engineClient = GetInterface<IVEngineClient>(engineInterfaceFn, "VEngineClient014");
	IBaseClientDLL* client = GetInterface<IBaseClientDLL>(clientInterfaceFn, "VClient018");
	entityList = GetInterface<IClientEntityList>(clientInterfaceFn, "VClientEntityList003");

	ClientClass *cClass = client->GetAllClasses();


	std::vector<int> NetvarDatabaseOffsets;
	std::vector<std::string> NetvarDatabaseStrings;
	InitializeNetvarDb(&NetvarDatabaseOffsets, &NetvarDatabaseStrings, cClass);

	int* localEntity = (int*)entityList->GetClientEntity(engineClient->GetLocalPlayer());

	IDirect3DDevice9* dwDevice = (IDirect3DDevice9*)**(DWORD**)(((DWORD)Memory::FindPattern((BYTE*)(GetModuleHandle("shaderapidx9.dll")), 0xC1000, (BYTE*)"\xA1\x00\x00\x00\x00\x6A\x00\x6A\x00\x6A\x00\x8B\x08\x6A\x00\x50\xFF\x51\x44", "a----abcdefghasdfaa")) + 1);

	VFTableHook d3dHook((DWORD*)dwDevice, true);

	p_EndScene = (EndScene_t)d3dHook.Hook(42, (PDWORD)hk_EndScene);

	while (1) {

	}*/

}