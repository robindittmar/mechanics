#ifndef __NETVAR_H__
#define __NETVAR_H__

// Std lib
#include <unordered_map>

// Source Sdk
#include "ClientClass.h"

// Custom
#include "murmurhash.h"

class CNetVar
{
public:
	CNetVar(int iOffset = 0, bool bIsTable = false);
	~CNetVar();

	void LoadTable(RecvTable* pTable, bool bRecursive = false);
	CNetVar* GetChild(const char* pNetVarName);

	void SetIsTable(bool bIsTable) { m_bIsTable = bIsTable; }
	bool GetIsTable() { return m_bIsTable; }

	void SetOffset(int iOffset) { m_iOffset = iOffset; }
	int GetOffset() { return m_iOffset; }
private:
	const char* pName;
	int m_iOffset;

	bool m_bIsTable;
	std::unordered_map<uint32_t, CNetVar*> m_mapChilds;
};

#endif // __NETVAR_H__