#include "ClientClass.h"

bool ClientClass::IsWeapon()
{
	if (this->m_ClassID >= 204 && this->m_ClassID <= 244 ||
		this->m_ClassID == CAK47 ||
		this->m_ClassID == CDEagle ||
		this->m_ClassID == CC4)
		return true;
	return false;
}

bool ClientClass::IsGrenade()
{
	if (this->m_ClassID == CBaseCSGrenadeProjectile ||
		this->m_ClassID == CDecoyProjectile ||
		this->m_ClassID == CSmokeGrenadeProjectile ||
		this->m_ClassID == CMolotovProjectile)
		return true;
	return false;
}