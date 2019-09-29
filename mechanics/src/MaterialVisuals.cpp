#include "MaterialVisuals.h"
#include "Application.h"

CMaterialVisuals::CMaterialVisuals()
	: m_fNightmodeValue(100.0f), m_fAsuswallsValue(100.0f), m_iSkychangerValue(SKY_NOCHANGE)
{
}

CMaterialVisuals::~CMaterialVisuals()
{
}

void CMaterialVisuals::SetDefaultSky()
{
	// TODO: Xor
	static ConVar* pSkyname = m_pApp->CVar()->FindVar("sv_skyname");
	memcpy(m_pDefaultSky, pSkyname->value, (strlen(pSkyname->value) + 1));
}

void CMaterialVisuals::Nightmode(float fValue)
{
	static CXorString xorSkyname(/*sv_skyname*/"\x64\x7D\xDA\xB1\x7C\x72\xEB\xA3\x7A\x6E");
	static CXorString xorDrawSpecificStaticProp(/*r_DrawSpecificStaticProp*/"\x64\x7D\xDA\xB1\x7C\x72\xEB\xA3\x7A\x6E");
	static CXorString xorWorld(/*World*/"\x40\x64\xF7\xAE\x73");
	static CXorString xorStaticProp(/*StaticProp*/"\x44\x7F\xE4\xB6\x7E\x68\xD5\xB0\x78\x7B");
	static CXorString xorSkyNight(/*sky_csgo_night02*/"\x64\x60\xFC\x9D\x74\x78\xE2\xAD\x48\x65\xEC\xA5\x7F\x7F\xB5\xF0");

	//static ConVar* r_drawspecificstaticprop = m_pApp->CVar()->FindVar(xorDrawSpecificStaticProp.ToCharArray());
	//r_drawspecificstaticprop->SetValue(0); // needed?
	float fEnd = fValue / 100.0f;
	for (auto i = m_pApp->MaterialSystem()->FirstMaterial(); i != m_pApp->MaterialSystem()->InvalidMaterial(); i = m_pApp->MaterialSystem()->NextMaterial(i))
	{
		IMaterial* pMaterial = m_pApp->MaterialSystem()->GetMaterial(i);

		if (!pMaterial || pMaterial->IsErrorMaterial())
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), xorWorld.ToCharArray()) || strstr(pMaterial->GetTextureGroupName(), xorStaticProp.ToCharArray()))
		{
			pMaterial->ColorModulate(fEnd, fEnd, fEnd);
		}
	}
}

void CMaterialVisuals::Asuswalls(float fValue)
{
	static CXorString xorWorld("\x40\x64\xF7\xAE\x73");

	m_fAsuswallsValue = fValue;

	float fEnd = fValue / 100.0f;
	for (auto i = m_pApp->MaterialSystem()->FirstMaterial(); i != m_pApp->MaterialSystem()->InvalidMaterial(); i = m_pApp->MaterialSystem()->NextMaterial(i))
	{
		IMaterial* pMaterial = m_pApp->MaterialSystem()->GetMaterial(i);

		if (!pMaterial || pMaterial->IsErrorMaterial())
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), xorWorld.ToCharArray()))
		{
			if (strstr(pMaterial->GetName(), "floor"))
			{
				continue;
			}

			pMaterial->AlphaModulate(fEnd);
		}
	}
}

void CMaterialVisuals::Skychanger(int iValue)
{
	m_iSkychangerValue = iValue;
	switch (m_iSkychangerValue)
	{
	case 19:
		m_pLoadSky("vietnam");
		break;
	case 18:
		m_pLoadSky("sky_dust");
		break;
	case 17:
		m_pLoadSky("vertigoblue_hdr");
		break;
	case 16:
		m_pLoadSky("vertigo");
		break;
	case 15:
		m_pLoadSky("sky_csgo_night02b");
		break;
	case 14:
		m_pLoadSky("sky_csgo_night02");
		break;
	case 13:
		m_pLoadSky("sky_csgo_cloudy01");
		break;
	case 12:
		m_pLoadSky("sky_venice");
		break;
	case 11:
		m_pLoadSky("nukeblank");
		break;
	case 10:
		m_pLoadSky("sky_cs15_daylight04_hdr");
		break;
	case 9:
		m_pLoadSky("sky_cs15_daylight03_hdr");
		break;
	case 8:
		m_pLoadSky("sky_cs15_daylight02_hdr");
		break;
	case 7:
		m_pLoadSky("sky_cs15_daylight01_hdr");
		break;
	case 6:
		m_pLoadSky("office");
		break;
	case 5:
		m_pLoadSky("jungle");
		break;
	case 4:
		m_pLoadSky("italy");
		break;
	case 3:
		m_pLoadSky("embassy");
		break;
	case 2:
		m_pLoadSky("cs_tibet");
		break;
	case 1:
		m_pLoadSky("cs_baggage_skybox_");
		break;
	case 0:
	default:
		m_pLoadSky(m_pDefaultSky);
		break;
	}
}

void CMaterialVisuals::Setup()
{
	IFeature::Setup();

	m_pLoadSky = (LoadSky_t)CPattern::FindPattern(
		(BYTE*)m_pApp->EngineDll(),
		ENGINEDLL_SIZE,
		(BYTE*)"\x55\x8B\xEC\x81\xEC\x34\x01\x00\x00\x56\x57\x8B\xF9",
		"123456789abcd"
	);

	if (m_pApp->EngineClient()->IsInGame())
	{
		this->SetDefaultSky();
	}
}

void CMaterialVisuals::Think(void* pParameters)
{
}
