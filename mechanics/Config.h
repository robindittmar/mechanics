#ifndef __CONFIG_H__
#define __CONFIG_H__

// Std Lib
#include <Windows.h>
#include <cstdio>
#include <unordered_map>

#define CONFIG_FOLDER	"cfg\\"

#define CONFIG_SECTION_NONE			-1
#define CONFIG_SECTION_RAGEBOT		0
#define CONFIG_SECTION_ANTIAIM		1
#define CONFIG_SECTION_RESOLVER		2
#define CONFIG_SECTION_LEGITBOT		3
#define CONFIG_SECTION_TRIGGERBOT	4
#define CONFIG_SECTION_ESP			5
#define CONFIG_SECTION_WEAPONESP	6
#define CONFIG_SECTION_SOUNDESP		7
#define CONFIG_SECTION_CHAMS		8
#define CONFIG_SECTION_EFFECTS		9
#define CONFIG_SECTION_VISUALSOTHER	10
#define CONFIG_SECTION_FOV			11
#define CONFIG_SECTION_MISC			12

class CApplication;

class CConfig
{
public:
	CConfig();
	~CConfig();

	void Init(CApplication* pApp);

	bool LoadFile(const char* pFilename);
	bool SaveFile(const char* pFilename);
private:
	void DeleteValues();

	int m_iCurSection;
	std::unordered_map<uint32_t, int> m_mapSections;
	std::unordered_map<uint32_t, const char*> m_mapValues;

	CApplication* m_pApp;
};

#endif // __CONFIG_H__