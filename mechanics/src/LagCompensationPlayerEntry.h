#ifndef __LAGCOMPENSATIONPLAYERENTRY_H__
#define __LAGCOMPENSATIONPLAYERENTRY_H__

#include "UserCmd.h"

#define MAXSTUDIOBONES				128
#define MAXSTUDIOPOSEPARAM			24

typedef unsigned long DWORD;

class CLagCompensationPlayerEntry
{
public:
	CLagCompensationPlayerEntry();

	int m_iTickCount;

	matrix3x4_t m_pBoneMatrix[MAXSTUDIOBONES];

	Vector m_vOrigin;
	Vector m_vHeadPos;
	Vector m_vVelocity;
	QAngle m_angEyeAngles;
	float m_fSimulationTime;
	int m_iSequenceNumber;
	DWORD m_fFlags;
	float m_fCycle;
	float m_fLowerBodyYaw;
	float m_fPoseParameters[MAXSTUDIOPOSEPARAM];

	float m_llAddTime;
	bool m_bIsEndOfList;

	//int FixedTickcount(IClientEntity* pCur);
private:
	//float LerpTime();
};

#endif // __LAGCOMPENSATIONPLAYERENTRY_H__