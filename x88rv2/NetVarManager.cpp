#include "NetVarManager.h"

CNetVarManager::CNetVarManager()
{
	m_bSummarizeOffsets = false;
}

CNetVarManager::~CNetVarManager()
{
	CNetVar* pCurrent;
	for (std::unordered_map<uint32_t, CNetVar*>::iterator it = m_mapNetVars.begin(); it != m_mapNetVars.end(); it++)
	{
		pCurrent = it->second;

		if (pCurrent)
			delete pCurrent;
	}
}

void CNetVarManager::AddTable(const char* pTable)
{
	uint32_t iHash = murmurhash(pTable, strlen(pTable), 0xB16B00B5);
	m_mapTablesToLoad[iHash] = true;
}

void CNetVarManager::LoadTables(ClientClass* pClass, bool bRecursive)
{
	uint32_t iHash;
	RecvTable* pTable;
	CNetVar* pNetVar;

	// Iterate over list
	while(pClass)
	{
		// Grab current table
		pTable = pClass->m_pRecvTable;

		// Build hash from name & 'load' if necessary
		iHash = murmurhash(pTable->m_pNetTableName, strlen(pTable->m_pNetTableName), 0xB16B00B5);
		if(m_mapTablesToLoad[iHash])
		{
			// Init and load netvar
			pNetVar = new CNetVar();
			pNetVar->LoadTable(pTable, bRecursive);

			// Add netvar to map
			m_mapNetVars[iHash] = pNetVar;
		}

		// Next element
		pClass = pClass->m_pNext;
	}
}

CNetVar* CNetVarManager::GetNetVar(const char* pTable, const char* pNetVarName)
{
	if (!pNetVarName)
	{
		return m_mapNetVars[murmurhash(pTable, strlen(pTable), 0xB16B00B5)];
	}

	return this->GetNetVar(1, pTable, pNetVarName);
}

CNetVar* CNetVarManager::GetNetVar(int iCountToResolve, const char* pTable, ...)
{
	uint32_t iHash = murmurhash(pTable, strlen(pTable), 0xB16B00B5);
	CNetVar* pNetVar = m_mapNetVars[iHash];

	const char* pCurArg;
	va_list pArgList;
	va_start(pArgList, pTable);
	for (int i = 0; i < iCountToResolve; i++)
	{
		if (!pNetVar->GetIsTable())
		{
			break;
		}

		pCurArg = va_arg(pArgList, const char*);
		pNetVar = pNetVar->GetChild(pCurArg);
		if (!pNetVar)
			return NULL;
	}

	va_end(pArgList);
	return pNetVar;
}

int CNetVarManager::GetOffset(const char* pTable, const char* pNetVarName)
{
	return this->GetOffset(1, pTable, pNetVarName);
}

int CNetVarManager::GetOffset(int iCountToResolve, const char* pTable, ...)
{
	int iOffset = 0;
	uint32_t iHash = murmurhash(pTable, strlen(pTable), 0xB16B00B5);
	CNetVar* pNetVar = m_mapNetVars[iHash];

	const char* pCurArg;
	va_list pArgList;
	va_start(pArgList, pTable);
	for(int i = 0; i < iCountToResolve; i++)
	{
		if (!pNetVar->GetIsTable())
		{
			iOffset = -1;
			break;
		}

		pCurArg = va_arg(pArgList, const char*);
		pNetVar = pNetVar->GetChild(pCurArg);
		if (!pNetVar)
		{
			iOffset = -1;
			break;
		}

		if (m_bSummarizeOffsets)
			iOffset += pNetVar->GetOffset();
		else
			iOffset = pNetVar->GetOffset();
	}

	va_end(pArgList);
	return iOffset;
}

void CNetVarManager::DumpAll(FILE* pFile, ClientClass* pClass)
{
	while(pClass)
	{
		DumpTable(pFile, pClass->m_pRecvTable);
		pClass = pClass->m_pNext;
	}
}

void CNetVarManager::DumpTable(FILE* pFile, ClientClass* pClass, const char* pTableName)
{
	RecvTable* pTable;
	while(pClass)
	{
		pTable = pClass->m_pRecvTable;

		if(!strcmp(pTable->m_pNetTableName, pTableName))
		{
			DumpTable(pFile, pTable);
		}

		pClass = pClass->m_pNext;
	}
}

void printTabs(FILE* pFile, int iCount)
{
	for(int i = 0; i < iCount; i++)
	{
		fputs("  ", pFile);
	}
}

void CNetVarManager::DumpTable(FILE* pFile, RecvTable* pTable, int iLevel)
{
	RecvProp* pProp;
	fprintf(pFile, "%s\n", pTable->m_pNetTableName);

	for (int i = 0; i < pTable->m_nProps; i++)
	{
		pProp = pTable->m_pProps + i;

		if (pProp->m_RecvType == SendPropType::DPT_DataTable)
		{
			printTabs(pFile, iLevel + 1);
			fprintf(pFile, "[%s (0x%X)] ", pProp->m_pVarName, pProp->m_Offset);

			DumpTable(pFile, pProp->m_pDataTable, iLevel + 1);
		}
		else
		{
			printTabs(pFile, iLevel + 1);
			fprintf(pFile, "%s (0x%X)\n", pProp->m_pVarName, pProp->m_Offset);
		}
	}
}
