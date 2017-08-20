#include "ResourceManager.h"
#include "Application.h"

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
	static CXorString xorVmt("5.öàpË5/ç£dnñ§oð°r)¥àalð«8|í«cnÚ£soì¶~}àà§æreó¯v{§â5)Ë5/è­snéà7)´à§æqgä¶5+§çs)Ë5/ë­t~é®5+§ò5Œà3xà®qbé®bf§â5:§È)¡ªvgã®vfç§e§â5:§È)¡¬xmê¥5+§ò5Œà3bâ¬xyà¸5+§çs)Ë5/ÿ¬rj÷§e)¥à')Ë5/ò«enã°vfàà7) ¦5øÈ");
	
	static CXorString xorVertexLitGeneric("An÷¶rsÉ«cLà¬ryì¡");
	static CXorString xorUnlitGeneric("Beé«cLà¬ryì¡");
	static CXorString xorMatName("ocè£cT ¦9}è¶");

	const char* pBaseType = (bIsLit == true ? xorVertexLitGeneric.ToCharArray() : xorUnlitGeneric.ToCharArray());
	char pMaterial[1024];
	char pName[512];
	KeyValues* pKeyValues;

	sprintf(pMaterial, xorVmt.ToCharArray(), pBaseType, (bIsFlat ? 1 : 0), (bIgnoreZ ? 1 : 0), (bWireframe ? 1 : 0));
	sprintf(pName, xorMatName.ToCharArray(), m_iMaterialCount++);

	pKeyValues = (KeyValues*)malloc(sizeof(KeyValues));
	m_pApp->InitKeyValues()(pKeyValues, pBaseType);
	// TODO: Idk
	//m_pApp->LoadFromBuffer()(pKeyValues, pName, pMaterial, NULL, NULL, NULL);

	IMaterial* pMat = m_pApp->MaterialSystem()->CreateMaterial(pName, pKeyValues);
	pMat->IncrementReferenceCount();

	return pMat;
}