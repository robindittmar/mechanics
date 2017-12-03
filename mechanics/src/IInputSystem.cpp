#include "IInputSystem.h"

void IInputSystem::ResetInputState()
{
	typedef void(__thiscall *ResetInputState)(void*);
	return ((ResetInputState)(*(void***)this)[39])(this);
}