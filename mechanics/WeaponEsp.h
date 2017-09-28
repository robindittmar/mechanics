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
	
	void SetDrawWeaponBoundingBox(bool bDrawWeaponBoundingBox) { m_bDrawWeaponBoundingBox = bDrawWeaponBoundingBox; }
	bool GetDrawWeaponBoundingBox() { return m_bDrawWeaponBoundingBox; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	void DrawWeaponName(CWeapon* pCurEntity, int posX, int posY);
	bool ShouldDrawBox(IClientEntity* pCurEntity, int &x0, int &y0, int &x1, int &y1);

	bool m_bDrawWeaponName;
	bool m_bDrawWeaponBoundingBox;
};

#endif //__WEAPONESP_H__