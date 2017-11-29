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

	static CXorString xorKnifeCTModel("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄¶rm‰∑{⁄°c%Ë¶{");
	static CXorString xorKnifeTModel("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄¶rm‰∑{⁄∂9f·Æ");
	static CXorString xorKnifeBayonetModel("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄†vrÍ¨r´Øsg");
	static CXorString xorKnifeButterflyModel("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄†bÒßemÈª9f·Æ");
	static CXorString xorKnifeFalchionModel("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄§vgÊ™~dÎùvoÛ£yh‡¶9f·Æ");
	static CXorString xorKnifeFlipModel("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄§{bıÏzoÈ");
	static CXorString xorKnifeGutModel("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄•b´Øsg");
	static CXorString xorKnifeM9BayonetModel("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄Ø.TÁ£n%Ë¶{");
	static CXorString xorKnifeKarambitModel("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄©vy‰Ø9f·Æ");
	static CXorString xorKnifePushModel("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄≤bxÌÏzoÈ");
	static CXorString xorKnifeSurvivalBowieModel("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄±byÛ´ajÈùudÚ´r%Ë¶{");
	static CXorString xorKnifeTacticalModel("zd·ß{x™µrjı≠yx™¥H`Î´qn⁄∂vhÒ´tjÈÏzoÈ");

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

	static CXorString xorDeagle(/*weapon_deagle*/"`n‰≤xe⁄¶rj‚Ær");
	static CXorString xorElite(/*weapon_elite*/"`n‰≤xe⁄ß{bÒß");
	static CXorString xorFiveseven(/*weapon_fiveseven*/"`n‰≤xe⁄§~}‡±r}‡¨");
	static CXorString xorGlock(/*weapon_glock*/"`n‰≤xe⁄•{dÊ©");
	static CXorString xorAk47(/*weapon_ak47*/"`n‰≤xe⁄£|?≤");
	static CXorString xorAug(/*weapon_aug*/"`n‰≤xe⁄£bl");
	static CXorString xorAwp(/*weapon_awp*/"`n‰≤xe⁄£`{");
	static CXorString xorFamas(/*weapon_famas*/"`n‰≤xe⁄§vf‰±");
	static CXorString xorG3sg1(/*weapon_g3sg1*/"`n‰≤xe⁄•$x‚Û");
	static CXorString xorGalil(/*weapon_galilar*/"`n‰≤xe⁄•vgÏÆvy");
	static CXorString xorM249(/*weapon_m249*/"`n‰≤xe⁄Ø%?º");
	static CXorString xorM4a1(/*weapon_m4a1*/"`n‰≤xe⁄Ø#j¥");
	static CXorString xorMac10(/*weapon_mac10*/"`n‰≤xe⁄Øvh¥Ú");
	static CXorString xorP90(/*weapon_p90*/"`n‰≤xe⁄≤.;");
	static CXorString xorUmp45(/*weapon_ump45*/"`n‰≤xe⁄∑z{±˜");
	static CXorString xorXm1014(/*weapon_xm1014*/"`n‰≤xe⁄∫z:µÛ#");
	static CXorString xorBizon(/*weapon_bizon*/"`n‰≤xe⁄†~qÍ¨");
	static CXorString xorMag7(/*weapon_mag7*/"`n‰≤xe⁄Øvl≤");
	static CXorString xorNegev(/*weapon_negev*/"`n‰≤xe⁄¨rl‡¥");
	static CXorString xorSawedoff(/*weapon_sawedoff*/"`n‰≤xe⁄±v|‡¶xm„");
	static CXorString xorTec9(/*weapon_tec9*/"`n‰≤xe⁄∂rhº");
	static CXorString xorHkp2000(/*weapon_hkp2000*/"`n‰≤xe⁄™|{∑Ú';");
	static CXorString xorMp7(/*weapon_mp7*/"`n‰≤xe⁄Øg<");
	static CXorString xorMp9(/*weapon_mp9*/"`n‰≤xe⁄Øg2");
	static CXorString xorNova(/*weapon_nova*/"`n‰≤xe⁄¨x}‰");
	static CXorString xorP250(/*weapon_p250*/"`n‰≤xe⁄≤%>µ");
	static CXorString xorScar20(/*weapon_scar20*/"`n‰≤xe⁄±tj˜'");
	static CXorString xorSg556(/*weapon_sg556*/"`n‰≤xe⁄±p>∞Ù");
	static CXorString xorSsg08(/*weapon_ssg08*/"`n‰≤xe⁄±dlµ˙");
	static CXorString xorM4a1Silencer(/*weapon_m4a1_silencer*/"`n‰≤xe⁄Ø#j¥ùdbÈßyh‡∞");
	static CXorString xorUspSilencer(/*weapon_usp_silencer*/"`n‰≤xe⁄∑d{⁄±~g‡¨tn˜");
	static CXorString xorCz75(/*weapon_cz75a*/"`n‰≤xe⁄°m<∞£");
	static CXorString xorRevolver(/*weapon_revolver*/"`n‰≤xe⁄∞r}ÍÆan˜");

	static CXorString xorBayonet(/*weapon_bayonet*/"`n‰≤xe⁄†vrÍ¨r");
	static CXorString xorKnifeFlip(/*weapon_knife_flip*/"`n‰≤xe⁄©yb„ßHmÈ´g");
	static CXorString xorKnifeGut(/*weapon_knife_gut*/"`n‰≤xe⁄©yb„ßHl∂");
	static CXorString xorKnifeKarambit(/*weapon_knife_karambit*/"`n‰≤xe⁄©yb„ßH`‰∞vfÁ´c");
	static CXorString xorKnifeM9Bayonet(/*weapon_knife_m9_bayonet*/"`n‰≤xe⁄©yb„ßHfºùuj¸≠ynÒ");
	static CXorString xorKnifeTactical(/*weapon_knife_tactical*/"`n‰≤xe⁄©yb„ßH‰°cbÊ£{");
	static CXorString xorKnifeFalchion(/*weapon_knife_falchion*/"`n‰≤xe⁄©yb„ßHm‰ÆtcÏ≠y");
	static CXorString xorKnifeSurvivalBowie(/*weapon_knife_survival_bowie*/"`n‰≤xe⁄©yb„ßHx∞abÛ£{TÁ≠`b‡");
	static CXorString xorKnifeButterfly(/*weapon_knife_butterfly*/"`n‰≤xe⁄©yb„ßHi∂cn˜§{r");
	static CXorString xorKnifePush(/*weapon_knife_push*/"`n‰≤xe⁄©yb„ßH{±");

	static CXorString xorDeagleReadable(/*Deagle*/"Sn‰•{n");
	static CXorString xorEliteReadable(/*Elite*/"RgÏ∂r");
	static CXorString xorFivesevenReadable(/*Fiveseven*/"QbÛßdnÛßy");
	static CXorString xorGlockReadable(/*Glock*/"PgÍ°|");
	static CXorString xorAk47Readable(/*AK-47*/"V@®ˆ ");
	static CXorString xorAugReadable(/*AUG*/"V^¬");
	static CXorString xorAwpReadable(/*AWP*/"V\\’");
	static CXorString xorFamasReadable(/*Famas*/"QjË£d");
	static CXorString xorG3sg1Readable(/*G3SG1*/"P8÷Ö&");
	static CXorString xorGalilReadable(/*Galil AR*/"PjÈ´{+ƒê");
	static CXorString xorM249Readable(/*M249*/"Z9±˚");
	static CXorString xorM4a1Readable(/*M4A1*/"Z?ƒÛ");
	static CXorString xorMac10Readable(/*Mac-10*/"ZjÊÔ&;");
	static CXorString xorP90Readable(/*P90*/"G2µ");
	static CXorString xorUmp45Readable(/*UMP-45*/"BF’Ô#>");
	static CXorString xorXm1014Readable(/*XM1014*/"OF¥Ú&?");
	static CXorString xorBizonReadable(/*Bizon*/"Ubˇ≠y");
	static CXorString xorMag7Readable(/*Mag7*/"Zj‚ı");
	static CXorString xorNegevReadable(/*Negev*/"Yn‚ßa");
	static CXorString xorSawedoffReadable(/*Sawed off*/"DjÚßs+Í§q");
	static CXorString xorTec9Readable(/*Tec NEIN*/"CnÊ‚YNÃå"); // TODO: Tec-9 :D
	static CXorString xorHkp2000Readable(/*HKP-2000*/"_@’Ô%;µÚ");
	static CXorString xorMp7Readable(/*MP-7*/"Z[®ı");
	static CXorString xorMp9Readable(/*MP-9*/"Z[®˚");
	static CXorString xorNovaReadable(/*Nova*/"YdÛ£");
	static CXorString xorP250Readable(/*P250*/"G9∞Ú");
	static CXorString xorScar20Readable(/*SCAR-20*/"DHƒê:9µ");
	static CXorString xorSg556Readable(/*SG-556*/"DL®˜\"=");
	static CXorString xorSsg08Readable(/*SSG-08*/"DX¬Ô'3");
	static CXorString xorM4a1SilencerReadable(/*M4A1-Silencer*/"Z?ƒÛ:XÏÆreÊße");
	static CXorString xorUspSilencerReadable(/*USP-Silencer*/"BX’ÔDbÈßyh‡∞");
	static CXorString xorCz75Readable(/*CZ75*/"TQ≤˜");
	static CXorString xorRevolverReadable(/*Revolver*/"EnÛ≠{}‡∞");

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

		int iWeaponId = *pWeapon->GetItemDefinitionIndex();

		this->ApplyCustomModel(pLocalEntity, pWeapon);

		if (m_bOnlyMyWeapons && pLocalInfo.xuidlow != *pWeapon->GetOriginalOwnerXuidLow())
			continue;

		this->ApplyCustomSkin(pWeapon, iWeaponId);

		// F¸r Stattrak (unknown user)
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

	int iCurPhase = -1;
	bool bEmerald = false;
	bool bRuby = false;
	bool bSapphire = false;
	bool bBlackPearl = false;

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
						char* pPhase = strstr(pValue, "phase");
						if (pPhase)
						{
							iCurPhase = *(pPhase + 5) - '0';
						}
						else
						{
							iCurPhase = -1;
						}
						
						bEmerald = strstr(pValue, "emerald") != nullptr;
						bRuby = strstr(pValue, "ruby") != nullptr;
						bSapphire = strstr(pValue, "sapphire") != nullptr;
						bBlackPearl = strstr(pValue, "blackpearl") != nullptr;

						iHash = murmurhash(pValue, strlen(pValue), 0xB16B00B5);
						m_mapPaintKits[iHash] = iCurPaintKit;
					}
					else if (!strcmp(pKey, "description_tag"))
					{
						wchar_t* pTranslation = m_pApp->Localize()->Find(pValue + 1);

						int iLen = wcslen(pTranslation) + 1;
						wchar_t* p;
						if (bEmerald)
						{
							p = new wchar_t[8];
							wcscpy(p, L"Emerald");
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
							swprintf(p, iLen, L"%ls Phase %d", pTranslation, iCurPhase);
						}

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

	std::sort(m_vWeapons.begin(), m_vWeapons.end(), [](const WeaponMetadata_t& a, const WeaponMetadata_t& b) {
		return strcmp(a.readableName, b.readableName) < 0;
	});
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

	static CXorString xorAttacker("vÒ£t`‡∞");
	static CXorString xorPlayerDeath("gg‰ªry⁄¶rjÒ™");
	static CXorString xorWeapon("`n‰≤xe");

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
			CXorString("|eÏ§rTË˚Hi‰ªxe‡∂").ToCharArray(),
			CXorString("uj¸≠ynÒ").ToCharArray()
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
