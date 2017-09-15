#include "Target.h"

CTarget::CTarget()
{
	this->Invalidate();
}

CTarget::~CTarget()
{
}

void CTarget::SetTarget(Vector& vAimPoint, QAngle& qAimAngles, IClientEntity* pEntity)
{
	m_vAimPoint = vAimPoint;
	m_qAimAngles = qAimAngles;
	m_pEntity = pEntity;
	m_bIsValid = true;
}

void CTarget::Invalidate()
{
	m_bIsValid = false;
	m_vAimPoint = Vector(0, 0, 0);
	m_qAimAngles = QAngle(0, 0, 0);
	m_pEntity = NULL;
}
