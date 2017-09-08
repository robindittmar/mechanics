#include "Autowall.h"
#include "Application.h"

namespace Autowall
{
	bool CanHit(Vector &point, float *damage_given)
	{
		IClientEntity* local = (IClientEntity*)CApplication::Instance()->EntityList()->GetClientEntity(CApplication::Instance()->EngineClient()->GetLocalPlayer());

		FireBulletData data(*local->GetOrigin() + *local->GetEyeOffset(), local);

		Vector angles = ACalcAngle(data.src, point);
		AngleVectors(angles, &data.direction);
		VectorNormalize(data.direction);

		if (SimulateFireBullet(local, (CWeapon*)local->GetActiveWeapon(), data))
		{
			*damage_given = data.current_damage;

			return true;
		}
		return false;
	}

	inline bool DidHitWorld(IClientEntity* m_pEnt)
	{
		return m_pEnt->EntIndex() == 0;
	}

	inline bool DidHitNonWorldEntity(IClientEntity* m_pEnt)
	{
		return m_pEnt != NULL && !DidHitWorld(m_pEnt);
	}

	float GetHitgroupDamageMult(int iHitGroup)
	{
		switch (iHitGroup)
		{
		case HITGROUP_HEAD:
			return 2.0f;
		case HITGROUP_STOMACH:
			return 0.75f;
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
		case HITGROUP_GEAR:
			return 0.5f;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			return 0.375f;
		default:
			return 1.0f;

		}

		return 1.0f;
	}

	void ScaleDamage(int hitgroup, IClientEntity *enemy, float weapon_armor_ratio, float &current_damage)
	{
		current_damage *= GetHitgroupDamageMult(hitgroup);

		if (enemy->GetArmor() > 0)
		{
			if (hitgroup == HITGROUP_HEAD)
			{
				if (enemy->HasHelmet())
					current_damage *= (weapon_armor_ratio);
			}
			else
			{
				current_damage *= (weapon_armor_ratio);
			}
		}
	}

	void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *tr)
	{
		typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
		static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)CPattern::FindPattern((BYTE*)CApplication::Instance()->ClientDll(), 0x50E5000, (BYTE*)"\x55\x8B\xEC\x83\xE4\xF0\x83\xEC\x7C\x56\x52", "xxxxxxxxxxx");
		//TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);

		__asm
		{
			mov eax, tr;
			push eax;
			mov ecx, collisionGroup;
			push ecx;
			mov edx, ignore;
			push edx;
			mov eax, mask;
			push eax;
			mov edx, vecAbsEnd;
			mov ecx, vecAbsStart;
			call TraceLine;
			add esp, 0x10;
		}
	}

	void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
	{
		static DWORD dwAddress = (DWORD)CPattern::FindPattern((BYTE*)CApplication::Instance()->ClientDll(), 0x50E5000, (BYTE*)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10", "xxxxxxxxxxxxxxxxxxxxxxxx----xxx");

		if (!dwAddress)
			return;

		_asm
		{
			MOV		EAX, filter
			LEA		ECX, tr
			PUSH	ECX
			PUSH	EAX
			PUSH	mask
			LEA		EDX, vecAbsEnd
			LEA		ECX, vecAbsStart
			CALL	dwAddress
			ADD		ESP, 0xC
		}
	}



	bool SimulateFireBullet(IClientEntity *local, CWeapon *weapon, FireBulletData &data)
	{
		data.penetrate_count = 4;
		data.trace_length = 0.0f;
		CWeaponInfo* wpn_data = weapon->GetWeaponInfo();
		data.current_damage = (float)wpn_data->iDamage;

		while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
		{
			data.trace_length_remaining = wpn_data->flRange - data.trace_length;
			Vector End_Point = data.src + data.direction * data.trace_length_remaining;

			UTIL_TraceLine(data.src, End_Point, 0x4600400B, local, 0, &data.enter_trace);
			UTIL_ClipTraceToPlayers(data.src, End_Point * 40.f, 0x4600400B, &data.filter, &data.enter_trace);

			if (data.enter_trace.fraction == 1.0f)
				break;

			if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0) && (local->GetTeamNum() != data.enter_trace.hit_entity->GetTeamNum()))
			{
				data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
				data.current_damage *= pow(wpn_data->flRangeModifier, data.trace_length * 0.002);
				ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.hit_entity, wpn_data->flArmorRatio, data.current_damage);
				return true;
			}

			if (!HandleBulletPenetration(wpn_data, data))
				break;
		}
		return false;
	}

	bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
	{
		typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
		static TraceToExitFn TraceToExit = (TraceToExitFn)CPattern::FindPattern((BYTE*)CApplication::Instance()->ClientDll(), 0x50E5000, (BYTE*)"\x55\x8B\xEC\x83\xEC\x30\xF3\x0F\x10\x75", "atehunpyqx");

		if (!TraceToExit)
			return false;

		int bRet;
		__asm
		{
			mov eax, trace;
			push eax;
			push vEnd.z;
			push vEnd.y;
			push vEnd.x;
			push start.z;
			push start.y;
			push start.x;
			mov edx, tr;
			mov ecx, end;
			call TraceToExit;
			add esp, 0x1C;

			mov bRet, eax;
		}

		return (bool)bRet;
	}

	bool HandleBulletPenetration(CWeaponInfo *wpn_data, FireBulletData &data)
	{
		surfacedata_t *enter_surface_data = CApplication::Instance()->PhysicsSurfaceProps()->GetSurfaceData(data.enter_trace.surface.surfaceProps);
		int enter_material = enter_surface_data->game.material;
		float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;

		data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
		data.current_damage *= pow(wpn_data->flRangeModifier, (data.trace_length * 0.002));

		if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
			data.penetrate_count = 0;

		if (data.penetrate_count <= 0)
			return false;

		Vector dummy;
		trace_t trace_exit;

		if (!TraceToExit(dummy, data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
			return false;

		surfacedata_t *exit_surface_data = CApplication::Instance()->PhysicsSurfaceProps()->GetSurfaceData(trace_exit.surface.surfaceProps);
		int exit_material = exit_surface_data->game.material;
		float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
		float final_damage_modifier = 0.16f;
		float combined_penetration_modifier = 0.0f;

		if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
		{
			combined_penetration_modifier = 3.0f; final_damage_modifier = 0.05f;
		}
		else
		{
			combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
		}

		if (enter_material == exit_material)
		{
			if (exit_material == 87 || exit_material == 85)
				combined_penetration_modifier = 3.0f;
			else if (exit_material == 76)
				combined_penetration_modifier = 2.0f;
		}

		float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
		float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.f, (3.0f / wpn_data->flPenetration) * 1.25f);
		float thickness = VectorLength(trace_exit.endpos - data.enter_trace.endpos);
		thickness *= thickness;
		thickness *= v34;
		thickness /= 24.0f;

		float lost_damage = fmaxf(0.0f, v35 + thickness);
		if (lost_damage > data.current_damage)
			return false;

		if (lost_damage >= 0.0f)
			data.current_damage -= lost_damage;

		if (data.current_damage < 1.0f)
			return false;

		data.src = trace_exit.endpos;
		data.penetrate_count--;

		return true;
	}

	inline vec_t VectorLength(const Vector& v)
	{
		return (vec_t)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	inline vec_t VectorNormalize(Vector& v)
	{
		vec_t l = v.Length();

		if (l != 0.0f)
		{
			v /= l;
		}
		else
		{
			v.x = v.y = 0.0f; v.z = 1.0f;
		}

		return l;
	}

	QAngle ACalcAngle(Vector& vStartPos, Vector& vEndPos)
	{
		Vector vRelativeDist = vEndPos - vStartPos;
		QAngle qAngle(
			RAD2DEG(-asinf(vRelativeDist.z / vRelativeDist.Length())),
			RAD2DEG(atan2f(vRelativeDist.y, vRelativeDist.x)),
			0.0f
		);

		return qAngle;
	}
}