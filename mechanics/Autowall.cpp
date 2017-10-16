#include "Autowall.h"
#include "Application.h"
#include "Utils.h"

namespace Autowall
{
	bool CanHit(Vector &point, float *damage_given, Vector* vHeadPos)
	{
		IClientEntity* local = (IClientEntity*)CApplication::Instance()->EntityList()->GetClientEntity(CApplication::Instance()->EngineClient()->GetLocalPlayer());

		Vector headPos = *local->GetOrigin() + *local->GetEyeOffset();
		if (vHeadPos)
		{
			headPos = *vHeadPos;
		}

		FireBulletData data(headPos, local);

		Vector angles = Utils::CalcAngle(data.src, point);
		AngleVectors(angles, &data.direction);
		VectorNormalize(data.direction);

		if (SimulateFireBullet(local, (CWeapon*)local->GetActiveWeapon(), data))
		{
			*damage_given = data.current_damage;

			return true;
		}
		return false;
	}

	bool IsBreakableEntity(IClientEntity* pEntity)
	{
		typedef bool(__thiscall *isBreakbaleEntityFn)(IClientEntity*);
		static isBreakbaleEntityFn IsBreakableEntity;

		if (!IsBreakableEntity)
			IsBreakableEntity = (isBreakbaleEntityFn)CPattern::FindPattern((BYTE*)CApplication::Instance()->ClientDll(), 0x50E5000, (BYTE*)"\x55\x8B\xEC\x51\x56\x8B\xF1\x85\xF6\x74\x68", "krngpthxzek");

		return IsBreakableEntity(pEntity);
	}

	inline bool DidHitWorld(IClientEntity* m_pEnt)
	{
		return m_pEnt->EntIndex() == 0;
	}

	inline bool DidHitNonWorldEntity(IClientEntity* m_pEnt)
	{
		return m_pEnt != NULL && !DidHitWorld(m_pEnt);
	}

	bool IsArmored(IClientEntity* Entity, int ArmorValue, int Hitgroup)
	{
		bool result = false;

		if (ArmorValue > 0)
		{
			switch (Hitgroup)
			{
			case HITGROUP_GENERIC:
			case HITGROUP_CHEST:
			case HITGROUP_STOMACH:
			case HITGROUP_LEFTARM:
			case HITGROUP_RIGHTARM:
				result = true;
				break;
			case HITGROUP_HEAD:
				result = Entity->HasHelmet();
				break;
			}
		}

		return result;
	}

	void ScaleDamage(int hitgroup, IClientEntity* enemy, float weapon_armor_ratio, float &current_damage)
	{
		bool bHeavArmor = false; // DT_CSPlayer -> m_bHasHeavyArmor todo
		auto iArmorValue = enemy->GetArmor();

		switch (hitgroup)
		{
		case HITGROUP_HEAD:
			if (bHeavArmor)
				current_damage = (current_damage * 4.f) * 0.5f;
			else
				current_damage *= 4.f;
			break;
		case HITGROUP_STOMACH:
			current_damage *= 1.25f;
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			current_damage *= 0.75f;
			break;
		}

		if (IsArmored(enemy, iArmorValue, hitgroup))
		{
			float v47 = 1.f, ArmorBonusRatio = 0.5f, ArmorRatio = weapon_armor_ratio * 0.5f;

			if (bHeavArmor)
			{
				ArmorBonusRatio = 0.33f;
				ArmorRatio = (weapon_armor_ratio * 0.5f) * 0.5f;
				v47 = 0.33f;
			}

			float NewDamage = current_damage * ArmorRatio;

			if (bHeavArmor)
				NewDamage *= 0.85f;

			if (((current_damage - (current_damage * ArmorRatio)) * (v47 * ArmorBonusRatio)) > iArmorValue)
				NewDamage = current_damage - (iArmorValue / ArmorBonusRatio);

			current_damage = NewDamage;
		}
	}

	void TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity* ignore, trace_t* tr)
	{
		Ray_t ray;
		CTraceFilterSkipEntity filter((IClientEntity*)ignore);

		ray.Init(vecAbsStart, vecAbsEnd);
		CApplication::Instance()->EngineTrace()->TraceRay(ray, mask, &filter, tr);
	}

	float DistanceToRay(const Vector& pos, const Vector& rayStart, const Vector& rayEnd, float* along = nullptr, Vector* pointOnRay = nullptr)
	{
		Vector to = pos - rayStart;
		Vector dir = rayEnd - rayStart;
		float length = dir.NormalizeInPlace();

		float rangeAlong = (dir.x * to.x + dir.y * to.y + dir.z * to.z);//g_Math.DotProduct(dir, to);
		if (along)
		{
			*along = rangeAlong;
		}

		float range;

		if (rangeAlong < 0.0f)
		{
			// off start point
			range = -(pos - rayStart).Length();

			if (pointOnRay)
			{
				*pointOnRay = rayStart;
			}
		}
		else if (rangeAlong > length)
		{
			// off end point
			range = -(pos - rayEnd).Length();

			if (pointOnRay)
			{
				*pointOnRay = rayEnd;
			}
		}
		else // within ray bounds
		{
			Vector onRay = rayStart + (dir.operator*(rangeAlong));
			range = (pos - onRay).Length();

			if (pointOnRay)
			{
				*pointOnRay = onRay;
			}
		}

		return range;
	}
	void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
	{
		trace_t playerTrace;
		Ray_t ray;
		float smallestFraction = tr->fraction;
		const float maxRange = 60.0f;

		ray.Init(vecAbsStart, vecAbsEnd);

		CApplication* pApp = CApplication::Instance();

		for (int k = 1; k <= pApp->GlobalVars()->maxClients; ++k)
		{
			IClientEntity* player = pApp->EntityList()->GetClientEntity(k);

			if (!player || !player->IsAlive())
				continue;

			if (player->IsDormant())
				continue;

			if (filter && filter->ShouldHitEntity(player, mask) == false)
				continue;

			Vector Max = ((CWeapon*)player)->GetVecMax() + *player->GetOrigin();
			Vector Min = ((CWeapon*)player)->GetVecMin() + *player->GetOrigin();
			Vector Size = Max - Min;
			Size /= 2;
			Size += Min;

			float range = DistanceToRay(Size, vecAbsStart, vecAbsEnd);
			if (range < 0.0f || range > maxRange)
				continue;

			pApp->EngineTrace()->ClipRayToEntity(ray, mask | CONTENTS_HITBOX, player, &playerTrace);
			if (playerTrace.fraction < smallestFraction)
			{
				// we shortened the ray - save off the trace
				tr = &playerTrace;
				smallestFraction = playerTrace.fraction;
			}
		}
	}

	bool SimulateFireBullet(IClientEntity *local, CWeapon *weapon, FireBulletData &data)
	{
		data.penetrate_count = 8;
		data.trace_length = 0.0f;
		CWeaponInfo* wpn_data = weapon->GetWeaponInfo();
		data.current_damage = (float)wpn_data->iDamage;

		while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
		{
			data.trace_length_remaining = wpn_data->flRange - data.trace_length;
			Vector End_Point = data.src + data.direction * data.trace_length_remaining;

			TraceLine(data.src, End_Point, MASK_SHOT, local, &data.enter_trace);
			UTIL_ClipTraceToPlayers(data.src, End_Point * 40.f, MASK_SHOT, &data.filter, &data.enter_trace);

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

	bool TraceToExit(Vector& end, trace_t& enter_trace, Vector vStart, Vector vEnd, trace_t* exit_trace)
	{
		CApplication* pApp = CApplication::Instance();
		float distance = 0.0f;

		while (distance <= 90.0f)
		{
			distance += 4.0f;
			end = vStart + vEnd * distance;

			auto point_contents = pApp->EngineTrace()->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);
			if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX)))
				continue;

			auto new_end = end - (vEnd * 4.0f);

			TraceLine(end, new_end, (MASK_SHOT | CONTENTS_GRATE), nullptr, exit_trace);

			if (exit_trace->hit_entity == nullptr)
				return false;

			if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX)
			{
				TraceLine(end, vStart, 0x600400B, (IClientEntity*)exit_trace->hit_entity, exit_trace);

				if ((exit_trace->fraction < 1.0f || exit_trace->allsolid) && !exit_trace->startsolid)
				{
					end = exit_trace->endpos;
					return true;
				}
				continue;
			}

			if (!(exit_trace->fraction < 1.0 || exit_trace->allsolid || exit_trace->startsolid) || exit_trace->startsolid)
			{
				if (exit_trace->hit_entity)
				{
					if (DidHitNonWorldEntity && IsBreakableEntity((IClientEntity*)enter_trace.hit_entity))
						return true;
				}
				continue;
			}

			if (((exit_trace->surface.flags >> 7) & 1) && !((enter_trace.surface.flags >> 7) & 1))
				continue;

			if (exit_trace->plane.normal.Dot(vEnd) <= 1.0f)
			{
				float fraction = exit_trace->fraction * 4.0f;
				end = end - (vEnd * fraction);

				return true;
			}
		}
		return false;
	}

	bool HandleBulletPenetration(CWeaponInfo *wpn_data, FireBulletData &data)
	{
		surfacedata_t* enter_surface_data = CApplication::Instance()->PhysicsSurfaceProps()->GetSurfaceData(data.enter_trace.surface.surfaceProps);
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

		surfacedata_t* exit_surface_data = CApplication::Instance()->PhysicsSurfaceProps()->GetSurfaceData(trace_exit.surface.surfaceProps);
		int exit_material = exit_surface_data->game.material;
		float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
		float final_damage_modifier = 0.16f;
		float combined_penetration_modifier = 0.0f;

		if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
		{
			combined_penetration_modifier = 3.0f;
			final_damage_modifier = 0.05f;
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
		float thickness = (trace_exit.endpos - data.enter_trace.endpos).Length();
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
}