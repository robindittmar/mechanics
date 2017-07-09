#ifndef __CREATEINTERFACE_H__
#define __CREATEINTERFACE_H__

#include <Windows.h>

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
CreateInterfaceFn GetCreateInterfaceFn(const char* pDllName);

#endif // __CREATEINTERFACE_H__