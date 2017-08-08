#ifndef __MISC_H__
#define __MISC_H__

#include "Feature.h"

#define NOFLASH_OFFSET 0xA304

#define MAXPACKETSCHOKED 16

class CApplication;
class CUserCmd;

class CMisc : public IFeature
{
public:
	CMisc();
	~CMisc();

	void IsNoRecoil(bool bNoRecoil) { m_bNoRecoil = bNoRecoil; };
	bool IsNoRecoil() { return m_bNoRecoil; };

	void IsFakelag(bool bFakelag) { m_bFakelag = bFakelag; };
	bool IsFakelag() { return m_bFakelag; };

	void IsAutoStrafe(bool bAutoStrafe) { m_bAutoStrafe = bAutoStrafe; };
	bool IsAutoStrafe() { return m_bAutoStrafe; };

	virtual void Setup();
	virtual void Update(void* pParameters);

	void NoRecoil(CUserCmd*);
	void Fakelag(CUserCmd*);
	void AutoStrafe(CUserCmd*);
private:
	bool m_bNoRecoil;
	bool m_bFakelag;
	bool m_bAutoStrafe;
};


#endif // __MISC_H__