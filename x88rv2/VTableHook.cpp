#include "VTableHook.h"

VTableHook::VTableHook(DWORD* pObj, bool bReplace)
{
	m_pObj = pObj;
	m_pOldVTable = (DWORD*)*m_pObj;

	CalculateLength();

	if (m_dwLen) {
		m_pNewVTable = new DWORD[m_dwLen];

		for (DWORD i = 0; i < m_dwLen; i++) {
			m_pNewVTable[i] = m_pOldVTable[i];
		}

		*m_pObj = (DWORD)m_pNewVTable;
	}
}


VTableHook::~VTableHook()
{
}

DWORD VTableHook::Hook(UINT index, DWORD* pFunc)
{
	if (index > m_dwLen)
		return false;
	
	m_pNewVTable[index] = (DWORD)pFunc;

	return m_pOldVTable[index];
}

void VTableHook::Rehook()
{
	*m_pObj = (DWORD)m_pNewVTable;
}

void VTableHook::Restore()
{
	*m_pObj = (DWORD)m_pOldVTable;
}

void VTableHook::CalculateLength()
{
	if (!m_pOldVTable) return;
	for (m_dwLen = 0; m_pOldVTable[m_dwLen]; m_dwLen++);
}
