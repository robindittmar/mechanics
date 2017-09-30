#ifndef __OFFSETS_H__
#define __OFFSETS_H__

namespace Offsets
{
	// DT_BaseEntity
	extern int m_nModelIndex;
	extern int m_hMyWeapons;
	extern int m_hViewModel;
	extern int m_flSimulationTime;
	extern int m_vecMins;
	extern int m_vecMaxs;
	extern int m_angRotation;

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
	extern int deadflag;
	extern int m_nTickBase;
	extern int m_flC4Blow;

	// DT_Local
	extern int m_nJumpTimeMsecs;
	extern int m_flFallVelocity;
	extern int m_viewPunchAngle;
	extern int m_aimPunchAngle;
	extern int m_aimPunchAngleVel;
	
	// DT_BaseCombatWeapon
	extern int m_AttributeManager;
	extern int m_Item;
	extern int m_iItemDefinitionIndex;
	extern int m_hOwner;
	extern int m_iClip1;
	extern int m_iClip2;
	extern int m_flNextPrimaryAttack;
	//extern int m_iViewModelIndex;		// 0x31E4
	//extern int m_iWorldModelIndex;	// 0x31E8
	extern int m_hWeaponWorldModel;
	
	// DT_WeaponCSBase
	extern int m_fAccuracyPenalty;
	extern int m_flPostponeFireReadyTime;

	// DT_BaseCSGrenade
	extern int m_fThrowTime;
}

#endif // __OFFSETS_H__