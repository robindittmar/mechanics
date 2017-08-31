#pragma once
#include <Windows.h>

class VTableHook
{
public:
	VTableHook(DWORD* pObj, bool bReplace);
	~VTableHook();

	DWORD Hook(UINT index, DWORD* pFunc);

	void Rehook();
	void Restore();
private:
	DWORD m_dwLen;
	void CalculateLength();

	DWORD* m_pObj;
	
	DWORD* m_pOldVTable;
	DWORD* m_pNewVTable;
};

