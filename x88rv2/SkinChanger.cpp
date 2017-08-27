#include "Skinchanger.h"
#include "Application.h"

CSkinChanger::CSkinChanger()
{
	m_pKnifeModel = NULL;
}

CSkinChanger::~CSkinChanger()
{
	if (m_pKnifeModel)
		delete[] m_pKnifeModel;
}

void CSkinChanger::SetKnifeModel(const char* pModel)
{
	if (m_pKnifeModel)
		delete[] m_pKnifeModel;

	int iLen = strlen(pModel);
	m_pKnifeModel = new char[iLen + 1];
	memcpy(m_pKnifeModel, pModel, iLen + 1);
}

void CSkinChanger::Setup()
{
	m_pApp = CApplication::Instance();
	//int defaultCtKnife = m_pApp->ModelInfo()->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	this->m_mapModelCfg[WEAPON_KNIFE] = "models/weapons/v_knife_default_ct.mdl";

	//int defaultTKnife = m_pApp->ModelInfo()->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	this->m_mapModelCfg[WEAPON_KNIFE_T] = "models/weapons/v_knife_default_t.mdl";

	this->m_mapModelCfg[WEAPON_KNIFE_BAYONET] = "models/weapons/v_knife_bayonet.mdl";
	this->m_mapModelCfg[WEAPON_KNIFE_BUTTERFLY] = "models/weapons/v_knife_butterfly.mdl";
	this->m_mapModelCfg[WEAPON_KNIFE_FALCHION] = "models/weapons/v_knife_falchion_advanced.mdl";
	this->m_mapModelCfg[WEAPON_KNIFE_FLIP] = "models/weapons/v_knife_flip.mdl";
	this->m_mapModelCfg[WEAPON_KNIFE_GUT] = "models/weapons/v_knife_gut.mdl";

	//int karambitKnife = m_pApp->ModelInfo()->GetModelIndex("models/weapons/v_knife_karam.mdl");
	this->m_mapModelCfg[WEAPON_KNIFE_KARAMBIT] = "models/weapons/v_knife_karam.mdl";

	//int m9Bayonet = m_pApp->ModelInfo()->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	this->m_mapModelCfg[WEAPON_KNIFE_M9_BAYONET] = "models/weapons/v_knife_karam.mdl";
	this->m_mapModelCfg[WEAPON_KNIFE_PUSH] = "models/weapons/v_knife_push.mdl";
	this->m_mapModelCfg[WEAPON_KNIFE_SURVIVAL_BOWIE] = "models/weapons/v_knife_survival_bowie.mdl";
	this->m_mapModelCfg[WEAPON_KNIFE_TACTICAL] = "models/weapons/v_knife_tactical.mdl";
}

void CSkinChanger::Update(void* pParameters)
{
	IClientEntity* pLocalEntity = (IClientEntity*)pParameters;

	PlayerInfo pLocalInfo;
	m_pApp->EngineClient()->GetPlayerInfo(pLocalEntity->EntIndex(), &pLocalInfo);

	CBaseAttributableItem* pWeapon = (CBaseAttributableItem*)pLocalEntity->GetActiveWeapon();
	if (pWeapon)
	{
		int wepIndex = *pWeapon->GetItemDefinitionIndex();

		//todo: animation fix, skin not being applied
		//pApp->SkinChanger()->ApplyCustomModel(pLocalEntity, pWeapon, wepIndex);
		if (pLocalInfo.xuidlow == *pWeapon->GetOriginalOwnerXuidLow()) //todo: skin only mine
		{
			m_pApp->SkinChanger()->ApplyCustomSkin(pWeapon, wepIndex);

			*pWeapon->GetAccountID() = pLocalInfo.xuidlow;
		}
	}
}





bool CSkinChanger::ApplyCustomModel(IClientEntity* pLocal, CBaseAttributableItem* pWeapon, int modelIndex)
{
	if (!m_bIsEnabled)
		return false;

	CWeapon* temp = (CWeapon*)pWeapon;
	if (temp->IsKnife())
	{
		CBaseViewModel* pViewModel = (CBaseViewModel*)CApplication::Instance()->EntityList()->GetClientEntityFromHandle(*(HANDLE*)((DWORD)pLocal + 0x32DC)); // local -> m_hViewModel

		if (!pViewModel)
			return false;

		pViewModel->SetWeaponModel(m_pKnifeModel, pWeapon);
	}

	return true;
}

bool CSkinChanger::ApplyCustomSkin(CBaseAttributableItem* pWeapon, int wepIndex)
{
	if (!m_bIsEnabled)
		return false;

	if (this->m_mapSkinChangerCfg.find(wepIndex) == this->m_mapSkinChangerCfg.end())
		return false;

	*pWeapon->GetFallbackPaintKit() = this->m_mapSkinChangerCfg[wepIndex].m_iFallbackPaintKit;
	*pWeapon->GetEntityQuality() = this->m_mapSkinChangerCfg[wepIndex].m_iEntityQuality; // quality, red, blue, etc.. 4/11 same as 0, 3 knife with star
	*pWeapon->GetFallbackSeed() = this->m_mapSkinChangerCfg[wepIndex].m_iFallbackSeed;
	*pWeapon->GetFallbackStatTrak() = -1; //-1 = nonst, > 0 = statcount
	*pWeapon->GetFallbackWear() = this->m_mapSkinChangerCfg[wepIndex].m_flFallbackWear; // standard, best
	if (this->m_mapSkinChangerCfg[wepIndex].m_iItemDefinitionIndex != -1)
		*pWeapon->GetItemDefinitionIndex() = this->m_mapSkinChangerCfg[wepIndex].m_iItemDefinitionIndex;

	if (this->m_mapSkinChangerCfg[wepIndex].m_szCustomName) {
		sprintf_s(pWeapon->GetCustomName(), 32, "%s", this->m_mapSkinChangerCfg[wepIndex].m_szCustomName);
	}

	*pWeapon->GetItemIDHigh() = 1;

	return true;
}