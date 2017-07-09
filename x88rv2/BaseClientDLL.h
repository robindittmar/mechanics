/// 
/// Source Engine declarations
///


#ifndef __BASECLIENT_H__
#define __BASECLIENT_H__

#include "ClientClass.h"
#include "CreateInterface.h"

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
};

#endif // __BASECLIENT_H__