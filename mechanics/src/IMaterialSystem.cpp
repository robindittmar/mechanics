#include "IMaterialSystem.h"

IMaterial* IMaterialSystem::CreateMaterial(const char* pFilename, KeyValues* pKeyValues)
{
	typedef IMaterial*(__thiscall *CreateMaterial_t)(void*, const char*, KeyValues*);
	return ((CreateMaterial_t)(*(void***)this)[83])(this, pFilename, pKeyValues);
}

IMaterial* IMaterialSystem::FindMaterial(char const* pMaterialName, const char* pTextureGroupName, bool complain, const char* pComplainPrefix)
{
	typedef IMaterial*(__thiscall *FindMaterial_t)(void*, char const*, const char*, bool, const char*);
	return ((FindMaterial_t)(*(void***)this)[84])(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
}

MaterialHandle_t IMaterialSystem::FirstMaterial()
{
	typedef MaterialHandle_t(__thiscall *FirstMaterial_t)(void*);
	return ((FirstMaterial_t)(*(void***)this)[86])(this);
}

MaterialHandle_t IMaterialSystem::NextMaterial(MaterialHandle_t h)
{
	typedef MaterialHandle_t(__thiscall *NextMaterial_t)(void*, MaterialHandle_t);
	return ((NextMaterial_t)(*(void***)this)[87])(this, h);
}

MaterialHandle_t IMaterialSystem::InvalidMaterial()
{
	typedef MaterialHandle_t(__thiscall *InvalidMaterial_t)(void*);
	return ((InvalidMaterial_t)(*(void***)this)[88])(this);
}

IMaterial* IMaterialSystem::GetMaterial(MaterialHandle_t h)
{
	typedef IMaterial*(__thiscall *GetMaterial_t)(void*, MaterialHandle_t);
	return ((GetMaterial_t)(*(void***)this)[89])(this, h);
}


void IMaterialSystem::GetBackBufferDimensions(int &width, int &height)
{
	typedef void(__thiscall *GetBackBufferDimensions_t)(void*, int&, int&);
	((GetBackBufferDimensions_t)(*(void***)this)[35])(this, width, height);
}

ImageFormat IMaterialSystem::GetBackBufferFormat()
{
	typedef ImageFormat(__thiscall *GetBackBufferFormat_t)(void*);
	return ((GetBackBufferFormat_t)(*(void***)this)[36])(this);
}

ITexture* IMaterialSystem::FindTexture(char const* pTextureName, const char* pTextureGroupName, bool complain, int nAdditionalCreationFlags)
{
	typedef ITexture*(__thiscall *FindTexture_t)(void*, char const*, const char*, bool, int);
	return ((FindTexture_t)(*(void***)this)[91])(this, pTextureName, pTextureGroupName, complain, nAdditionalCreationFlags);
}

void IMaterialSystem::BeginRenderTargetAllocation()
{
	typedef void(__thiscall *BeginRenderTargetAllocation_t)(void*);
	((BeginRenderTargetAllocation_t)(*(void***)this)[94])(this);
}

void IMaterialSystem::EndRenderTargetAllocation()
{
	typedef void(__thiscall *EndRenderTargetAllocation_t)(void*);
	((EndRenderTargetAllocation_t)(*(void***)this)[95])(this);
}

ITexture* IMaterialSystem::CreateRenderTargetTexture(int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth)
{
	typedef ITexture*(__thiscall *CreateRenderTargetTexture_t)(void*, int, int, RenderTargetSizeMode_t, ImageFormat, MaterialRenderTargetDepth_t);
	return ((CreateRenderTargetTexture_t)(*(void***)this)[96])(this, w, h, sizeMode, format, depth);
}

ITexture* IMaterialSystem::CreateNamedRenderTargetTextureEx(const char *pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth, unsigned int textureFlags, unsigned int renderTargetFlags)
{
	typedef ITexture*(__thiscall *CreateNamedRenderTargetTextureEx_t)(void*, const char*, int, int, RenderTargetSizeMode_t, ImageFormat, MaterialRenderTargetDepth_t, unsigned int, unsigned int);
	return ((CreateNamedRenderTargetTextureEx_t)(*(void***)this)[97])(this, pRTName, w, h, sizeMode, format, depth, textureFlags, renderTargetFlags);
}

IMatRenderContext* IMaterialSystem::GetRenderContext()
{
	typedef IMatRenderContext*(__thiscall *GetRenderContext_t)(void*);
	return ((GetRenderContext_t)(*(void***)this)[115])(this);
}
