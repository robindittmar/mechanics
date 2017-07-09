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
	UINT CalculateLength();

	PDWORD m_pObj;

	PDWORD m_pOldVTable;
	PDWORD m_pNewVTable;
};

