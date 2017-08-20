#include "Pattern.h"

bool CPattern::CompareData(BYTE* pAddr, BYTE* pPattern, const char* pMask)
{
	int iLen = strlen(pMask);
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
	BYTE* pCurAddr = pStartAddr;
	BYTE* iEndAddr = (pStartAddr + iLength - strlen(pMask));

	while (pCurAddr < iEndAddr)
	{
		if (CompareData(pCurAddr, pPattern, pMask))
		{
			return (DWORD)pCurAddr;
		}

		pCurAddr++;
	}

	return 0;
}