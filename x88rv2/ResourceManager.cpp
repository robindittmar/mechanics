#include "ResourceManager.h"
#include "Application.h"

CResourceManager* g_pResourceManager;

CResourceManager::CResourceManager()
{
	m_iMaterialCount = 0;
}

CResourceManager::~CResourceManager()
{
}

void CResourceManager::Init(CApplication* pApp)
{
	m_pApp = pApp;
}

IMaterial* CResourceManager::CreateMaterial(bool bIsLit, bool bIsFlat, bool bIgnoreZ, bool bWireframe)
{
	//static CXorString xorVmt("5.��p��5/�dn�o�r)��al�8|�cnڣso�~}����re�v{��5)��5/�sn��7)����qg�5+��s)��5/�t~�5+��5��3x�qb�bf��5:��)��vg�vf�e��5:��)��xm�5+��5��3b�xy�5+��s)��5/��rj��e)��')��5/�en�vf��7)��5��");
	
	static CXorString xorVertexLitGeneric("An��rsɫcL�ry�");
	static CXorString xorUnlitGeneric("Be�cL�ry�");
	static CXorString xorMatName("oc�cT��9}�");

	const char* pBaseType = (bIsLit == true ? xorVertexLitGeneric.ToCharArray() : xorUnlitGeneric.ToCharArray());
	//char pMaterial[1024];
	char pName[512];
	KeyValues* pKeyValues;

	//sprintf(pMaterial, xorVmt.ToCharArray(), pBaseType, (bIsFlat ? 1 : 0), (bIgnoreZ ? 1 : 0), (bWireframe ? 1 : 0));
	sprintf(pName, xorMatName.ToCharArray(), m_iMaterialCount++);

	pKeyValues = (KeyValues*)malloc(sizeof(KeyValues));
	m_pApp->InitKeyValues()(pKeyValues, pBaseType);
	// TODO: Idk
	//m_pApp->LoadFromBuffer()(pKeyValues, pName, pMaterial, NULL, NULL, NULL);

	IMaterial* pMat = m_pApp->MaterialSystem()->CreateMaterial(pName, pKeyValues);
	pMat->IncrementReferenceCount();

	return pMat;
}

void CResourceManager::CreateFonts()
{
	static CXorString xorArial("Vy�{");

	ISurface* pSurface = m_pApp->Surface();

	unsigned int fontNormal = pSurface->SCreateFont();
	unsigned int fontHeader = pSurface->SCreateFont();
	unsigned int fontSubheader = pSurface->SCreateFont();

	pSurface->SetFontGlyphSet(fontNormal, xorArial.ToCharArray(), 16, 255, 0, 0, 0x200);
	pSurface->SetFontGlyphSet(fontHeader, xorArial.ToCharArray(), 20, 255, 0, 0, 0x200);
	pSurface->SetFontGlyphSet(fontSubheader, xorArial.ToCharArray(), 18, 255, 0, 0, 0x200);

	m_mapFonts[RM_FONT_NORMAL] = fontNormal;
	m_mapFonts[RM_FONT_HEADER] = fontHeader;
	m_mapFonts[RM_FONT_SUBHEADER] = fontSubheader;
}

unsigned int CResourceManager::GetFont(int fontId)
{
	return m_mapFonts[fontId];
}