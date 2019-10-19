#ifndef __ICVAR_H__
#define __ICVAR_H__

#include "ConVar.h"
#include "IAppSystem.h"

struct CVarDLLIdentifier_t;

class ICVar : public IAppSystem
{
public:
	virtual void func10() = 0;
	virtual void RegisterConCommand(ConVar* pCommandBase) = 0;
	virtual void UnregisterConCommand(ConVar* pCommandBase) = 0;
	virtual void func13() = 0;
	virtual void func14() = 0;
	virtual void func15() = 0;
	virtual ConVar* FindVar(const char* var_name) = 0;
};

#endif // __ICVAR_H__