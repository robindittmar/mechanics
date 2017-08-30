#include "Skinchanger.h"
#include "Application.h"

CSkinChanger::CSkinChanger()
{
}

CSkinChanger::~CSkinChanger()
{
	this->DeleteModelNames();
	this->DeleteSkinMetadata();
}

void CSkinChanger::Setup()
{
	m_pApp = CApplication::Instance();

	/*
	Xord "zd�{x��rj��yx��H`�qnڦrm�{ڡc%�{"
	this->m_mapModelCfg[WEAPON_KNIFE] = "models/weapons/v_knife_default_ct.mdl";

	Xord "zd�{x��rj��yx��H`�qnڦrm�{ڶ9f�"
	this->m_mapModelCfg[WEAPON_KNIFE_T] = "models/weapons/v_knife_default_t.mdl";

	Xord "zd�{x��rj��yx��H`�qnڠvr�r��sg"
	this->m_mapModelCfg[WEAPON_KNIFE_BAYONET] = "models/weapons/v_knife_bayonet.mdl";

	Xord "zd�{x��rj��yx��H`�qnڠb�em�9f�"
	this->m_mapModelCfg[WEAPON_KNIFE_BUTTERFLY] = "models/weapons/v_knife_butterfly.mdl";

	Xord "zd�{x��rj��yx��H`�qnڤvg�~d�vo�yh�9f�"
	this->m_mapModelCfg[WEAPON_KNIFE_FALCHION] = "models/weapons/v_knife_falchion_advanced.mdl";
	
	Xord "zd�{x��rj��yx��H`�qnڤ{b��zo�"
	this->m_mapModelCfg[WEAPON_KNIFE_FLIP] = "models/weapons/v_knife_flip.mdl";

	Xord "zd�{x��rj��yx��H`�qnڥb��sg"
	this->m_mapModelCfg[WEAPON_KNIFE_GUT] = "models/weapons/v_knife_gut.mdl";

	Xord "zd�{x��rj��yx��H`�qnگ.T�n%�{"
	this->m_mapModelCfg[WEAPON_KNIFE_M9_BAYONET] = "models/weapons/v_knife_m9_bay.mdl";

	Xord "zd�{x��rj��yx��H`�qnکvy�9f�"
	this->m_mapModelCfg[WEAPON_KNIFE_KARAMBIT] = "models/weapons/v_knife_karam.mdl";

	Xord "zd�{x��rj��yx��H`�qnڲbx��zo�"
	this->m_mapModelCfg[WEAPON_KNIFE_PUSH] = "models/weapons/v_knife_push.mdl";

	Xord "zd�{x��rj��yx��H`�qnڱby�aj�ud�r%�{"
	this->m_mapModelCfg[WEAPON_KNIFE_SURVIVAL_BOWIE] = "models/weapons/v_knife_survival_bowie.mdl";

	Xord "zd�{x��rj��yx��H`�qnڶvh�tj��zo�"
	this->m_mapModelCfg[WEAPON_KNIFE_TACTICAL] = "models/weapons/v_knife_tactical.mdl";
	*/
}

void CSkinChanger::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	PlayerInfo pLocalInfo;
	IClientEntity* pLocalEntity = (IClientEntity*)pParameters;
	pLocalEntity->GetPlayerInfo(&pLocalInfo);

	CBaseAttributableItem* pWeapon;
	HANDLE* hMyWeapons = pLocalEntity->GetWeapons();

	if (!hMyWeapons)
		return;

	for (int i = 0; hMyWeapons[i]; i++)
	{
		pWeapon = (CBaseAttributableItem*)m_pApp->EntityList()->GetClientEntityFromHandle(hMyWeapons[i]);
		if (!pWeapon)
			continue;

		int iWeaponId = *pWeapon->GetItemDefinitionIndex();

		this->ApplyCustomModel(pLocalEntity, pWeapon);
		if (pLocalInfo.xuidlow != *pWeapon->GetOriginalOwnerXuidLow()) //todo: skin only mine
			continue;
		this->ApplyCustomSkin(pWeapon, iWeaponId);

		*pWeapon->GetAccountID() = pLocalInfo.xuidlow;
	}
}

void CSkinChanger::ClearReplacements()
{
	this->DeleteModelNames();
	this->DeleteSkinMetadata();

	m_mapModelMetadata.clear();
	m_mapSkinMetadata.clear();
}

void CSkinChanger::AddModelReplacement(const char* pOld, const char* pNew)
{
	int iModelId = m_pApp->ModelInfo()->GetModelIndex(pOld);

	int iLenNew = strlen(pNew) + 1;
	char* pTemp = new char[iLenNew];
	memcpy(pTemp, pNew, iLenNew);

	m_mapModelMetadata[iModelId] = pTemp;
}

void CSkinChanger::AddSkinReplacement(int iWeaponId, CSkinMetadata* pSkin)
{
	m_mapSkinMetadata[iWeaponId] = pSkin;
}

bool CSkinChanger::ApplyCustomModel(IClientEntity* pLocal, CBaseAttributableItem* pItem)
{
	CBaseViewModel* pViewModel = pLocal->GetViewModel();
	// No viewmodel :s
	if (!pViewModel)
		return false;

	int iModelIdx = pViewModel->GetModelIndex();
	// We have no mapping
	if (m_mapModelMetadata.find(iModelIdx) == m_mapModelMetadata.end())
		return false;

	// Grab Replacement model & set it
	int iNewModelIdx = m_pApp->ModelInfo()->GetModelIndex(m_mapModelMetadata[iModelIdx]);
	if (iNewModelIdx <= 0)
		return false;

	pItem->SetModelIndex(iNewModelIdx);
	pViewModel->SetModelIndex(iNewModelIdx);
	static bool test = false;
	if (!test)
	{
		m_pApp->ClientState()->ForceFullUpdate();
		test = true;
	}
	return true;
}

bool CSkinChanger::ApplyCustomSkin(CBaseAttributableItem* pWeapon, int iWeaponId)
{
	if (m_mapSkinMetadata.find(iWeaponId) == m_mapSkinMetadata.end())
		return false;

	CSkinMetadata* pSkin = this->m_mapSkinMetadata[iWeaponId];

	if (pSkin->m_iItemDefinitionIndex != -1)
	{
		*pWeapon->GetItemDefinitionIndex() = pSkin->m_iItemDefinitionIndex;
	}

	*pWeapon->GetFallbackPaintKit() = pSkin->m_iFallbackPaintKit;
	*pWeapon->GetEntityQuality() = pSkin->m_iEntityQuality;
	*pWeapon->GetFallbackSeed() = pSkin->m_iFallbackSeed;
	*pWeapon->GetFallbackStatTrak() = pSkin->m_iFallbackStatTrak;
	*pWeapon->GetFallbackWear() = pSkin->m_fFallbackWear;

	if (pSkin->m_pCustomName)
	{
		int iLen = strlen(pSkin->m_pCustomName);
		if (iLen > 32)
			pSkin->m_pCustomName[32] = '\0';

		sprintf(pWeapon->GetCustomName(), "%s", pSkin->m_pCustomName);
	}

	*pWeapon->GetItemIDHigh() = -1;
	return true;
}

void CSkinChanger::DeleteModelNames()
{
	// Clean up Model names
	const char* pCurrent;
	for (std::unordered_map<int, const char*>::iterator it = m_mapModelMetadata.begin(); it != m_mapModelMetadata.end(); it++)
	{
		pCurrent = it->second;

		if (pCurrent)
			delete pCurrent;
	}
}

void CSkinChanger::DeleteSkinMetadata()
{
	// Clean up SkinMetadata
	CSkinMetadata* pCurrent;
	for (std::unordered_map<int, CSkinMetadata*>::iterator it = m_mapSkinMetadata.begin(); it != m_mapSkinMetadata.end(); it++)
	{
		pCurrent = it->second;

		if (pCurrent)
			delete pCurrent;
	}
}