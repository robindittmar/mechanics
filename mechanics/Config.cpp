#include "Config.h"
#include "Application.h"

CConfig::CConfig()
{
	m_iCurSection = CONFIG_SECTION_NONE;

	m_mapSections[0x46d432d8] = CONFIG_SECTION_RAGEBOT;
	m_mapSections[0x7f16f4fb] = CONFIG_SECTION_ANTIAIM;
	m_mapSections[0xd44b1ec3] = CONFIG_SECTION_RESOLVER;
	m_mapSections[0x6afa6af7] = CONFIG_SECTION_LEGITBOT;
	m_mapSections[0x5d54d281] = CONFIG_SECTION_TRIGGERBOT;
	m_mapSections[0x77ff3728] = CONFIG_SECTION_ESP;
	m_mapSections[0x5eb8e60e] = CONFIG_SECTION_WEAPONESP;
	m_mapSections[0xe8592cfe] = CONFIG_SECTION_SOUNDESP;
	m_mapSections[0x89cd7231] = CONFIG_SECTION_CHAMS;
	m_mapSections[0x883cc607] = CONFIG_SECTION_EFFECTS;
	m_mapSections[0x2ce5a7d7] = CONFIG_SECTION_VISUALSOTHER;
	m_mapSections[0xa58f59a7] = CONFIG_SECTION_FOV;
	m_mapSections[0x8bad1a17] = CONFIG_SECTION_MISC;
}

CConfig::~CConfig()
{
	this->DeleteValues();
}

void CConfig::Init(CApplication* pApp)
{
	m_pApp = pApp;
}

bool CConfig::LoadFile(const char* pFilename)
{
	this->DeleteValues();

	char pFullpath[MAX_PATH];
	sprintf(pFullpath, "%s%s%s", m_pApp->GetWorkingDirectory(), CONFIG_FOLDER, pFilename);

	FILE* pFile = fopen(pFullpath, "r");
	if (!pFile)
		return false;

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
				break;
			}
		}

		// New section
		if (pBuffer[0] == '[')
		{
			m_iCurSection = m_mapSections[murmurhash(pBuffer, strlen(pBuffer), 0xB16B00B5)];
			continue;
		}
		else
		{
			pKey = strtok(pBuffer, "=");
			pValue = strtok(NULL, "=");

			uint32_t iHash = murmurhash(pKey, strlen(pKey), 0xB16B00B5);

			int iLenValue = strlen(pValue);
			char* pVal = new char[iLenValue + 1];
			strncpy(pVal, pValue, iLenValue);

			m_mapValues[iHash] = pVal;

			// TODO:
			// Wie persitieren wir die Section in der map?
			// => Wer und wie wird die config in die IFeature's übertragen?
		}
	}

	fclose(pFile);
	return true;
}

bool CConfig::SaveFile(const char* pFilename)
{
	return false;
}

void CConfig::DeleteValues()
{
	for (std::unordered_map<uint32_t, const char*>::iterator it = m_mapValues.begin(); it != m_mapValues.end(); it++)
	{
		delete[] it->second;
	}

	m_mapValues.clear();
}
