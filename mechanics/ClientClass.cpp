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