#include "ResourceManager.h"
#include "Application.h"

CResourceManager* g_pResourceManager;

CResourceManager::CResourceManager() :
	/*VertexLitGeneric*/m_xorVertexLitGeneric("An��rsɫcL�ry�"),
	/*UnlitGeneric*/m_xorUnlitGeneric("Be�cL�ry�"),
	/*mechmat_%d.vmt*/m_xorMatName("zn�zj�2o��z")
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
	// TODO: Try to set envmap in chams material
	static CXorString xorVmt("5.��p��5/�dn�o�r)��al�8|�cnڣso�~}����re�v{��5)��5/�sn��7)����qg�5+��s)��5/�t~�5+��5��3x�qb�bf��5:��)��vg�vf�e��5:��)��xm�5+��5��3b�xy�5+��s)��5/��rj��e)��')��5/�en�vf��7)��5��");
	
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
	m_pMirror = pMatSys->CreateNamedRenderTargetTextureEx(/*mechanics_mirr*/CXorString("zn�ve�dT�ey").ToCharArray(), MIRROR_WIDTH, MIRROR_HEIGHT, RT_SIZE_DEFAULT, IMAGE_FORMAT_RGBA8888);
	pMatSys->EndRenderTargetAllocation();
	*pInitialized = bOrig;

	if (m_pMirror)
	{
		// Prevent it from getting cleaned up
		m_pMirror->IncrementReferenceCount();
		
		CXorString xorMatMechanicsMirr("zj�zn�ve�dT�ey"); // mat_mechanics_mirr
		KeyValues* pKeyValues;
		pKeyValues = (KeyValues*)malloc(sizeof(KeyValues));
		m_pApp->InitKeyValues()(pKeyValues, m_xorUnlitGeneric.ToCharArray());
		m_pApp->LoadFromBuffer()(pKeyValues, xorMatMechanicsMirr.ToCharArray(), /*\"UnlitGeneric\"\n{\n\t\"$basetexture\" \"mechanics_mirr\"\n}*/CXorString("5^�~§yn��t)��\x1d\x2��uj��cn��by��7)�tc�~h��zb��5\x1�").ToCharArray(), NULL, NULL, NULL);

		m_pMatMirror = m_pApp->MaterialSystem()->CreateMaterial(xorMatMechanicsMirr.ToCharArray(), pKeyValues);
		m_pMatMirror->IncrementReferenceCount();
	}
	
}

void CResourceManager::BuildFadeTexture()
{
	constexpr int iHeight = 360;

	int curR, curG, curB;
	unsigned char pTexColorFade[iHeight * 4];
	for (int y = 0; y < iHeight * 4; y += 4)
	{
		Utils::HsvToRgb(y, 1.0f, 1.0f, curR, curG, curB);

		pTexColorFade[y] = curR;
		pTexColorFade[y + 1] = curG;
		pTexColorFade[y + 2] = curB;
		pTexColorFade[y + 3] = 255; // Alpha
	}

	m_pApp->Surface()->DrawSetTextureRGBA(this->GetTexture(RM_TEXTURE_COLORFADE), pTexColorFade, 1, iHeight);
}

void CResourceManager::BuildSaturationLightnessTexture(int iHue)
{
	constexpr int iWidth = 100;
	constexpr int iHeight = 100;

	int idx;
	int curR, curG, curB;
	unsigned char pTexture[iWidth * iHeight * 4];
	for (int y = 0; y < iHeight; y++)
	{
		for (int x = 0; x < iWidth; x++)
		{
			Utils::HsvToRgb(iHue, (x + 1) / (float)iWidth, 1.0f - ((y + 1) / (float)iHeight), curR, curG, curB);

			idx = (y * iWidth + x) * 4;
			pTexture[idx++] = curR;
			pTexture[idx++] = curG;
			pTexture[idx++] = curB;
			pTexture[idx++] = 255; // Alpha
		}
	}

	m_pApp->Surface()->DrawSetTextureRGBA(this->GetTexture(RM_TEXTURE_SLFADE), pTexture, iWidth, iHeight);
}

void CResourceManager::CreateTextures()
{
	ISurface* pSurface = m_pApp->Surface();

	int textureColorFade = pSurface->CreateNewTextureID(true);
	int textureSLFade = pSurface->CreateNewTextureID(true);

	m_iTextureWhite = pSurface->DrawGetTextureId(/*vgui/white*/CXorString("al�8|�cn"));
	m_mapTextures[RM_TEXTURE_COLORFADE] = textureColorFade;
	m_mapTextures[RM_TEXTURE_SLFADE] = textureSLFade;
	m_mapTextures[RM_TEXTURE_BACKGROUND] = this->LoadPngToTexture(pSurface, "background.png");

	this->BuildFadeTexture();
	this->BuildSaturationLightnessTexture(0);
}

int CResourceManager::GetTexture(int textureId)
{
	return m_mapTextures[textureId];
}

void CResourceManager::CreateFonts()
{
	static CXorString xorTahoma("Cj�zj");

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

int CResourceManager::LoadPngToTexture(ISurface* pSurface, const char* pFilename)
{
	// TODO:
	// <temp>
	CBenchmark benchmark(true);
	// </temp>

	// PNG Data
	std::vector<unsigned char> vRawImage; //the raw pixels
	unsigned iWidth, iHeight;
	char pPath[MAX_PATH];

	// Make full path
	sprintf(pPath, "%smedia\\%s", m_pApp->GetWorkingDirectory(), pFilename);

	// Decode PNG
	unsigned int iErr = lodepng::decode(vRawImage, iWidth, iHeight, pPath);
	if (iErr)
	{
#ifdef _DEBUG
		g_pConsole->Write(LOGLEVEL_ERROR, "Loading png '%s': %s\n", pFilename, lodepng_error_text(iErr));
#endif // _DEBUG

		return -1;
	}


	// TODO: Maybe improve performance somehow ^-^
	unsigned int dim = iWidth * iHeight * 4;
	unsigned char* pImgData = new unsigned char[dim];
	for (unsigned int i = 0; i < dim; i++)
	{
		pImgData[i] = vRawImage[i];
	}

	int textureId = pSurface->CreateNewTextureID(true);
	pSurface->DrawSetTextureRGBA(textureId, pImgData, iWidth, iHeight);

	delete[] pImgData;

	// TODO:
	// <temp>
	benchmark.FinishBenchmark();
	benchmark.PrintBenchmark("Create Texture from *.png");
	// </temp>

	return textureId;
}
