#ifndef __MISC_H__
#define __MISC_H__

#include "IFeature.h"

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

	void SetNoRecoil(bool bNoRecoil) { m_bNoRecoil = bNoRecoil; };
	bool GetNoRecoil() { return m_bNoRecoil; };

	void SetFakelag(bool bFakelag) { m_bFakelag = bFakelag; };
	bool GetFakelag() { return m_bFakelag; };

	void SetAutoStrafe(bool bAutoStrafe) { m_bAutoStrafe = bAutoStrafe; };
	bool GetAutoStrafe() { return m_bAutoStrafe; };

	void SetNoScope(bool bNoScope) { m_bNoScope = bNoScope; };
	bool GetNoScope() { return m_bNoScope; };

	void SetAutoPistol(bool bAutoPistol) { m_bAutoPistol = bAutoPistol; };
	bool GetAutoPistol() { return m_bAutoPistol; };

	void SetShowSpectators(bool bSpectators) { m_bSpectators = bSpectators; };
	bool GetShowSpectators() { return m_bSpectators; };

	void SetShowOnlyMySpectators(bool bOnlyMySpectators) { m_bOnlyMySpectators = bOnlyMySpectators; };
	bool GetShowOnlyMySpectators() { return m_bOnlyMySpectators; };

	void SetShowOnlyMyTeamSpectators(bool bOnlyMyTeamSpectators) { m_bOnlyMyTeamSpectators = bOnlyMyTeamSpectators; };
	bool GetShowOnlyMyTeamSpectators() { return m_bOnlyMyTeamSpectators; };

	void SetDisablePostProcessing(bool bDisablePostProcessing) { m_bDisablePostProcessing = bDisablePostProcessing; };
	bool GetDisablePostProcessing() { return m_bDisablePostProcessing; };

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
	void DisablePostProcessing();
private:
	bool m_bNoRecoil;
	bool m_bFakelag;
	bool m_bAutoStrafe;
	bool m_bNoScope;
	bool m_bAutoPistol;
	bool m_bSpectators;
	bool m_bOnlyMySpectators;
	bool m_bOnlyMyTeamSpectators;
	bool m_bDisablePostProcessing;

	unsigned long* m_dwOverridePostProcessingDisable;

	SetClanTag_t m_pSetClanTag;
};


#endif // __MISC_H__