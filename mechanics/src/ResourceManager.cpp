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
    static CXorString xorVmt(/*"%s"\n{\n\t"$basetexture" "vgui/white"\n\t"$envmap" ""\n\t"$model" "1"\n\t"$flat" "%d"\n\t"$nocull" "0"\n\t"$selfillum" "1"\n\t"$halflambert" "1"\n\t"$nofog" "0"\n\t"$ignorez" "%d"\n\t"$znearer" "0"\n\t"$wireframe" "%d"\n}*/
    "\x35\x2E\xF6\xE0\x1D\x70\x8F\xCB\x35\x2F\xE7\xA3\x64\x6E\xF1\xA7\x6F\x7F\xF0\xB0\x72\x29\xA5\xE0\x61\x6C\xF0\xAB\x38\x7C\xED\xAB\x63\x6E\xA7\xC8\x1E\x29\xA1\xA7\x79\x7D\xE8\xA3\x67\x29\xA5\xE0\x35\x01\x8C\xE0\x33\x66\xEA\xA6\x72\x67\xA7\xE2\x35\x3A\xA7\xC8\x1E\x29\xA1\xA4\x7B\x6A\xF1\xE0\x37\x29\xA0\xA6\x35\x01\x8C\xE0\x33\x65\xEA\xA1\x62\x67\xE9\xE0\x37\x29\xB5\xE0\x1D\x02\xA7\xE6\x64\x6E\xE9\xA4\x7E\x67\xE9\xB7\x7A\x29\xA5\xE0\x26\x29\x8F\xCB\x35\x2F\xED\xA3\x7B\x6D\xE9\xA3\x7A\x69\xE0\xB0\x63\x29\xA5\xE0\x26\x29\x8F\xCB\x35\x2F\xEB\xAD\x71\x64\xE2\xE0\x37\x29\xB5\xE0\x1D\x02\xA7\xE6\x7E\x6C\xEB\xAD\x65\x6E\xFF\xE0\x37\x29\xA0\xA6\x35\x01\x8C\xE0\x33\x71\xEB\xA7\x76\x79\xE0\xB0\x35\x2B\xA7\xF2\x35\x01\x8C\xE0\x33\x7C\xEC\xB0\x72\x6D\xF7\xA3\x7A\x6E\xA7\xE2\x35\x2E\xE1\xE0\x1D\x76\x00\xFD\xFD\xFD\xFD\x00\x00\x00\x00\x00\xC6\x09\x9B\xDD\x2A\xF6\x00\x0F\x58\x47\xD5\x00\x68\xDB");

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
