#pragma once
#include <Windows.h>

class VFTableHook
{
public:
	VFTableHook();
	VFTableHook(PDWORD pObj, bool bReplace);
	~VFTableHook();
	DWORD Hook(UINT index, PDWORD pFunc);
	void Restore();

private:
	DWORD m_dwLen;
	void CalculateLength();

	DWORD* m_pObj;
	
	DWORD* m_pOldVTable;
	DWORD* m_pNewVTable;
};

