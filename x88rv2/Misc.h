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

	void IsNoScope(bool bNoScope) { m_bNoScope = bNoScope; };
	bool IsNoScope() { return m_bNoScope; };

	void IsAutoPistol(bool bAutoPistol) { m_bAutoPistol = bAutoPistol; };
	bool IsAutoPistol() { return m_bAutoPistol; };

	virtual void Setup();
	virtual void Update(void* pParameters);

	void NoRecoil(CUserCmd*);
	void Fakelag(CUserCmd*);
	void AutoStrafe(CUserCmd*);
	void DrawNoScope();
	bool NoScope(unsigned int vguiPanel);
private:
	bool m_bNoRecoil;
	bool m_bFakelag;
	bool m_bAutoStrafe;
	bool m_bNoScope;
	bool m_bAutoPistol;
};


#endif // __MISC_H__