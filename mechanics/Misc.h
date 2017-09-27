#ifndef __MISC_H__
#define __MISC_H__

#include "IFeature.h"
#include "XorString.h"

#define MAXPACKETSCHOKED		16

#define AUTOSTRAFEMODE_NONE		0
#define AUTOSTRAFEMODE_LEGIT	1
#define AUTOSTRAFEMODE_RAGE		2

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

typedef void(*IsReadyCallback_t) ();
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

	void SetFakelagChokedAmount(int iFakelagChokedAmount) { m_iFakelagChokedAmount = iFakelagChokedAmount; };
	int GetFakelagChokedAmount() { return m_iFakelagChokedAmount; };

	void SetFakelagChokeAmount(int iFakelagChokeAmount) { m_iFakelagChokeAmount = iFakelagChokeAmount; };
	int GetFakelagChokeAmount() { return m_iFakelagChokeAmount; };

	void SetAutoStrafeMode(int iAutoStrafeMode) { m_iAutoStrafeMode = iAutoStrafeMode; };
	int GetAutoStrafeMode() { return m_iAutoStrafeMode; };

	void SetAutoPistol(bool bAutoPistol) { m_bAutoPistol = bAutoPistol; };
	bool GetAutoPistol() { return m_bAutoPistol; };

	void SetShowSpectators(bool bSpectators) { m_bSpectators = bSpectators; };
	bool GetShowSpectators() { return m_bSpectators; };

	void SetShowOnlyMySpectators(bool bOnlyMySpectators) { m_bOnlyMySpectators = bOnlyMySpectators; };
	bool GetShowOnlyMySpectators() { return m_bOnlyMySpectators; };

	void SetShowOnlyMyTeamSpectators(bool bOnlyMyTeamSpectators) { m_bOnlyMyTeamSpectators = bOnlyMyTeamSpectators; };
	bool GetShowOnlyMyTeamSpectators() { return m_bOnlyMyTeamSpectators; };

	void SetJumpScout(bool bJumpScout) { m_bJumpScout = bJumpScout; };
	bool GetJumpScout() { return m_bJumpScout; };

	void SetNoName(bool bNoName) { m_bNoName = bNoName; };
	bool GetNoName() { return m_bNoName; };

	bool GetIsCustomClanTag() { return m_bIsCustomClanTag; };

	void SetAutoAccept(bool bAutoAccept) { m_bAutoAccept = bAutoAccept; };
	bool GetAutoAccept() { return m_bAutoAccept; };

	void SetReadyCallback(IsReadyCallback_t pIsReadyCallback) { m_IsReadyCallback = pIsReadyCallback; };
	IsReadyCallback_t GetReadyCallback() { return m_IsReadyCallback; };

	virtual void Setup();
	virtual void Update(void* pParameters);

	void NoRecoil(CUserCmd* pUserCmd);
	void Fakelag(CUserCmd* pUserCmd);
	void AutoStrafe(CUserCmd* pUserCmd);
	void AutoPistol(CUserCmd* pUserCmd);
	void SpectatorList();
	void SetClanTag(const char* tag);
	void SetNoNameClanTag(bool bSetNoName);
	void JumpScout(CUserCmd* pUserCmd);
	void SetName(const char*);
	void SpamNameFix();
	void AutoAccept(const char* filename);
private:
	bool m_bNoRecoil;

	bool m_bFakelag;
	int m_iFakelagChokeAmount;
	int m_iFakelagChokedAmount;

	int m_iAutoStrafeMode;
	bool m_bAutoPistol;
	bool m_bSpectators;
	bool m_bOnlyMySpectators;
	bool m_bOnlyMyTeamSpectators;
	bool m_bJumpScout;
	bool m_bIsCustomClanTag;
	bool m_bAutoAccept;
	bool m_bNoName;

	IsReadyCallback_t m_IsReadyCallback;

	char m_pClanTag[128];

	CXorString m_xorName;

	SetClanTag_t m_pSetClanTag;
};


#endif // __MISC_H__