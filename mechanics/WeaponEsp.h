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

	void SetDrawGrenadeName(bool bDrawGrenadeName) { m_bDrawGrenadeName = bDrawGrenadeName; }
	bool GetDrawGrenadeName() { return m_bDrawGrenadeName; }

	void SetDrawGrenadeBoundingBox(bool bDrawGrenadeBoundingBox) { m_bDrawGrenadeBoundingBox = bDrawGrenadeBoundingBox; }
	bool GetDrawGrenadeBoundingBox() { return m_bDrawGrenadeBoundingBox; }

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	void DrawWeaponName(CWeapon* pCurEntity, int posX, int posY);
	bool ShouldDrawBox(IClientEntity* pCurEntity, int &x0, int &y0, int &x1, int &y1);

	void GrenadeEsp(IClientEntity* pCurEntity);
	void WeaponEsp(IClientEntity* pCurEntity);

	bool m_bDrawWeaponName;
	bool m_bDrawWeaponBoundingBox;

	bool m_bDrawGrenadeName;
	bool m_bDrawGrenadeBoundingBox;

	int m_iFont;
};

#endif //__WEAPONESP_H__