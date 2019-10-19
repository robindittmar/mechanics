#include "ICVar.h"

void ConVar::SetValue(const char* value)
{
	typedef void(__thiscall* SetValue_t)(void*, const char*);
	return ((SetValue_t)(*(void***)this)[14])(this, value);
}

void ConVar::SetValue(float value)
{
	typedef void(__thiscall* SetValue_t)(void*, float);
	return ((SetValue_t)(*(void***)this)[15])(this, value);
}

void ConVar::SetValue(int value)
{
	typedef void(__thiscall* SetValue_t)(void*, int);
	return ((SetValue_t)(*(void***)this)[16])(this, value);
}

char* ConVar::GetName()
{
	typedef char*(__thiscall* GetName_t)(void*);
	return ((GetName_t)(*(void***)this)[5])(this);
}
