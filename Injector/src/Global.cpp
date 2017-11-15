#include "Global.h"

CGlobal* g_pGlobal;

CGlobal::CGlobal(int argc, char** argv)
{
	m_pWorkingDir = NULL;
	m_pExecutable = NULL;

	ParseFullExePath(argv[0]);

	// Loop over arguments
	for (int i = 1; i < argc; i++)
	{
		// TODO
	}
}

CGlobal::~CGlobal()
{
	DELETE_ARR(m_pWorkingDir);
	DELETE_ARR(m_pExecutable);
}

void CGlobal::WorkingDirectory(const char* pWorkingDir)
{
	DELETE_ARR(m_pWorkingDir);

	int iLen = strlen(pWorkingDir);
	m_pWorkingDir = new char[iLen + 1];
	memcpy(m_pWorkingDir, pWorkingDir, iLen + 1);
}

void CGlobal::Executable(const char* pExecutable)
{
	DELETE_ARR(m_pExecutable);

	int iLen = strlen(pExecutable);
	m_pExecutable = new char[iLen + 1];
	memcpy(m_pExecutable, pExecutable, iLen + 1);
}

void CGlobal::ParseFullExePath(const char* pPath)
{
	int iLen = strlen(pPath);

	char* pPathCopy = new char[iLen + 1];
	memcpy(pPathCopy, pPath, iLen + 1);

	for (int i = iLen - 1; i >= 0; i--)
	{
		if (pPathCopy[i] == '\\' ||
			pPathCopy[i] == '/')
		{
			const char* pExec = pPathCopy + i + 1;

			pPathCopy[i] = '\0';

			WorkingDirectory(pPathCopy);
			Executable(pExec);
			break;
		}
	}
}
