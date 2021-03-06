#include "ClientClass.h"

bool ClientClass::IsWeapon()
{
	if (this->m_ClassID >= 231 && this->m_ClassID <= 272 ||
		this->m_ClassID == CAK47 ||
		this->m_ClassID == CDEagle ||
		this->m_ClassID == CC4 ||
		this->m_ClassID == CHEGrenade ||
		this->m_ClassID == CMolotovGrenade ||
		this->m_ClassID == CSmokeGrenade ||
		this->m_ClassID == CFlashbang ||
		this->m_ClassID == CDecoyGrenade)
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