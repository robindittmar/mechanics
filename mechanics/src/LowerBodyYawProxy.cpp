#include "LowerBodyYawProxy.h"
#include "Application.h"

RecvVarProxy_t g_pLowerBodyYawProxy;

void __cdecl hk_SetLowerBodyYawTarget(const CRecvProxyData* pDataConst, void* pStruct, void* pOut)
{
	CApplication* pApp = CApplication::Instance();

	CRecvProxyData* pData = (CRecvProxyData*)pDataConst;
	IClientEntity* pEntity = (IClientEntity*)pStruct;
	IClientEntity* pLocal = pApp->GetLocalPlayer();
	if (pData && pEntity && pLocal)
	{
		CResolverPlayer* pResolverPlayer = pApp->Resolver()->GetResolverPlayer(pEntity->EntIndex());

		if (fabsf(pResolverPlayer->GetLbyProxyLastValue() - pData->m_Value.m_Float) >= 30.0f)
		{
			pResolverPlayer->SetLbyProxyLastValue(pData->m_Value.m_Float);
			pResolverPlayer->SetLbyUpdateTime(pApp->GlobalVars()->curtime);
			pResolverPlayer->SetLbyProxyUpdatedTime(pApp->GlobalVars()->curtime);
		}
	}

	g_pLowerBodyYawProxy(pDataConst, pStruct, pOut);
}
