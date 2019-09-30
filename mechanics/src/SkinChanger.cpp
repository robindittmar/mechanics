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

	static CXorString xorKnifeCTModel(/*models/weapons/v_knife_default_ct.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xA6\x72\x6D\xE4\xB7\x7B\x7F\xDA\xA1\x63\x25\xE8\xA6\x7B");
	static CXorString xorKnifeTModel(/*models/weapons/v_knife_t.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xB6\x39\x66\xE1\xAE");
	static CXorString xorKnifeBayonetModel(/*models/weapons/v_knife_bayonet.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xA0\x76\x72\xEA\xAC\x72\x7F\xAB\xAF\x73\x67");
	static CXorString xorKnifeButterflyModel(/*models/weapons/v_knife_butterfly.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xA0\x62\x7F\xF1\xA7\x65\x6D\xE9\xBB\x39\x66\xE1\xAE");
	static CXorString xorKnifeFalchionModel(/*models/weapons/v_knife_falchion_advanced.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xA4\x76\x67\xE6\xAA\x7E\x64\xEB\x9D\x76\x6F\xF3\xA3\x79\x68\xE0\xA6\x39\x66\xE1\xAE");
	static CXorString xorKnifeFlipModel(/*models/weapons/v_knife_flip.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xA4\x7B\x62\xF5\xEC\x7A\x6F\xE9");
	static CXorString xorKnifeGutModel(/*models/weapons/v_knife_gut.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xA5\x62\x7F\xAB\xAF\x73\x67");
	static CXorString xorKnifeM9BayonetModel(/*models/weapons/v_knife_m9_bay.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xAF\x2E\x54\xE7\xA3\x6E\x25\xE8\xA6\x7B");
	static CXorString xorKnifeKarambitModel(/*models/weapons/v_knife_karam.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xA9\x76\x79\xE4\xAF\x39\x66\xE1\xAE");
	static CXorString xorKnifePushModel(/*models/weapons/v_knife_push.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xB2\x62\x78\xED\xEC\x7A\x6F\xE9");
	static CXorString xorKnifeSurvivalBowieModel(/*models/weapons/v_knife_survival_bowie.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xB1\x62\x79\xF3\xAB\x61\x6A\xE9\x9D\x75\x64\xF2\xAB\x72\x25\xE8\xA6\x7B");
	static CXorString xorKnifeTacticalModel(/*models/weapons/v_knife_tactical.mdl*/"\x7A\x64\xE1\xA7\x7B\x78\xAA\xB5\x72\x6A\xF5\xAD\x79\x78\xAA\xB4\x48\x60\xEB\xAB\x71\x6E\xDA\xB6\x76\x68\xF1\xAB\x74\x6A\xE9\xEC\x7A\x6F\xE9");

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

	static CXorString xorDeagle(/*weapon_deagle*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA6\x72\x6A\xE2\xAE\x72");
	static CXorString xorElite(/*weapon_elite*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA7\x7B\x62\xF1\xA7");
	static CXorString xorFiveseven(/*weapon_fiveseven*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA4\x7E\x7D\xE0\xB1\x72\x7D\xE0\xAC");
	static CXorString xorGlock(/*weapon_glock*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA5\x7B\x64\xE6\xA9");
	static CXorString xorAk47(/*weapon_ak47*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA3\x7C\x3F\xB2");
	static CXorString xorAug(/*weapon_aug*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA3\x62\x6C");
	static CXorString xorAwp(/*weapon_awp*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA3\x60\x7B");
	static CXorString xorFamas(/*weapon_famas*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA4\x76\x66\xE4\xB1");
	static CXorString xorG3sg1(/*weapon_g3sg1*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA5\x24\x78\xE2\xF3");
	static CXorString xorGalil(/*weapon_galilar*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA5\x76\x67\xEC\xAE\x76\x79");
	static CXorString xorM249(/*weapon_m249*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xAF\x25\x3F\xBC");
	static CXorString xorM4a1(/*weapon_m4a1*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xAF\x23\x6A\xB4");
	static CXorString xorMac10(/*weapon_mac10*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xAF\x76\x68\xB4\xF2");
	static CXorString xorP90(/*weapon_p90*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xB2\x2E\x3B");
	static CXorString xorUmp45(/*weapon_ump45*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xB7\x7A\x7B\xB1\xF7");
	static CXorString xorXm1014(/*weapon_xm1014*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xBA\x7A\x3A\xB5\xF3\x23");
	static CXorString xorBizon(/*weapon_bizon*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA0\x7E\x71\xEA\xAC");
	static CXorString xorMag7(/*weapon_mag7*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xAF\x76\x6C\xB2");
	static CXorString xorNegev(/*weapon_negev*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xAC\x72\x6C\xE0\xB4");
	static CXorString xorSawedoff(/*weapon_sawedoff*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xB1\x76\x7C\xE0\xA6\x78\x6D\xE3");
	static CXorString xorTec9(/*weapon_tec9*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xB6\x72\x68\xBC");
	static CXorString xorHkp2000(/*weapon_hkp2000*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xAA\x7C\x7B\xB7\xF2\x27\x3B");
	static CXorString xorMp7(/*weapon_mp7*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xAF\x67\x3C");
	static CXorString xorMp9(/*weapon_mp9*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xAF\x67\x32");
	static CXorString xorNova(/*weapon_nova*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xAC\x78\x7D\xE4");
	static CXorString xorP250(/*weapon_p250*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xB2\x25\x3E\xB5");
	static CXorString xorScar20(/*weapon_scar20*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xB1\x74\x6A\xF7\xF0\x27");
	static CXorString xorSg556(/*weapon_sg556*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xB1\x70\x3E\xB0\xF4");
	static CXorString xorSsg08(/*weapon_ssg08*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xB1\x64\x6C\xB5\xFA");
	static CXorString xorM4a1Silencer(/*weapon_m4a1_silencer*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xAF\x23\x6A\xB4\x9D\x64\x62\xE9\xA7\x79\x68\xE0\xB0");
	static CXorString xorUspSilencer(/*weapon_usp_silencer*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xB7\x64\x7B\xDA\xB1\x7E\x67\xE0\xAC\x74\x6E\xF7");
	static CXorString xorCz75(/*weapon_cz75a*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA1\x6D\x3C\xB0\xA3");
	static CXorString xorRevolver(/*weapon_revolver*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xB0\x72\x7D\xEA\xAE\x61\x6E\xF7");

	static CXorString xorBayonet(/*weapon_bayonet*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA0\x76\x72\xEA\xAC\x72\x7F");
	static CXorString xorKnifeFlip(/*weapon_knife_flip*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA9\x79\x62\xE3\xA7\x48\x6D\xE9\xAB\x67");
	static CXorString xorKnifeGut(/*weapon_knife_gut*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA9\x79\x62\xE3\xA7\x48\x6C\xF0\xB6");
	static CXorString xorKnifeKarambit(/*weapon_knife_karambit*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA9\x79\x62\xE3\xA7\x48\x60\xE4\xB0\x76\x66\xE7\xAB\x63");
	static CXorString xorKnifeM9Bayonet(/*weapon_knife_m9_bayonet*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA9\x79\x62\xE3\xA7\x48\x66\xBC\x9D\x75\x6A\xFC\xAD\x79\x6E\xF1");
	static CXorString xorKnifeTactical(/*weapon_knife_tactical*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA9\x79\x62\xE3\xA7\x48\x7F\xE4\xA1\x63\x62\xE6\xA3\x7B");
	static CXorString xorKnifeFalchion(/*weapon_knife_falchion*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA9\x79\x62\xE3\xA7\x48\x6D\xE4\xAE\x74\x63\xEC\xAD\x79");
	static CXorString xorKnifeSurvivalBowie(/*weapon_knife_survival_bowie*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA9\x79\x62\xE3\xA7\x48\x78\xF0\xB0\x61\x62\xF3\xA3\x7B\x54\xE7\xAD\x60\x62\xE0");
	static CXorString xorKnifeButterfly(/*weapon_knife_butterfly*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA9\x79\x62\xE3\xA7\x48\x69\xF0\xB6\x63\x6E\xF7\xA4\x7B\x72");
	static CXorString xorKnifePush(/*weapon_knife_push*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA9\x79\x62\xE3\xA7\x48\x7B\xF0\xB1\x7F");

	static CXorString xorDeagleReadable(/*Deagle*/"\x53\x6E\xE4\xA5\x7B\x6E");
	static CXorString xorEliteReadable(/*Elite*/"\x52\x67\xEC\xB6\x72");
	static CXorString xorFivesevenReadable(/*Fiveseven*/"\x51\x62\xF3\xA7\x64\x6E\xF3\xA7\x79");
	static CXorString xorGlockReadable(/*Glock*/"\x50\x67\xEA\xA1\x7C");
	static CXorString xorAk47Readable(/*AK-47*/"\x56\x40\xA8\xF6\x20");
	static CXorString xorAugReadable(/*AUG*/"\x56\x5E\xC2");
	static CXorString xorAwpReadable(/*AWP*/"\x56\x5C\xD5");
	static CXorString xorFamasReadable(/*Famas*/"\x51\x6A\xE8\xA3\x64");
	static CXorString xorG3sg1Readable(/*G3SG1*/"\x50\x38\xD6\x85\x26");
	static CXorString xorGalilReadable(/*Galil AR*/"\x50\x6A\xE9\xAB\x7B\x2B\xC4\x90");
	static CXorString xorM249Readable(/*M249*/"\x5A\x39\xB1\xFB");
	static CXorString xorM4a1Readable(/*M4A1*/"\x5A\x3F\xC4\xF3");
	static CXorString xorMac10Readable(/*Mac-10*/"\x5A\x6A\xE6\xEF\x26\x3B");
	static CXorString xorP90Readable(/*P90*/"\x47\x32\xB5");
	static CXorString xorUmp45Readable(/*UMP-45*/"\x42\x46\xD5\xEF\x23\x3E");
	static CXorString xorXm1014Readable(/*XM1014*/"\x4F\x46\xB4\xF2\x26\x3F");
	static CXorString xorBizonReadable(/*Bizon*/"\x55\x62\xFF\xAD\x79");
	static CXorString xorMag7Readable(/*Mag7*/"\x5A\x6A\xE2\xF5");
	static CXorString xorNegevReadable(/*Negev*/"\x59\x6E\xE2\xA7\x61");
	static CXorString xorSawedoffReadable(/*Sawed off*/"\x44\x6A\xF2\xA7\x73\x2B\xEA\xA4\x71");
	static CXorString xorTec9Readable(/*Tec-9*/"\x43\x6E\xE6\xEF\x2E");
	static CXorString xorHkp2000Readable(/*HKP-2000*/"\x5F\x40\xD5\xEF\x25\x3B\xB5\xF2");
	static CXorString xorMp7Readable(/*MP-7*/"\x5A\x5B\xA8\xF5");
	static CXorString xorMp9Readable(/*MP-9*/"\x5A\x5B\xA8\xFB");
	static CXorString xorNovaReadable(/*Nova*/"\x59\x64\xF3\xA3");
	static CXorString xorP250Readable(/*P250*/"\x47\x39\xB0\xF2");
	static CXorString xorScar20Readable(/*SCAR-20*/"\x44\x48\xC4\x90\x3A\x39\xB5");
	static CXorString xorSg556Readable(/*SG-556*/"\x44\x4C\xA8\xF7\x22\x3D");
	static CXorString xorSsg08Readable(/*SSG-08*/"\x44\x58\xC2\xEF\x27\x33");
	static CXorString xorM4a1SilencerReadable(/*M4A1-Silencer*/"\x5A\x3F\xC4\xF3\x3A\x58\xEC\xAE\x72\x65\xE6\xA7\x65");
	static CXorString xorUspSilencerReadable(/*USP-Silencer*/"\x42\x58\xD5\xEF\x44\x62\xE9\xA7\x79\x68\xE0\xB0");
	static CXorString xorCz75Readable(/*CZ75*/"\x54\x51\xB2\xF7");
	static CXorString xorRevolverReadable(/*Revolver*/"\x45\x6E\xF3\xAD\x7B\x7D\xE0\xB0");

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

	strcat(pFilepathSkins, CXorString(/*scripts\\items\\items_game.txt*/"\x64\x68\xF7\xAB\x67\x7F\xF6\x9E\x4B\x62\xF1\xA7\x7A\x78\xD9\x9E\x7E\x7F\xE0\xAF\x64\x54\xE2\xA3\x7A\x6E\xAB\xB6\x6F\x7F"));

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

	CXorString xorName(/*name*/"\x79\x6A\xE8\xA7");
	CXorString xorPhase(/*phase*/"\x67\x63\xE4\xB1\x72");
	CXorString xorEmerald(/*emerald*/"\x72\x66\xE0\xB0\x76\x67\xE1");
	CXorString xorRuby(/*ruby*/"\x65\x7E\xE7\xBB");
	CXorString xorSapphire(/*sapphire*/"\x64\x6A\xF5\xB2\x7F\x62\xF7\xA7");
	CXorString xorBlackPearl(/*blackpearl*/"\x75\x67\xE4\xA1\x7C\x7B\xE0\xA3\x65\x67");
	CXorString xorDescriptionTag(/*description_tag*/"\x73\x6E\xF6\xA1\x65\x62\xF5\xB6\x7E\x64\xEB\x9D\x63\x6A\xE2");
	CXorString xorWeaponUnderscore(/*weapon_*/"\x60\x6E\xE4\xB2\x78\x65\xDA");
	CXorString xorUnderscoreLight(/*_light*/"\x48\x67\xEC\xA5\x7F\x7F");
	CXorString xorWeaponKnife(/*weapon_knife*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA9\x79\x62\xE3\xA7");
	CXorString xorWeaponBayonet(/*weapon_bayonet*/"\x60\x6E\xE4\xB2\x78\x65\xDA\xA0\x76\x72\xEA\xAC\x72\x7F");
	CXorString xorPaintKits(/*paint_kits*/"\x67\x6A\xEC\xAC\x63\x54\xEE\xAB\x63\x78");
	CXorString xorClientLootLists(/*client_loot_lists*/"\x74\x67\xEC\xA7\x79\x7F\xDA\xAE\x78\x64\xF1\x9D\x7B\x62\xF6\xB6\x64");
	CXorString xorAlternateIcons2(/*alternate_icons2*/"\x76\x67\xF1\xA7\x65\x65\xE4\xB6\x72\x54\xEC\xA1\x78\x65\xF6\xF0");

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

	static CXorString xorAttacker(/*attacker*/"\x76\x7F\xF1\xA3\x74\x60\xE0\xB0");
	static CXorString xorPlayerDeath(/*player_death*/"\x67\x67\xE4\xBB\x72\x79\xDA\xA6\x72\x6A\xF1\xAA");
	static CXorString xorWeapon(/*weapon*/"\x60\x6E\xE4\xB2\x78\x65");

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
		//AddKillIconReplacement(
		//	CXorString(/*knife_m9_bayonet*/"\x7C\x65\xEC\xA4\x72\x54\xE8\xFB\x48\x69\xE4\xBB\x78\x65\xE0\xB6").ToCharArray(),
		//	CXorString(/*bayonet*/"\x75\x6A\xFC\xAD\x79\x6E\xF1").ToCharArray()
		//);
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
