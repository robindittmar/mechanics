#ifndef __AUTOWALL_H__
#define __AUTOWALL_H__

// Source SDK
#include "../source_sdk/Vector.h"
#include "../source_sdk/ClientEntity.h"
#include "../source_sdk/IEngineTrace.h"
#include "../source_sdk/CGameTrace.h"

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

	// Function
	bool CanHit(Vector &point, float *damage_given, Vector* vHeadPos = NULL, IClientEntity* pEntity = NULL);
	bool SimulateFireBullet(IClientEntity *local, CWeapon *weapon, FireBulletData &data);

	// Engine Functions
	void TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, trace_t* tr, IClientEntity* ignore = NULL);
	bool TraceToExit(Vector& end, trace_t& enter_trace, Vector vStart, Vector vEnd, trace_t* exit_trace);
	bool IsBreakableEntity(IClientEntity* pEntity);
	void ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr);
	bool HandleBulletPenetration(CWeaponInfo* wpn_data, FireBulletData& data);

	// Helper Funcs
	bool IsArmored(IClientEntity* pEntity, int iArmorValue, int iHitgroup);
	void ScaleDamage(int iHitgroup, IClientEntity* pEntity, float weapon_armor_ratio, float &current_damage);

	float DistanceToRay(const Vector& pos, const Vector& rayStart, const Vector& rayEnd, float* along = nullptr, Vector* pointOnRay = nullptr);

	inline bool DidHitNonWorldEntity(IClientEntity* pEntity);
	inline bool DidHitWorld(IClientEntity* pEntity);

	inline vec_t VectorNormalize(Vector& v);
}

#endif // __AUTOWALL_H__