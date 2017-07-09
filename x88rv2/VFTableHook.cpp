#include "VFTableHook.h"



VFTableHook::VFTableHook()
{
}

VFTableHook::VFTableHook(PDWORD pObj, bool bReplace)
{
	m_pObj = pObj;
	m_pOldVTable = (DWORD*)*pObj;

	DWORD dwLen = CalculateLength();

	if (dwLen) {
		m_pNewVTable = new DWORD[dwLen];

		for (auto i = 0; i < dwLen; i++) {
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
	DWORD dwLen = CalculateLength();
	if (index > dwLen) {
		return false;
	}
	m_pNewVTable[index] = (DWORD)pFunc;

	return m_pOldVTable[index];
}

void VFTableHook::Restore()
{
}

UINT VFTableHook::CalculateLength()
{
	int dwIndex;
	if (!m_pOldVTable) return 0;

	for (dwIndex = 0; m_pOldVTable[dwIndex]; dwIndex++) {
		if (m_pOldVTable[dwIndex] == 1) break;
		if (IsBadCodePtr((FARPROC)m_pOldVTable[dwIndex])) {
			break;
		}
	}
	return dwIndex;
}
