#ifndef __CONFIG_H__
#define __CONFIG_H__

// Std Lib
#include <Windows.h>
#include <cstdio>
#include <unordered_map>
#include <unordered_set>

// Source SDK
#include "../source_sdk/ISurface.h"

// Custom
#include "WritableSection.h"

#define CONFIG_FOLDER	"cfg\\"

class CApplication;

class CConfig
{
public:
	CConfig();
	~CConfig();

	void Init(CApplication* pApp);

	bool LoadFile(const char* pFilename);
	bool SaveFile(const char* pFilename);

	const char* GetFirstSection();
	const char* GetNextSection();
	void DeleteSectionList();

	bool GetBool(const char* pSection, const char* pKey, bool* pOut = NULL);
	int GetInt(const char* pSection, const char* pKey, int* pOut = NULL);
	float GetFloat(const char* pSection, const char* pKey, float* pOut = NULL);
	Color GetColor(const char* pSection, const char* pKey, Color* pOut = NULL);
	const char* GetString(const char* pSection, const char* pKey, char* pOut = NULL, int iMaxLen = 256);

	void SetBool(const char* pSection, const char* pKey, bool bValue);
	void SetInt(const char* pSection, const char* pKey, int iValue);
	void SetFloat(const char* pSection, const char* pKey, float fValue);
	void SetColor(const char* pSection, const char* pKey, Color cValue);
	void SetString(const char* pSection, const char* pKey, const char* pValue);
private:
	uint32_t BuildSectionKeyHash(const char* pSection, const char* pKey);
	uint32_t BuildAndStoreSectionKeyHash(const char* pSection, const char* pKey);
	void DeleteValues();

	char* m_pCurSection;
	std::unordered_set<const char*>::iterator m_itSection;
	std::unordered_set<const char*> m_setSections;
	std::unordered_map<uint32_t, const char*> m_mapKeys;
	std::unordered_map<uint32_t, const char*> m_mapValues;

	CApplication* m_pApp;
};

#endif // __CONFIG_H__