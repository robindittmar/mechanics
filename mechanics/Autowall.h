#ifndef __AUTOWALL_H__
#define __AUTOWALL_H__

// Source SDK
#include "Vector.h"
#include "ClientEntity.h"
#include "IEngineTrace.h"
#include "CGameTrace.h"

namespace Autowall
{
	struct FireBulletData
	{
		FireBulletData(const Vector &eye_pos, IClientEntity* pLocal = NULL)
			: src(eye_pos),
			filter((IHandleEntity*)pLocal)
		{
		}

		Vector           src;
		trace_t          enter_trace;
		Vector           direction;
		CTraceFilterSkipEntity    filter;
		float           trace_length;
		float           trace_length_remaining;
		float           current_damage;
		int             penetrate_count;
	};

	bool CanHit(Vector &point, float *damage_given, Vector* vHeadPos = NULL);
	inline bool DidHitWorld(IClientEntity* m_pEnt);
	inline bool DidHitNonWorldEntity(IClientEntity* m_pEnt);
	void ScaleDamage(int hitgroup, IClientEntity *enemy, float weapon_armor_ratio, float &current_damage);
	void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *tr);
	void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr);
	bool SimulateFireBullet(IClientEntity *local, CWeapon *weapon, FireBulletData &data);
	bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace);
	bool HandleBulletPenetration(CWeaponInfo *wpn_data, FireBulletData &data);
	inline vec_t VectorLength(const Vector& v);
	inline vec_t VectorNormalize(Vector& v);
	QAngle ACalcAngle(Vector& vStartPos, Vector& vEndPos);
}

#endif // __AUTOWALL_H__