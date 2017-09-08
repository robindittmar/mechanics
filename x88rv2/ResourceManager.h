#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include <unordered_map>

#include "IMaterialSystem.h"
#include "ISurface.h"
#include "KeyValues.h"

#define RM_TEXTURE_CURSOR		0
#define RM_TEXTURE_COLORFADE	1

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

	void CreateMirror();
	ITexture* GetMirror() { return m_pMirror; }

	void CreateTextures();
	int GetTexture(int textureId);

	void CreateFonts();
	unsigned int GetFont(int fontId);
private:
	void HslToRgb(int h, float s, float l, int& r, int &g, int& b);

	int m_iMaterialCount;

	ITexture* m_pMirror;
	std::unordered_map<int, int> m_mapTextures;
	std::unordered_map<int, unsigned int> m_mapFonts;

	CApplication* m_pApp;
};

extern CResourceManager* g_pResourceManager;

#endif // __RESOURCEMANAGER_H__