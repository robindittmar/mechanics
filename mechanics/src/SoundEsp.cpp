#include "SoundEsp.h"
#include "Application.h"

CSoundEsp::CSoundEsp()
	: m_fShowTime(SOUND_SHOWTIME), m_fFadeoutTime(SOUND_FADEOUTTIME), m_bFadeoutEnabled(false),
	m_bDrawOwnTeam(false), m_bDrawOnlyNotVisible(false)
{
}

CSoundEsp::~CSoundEsp()
{
	CSoundInfo* pCurrent;
	for (std::vector<CSoundInfo*>::iterator it = m_vecSounds.begin(); it != m_vecSounds.end(); it++)
	{
		pCurrent = *it;

		if (pCurrent)
			delete pCurrent;
	}
}

void CSoundEsp::AddSound(CSoundInfo* pSound)
{
	if (!m_bIsEnabled)
		return;

	static CXorString footstep(/*footstep*/"\x71\x64\xEA\xB6\x64\x7F\xE0\xB2");
	if (!strstr(pSound->GetSample(), footstep.ToCharArray()))
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	IClientEntity* pCurEntity = m_pApp->EntityList()->GetClientEntity(pSound->GetEntityIndex());

	// Local player
	if (pLocalEntity == pCurEntity)
		return;

	if (pLocalEntity->GetTeamNum() == pCurEntity->GetTeamNum() && !m_bDrawOwnTeam)
		return;

	if (m_bDrawOnlyNotVisible)
	{
		Vector vMyHeadPos = *pLocalEntity->GetOrigin() + *pLocalEntity->GetEyeOffset();

		Ray_t ray;
		trace_t trace;
		CTraceFilterSkipEntity traceFilter(pLocalEntity);

		ray.Init(vMyHeadPos, pSound->GetOrigin());
		m_pApp->EngineTrace()->TraceRay(ray, MASK_VISIBLE, &traceFilter, &trace);
		if (trace.IsEntityVisible(pCurEntity))
			return;
	}

	static CXorString step(/*step*/"\x64\x7F\xE0\xB2");
	pSound->SetSample(step);
	m_vecSounds.push_back(pSound);
}

void CSoundEsp::Setup()
{
	IFeature::Setup();

	m_pGui = CGui::Instance();
	m_iFont = g_pResourceManager->GetFont(RM_FONT_NORMAL);
}

void CSoundEsp::Think(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	float fCurtime = m_pApp->GlobalVars()->curtime;
	CSoundInfo* pCurrent;
	for (std::vector<CSoundInfo*>::iterator it = m_vecSounds.begin(); it != m_vecSounds.end();)
	{
		pCurrent = *it;

		float fFullShowTime = m_fShowTime + (m_bFadeoutEnabled ? m_fFadeoutTime : 0.0f);
		if (pCurrent->GetTimeSinceCreation(fCurtime) > fFullShowTime)
		{
			delete pCurrent;
			it = m_vecSounds.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void CSoundEsp::Draw(ISurface* pSurface)
{
	if (!m_bIsEnabled)
		return;

	float fCurtime = m_pApp->GlobalVars()->curtime;
	float fTimeAlive;

	// TODO: Optimize render routine
	wchar_t penis[256];
	Vector vScreen;
	CSoundInfo* pCurrent;

	m_pApp->Surface()->DrawSetTextFont(m_iFont);
	for (std::vector<CSoundInfo*>::iterator it = m_vecSounds.begin(); it != m_vecSounds.end(); it++)
	{
		pCurrent = *it;

		if (m_pGui->WorldToScreen(pCurrent->GetOrigin(), vScreen))
		{
			mbstowcs(penis, pCurrent->GetSample(), 256);
			int iLen = wcslen(penis);

			int w, h;
			m_pApp->Surface()->GetTextSize(m_iFont, penis, w, h);

			fTimeAlive = pCurrent->GetTimeSinceCreation(fCurtime);
			if (fTimeAlive > m_fShowTime)
			{
				if (m_bFadeoutEnabled)
					m_pApp->Surface()->DrawSetTextColor(((m_fFadeoutTime - (fTimeAlive - m_fShowTime)) / m_fFadeoutTime) * 255, 255, 255, 255);
				else
					m_pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);
			}

			m_pApp->Surface()->DrawSetTextPos(vScreen.x - w / 2, vScreen.y - h / 2);
			m_pApp->Surface()->DrawPrintText(penis, iLen);
		}
	}
}
