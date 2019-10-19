#include "IMatRenderContext.h"

/*void IMatRenderContext::Rotate(float angle, float x, float y, float z)
{
	typedef void(__thiscall *Rotate_t)(void*, float, float, float, float);
	return ((Rotate_t)(*(void***)this)[36])(this, angle, x, y, z);
}*/

// 104?
void IMatRenderContext::DrawScreenSpaceRectangle(IMaterial *pMaterial, int destx, int desty, int width, int height, float src_texture_x0, float src_texture_y0, float src_texture_x1, float src_texture_y1, int src_texture_width, int src_texture_height, void *pClientRenderable, int nXDice, int nYDice)
{
	typedef void(__thiscall *DrawScreenSpaceRectangle_t)(void*, IMaterial*, int, int, int, int, float, float, float, float, int, int, void*, int, int);
	return ((DrawScreenSpaceRectangle_t)(*(void***)this)[114])(this, pMaterial, destx, desty, width, height, src_texture_x0, src_texture_y0, src_texture_x1, src_texture_y1, src_texture_width, src_texture_height, pClientRenderable, nXDice, nYDice);
}

// 113?
void IMatRenderContext::CopyTextureToRenderTargetEx(int nRenderTargetID, ITexture* pTexture, Rect_t* pSrcRect, Rect_t* pDstRect)
{
	typedef void(__thiscall *CopyTextureToRenderTargetEx_t)(void*, int, ITexture*, Rect_t*, Rect_t*);
	return ((CopyTextureToRenderTargetEx_t)(*(void***)this)[113])(this, nRenderTargetID, pTexture, pSrcRect, pDstRect);
}