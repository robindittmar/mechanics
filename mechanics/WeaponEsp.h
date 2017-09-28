#ifndef __WEAPONESP_H__
#define __WEAPONESP_H__

// Source SDK
#include "ClientEntity.h"

#include "IFeature.h"

class CApplication;

class CWeaponEsp : public IFeature
{
public:
	CWeaponEsp();
	~CWeaponEsp();

	void SetDrawWeaponName(bool bDrawWeaponName) { m_bDrawWeaponName = bDrawWeaponName; }
	bool GetDrawWeaponName() { return m_bDrawWeaponName; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	void DrawWeaponName(CWeapon* pCurEntity, int posX, int posY);

	bool m_bDrawWeaponName;
};

#endif //__WEAPONESP_H__