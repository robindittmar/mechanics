#ifndef __IMATERIALSYSTEM_H__
#define __IMATERIALSYSTEM_H__

#include "IMaterial.h"

#define NULL 0

class IMaterialSystem
{
public:
	IMaterial* IMaterialSystem::FindMaterial(char const* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = NULL);
private:
};

#endif // __IMATERIALSYSTEM_H__