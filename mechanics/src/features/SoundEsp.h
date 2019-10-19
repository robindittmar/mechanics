#ifndef __SOUNDESP_H__
#define __SOUNDESP_H__

// Std lib
#include <assert.h>
#include <vector>

// Source SDK
#include "../source_sdk/Vector.h"
#include "../source_sdk/ISurface.h"
#include "../source_sdk/ClientEntity.h"

// Custom
#include "IFeature.h"
#include "../gui/Gui.h"
#include "../core/SoundInfo.h"

#define SOUND_SHOWTIME			1.0f
#define SOUND_FADEOUTTIME		1.0f

class CSoundEsp : public IFeature
{
public:
	CSoundEsp();
	virtual ~CSoundEsp();

	void SetShowTime(float fShowTime) { m_fShowTime = fShowTime; }
	float GetShowTime() { return m_fShowTime; }

	void SetFadeoutTime(float fFadeoutTime) { m_fFadeoutTime = fFadeoutTime; }
	float GetFadeoutTime() { return m_fFadeoutTime; }

	void SetFadeoutEnabled(bool bFadeoutEnabled) { m_bFadeoutEnabled = bFadeoutEnabled; }
	bool GetFadeoutEnabled() { return m_bFadeoutEnabled; }

	void SetDrawOwnTeam(bool bDrawOwnTeam) { m_bDrawOwnTeam = bDrawOwnTeam; }
	bool GetDrawOwnTeam() { return m_bDrawOwnTeam; }

	void SetDrawOnlyNotVisible(bool bDrawOnlyNotVisible) { m_bDrawOnlyNotVisible = bDrawOnlyNotVisible; }
	bool GetDrawOnlyNotVisible() { return m_bDrawOnlyNotVisible; }

	// Only add CSoundInfo's created with 'new'
	// cleanup ('delete') will be done by this class
	void AddSound(CSoundInfo* pSound);

	virtual void Setup() override;
	virtual void Think(void* pParameters = nullptr) override;

	void Draw(ISurface* pSurface);
private:
	float m_fShowTime;
	float m_fFadeoutTime;

	bool m_bFadeoutEnabled;	
	bool m_bDrawOwnTeam;
	bool m_bDrawOnlyNotVisible;

	unsigned int m_iFont;

	std::vector<CSoundInfo*> m_vecSounds;

	CGui* m_pGui;
};

#endif // __SOUNDESP_H__