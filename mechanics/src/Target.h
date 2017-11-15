#ifndef __TARGET_H__
#define __TARGET_H__

#include "Vector.h"
#include "ClientEntity.h"

#define INVALID_DISTANCE -1.0f

class CTarget
{
public:
	CTarget();
	~CTarget();

	void SetTarget(Vector& vAimPoint, QAngle& qAimAngles, IClientEntity* pEntity, float fViewAngleDist = INVALID_DISTANCE, float fOriginDist = INVALID_DISTANCE, int bIsBacktracked = -1, float fDamage = 0.0f);
	void Invalidate();

	void SetValid(bool b) { m_bIsValid = b; }
	bool GetValid() { return m_bIsValid; }

	void SetAimPoint(Vector& v) { m_vAimPoint = v; }
	Vector* GetAimPoint() { return &m_vAimPoint; }
	
	void SetAimAngles(QAngle& q) { m_qAimAngles = q; }
	QAngle* GetAimAngles() { return &m_qAimAngles; }

	void SetEntity(IClientEntity* p) { m_pEntity = p; }
	IClientEntity* GetEntity() { return m_pEntity; }

	float GetViewAngleDist() { return m_fViewAngleDist; }

	float GetOriginDist() { return m_fOriginDist; }

	int GetIsBacktracked() { return m_iIsBacktracked; }
	float GetDamage() { return m_fDamage; }
private:
	bool m_bIsValid;
	Vector m_vAimPoint;
	QAngle m_qAimAngles;
	IClientEntity* m_pEntity;

	float m_fViewAngleDist;
	float m_fOriginDist;

	int m_iIsBacktracked;
	float m_fDamage;
};

#endif // __TARGET_H__