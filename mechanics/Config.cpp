#include "Config.h"
#include "Application.h"

CConfig::CConfig()
{
	m_pCurSection = NULL;
}

CConfig::~CConfig()
{
	if (m_pCurSection)
		delete[] m_pCurSection;

	this->DeleteValues();
}

void CConfig::Init(CApplication* pApp)
{
	m_pApp = pApp;
}

bool CConfig::LoadFile(const char* pFilename)
{
#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Loading config '%s'\n", pFilename);
#endif // _DEBUG

	this->DeleteValues();

	char pFullpath[MAX_PATH];
	sprintf(pFullpath, "%s%s%s", m_pApp->GetWorkingDirectory(), CONFIG_FOLDER, pFilename);

	FILE* pFile = fopen(pFullpath, "r");
	if (!pFile)
	{
#ifdef _DEBUG
		g_pConsole->Write(LOGLEVEL_ERROR, "Couldn't open file '%s'\n", pFullpath);
#endif // _DEBUG

		return false;
	}

	char pKeyBuffer[512];
	const char* pKey;
	const char* pValue;

	char pBuffer[1024];
	while (fgets(pBuffer, 1024, pFile))
	{
		if (pBuffer[0] == '\0' || pBuffer[0] == '\n' || pBuffer[0] == '#')
			continue;

		// Remove '\n'
		int iLen = strlen(pBuffer);
		for (int i = iLen - 1; i >= 0; i--)
		{
			if (pBuffer[i] == '\n')
			{
				pBuffer[i] = '\0';
				iLen = i;
				break;
			}
		}

		// New section
		if (pBuffer[0] == '[')
		{
			if (m_pCurSection)
				delete[] m_pCurSection;

			// -1 because of []
			m_pCurSection = new char[iLen - 1];
			memcpy(m_pCurSection, pBuffer + 1, iLen - 2);
			m_pCurSection[iLen - 2] = '\0';
			continue;
		}
		else
		{
			pKey = strtok(pBuffer, "=");
			pValue = strtok(NULL, "=");

			int iLenKey = sprintf(pKeyBuffer, "%s_%s", m_pCurSection, pKey);
			uint32_t iHash = murmurhash(pKeyBuffer, iLenKey, 0xB16B00B5);

			iLenKey++;
			char* pKeyCopy = new char[iLenKey];
			memcpy(pKeyCopy, pKeyBuffer, iLenKey);

			int iLenValue = strlen(pValue) + 1;
			char* pValueCopy = new char[iLenValue];
			memcpy(pValueCopy, pValue, iLenValue);

			m_mapKeys[iHash] = pKeyCopy;
			m_mapValues[iHash] = pValueCopy;

#ifdef _DEBUG
			g_pConsole->Write(LOGLEVEL_INFO, "%s\t=\t%s\n", pKeyBuffer, pValueCopy);
#endif // _DEBUG
		}
	}

	fclose(pFile);

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Done loading config\n");
#endif // _DEBUG
	return true;
}

bool CConfig::SaveFile(const char* pFilename)
{
	return false;
}

bool CConfig::GetBool(const char* pSection, const char* pKey, bool* pOut)
{
	uint32_t iHash = this->BuildSectionKeyHash(pSection, pKey);
	bool bValue = m_mapValues[iHash][0] == '1';

	if (pOut)
		*pOut = bValue;

	return bValue;
}

int CConfig::GetInt(const char* pSection, const char* pKey, int* pOut)
{
	uint32_t iHash = this->BuildSectionKeyHash(pSection, pKey);
	int iValue = atoi(m_mapValues[iHash]);

	if (pOut)
		*pOut = iValue;

	return iValue;
}

float CConfig::GetFloat(const char* pSection, const char* pKey, float* pOut)
{
	uint32_t iHash = this->BuildSectionKeyHash(pSection, pKey);
	float fValue = atof(m_mapValues[iHash]);

	if (pOut)
		*pOut = fValue;

	return fValue;
}

const char* CConfig::GetString(const char* pSection, const char* pKey, char* pOut, int iMaxLen)
{
	uint32_t iHash = this->BuildSectionKeyHash(pSection, pKey);
	const char* pValue = m_mapValues[iHash];

	if (pOut)
		strncpy(pOut, pValue, iMaxLen);

	return pValue;
}

uint32_t CConfig::BuildSectionKeyHash(const char* pSection, const char* pKey)
{
	char pSectionKey[512];
	sprintf(pSectionKey, "%s_%s", pSection, pKey);

	return murmurhash(pSectionKey, strlen(pSectionKey), 0xB16B00B5);
}

void CConfig::DeleteValues()
{
	const char* ptr;

	for (std::unordered_map<uint32_t, const char*>::iterator it = m_mapKeys.begin(); it != m_mapKeys.end(); it++)
	{
		ptr = it->second;
		
		if (ptr)
			delete[] ptr;
	}

	for (std::unordered_map<uint32_t, const char*>::iterator it = m_mapValues.begin(); it != m_mapValues.end(); it++)
	{
		ptr = it->second;

		if (ptr)
			delete[] ptr;
	}

	m_mapKeys.clear();
	m_mapValues.clear();
}
