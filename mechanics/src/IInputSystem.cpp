#include "IInputSystem.h"

void IInputSystem::ResetInputState()
{
	typedef void(__thiscall *ResetInputState)(void*);
	return ((ResetInputState)(*(void***)this)[39])(this);
}

bool IInputSystem::IsButtonDown(ButtonCode_t code)
{
    typedef bool(__thiscall* ButtonCode_t)(void*, ButtonCode_t);
    return ((ButtonCode_t)(*(bool***)this)[15])(this, code);
}