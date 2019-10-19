#ifndef __CWEAPON_H__
#define __CWEAPON_H__

#include "../core/Offsets.h"
#include "Vector.h"

#define OFFSET_ARMORRATIO			0x850
#define OFFSET_PENETRATION			0x864
#define OFFSET_DAMAGE				0x868
#define OFFSET_RANGE				0x86C
#define OFFSET_RANGEMODIFIER		0x870

enum ItemDefinitionIndex : int {
	INVALID = -1,
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034
};

class CWeaponInfo
{
public:
	char pad_0x0000[0x4]; //0x0000
	char* m_WeaponName; //0x0004 
	char pad_0x0008[0xC]; //0x0008
	__int32 m_MaxClip; //0x0014 
	char pad_0x0018[0xC]; //0x0018
	__int32 m_MaxReservedAmmo; //0x0024 
	char pad_0x0028[0x4]; //0x0028
	char* m_WeaponMdlPath; //0x002C 
	char pad_0x0030[0x4]; //0x0030
	char* m_DropWeaponMdlPath; //0x0034 
	char pad_0x0038[0x48]; //0x0038
	char* m_BulletType; //0x0080 
	char pad_0x0084[0x4]; //0x0084
	char* m_WeaponHudName; //0x0088 
	char pad_0x008C[0x40]; //0x008C
	__int32 m_WeaponType; //0x00CC 
	__int32 m_WeaponPrice; //0x00D0 
	__int32 m_WeaponReward; //0x00D4 
	char* m_WeaponGroupName; //0x00D8 
	char pad_0x00DC[0x10]; //0x00DC
	unsigned char m_FullAuto; //0x00EC 
	char pad_0x00ED[0x3]; //0x00ED
	__int32 m_WeaponDamage; //0x00F0 
	float m_ArmorRatio; //0x00F4 
	__int32 m_iBullets;
	float m_Penetration; //0x00F8
	char pad_0x00F8[0x8]; //0x00FC
	float m_WeaponRange; //0x0108 
	float m_RangeModifier; //0x010C 
	char pad_0x0110[0x10]; //0x0110
	unsigned char m_HasSilencer; //0x0120 
	char pad_0x0121[0xF]; //0x0121
	float m_MaxSpeed; //0x0130 
	float m_MaxSpeed2; //0x0134 
	char pad_0x0138[0x4C]; //0x0138
	__int32 m_RecoilSeed; //0x0184 
	char pad_0x0188[0x68]; //0x0188
	char* m_WeaponTracesType; //0x01F0 
	char pad_0x01F4[0x638]; //0x01F4
};

class CWeapon
{
public:
	short GetWeaponId();
	bool IsWeaponX(int weaponId);
	bool IsKnife();
	bool IsSniper();
	bool IsNade();
	bool IsC4();
	bool IsPistol();
	bool IsTaser();
	int GetOwnerId();
	int GetClip1();
	int GetClip2();
	float GetNextPrimaryAttack();
	CWeaponInfo* GetWeaponInfo();
	float GetAccuracyPenalty();
	float GetPostPoneFireReady();

	Vector GetVecMin();
	Vector GetVecMax();

	void UpdateAccuracyPenalty();
	float GetInaccuracy();
	float GetSpread();
private:
};

class CGrenade {
public:
	float GetThrowTime();
private:
};

#endif // __CWEAPON_H__