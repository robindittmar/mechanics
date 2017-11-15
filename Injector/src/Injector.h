#ifndef __INJECTOR_H__
#define __INJECTOR_H__

#include <Windows.h>
#include <TlHelp32.h>
#include "Library.h"

class CInjector
{
public:
	CInjector();
	~CInjector();

	bool Inject(const char* pProcess, CLibrary* pLibrary);
	bool Detach(const char* pProcess, CLibrary* pLibrary);
private:
	DWORD GetProcessId(const char* pProcess);
	DWORD WaitForProcess(const char* pProcess);
};

#endif // __INJECTOR_H__