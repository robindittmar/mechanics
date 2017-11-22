#include "Config.h"
#include "Application.h"

CConfig::CConfig()
	: m_pApp(nullptr), m_pCurSection(nullptr)
{
}

CConfig::~CConfig()
{
	if (m_pCurSection)
		delete[] m_pCurSection;

	this->DeleteSectionList();
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
		g_pConsole->Write(LOGLEVEL_ERROR, "Couldn't open file '%s'\n\n", pFullpath);
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

			// Another copy for our set
			char* pSection = new char[iLen - 1];
			memcpy(pSection, m_pCurSection, iLen - 1);
			m_setSections.emplace(pSection);

			continue;
		}
		else
		{
			pKey = strtok(pBuffer, "=");
			pValue = strtok(NULL, "=");

			if (!pValue)
				pValue = "";

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
			g_pConsole->Write(LOGLEVEL_INFO, "%-32s = %16s\n", pKeyBuffer, pValueCopy);
#endif // _DEBUG
		}
	}

	fclose(pFile);

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Done loading config\n\n");
#endif // _DEBUG
	return true;
}

bool CConfig::SaveFile(const char* pFilename)
{
#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Saving config '%s'\n", pFilename);
#endif // _DEBUG

	uint32_t iHash;
	const char* pKey;
	std::unordered_map<uint32_t, CWritableSection> m_mapSections;
	char pBuffer[512];
	char pLine[512];

	for (std::unordered_map<uint32_t, const char*>::iterator it = m_mapKeys.begin(); it != m_mapKeys.end(); it++)
	{
		iHash = it->first;

		int iLen = strlen(it->second) + 1;
		memcpy(pBuffer, it->second, iLen);

		char* pSection = strtok(pBuffer, "_");
		char* pKey = strtok(NULL, "\0");
		sprintf(pLine, "%s=%s", pKey, m_mapValues[iHash]);

		uint32_t iSectionHash = murmurhash(pSection, strlen(pSection), 0xB16B00B5);
		if (m_mapSections.find(iSectionHash) == m_mapSections.end())
		{
			m_mapSections[iSectionHash] = CWritableSection(pSection);
		}

		m_mapSections[iSectionHash].AddLine(pLine);
	}

	char pFullpath[MAX_PATH];
	sprintf(pFullpath, "%s%s%s", m_pApp->GetWorkingDirectory(), CONFIG_FOLDER, pFilename);

	FILE* pFile = fopen(pFullpath, "w");
	if (!pFile)
	{
#ifdef _DEBUG
		g_pConsole->Write(LOGLEVEL_ERROR, "Couldn't open file '%s'\n\n", pFullpath);
#endif // _DEBUG

		return false;
	}

	for (std::unordered_map<uint32_t, CWritableSection>::iterator it = m_mapSections.begin(); it != m_mapSections.end(); it++)
	{
		it->second.WriteToFile(pFile);
	}

	fclose(pFile);

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Done saving config\n\n");
#endif // _DEBUG
	return false;
}

const char* CConfig::GetFirstSection()
{
	m_itSection = m_setSections.begin();
	if (m_itSection != m_setSections.end())
		return *m_itSection;
	
	return nullptr;
}

const char* CConfig::GetNextSection()
{
	m_itSection++;
	if (m_itSection != m_setSections.end())
		return *m_itSection;

	return nullptr;
}

void CConfig::DeleteSectionList()
{
	for (std::unordered_set<const char*>::iterator it = m_setSections.begin(); it != m_setSections.end(); it++)
	{
		delete[] *it;
	}
}

bool CConfig::GetBool(const char* pSection, const char* pKey, bool* pOut)
{
	uint32_t iHash = this->BuildSectionKeyHash(pSection, pKey);
	std::unordered_map<uint32_t, const char*>::iterator it = m_mapValues.find(iHash);
	if (it == m_mapValues.end())
		return false;

	bool bValue = it->second[0] == '1';
	if (pOut)
		*pOut = bValue;

	return bValue;
}

int CConfig::GetInt(const char* pSection, const char* pKey, int* pOut)
{
	uint32_t iHash = this->BuildSectionKeyHash(pSection, pKey);
	std::unordered_map<uint32_t, const char*>::iterator it = m_mapValues.find(iHash);
	if (it == m_mapValues.end())
		return 0;

	int iValue = atoi(it->second);
	if (pOut)
		*pOut = iValue;

	return iValue;
}

float CConfig::GetFloat(const char* pSection, const char* pKey, float* pOut)
{
	uint32_t iHash = this->BuildSectionKeyHash(pSection, pKey);
	std::unordered_map<uint32_t, const char*>::iterator it = m_mapValues.find(iHash);
	if (it == m_mapValues.end())
		return 0.0f;

	float fValue = atof(it->second);
	if (pOut)
		*pOut = fValue;

	return fValue;
}

Color CConfig::GetColor(const char* pSection, const char* pKey, Color* pOut)
{
	Color cValue;
	cValue.SetRawColor(this->GetInt(pSection, pKey));

	if (pOut)
		*pOut = cValue;

	return cValue;
}

const char* CConfig::GetString(const char* pSection, const char* pKey, char* pOut, int iMaxLen)
{
	uint32_t iHash = this->BuildSectionKeyHash(pSection, pKey);
	std::unordered_map<uint32_t, const char*>::iterator it = m_mapValues.find(iHash);
	if (it == m_mapValues.end())
		return NULL;

	const char* pValue = it->second;
	if (pOut)
		strncpy(pOut, pValue, iMaxLen);

	return pValue;
}

void CConfig::SetBool(const char* pSection, const char* pKey, bool bValue)
{
	uint32_t iHash = this->BuildAndStoreSectionKeyHash(pSection, pKey);

	char* pValue = new char[2];
	pValue[0] = bValue ? '1' : '0';
	pValue[1] = '\0';

	m_mapValues[iHash] = pValue;
}

void CConfig::SetInt(const char* pSection, const char* pKey, int iValue)
{
	uint32_t iHash = this->BuildAndStoreSectionKeyHash(pSection, pKey);
	
	char pValueBuffer[64];
	int iLen = sprintf(pValueBuffer, "%d", iValue) + 1;
	
	char* pValue = new char[iLen];
	memcpy(pValue, pValueBuffer, iLen);

	m_mapValues[iHash] = pValue;
}

void CConfig::SetFloat(const char* pSection, const char* pKey, float fValue)
{
	uint32_t iHash = this->BuildAndStoreSectionKeyHash(pSection, pKey);

	char pValueBuffer[64];
	int iLen = sprintf(pValueBuffer, "%f", fValue) + 1;

	char* pValue = new char[iLen];
	memcpy(pValue, pValueBuffer, iLen);

	m_mapValues[iHash] = pValue;
}

void CConfig::SetColor(const char* pSection, const char* pKey, Color cValue)
{
	this->SetInt(pSection, pKey, cValue.GetRawColor());
}

void CConfig::SetString(const char* pSection, const char* pKey, const char* pValue)
{
	if (!pValue)
		return;

	uint32_t iHash = this->BuildAndStoreSectionKeyHash(pSection, pKey);

	int iLen = strlen(pValue) + 1;
	char* pValueFinal = new char[iLen];
	memcpy(pValueFinal, pValue, iLen);

	m_mapValues[iHash] = pValueFinal;
}

uint32_t CConfig::BuildSectionKeyHash(const char* pSection, const char* pKey)
{
	char pSectionKey[512];
	sprintf(pSectionKey, "%s_%s", pSection, pKey);

	return murmurhash(pSectionKey, strlen(pSectionKey), 0xB16B00B5);
}

uint32_t CConfig::BuildAndStoreSectionKeyHash(const char* pSection, const char* pKey)
{
	char pSectionKey[512];
	int iLen = sprintf(pSectionKey, "%s_%s", pSection, pKey) + 1;

	uint32_t iHash = murmurhash(pSectionKey, strlen(pSectionKey), 0xB16B00B5);
	if (m_mapKeys.find(iHash) == m_mapKeys.end())
	{
		char* pSectionKeyFinal = new char[iLen];
		memcpy(pSectionKeyFinal, pSectionKey, iLen);

		m_mapKeys[iHash] = pSectionKeyFinal;
	}

	return iHash;
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
