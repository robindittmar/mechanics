#include "Console.h"

CConsole* g_pConsole = NULL;

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
	m_hIn = NULL;
	m_hOut = NULL;

	FreeConsole();
}

void CConsole::Write(int iLogLevel, const char* pFormat, ...)
{
	// Save attributes
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(m_hOut, &csbi);

	this->Write("[");

	switch (iLogLevel)
	{
	case LOGLEVEL_INFO:
		SetConsoleTextAttribute(m_hOut, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		this->Write("INFO");
		break;
	case LOGLEVEL_WARNING:
		SetConsoleTextAttribute(m_hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		this->Write("WARN");
		break;
	case LOGLEVEL_ERROR:
		SetConsoleTextAttribute(m_hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
		this->Write("ERRO");
		break;
	}

	// Restore attributes
	SetConsoleTextAttribute(m_hOut, csbi.wAttributes);

	this->Write("] ");

	va_list args;
	va_start(args, pFormat);
	this->Write(pFormat, args);
	va_end(args);
}

void CConsole::WritePlain(const char* pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);
	this->Write(pFormat, args);
	va_end(args);
}

void CConsole::Write(const char* pFormat, va_list args)
{
	char pBuffer[4096];
	vsprintf(pBuffer, pFormat, args);

	this->Write((const char*)pBuffer);
}

void CConsole::Write(const char* pText)
{
	WriteConsole(m_hOut, pText, strlen(pText), NULL, NULL);
}
