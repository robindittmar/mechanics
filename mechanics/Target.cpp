#include "Target.h"

CTarget::CTarget()
{
	this->Invalidate();
}

CTarget::~CTarget()
{
}

void CTarget::SetTarget(Vector& vAimPoint, QAngle& qAimAngles, IClientEntity* pEntity, float fViewAngleDist, float fOriginDist, int bIsBacktracked)
{
	m_vAimPoint = vAimPoint;
	m_qAimAngles = qAimAngles;
	m_pEntity = pEntity;
	m_bIsValid = true;
	m_fViewAngleDist = fViewAngleDist;
	m_fOriginDist = fOriginDist;
	m_iIsBacktracked = bIsBacktracked;
}

void CTarget::Invalidate()
{
	m_bIsValid = false;
	m_vAimPoint = Vector(0, 0, 0);
	m_qAimAngles = QAngle(0, 0, 0);
	m_pEntity = NULL;
	m_fViewAngleDist = INVALID_DISTANCE;
	m_fOriginDist = INVALID_DISTANCE;
	m_iIsBacktracked = false;
}
