#ifndef __TARGETSELECTOR_H__
#define __TARGETSELECTOR_H__

/*
TODO:

The whole TargetSelector at one point moved out of the Ragebot -- into the TargetSelector
The purpose of that was, to reuse the targets (or atleast the targetfinding code) in Ragebot/Legitbot/Antiaim, etc etc.

But today we know, that it's probably better to keep this in the Ragebot, as the targetselection is quiet different for each case
and if you need to know the current target (in AntiAim for example) you can just ask the Ragebot, instead of the Targetselector
(I guess you'll never use AntiAim with legitbot.. :D)
*/

// Source SDK
#include "ClientEntity.h"
#include "ClientEntityList.h"
#include "IVEngineClient.h"
#include "IVModelInfo.h"

// Custom
#include "Utils.h"
#include "Target.h"
#include "Autowall.h"

#define MAXSTUDIOBONES			128
#define BONE_USED_BY_HITBOX		0x00000100

// Defines how to choose the next target for the aimbot
// -> Origin: Chooses the target that's standing closest to you
// -> Viewangle: Choose the target that's closest to your crosshair
#define TARGETCRITERIA_UNSPECIFIED				0
#define TARGETCRITERIA_ORIGIN					1
#define TARGETCRITERIA_VIEWANGLES				2

// Take LAST 'TARGETCRITERIA_*' value and add 1
#define TARGETCRITERIA_COUNT					3

// Defines how to check for visibility
// -> ignore: no visibility check at all
// -> canhit: check if can do dmg
// -> fullvisible: fully visibile
#define VISIBLEMODE_IGNORE						0
#define VISIBLEMODE_CANHIT						1
#define VISIBLEMODE_FULLVISIBLE					2

#define TARGET_HITBOX_HEAD						0
#define TARGET_HITBOX_CHEST						1
#define TARGET_HITBOX_PELVIS					2
#define TARGET_HITBOX_RIGHT_FOREARM				3
#define TARGET_HITBOX_LEFT_FOREARM				4
#define TARGET_HITBOX_RIGHT_CALF				5
#define TARGET_HITBOX_LEFT_CALF					6

#define TARGET_HITBOX_COUNT						7

class CApplication;

class CTargetSelector
{
public:
	CTargetSelector();
	~CTargetSelector();

	void Setup(CApplication* pApp);

	void SetHasTargets(bool bHasTargets) { m_bHasTargets = bHasTargets; }
	bool GetHasTargets() { return m_bHasTargets; }

	void SelectTargets(float fInputSampleTime);
	CTarget* GetTarget(int iTargetCriteria) { return &m_pTargets[iTargetCriteria]; }

	void SetVisibleMode(int iVisibleMode) { m_iVisibleMode = iVisibleMode; }
	int GetVisibleMode() { return m_iVisibleMode; }

	void SetCheckHitbox(int iHitbox, bool bCheck) { m_bCheckHitbox[iHitbox] = bCheck; }
	bool GetCheckHitbox(int iHitbox) { return m_bCheckHitbox[iHitbox]; }

	void SetMultipoint(bool bMultipoint) { m_bMultipoint = bMultipoint; }
	bool GetMultipoint() { return m_bMultipoint; }

	void SetMultipointScale(float fMultipointScale) { m_fMultipointScale = fMultipointScale; }
	float GetMultipointScale() { return m_fMultipointScale; }
private:
	void GetHitBoxCenter(mstudiobbox_t* hitBox, matrix3x4_t* boneMatrix, Vector& hitBoxVector);
	void GetHitBoxVectors(mstudiobbox_t* hitBox, matrix3x4_t* boneMatrix, Vector* hitBoxVectors);

	float GetOriginDist(Vector& a, Vector& b);
	float GetViewangleDist(QAngle& a, QAngle& b/*, float fOriginDistance*/);

	void ResetTargets();

	bool m_bHasTargets;

	int m_iHitboxes[TARGET_HITBOX_COUNT];
	bool m_bCheckHitbox[TARGET_HITBOX_COUNT];

	int m_iVisibleMode;
	bool m_bMultipoint;
	float m_fMultipointScale;
	CTarget m_pTargets[TARGETCRITERIA_COUNT];

	CApplication* m_pApp;
	IVEngineClient* m_pEngineClient;
	IClientEntityList* m_pEntityList;
};

#endif // __TARGETSELECTOR_H__