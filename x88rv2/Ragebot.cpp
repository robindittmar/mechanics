#include "Ragebot.h"
#include "Application.h"

CRagebot::CRagebot()
{
	m_bSilentAim = false;
	m_bAutoshoot = true;
	m_bAutoscope = false;
	m_bDoNoRecoil = true;
	m_bAutoReload = true;

	m_iTargetCriteria = TARGETCRITERIA_UNSPECIFIED;
}

CRagebot::~CRagebot()
{
}

void CRagebot::Setup()
{
	m_pApp = CApplication::Instance();

	m_pTargetSelector = m_pApp->TargetSelector();
	m_pEngineClient = m_pApp->EngineClient();
	m_pEntityList = m_pApp->EntityList();
}

/*int GetBoneByName(CApplication* pApp, IClientEntity* player, const char* bone)
{
	studiohdr_t pStudioModel = pApp->ModelInfo()->GetStudioModel(player->GetModel());
	if (!pStudioModel)
		return -1;

	return 0;
}*/

void CRagebot::Update(void* pParameters)
{
	if (!m_bIsEnabled)
		return;

	this->ResetTickVariables();

	// Local vars
	CreateMoveParam* pParam;
	CUserCmd* pUserCmd;
	IClientEntity* pLocalEntity;
	CWeapon* pMyActiveWeapon;

	// Initialize local variables
	pParam = (CreateMoveParam*)pParameters;
	pUserCmd = pParam->pUserCmd;

	pLocalEntity = m_pEntityList->GetClientEntity(m_pEngineClient->GetLocalPlayer());
	if (!pLocalEntity)
		return;

	pMyActiveWeapon = pLocalEntity->GetActiveWeapon();
	if (!pMyActiveWeapon)
		return;

	// Aimbot invalid weapons
	if (pMyActiveWeapon->IsKnife() ||
		pMyActiveWeapon->IsNade() ||
		pMyActiveWeapon->IsC4())
		return;

	// Clip empty?
	if (pMyActiveWeapon->GetClip1() == 0)
	{
		if(m_bAutoReload)
			pUserCmd->buttons |= IN_RELOAD;
		
		return;
	}

	// Choose target
	m_pTarget = m_pTargetSelector->GetTarget(m_iTargetCriteria);
	if (!m_pTarget) // This should never happen
		return;

	if (!m_pTarget->GetValid())
		return;

	// Copy target angles
	m_qAimAngles = *m_pTarget->GetAimAngles();

	// Set ClientViewAngles if we don't have silentaim activated
	// (we do so before applying NoRecoil :D)
	if (!this->m_bSilentAim)
	{
		m_pApp->SetClientViewAngles(m_qAimAngles);
	}

	// Do NoRecoil
	this->ApplyNoRecoil(pLocalEntity);
	// Apply viewangles & shoot if necessary
	this->ApplyViewanglesAndShoot(pUserCmd, pLocalEntity);
}

inline bool DidHitWorld(IClientEntity* m_pEnt)
{
	return m_pEnt->EntIndex() == 0;
}

inline bool DidHitNonWorldEntity(IClientEntity* m_pEnt)
{
	return m_pEnt != NULL && !DidHitWorld(m_pEnt);
}

bool HandleBulletPenetration(CWeaponInfo *wpn_data, FireBulletData &data);

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

inline vec_t VectorLength(const Vector& v)
{
	return (vec_t)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
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

/*bool CRagebot::CanHit(Vector &point, float *damage_given)
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
}*/


void inline CRagebot::ResetTickVariables()
{
	m_bIsShooting = false;
	m_bDidNoRecoil = false;
	
	m_fDamage = 0.0f;
}

//bool CRagebot::ChooseTarget(float fInputSampleTime, CUserCmd* pUserCmd)
//{
//	// No selected target
//	m_iSelectedTarget = -1;
//
//	int iLocalPlayerIdx = m_pEngineClient->GetLocalPlayer();
//	IClientEntity* pLocalEntity = m_pEntityList->GetClientEntity(iLocalPlayerIdx);
//	IClientEntity* pCurEntity;
//	CWeapon* pMyActiveWeapon;
//	QAngle qAimAngles;
//	QAngle qLocalViewAngles;
//
//	// Return if localplayer invalid
//	if (!pLocalEntity)
//		return false;
//	
//	// Get headpos & add eyeoffset
//	Vector vMyHeadPos;
//	int iMyTeamNum;
//
//	Vector vTargetPos;
//	QAngle qTargetAngles;
//
//	matrix3x4_t pBoneMatrix[MAXSTUDIOBONES];
//	Vector vEnemyHeadPos;
//
//	Ray_t ray;
//	trace_t trace;
//	CTraceFilterSkipEntity traceFilter(pLocalEntity);
//	
//	// Model stuff (hitboxes)
//	studiohdr_t* pStudioModel;
//	mstudiohitboxset_t* pHitboxSet;
//	mstudiobbox_t* pHitbox;
//
//	// Hitbox
//	Vector vHitbox[9];
//
//	float fViewangleDist;
//	float fOriginDist;
//	float fLowestDist = 999999.0f;
//	float fDamage;
//
//	// Grab my values
//	iMyTeamNum = pLocalEntity->GetTeamNum();
//	vMyHeadPos = *pLocalEntity->GetOrigin() + (*pLocalEntity->GetVelocity() * fInputSampleTime);
//	vMyHeadPos += *pLocalEntity->GetEyeOffset();
//	pMyActiveWeapon = (CWeapon*)pLocalEntity->GetActiveWeapon();
//	qLocalViewAngles = m_pApp->GetClientViewAngles();
//	
//	//g_pConsole->Write("%f\n", pMyActiveWeapon->GetAccuracyPenalty());
//
//	// If we're not autoshooting or not attacking ourselves
//	// TODO: Aimkey?
//	if (!this->m_bAutoshoot && !(pUserCmd->buttons & IN_ATTACK) && !(GetAsyncKeyState(VK_MBUTTON) & 0x8000))
//		return false;
//
//	// If invalid weapon for aimbot
//	if (pMyActiveWeapon->IsKnife() ||
//		pMyActiveWeapon->IsNade() ||
//		pMyActiveWeapon->IsC4() ||
//		pMyActiveWeapon->GetClip1() == 0)
//		return false;
//
//	int iMaxEntities = m_pApp->EngineClient()->GetMaxClients();
//	for (int i = 1; i < iMaxEntities; i++)
//	{
//		pCurEntity = m_pEntityList->GetClientEntity(i);
//
//		// Filter entites
//		if (!pCurEntity)
//			continue;
//
//		// Skip dormant entities
//		if (pCurEntity->IsDormant())
//			continue;
//		
//		// Can't shoot ourself
//		if (i == iLocalPlayerIdx)
//			continue;
//
//		// If the possible target isn't alive
//		if (!pCurEntity->IsAlive())
//			continue;
//
//		// Only from enemy team & we don't want spectators or something
//		int entityTeam = pCurEntity->GetTeamNum();
//		if (entityTeam == iMyTeamNum || entityTeam != 2 && entityTeam != 3)
//			continue;
//
//		// Spawn protection
//		if (pCurEntity->IsInvincible())
//			continue;
//
//		if (!pCurEntity->SetupBones(pBoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, m_pApp->EngineClient()->GetLastTimeStamp()))
//			continue;
//
//		const model_t* pModel = pCurEntity->GetModel();
//		if (!pModel)
//			continue;
//		
//		pStudioModel = m_pApp->ModelInfo()->GetStudiomodel(pModel);
//		if (!pStudioModel)
//			continue;
//
//		pHitboxSet = pStudioModel->pHitboxSet(0);
//		if (!pHitboxSet)
//			continue;
//
//		//
//		// = TODO =
//		// -> GetCenterOfHitbox
//		// =============
//		// -> FixHitbox
//		// -> Multipoint scan variable enable/disable
//		//
//
//		bool bIsHittable = false;
//		IEngineTrace* pEngineTrace = m_pApp->EngineTrace();
//		Vector vCurVelocity = (*pCurEntity->GetVelocity() * fInputSampleTime);
//		for (int i = 0; i < HITBOX_MAX; i++)
//		{
//			if (!m_bCheckHitbox[i])
//				continue;
//
//			pHitbox = pHitboxSet->pHitbox(i);
//			if (!pHitbox)
//				continue;
//
//			GetHitBoxVectors(pHitbox, pBoneMatrix, vHitbox);
//			// TODO: FixHitbox?
//
//			for (int i = 0; i < sizeof(vHitbox) / sizeof(Vector); i++)
//			{
//				vHitbox[i] += vCurVelocity;
//
//				ray.Init(vMyHeadPos, vHitbox[i]);
//				pEngineTrace->TraceRay(ray, (MASK_SHOT_HULL | CONTENTS_HITBOX), &traceFilter, &trace);
//				if (trace.IsEntityVisible(pCurEntity))
//				{
//					vEnemyHeadPos = vHitbox[i];
//					bIsHittable = true;
//					break;
//				}
//				else if (CanHit(vHitbox[i], &fDamage))
//				{
//					if (fDamage > m_fDamage)
//					{
//						m_iTargetBone = i;
//						vEnemyHeadPos = vHitbox[i];
//
//						m_fDamage = fDamage;
//						bIsHittable = true;
//					}
//				}
//			}
//
//			if (bIsHittable)
//				break;
//		}
//
//		// TODO: Don't delete this yet pls
//		//int curBone;
//		//int boneIdx[] = { 8, 10, 72, 79 };
//		//int bone = 0;
//		//int boneCount = sizeof(boneIdx) / sizeof(int);
//		//Vector vBonePos;
//		/*for (bone = 0; bone < boneCount; bone++)
//		{
//			curBone = boneIdx[bone];
//
//			// Get matrix for current bone
//			//pBoneMatrix = (matrix3x4_t*)((*(DWORD*)((DWORD)pCurEntity + 0x2698)) + (0x30 * boneIdx[bone]));
//
//			// Get position
//			vBonePos.x = boneMatrix[curBone].c[0][3];
//			vBonePos.y = boneMatrix[curBone].c[1][3];
//			vBonePos.z = boneMatrix[curBone].c[2][3];
//
//			// Prediction
//			vBonePos += (*pCurEntity->GetVelocity() * fInputSampleTime);
//
//			//ray.Init(vMyHeadPos, vEnemyHeadPos);
//			//m_pApp->EngineTrace()->TraceRay(ray, MASK_SHOT, &traceFilter, &trace);
//
//			if (CanHit(vBonePos, &fDamage))
//			{
//				if (fDamage > m_fDamage)
//				{
//					m_iTargetBone = bone;
//					vEnemyHeadPos = vBonePos;
//
//					m_fDamage = fDamage;
//					bIsHittable = true;
//				}
//			}
//			/*else if (trace.IsVisible(pCurEntity))
//			{
//				bIsHittable = true;
//				break;
//			}*/
//		//}
//
//		// Nothing visible :(
//		if (!bIsHittable)
//			continue;
//
//		switch (m_iTargetCriteria)
//		{
//		case TARGETCRITERIA_ORIGIN:
//			fOriginDist = this->GetOriginDist(vMyHeadPos, vEnemyHeadPos);
//			if (fOriginDist < fLowestDist)
//			{
//				fLowestDist = fOriginDist;
//
//				m_iSelectedTarget = i;
//				vTargetPos = vEnemyHeadPos;
//
//				continue;
//			}
//
//			break;
//		case TARGETCRITERIA_VIEWANGLES:
//			// Get Origin distance for "real" FOV (independent of distance)
//			//fOriginDist = this->GetOriginDist(myHeadPos, headPos);
//
//			// Calc angle
//			m_qAimAngles = this->CalcAngle(vMyHeadPos, vEnemyHeadPos);
//
//			// Calculate our fov to the enemy
//			fViewangleDist = fabs(this->GetViewangleDist(qLocalViewAngles, m_qAimAngles/*, fOriginDist*/));
//			if (fViewangleDist < fLowestDist)
//			{
//				fLowestDist = fViewangleDist;
//
//				m_iSelectedTarget = i;
//				qTargetAngles = m_qAimAngles;
//
//				continue;
//			}
//			break;
//		case TARGETCRITERIA_UNSPECIFIED:
//		default:
//			m_iSelectedTarget = i;
//			vTargetPos = vEnemyHeadPos;
//
//			// Force the entityloop to exit out (break would only break switch)
//			i = iMaxEntities;
//			break;
//		}
//	}
//
//	if (m_iTargetCriteria != TARGETCRITERIA_VIEWANGLES)
//	{
//		// Calculate our viewangles to aim at the enemy
//		m_qAimAngles = this->CalcAngle(vMyHeadPos, vTargetPos);
//	}
//	else
//	{
//		// We already got our viewangles :)
//		m_qAimAngles = qTargetAngles;
//	}
//
//	return (m_iSelectedTarget != -1);
//}

void CRagebot::ApplyNoRecoil(IClientEntity* pLocalEntity)
{
	// If we have no recoil activated in the aimbot, do it
	// (and remember that we did!)
	if (m_bDoNoRecoil)
	{
		QAngle aimPunchAngle = *(QAngle*)((DWORD)pLocalEntity + (OFFSET_LOCAL + OFFSET_AIMPUNCHANGLE));
		m_qAimAngles.x -= aimPunchAngle.x * m_pApp->GetRecoilCompensation();
		m_qAimAngles.y -= aimPunchAngle.y * m_pApp->GetRecoilCompensation();

		m_pApp->m_oldAimPunchAngle.x = aimPunchAngle.x * m_pApp->GetRecoilCompensation();
		m_pApp->m_oldAimPunchAngle.y = aimPunchAngle.y * m_pApp->GetRecoilCompensation();

		m_bDidNoRecoil = true;
	}
}

void CRagebot::ApplyViewanglesAndShoot(CUserCmd* pUserCmd, IClientEntity* pLocalEntity)
{
	CWeapon* pActiveWeapon = (CWeapon*)pLocalEntity->GetActiveWeapon();
	float fNextattack = pActiveWeapon->GetNextPrimaryAttack();
	float fServertime = pLocalEntity->GetTickBase() * m_pApp->GlobalVars()->interval_per_tick;

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

void inline CRagebot::Shoot(CUserCmd* pUserCmd, float fNextPrimaryAttack, float fServerTime)
{
	if (fNextPrimaryAttack <= fServerTime)
	{
		this->Aim(pUserCmd);

		// Shoot
		pUserCmd->buttons |= IN_ATTACK;
		m_bIsShooting = true;
	}
}

void inline CRagebot::Aim(CUserCmd* pUserCmd)
{
	// Write viewangles
	pUserCmd->viewangles[0] = m_qAimAngles.x;
	pUserCmd->viewangles[1] = m_qAimAngles.y;
}