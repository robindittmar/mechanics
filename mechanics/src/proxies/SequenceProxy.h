#ifndef __SEQUENCEPROXY_H__
#define __SEQUENCEPROXY_H__

#include "../source_sdk/ClientClass.h"
#include "../core/NetVar.h"

extern RecvVarProxy_t g_pSequenceProxy;

void __cdecl hk_SetViewModelSequence(const CRecvProxyData* ecx, void* pStruct, void* pOut);

#endif // __SEQUENCEPROXY_H__