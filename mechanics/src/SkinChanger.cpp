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

	const char* pCurrent;
	for (std::unordered_map<uint32_t, const char*>::iterator it = m_mapPaintKitDescription.begin(); it != m_mapPaintKitDescription.end(); it++)
	{
		pCurrent = it->second;

		if (pCurrent)
			delete[] pCurrent;
	}

	m_mapPaintKitDescription.clear();
}

void CSkinChanger::Setup()
{
	IFeature::Setup();

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

	static CXorString xorDeagle("weapon_deagle");
	static CXorString xorElite("weapon_elite");
	static CXorString xorFiveseven("weapon_fiveseven");
	static CXorString xorGlock("weapon_glock");
	static CXorString xorAk47("weapon_ak47");
	static CXorString xorAug("weapon_aug");
	static CXorString xorAwp("weapon_awp");
	static CXorString xorFamas("weapon_famas");
	static CXorString xorG3sg1("weapon_g3sg1");
	static CXorString xorGalil("weapon_galilar");
	static CXorString xorM249("weapon_m249");
	static CXorString xorM4a1("weapon_m4a1");
	static CXorString xorMac10("weapon_mac10");
	static CXorString xorP90("weapon_p90");
	static CXorString xorUmp45("weapon_ump45");
	static CXorString xorXm1014("weapon_xm1014");
	static CXorString xorBizon("weapon_bizon");
	static CXorString xorMag7("weapon_mag7");
	static CXorString xorNegev("weapon_negev");
	static CXorString xorSawedoff("weapon_sawedoff");
	static CXorString xorTec9("weapon_tec9");
	static CXorString xorHkp2000("weapon_hkp2000");
	static CXorString xorMp7("weapon_mp7");
	static CXorString xorMp9("weapon_mp9");
	static CXorString xorNova("weapon_nova");
	static CXorString xorP250("weapon_p250");
	static CXorString xorScar20("weapon_scar20");
	static CXorString xorSg556("weapon_sg556");
	static CXorString xorSsg08("weapon_ssg08");
	static CXorString xorM4a1Silencer("weapon_m4a1_silencer");
	static CXorString xorUspSilencer("weapon_usp_silencer");
	static CXorString xorCz75("weapon_cz75a");
	static CXorString xorRevolver("weapon_revolver");

	xorDeagle.Xor();
	xorElite.Xor();
	xorFiveseven.Xor();
	xorGlock.Xor();
	xorAk47.Xor();
	xorAug.Xor();
	xorAwp.Xor();
	xorFamas.Xor();
	xorG3sg1.Xor();
	xorGalil.Xor();
	xorM249.Xor();
	xorM4a1.Xor();
	xorMac10.Xor();
	xorP90.Xor();
	xorUmp45.Xor();
	xorXm1014.Xor();
	xorBizon.Xor();
	xorMag7.Xor();
	xorNegev.Xor();
	xorSawedoff.Xor();
	xorTec9.Xor();
	xorHkp2000.Xor();
	xorMp7.Xor();
	xorMp9.Xor();
	xorNova.Xor();
	xorP250.Xor();
	xorScar20.Xor();
	xorSg556.Xor();
	xorSsg08.Xor();
	xorM4a1Silencer.Xor();
	xorUspSilencer.Xor();
	xorCz75.Xor();
	xorRevolver.Xor();

	this->m_mapWeapons[murmurhash(xorDeagle.ToCharArray(), strlen(xorDeagle.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_DEAGLE, xorDeagle.ToCharArray());
	this->m_mapWeapons[murmurhash(xorElite.ToCharArray(), strlen(xorElite.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_ELITE, xorElite.ToCharArray());
	this->m_mapWeapons[murmurhash(xorFiveseven.ToCharArray(), strlen(xorFiveseven.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_FIVESEVEN, xorFiveseven.ToCharArray());
	this->m_mapWeapons[murmurhash(xorGlock.ToCharArray(), strlen(xorGlock.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_GLOCK, xorGlock.ToCharArray());
	this->m_mapWeapons[murmurhash(xorAk47.ToCharArray(), strlen(xorAk47.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_AK47, xorAk47.ToCharArray());
	this->m_mapWeapons[murmurhash(xorAug.ToCharArray(), strlen(xorAug.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_AUG, xorAug.ToCharArray());
	this->m_mapWeapons[murmurhash(xorAwp.ToCharArray(), strlen(xorAwp.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_AWP, xorAwp.ToCharArray());
	this->m_mapWeapons[murmurhash(xorFamas.ToCharArray(), strlen(xorFamas.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_FAMAS, xorFamas.ToCharArray());
	this->m_mapWeapons[murmurhash(xorG3sg1.ToCharArray(), strlen(xorG3sg1.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_G3SG1, xorG3sg1.ToCharArray());
	this->m_mapWeapons[murmurhash(xorGalil.ToCharArray(), strlen(xorGalil.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_GALILAR, xorGalil.ToCharArray());
	this->m_mapWeapons[murmurhash(xorM249.ToCharArray(), strlen(xorM249.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_M249, xorM249.ToCharArray());
	this->m_mapWeapons[murmurhash(xorM4a1.ToCharArray(), strlen(xorM4a1.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_M4A1, xorM4a1.ToCharArray());
	this->m_mapWeapons[murmurhash(xorMac10.ToCharArray(), strlen(xorMac10.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_MAC10, xorMac10.ToCharArray());
	this->m_mapWeapons[murmurhash(xorP90.ToCharArray(), strlen(xorP90.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_P90, xorP90.ToCharArray());
	this->m_mapWeapons[murmurhash(xorUmp45.ToCharArray(), strlen(xorUmp45.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_UMP45, xorUmp45.ToCharArray());
	this->m_mapWeapons[murmurhash(xorXm1014.ToCharArray(), strlen(xorXm1014.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_XM1014, xorXm1014.ToCharArray());
	this->m_mapWeapons[murmurhash(xorBizon.ToCharArray(), strlen(xorBizon.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_BIZON, xorBizon.ToCharArray());
	this->m_mapWeapons[murmurhash(xorMag7.ToCharArray(), strlen(xorMag7.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_MAG7, xorMag7.ToCharArray());
	this->m_mapWeapons[murmurhash(xorNegev.ToCharArray(), strlen(xorNegev.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_NEGEV, xorNegev.ToCharArray());
	this->m_mapWeapons[murmurhash(xorSawedoff.ToCharArray(), strlen(xorSawedoff.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_SAWEDOFF, xorSawedoff.ToCharArray());
	this->m_mapWeapons[murmurhash(xorTec9.ToCharArray(), strlen(xorTec9.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_TEC9, xorTec9.ToCharArray());
	this->m_mapWeapons[murmurhash(xorHkp2000.ToCharArray(), strlen(xorHkp2000.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_HKP2000, xorHkp2000.ToCharArray());
	this->m_mapWeapons[murmurhash(xorMp7.ToCharArray(), strlen(xorMp7.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_MP7, xorMp7.ToCharArray());
	this->m_mapWeapons[murmurhash(xorMp9.ToCharArray(), strlen(xorMp9.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_MP9, xorMp9.ToCharArray());
	this->m_mapWeapons[murmurhash(xorNova.ToCharArray(), strlen(xorNova.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_NOVA, xorNova.ToCharArray());
	this->m_mapWeapons[murmurhash(xorP250.ToCharArray(), strlen(xorP250.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_P250, xorP250.ToCharArray());
	this->m_mapWeapons[murmurhash(xorScar20.ToCharArray(), strlen(xorScar20.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_SCAR20, xorScar20.ToCharArray());
	this->m_mapWeapons[murmurhash(xorSg556.ToCharArray(), strlen(xorSg556.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_SG556, xorSg556.ToCharArray());
	this->m_mapWeapons[murmurhash(xorSsg08.ToCharArray(), strlen(xorSsg08.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_SSG08, xorSsg08.ToCharArray());
	this->m_mapWeapons[murmurhash(xorM4a1Silencer.ToCharArray(), strlen(xorM4a1Silencer.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_M4A1_SILENCER, xorM4a1Silencer.ToCharArray());
	this->m_mapWeapons[murmurhash(xorUspSilencer.ToCharArray(), strlen(xorUspSilencer.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_USP_SILENCER, xorUspSilencer.ToCharArray());
	this->m_mapWeapons[murmurhash(xorCz75.ToCharArray(), strlen(xorCz75.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_CZ75A, xorCz75.ToCharArray());
	this->m_mapWeapons[murmurhash(xorRevolver.ToCharArray(), strlen(xorRevolver.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_REVOLVER, xorRevolver.ToCharArray());

	this->ParseSkinFile();
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

void CSkinChanger::ParseSkinFile()
{
	int iLenPath;
	char pFilepathSkins[MAX_PATH];

	if ((iLenPath = GetModuleFileName((HMODULE)m_pApp->ClientDll(), pFilepathSkins, MAX_PATH)) == 0)
		return;

	int iCountSlahes = 0;
	for (int i = iLenPath - 1; i >= 0; i--)
	{
		if (pFilepathSkins[i] == '\\')
		{
			if (++iCountSlahes >= 2)
			{
				pFilepathSkins[i + 1] = '\0';
				break;
			}
		}
	}

	strcat(pFilepathSkins, "scripts\\items\\items_game.txt");

#ifdef _DEBUG
	g_pConsole->Write(LOGLEVEL_INFO, "Parsing skin file (%s)\n", pFilepathSkins);
#endif // _DEBUG

	FILE* pFile = fopen(pFilepathSkins, "r");
	if (!pFile)
	{
#ifdef _DEBUG
		g_pConsole->Write(LOGLEVEL_ERROR, "Couldn't open file");
#endif // _DEBUG

		return;
	}

	uint32_t iHash;
	int iCurPaintKit = 0;

	int iLevels = 0;
	bool bInPaintKits = false;
	bool bInClientLootList = false;
	//bool bInItemSets = false;
	bool bInAlternateIcons = false;
	char pBuffer[1024];
	std::vector<const char*> iconpaths;
	while (fgets(pBuffer, 1024, pFile))
	{
		if (bInPaintKits)
		{
			if (strstr(pBuffer, "{"))
			{
				iLevels++;
				continue;
			}
			else if (strstr(pBuffer, "}"))
			{
				if (--iLevels == 0)
				{
					bInPaintKits = false;
					continue;
				}
			}

			// Paintkit ID incoming
			if (iLevels == 1)
			{
				char* p = strstr(pBuffer, "\"");
				if (!p)
					continue;

				p++;
				int iLen = strlen(p);
				p[iLen - 1] = '\0';

				iCurPaintKit = atoi(p);
			}
			// Current Paintkit key/values
			else if (iLevels == 2)
			{
				if (iCurPaintKit != 0)
				{
					char* pKey = strtok(pBuffer, "\"\t \n");
					char* pValue = strtok(NULL, "\"\t \n");

					if (!strcmp(pKey, "name"))
					{
						iHash = murmurhash(pValue, strlen(pValue), 0xB16B00B5);
						m_mapPaintKits[iHash] = iCurPaintKit;
					}
					else if (!strcmp(pKey, "description_tag"))
					{
						wchar_t* pTranslation = m_pApp->Localize()->Find(pValue + 1);

						int iLen = wcslen(pTranslation) + 1;
						char* p = new char[iLen];
						wcstombs(p, pTranslation, iLen);

						m_mapPaintKitDescription[iHash] = p;
					}
				}
			}
		}
		else if (bInClientLootList/* || bInItemSets*/)
		{
			if (strstr(pBuffer, "{"))
			{
				iLevels++;
				continue;
			}
			else if (strstr(pBuffer, "}"))
			{
				if (--iLevels == 0)
				{
					bInClientLootList = false;
					//bInItemSets = false;
					continue;
				}
			}

			if (bInClientLootList && iLevels == 2/* ||
				bInItemSets && iLevels == 3*/)
			{
				char* pPaintName = strtok(pBuffer, "\"\n\t \n[]");
				char* pWeaponName = strtok(NULL, "\"\n\t \n[]");

				if (strncmp(pWeaponName, "weapon_", 7))
				{
					continue;
				}

				iHash = murmurhash(pPaintName, strlen(pPaintName), 0xB16B00B5);
				m_mapWeaponSkins[iHash].push_back(m_mapWeapons[murmurhash(pWeaponName, strlen(pWeaponName), 0xB16B00B5)].id);
			}
		}
		else if (bInAlternateIcons)
		{
			if (strstr(pBuffer, "{"))
			{
				iLevels++;
				continue;
			}
			else if (strstr(pBuffer, "}"))
			{
				if (--iLevels == 0)
				{
					bInAlternateIcons = false;
					continue;
				}
			}

			if (iLevels == 3)
			{
				char* pKey = strtok(pBuffer, "\"\t \n");
				char* pValue = strtok(NULL, "\"\t \n");
				char* temp;
				if ((temp = strstr(pValue, "weapon_knife")))
				{
					int iLen = strlen(temp) + 1;
					char* p = new char[iLen];
					memcpy(p, temp, iLen);

					iconpaths.push_back(p);
				}
			}
		}
		else if (strstr(pBuffer, "paint_kits"))
		{
			bInPaintKits = true;
			iLevels = 0;
		}
		else if (strstr(pBuffer, "client_loot_lists"))
		{
			bInClientLootList = true;
			iLevels = 0;
		}
		/*else if (strstr(pBuffer, "item_sets"))
		{
			bInItemSets = true;
			iLevels = 0;
		}*/
		else if (strstr(pBuffer, "alternate_icons2"))
		{
			bInAlternateIcons = true;
			iLevels = 0;
		}
	}

	fclose(pFile);

	for (std::unordered_map<uint32_t, std::vector<int>>::iterator it = m_mapWeaponSkins.begin(); it != m_mapWeaponSkins.end(); it++)
	{
		for (std::vector<int>::iterator weapIt = it->second.begin(); weapIt != it->second.end(); weapIt++)
		{
			m_mapSkins[*weapIt][m_mapPaintKits[it->first]] = m_mapPaintKitDescription[it->first];
		}
	}

	// add knife skins !!!
	// for all iconpaths
	// for all paintkits
	// if iconpath contains paintkit NAME add to list only once!!
	for (std::vector<const char*>::iterator it = iconpaths.begin(); it != iconpaths.end(); it++)
	{
		delete[] *it;
	}
	
}

void CSkinChanger::ClearReplacements()
{
	this->DeleteModelNames();
	this->DeleteSkinMetadata();
	this->DeleteKillIcons();
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

	if (m_bForceFullUpdate)
	{
		m_bForceFullUpdate = false;
		m_pApp->ClientState()->ForceFullUpdate();
	}
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
	if (m_iDesiredKnifeModelIndex == iDesiredKnifeModelIndex && !m_bNewMap)
		return;

	m_iDesiredKnifeModelIndex = iDesiredKnifeModelIndex;

	this->DeleteModelNames();
	this->DeleteKillIcons();
	if (m_iDesiredKnifeModelIndex == 0)
	{
		m_pApp->ClientState()->ForceFullUpdate();
		return;
	}


	const char* pDesiredKnife = this->m_mapKnives[m_iDesiredKnifeModelIndex];

	for (std::unordered_map<int, const char*>::iterator it = m_mapKnives.begin(); it != m_mapKnives.end(); it++)
	{
		AddSkinReplacement(
			it->first,
			new CSkinMetadata(
				m_iDesiredKnifeModelIndex,
				0,
				0,
				-1,
				3
			)
		);

		// No model replacement for our desired knife
		if (it->first == m_iDesiredKnifeModelIndex)
		{
			continue;
		}

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
	m_bNewMap = false;
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
