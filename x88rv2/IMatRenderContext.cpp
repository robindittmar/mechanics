#include "IMatRenderContext.h"

/*void IMatRenderContext::Rotate(float angle, float x, float y, float z)
{
	typedef void(__thiscall *Rotate_t)(void*, float, float, float, float);
	return ((Rotate_t)(*(void***)this)[36])(this, angle, x, y, z);
}*/

// 113

void IMatRenderContext::CopyTextureToRenderTargetEx(int nRenderTargetID, ITexture* pTexture, Rect_t* pSrcRect, Rect_t* pDstRect)
{
	typedef void(__thiscall *CopyTextureToRenderTargetEx_t)(void*, int, ITexture*, Rect_t*, Rect_t*);
	return ((CopyTextureToRenderTargetEx_t)(*(void***)this)[113])(this, nRenderTargetID, pTexture, pSrcRect, pDstRect);
}