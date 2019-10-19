#ifndef __VMTHOOK_H__
#define __VMTHOOK_H__

// Std Lib
#include <Windows.h>

// Source SDK

// Custom
#include "Console.h"

class VMTHook
{
public:
	VMTHook(DWORD* pObj);
	~VMTHook();

	/// <summary>
	/// Hook specific method of VMT
	/// </summary>
	/// <param name="index">The index of the method to hook</param>
	/// <param name="pFunc">The address of the new function</param>
	/// <returns>The address of the original method</returns>
	DWORD Hook(UINT index, DWORD* pFunc);

	/// <summary>
	/// Replaces the original VMT with our copy
	/// (which may be modified)
	/// </summary>
	void Replace();

	/// <summary>
	/// Restore the original VMT
	/// </summary>
	void Restore();
private:
	int m_iCountMethods;
	void EvaluateVMT();

	bool m_bIsReplaced;
	DWORD* m_pObj;
	
	DWORD* m_pOldVTable;
	DWORD* m_pNewVTable;
};

#endif // __VMTHOOK_H__