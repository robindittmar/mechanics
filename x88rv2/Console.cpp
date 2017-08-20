#include "Console.h"

CConsole* g_pConsole;

CConsole::CConsole()
{
	AllocConsole();

	// Reopen standard input/output
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	m_hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	m_hIn = GetStdHandle(STD_INPUT_HANDLE);
}

CConsole::~CConsole()
{
	FreeConsole();
}

void CConsole::Write(const char* pFormat, ...)
{
	char pBuffer[4096];
	va_list args;
	va_start(args, pFormat);
	vsprintf(pBuffer, pFormat, args);
	va_end(args);

	WriteConsole(m_hOut, pBuffer, strlen(pBuffer), NULL, NULL);
}