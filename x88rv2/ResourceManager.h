#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include "IMaterialSystem.h"
#include "ISurface.h"
#include "KeyValues.h"

class CApplication;

class CResourceManager
{
public:
	CResourceManager();
	~CResourceManager();

	void Init(CApplication* pApp);
	IMaterial* CreateMaterial(bool bIsLit = false, bool bIsFlat = true, bool bIgnoreZ = false, bool bWireframe = false);
private:
	int m_iMaterialCount;

	CApplication* m_pApp;
};

#endif // __RESOURCEMANAGER_H__