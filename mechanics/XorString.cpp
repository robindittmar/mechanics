#include "XorString.h"

CXorString::CXorString(const char* p, unsigned long xor, bool xorInstantly)
{
	m_pBuffer = NULL;

	this->String(p);
	m_iXorKey = xor;

	if (xorInstantly)
	{
		this->Xor();
	}
}

CXorString::~CXorString()
{
	if (m_pBuffer)
		delete[] m_pBuffer;
}

void CXorString::String(const char* p)
{
	if (m_pBuffer)
		delete[] m_pBuffer;

	int iLen = strlen(p);
	m_pBuffer = new char[iLen + 1];
	memcpy(m_pBuffer, p, iLen + 1);
}

const char* CXorString::Xor()
{
	int iLenStr = strlen(m_pBuffer);
	int iLenKey = sizeof(m_iXorKey);

	for (int i = 0; i < iLenStr; i++)
	{
		m_pBuffer[i] = m_pBuffer[i] ^ (m_iXorKey >> (i % iLenKey));
	}

	return m_pBuffer;
}
