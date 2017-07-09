#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "Defines.h"
#include <cstring>

class CGlobal
{
public:
	CGlobal(int argc, char** argv);
	~CGlobal();

	const char* WorkingDirectory() { return m_pWorkingDir; }
	const char* Executable() { return m_pExecutable; }

	void WorkingDirectory(const char* pWorkingDir);
	void Executable(const char* pExecutable);
private:
	void ParseFullExePath(const char* pPath);

	char* m_pWorkingDir;
	char* m_pExecutable;
};

extern CGlobal* g_pGlobal;

#endif // __GLOBAL_H__