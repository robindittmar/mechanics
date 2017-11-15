#ifndef __MIRROR_H__
#define __MIRROR_H__

// Source SDK
#include "IViewRender.h"

// Custom
#include "IFeature.h"
#include "ResourceManager.h"
#include "Window.h"

#define MIRROR_WIDTH	320
#define MIRROR_HEIGHT	180

class CMirror : public IFeature
{
public:
	CMirror();
	~CMirror();

	virtual void Setup() override;
	virtual void Update(void* pParameters = 0) override;

	void OnRenderView(void* pThis, const CViewSetup& view, const CViewSetup& hudViewSetup, int nClearFlags, int whatToDraw);
	void Render(ISurface* pSurface, CWindow* pTargetWindow);
private:
};

#endif // __MIRROR_H__