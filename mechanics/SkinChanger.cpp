#include "Skinchanger.h"
#include "Application.h"

CSkinChanger::CSkinChanger()
{
	m_bForceFullUpdate = true;
	m_iDesiredKnifeModelIndex = 0;
}

CSkinChanger::~CSkinChanger()
{
	this->ClearReplacements();
}

void CSkinChanger::Setup()
{
	m_pApp = CApplication::Instance();

	static CXorString xorKnifeCT("zdá§{xªµrjõ­yxª´H`ë«qnÚ¦rmä·{Ú¡c%è¦{");
	static CXorString xorKnifeT("zdá§{xªµrjõ­yxª´H`ë«qnÚ¦rmä·{Ú¶9fá®");
	static CXorString xorKnifeBayonet("zdá§{xªµrjõ­yxª´H`ë«qnÚ vrê¬r«¯sg");
	static CXorString xorKnifeButterfly("zdá§{xªµrjõ­yxª´H`ë«qnÚ bñ§emé»9fá®");
	static CXorString xorKnifeFalchion("zdá§{xªµrjõ­yxª´H`ë«qnÚ¤vgæª~dëvoó£yhà¦9fá®");
	static CXorString xorKnifeFlip("zdá§{xªµrjõ­yxª´H`ë«qnÚ¤{bõìzoé");
	static CXorString xorKnifeGut("zdá§{xªµrjõ­yxª´H`ë«qnÚ¥b«¯sg");
	static CXorString xorKnifeM9Bayonet("zdá§{xªµrjõ­yxª´H`ë«qnÚ¯.Tç£n%è¦{");
	static CXorString xorKnifeKarambit("zdá§{xªµrjõ­yxª´H`ë«qnÚ©vyä¯9fá®");
	static CXorString xorKnifePush("zdá§{xªµrjõ­yxª´H`ë«qnÚ²bxíìzoé");
	static CXorString xorKnifeSurvivalBowie("zdá§{xªµrjõ­yxª´H`ë«qnÚ±byó«ajéudò«r%è¦{");
	static CXorString xorKnifeTactical("zdá§{xªµrjõ­yxª´H`ë«qnÚ¶vhñ«tjéìzoé");

	this->m_mapKnives[WEAPON_KNIFE] = xorKnifeCT.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_T] = xorKnifeT.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_BAYONET] = xorKnifeBayonet.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_BUTTERFLY] = xorKnifeButterfly.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_FALCHION] = xorKnifeFalchion.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_FLIP] = xorKnifeFlip.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_GUT] = xorKnifeGut.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_M9_BAYONET] = xorKnifeM9Bayonet.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_KARAMBIT] = xorKnifeKarambit.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_PUSH] = xorKnifePush.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_SURVIVAL_BOWIE] = xorKnifeSurvivalBowie.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_TACTICAL] = xorKnifeTactical.ToCharArray();
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
	this->DeleteKillIcons();

	m_mapModelMetadata.clear();
	m_mapSkinMetadata.clear();
	m_mapKillIcon.clear();
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

void CSkinChanger::AddKillIconReplacement(const char* pOld, const char* pNew)
{
	uint32_t iHash = murmurhash(pOld, strlen(pOld), 0xB16B00B5);

	int iLen = strlen(pNew) + 1;
	char* pNewWeap = new char[iLen];
	memcpy(pNewWeap, pNew, iLen);

	m_mapKillIcon[iHash] = pNewWeap;
}

bool CSkinChanger::ApplyCustomModel(IClientEntity* pLocal, CBaseAttributableItem* pItem)
{
	CBaseViewModel* pViewModel = pLocal->GetViewModel();
	CBaseAttributableItem* pWorldModel = (CBaseAttributableItem*)m_pApp->EntityList()->GetClientEntityFromHandle(pItem->GetWorldModel());

	// No viewmodel :s
	if (!pViewModel)
		return false;

	// No worldmodel :s
	if (!pWorldModel)
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
	pWorldModel->SetModelIndex(iNewModelIdx + 1);
	pViewModel->SetModelIndex(iNewModelIdx);

	if (m_bForceFullUpdate)
	{
		m_bForceFullUpdate = false;
		m_pApp->ClientState()->ForceFullUpdate();
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

bool CSkinChanger::ApplyCustomKillIcon(IGameEvent* pEvent)
{
	if (!m_bIsEnabled)
		return false;

	static CXorString xorAttacker("vñ£t`à°");
	static CXorString xorPlayerDeath("ggä»ryÚ¦rjñª");
	static CXorString xorWeapon("`nä²xe");

	// Check if event is valid
	if (!pEvent)
		return false;

	// Check if we are in player_death event
	if (strcmp(pEvent->GetName(), xorPlayerDeath.ToCharArray()))
		return false;

	// Check if attacker is us :)
	IVEngineClient* pEngine = m_pApp->EngineClient();
	if (pEngine->GetPlayerForUserID(pEvent->GetInt(xorAttacker.ToCharArray())) != pEngine->GetLocalPlayer())
		return false;

	// Check if we have a replacement weapon
	const char* pWeapon = pEvent->GetString(xorWeapon.ToCharArray());
	uint32_t iHash = murmurhash(pWeapon, strlen(pWeapon), 0xB16B00B5);
	if (m_mapKillIcon.find(iHash) == m_mapKillIcon.end())
		return false;

	// Override weapon
	pEvent->SetString(xorWeapon.ToCharArray(), m_mapKillIcon[iHash]);
	return true;
}

void CSkinChanger::ApplyDesiredKnife(int iDesiredKnifeModelIndex)
{
	if (m_iDesiredKnifeModelIndex == iDesiredKnifeModelIndex)
		return;

	m_iDesiredKnifeModelIndex = iDesiredKnifeModelIndex;

	this->DeleteModelNames();
	this->DeleteKillIcons();
	if (iDesiredKnifeModelIndex == 0)
	{
		m_pApp->ClientState()->ForceFullUpdate();
		return;
	}


	const char* pDesiredKnife = this->m_mapKnives[iDesiredKnifeModelIndex];

	for (std::unordered_map<int, const char*>::iterator it = m_mapKnives.begin(); it != m_mapKnives.end(); it++)
	{
		if (it->first == m_iDesiredKnifeModelIndex)
			continue;

		AddModelReplacement(
			it->second,
			pDesiredKnife
		);

		// todo: also skin!!
		/*AddKillIconReplacement(
			CXorString("|eì¤rTèûHiä»xeà¶").ToCharArray(),
			CXorString("ujü­ynñ").ToCharArray()
		);*/
	}


	m_bForceFullUpdate = true;
}

void CSkinChanger::DeleteModelNames()
{
	// Clean up Model names
	const char* pCurrent;
	for (std::unordered_map<int, const char*>::iterator it = m_mapModelMetadata.begin(); it != m_mapModelMetadata.end(); it++)
	{
		pCurrent = it->second;

		if (pCurrent)
			delete[] pCurrent;
	}

	m_mapModelMetadata.clear();
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

	m_mapSkinMetadata.clear();
}

void CSkinChanger::DeleteKillIcons()
{
	const char* pCurrent;
	for (std::unordered_map<uint32_t, const char*>::iterator it = m_mapKillIcon.begin(); it != m_mapKillIcon.end(); it++)
	{
		pCurrent = it->second;

		if (pCurrent)
			delete[] pCurrent;
	}

	m_mapKillIcon.clear();
}
