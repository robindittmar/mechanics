#include "SkinMetadata.h"

CSkinMetadata::CSkinMetadata(int itemIdx, int paintKit, int seed, int statTrak, int entityQuality, const char* pCustomName, float wear)
{
	m_iItemDefinitionIndex = itemIdx;
	m_iFallbackPaintKit = paintKit;
	m_iFallbackSeed = seed;
	m_iFallbackStatTrak = statTrak;
	m_iEntityQuality = entityQuality;
	m_pCustomName = nullptr;
	m_fFallbackWear = wear;

	if(pCustomName)
	{
		this->SetCustomName(pCustomName);
	}
}

CSkinMetadata::~CSkinMetadata()
{
	if (m_pCustomName)
		delete[] m_pCustomName;
}

void CSkinMetadata::SetCustomName(const char* pCustomName)
{
	if (m_pCustomName)
		delete[] m_pCustomName;

	int iLen = strlen(pCustomName) + 1;
	m_pCustomName = new char[iLen];
	memcpy(m_pCustomName, pCustomName, iLen);
}