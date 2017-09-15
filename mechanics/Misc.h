#ifndef __MISC_H__
#define __MISC_H__

#include "IFeature.h"
#include "XorString.h"

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

struct CServerConfirmedReservationCheckCallback
{
	char pad[0x2200];
};

typedef void(IsReadyCallback_t) (void*);
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

	void SetJumpScout(bool bJumpScout) { m_bJumpScout = bJumpScout; };
	bool GetJumpScout() { return m_bJumpScout; };

	void SetNoName(bool bNoName) { m_bNoName = bNoName; };
	bool GetNoName() { return m_bNoName; };

	bool GetIsCustomClanTag() { return m_bIsCustomClanTag; };

	void SetAutoAccept(bool bAutoAccept) { m_bAutoAccept = bAutoAccept; };
	bool GetAutoAccept() { return m_bAutoAccept; };

	void SetReadyCallback(IsReadyCallback_t* pIsReadyCallback) { m_IsReadyCallback = pIsReadyCallback; };
	IsReadyCallback_t* GetReadyCallback() { return m_IsReadyCallback; };

	virtual void Setup();
	virtual void Update(void* pParameters);

	void NoRecoil(CUserCmd* pUserCmd);
	void Fakelag(CUserCmd* pUserCmd);
	void AutoStrafe(CUserCmd* pUserCmd);
	void DrawNoScope();
	bool NoScope(unsigned int vguiPanel);
	void AutoPistol(CUserCmd* pUserCmd);
	void SpectatorList();
	void SetClanTag(const char* tag);
	void SetNoNameClanTag(bool bSetNoName);
	void AutoRevolver(CUserCmd* pUserCmd);
	void DisablePostProcessing();
	void JumpScout(CUserCmd* pUserCmd);
	void SetName(const char*);
	void SpamNameFix();
	void AutoAccept(const char* filename);
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
	bool m_bJumpScout;
	bool m_bNoName;
	bool m_bIsCustomClanTag;
	bool m_bAutoAccept;

	IsReadyCallback_t* m_IsReadyCallback;

	char m_pClanTag[128];

	CXorString m_xorName;

	unsigned long* m_dwOverridePostProcessingDisable;

	SetClanTag_t m_pSetClanTag;
};


#endif // __MISC_H__