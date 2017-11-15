#include "Mirror.h"
#include "Application.h"

CMirror::CMirror()
{
}

CMirror::~CMirror()
{
}

void CMirror::Setup()
{
	IFeature::Setup();

	g_pResourceManager->CreateMirror();
}

void CMirror::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;
}

void CMirror::OnRenderView(void* pThis, const CViewSetup& view, const CViewSetup& hudViewSetup, int nClearFlags, int whatToDraw)
{
	if (!m_bIsEnabled)
		return;

	if (m_pApp->EngineClient()->IsInGame())
	{
		CViewSetup myView = view;
		myView.x = 0;
		myView.y = 0;
		myView.width = MIRROR_WIDTH;
		myView.height = MIRROR_HEIGHT;

		myView.angles.x = -myView.angles.x;
		myView.angles.y += 180.0f;
		myView.angles.NormalizeAngles();

		ITexture* pMirrorTexture = g_pResourceManager->GetMirror();
		IMatRenderContext* pRenderContext = m_pApp->MaterialSystem()->GetRenderContext();
		ITexture* pOldTarget = pRenderContext->GetRenderTarget();
		pRenderContext->SetRenderTarget(pMirrorTexture);

		g_pRenderView(
			pThis,
			myView,
			hudViewSetup,
			nClearFlags,
			RENDERVIEW_UNSPECIFIED
		);

		pRenderContext->SetRenderTarget(pOldTarget);
	}
}

void CMirror::Render(ISurface* pSurface, CWindow* pTargetWindow)
{
	if (!m_bIsEnabled)
		return;

	IMatRenderContext* pRenderContext = m_pApp->MaterialSystem()->GetRenderContext();
	if (!pRenderContext)
		return;

	int x, y;
	pTargetWindow->GetClientPosition(&x, &y);
	pTargetWindow->Draw(pSurface);

	pRenderContext->DrawScreenSpaceRectangle(
		g_pResourceManager->GetMirrorMaterial(),
		x,
		y,
		MIRROR_WIDTH,
		MIRROR_HEIGHT,
		0.0f,
		0.0f,
		MIRROR_WIDTH,
		MIRROR_HEIGHT,
		MIRROR_WIDTH,
		MIRROR_HEIGHT
	);
}