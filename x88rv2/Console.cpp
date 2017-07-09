#include "Console.h"

CConsole::CConsole()
{
	AllocConsole();

	m_hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	m_hIn = GetStdHandle(STD_INPUT_HANDLE);
}

CConsole::~CConsole()
{
	CloseHandle(m_hOut);
	CloseHandle(m_hIn);

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