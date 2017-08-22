#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include <map>

#include "IMaterialSystem.h"
#include "ISurface.h"
#include "KeyValues.h"

#define RM_FONT_NORMAL			0
#define RM_FONT_HEADER			1
#define RM_FONT_SUBHEADER		2

class CApplication;

class CResourceManager
{
public:
	CResourceManager();
	~CResourceManager();

	void Init(CApplication* pApp);
	IMaterial* CreateMaterial(bool bIsLit = false, bool bIsFlat = true, bool bIgnoreZ = false, bool bWireframe = false);

	void CreateFonts();
	unsigned int GetFont(int fontId);
private:
	int m_iMaterialCount;

	std::map<int, unsigned int> m_mapFonts;

	CApplication* m_pApp;
};

extern CResourceManager* g_pResourceManager;

#endif // __RESOURCEMANAGER_H__