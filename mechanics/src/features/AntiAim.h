#ifndef __ANTIAIM_H__
#define __ANTIAIM_H__

#include "IFeature.h"
#include "../source_sdk/Vector.h"
#include "../core/ResolverPlayer.h"
#include "../source_sdk/ClientEntity.h"
#include "../source_sdk/UserCmd.h"
#include "../core/Target.h"

class CApplication;

// Pitch
#define PITCHANTIAIM_NONE						0
#define PITCHANTIAIM_UP							1
#define PITCHANTIAIM_DOWN						2

// Yaw
#define YAWANTIAIM_NONE							0
#define YAWANTIAIM_ADAPTIVE						1
#define YAWANTIAIM_STATIC						2
#define YAWANTIAIM_BACKWARDS					3
#define YAWANTIAIM_STATICJITTERBACKWARDS		4

// Yaw Fake
#define FAKEYAWANTIAIM_NONE						0
#define FAKEYAWANTIAIM_ADAPTIVE					1
#define FAKEYAWANTIAIM_STATIC					2
#define FAKEYAWANTIAIM_STATICJITTER				3


// Hide Direction
#define HIDEDIRECTION_INVALID					-1
#define HIDEDIRECTION_NEXT						0
#define HIDEDIRECTION_RIGHT						1
#define HIDEDIRECTION_LEFT						2

#define EDGEANTIAIM_DAMAGEOFFSET				10
#define TARGETCRITERIA_COUNT					3

#define EDGEANTIAIMPOINT_NODAMAGE				0
#define EDGEANTIAIMPOINT_LESSDAMAGE				1
#define EDGEANTIAIMPOINT_HIGHERDAMAGE			2
#define EDGEANTIAIMPOINT_HIGHESTDAMAGE			3

struct EdgeAntiAimPoint
{
	Vector vPoint;
	int iIsHidden;
};

class CAntiAim : public IFeature
{
public:
	CAntiAim();
	virtual ~CAntiAim();

	// Standing
	void SetPitchSettingStanding(int iPitchSetting) { m_iPitchSettingStanding = iPitchSetting; }
	int GetPitchSettingStanding() { return m_iPitchSettingStanding; }

	void SetYawSettingStanding(int iYawSetting) { m_iYawSettingStanding = iYawSetting; }
	int GetYawSettingStanding() { return m_iYawSettingStanding; }

	void SetYawFakeSettingStanding(int iYawFakeSetting) { m_iYawFakeSettingStanding = iYawFakeSetting; }
	int GetYawFakeSettingStanding() { return m_iYawFakeSettingStanding; }

	void SetYawOffsetStanding(float fYawOffset) { m_fYawOffsetStanding = fYawOffset; }
	float GetYawOffsetStanding() { return m_fYawOffsetStanding; }

	void SetYawFakeOffsetStanding(float fYawFakeOffset) { m_fYawFakeOffsetStanding = fYawFakeOffset; }
	float GetYawFakeOffsetStanding() { return m_fYawFakeOffsetStanding; }

	// Moving
	void SetPitchSettingMoving(int iPitchSetting) { m_iPitchSettingMoving = iPitchSetting; }
	int GetPitchSettingMoving() { return m_iPitchSettingMoving; }

	void SetYawSettingMoving(int iYawSetting) { m_iYawSettingMoving = iYawSetting; }
	int GetYawSettingMoving() { return m_iYawSettingMoving; }

	void SetYawFakeSettingMoving(int iYawFakeSetting) { m_iYawFakeSettingMoving = iYawFakeSetting; }
	int GetYawFakeSettingMoving() { return m_iYawFakeSettingMoving; }

	void SetYawOffsetMoving(float fYawOffset) { m_fYawOffsetMoving = fYawOffset; }
	float GetYawOffsetMoving() { return m_fYawOffsetMoving; }

	void SetYawFakeOffsetMoving(float fYawFakeOffset) { m_fYawFakeOffsetMoving = fYawFakeOffset; }
	float GetYawFakeOffsetMoving() { return m_fYawFakeOffsetMoving; }

	void SetDrawLbyIndicator(bool bDrawLbyIndicator) { m_bDrawLbyIndicator = bDrawLbyIndicator; }
	bool GetDrawLbyIndicator() { return m_bDrawLbyIndicator; }

	void SetLbyBreaker(bool bLbyBreaker) { m_bLbyBreaker = bLbyBreaker; }
	bool GetLbyBreaker() { return m_bLbyBreaker; }

	void SetDoEdgeAntiAim(bool bDoEdgeAntiAim) { m_bDoEdgeAntiAim = bDoEdgeAntiAim; }
	bool GetDoEdgeAntiAim() { return m_bDoEdgeAntiAim; }

	void SetEdgeAntiAimCheckPointsAmount(int iEdgeAntiAimCheckPointAmount);
	int GetEdgeAntiAimCheckPointsAmount() { return m_iEdgeAntiAimCheckPointAmount; }

	void SetDrawEdgeAntiAimPoints(bool bDrawEdgeAntiAimPoints) { m_bDrawEdgeAntiAimPoints = bDrawEdgeAntiAimPoints; }
	bool GetDrawEdgeAntiAimPoints() { return m_bDrawEdgeAntiAimPoints; }

	void SetDrawEdgeAntiAimLines(bool bDrawEdgeAntiAimLines) { m_bDrawEdgeAntiAimLines = bDrawEdgeAntiAimLines; }
	bool GetDrawEdgeAntiAimLines() { return m_bDrawEdgeAntiAimLines; }

	float GetRealYaw() { return m_fCurRealYaw; }
	float GetFakeYaw() { return m_fCurFakeYaw; }
	bool IsLbyUpdate() { return m_bNextLbyUpdate && m_bLbyBreaker; }

	bool IsFakeYaw();
	bool NextLBYUpdate(CResolverPlayer* pResolverPlayer, bool bIsLocalPlayer = false, bool bIsFakeWalking = false);

	void DrawLBYIndicator();
	bool m_bNextLbyUpdate;

	CTarget* GetTarget(int iTargetCriteria) { return &m_pTargets[iTargetCriteria]; }

	void DrawEdgeAntiAimPoints();

	virtual void Think(void* pParameters = nullptr) override;
private:
	bool m_bFakeAngleSwitch;

	// Standing
	int m_iPitchSettingStanding;
	int m_iYawSettingStanding;
	int m_iYawFakeSettingStanding;

	float m_fYawOffsetStanding;
	float m_fYawFakeOffsetStanding;

	// Moving
	int m_iPitchSettingMoving;
	int m_iYawSettingMoving;
	int m_iYawFakeSettingMoving;

	float m_fYawOffsetMoving;
	float m_fYawFakeOffsetMoving;

	bool m_bDrawLbyIndicator;
	bool m_bLbyBreaker;

	bool m_bDoEdgeAntiAim;
	bool m_bIsEdgeAntiAim;
	bool m_bEdgeAntiAimFakeSwitch;

	int m_iEdgeAntiAimCheckPointAmount;
	bool m_bDrawEdgeAntiAimPoints;
	bool m_bDrawEdgeAntiAimLines;

	float m_fCurRealYaw;
	float m_fCurFakeYaw;

	bool m_bIsMoving;
	bool m_bWasMoving;

	bool m_bHasTargets;
	CTarget m_pTargets[TARGETCRITERIA_COUNT];

	EdgeAntiAimPoint* m_pEdgeAntiAimPointsRight;
	EdgeAntiAimPoint* m_pEdgeAntiAimPointsLeft;

	void HideHead(CUserCmd* pUserCmd, QAngle qAimAngle, int iHideDirection);
	int CheckHeadPoint(IClientEntity* pEnemy, int iIndex);
	bool EdgeAntiAim(IClientEntity* pLocalEntity, CUserCmd* pUserCmd);

	void GetAntiAimTargets();
	float GetOriginDist(Vector& a, Vector& b);
	float GetViewangleDist(QAngle& a, QAngle& b);

	void ApplyPitchAntiAim(QAngle* angles);
	void ApplyYawAntiAim(QAngle* angles);
	void ApplyYawFakeAntiAim(QAngle* angles, float fRealAngle);

	int m_iInJumpCount;
	int m_iInJumpPredCount;
	float m_fInJumpAngPerTick;

	void CheckForInJumpAntiAim();
	float GetInJumpAntiAim();
};

#endif // __ANTIAIM_H__