#include "Aimbot.h"
#include "Application.h"
#include "ray.h"
#include "Console.h"

CAimbot::CAimbot()
{
	m_bSilentAim = false;
	m_bAutoshoot = true;
	m_bAutoscope = false;
	m_bDoNoRecoil = true;

	m_tTargetCriteria = TargetCriteriaUnspecified;
	m_fSpeed = 1.0f;
	m_fFov = 360.0f;
}

CAimbot::~CAimbot()
{
}

void CAimbot::Setup()
{
	m_pApp = CApplication::Instance();

	m_pEngineClient = m_pApp->EngineClient();
	m_pEntityList = m_pApp->EntityList();
}

int GetBoneByName(CApplication* pApp, IClientEntity* player, const char* bone)
{
	/*studiohdr_t pStudioModel = pApp->ModelInfo()->GetStudioModel(player->GetModel());
	if (!pStudioModel)
		return -1;

	*/
	return 0;
}

struct mat3x4
{
	float c[3][4];
};

void CAimbot::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	this->ResetTickVariables();

	// Local vars
	AimbotUpdateParam* pParam;
	CUserCmd* pUserCmd;
	IClientEntity* pLocalEntity;
	// TODO
	//*m_pApp->m_bSendPackets = true;

	// Initialize local variables
	pParam = (AimbotUpdateParam*)pParameters;
	pUserCmd = pParam->pUserCmd;
	pLocalEntity = m_pEntityList->GetClientEntity(m_pEngineClient->GetLocalPlayer());

	// Choose target
	if (!this->ChooseTarget(pParam->fInputSampleTime, pUserCmd))
		return;

	// Set ClientViewAngles if we don't have silentaim activated
	// (we do so before applying NoRecoil :D)
	if (!this->m_bSilentAim)
	{
		m_pApp->ClientViewAngles(m_qAimAngles);
	}

	// Do NoRecoil
	this->ApplyNoRecoil(pLocalEntity);
	// Apply viewangles & shoot if necessary
	this->ApplyViewanglesAndShoot(pUserCmd, pLocalEntity);
	
	// TODO
	//if (this->m_bIsShooting)
	//	*m_pApp->m_bSendPackets = false;
}

float GetHitgroupDamageMultiplier(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_HEAD:
		return 4.0f;
	case HITGROUP_CHEST:
	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:
		return 1.0f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	default:
		return 1.0f;
	}
}

void TraceLine(Vector vecAbsStart, Vector vecAbsEnd, unsigned int mask, IClientEntity* ignore, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilterSkipEntity filter(ignore);

	CApplication::Instance()->EngineTrace()->TraceRay(ray, mask, &filter, ptr);
}

bool TraceToExit(Vector& end, trace_t* enter_trace, Vector start, Vector dir, trace_t* exit_trace) {
	float distance = 0.0f;

	while (distance <= 90.0f)
	{
		distance += 4.0f;
		end = start + dir * distance;

		auto point_contents = CApplication::Instance()->EngineTrace()->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL);

		if (point_contents & MASK_SHOT_HULL && !(point_contents & CONTENTS_HITBOX))
			continue;

		auto new_end = end - (dir * 4.0f);

		Ray_t ray;
		ray.Init(end, new_end);
		CApplication::Instance()->EngineTrace()->TraceRay(ray, MASK_SHOT, 0, exit_trace);

		if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX)
		{
			ray.Init(end, start);

			CTraceFilterSkipEntity filter(exit_trace->hit_entity);

			CApplication::Instance()->EngineTrace()->TraceRay(ray, 0x600400B, &filter, exit_trace);

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
				if (enter_trace->hit_entity && enter_trace->hit_entity == CApplication::Instance()->EntityList()->GetClientEntity(CApplication::Instance()->Aimbot()->SelectedTarget()))
					return true;
			}

			continue;
		}

		if (exit_trace->surface.flags >> 7 & 1 && !(enter_trace->surface.flags >> 7 & 1))
			continue;

		if (exit_trace->plane.normal.Dot(dir) <= 1.0f)
		{
			auto fraction = exit_trace->fraction * 4.0f;
			end = end - (dir * fraction);

			return true;
		}
	}

	return false;
}

bool HandleBulletPenetration(CWeaponInfo *wpn_data, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = CApplication::Instance()->PhysicsSurfaceProps()->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = *(float*)((uint8_t*)enter_surface_data + 76);

	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= powf(wpn_data->RangeModifier(), data.trace_length * 0.002f);

	if (data.trace_length > 3000.f || enter_surf_penetration_mod < 0.1f)
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;
	trace_t trace_exit;

	if (!TraceToExit(dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;

	surfacedata_t *exit_surface_data = CApplication::Instance()->PhysicsSurfaceProps()->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = *(float*)((uint8_t*)exit_surface_data + 76);
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if ((data.enter_trace.contents & CONTENTS_GRATE) != 0 || enter_material == 89 || enter_material == 71)
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->Penetration()) * 1.25f);
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

void ScaleDamage(int hitgroup, IClientEntity *enemy, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMultiplier(hitgroup);

	if (enemy->Armor() > 0)
	{
		if (hitgroup == HITGROUP_HEAD)
		{
			if (enemy->HasHelmet())
				current_damage *= (weapon_armor_ratio * .5f);
		}
		else
		{
			current_damage *= (weapon_armor_ratio * .5f);
		}
	}
}

bool CAimbot::SimulateFireBullet(IClientEntity *local, bool teamCheck, FireBulletData &data)
{
	CWeaponInfo* weaponInfo = ((CWeapon*)local->ActiveWeapon())->GetWeaponInfo();

	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	data.current_damage = (float)weaponInfo->Damage();

	while (data.penetrate_count > 0 && data.current_damage >= 1.0f)
	{
		data.trace_length_remaining = weaponInfo->Range() - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		// data.enter_trace
		TraceLine(data.src, end, MASK_SHOT, local, &data.enter_trace);

		Ray_t ray;
		ray.Init(data.src, end + data.direction * 40.f);

		m_pApp->EngineTrace()->TraceRay(ray, MASK_SHOT, &data.filter, &data.enter_trace);

		TraceLine(data.src, end + data.direction * 40.f, MASK_SHOT, local, &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f)
			break;

		if (data.enter_trace.hitgroup <= HITGROUP_RIGHTLEG && data.enter_trace.hitgroup > 0)
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= powf(weaponInfo->RangeModifier(), data.trace_length * 0.002f);

			IClientEntity* player = (IClientEntity*)data.enter_trace.hit_entity;
			if (teamCheck && player->TeamNum() == local->TeamNum())
				return false;

			ScaleDamage(data.enter_trace.hitgroup, player, weaponInfo->ArmorRatio(), data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(weaponInfo, data))
			break;
	}

	return false;
}

QAngle CAimbot::CalcAngle(Vector& vStartPos, Vector& vEndPos)
{
	Vector vRelativeDist = vEndPos - vStartPos;
	QAngle qAngle(
		RAD2DEG(-asinf(vRelativeDist.z / vRelativeDist.Length())),
		RAD2DEG(atan2f(vRelativeDist.y, vRelativeDist.x)),
		0.0f
	);

	return qAngle;
}

float CAimbot::GetOriginDist(Vector& vSource, Vector& vTarget)
{
	return (vTarget - vSource).Length();
}

float CAimbot::GetViewangleDist(QAngle& qSource, QAngle& qTarget/*, float fOriginDistance*/)
{
	QAngle qDist = qTarget - qSource;
	qDist.Normalize();
	float fAng = qDist.Length();

	// Causes weird angles to have giant "fovs"
	//return (sinf(DEG2RAD(fAng)) * fOriginDistance);
	return fAng;
}

void inline CAimbot::ResetTickVariables()
{
	m_bHasTarget = false;
	m_bIsShooting = false;
	m_bDidNoRecoil = false;
}

bool CAimbot::ChooseTarget(float fInputSampleTime, CUserCmd* pUserCmd)
{
	// No selected target
	m_iSelectedTarget = -1;

	int iLocalPlayerIdx = m_pEngineClient->GetLocalPlayer();
	IClientEntity* pLocalEntity = m_pEntityList->GetClientEntity(iLocalPlayerIdx);
	IClientEntity* pCurEntity;
	CWeapon* pMyActiveWeapon;
	QAngle qAimAngles;
	QAngle qLocalViewAngles;

	// Return if localplayer invalid
	if (!pLocalEntity)
		return false;
	
	// Get headpos & add eyeoffset
	Vector vMyHeadPos;
	int iMyTeamNum;

	Vector vTargetPos;
	QAngle qTargetAngles;

	mat3x4* pBoneMatrix;
	Vector vEnemyHeadPos;

	Ray_t ray;
	trace_t trace;
	CTraceFilterSkipEntity traceFilter(pLocalEntity);
	
	float fViewangleDist;
	float fOriginDist;
	float fLowestDist = 999999.0f;
	float fDamage;

	// Grab my values
	iMyTeamNum = pLocalEntity->TeamNum();
	vMyHeadPos = *pLocalEntity->Origin() + (*pLocalEntity->Velocity() * fInputSampleTime);
	vMyHeadPos += *pLocalEntity->EyeOffset();
	pMyActiveWeapon = (CWeapon*)pLocalEntity->ActiveWeapon();
	qLocalViewAngles = m_pApp->ClientViewAngles();
	
	// If we're not autoshooting or not attacking ourselves
	if (!this->m_bAutoshoot && !(pUserCmd->buttons & IN_ATTACK))
		return false;

	// If invalid weapon for aimbot
	if (pMyActiveWeapon->IsKnife() ||
		pMyActiveWeapon->IsNade() ||
		pMyActiveWeapon->IsC4() ||
		pMyActiveWeapon->Clip1() == 0)
		return false;

	int iMaxEntities = m_pApp->EngineClient()->GetMaxClients();
	for (int i = 1; i < iMaxEntities; i++)
	{
		pCurEntity = m_pEntityList->GetClientEntity(i);

		// Filter entites
		if (!pCurEntity)
			continue;

		// Skip dormant entities
		if (pCurEntity->IsDormant())
			continue;

		// Can't shoot ourself
		if (i == iLocalPlayerIdx)
			continue;

		// If the possible target isn't alive
		if (!pCurEntity->IsAlive())
			continue;

		// Only from enemy team & we don't want spectators or something
		int entityTeam = pCurEntity->TeamNum();
		if (entityTeam == iMyTeamNum || entityTeam != 2 && entityTeam != 3)
			continue;

		// Spawn protection
		if (pCurEntity->IsInvincible())
			continue;

		// Bone ID: 8 (maybe 7=neck)
		// 10 ca chest
		//studiohdr_t* pModel = m_pApp->ModelInfo()->GetStudioModel(pCurEntity->GetModel());

		// TODO
		// 8, 10, 72, 79
		bool bIsHittable = false;
		int boneIdx[] = { 8, 10, 72, 79 };
		int bone = 0;
		int boneCount = sizeof(boneIdx) / sizeof(int);

		for (bone = 0; bone < boneCount; bone++)
		{
			// Get matrix for current bone
			pBoneMatrix = (mat3x4*)((*(DWORD*)((DWORD)pCurEntity + 0x2698)) + (0x30 * boneIdx[bone]));

			// Get position
			vEnemyHeadPos.x = pBoneMatrix->c[0][3];
			vEnemyHeadPos.y = pBoneMatrix->c[1][3];
			vEnemyHeadPos.z = pBoneMatrix->c[2][3];

			// Prediction
			vEnemyHeadPos += (*pCurEntity->Velocity() * fInputSampleTime);

			ray.Init(vMyHeadPos, vEnemyHeadPos);
			m_pApp->EngineTrace()->TraceRay(ray, MASK_SHOT, &traceFilter, &trace);

			if (trace.IsVisible(pCurEntity))
			{
				bIsHittable = true;
				break;
			}
			/*else
			{
			m_iSelectedTarget = i;

			FireBulletData data(myHeadPos, pLocalEntity);

			QAngle angles;
			Vector headPosTemp = headPos - myHeadPos;
			angles = this->CalcAngle(headPosTemp);
			AngleVectors(angles, &data.direction, NULL, NULL);
			data.direction.Normalize();

			if (SimulateFireBullet(pLocalEntity, true, data))
			{
			fDamage = data.current_damage;
			if (fDamage > 0.0f)
			{
			isVisible = true;
			break;
			}
			}
			}*/
		}

		// Nothing visible :(
		if (!bIsHittable)
			continue;

		switch (m_tTargetCriteria)
		{
		case TargetCriteriaOrigin:
			fOriginDist = this->GetOriginDist(vMyHeadPos, vEnemyHeadPos);
			if (fOriginDist < fLowestDist)
			{
				fLowestDist = fOriginDist;

				m_iSelectedTarget = i;
				vTargetPos = vEnemyHeadPos;

				continue;
			}

			break;
		case TargetCriteriaViewangle:
			// Get Origin distance for "real" FOV (independent of distance)
			//fOriginDist = this->GetOriginDist(myHeadPos, headPos);

			// Calc angle
			m_qAimAngles = this->CalcAngle(vMyHeadPos, vEnemyHeadPos);

			// Calculate our fov to the enemy
			fViewangleDist = fabs(this->GetViewangleDist(qLocalViewAngles, m_qAimAngles/*, fOriginDist*/));
			// TODO: GetViewangleDist doesn't return a nice FOV, as it doesn't take fOriginDist into account (RIGHT NOW!)
			if (fViewangleDist > m_fFov)
			{
				continue;
			}
			else if (fViewangleDist < fLowestDist)
			{
				fLowestDist = fViewangleDist;

				m_iSelectedTarget = i;
				qTargetAngles = m_qAimAngles;

				continue;
			}
			break;
		case TargetCriteriaUnspecified:
			m_iSelectedTarget = i;
			vTargetPos = vEnemyHeadPos;

			// Force the entityloop to exit out (break would only break switch)
			i = iMaxEntities;
			break;
		}
	}

	if (m_tTargetCriteria != TargetCriteriaViewangle)
	{
		// Calculate our viewangles to aim at the enemy
		m_qAimAngles = this->CalcAngle(vMyHeadPos, vTargetPos);
	}
	else
	{
		// We already got our viewangles :)
		m_qAimAngles = qTargetAngles;
	}

	return (m_iSelectedTarget != -1);
}

void CAimbot::ApplyNoRecoil(IClientEntity* pLocalEntity)
{
	// If we have no recoil activated in the aimbot, do it
	// (and remember that we did!)
	if (m_bDoNoRecoil)
	{
		QAngle aimPunchAngle = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));
		m_qAimAngles.x -= aimPunchAngle.x * RECOIL_COMPENSATION;
		m_qAimAngles.y -= aimPunchAngle.y * RECOIL_COMPENSATION;

		m_pApp->m_oldAimPunchAngle.x = aimPunchAngle.x * RECOIL_COMPENSATION;
		m_pApp->m_oldAimPunchAngle.y = aimPunchAngle.y * RECOIL_COMPENSATION;

		m_bDidNoRecoil = true;
	}
}

void CAimbot::ApplyViewanglesAndShoot(CUserCmd* pUserCmd, IClientEntity* pLocalEntity)
{
	CWeapon* pActiveWeapon = (CWeapon*)pLocalEntity->ActiveWeapon();
	float fNextattack = pActiveWeapon->NextPrimaryAttack();
	float fServertime = pLocalEntity->TickBase() * m_pApp->GlobalVars()->interval_per_tick;

	if (this->m_bAutoshoot && pActiveWeapon->IsSniper() && !pActiveWeapon->IsTaser())
	{
		if (this->m_bAutoscope)
		{
			if (pLocalEntity->IsScoped())
			{
				this->Shoot(pUserCmd, fNextattack, fServertime);
			}
			else
			{
				pUserCmd->buttons |= IN_ATTACK2;
			}
		}
		else
		{
			this->Shoot(pUserCmd, fNextattack, fServertime);
		}
	}
	else if (this->m_bAutoshoot && !pActiveWeapon->IsTaser())
	{
		this->Shoot(pUserCmd, fNextattack, fServertime);
	}
	else if(!this->m_bAutoshoot)
	{
		this->Aim(pUserCmd);
	}
}

void inline CAimbot::Shoot(CUserCmd* pUserCmd, float fNextPrimaryAttack, float fServerTime)
{
	if (fNextPrimaryAttack <= fServerTime)
	{
		this->Aim(pUserCmd);

		// Shoot
		pUserCmd->buttons |= IN_ATTACK;
		m_bIsShooting = true;
	}
}

void inline CAimbot::Aim(CUserCmd* pUserCmd)
{
	// Write viewangles
	pUserCmd->viewangles[0] = m_qAimAngles.x;
	pUserCmd->viewangles[1] = m_qAimAngles.y;
}