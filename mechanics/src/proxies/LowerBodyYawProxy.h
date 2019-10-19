#ifndef __LOWERBODYYAWPROXY_H__
#define __LOWERBODYYAWPROXY_H__

#include "../source_sdk/ClientClass.h"
#include "../core/NetVar.h"

extern RecvVarProxy_t g_pLowerBodyYawProxy;

void __cdecl hk_SetLowerBodyYawTarget(const CRecvProxyData* pDataConst, void* pStruct, void* pOut);

#endif // __LOWERBODYYAWPROXY_H__