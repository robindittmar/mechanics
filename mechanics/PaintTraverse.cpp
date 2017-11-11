#include "PaintTraverse.h"
#include "Application.h"

PaintTraverse_t g_pPaintTraverse;

void __fastcall hk_PaintTraverse(void* ecx, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	CApplication* pApp = CApplication::Instance();
	ISurface* pSurface = pApp->Surface();

	static unsigned int vguiMatSystemTopPanel;
	if (vguiMatSystemTopPanel == NULL)
	{
		static CXorString matSystemTopPanel("Zjñ‘nxñ§z_ê²Gjë§{");
		const char* szName = pApp->Panel()->GetName(vguiPanel);
		if (stricmp(szName, matSystemTopPanel.ToCharArray()) == 0)
		{
			vguiMatSystemTopPanel = vguiPanel;
		}
	}

	if (pApp->EngineClient()->IsInGame())
	{
		if (pApp->Visuals()->NoScope(vguiPanel))
			return;

		if (vguiMatSystemTopPanel == vguiPanel)
		{
			pApp->Gui()->GetWorldToScreenMatrix();

			// Draw NoScope
			pApp->Visuals()->DrawNoScope();

			// Draw SpecList
			pApp->Misc()->SpectatorList();

			// Draw Esp
			pApp->Esp()->Update((void*)pSurface);

			// Draw Weapon Esp
			pApp->WeaponEsp()->Update();

			// Draw Sound Esp
			pApp->SoundEsp()->Update();

			// Draw rear mirror
			pApp->Mirror()->Render(pSurface, pApp->Menu()->GetMirrorWindow());

			// Draw Legitbot Target
			pApp->Legitbot()->DrawTarget(pSurface);

			// Draw Hitmarker
			pApp->Visuals()->DrawHitmarker(pSurface);
			pApp->Visuals()->DrawHitmarkerHitpoint(pSurface);

			// Draw SpreadCone
			pApp->Visuals()->DrawSpreadCone();

			// Draw Crosshair last (but not least)
			pApp->Visuals()->DrawCrosshair();

			// LBY Indicator
			pApp->AntiAim()->DrawLBYIndicator();
			pApp->AntiAim()->DrawEdgeAntiAimPoints();

			// LC Draw
			pApp->LagCompensation()->DrawLagCompensationEntries();
			pApp->LagCompensation()->DrawLagCompensationIndicator();

			// ****TEST*****
			/*pApp->Surface()->DrawSetTextColor(255, 255, 0, 0);
			pApp->Surface()->DrawSetTextFont(g_pResourceManager->GetFont(RM_FONT_NORMAL));
			IClientEntity* pLocal = pApp->GetLocalPlayer();
			for (int i = 0; i < MAX_PLAYERS; i++)
			{
			IClientEntity* pCurEnt = pApp->EntityList()->GetClientEntity(i);
			CResolverPlayer* pCurRes = pApp->Resolver()->GetResolverPlayer(i);

			if (!pCurEnt)
			continue;

			if (pCurEnt->IsDormant())
			continue;

			if (!pCurEnt->IsAlive())
			continue;

			if (!pCurRes)
			continue;

			Vector screen, origin = *pCurEnt->GetOrigin();
			if (pApp->Gui()->WorldToScreen(origin, screen))
			{
			if (pCurRes->m_bHasFakeActive)
			{
			pApp->Surface()->DrawSetTextPos(screen.x - 3, screen.y + 10);
			pApp->Surface()->DrawPrintText(L"FAKE", lstrlenW(L"FAKE"));
			}

			if (pCurRes->m_bStartPredLbyBreaks)
			{
			pApp->Surface()->DrawSetTextPos(screen.x - 8, screen.y + 25);
			pApp->Surface()->DrawPrintText(L"LBY-Breaker", lstrlenW(L"LBY-Breaker"));
			}
			}
			}*/
		}
	}

	if (vguiMatSystemTopPanel == vguiPanel)
	{
		// Draw Menu least ;)
		pApp->Menu()->Draw(pSurface);
	}

	g_pPaintTraverse(ecx, vguiPanel, forceRepaint, allowForce);
}
