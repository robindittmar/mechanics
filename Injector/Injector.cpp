#include "Injector.h"

CInjector::CInjector()
{
}

CInjector::~CInjector()
{
}

bool CInjector::Inject(const char* pProcess, CLibrary* pLibrary)
{
	// Wait for process to spawn
	DWORD dwProcessId = this->WaitForProcess(pProcess);

	// Get address of LoadLibrary
	void* pLoadLibrary = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	if (!pLoadLibrary)
	{
		return false;
	}
	
	// Open target process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (!hProcess)
	{
		return false;
	}

	// Allocate memory in target process
	int iLenDll = strlen(pLibrary->Filepath());
	void* pMemory = VirtualAllocEx(hProcess, NULL, iLenDll + 1, MEM_COMMIT, PAGE_READWRITE);
	if (!pMemory)
	{
		return false;
	}

	// Write to memory
	SIZE_T iBytesWritten;
	if (!WriteProcessMemory(hProcess, pMemory, pLibrary->Filepath(), iLenDll + 1, &iBytesWritten))
	{
		return false;
	}
	
	// Written wrong amount of memory?
	if (iBytesWritten != iLenDll + 1)
	{
		return false;
	}

	// Inject the dope (#420) ;)
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)pLoadLibrary, pMemory, NULL, NULL);
	if (!hThread)
	{
		return false;
	}

	// Wait for DLL to finish DllMain(...)
	WaitForSingleObject(hThread, INFINITE);

	// Close process handle
	CloseHandle(hProcess);
	return true;
}

bool CInjector::Detach(const char* pProcess, CLibrary* pLibrary)
{
	return true;
}

DWORD CInjector::GetProcessId(const char* pProcess)
{
	PROCESSENTRY32 pe;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(hSnapshot, &pe))
	{
		do
		{
			if (!strcmp(pe.szExeFile, pProcess))
			{
				CloseHandle(hSnapshot);
				return pe.th32ProcessID;
			}
		} while (Process32Next(hSnapshot, &pe));
	}

	CloseHandle(hSnapshot);
	return 0;
}

DWORD CInjector::WaitForProcess(const char* pProcess)
{
	DWORD dwProcessId = GetProcessId(pProcess);
	while (!dwProcessId)
	{
		// TODO: Sleep time ok?
		Sleep(5000);
		dwProcessId = GetProcessId(pProcess);
	}

	return dwProcessId;
}