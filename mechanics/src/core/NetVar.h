#ifndef __NETVAR_H__
#define __NETVAR_H__

// Std lib
#include <map>

// Source Sdk
#include "../source_sdk/ClientClass.h"

// Custom
#include "../third_party/murmurhash/murmurhash.h"

typedef void(__cdecl *RecvVarProxy_t)(const CRecvProxyData*, void*, void*);

class CNetVar
{
public:
	CNetVar(int iOffset = 0, void** pProxyFn = NULL, bool bIsTable = false);
	~CNetVar();

	void LoadTable(RecvTable* pTable, bool bRecursive = false);
	CNetVar* GetChild(const char* pNetVarName);

	// Returns ptr to original proxy func
	RecvVarProxy_t HookProxy(RecvVarProxy_t pHookFunc);
	void UnhookProxy();

	void SetIsTable(bool bIsTable) { m_bIsTable = bIsTable; }
	bool GetIsTable() { return m_bIsTable; }

	void SetOffset(int iOffset) { m_iOffset = iOffset; }
	int GetOffset() { return m_iOffset; }
private:
	const char* pName;
	int m_iOffset;

	bool m_bProxyHooked;
	void** m_pProxyFn;
	RecvVarProxy_t m_pOrigProxyFn;

	bool m_bIsTable;
	std::map<uint32_t, CNetVar*> m_mapChilds;
};

#endif // __NETVAR_H__