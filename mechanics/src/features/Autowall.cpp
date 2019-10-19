#include "Autowall.h"
#include "../Application.h"
#include "../utils/Utils.h"

namespace Autowall
{
	// Function
	bool CanHit(Vector &point, float *damage_given, Vector* vHeadPos, IClientEntity* pEntity)
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

		CWeapon* pActiveWeapon = (!pEntity ? local->GetActiveWeapon() : pEntity->GetActiveWeapon());
		if (SimulateFireBullet(local, pActiveWeapon, data))
		{
			*damage_given = data.current_damage;

			return true;
		}
		return false;
	}
	bool SimulateFireBullet(IClientEntity* local, CWeapon* weapon, FireBulletData& data)
	{
		data.penetrate_count = 4;
		data.trace_length = 0.0f;
		CWeaponInfo* wpn_data = weapon->GetWeaponInfo();
		data.current_damage = (float)wpn_data->m_WeaponDamage;

		while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
		{
			data.trace_length_remaining = wpn_data->m_WeaponRange - data.trace_length;
			Vector End_Point = data.src + data.direction * data.trace_length_remaining;

			TraceLine(data.src, End_Point, MASK_SHOT, &data.enter_trace);
			ClipTraceToPlayers(data.src, End_Point * 40.f, MASK_SHOT, &data.filter, &data.enter_trace);

			if (data.enter_trace.fraction == 1.0f)
				break;

			if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0) && (local->GetTeamNum() != data.enter_trace.hit_entity->GetTeamNum()))
			{
				data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
				data.current_damage *= pow(wpn_data->m_RangeModifier, data.trace_length * 0.002);
				ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.hit_entity, wpn_data->m_ArmorRatio, data.current_damage);
				return true;
			}

			if (!HandleBulletPenetration(wpn_data, data))
				break;
		}
		return false;
	}

	// Engine Functions
	void TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, trace_t* tr, IClientEntity* ignore)
	{
		Ray_t ray;
		if (!ignore)
		{
			ignore = CApplication::Instance()->GetLocalPlayer();
		}
		CTraceFilterSkipEntity filter(ignore);

		ray.Init(vecAbsStart, vecAbsEnd);
		CApplication::Instance()->EngineTrace()->TraceRay(ray, mask, &filter, tr);
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

			TraceLine(end, new_end, (MASK_SHOT | CONTENTS_GRATE), exit_trace, nullptr);

			if (exit_trace->hit_entity == nullptr)
				return false;

			if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX)
			{
				TraceLine(end, vStart, 0x600400B, exit_trace, (IClientEntity*)exit_trace->hit_entity);

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
	bool IsBreakableEntity(IClientEntity* pEntity)
	{
		// todo: rebuild !!
		typedef bool(__thiscall *isBreakbaleEntityFn)(IClientEntity*);
		static isBreakbaleEntityFn IsBreakableEntity;

		if (!IsBreakableEntity)
			IsBreakableEntity = (isBreakbaleEntityFn)CPattern::FindPattern((BYTE*)CApplication::Instance()->ClientDll(), CLIENTDLL_SIZE, (BYTE*)"\x55\x8B\xEC\x51\x56\x8B\xF1\x85\xF6\x74\x68", "krngpthxzek");

		return IsBreakableEntity(pEntity);
	}
	void ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
	{
		CApplication* pApp = CApplication::Instance();

		trace_t playerTrace;
		Ray_t ray;
		float smallestFraction = tr->fraction;
		const float maxRange = 60.0f;

		ray.Init(vecAbsStart, vecAbsEnd);

		for (int k = 1; k <= pApp->EngineClient()->GetMaxClients(); ++k)
		{
			IClientEntity* player = pApp->EntityList()->GetClientEntity(k);

			if (!player || !player->IsAlive())
				continue;

			if (player->IsDormant())
				continue;

			if (filter && filter->ShouldHitEntity(player, mask) == false)
				continue;

			Vector vMax = ((CWeapon*)player)->GetVecMax() + *player->GetOrigin();
			Vector vMin = ((CWeapon*)player)->GetVecMin() + *player->GetOrigin();
			Vector vSize = vMax - vMin;
			vSize /= 2;
			vSize += vMin;

			float range = DistanceToRay(vSize, vecAbsStart, vecAbsEnd);
			if (range < 0.0f || range > maxRange)
				continue;

			pApp->EngineTrace()->ClipRayToEntity(ray, (mask | CONTENTS_HITBOX), player, &playerTrace);
			if (playerTrace.fraction < smallestFraction)
			{
				tr = &playerTrace;
				smallestFraction = playerTrace.fraction;
			}
		}
	}
	bool HandleBulletPenetration(CWeaponInfo* wpn_data, FireBulletData &data)
	{
		surfacedata_t* enter_surface_data = CApplication::Instance()->PhysicsSurfaceProps()->GetSurfaceData(data.enter_trace.surface.surfaceProps);
		int enter_material = enter_surface_data->game.material;
		float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;

		data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
		data.current_damage *= pow(wpn_data->m_RangeModifier, (data.trace_length * 0.002));

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
		float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.f, (3.0f / wpn_data->m_Penetration) * 1.25f);
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

	// Helper Functions
	bool IsArmored(IClientEntity* pEntity, int iArmorValue, int iHitgroup)
	{
		bool result = false;

		if (iArmorValue > 0)
		{
			switch (iHitgroup)
			{
			case HITGROUP_GENERIC:
			case HITGROUP_CHEST:
			case HITGROUP_STOMACH:
			case HITGROUP_LEFTARM:
			case HITGROUP_RIGHTARM:
				result = true;
				break;
			case HITGROUP_HEAD:
				result = pEntity->HasHelmet();
				break;
			}
		}

		return result;
	}
	void ScaleDamage(int iHitgroup, IClientEntity* pEntity, float weapon_armor_ratio, float &current_damage)
	{
		bool bHeavyArmor = pEntity->HasHeavyArmor();
		auto iArmorValue = pEntity->GetArmor();

		switch (iHitgroup)
		{
		case HITGROUP_HEAD:
			if (bHeavyArmor)
				current_damage = (current_damage * 4.0f) * 0.5f;
			else
				current_damage *= 4.0f;
			break;
		case HITGROUP_STOMACH:
			current_damage *= 1.25f;
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			current_damage *= 0.75f;
			break;
		}

		if (IsArmored(pEntity, iArmorValue, iHitgroup))
		{
			float v47 = 1.f, ArmorBonusRatio = 0.5f, ArmorRatio = weapon_armor_ratio * 0.5f;

			if (bHeavyArmor)
			{
				ArmorBonusRatio = 0.33f;
				ArmorRatio = (weapon_armor_ratio * 0.5f) * 0.5f;
				v47 = 0.33f;
			}

			float NewDamage = current_damage * ArmorRatio;

			if (bHeavyArmor)
				NewDamage *= 0.85f;

			if (((current_damage - (current_damage * ArmorRatio)) * (v47 * ArmorBonusRatio)) > iArmorValue)
				NewDamage = current_damage - (iArmorValue / ArmorBonusRatio);

			current_damage = NewDamage;
		}
	}

	float DistanceToRay(const Vector& pos, const Vector& rayStart, const Vector& rayEnd, float* along, Vector* pointOnRay)
	{
		Vector to = pos - rayStart;
		Vector dir = rayEnd - rayStart;
		float length = dir.NormalizeInPlace();

		float rangeAlong = dir.Dot(to);
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

	inline bool DidHitNonWorldEntity(IClientEntity* pEntity)
	{
		return pEntity != NULL && !DidHitWorld(pEntity);
	}
	inline bool DidHitWorld(IClientEntity* pEntity)
	{
		return pEntity->EntIndex() == 0;
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