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
	virtual ~CWeaponEsp();

	void SetDrawWeaponName(bool bDrawWeaponName) { m_bDrawWeaponName = bDrawWeaponName; }
	bool GetDrawWeaponName() { return m_bDrawWeaponName; }
	
	void SetDrawWeaponBoundingBox(bool bDrawWeaponBoundingBox) { m_bDrawWeaponBoundingBox = bDrawWeaponBoundingBox; }
	bool GetDrawWeaponBoundingBox() { return m_bDrawWeaponBoundingBox; }

	void SetDrawGrenadeName(bool bDrawGrenadeName) { m_bDrawGrenadeName = bDrawGrenadeName; }
	bool GetDrawGrenadeName() { return m_bDrawGrenadeName; }

	void SetDrawGrenadeBoundingBox(bool bDrawGrenadeBoundingBox) { m_bDrawGrenadeBoundingBox = bDrawGrenadeBoundingBox; }
	bool GetDrawGrenadeBoundingBox() { return m_bDrawGrenadeBoundingBox; }

	void SetDrawBombName(bool bDrawBombName) { m_bDrawBombName = bDrawBombName; }
	bool GetDrawBombName() { return m_bDrawBombName; }

	void SetDrawBombBoundingBox(bool bDrawBombBoundingBox) { m_bDrawBombBoundingBox = bDrawBombBoundingBox; }
	bool GetDrawBombBoundingBox() { return m_bDrawBombBoundingBox; }

	void SetDrawBombTimer(bool bDrawBombTimer) { m_bDrawBombTimer = bDrawBombTimer; }
	bool GetDrawBombTimer() { return m_bDrawBombTimer; }

	void SetDrawBombDefuseTimer(bool bDrawBombDefuseTimer) { m_bDrawBombDefuseTimer = bDrawBombDefuseTimer; }
	bool GetDrawBombDefuseTimer() { return m_bDrawBombDefuseTimer; }

	void SetDrawBombDamageIndicator(bool bDrawBombDamageIndicator) { m_bDrawBombDamageIndicator = bDrawBombDamageIndicator; }
	bool GetDrawBombDamageIndicator() { return m_bDrawBombDamageIndicator; }

	virtual void Setup() override;
	virtual void Think(void* pParameters = nullptr) override;
private:
	void GrenadeEsp(IClientEntity* pCurEntity);
	void WeaponEsp(IClientEntity* pCurEntity);
	void BombEsp(IClientEntity* pCurEntity);

	float DamageIndicatorArmor(float flDamage, int ArmorValue);
	void DrawWeaponName(CWeapon* pCurEntity, int posX, int posY);
	int DrawWeaponName(wchar_t* pWeaponName, int posX, int posY);
	bool ShouldDrawBox(IClientEntity* pCurEntity, int &x0, int &y0, int &x1, int &y1);

	bool m_bDrawWeaponName;
	bool m_bDrawWeaponBoundingBox;

	bool m_bDrawGrenadeName;
	bool m_bDrawGrenadeBoundingBox;

	bool m_bDrawBombName;
	bool m_bDrawBombBoundingBox;
	bool m_bDrawBombTimer;
	bool m_bDrawBombDefuseTimer;
	bool m_bDrawBombDamageIndicator;

	wchar_t m_pSmokeGrenade[64];
	wchar_t m_pDecoy[64];
	wchar_t m_pFlashbang[64];
	wchar_t m_pHeGrenade[64];
	wchar_t m_pMolotov[64];

	wchar_t m_pC4Planted[64];
	wchar_t m_pC4Time[64];
	wchar_t m_pC4DefuseCountDown[64];
	wchar_t m_pC4DamageIndicator[64];

	int m_iFont;
};

#endif //__WEAPONESP_H__