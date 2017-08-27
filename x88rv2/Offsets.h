#ifndef __OFFSETS_H__
#define __OFFSETS_H__

namespace Offsets
{
	// DT_CSPlayer
	extern int m_vecOrigin;
	extern int m_vecViewOffset;
	extern int m_angEyeAngles;
	extern int m_flLowerBodyYawTarget;
	extern int m_vecVelocity;
	extern int m_iTeamNum;
	extern int m_lifeState;
	extern int m_iHealth;
	extern int m_ArmorValue;
	extern int m_bHasHelmet;
	extern int m_fFlags;
	extern int m_bSpotted;
	extern int m_bGunGameImmunity;
	extern int m_flFlashMaxAlpha;
	extern int m_bMoveType;
	extern int m_bIsScoped;
	extern int m_iShotsFired;
	extern int m_hActiveWeapon;
	extern int m_hObserverTarget;
	extern int m_deadflag;
	extern int m_nTickBase;

	// DT_BaseCombatWeapon
	/*extern int m_AttributeManager;
	extern int m_Item;*/
	extern int m_iItemDefinitionIndex;
	extern int m_iClip1;
	extern int m_iClip2;
	extern int m_flNextPrimaryAttack;
	
	// DT_WeaponCSBase
	extern int m_flPostponeFireReadyTime;

	// DT_BaseCSGrenade
	extern int m_fThrowTime;
}

#endif // __OFFSETS_H__