#include "Skinchanger.h"
#include "Application.h"

CSkinChanger::CSkinChanger()
	: m_bForceFullUpdate(true), m_iDesiredKnifeModelIndexCT(0), m_iDesiredKnifeModelIndexT(0)
{
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

	static CXorString xorKnifeCTModel("zdá§{xªµrjõ­yxª´H`ë«qnÚ¦rmä·{Ú¡c%è¦{");
	static CXorString xorKnifeTModel("zdá§{xªµrjõ­yxª´H`ë«qnÚ¦rmä·{Ú¶9fá®");
	static CXorString xorKnifeBayonetModel("zdá§{xªµrjõ­yxª´H`ë«qnÚ vrê¬r«¯sg");
	static CXorString xorKnifeButterflyModel("zdá§{xªµrjõ­yxª´H`ë«qnÚ bñ§emé»9fá®");
	static CXorString xorKnifeFalchionModel("zdá§{xªµrjõ­yxª´H`ë«qnÚ¤vgæª~dëvoó£yhà¦9fá®");
	static CXorString xorKnifeFlipModel("zdá§{xªµrjõ­yxª´H`ë«qnÚ¤{bõìzoé");
	static CXorString xorKnifeGutModel("zdá§{xªµrjõ­yxª´H`ë«qnÚ¥b«¯sg");
	static CXorString xorKnifeM9BayonetModel("zdá§{xªµrjõ­yxª´H`ë«qnÚ¯.Tç£n%è¦{");
	static CXorString xorKnifeKarambitModel("zdá§{xªµrjõ­yxª´H`ë«qnÚ©vyä¯9fá®");
	static CXorString xorKnifePushModel("zdá§{xªµrjõ­yxª´H`ë«qnÚ²bxíìzoé");
	static CXorString xorKnifeSurvivalBowieModel("zdá§{xªµrjõ­yxª´H`ë«qnÚ±byó«ajéudò«r%è¦{");
	static CXorString xorKnifeTacticalModel("zdá§{xªµrjõ­yxª´H`ë«qnÚ¶vhñ«tjéìzoé");

	this->m_mapKnives[WEAPON_KNIFE] = xorKnifeCTModel.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_T] = xorKnifeTModel.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_BAYONET] = xorKnifeBayonetModel.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_BUTTERFLY] = xorKnifeButterflyModel.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_FALCHION] = xorKnifeFalchionModel.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_FLIP] = xorKnifeFlipModel.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_GUT] = xorKnifeGutModel.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_M9_BAYONET] = xorKnifeM9BayonetModel.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_KARAMBIT] = xorKnifeKarambitModel.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_PUSH] = xorKnifePushModel.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_SURVIVAL_BOWIE] = xorKnifeSurvivalBowieModel.ToCharArray();
	this->m_mapKnives[WEAPON_KNIFE_TACTICAL] = xorKnifeTacticalModel.ToCharArray();

	static CXorString xorDeagle(/*weapon_deagle*/"weapon_deagle");
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

	static CXorString xorBayonet("weapon_bayonet");
	static CXorString xorKnifeFlip("weapon_knife_flip");
	static CXorString xorKnifeGut("weapon_knife_gut");
	static CXorString xorKnifeKarambit("weapon_knife_karambit");
	static CXorString xorKnifeM9Bayonet("weapon_knife_m9_bayonet");
	static CXorString xorKnifeTactical("weapon_knife_tactical");
	static CXorString xorKnifeFalchion("weapon_knife_falchion");
	static CXorString xorKnifeSurvivalBowie("weapon_knife_survival_bowie");
	static CXorString xorKnifeButterfly("weapon_knife_butterfly");
	static CXorString xorKnifePush("weapon_knife_push");

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

	xorBayonet.Xor();
	xorKnifeFlip.Xor();
	xorKnifeGut.Xor();
	xorKnifeKarambit.Xor();
	xorKnifeM9Bayonet.Xor();
	xorKnifeTactical.Xor();
	xorKnifeFalchion.Xor();
	xorKnifeSurvivalBowie.Xor();
	xorKnifeButterfly.Xor();
	xorKnifePush.Xor();

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

	this->m_mapKnifeWeapons[murmurhash(xorBayonet.ToCharArray(), strlen(xorBayonet.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_BAYONET, xorBayonet.ToCharArray());
	this->m_mapKnifeWeapons[murmurhash(xorKnifeFlip.ToCharArray(), strlen(xorKnifeFlip.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_FLIP, xorKnifeFlip.ToCharArray());
	this->m_mapKnifeWeapons[murmurhash(xorKnifeGut.ToCharArray(), strlen(xorKnifeGut.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_GUT, xorKnifeGut.ToCharArray());
	this->m_mapKnifeWeapons[murmurhash(xorKnifeKarambit.ToCharArray(), strlen(xorKnifeKarambit.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_KARAMBIT, xorKnifeKarambit.ToCharArray());
	this->m_mapKnifeWeapons[murmurhash(xorKnifeM9Bayonet.ToCharArray(), strlen(xorKnifeM9Bayonet.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_M9_BAYONET, xorKnifeM9Bayonet.ToCharArray());
	this->m_mapKnifeWeapons[murmurhash(xorKnifeTactical.ToCharArray(), strlen(xorKnifeTactical.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_TACTICAL, xorKnifeTactical.ToCharArray());
	this->m_mapKnifeWeapons[murmurhash(xorKnifeFalchion.ToCharArray(), strlen(xorKnifeFalchion.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_FALCHION, xorKnifeFalchion.ToCharArray());
	this->m_mapKnifeWeapons[murmurhash(xorKnifeSurvivalBowie.ToCharArray(), strlen(xorKnifeSurvivalBowie.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_SURVIVAL_BOWIE, xorKnifeSurvivalBowie.ToCharArray());
	this->m_mapKnifeWeapons[murmurhash(xorKnifeButterfly.ToCharArray(), strlen(xorKnifeButterfly.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_BUTTERFLY, xorKnifeButterfly.ToCharArray());
	this->m_mapKnifeWeapons[murmurhash(xorKnifePush.ToCharArray(), strlen(xorKnifePush.ToCharArray()), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_PUSH, xorKnifePush.ToCharArray());

	this->ParseSkinFile();
}

void CSkinChanger::Think(void* pParameters)
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

		if (m_bOnlyMyWeapons && pLocalInfo.xuidlow != *pWeapon->GetOriginalOwnerXuidLow())
			continue;

		this->ApplyCustomSkin(pWeapon, iWeaponId);

		// Für Stattrak (unknown user)
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

				char* temp = strstr(pValue, "weapon_knife");
				if (!temp)
					temp = strstr(pValue, "weapon_bayonet");

				if (temp && strstr(pValue, "_light"))
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

	// add knife skins !!!
	// for all iconpaths
	// for all paintkits
	// if iconpath contains paintkit NAME add to list only once!!
	const int iLenUnderscoreLight = strlen("_light");
	const char* p;
	for (std::vector<const char*>::iterator it = iconpaths.begin(); it != iconpaths.end(); it++)
	{
		p = *it;
		for (std::unordered_map<uint32_t, WeaponMetadata_t>::iterator it = m_mapKnifeWeapons.begin(); it != m_mapKnifeWeapons.end(); it++)
		{
			if (strstr(p, it->second.name))
			{
				int iLenFull = strlen(p);
				int iLenKnife = strlen(it->second.name) + 1;
				int iLenPaintKit = iLenFull - iLenKnife - iLenUnderscoreLight;
				
				char* pBuffer = new char[iLenPaintKit + 1];
				memcpy(pBuffer, p + iLenKnife, iLenPaintKit);
				pBuffer[iLenPaintKit] = '\0';

				iHash = murmurhash(pBuffer, iLenPaintKit, 0xB16B00B5);
				m_mapWeaponSkins[iHash].push_back(it->second.id);
				break;
			}
		}

		delete[] p;
	}

	for (std::unordered_map<uint32_t, std::vector<int>>::iterator it = m_mapWeaponSkins.begin(); it != m_mapWeaponSkins.end(); it++)
	{
		for (std::vector<int>::iterator weapIt = it->second.begin(); weapIt != it->second.end(); weapIt++)
		{
			m_mapSkins[*weapIt][m_mapPaintKits[it->first]] = m_mapPaintKitDescription[it->first];
		}
	}
}

void CSkinChanger::ClearReplacements()
{
	this->DeleteSkinMetadata();
	this->DeleteModelNames(TEAMNUM_CT);
	this->DeleteModelNames(TEAMNUM_T);
	this->DeleteKillIcons(TEAMNUM_CT);
	this->DeleteKillIcons(TEAMNUM_T);
}

CSkinMetadata* CSkinChanger::GetSkinMetadataForWeapon(int iWeaponId)
{
	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return nullptr;

	int iLocalTeam = pLocalEntity->GetTeamNum();
	if (iLocalTeam != TEAMNUM_CT && iLocalTeam != TEAMNUM_T)
		return nullptr;

	std::unordered_map<int, CSkinMetadata*>& map = iLocalTeam == TEAMNUM_CT ? m_mapSkinMetadataCT : m_mapSkinMetadataT;

	std::unordered_map<int, CSkinMetadata*>::iterator it = map.find(iWeaponId);

	if (it == map.end())
		return nullptr;

	return it->second;
}

void CSkinChanger::AddModelReplacement(int iTeamNum, const char* pOld, const char* pNew)
{
	int iModelId = m_pApp->ModelInfo()->GetModelIndex(pOld);

	int iLenNew = strlen(pNew) + 1;
	char* pTemp = new char[iLenNew];
	memcpy(pTemp, pNew, iLenNew);

	std::unordered_map<int, const char*>& map = iTeamNum == TEAMNUM_CT ? m_mapModelMetadataCT : m_mapModelMetadataT;
	map[iModelId] = pTemp;
}

void CSkinChanger::AddSkinReplacement(int iTeamNum, int iWeaponId, CSkinMetadata* pSkin)
{
	std::unordered_map<int, CSkinMetadata*>& map = iTeamNum == TEAMNUM_CT ? m_mapSkinMetadataCT : m_mapSkinMetadataT;
	map[iWeaponId] = pSkin;
}

void CSkinChanger::AddKillIconReplacement(int iTeamNum, const char* pOld, const char* pNew)
{
	uint32_t iHash = murmurhash(pOld, strlen(pOld), 0xB16B00B5);

	int iLen = strlen(pNew) + 1;
	char* pNewWeap = new char[iLen];
	memcpy(pNewWeap, pNew, iLen);

	std::unordered_map<uint32_t, const char*>& map = iTeamNum == TEAMNUM_CT ? m_mapKillIconCT : m_mapKillIconT;
	map[iHash] = pNewWeap;
}

bool CSkinChanger::ApplyCustomModel(IClientEntity* pLocal, CBaseAttributableItem* pItem)
{
	int iLocalTeam = pLocal->GetTeamNum();
	if (iLocalTeam != TEAMNUM_CT && iLocalTeam != TEAMNUM_T)
		return false;

	std::unordered_map<int, const char*>& map = iLocalTeam == TEAMNUM_CT ? m_mapModelMetadataCT : m_mapModelMetadataT;
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
	if (map.find(iModelIdx) == map.end())
		return false;

	// Grab Replacement model & set it
	int iNewModelIdx = m_pApp->ModelInfo()->GetModelIndex(map[iModelIdx]);
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
	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return false;

	int iLocalTeam = pLocalEntity->GetTeamNum();
	if (iLocalTeam != TEAMNUM_CT && iLocalTeam != TEAMNUM_T)
		return false;

	std::unordered_map<int, CSkinMetadata*>& map = iLocalTeam == TEAMNUM_CT ? m_mapSkinMetadataCT : m_mapSkinMetadataT;
	if (map.find(iWeaponId) == map.end()) // TODO: Iterator speichern?
		return false;

	CSkinMetadata* pSkin = map[iWeaponId]; // ^ statt hier neu zu holen

	if (pSkin->m_iItemDefinitionIndex != -1)
	{
		*pWeapon->GetItemDefinitionIndex() = pSkin->m_iItemDefinitionIndex;
	}
	
	int* pPaintKit = pWeapon->GetFallbackPaintKit();
	if (*pPaintKit != pSkin->m_iFallbackPaintKit)
	{
		m_bForceFullUpdate = true;
	}
	*pPaintKit = pSkin->m_iFallbackPaintKit;
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

	IClientEntity* pLocalEntity = m_pApp->GetLocalPlayer();
	if (!pLocalEntity)
		return false;

	int iLocalTeam = pLocalEntity->GetTeamNum();
	if (iLocalTeam != TEAMNUM_CT && iLocalTeam != TEAMNUM_T)
		return false;

	std::unordered_map<uint32_t, const char*>& map = iLocalTeam == TEAMNUM_CT ? m_mapKillIconCT : m_mapKillIconT;

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
	if (map.find(iHash) == map.end())
		return false;

	// Override weapon
	pEvent->SetString(xorWeapon.ToCharArray(), map[iHash]);
	return true;
}

void CSkinChanger::ApplyDesiredKnife(int iTeamNum, int iDesiredKnifeModelIndex, int iPaintKit, int iSeed, int iStatTrak, const char* pName, float fWear, bool bApplySkin)
{
	int& iDesiredKnife = iTeamNum == TEAMNUM_CT ? m_iDesiredKnifeModelIndexCT : m_iDesiredKnifeModelIndexT;
	/*if (iDesiredKnife == iDesiredKnifeModelIndex && !m_bNewMap)
		return;*/
	
	iDesiredKnife = iDesiredKnifeModelIndex;

	this->DeleteModelNames(iTeamNum);
	this->DeleteKillIcons(iTeamNum);
	if (iDesiredKnife == 0)
	{
		m_pApp->ClientState()->ForceFullUpdate();
		return;
	}

	const char* pDesiredKnife = this->m_mapKnives[iDesiredKnife];
	for (std::unordered_map<int, const char*>::iterator it = m_mapKnives.begin(); it != m_mapKnives.end(); it++)
	{
		if (bApplySkin)
		{
			AddSkinReplacement(
				iTeamNum,
				it->first,
				new CSkinMetadata(
					iDesiredKnife,
					iPaintKit,
					iSeed,
					iStatTrak,
					3,
					pName,
					fWear
				)
			);
		}

		// No model replacement for our desired knife
		if (it->first == iDesiredKnife)
		{
			continue;
		}

		AddModelReplacement(
			iTeamNum,
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

void CSkinChanger::WriteToConfig(const char* pFilename)
{
	//CConfig config;
	//config.Init(m_pApp);
	//
	////std::unordered_map<int, CSkinMetadata*> m_mapSkinMetadata;
	////std::unordered_map<int, const char*> m_mapModelMetadata;

	//config.SetInt("knife", "id", this->GetDesiredKnifeModelIndex());

	//char pWeaponId[16];
	//for (std::unordered_map<int, CSkinMetadata*>::iterator it = m_mapSkinMetadata.begin(); it != m_mapSkinMetadata.end(); it++)
	//{
	//	if (it->first == 0)
	//		continue;

	//	itoa(it->first, pWeaponId, 10);
	//	config.SetInt(pWeaponId, "item", it->second->m_iItemDefinitionIndex);
	//	config.SetInt(pWeaponId, "paintkit", it->second->m_iFallbackPaintKit);
	//	config.SetInt(pWeaponId, "stattrak", it->second->m_iFallbackStatTrak);
	//	config.SetInt(pWeaponId, "seed", it->second->m_iFallbackSeed);
	//	config.SetFloat(pWeaponId, "wear", it->second->m_fFallbackWear);
	//	config.SetInt(pWeaponId, "quality", it->second->m_iEntityQuality);
	//	config.SetString(pWeaponId, "name", it->second->m_pCustomName);
	//}

	//config.SaveFile(pFilename);
}

void CSkinChanger::LoadFromConfig(const char* pFilename)
{
	/*CConfig config;
	config.Init(m_pApp);
	config.LoadFile(pFilename);

	this->ApplyDesiredKnife(config.GetInt("knife", "id"), false);

	int iWeaponId = 0;
	const char* pSection = config.GetFirstSection();
	for (; pSection; pSection = config.GetNextSection())
	{
		if (!strcmp(pSection, "knife"))
			continue;

		iWeaponId = atoi(pSection);

		this->AddSkinReplacement(
			iWeaponId,
			new CSkinMetadata(
				config.GetInt(pSection, "item"),
				config.GetInt(pSection, "paintkit"),
				config.GetInt(pSection, "seed"),
				config.GetInt(pSection, "stattrak"),
				config.GetInt(pSection, "quality"),
				config.GetString(pSection, "name"),
				config.GetFloat(pSection, "wear")
			)
		);
	}*/
}

void CSkinChanger::DeleteSkinMetadata()
{
	// Clean up SkinMetadata
	CSkinMetadata* pCurrent;
	for (std::unordered_map<int, CSkinMetadata*>::iterator it = m_mapSkinMetadataCT.begin(); it != m_mapSkinMetadataCT.end(); it++)
	{
		pCurrent = it->second;

		if (pCurrent)
			delete pCurrent;
	}
	m_mapSkinMetadataCT.clear();

	for (std::unordered_map<int, CSkinMetadata*>::iterator it = m_mapSkinMetadataT.begin(); it != m_mapSkinMetadataT.end(); it++)
	{
		pCurrent = it->second;

		if (pCurrent)
			delete pCurrent;
	}
	m_mapSkinMetadataT.clear();
}

void CSkinChanger::DeleteModelNames(int iTeamNum)
{
	// Clean up Model names
	const char* pCurrent;

	if (iTeamNum == TEAMNUM_CT)
	{
		for (std::unordered_map<int, const char*>::iterator it = m_mapModelMetadataCT.begin(); it != m_mapModelMetadataCT.end(); it++)
		{
			pCurrent = it->second;

			if (pCurrent)
				delete[] pCurrent;
		}
		m_mapModelMetadataCT.clear();
	}
	else
	{
		for (std::unordered_map<int, const char*>::iterator it = m_mapModelMetadataT.begin(); it != m_mapModelMetadataT.end(); it++)
		{
			pCurrent = it->second;

			if (pCurrent)
				delete[] pCurrent;
		}
		m_mapModelMetadataT.clear();
	}
}

void CSkinChanger::DeleteKillIcons(int iTeamNum)
{
	// Clean up KillIcons
	const char* pCurrent;

	if (iTeamNum == TEAMNUM_CT)
	{
		for (std::unordered_map<uint32_t, const char*>::iterator it = m_mapKillIconCT.begin(); it != m_mapKillIconCT.end(); it++)
		{
			pCurrent = it->second;

			if (pCurrent)
				delete[] pCurrent;
		}
		m_mapKillIconCT.clear();
	}
	else
	{
		for (std::unordered_map<uint32_t, const char*>::iterator it = m_mapKillIconT.begin(); it != m_mapKillIconT.end(); it++)
		{
			pCurrent = it->second;

			if (pCurrent)
				delete[] pCurrent;
		}
		m_mapKillIconT.clear();
	}
}
