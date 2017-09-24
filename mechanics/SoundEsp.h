#ifndef __SOUNDESP_H__
#define __SOUNDESP_H__

// Std lib
#include <assert.h>
#include <vector>

// Source SDK
#include "Vector.h"
#include "ISurface.h"
#include "ClientEntity.h"

// Custom
#include "IFeature.h"
#include "Gui.h"
#include "SoundInfo.h"

#define SOUND_SHOWTIME			1000
#define SOUND_FADETIME			1000

class CSoundEsp : public IFeature
{
public:
	CSoundEsp();
	~CSoundEsp();

	void SetShowTime(float fShowTime) { m_iShowTime = fShowTime * 1000; }
	float GetShowTime() { return m_iShowTime / 1000.0f; }

	void SetFadeTime(float fFadeTime) { m_iFadeoutTime = fFadeTime * 1000; }
	float GetFadeTime() { return m_iFadeoutTime / 1000.0f; }

	void SetFadeoutEnabled(bool bFadeoutEnabled) { m_bFadeoutEnabled = bFadeoutEnabled; }
	bool GetFadeoutEnabled() { return m_bFadeoutEnabled; }

	void SetDrawOwnTeam(bool bDrawOwnTeam) { m_bDrawOwnTeam = bDrawOwnTeam; }
	bool GetDrawOwnTeam() { return m_bDrawOwnTeam; }

	void SetDrawVisible(bool bDrawVisible) { m_bDrawVisible = !bDrawVisible; } // TODO: Ghettofix
	bool GetDrawVisible() { return !m_bDrawVisible; }

	// Only add CSoundInfo's created with 'new'
	// cleanup ('delete') will be done by this class
	void AddSound(CSoundInfo* pSound);
	void UpdateSounds();

	virtual void Setup();
	virtual void Update(void* pParameters = 0);
private:
	int m_iShowTime;
	int m_iFadeoutTime;

	bool m_bFadeoutEnabled;	
	bool m_bDrawOwnTeam;
	bool m_bDrawVisible;

	unsigned int m_iFont;

	std::vector<CSoundInfo*> m_vecSounds;

	CGui* m_pGui;
};

#endif // __SOUNDESP_H__