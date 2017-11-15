#include "Library.h"

CLibrary::CLibrary(const char* pFilepath)
{
	m_pFilepath = NULL;

	if (pFilepath != "")
	{
		this->Filepath(pFilepath);
	}
}

CLibrary::~CLibrary()
{
	DELETE_ARR(m_pFilepath);
}

void CLibrary::Filepath(const char* pFilepath)
{
	DELETE_ARR(m_pFilepath);

	int iLen = strlen(pFilepath);
	m_pFilepath = new char[iLen + 1];
	memcpy(m_pFilepath, pFilepath, iLen + 1);
}

bool CLibrary::RandomizeJunkcode()
{
	// TODO
	return true;
}