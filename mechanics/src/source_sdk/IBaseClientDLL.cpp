#include "IBaseClientDLL.h"

void IBaseClientDLL::RenderView(const CViewSetup &view, int nClearFlags, int whatToDraw)
{
	typedef void(__thiscall* RenderView_t)(void*, const CViewSetup&, int, int);
	((RenderView_t)(*(void***)this)[27])(this, view, nClearFlags, whatToDraw);
}

bool IBaseClientDLL::GetPlayerView(CViewSetup &playerView)
{
	typedef bool(__thiscall* GetPlayerView_t)(void*, CViewSetup&);
	return ((GetPlayerView_t)(*(void***)this)[69])(this, playerView);
}
