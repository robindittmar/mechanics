#include "ResourceManager.h"
#include "Application.h"

CResourceManager* g_pResourceManager;

CResourceManager::CResourceManager() :
	m_xorVertexLitGeneric(/*VertexLitGeneric*/"\x41\x6E\xF7\xB6\x72\x73\xC9\xAB\x63\x4C\xE0\xAC\x72\x79\xEC\xA1"),
	m_xorUnlitGeneric(/*UnlitGeneric*/"\x42\x65\xE9\xAB\x63\x4C\xE0\xAC\x72\x79\xEC\xA1"),
	m_xorMatName(/*mechmat_%d.vmt*/"\x7A\x6E\xE6\xAA\x7A\x6A\xF1\x9D\x32\x6F\xAB\xB4\x7A\x7F")
{
	m_iMaterialCount = 0;
	m_pMirror = nullptr;
}

CResourceManager::~CResourceManager()
{
	if (m_pMatMirror)
		m_pMatMirror->DecrementReferenceCount();

	if (m_pMirror)
		m_pMirror->DecrementReferenceCount();

	ISurface* pSurface = m_pApp->Surface();
	for(auto& m_mapTexture : m_mapTextures)
	{
		pSurface->DeleteTextureByID(m_mapTexture.second);
	}
}

void CResourceManager::Init(CApplication* pApp)
{
	m_pApp = pApp;
}

IMaterial* CResourceManager::CreateMaterial(bool bIsLit, bool bIsFlat, bool bIgnoreZ, bool bWireframe)
{
	// TODO: Try to set envmap in chams material
	//		 (this one below should be it; behaves weird tho right now)
	//static CXorString xorVmt("5.��.p.�3i�r.�c~��5+��p~��`c�rT�sb�an��5/�af�.5+��y}ڡbi�v{��5/�.sn��7)��.)��{j��7)�5.��yd�{g��5;��5/��{m�{~��7)��.)��vg�vf�e.��5:��5/�.qd��7)��.)��pe�rq��5.��.)��yn�ry��5;��5/�en�vf��7)��5.�.��������c<Pj6m..�.K.�AK");
	
	// vgui/white_additive
	//static CXorString xorVmt("5.��p��5/�dn�o�r)��al�8|�cnڣso�~}����re�v{��5)��5/�sn��7)����qg�5+��s)��5/�t~�5+��5��3x�qb�bf��5:��)��vg�vf�e��5:��)��xm�5+��5��3b�xy�5+��s)��5/��rj��e)��')��5/�en�vf��7)��5��");
	// vgui/white
	/// static CXorString xorVmt("5.��p��5/�dn�o�r)��al�8|�cn��)��y}�g)��5��3f�rg��5:��)��{j��7)��5��3e�bg��7)����dn�~g�z)��&)��5/�{m�zi�c)��&)��5/�qd��7)����~l�en��7)��5��3q�vy�5+��5��3|�rm��zn��5.��v");
    static CXorString xorVmt(/*vgui/white*/"\x61\x6C\xF0\xAB\x38\x7C\xED\xAB\x63\x6E");

	const char* pBaseType = (bIsLit ? m_xorVertexLitGeneric.ToCharArray() : m_xorUnlitGeneric.ToCharArray());
	char pMaterial[1024];
	char pName[512];
	KeyValues* pKeyValues;

	sprintf(pMaterial, xorVmt.ToCharArray(), pBaseType, (bIsFlat ? 1 : 0), (bIgnoreZ ? 1 : 0), (bWireframe ? 1 : 0));
	sprintf(pName, m_xorMatName.ToCharArray(), m_iMaterialCount++);

	pKeyValues = (KeyValues*)malloc(sizeof(KeyValues));
	m_pApp->InitKeyValues()(pKeyValues, pBaseType);
	m_pApp->LoadFromBuffer()(pKeyValues, pName, pMaterial, nullptr, nullptr, nullptr, nullptr);

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
	m_pMirror = pMatSys->CreateNamedRenderTargetTextureEx(/*mechanics_mirr*/CXorString("\x7A\x6E\xE6\xAA\x76\x65\xEC\xA1\x64\x54\xE8\xAB\x65\x79").ToCharArray(), MIRROR_WIDTH, MIRROR_HEIGHT, RT_SIZE_DEFAULT, IMAGE_FORMAT_RGBA8888);
	pMatSys->EndRenderTargetAllocation();
	*pInitialized = bOrig;

	if (m_pMirror)
	{
		// Prevent it from getting cleaned up
		m_pMirror->IncrementReferenceCount();
		
		CXorString xorMatMechanicsMirr(/*mat_mechanics_mirr*/"\x7A\x6A\xF1\x9D\x7A\x6E\xE6\xAA\x76\x65\xEC\xA1\x64\x54\xE8\xAB\x65\x79");
		KeyValues* pKeyValues;
		pKeyValues = (KeyValues*)malloc(sizeof(KeyValues));
		m_pApp->InitKeyValues()(pKeyValues, m_xorUnlitGeneric.ToCharArray());
		m_pApp->LoadFromBuffer()(pKeyValues, xorMatMechanicsMirr.ToCharArray(),
		        /*\"UnlitGeneric\"\n{\n\t\"$basetexture\" \"mechanics_mirr\"\n}*/CXorString("\x4B\x29\xD0\xAC\x7B\x62\xF1\x85\x72\x65\xE0\xB0\x7E\x68\xD9\xE0\x4B\x65\xFE\x9E\x79\x57\xF1\x9E\x35\x2F\xE7\xA3\x64\x6E\xF1\xA7\x6F\x7F\xF0\xB0\x72\x57\xA7\xE2\x4B\x29\xE8\xA7\x74\x63\xE4\xAC\x7E\x68\xF6\x9D\x7A\x62\xF7\xB0\x4B\x29\xD9\xAC\x6A").ToCharArray()
		        , nullptr, nullptr, nullptr, nullptr);

		m_pMatMirror = m_pApp->MaterialSystem()->CreateMaterial(xorMatMechanicsMirr.ToCharArray(), pKeyValues);
		m_pMatMirror->IncrementReferenceCount();
	}
	
}

void CResourceManager::BuildFadeTexture()
{
	constexpr int iWidth = 1;
	constexpr int iHeight = 360;

	int idx;
	int curR, curG, curB;
	unsigned char pTexColorFade[iWidth * iHeight * 4];
	for (int y = 0; y < iHeight; y++)
	{
		for (int x = 0; x < iWidth; x++)
		{
			idx = (y * iWidth + x) * 4;

			Utils::HsvToRgb(y, 1.0f, 1.0f, curR, curG, curB);

			pTexColorFade[idx++] = curR;
			pTexColorFade[idx++] = curG;
			pTexColorFade[idx++] = curB;
			pTexColorFade[idx++] = 255; // Alpha
		}
	}

	m_pApp->Surface()->DrawSetTextureRGBA(this->GetTexture(RM_TEXTURE_COLORFADE), pTexColorFade, iWidth, iHeight);
}

void CResourceManager::BuildSaturationValueTexture(int iHue)
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

	m_pApp->Surface()->DrawSetTextureRGBA(this->GetTexture(RM_TEXTURE_SVFADE), pTexture, iWidth, iHeight);
}

void CResourceManager::CreateTextures()
{
	ISurface* pSurface = m_pApp->Surface();

	int textureColorFade = pSurface->CreateNewTextureID(true);
	int textureSLFade = pSurface->CreateNewTextureID(true);

	m_iTextureWhite = pSurface->DrawGetTextureId(/*vgui/white*/CXorString("\x61\x6C\xF0\xAB\x38\x7C\xED\xAB\x63\x6E"));
	m_mapTextures[RM_TEXTURE_COLORFADE] = textureColorFade;
	m_mapTextures[RM_TEXTURE_SVFADE] = textureSLFade;
	m_mapTextures[RM_TEXTURE_BACKGROUND] = this->LoadPngToTexture(pSurface, "background.png");

	this->BuildFadeTexture();
	this->BuildSaturationValueTexture(0);
}

int CResourceManager::GetTexture(int textureId)
{
	return m_mapTextures[textureId];
}

void CResourceManager::CreateFonts()
{
	static CXorString xorTahoma(/*Tahoma*/"\x43\x6A\xED\xAD\x7A\x6A");

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
