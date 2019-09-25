#include "Skinchanger.h"
#include "Application.h"

CSkinChanger::CSkinChanger()
	: m_bForceFullUpdate(true), m_iDesiredKnifeModelIndexCT(0), m_iDesiredKnifeModelIndexT(0)
{
}

CSkinChanger::~CSkinChanger()
{
	this->ClearReplacements();

	const wchar_t* pCurrent;
	for (std::unordered_map<uint32_t, const wchar_t*>::iterator it = m_mapPaintKitDescription.begin(); it != m_mapPaintKitDescription.end(); it++)
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


	//
	// Note: All CXorString's in here are static; this has a reason!
	// The strings are used in WeaponMetadata_t (which is accessed by the Menu)
	// which means if their memory is freed we'd run into problems.
	//

	static CXorString xorKnifeCTModel("zd�{x��rj��yx��H`�qnڦrm�{ڡc%�{");
	static CXorString xorKnifeTModel("zd�{x��rj��yx��H`�qnڦrm�{ڶ9f�");
	static CXorString xorKnifeBayonetModel("zd�{x��rj��yx��H`�qnڠvr�r��sg");
	static CXorString xorKnifeButterflyModel("zd�{x��rj��yx��H`�qnڠb�em�9f�");
	static CXorString xorKnifeFalchionModel("zd�{x��rj��yx��H`�qnڤvg�~d�vo�yh�9f�");
	static CXorString xorKnifeFlipModel("zd�{x��rj��yx��H`�qnڤ{b��zo�");
	static CXorString xorKnifeGutModel("zd�{x��rj��yx��H`�qnڥb��sg");
	static CXorString xorKnifeM9BayonetModel("zd�{x��rj��yx��H`�qnگ.T�n%�{");
	static CXorString xorKnifeKarambitModel("zd�{x��rj��yx��H`�qnکvy�9f�");
	static CXorString xorKnifePushModel("zd�{x��rj��yx��H`�qnڲbx��zo�");
	static CXorString xorKnifeSurvivalBowieModel("zd�{x��rj��yx��H`�qnڱby�aj�ud�r%�{");
	static CXorString xorKnifeTacticalModel("zd�{x��rj��yx��H`�qnڶvh�tj��zo�");

	this->m_mapKnives[WEAPON_KNIFE] = xorKnifeCTModel;
	this->m_mapKnives[WEAPON_KNIFE_T] = xorKnifeTModel;
	this->m_mapKnives[WEAPON_KNIFE_BAYONET] = xorKnifeBayonetModel;
	this->m_mapKnives[WEAPON_KNIFE_BUTTERFLY] = xorKnifeButterflyModel;
	this->m_mapKnives[WEAPON_KNIFE_FALCHION] = xorKnifeFalchionModel;
	this->m_mapKnives[WEAPON_KNIFE_FLIP] = xorKnifeFlipModel;
	this->m_mapKnives[WEAPON_KNIFE_GUT] = xorKnifeGutModel;
	this->m_mapKnives[WEAPON_KNIFE_M9_BAYONET] = xorKnifeM9BayonetModel;
	this->m_mapKnives[WEAPON_KNIFE_KARAMBIT] = xorKnifeKarambitModel;
	this->m_mapKnives[WEAPON_KNIFE_PUSH] = xorKnifePushModel;
	this->m_mapKnives[WEAPON_KNIFE_SURVIVAL_BOWIE] = xorKnifeSurvivalBowieModel;
	this->m_mapKnives[WEAPON_KNIFE_TACTICAL] = xorKnifeTacticalModel;

	static CXorString xorDeagle(/*weapon_deagle*/"`n�xeڦrj�r");
	static CXorString xorElite(/*weapon_elite*/"`n�xeڧ{b�");
	static CXorString xorFiveseven(/*weapon_fiveseven*/"`n�xeڤ~}�r}�");
	static CXorString xorGlock(/*weapon_glock*/"`n�xeڥ{d�");
	static CXorString xorAk47(/*weapon_ak47*/"`n�xeڣ|?�");
	static CXorString xorAug(/*weapon_aug*/"`n�xeڣbl");
	static CXorString xorAwp(/*weapon_awp*/"`n�xeڣ`{");
	static CXorString xorFamas(/*weapon_famas*/"`n�xeڤvf�");
	static CXorString xorG3sg1(/*weapon_g3sg1*/"`n�xeڥ$x��");
	static CXorString xorGalil(/*weapon_galilar*/"`n�xeڥvg�vy");
	static CXorString xorM249(/*weapon_m249*/"`n�xeگ%?�");
	static CXorString xorM4a1(/*weapon_m4a1*/"`n�xeگ#j�");
	static CXorString xorMac10(/*weapon_mac10*/"`n�xeگvh��");
	static CXorString xorP90(/*weapon_p90*/"`n�xeڲ.;");
	static CXorString xorUmp45(/*weapon_ump45*/"`n�xeڷz{��");
	static CXorString xorXm1014(/*weapon_xm1014*/"`n�xeںz:��#");
	static CXorString xorBizon(/*weapon_bizon*/"`n�xeڠ~q�");
	static CXorString xorMag7(/*weapon_mag7*/"`n�xeگvl�");
	static CXorString xorNegev(/*weapon_negev*/"`n�xeڬrl�");
	static CXorString xorSawedoff(/*weapon_sawedoff*/"`n�xeڱv|�xm�");
	static CXorString xorTec9(/*weapon_tec9*/"`n�xeڶrh�");
	static CXorString xorHkp2000(/*weapon_hkp2000*/"`n�xeڪ|{��';");
	static CXorString xorMp7(/*weapon_mp7*/"`n�xeگg<");
	static CXorString xorMp9(/*weapon_mp9*/"`n�xeگg2");
	static CXorString xorNova(/*weapon_nova*/"`n�xeڬx}�");
	static CXorString xorP250(/*weapon_p250*/"`n�xeڲ%>�");
	static CXorString xorScar20(/*weapon_scar20*/"`n�xeڱtj��'");
	static CXorString xorSg556(/*weapon_sg556*/"`n�xeڱp>��");
	static CXorString xorSsg08(/*weapon_ssg08*/"`n�xeڱdl��");
	static CXorString xorM4a1Silencer(/*weapon_m4a1_silencer*/"`n�xeگ#j��db�yh�");
	static CXorString xorUspSilencer(/*weapon_usp_silencer*/"`n�xeڷd{ڱ~g�tn�");
	static CXorString xorCz75(/*weapon_cz75a*/"`n�xeڡm<��");
	static CXorString xorRevolver(/*weapon_revolver*/"`n�xeڰr}�an�");

	static CXorString xorBayonet(/*weapon_bayonet*/"`n�xeڠvr�r");
	static CXorString xorKnifeFlip(/*weapon_knife_flip*/"`n�xeکyb�Hm�g");
	static CXorString xorKnifeGut(/*weapon_knife_gut*/"`n�xeکyb�Hl�");
	static CXorString xorKnifeKarambit(/*weapon_knife_karambit*/"`n�xeکyb�H`�vf�c");
	static CXorString xorKnifeM9Bayonet(/*weapon_knife_m9_bayonet*/"`n�xeکyb�Hf��uj��yn�");
	static CXorString xorKnifeTactical(/*weapon_knife_tactical*/"`n�xeکyb�H�cb�{");
	static CXorString xorKnifeFalchion(/*weapon_knife_falchion*/"`n�xeکyb�Hm�tc�y");
	static CXorString xorKnifeSurvivalBowie(/*weapon_knife_survival_bowie*/"`n�xeکyb�Hx�ab�{T�`b�");
	static CXorString xorKnifeButterfly(/*weapon_knife_butterfly*/"`n�xeکyb�Hi�cn��{r");
	static CXorString xorKnifePush(/*weapon_knife_push*/"`n�xeکyb�H{�");

	static CXorString xorDeagleReadable(/*Deagle*/"Sn�{n");
	static CXorString xorEliteReadable(/*Elite*/"Rg�r");
	static CXorString xorFivesevenReadable(/*Fiveseven*/"Qb�dn�y");
	static CXorString xorGlockReadable(/*Glock*/"Pg�|");
	static CXorString xorAk47Readable(/*AK-47*/"V@�� ");
	static CXorString xorAugReadable(/*AUG*/"V^�");
	static CXorString xorAwpReadable(/*AWP*/"V\\�");
	static CXorString xorFamasReadable(/*Famas*/"Qj�d");
	static CXorString xorG3sg1Readable(/*G3SG1*/"P8օ&");
	static CXorString xorGalilReadable(/*Galil AR*/"Pj�{+Đ");
	static CXorString xorM249Readable(/*M249*/"Z9��");
	static CXorString xorM4a1Readable(/*M4A1*/"Z?��");
	static CXorString xorMac10Readable(/*Mac-10*/"Zj��&;");
	static CXorString xorP90Readable(/*P90*/"G2�");
	static CXorString xorUmp45Readable(/*UMP-45*/"BF��#>");
	static CXorString xorXm1014Readable(/*XM1014*/"OF��&?");
	static CXorString xorBizonReadable(/*Bizon*/"Ub��y");
	static CXorString xorMag7Readable(/*Mag7*/"Zj��");
	static CXorString xorNegevReadable(/*Negev*/"Yn�a");
	static CXorString xorSawedoffReadable(/*Sawed off*/"Dj�s+�q");
	static CXorString xorTec9Readable(/*Tec NEIN*/"Cn��YŇ"); // TODO: Tec-9 :D
	static CXorString xorHkp2000Readable(/*HKP-2000*/"_@��%;��");
	static CXorString xorMp7Readable(/*MP-7*/"Z[��");
	static CXorString xorMp9Readable(/*MP-9*/"Z[��");
	static CXorString xorNovaReadable(/*Nova*/"Yd�");
	static CXorString xorP250Readable(/*P250*/"G9��");
	static CXorString xorScar20Readable(/*SCAR-20*/"DHĐ:9�");
	static CXorString xorSg556Readable(/*SG-556*/"DL��\"=");
	static CXorString xorSsg08Readable(/*SSG-08*/"DX��'3");
	static CXorString xorM4a1SilencerReadable(/*M4A1-Silencer*/"Z?��:X�re�e");
	static CXorString xorUspSilencerReadable(/*USP-Silencer*/"BX��Db�yh�");
	static CXorString xorCz75Readable(/*CZ75*/"TQ��");
	static CXorString xorRevolverReadable(/*Revolver*/"En�{}�");

	this->m_mapWeapons[murmurhash(xorDeagle, strlen(xorDeagle), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_DEAGLE, xorDeagle, xorDeagleReadable);
	this->m_mapWeapons[murmurhash(xorElite, strlen(xorElite), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_ELITE, xorElite, xorEliteReadable);
	this->m_mapWeapons[murmurhash(xorFiveseven, strlen(xorFiveseven), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_FIVESEVEN, xorFiveseven, xorFivesevenReadable);
	this->m_mapWeapons[murmurhash(xorGlock, strlen(xorGlock), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_GLOCK, xorGlock, xorGlockReadable);
	this->m_mapWeapons[murmurhash(xorAk47, strlen(xorAk47), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_AK47, xorAk47, xorAk47Readable);
	this->m_mapWeapons[murmurhash(xorAug, strlen(xorAug), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_AUG, xorAug, xorAugReadable);
	this->m_mapWeapons[murmurhash(xorAwp, strlen(xorAwp), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_AWP, xorAwp, xorAwpReadable);
	this->m_mapWeapons[murmurhash(xorFamas, strlen(xorFamas), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_FAMAS, xorFamas, xorFamasReadable);
	this->m_mapWeapons[murmurhash(xorG3sg1, strlen(xorG3sg1), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_G3SG1, xorG3sg1, xorG3sg1Readable);
	this->m_mapWeapons[murmurhash(xorGalil, strlen(xorGalil), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_GALILAR, xorGalil, xorGalilReadable);
	this->m_mapWeapons[murmurhash(xorM249, strlen(xorM249), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_M249, xorM249, xorM249Readable);
	this->m_mapWeapons[murmurhash(xorM4a1, strlen(xorM4a1), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_M4A1, xorM4a1, xorM4a1Readable);
	this->m_mapWeapons[murmurhash(xorMac10, strlen(xorMac10), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_MAC10, xorMac10, xorMac10Readable);
	this->m_mapWeapons[murmurhash(xorP90, strlen(xorP90), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_P90, xorP90, xorP90Readable);
	this->m_mapWeapons[murmurhash(xorUmp45, strlen(xorUmp45), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_UMP45, xorUmp45, xorUmp45Readable);
	this->m_mapWeapons[murmurhash(xorXm1014, strlen(xorXm1014), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_XM1014, xorXm1014, xorXm1014Readable);
	this->m_mapWeapons[murmurhash(xorBizon, strlen(xorBizon), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_BIZON, xorBizon, xorBizonReadable);
	this->m_mapWeapons[murmurhash(xorMag7, strlen(xorMag7), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_MAG7, xorMag7, xorMag7Readable);
	this->m_mapWeapons[murmurhash(xorNegev, strlen(xorNegev), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_NEGEV, xorNegev, xorNegevReadable);
	this->m_mapWeapons[murmurhash(xorSawedoff, strlen(xorSawedoff), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_SAWEDOFF, xorSawedoff, xorSawedoffReadable);
	this->m_mapWeapons[murmurhash(xorTec9, strlen(xorTec9), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_TEC9, xorTec9, xorTec9Readable);
	this->m_mapWeapons[murmurhash(xorHkp2000, strlen(xorHkp2000), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_HKP2000, xorHkp2000, xorHkp2000Readable);
	this->m_mapWeapons[murmurhash(xorMp7, strlen(xorMp7), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_MP7, xorMp7, xorMp7Readable);
	this->m_mapWeapons[murmurhash(xorMp9, strlen(xorMp9), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_MP9, xorMp9, xorMp9Readable);
	this->m_mapWeapons[murmurhash(xorNova, strlen(xorNova), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_NOVA, xorNova, xorNovaReadable);
	this->m_mapWeapons[murmurhash(xorP250, strlen(xorP250), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_P250, xorP250, xorP250Readable);
	this->m_mapWeapons[murmurhash(xorScar20, strlen(xorScar20), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_SCAR20, xorScar20, xorScar20Readable);
	this->m_mapWeapons[murmurhash(xorSg556, strlen(xorSg556), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_SG556, xorSg556, xorSg556Readable);
	this->m_mapWeapons[murmurhash(xorSsg08, strlen(xorSsg08), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_SSG08, xorSsg08, xorSsg08Readable);
	this->m_mapWeapons[murmurhash(xorM4a1Silencer, strlen(xorM4a1Silencer), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_M4A1_SILENCER, xorM4a1Silencer, xorM4a1SilencerReadable);
	this->m_mapWeapons[murmurhash(xorUspSilencer, strlen(xorUspSilencer), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_USP_SILENCER, xorUspSilencer, xorUspSilencerReadable);
	this->m_mapWeapons[murmurhash(xorCz75, strlen(xorCz75), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_CZ75A, xorCz75, xorCz75Readable);
	this->m_mapWeapons[murmurhash(xorRevolver, strlen(xorRevolver), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_REVOLVER, xorRevolver, xorRevolverReadable);

	this->m_mapKnifeWeapons[murmurhash(xorBayonet, strlen(xorBayonet), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_BAYONET, xorBayonet);
	this->m_mapKnifeWeapons[murmurhash(xorKnifeFlip, strlen(xorKnifeFlip), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_FLIP, xorKnifeFlip);
	this->m_mapKnifeWeapons[murmurhash(xorKnifeGut, strlen(xorKnifeGut), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_GUT, xorKnifeGut);
	this->m_mapKnifeWeapons[murmurhash(xorKnifeKarambit, strlen(xorKnifeKarambit), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_KARAMBIT, xorKnifeKarambit);
	this->m_mapKnifeWeapons[murmurhash(xorKnifeM9Bayonet, strlen(xorKnifeM9Bayonet), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_M9_BAYONET, xorKnifeM9Bayonet);
	this->m_mapKnifeWeapons[murmurhash(xorKnifeTactical, strlen(xorKnifeTactical), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_TACTICAL, xorKnifeTactical);
	this->m_mapKnifeWeapons[murmurhash(xorKnifeFalchion, strlen(xorKnifeFalchion), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_FALCHION, xorKnifeFalchion);
	this->m_mapKnifeWeapons[murmurhash(xorKnifeSurvivalBowie, strlen(xorKnifeSurvivalBowie), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_SURVIVAL_BOWIE, xorKnifeSurvivalBowie);
	this->m_mapKnifeWeapons[murmurhash(xorKnifeButterfly, strlen(xorKnifeButterfly), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_BUTTERFLY, xorKnifeButterfly);
	this->m_mapKnifeWeapons[murmurhash(xorKnifePush, strlen(xorKnifePush), 0xB16B00B5)] = WeaponMetadata_t(WEAPON_KNIFE_PUSH, xorKnifePush);

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

		short iWeaponId = *pWeapon->GetItemDefinitionIndex();

		this->ApplyCustomModel(pLocalEntity, pWeapon);

		if (m_bOnlyMyWeapons && pLocalInfo.xuidlow != *pWeapon->GetOriginalOwnerXuidLow())
			continue;

		this->ApplyCustomSkin(pWeapon, iWeaponId);

		// F�r Stattrak (unknown user)
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

	strcat(pFilepathSkins, /*scripts\\items\\items_game.txt*/CXorString("dh��g��Kb�zxٞ~�dT�zn��o"));

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

	CXorString xorName(/*name*/"yj�");
	CXorString xorPhase(/*phase*/"gc�r");
	CXorString xorEmerald(/*emerald*/"rf�vg�");
	CXorString xorRuby(/*ruby*/"e~�");
	CXorString xorSapphire(/*sapphire*/"dj��b��");
	CXorString xorBlackPearl(/*blackpearl*/"ug�|{�eg");
	CXorString xorDescriptionTag(/*description_tag*/"sn��eb��~d�cj�");
	CXorString xorWeaponUnderscore(/*weapon_*/"`n�xe�");
	CXorString xorUnderscoreLight(/*_light*/"Hg�");
	CXorString xorWeaponKnife(/*weapon_knife*/"`n�xeکyb�");
	CXorString xorWeaponBayonet(/*weapon_bayonet*/"`n�xeڠvr�r");
	CXorString xorPaintKits(/*paint_kits*/"gj�cT�cx");
	CXorString xorClientLootLists(/*client_loot_lists*/"tg�yڮxd�{b��d");
	CXorString xorAlternateIcons2(/*alternate_icons2*/"vg�ee�rT�xe��");

	uint32_t iHash;
	int iCurPaintKit = 0;

	int iCurPhase = -1;
	bool bEmerald = false;
	bool bRuby = false;
	bool bSapphire = false;
	bool bBlackPearl = false;

	int iLevels = 0;
	bool bInPaintKits = false;
	bool bInClientLootList = false;
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

					if (!strcmp(pKey, xorName))
					{
						char* pPhase = strstr(pValue, xorPhase);
						if (pPhase)
						{
							iCurPhase = *(pPhase + 5) - '0';
						}
						else
						{
							iCurPhase = -1;
						}
						
						bEmerald = strstr(pValue, xorEmerald) != nullptr;
						bRuby = strstr(pValue, xorRuby) != nullptr;
						bSapphire = strstr(pValue, xorSapphire) != nullptr;
						bBlackPearl = strstr(pValue, xorBlackPearl) != nullptr;

						iHash = murmurhash(pValue, strlen(pValue), 0xB16B00B5);
						m_mapPaintKits[iHash] = iCurPaintKit;
					}
					else if (!strcmp(pKey, xorDescriptionTag))
					{
						wchar_t* pTranslation = m_pApp->Localize()->Find(pValue + 1);

						int iLen = wcslen(pTranslation) + 1;
						wchar_t* p;
						if (bEmerald)
						{
							p = new wchar_t[8];
							wcscpy(p, L"Emerald"); // TODO: CXorWString?
						}
						else if (bRuby)
						{
							p = new wchar_t[5];
							wcscpy(p, L"Ruby");
						}
						else if (bSapphire)
						{
							p = new wchar_t[9];
							wcscpy(p, L"Sapphire");
						}
						else if (bBlackPearl)
						{
							p = new wchar_t[12];
							wcscpy(p, L"Black Pearl");
						}
						else if (iCurPhase == -1)
						{
							p = new wchar_t[iLen];
							memcpy(p, pTranslation, iLen * sizeof(wchar_t));
						}
						else
						{
							iLen += 8; // " x"
							p = new wchar_t[iLen];
#ifdef __MINGW32__
                            swprintf(p, L"%ls Phase %d", pTranslation, iCurPhase);
#else
							swprintf(p, iLen, L"%ls Phase %d", pTranslation, iCurPhase);
#endif
						}

						m_mapPaintKitDescription[iHash] = p;
					}
				}
			}
		}
		else if (bInClientLootList)
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
					continue;
				}
			}

			if (iLevels == 2)
			{
				char* pPaintName = strtok(pBuffer, "\"\n\t \n[]");
				char* pWeaponName = strtok(NULL, "\"\n\t \n[]");

				if (strncmp(pWeaponName, xorWeaponUnderscore, 7))
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

				char* temp = strstr(pValue, xorWeaponKnife);
				if (!temp)
					temp = strstr(pValue, xorWeaponBayonet);

				if (temp && strstr(pValue, xorUnderscoreLight))
				{
					int iLen = strlen(temp) + 1;
					char* p = new char[iLen];
					memcpy(p, temp, iLen);

					iconpaths.push_back(p);
				}
			}
		}
		else if (strstr(pBuffer, xorPaintKits))
		{
			bInPaintKits = true;
			iLevels = 0;
		}
		else if (strstr(pBuffer, xorClientLootLists))
		{
			bInClientLootList = true;
			iLevels = 0;
		}
		else if (strstr(pBuffer, xorAlternateIcons2))
		{
			bInAlternateIcons = true;
			iLevels = 0;
		}
	}

	fclose(pFile);

	const int iLenUnderscoreLight = strlen(xorUnderscoreLight);
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

	this->SortWeaponsAndSkinsByName();
}

void CSkinChanger::SortWeaponsAndSkinsByName()
{
	CBenchmark sortWeaps(true);
	m_vWeapons.reserve(m_mapWeapons.size());
	for (std::unordered_map<uint32_t, WeaponMetadata_t>::iterator mIt = m_mapWeapons.begin(); mIt != m_mapWeapons.end(); mIt++)
	{
		m_vWeapons.emplace_back(mIt->second);
	}

	/*std::sort(m_vWeapons.begin(), m_vWeapons.end(), [](const WeaponMetadata_t& a, const WeaponMetadata_t& b) {
		return strcmp(a.readableName, b.readableName) < 0;
	});*/
	sortWeaps.FinishBenchmark();
	sortWeaps.PrintBenchmark("Sorting Weapons");

	CBenchmark sortSkins(true);
	std::unordered_map<int, std::vector<SkinMetadata_t>>::iterator it;
	for (std::unordered_map<int, std::unordered_map<int, const wchar_t*>>::iterator itWeaps = m_mapSkins.begin(); itWeaps != m_mapSkins.end(); itWeaps++)
	{
		int iWeaponId = itWeaps->first;

		m_vSkins[iWeaponId].reserve(itWeaps->second.size());
		for (std::unordered_map<int, const wchar_t*>::iterator itPaintKits = itWeaps->second.begin(); itPaintKits != itWeaps->second.end(); itPaintKits++)
		{
			m_vSkins[iWeaponId].emplace_back(SkinMetadata_t(itPaintKits->first, itPaintKits->second));
		}

		std::sort(m_vSkins[iWeaponId].begin(), m_vSkins[iWeaponId].end(), [](const SkinMetadata_t& a, const SkinMetadata_t& b) {
			return wcscmp(a.name, b.name) < 0;
		});
	}
	sortSkins.FinishBenchmark();
	sortSkins.PrintBenchmark("Sorting Skins");
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

	static bool bWasForceFullUpdate = false;
	if (m_bForceFullUpdate)
	{
		bWasForceFullUpdate = m_bForceFullUpdate;
	}

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

	if (bWasForceFullUpdate)
	{
		m_bForceFullUpdate = bWasForceFullUpdate;
		bWasForceFullUpdate = false;
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

	static CXorString xorAttacker("v�t`�");
	static CXorString xorPlayerDeath("gg�ryڦrj�");
	static CXorString xorWeapon("`n�xe");

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

void CSkinChanger::ApplyDesiredKnife(int iTeamNum, int iDesiredKnifeModelIndex, bool bApplySkin, int iPaintKit, int iSeed, int iStatTrak, const char* pName, float fWear)
{
	int& iDesiredKnife = iTeamNum == TEAMNUM_CT ? m_iDesiredKnifeModelIndexCT : m_iDesiredKnifeModelIndexT;

	iDesiredKnife = iDesiredKnifeModelIndex;

	this->DeleteModelNames(iTeamNum);
	this->DeleteKillIcons(iTeamNum);
	// Changed this because crash in game_newmap
	if (iDesiredKnife <= 0) // Used to be: (iDesiredKnife == -1)
	{
		// TODO: delete skins for old knifechange

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

		// todo:
		/*AddKillIconReplacement(
			CXorString("|e�rT��Hi�xe�").ToCharArray(),
			CXorString("uj��yn�").ToCharArray()
		);*/
	}

	m_bForceFullUpdate = true;
}

bool CSkinChanger::WriteToConfig(const char* pFilename)
{
	char pFullFilename[MAX_PATH];
	snprintf(pFullFilename, MAX_PATH, "%s%s", SKINCHANGER_CONFIG_FOLDER, pFilename);

	CConfig config;
	config.Init(m_pApp);

	config.SetInt("knife$ct", "id", this->GetDesiredKnifeModelIndexCT());
	config.SetInt("knife$t", "id", this->GetDesiredKnifeModelIndexT());

	for (int i = 0; i < 2; i++)
	{
		const char* pTeamStr = i == 0 ? "ct" : "t";
		std::unordered_map<int, CSkinMetadata*>& map = i == 0 ? m_mapSkinMetadataCT : m_mapSkinMetadataT;

		char pWeaponId[16];
		for (std::unordered_map<int, CSkinMetadata*>::iterator it = map.begin(); it != map.end(); it++)
		{
			if (it->first == 0)
				continue;

			snprintf(pWeaponId, 16, "%d$%s", it->first, pTeamStr);
			config.SetInt(pWeaponId, "item", it->second->m_iItemDefinitionIndex);
			config.SetInt(pWeaponId, "paintkit", it->second->m_iFallbackPaintKit);
			config.SetInt(pWeaponId, "stattrak", it->second->m_iFallbackStatTrak);
			config.SetInt(pWeaponId, "seed", it->second->m_iFallbackSeed);
			config.SetFloat(pWeaponId, "wear", it->second->m_fFallbackWear);
			config.SetInt(pWeaponId, "quality", it->second->m_iEntityQuality);
			config.SetString(pWeaponId, "name", it->second->m_pCustomName);
		}
	}

	return config.SaveFile(pFullFilename);
}

bool CSkinChanger::LoadFromConfig(const char* pFilename)
{
	char pFullFilename[MAX_PATH];
	snprintf(pFullFilename, MAX_PATH, "%s%s", SKINCHANGER_CONFIG_FOLDER, pFilename);

	CConfig config;
	config.Init(m_pApp);
	if (!config.LoadFile(pFullFilename))
		return false;

	this->ApplyDesiredKnife(TEAMNUM_CT, config.GetInt("knife$ct", "id"), false);
	this->ApplyDesiredKnife(TEAMNUM_T, config.GetInt("knife$t", "id"), false);

	int iWeaponId = 0;
	int iTeamNum = 0;
	
	const char* pWeaponId;
	const char* pTeamId;

	char pBuffer[128];
	const char* pSection = config.GetFirstSection();
	for (; pSection; pSection = config.GetNextSection())
	{
		if (!strncmp(pSection, "knife", 5))
			continue;

		strcpy(pBuffer, pSection);
		pWeaponId = strtok(pBuffer, "$");
		pTeamId = strtok(NULL, "$");

		iWeaponId = atoi(pWeaponId);
		iTeamNum = pTeamId[0] == 't' ? TEAMNUM_T : TEAMNUM_CT;

		this->AddSkinReplacement(
			iTeamNum,
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
	}

	return true;
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
