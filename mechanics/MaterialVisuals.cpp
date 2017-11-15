#include "MaterialVisuals.h"
#include "Application.h"

CMaterialVisuals::CMaterialVisuals()
{
}

CMaterialVisuals::~CMaterialVisuals()
{
}

void CMaterialVisuals::Nightmode(float fValue)
{
}

void CMaterialVisuals::Asuswalls(float fValue)
{
	static CXorString xorWorld("@d÷®s");

	m_fAsuswallsValue = fValue;

	for (auto i = m_pApp->MaterialSystem()->FirstMaterial(); i != m_pApp->MaterialSystem()->InvalidMaterial(); i = m_pApp->MaterialSystem()->NextMaterial(i))
	{
		IMaterial* pMaterial = m_pApp->MaterialSystem()->GetMaterial(i);

		if (!pMaterial || pMaterial->IsErrorMaterial())
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), xorWorld.ToCharArray()))
		{
			pMaterial->AlphaModulate((fValue / 100.0f));
		}
	}
}

void CMaterialVisuals::Update(void* pParameters)
{
}
