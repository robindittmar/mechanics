#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <Windows.h>
#include <cstdarg>
#include <cstdio>

class CConsole
{
public:
	CConsole();
	~CConsole();

	void Write(const char* pFormat, ...);
private:
	HANDLE m_hOut, m_hIn;
};

extern CConsole* g_pConsole;

#endif // __CONSOLE_H_