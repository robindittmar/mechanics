#include "VFTableHook.h"



VFTableHook::VFTableHook()
{
}

VFTableHook::VFTableHook(PDWORD pObj, bool bReplace)
{
	m_pObj = pObj;
	m_pOldVTable = (DWORD*)*pObj;

	CalculateLength();

	if (m_dwLen) {
		m_pNewVTable = new DWORD[m_dwLen];

		for (DWORD i = 0; i < m_dwLen; i++) {
			m_pNewVTable[i] = m_pOldVTable[i];
		}

		*pObj = (DWORD)m_pNewVTable;
	}
}


VFTableHook::~VFTableHook()
{
}

DWORD VFTableHook::Hook(UINT index, PDWORD pFunc)
{
	if (index > m_dwLen) {
		return false;
	}
	m_pNewVTable[index] = (DWORD)pFunc;

	return m_pOldVTable[index];
}

void VFTableHook::Restore()
{
}

void VFTableHook::CalculateLength()
{
	if (!m_pOldVTable) return;
	for (m_dwLen = 0; m_pOldVTable[m_dwLen]; m_dwLen++);
}
