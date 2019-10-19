#ifndef __NETVARMANAGER_H__
#define __NETVARMANAGER_H__

// Std lib
#include <cstdarg>
#include <map>

// Source SDK
#include "../source_sdk/ClientClass.h"

// Custom
#include "../utils/Console.h"
#include "NetVar.h"

class CNetVarManager
{
public:
	CNetVarManager();
	~CNetVarManager();

	void AddTable(const char* pTable);
	void LoadTables(ClientClass* pClass, bool bRecursive = false);
	
	CNetVar* GetNetVar(const char* pTable, const char* pNetVarName = NULL);
	CNetVar* GetNetVar(int iCountToResolve, const char* pTable, ...);

	int GetOffset(const char* pTable, const char* pNetVarName);
	int GetOffset(int iCountToResolve, const char* pTable, ...);

	void SetSummarizeOffsets(bool bValue) { m_bSummarizeOffsets = bValue; }
	bool GetSummarizeOffsets() { return m_bSummarizeOffsets; }

	static void DumpAll(FILE* pFile, ClientClass* pClass);
	static void DumpTable(FILE* pFile, ClientClass* pClass, const char* pTableName);
	static void DumpClientClasses(FILE* pFile, ClientClass* pClass);
private:
	static void DumpTable(FILE* pFile, RecvTable* pTable, int iLevel = 0);

	// Describes if we add all offsets of sub-tables
	// or only return the last one (default = only last one)
	bool m_bSummarizeOffsets;
	std::map<uint32_t, bool> m_mapTablesToLoad;
	std::map<uint32_t, CNetVar*> m_mapNetVars;
};

#endif // __NETVARMANAGER_H__