#ifndef __GUNACCURACY_H__
#define __GUNACCURACY_H__

// Std Lib
#include <assert.h>

// Source SDK
#include "Vector.h"
#include "ClientEntity.h"

// Custom
#include "IFeature.h"

class CGunAccuracy : public IFeature
{
public:
	CGunAccuracy();
	virtual ~CGunAccuracy();

	void SetRecoilCompensation(float fRecoilCompensation) { m_fRecoilCompensation = fRecoilCompensation; }
	float GetRecoilCompensation() { return m_fRecoilCompensation; }

	void SetNoRecoil(bool bNoRecoil) { m_bNoRecoil = bNoRecoil; }
	bool GetNoRecoil() { return m_bNoRecoil; }

	void SetNoSpread(bool bNoSpread) { m_bNoSpread = bNoSpread; }
	bool GetNoSpread() { return m_bNoSpread; }

	virtual void Think(void* pParameters = nullptr) override;
private:
	void ApplyNoRecoil(IClientEntity* pLocalEntity);
	void ApplyNoSpread(IClientEntity* pLocalEntity, CWeapon* pActiveWeapon, int iSeed);

	float m_fRecoilCompensation;

	QAngle m_qAngles;
	bool m_bNoRecoil;
	bool m_bNoSpread;
};

#endif // __GUNACCURACY_H__