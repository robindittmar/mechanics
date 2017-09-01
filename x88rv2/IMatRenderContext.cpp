#include "IMatRenderContext.h"

void IMatRenderContext::Rotate(float angle, float x, float y, float z)
{
	typedef void(__thiscall *Rotate_t)(void*, float, float, float, float);
	return ((Rotate_t)(*(void***)this)[36])(this, angle, x, y, z);
}
