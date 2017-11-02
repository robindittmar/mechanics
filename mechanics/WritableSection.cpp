#include "WritableSection.h"

CWritableSection::CWritableSection(const char* pSection)
	: m_pSection(NULL)
{
	this->SetSection(pSection);
}

CWritableSection::~CWritableSection()
{
	const char* p;
	for (std::vector<const char*>::iterator it = m_vLines.begin(); it != m_vLines.end(); it++)
	{
		p = *it;

		if (p)
			delete[] p;
	}
}

void CWritableSection::SetSection(const char* pSection)
{
	if (m_pSection)
		delete[] m_pSection;

	if (!pSection)
		return;

	int iLen = strlen(pSection) + 1;
	m_pSection = new char[iLen];
	memcpy(m_pSection, pSection, iLen);
}

void CWritableSection::AddLine(const char* pLine)
{
	int iLen = strlen(pLine) + 1;
	char* pLineCopy = new char[iLen];
	memcpy(pLineCopy, pLine, iLen);

	m_vLines.push_back(pLineCopy);
}

void CWritableSection::WriteToFile(FILE* pFile)
{
	fprintf(pFile, "[%s]\n", m_pSection);

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "[%s]\n", m_pSection);
#endif // _DEBUG

	for (std::vector<const char*>::iterator it = m_vLines.begin(); it != m_vLines.end(); it++)
	{
		fprintf(pFile, "%s\n", *it);

#ifdef _DEBUG
		g_pConsole->Write(LOGLEVEL_INFO, "%s\n", *it);
#endif // _DEBUG
	}
	fputs("\n", pFile);
	
#ifdef _DEBUG
	g_pConsole->WritePlain("\n");
#endif // _DEBUG
}
