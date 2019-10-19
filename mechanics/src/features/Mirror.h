#ifndef __MIRROR_H__
#define __MIRROR_H__

// Source SDK
#include "../source_sdk/IViewRender.h"

// Custom
#include "IFeature.h"
#include "../core/ResourceManager.h"
#include "../gui/Window.h"

#define MIRROR_WIDTH	320
#define MIRROR_HEIGHT	180

class CMirror : public IFeature
{
public:
	CMirror();
	virtual ~CMirror();

	virtual void Setup() override;
	virtual void Think(void* pParameters = nullptr) override;

	void OnRenderView(void* pThis, const CViewSetup& view, const CViewSetup& hudViewSetup, int nClearFlags, int whatToDraw);
	void Render(ISurface* pSurface, CWindow* pTargetWindow);
private:
};

#endif // __MIRROR_H__