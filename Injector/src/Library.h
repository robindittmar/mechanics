#ifndef __LIBRARY_H__
#define __LIBRARY_H__

#include <cstring>
#include "Defines.h"

#define JUNKCODE_PATTERN	"\x00\x00\x00\x00" // TODO

class CLibrary
{
public:
	CLibrary(const char* pFilepath = "");
	~CLibrary();

	void Filepath(const char* pFilepath);
	const char* Filepath() { return m_pFilepath; }

	/// <summary>
	/// Randomizes the junkcode in the given library
	/// </summary>
	/// <returns></returns>
	bool RandomizeJunkcode();
private:
	char* m_pFilepath;
};

#endif // __LIBRARY_H__