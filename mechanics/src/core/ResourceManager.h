#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

// Std Lib
#include <unordered_map>

// Source SDK
#include "../source_sdk/IMaterialSystem.h"
#include "../source_sdk/ISurface.h"
#include "../source_sdk/KeyValues.h"

// Libs
#include "../third_party/lodepng/lodepng.h"

// Custom
#include "../utils/XorString.h"
#include "../utils/Utils.h"

#define RM_TEXTURE_COLORFADE	1
#define RM_TEXTURE_SVFADE		2
#define RM_TEXTURE_BACKGROUND	100

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
	IMaterial* GetMirrorMaterial() { return m_pMatMirror; }

	void BuildFadeTexture();
	void BuildSaturationValueTexture(int iHue);

	void CreateTextures();
	int GetTexture(int textureId);
	int GetWhiteTexture() { return m_iTextureWhite; }

	void CreateFonts();
	unsigned int GetFont(int fontId);
private:
	int LoadPngToTexture(ISurface* pSurface, const char* pFilename);

	int m_iMaterialCount;

	ITexture* m_pMirror;
	IMaterial* m_pMatMirror;
	std::unordered_map<int, int> m_mapTextures;
	std::unordered_map<int, unsigned int> m_mapFonts;

	int m_iTextureWhite;

	CXorString m_xorVertexLitGeneric;
	CXorString m_xorUnlitGeneric;
	CXorString m_xorMatName;

	CApplication* m_pApp;
};

extern CResourceManager* g_pResourceManager;

#endif // __RESOURCEMANAGER_H__