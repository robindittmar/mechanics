#ifndef __WRITABLESECTION_H__
#define __WRITABLESECTION_H__

// Std Lib
#include <vector>

// Custom
#include "Console.h"

class CWritableSection
{
public:
	CWritableSection(const char* pSection = NULL);
	~CWritableSection();

	void SetSection(const char* pSection);
	void AddLine(const char* pLine);

	void WriteToFile(FILE* pFile);
private:
	char* m_pSection;
	std::vector<const char*> m_vLines;
};

#endif // __WRITABLESECTION_H__