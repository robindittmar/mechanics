#ifndef __IMATRENDERCONTEXT_H__
#define __IMATRENDERCONTEXT_H__

#include "IRefCounted.h"

class ITexture;

class IMatRenderContext : public IRefCounted
{
public:
	void Rotate(float angle, float x, float y, float z);
};

#endif // __IMATRENDERCONTEXT_H__