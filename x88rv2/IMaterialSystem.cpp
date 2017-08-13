#include "IMaterialSystem.h"

IMaterial* IMaterialSystem::FindMaterial(char const* pMaterialName, const char* pTextureGroupName, bool complain, const char* pComplainPrefix)
{
	typedef IMaterial* (__thiscall *FindMaterial_t)(void*, char const*, const char*, bool, const char*);
	return ((FindMaterial_t)(*(void***)this)[84])(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
}
