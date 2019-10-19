#include "EyeAnglesProxy.h"
#include "../Application.h"

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
		CResolverPlayer* pResolverPlayer = pApp->Resolver()->GetResolverPlayer(pEntity->EntIndex());
		if (pResolverPlayer)
		{
			pResolverPlayer->SetYawAngle(pData->m_Value.m_Float);
		}
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
		CResolverPlayer* pResolverPlayer = pApp->Resolver()->GetResolverPlayer(pEntity->EntIndex());
		if (pResolverPlayer)
		{
			pResolverPlayer->SetYawAngle(pData->m_Value.m_Float);
		}
	}

	g_pEyeAnglesProxy1(pDataConst, pStruct, pOut);
}
