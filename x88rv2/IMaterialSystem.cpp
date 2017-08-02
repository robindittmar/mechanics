#include "IMaterialSystem.h"

inline const void** getvtable(const void* inst, size_t offset = 0)
{
	return *reinterpret_cast<const void***>((size_t)inst + offset);
}

template<typename Fn>
inline Fn getvfunc(const void* inst, size_t index, size_t offset = 0)
{
	return reinterpret_cast<Fn>(getvtable(inst, offset)[index]);
}

IMaterial* IMaterialSystem::FindMaterial(char const* pMaterialName, const char* pTextureGroupName, bool complain, const char* pComplainPrefix)
{
	typedef IMaterial* (__thiscall *FindMaterial_t)(void*, char const*, const char*, bool, const char*);
	return getvfunc<FindMaterial_t>(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
}