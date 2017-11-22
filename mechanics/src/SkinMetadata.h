#ifndef __SKINMETADATA_H__
#define __SKINMETADATA_H__

#include <cstring>

class CSkinMetadata
{
public:
	CSkinMetadata(int itemIdx = -1, int paintKit = 0, int seed = 0, int statTrak = -1, int entityQuality = 4, const char* pCustomName = nullptr, float wear = 0.01f);
	~CSkinMetadata();

	void SetCustomName(const char* pCustomName);
	const char* GetCustomName() { return m_pCustomName; }

	int m_iItemDefinitionIndex;
	int m_iFallbackPaintKit;
	int m_iFallbackSeed;
	int m_iFallbackStatTrak;
	int m_iEntityQuality;
	char* m_pCustomName;
	float m_fFallbackWear;
};

#endif // __SKINMETADATA_H__