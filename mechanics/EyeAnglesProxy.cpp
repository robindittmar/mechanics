#include "EyeAnglesProxy.h"
#include "Application.h"

RecvVarProxy_t g_pEyeAnglesProxy0;
RecvVarProxy_t g_pEyeAnglesProxy1;

void __cdecl hk_SetEyeAnglesProxy0(const CRecvProxyData* pDataConst, void* pStruct, void* pOut)
{
	CApplication* pApp = CApplication::Instance();

	CRecvProxyData* pData = (CRecvProxyData*)pDataConst;
	IClientEntity* pEntity = (IClientEntity*)pStruct;
	IClientEntity* pLocal = pApp->GetLocalPlayer();
	if (pData && pEntity && pLocal)
	{
		/*CResolverPlayer* pResolverPlayer = pApp->Resolver()->GetResolverPlayer(pEntity->EntIndex());

		if (fabsf(pResolverPlayer->GetLbyProxyLastValue() - pData->m_Value.m_Float) >= 30.0f)
		{
			pResolverPlayer->SetLbyProxyLastValue(pData->m_Value.m_Float);
			pResolverPlayer->SetLbyUpdateTime(pApp->GlobalVars()->curtime);
			pResolverPlayer->SetLbyProxyUpdatedTime(pApp->GlobalVars()->curtime);
		}*/
		// TODO
		g_pConsole->Write(LOGLEVEL_WARNING, "%d: %f\n", pEntity->EntIndex(), pData->m_Value.m_Float);
	}

	g_pEyeAnglesProxy0(pDataConst, pStruct, pOut);
}

void __cdecl hk_SetEyeAnglesProxy1(const CRecvProxyData* pDataConst, void* pStruct, void* pOut)
{
	CApplication* pApp = CApplication::Instance();

	CRecvProxyData* pData = (CRecvProxyData*)pDataConst;
	IClientEntity* pEntity = (IClientEntity*)pStruct;
	IClientEntity* pLocal = pApp->GetLocalPlayer();
	if (pData && pEntity && pLocal)
	{
		/*CResolverPlayer* pResolverPlayer = pApp->Resolver()->GetResolverPlayer(pEntity->EntIndex());

		if (fabsf(pResolverPlayer->GetLbyProxyLastValue() - pData->m_Value.m_Float) >= 30.0f)
		{
		pResolverPlayer->SetLbyProxyLastValue(pData->m_Value.m_Float);
		pResolverPlayer->SetLbyUpdateTime(pApp->GlobalVars()->curtime);
		pResolverPlayer->SetLbyProxyUpdatedTime(pApp->GlobalVars()->curtime);
		}*/
		// TODO
		g_pConsole->Write(LOGLEVEL_WARNING, "%d: %f\n", pEntity->EntIndex(), pData->m_Value.m_Float);
	}

	g_pEyeAnglesProxy1(pDataConst, pStruct, pOut);
}
