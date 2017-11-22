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
			pApp->Legitbot()->DrawPath(pSurface);
			pApp->Legitbot()->DrawFieldOfView(pSurface);

			// Draw Hitmarker
			pApp->GunHud()->DrawHitmarker();
			pApp->GunHud()->DrawHitmarkerHitpoint();

			// Draw SpreadCone
			pApp->GunHud()->DrawSpreadCone();

			// Draw Crosshair last (but not least)
			pApp->GunHud()->DrawCrosshair();

			// LBY Indicator
			pApp->AntiAim()->DrawLBYIndicator();
			pApp->AntiAim()->DrawEdgeAntiAimPoints();

			// LC Draw
			pApp->LagCompensation()->DrawLagCompensationEntries();
			pApp->LagCompensation()->DrawLagCompensationIndicator();
		}
	}

	if (vguiMatSystemTopPanel == vguiPanel)
	{
		// Draw Menu least ;)
		pApp->Menu()->Draw(pSurface);
	}

	g_pPaintTraverse(ecx, vguiPanel, forceRepaint, allowForce);
}
