#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <Windows.h>
#include <cstdarg>
#include <cstdio>

#define LOGLEVEL_INFO		0
#define LOGLEVEL_WARNING	1
#define LOGLEVEL_ERROR		2

class CConsole
{
public:
	CConsole();
	~CConsole();

	void Write(int iLogLevel, const char* pFormat, ...);
	void WritePlain(const char* pFormat, ...);
private:
	void Write(const char* pFormat, va_list args);
	void Write(const char* pText);

	HANDLE m_hOut, m_hIn;
};

extern CConsole* g_pConsole;

#endif // __CONSOLE_H_