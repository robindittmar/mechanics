#include "CreateInterface.h"

CreateInterfaceFn GetCreateInterfaceFn(const char *pDllName)
{
	return (CreateInterfaceFn)GetProcAddress(GetModuleHandle(pDllName), "CreateInterface");
}
