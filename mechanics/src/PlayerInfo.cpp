#include "PlayerInfo.h"
#include <Windows.h>

int PlayerInfo::GetName(wchar_t* p, int len) {
	mbstowcs(p, name, len);
	return lstrlenW(p);
}