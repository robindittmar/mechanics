#include "ICvar.h"

void ConVar::SetValue(const char* value)
{
	typedef void(__thiscall* SetString_t)(void*, const char*);
	return ((SetString_t)(*(void***)this)[14])(this, value);
}

void ConVar::SetValue(float value)
{
	typedef void(__thiscall* OriginalFn)(void*, float);
	return ((OriginalFn)(*(void***)this)[15])(this, value);
}

void ConVar::SetValue(int value)
{
	typedef void(__thiscall* OriginalFn)(void*, int);
	return ((OriginalFn)(*(void***)this)[16])(this, value);
}

char* ConVar::GetName()
{
	typedef char*(__thiscall* OriginalFn)(void*);
	return ((OriginalFn)(*(void***)this)[5])(this);
}
