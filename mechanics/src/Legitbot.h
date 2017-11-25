#ifndef __LEGITBOT_H__
#define __LEGITBOT_H__

// Source SDK
#include "Vector.h"
#include "ISurface.h"

// Custom
#include "IFeature.h"

#define TARGET_HITBOX_HEAD						0
#define TARGET_HITBOX_CHEST						1
#define TARGET_HITBOX_PELVIS					2
#define TARGET_HITBOX_RIGHT_FOREARM				3
#define TARGET_HITBOX_LEFT_FOREARM				4
#define TARGET_HITBOX_RIGHT_CALF				5
#define TARGET_HITBOX_LEFT_CALF					6

#define TARGET_HITBOX_COUNT						7

class CLegitbot : public IFeature
{
public:
	CLegitbot();
	virtual ~CLegitbot();

	void SetTriggerKey(int iTriggerKey) { m_iTriggerKey = iTriggerKey; }
	int GetTriggerKey() { return m_iTriggerKey; }

	void SetAutoshoot(bool bAutoshoot) { m_bAutoshoot = bAutoshoot; }
	bool GetAutoshoot() { return m_bAutoshoot; }

	void SetHelpAfterShots(int iHelpAfterShots) { m_iHelpAfterShots = iHelpAfterShots; }
	int GetHelpAfterShots() { return m_iHelpAfterShots; }

	void SetTimeToAim(float fTimeToAim) { m_fTimeToAim = fTimeToAim; }
	float GetTimeToAim() { return m_fTimeToAim; }

	// Curve amount of the path (determines position of intermediate point)
	void SetCurve(float fCurve) { m_fCurve = fCurve; }
	float GetCurve() { return m_fCurve; }

	void SetFieldOfView(float fFieldOfView) { m_fFieldOfView = fFieldOfView; }
	float GetFieldOfView() { return m_fFieldOfView; }

	void SetPointScale(float fPointScale) { m_fPointScale = fPointScale; }
	float GetPointScale() { return m_fPointScale; }

	void SetDrawFieldOfView(bool bDrawFieldOfView) { m_bDrawFieldOfView = bDrawFieldOfView; }
	bool GetDrawFieldOfView() { return m_bDrawFieldOfView; }

	void SetFieldOfViewColor(Color clrFieldOfView) { m_clrFieldOfView = clrFieldOfView; }
	Color GetFieldOfViewColor() { return m_clrFieldOfView; }

	void SetDrawPath(bool bDrawPath) { m_bDrawPath = bDrawPath; }
	bool GetDrawPath() { return m_bDrawPath; }

	void SetPathColor(Color clrPath) { m_clrPath = clrPath; }
	Color GetPathColor() { return m_clrPath; }

	void SetCheckHitbox(int iHitbox, bool bCheckHitbox) { m_bCheckHitbox[iHitbox] = bCheckHitbox; }
	bool GetCheckHitbox(int iHitbox) { return m_bCheckHitbox[iHitbox]; }

	virtual void Think(void* pParameters = nullptr) override;
	void DrawFieldOfView(ISurface* pSurface);
	void DrawPath(ISurface* pSurface);
private:
	QAngle QuadraticBezier(QAngle& p0, QAngle& p1, QAngle& p2, float t);

	int m_iTriggerKey;

	bool m_bAutoshoot;
	int m_iHelpAfterShots;
	float m_fTimeToAim;
	float m_fCurve;
	float m_fFieldOfView;
	float m_fPointScale;

	bool m_bDrawFieldOfView;
	Color m_clrFieldOfView;
	bool m_bDrawPath;
	Color m_clrPath;

	bool m_bHasTarget;
	int m_iTarget;
	float m_fStepSize;
	float m_fAimProgress;
	bool m_bShoot;
	int m_iOldShotsFired;

	bool m_bHasDrawTarget;

	QAngle m_qStart;
	QAngle m_qIntermediate;
	QAngle m_qEnd;

	int m_iHitboxes[TARGET_HITBOX_COUNT];
	bool m_bCheckHitbox[TARGET_HITBOX_COUNT];
};

#endif // __LEGITBOT_H__