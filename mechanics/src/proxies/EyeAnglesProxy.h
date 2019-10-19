#ifndef __EYEANGLESPROXY_H__
#define __EYEANGLESPROXY_H__

#include "../source_sdk/ClientClass.h"
#include "../core/NetVar.h"

extern RecvVarProxy_t g_pEyeAnglesProxy0;
extern RecvVarProxy_t g_pEyeAnglesProxy1;

void __cdecl hk_SetEyeAnglesProxy0(const CRecvProxyData* pDataConst, void* pStruct, void* pOut);
void __cdecl hk_SetEyeAnglesProxy1(const CRecvProxyData* pDataConst, void* pStruct, void* pOut);

#endif // __EYEANGLESPROXY_H__