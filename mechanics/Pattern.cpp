#include "Pattern.h"

bool CPattern::CompareData(BYTE* pAddr, BYTE* pPattern, const char* pMask, int iLen)
{
	for (int i = 0; i < iLen; i++)
	{
		if (pMask[i] == '-')
		{
			continue;
		}

		if (pAddr[i] != pPattern[i])
		{
			return false;
		}
	}

	return true;
}

DWORD CPattern::FindPattern(BYTE* pStartAddr, int iLength, BYTE* pPattern, const char* pMask)
{
	int iLen = strlen(pMask);

	BYTE* pCurAddr = pStartAddr;
	BYTE* iEndAddr = (pStartAddr + iLength - iLen);

	while (pCurAddr < iEndAddr)
	{
		if (CompareData(pCurAddr, pPattern, pMask, iLen))
		{
			return (DWORD)pCurAddr;
		}

		pCurAddr++;
	}

	return 0;
}