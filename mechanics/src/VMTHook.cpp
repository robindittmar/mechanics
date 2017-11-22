#include "VMTHook.h"

VMTHook::VMTHook(DWORD* pObj)
	: m_pObj(pObj), m_bIsReplaced(false)
{
	m_pOldVTable = (DWORD*)*m_pObj;

	this->EvaluateVMT();
	if (m_iCountMethods > 0)
	{
		m_pNewVTable = new DWORD[m_iCountMethods];
		for (int i = 0; i < m_iCountMethods; i++)
		{
			m_pNewVTable[i] = m_pOldVTable[i];
		}

		this->Replace();
	}
}

VMTHook::~VMTHook()
{
	if (m_bIsReplaced)
		this->Restore();
}

DWORD VMTHook::Hook(UINT index, DWORD* pFunc)
{
	if (index > m_iCountMethods)
		return false;
	
#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Hooking method of [0x%08X], method index: %d\n", m_pObj, index);
#endif // _DEBUG

	m_pNewVTable[index] = (DWORD)pFunc;
	return m_pOldVTable[index];
}

void VMTHook::Replace()
{
#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Replacing VMT of [0x%08X] (0x%08X -> 0x%08X)\n", m_pObj, m_pOldVTable, m_pNewVTable);
#endif // _DEBUG

	*m_pObj = (DWORD)m_pNewVTable;
	m_bIsReplaced = true;
}

void VMTHook::Restore()
{
#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Restoring VMT of [0x%08X] (0x%08X -> 0x%08X)\n", m_pObj, m_pNewVTable, m_pOldVTable);
#endif // _DEBUG

	*m_pObj = (DWORD)m_pOldVTable;
	m_bIsReplaced = false;
}

void VMTHook::EvaluateVMT()
{
	if (!m_pOldVTable)
		return;

	for (m_iCountMethods = 0; m_pOldVTable[m_iCountMethods]; m_iCountMethods++);
}
