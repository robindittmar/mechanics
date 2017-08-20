#ifndef __MISC_H__
#define __MISC_H__

#include "Feature.h"

#define OFFSET_NOFLASH 0xA2E4
#define OFFSET_OBSERVER 0x3360

#define MAXPACKETSCHOKED 16

class CApplication;
class CUserCmd;

struct Observers
{
	int Observer, Observing;

	Observers() {};
	Observers(int ObserverEntNum, int ObservingEntNum)
	{
		this->Observer = ObserverEntNum;
		this->Observing = ObservingEntNum;
	}
};

typedef void(__fastcall* SetClanTag_t)(const char*, const char*);

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

	void ShowSpectators(bool bSpectators) { m_bSpectators = bSpectators; };
	bool ShowSpectators() { return m_bSpectators; };

	void ShowOnlyMySpectators(bool bOnlyMySpectators) { m_bOnlyMySpectators = bOnlyMySpectators; };
	bool ShowOnlyMySpectators() { return m_bOnlyMySpectators; };

	void ShowOnlyMyTeamSpectators(bool bOnlyMyTeamSpectators) { m_bOnlyMyTeamSpectators = bOnlyMyTeamSpectators; };
	bool ShowOnlyMyTeamSpectators() { return m_bOnlyMyTeamSpectators; };

	virtual void Setup();
	virtual void Update(void* pParameters);

	void NoRecoil(CUserCmd*);
	void Fakelag(CUserCmd*);
	void AutoStrafe(CUserCmd*);
	void DrawNoScope();
	bool NoScope(unsigned int vguiPanel);
	void AutoPistol(CUserCmd*);
	void SpectatorList();
	void SetClanTag(const char*);
	void AutoRevolver(CUserCmd*);
private:
	bool m_bNoRecoil;
	bool m_bFakelag;
	bool m_bAutoStrafe;
	bool m_bNoScope;
	bool m_bAutoPistol;
	bool m_bSpectators;
	bool m_bOnlyMySpectators;
	bool m_bOnlyMyTeamSpectators;

	SetClanTag_t m_pSetClanTag;
};


#endif // __MISC_H__