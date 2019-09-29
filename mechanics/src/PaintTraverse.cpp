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
		static CXorString matSystemTopPanel(/*MatSystemTopPanel*/"\x5A\x6A\xF1\x91\x6E\x78\xF1\xA7\x7A\x5F\xEA\xB2\x47\x6A\xEB\xA7\x7B");
		const char* szName = pApp->Panel()->GetName(vguiPanel);
		// stricmp
		if (strcmp(szName, matSystemTopPanel.ToCharArray()) == 0)
		{
			vguiMatSystemTopPanel = vguiPanel;
		}
	}

	if (pApp->EngineClient()->IsInGame())
	{
		if (pApp->Visuals()->NoScope(vguiPanel))
			return;

		if (!pApp->Gui()->GetEnableGameInput())
		{
			pApp->InputSystem()->ResetInputState();
		}
	}

	if (vguiMatSystemTopPanel == vguiPanel)
	{
		pApp->Gui()->GetScreenSize();

		if (pApp->EngineClient()->IsInGame())
		{
			pApp->Gui()->GetWorldToScreenMatrix();

			// Draw NoScope
			pApp->Visuals()->DrawNoScope();

			// Draw SpecList
			pApp->Misc()->SpectatorList();

			// Draw Esp
			pApp->Esp()->Think((void*)pSurface);

			// Draw Weapon Esp
			pApp->WeaponEsp()->Think();

			// Draw Sound Esp
			pApp->SoundEsp()->Draw(pSurface);

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

		// Draw Menu least ;)
		pApp->Menu()->Draw(pSurface);
	}

	g_pPaintTraverse(ecx, vguiPanel, forceRepaint, allowForce);
}
