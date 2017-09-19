#include "ResourceManager.h"
#include "Application.h"

CResourceManager* g_pResourceManager;

CResourceManager::CResourceManager() :
	/*VertexLitGeneric*/m_xorVertexLitGeneric("An÷¶rsÉ«cLà¬ryì¡"),
	/*UnlitGeneric*/m_xorUnlitGeneric("Beé«cLà¬ryì¡"),
	/*mechmat_%d.vmt*/m_xorMatName("znæªzjñ2o«´z")
{
	m_iMaterialCount = 0;
	m_pMirror = NULL;
}

CResourceManager::~CResourceManager()
{
	if (m_pMatMirror)
		m_pMatMirror->DecrementReferenceCount();

	if (m_pMirror)
		m_pMirror->DecrementReferenceCount();

	ISurface* pSurface = m_pApp->Surface();
	for(std::unordered_map<int, int>::iterator it = m_mapTextures.begin(); it != m_mapTextures.end(); it++)
	{
		pSurface->DeleteTextureByID(it->second);
	}
}

void CResourceManager::Init(CApplication* pApp)
{
	m_pApp = pApp;
}

IMaterial* CResourceManager::CreateMaterial(bool bIsLit, bool bIsFlat, bool bIgnoreZ, bool bWireframe)
{
	static CXorString xorVmt("5.öàpË5/ç£dnñ§oð°r)¥àalð«8|í«cnÚ£soì¶~}àà§æreó¯v{§â5)Ë5/è­snéà7)´à§æqgä¶5+§çs)Ë5/ë­t~é®5+§ò5Œà3xà®qbé®bf§â5:§È)¡ªvgã®vfç§e§â5:§È)¡¬xmê¥5+§ò5Œà3bâ¬xyà¸5+§çs)Ë5/ÿ¬rj÷§e)¥à')Ë5/ò«enã°vfàà7) ¦5øÈ");

	const char* pBaseType = (bIsLit == true ? m_xorVertexLitGeneric.ToCharArray() : m_xorUnlitGeneric.ToCharArray());
	char pMaterial[1024];
	char pName[512];
	KeyValues* pKeyValues;

	sprintf(pMaterial, xorVmt.ToCharArray(), pBaseType, (bIsFlat ? 1 : 0), (bIgnoreZ ? 1 : 0), (bWireframe ? 1 : 0));
	sprintf(pName, m_xorMatName.ToCharArray(), m_iMaterialCount++);

	pKeyValues = (KeyValues*)malloc(sizeof(KeyValues));
	m_pApp->InitKeyValues()(pKeyValues, pBaseType);
	m_pApp->LoadFromBuffer()(pKeyValues, pName, pMaterial, NULL, NULL, NULL);

	IMaterial* pMat = m_pApp->MaterialSystem()->CreateMaterial(pName, pKeyValues);
	pMat->IncrementReferenceCount();

	return pMat;
}

void CResourceManager::CreateMirror()
{
	IMaterialSystem* pMatSys = m_pApp->MaterialSystem();
	
	// 0x2C68
	DWORD dwMatSysInit = *(DWORD*)(CPattern::FindPattern(
		(BYTE*)m_pApp->MaterialSystemDll(),
		MATERIALSYSTEMDLL_SIZE,
		(BYTE*)"\x80\xB9\x00\x00\x00\x00\x00\x74\x0F",
		"gf----hfg"
	) + 0x02);

	bool* pInitialized = (bool*)((DWORD)pMatSys + dwMatSysInit);
	bool bOrig = *pInitialized;
	*pInitialized = false;
	pMatSys->BeginRenderTargetAllocation();
	m_pMirror = pMatSys->CreateNamedRenderTargetTextureEx(/*mechanics_mirr*/CXorString("znæªveì¡dTè«ey").ToCharArray(), MIRROR_WIDTH, MIRROR_HEIGHT, RT_SIZE_DEFAULT, IMAGE_FORMAT_RGBA8888);
	pMatSys->EndRenderTargetAllocation();
	*pInitialized = bOrig;

	if (m_pMirror)
	{
		// Prevent it from getting cleaned up
		m_pMirror->IncrementReferenceCount();
		
		CXorString xorMatMechanicsMirr("zjñznæªveì¡dTè«ey"); // mat_mechanics_mirr
		KeyValues* pKeyValues;
		pKeyValues = (KeyValues*)malloc(sizeof(KeyValues));
		m_pApp->InitKeyValues()(pKeyValues, m_xorUnlitGeneric.ToCharArray());
		m_pApp->LoadFromBuffer()(pKeyValues, xorMatMechanicsMirr.ToCharArray(), /*\"UnlitGeneric\"\n{\n\t\"$basetexture\" \"mechanics_mirr\"\n}*/CXorString("5^ë®~Â§yn÷«t)¹\x1d\x2§æujö§cný¶byàà7)è§tcä¬~hözb÷°5\x1ø").ToCharArray(), NULL, NULL, NULL);

		m_pMatMirror = m_pApp->MaterialSystem()->CreateMaterial(xorMatMechanicsMirr.ToCharArray(), pKeyValues);
		m_pMatMirror->IncrementReferenceCount();
	}
	
}

void CResourceManager::CreateTextures()
{
	ISurface* pSurface = m_pApp->Surface();

	int textureCursor = pSurface->CreateNewTextureID(true);
	int textureColorFade = pSurface->CreateNewTextureID(true);

	unsigned char pTexCursor[] = { 0, 255, 90, 0 };

	int curR, curG, curB;
	unsigned char pTexColorFade[360 * 4];
	for(int y = 0; y < 360 * 4; y += 4)
	{
		this->HslToRgb(y, 1.0f, 1.0f, curR, curG, curB);

		pTexColorFade[y] = curR;
		pTexColorFade[y + 1] = curG;
		pTexColorFade[y + 2] = curB;
		pTexColorFade[y + 3] = 255; // Alpha
	}

	pSurface->DrawSetTextureRGBA(textureCursor, pTexCursor, 1, 1);
	pSurface->DrawSetTextureRGBA(textureColorFade, pTexColorFade, 1, 360);

	m_mapTextures[RM_TEXTURE_CURSOR] = textureCursor;
	m_mapTextures[RM_TEXTURE_COLORFADE] = textureColorFade;
}

int CResourceManager::GetTexture(int textureId)
{
	return m_mapTextures[textureId];
}

void CResourceManager::CreateFonts()
{
	static CXorString xorTahoma("Cjí­zj");

	ISurface* pSurface = m_pApp->Surface();

	unsigned int fontNormal = pSurface->SCreateFont();
	unsigned int fontHeader = pSurface->SCreateFont();
	unsigned int fontSubheader = pSurface->SCreateFont();

	pSurface->SetFontGlyphSet(fontNormal, xorTahoma.ToCharArray(), 11, 255, 0, 0, FONTFLAG_NONE);
	pSurface->SetFontGlyphSet(fontHeader, xorTahoma.ToCharArray(), 16, 255, 0, 0, FONTFLAG_OUTLINE);
	pSurface->SetFontGlyphSet(fontSubheader, xorTahoma.ToCharArray(), 14, 255, 0, 0, FONTFLAG_OUTLINE);

	m_mapFonts[RM_FONT_NORMAL] = fontNormal;
	m_mapFonts[RM_FONT_HEADER] = fontHeader;
	m_mapFonts[RM_FONT_SUBHEADER] = fontSubheader;
}

unsigned int CResourceManager::GetFont(int fontId)
{
	return m_mapFonts[fontId];
}

void CResourceManager::HslToRgb(int h, float s, float l, int& r, int &g, int& b)
{
	double      hh, p, q, t, ff;
	long        i;

	if (s <= 0.0) {       // < is bogus, just shuts up warnings
		r = l;
		g = l;
		b = l;
		return;
	}
	hh = h;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = l * (1.0 - s);
	q = l * (1.0 - (s * ff));
	t = l * (1.0 - (s * (1.0 - ff)));

	p *= 255.0f;
	q *= 255.0f;
	t *= 255.0f;

	double ll = l * 255.0f;

	switch (i)
	{
	case 0:
		r = ll;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = ll;
		b = p;
		break;
	case 2:
		r = p;
		g = ll;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = ll;
		break;
	case 4:
		r = t;
		g = p;
		b = ll;
		break;
	case 5:
	default:
		r = ll;
		g = p;
		b = q;
		break;
	}
}
