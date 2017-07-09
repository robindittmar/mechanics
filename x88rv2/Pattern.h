#ifndef __PATTERN_H__
#define __PATTERN_H__

#include <Windows.h>

class CPattern
{
public:
	static bool CompareData(BYTE* pAddr, BYTE* pPattern, const char* pMask);
	static DWORD* FindPattern(BYTE* pStartAddr, int iLength, BYTE* pPattern, const char* pMask);
private:
};

#endif // __PATTERN_H__