#include "SoundEsp.h"
#include "Application.h"

CSoundEsp::CSoundEsp()
{
	m_iShowTime = SOUND_SHOWTIME;
	m_iFadeoutTime = SOUND_FADETIME;
	m_bDrawOwnTeam = false;
	m_bDrawVisible = false;
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

	// TODO: xor
	if (!strstr(pSound->GetSample(), "footstep"))
		return;

	IClientEntity* pLocalEntity = m_pApp->EntityList()->GetClientEntity(m_pApp->EngineClient()->GetLocalPlayer());
	IClientEntity* pCurEntity = m_pApp->EntityList()->GetClientEntity(pSound->GetEntityIndex());

	// Local player
	if (pLocalEntity == pCurEntity)
		return;

	if (pLocalEntity->GetTeamNum() == pCurEntity->GetTeamNum() && !m_bDrawOwnTeam)
		return;

	if (!m_bDrawVisible)
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

	// TODO: xor
	pSound->SetSample("Footstep");
	m_vecSounds.push_back(pSound);
}

void CSoundEsp::UpdateSounds()
{
	if (!m_bIsEnabled)
		return;

	ULONGLONG timestamp = GetTickCount64();
	CSoundInfo* pCurrent;
	for (std::vector<CSoundInfo*>::iterator it = m_vecSounds.begin(); it != m_vecSounds.end();)
	{
		pCurrent = *it;

		if (pCurrent->GetTimeSinceCreation(timestamp) > m_iShowTime + m_iFadeoutTime)
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

void CSoundEsp::Setup()
{
	m_pApp = CApplication::Instance();
	m_pGui = CGui::Instance();

	m_iFont = g_pResourceManager->GetFont(RM_FONT_NORMAL);
}

void CSoundEsp::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	this->UpdateSounds();

	ULONGLONG llCurTimestamp = GetTickCount64();
	ULONGLONG llTimeAlive;

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

			llTimeAlive = pCurrent->GetTimeSinceCreation(llCurTimestamp);
			if (llTimeAlive > m_iShowTime)
				m_pApp->Surface()->DrawSetTextColor(((m_iFadeoutTime - (llTimeAlive - m_iShowTime)) / (float)m_iFadeoutTime) * 255, 255, 255, 255);
			else
				m_pApp->Surface()->DrawSetTextColor(255, 255, 255, 255);

			m_pApp->Surface()->DrawSetTextPos(vScreen.x - w / 2, vScreen.y - h / 2);
			m_pApp->Surface()->DrawPrintText(penis, iLen);
		}
	}
}
