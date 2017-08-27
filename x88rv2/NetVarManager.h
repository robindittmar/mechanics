#ifndef __NETVARMANAGER_H__
#define __NETVARMANAGER_H__

// Std lib
#include <cstdarg>

// Source SDK
#include "ClientClass.h"

// Custom
#include "NetVar.h"

class CNetVarManager
{
public:
	CNetVarManager();
	~CNetVarManager();

	void AddTable(const char* pTable);
	void LoadTables(ClientClass* pClass);

	int GetOffset(int iCountToResolve, const char* pTable, ...);

	static void DumpAll(FILE* pFile, ClientClass* pClass);
	static void DumpTable(FILE* pFile, ClientClass* pClass, const char* pTableName);
private:
	static void DumpTable(FILE* pFile, RecvTable* pTable, int iLevel = 0);

	std::unordered_map<uint32_t, bool> m_mapTablesToLoad;
	std::unordered_map<uint32_t, CNetVar*> m_mapNetVars;
};

#endif // __NETVARMANAGER_H__