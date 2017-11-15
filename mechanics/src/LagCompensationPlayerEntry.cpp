#include "LagCompensationPlayerEntry.h"


CLagCompensationPlayerEntry::CLagCompensationPlayerEntry()
{
	m_bIsEndOfList = true;
}


//float CLagCompensationPlayerEntry::LerpTime()
//{
//	CApplication* pApp = CApplication::Instance();
//	static ConVar* cl_update_rate = pApp->CVar()->FindVar("cl_updaterate");
//	static ConVar* min_ud_rate = pApp->CVar()->FindVar("sv_minupdaterate");
//	static ConVar* max_ud_rate = pApp->CVar()->FindVar("sv_maxupdaterate");
//
//	static ConVar* cl_interp_ratio = pApp->CVar()->FindVar("cl_interp_ratio");
//
//	static ConVar* cl_interp = pApp->CVar()->FindVar("cl_interp");
//	static ConVar* c_min_ratio = pApp->CVar()->FindVar("sv_client_min_interp_ratio");
//	static ConVar* c_max_ratio = pApp->CVar()->FindVar("sv_client_max_interp_ratio");
//
//	int ud_rate = atoi(cl_update_rate->value);
//
//
//	if (min_ud_rate && max_ud_rate)
//		ud_rate = atoi(max_ud_rate->value);
//
//	float ratio = atof(cl_interp_ratio->value);
//
//	if (ratio == 0)
//		ratio = 1.0f;
//
//	float lerp = atof(cl_interp->value);
//
//	if (c_min_ratio && c_max_ratio && atof(c_min_ratio->value) != 1)
//		ratio = clamp(ratio, atof(c_min_ratio->value), atof(c_max_ratio->value));
//
//	return fmax(lerp, (ratio / ud_rate));
//}
//
//int CLagCompensationPlayerEntry::FixedTickcount(IClientEntity* pCur)
//{
//	if (m_fSimulationTime == 0.0f)
//		return TIME_TO_TICKS(pCur->GetSimulationTime() + LerpTime()) + 1;
//
//	int iLerpTicks = TIME_TO_TICKS(LerpTime());
//	int iTargetTickCount = TIME_TO_TICKS(m_fSimulationTime) + iLerpTicks;
//
//	return iTargetTickCount + 1;
//}