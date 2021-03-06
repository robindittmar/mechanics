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
	extern int m_flFlashDuration;
	extern int m_flFlashMaxAlpha;
	extern int m_bMoveType;
	extern int m_bIsScoped;
	extern int m_iShotsFired;
	extern int m_hActiveWeapon;
	extern int m_hObserverTarget;
	extern int deadflag;
	extern int m_nTickBase;
	extern int m_bIsDefusing;
	extern int m_flC4Blow;
	extern int m_flDefuseCountDown;
	extern int m_hBombDefuser;
	extern int m_bHasHeavyArmor;
    extern int m_flDuckSpeed;
    extern int m_flDuckAmount;

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

	// DT_BaseAttributableItem
	extern int m_iItemIDHigh;
	extern int m_iItemIDLow;
	extern int m_iAccountID;
	extern int m_iEntityQuality;
	extern int m_szCustomName;
	extern int m_OriginalOwnerXuidLow;
	extern int m_OriginalOwnerXuidHigh;
	extern int m_nFallbackPaintKit;
	extern int m_nFallbackSeed;
	extern int m_flFallbackWear;
	extern int m_nFallbackStatTrak;

	// DT_BaseViewModel
	namespace DT_BaseViewModel
	{
		extern int m_hWeapon;
		extern int m_hOwner;
	}

	// DT_BaseAnimating
	extern int m_nSequence;
	extern int m_flPoseParameter;
	extern int m_flCycle;
}

#endif // __OFFSETS_H__