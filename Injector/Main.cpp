#include <Windows.h>
#include "Global.h"
#include "Console.h"
#include "Injector.h"
#include "Library.h"

#define _CRT_SECURE_NO_WARNINGS

int main(int argc, char** argv)
{
	g_pGlobal = new CGlobal(argc, argv);

	// Build full path for dll
	char pLibrary[256];
	sprintf(pLibrary, "%s\\%s", g_pGlobal->WorkingDirectory(), "mechanics.dll");

	CLibrary library(pLibrary);
	//CLibrary library("C:\\Users\\Robin\\Documents\\Visual Studio 2017\\Projects\\x88rv2\\Debug\\x88rv2.dll");
	CInjector injector;

	injector.Inject("csgo.exe", &library);

	delete g_pGlobal;
	return 0;
}