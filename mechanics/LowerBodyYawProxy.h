#ifndef __LOWERBODYYAWPROXY_H__
#define __LOWERBODYYAWPROXY_H__

#include "ClientClass.h"
#include "NetVar.h"

extern RecvVarProxy_t g_pLowerBodyYawProxy;

void __cdecl hk_SetLowerBodyYawTarget(const CRecvProxyData* ecx, void* pStruct, void* pOut);

#endif // __LOWERBODYYAWPROXY_H__