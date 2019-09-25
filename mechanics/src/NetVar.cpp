#include "NetVar.h"

CNetVar::CNetVar(int iOffset, void** pProxyFn, bool bIsTable)
{
	m_iOffset = iOffset;
	m_pProxyFn = pProxyFn;
	m_bIsTable = bIsTable;
}

CNetVar::~CNetVar()
{
	CNetVar* pCurrent;
	for(std::map<uint32_t, CNetVar*>::iterator it = m_mapChilds.begin(); it != m_mapChilds.end(); it++)
	{
		pCurrent = it->second;

		if (pCurrent)
			delete pCurrent;
	}
}

void CNetVar::LoadTable(RecvTable* pTable, bool bRecursive)
{
	if (pTable == NULL)
		return;

	m_bIsTable = true;
	this->pName = pTable->m_pNetTableName;

	uint32_t iHash;
	int iCountProperties = pTable->m_nProps;
	RecvProp* pProp;

	for(int i = 0; i < iCountProperties; i++)
	{
		pProp = pTable->m_pProps + i;

		CNetVar* pNetVar = new CNetVar(pProp->m_Offset, &pProp->m_ProxyFn);
		if(pProp->m_RecvType == SendPropType::DPT_DataTable)
		{
			iHash = murmurhash(pProp->m_pDataTable->m_pNetTableName, strlen(pProp->m_pDataTable->m_pNetTableName), 0xB16B00B5);

			if(bRecursive)
				pNetVar->LoadTable(pProp->m_pDataTable, bRecursive);
		}
		else
		{
			iHash = murmurhash(pProp->m_pVarName, strlen(pProp->m_pVarName), 0xB16B00B5);
			pNetVar->pName = pProp->m_pVarName;
		}

		m_mapChilds[iHash] = pNetVar;
	}
}

CNetVar* CNetVar::GetChild(const char* pNetVarName)
{
	uint32_t iHash = murmurhash(pNetVarName, strlen(pNetVarName), 0xB16B00B5);
	return m_mapChilds[iHash];
}

RecvVarProxy_t CNetVar::HookProxy(RecvVarProxy_t pHookFunc)
{
	m_pOrigProxyFn = (RecvVarProxy_t)*m_pProxyFn;
	*m_pProxyFn = (void*)pHookFunc;
	m_bProxyHooked = true;

	return m_pOrigProxyFn;
}

void CNetVar::UnhookProxy()
{
	if (m_bProxyHooked)
	{
		*m_pProxyFn = (void*)m_pOrigProxyFn;
		m_bProxyHooked = false;
	}
}