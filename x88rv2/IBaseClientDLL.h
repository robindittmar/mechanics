#ifndef __IBASECLIENT_H__
#define __IBASECLIENT_H__

#include "ClientClass.h"
#include "CreateInterface.h"
#include "CViewSetup.h"

class IBaseClientDLL {
public:
	// Connect appsystem components, get global interfaces, don't run any other init code
	virtual int              Connect(CreateInterfaceFn appSystemFactory, void *pGlobals) = 0;
	virtual int              Disconnect(void) = 0;
	virtual int              Init(CreateInterfaceFn appSystemFactory, void *pGlobals) = 0;
	virtual void             PostInit() = 0;
	virtual void             Shutdown(void) = 0;
	virtual void             LevelInitPreEntity(char const* pMapName) = 0;
	virtual void             LevelInitPostEntity() = 0;
	virtual void             LevelShutdown(void) = 0;
	virtual ClientClass*     GetAllClasses(void) = 0;

	void RenderView(const CViewSetup &view, int nClearFlags, int whatToDraw);
	bool GetPlayerView(CViewSetup &playerView);
};

#endif // __IBASECLIENT_H__