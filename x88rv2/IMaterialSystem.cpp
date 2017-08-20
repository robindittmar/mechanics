#include "IMaterialSystem.h"

IMaterial* IMaterialSystem::CreateMaterial(const char* pFilename, KeyValues* pKeyValues)
{
	typedef IMaterial* (__thiscall *CreateMaterial_t)(void*, const char*, KeyValues*);
	return ((CreateMaterial_t)(*(void***)this)[83])(this, pFilename, pKeyValues);
}

IMaterial* IMaterialSystem::FindMaterial(char const* pMaterialName, const char* pTextureGroupName, bool complain, const char* pComplainPrefix)
{
	typedef IMaterial* (__thiscall *FindMaterial_t)(void*, char const*, const char*, bool, const char*);
	return ((FindMaterial_t)(*(void***)this)[84])(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
}
