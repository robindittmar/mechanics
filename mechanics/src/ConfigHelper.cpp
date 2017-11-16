#include "ConfigHelper.h"
#include "Application.h"

namespace ConfigHelper
{
	void ConfigToFeatures(CConfig* pConfig)
	{
		CApplication* pApp = CApplication::Instance();

		// Target Selector
		pApp->TargetSelector()->SetMultipoint(pConfig->GetBool("targetselector", "multipoint"));
		pApp->TargetSelector()->SetMultipointScale(pConfig->GetFloat("targetselector", "multipointscale"));
		pApp->TargetSelector()->SetVisibleMode(pConfig->GetInt("targetselector", "visiblemode"));
		pApp->TargetSelector()->SetCheckHitbox(TARGET_HITBOX_HEAD, pConfig->GetBool("targetselector", "checkhead"));
		pApp->TargetSelector()->SetCheckHitbox(TARGET_HITBOX_CHEST, pConfig->GetBool("targetselector", "checkchest"));
		pApp->TargetSelector()->SetCheckHitbox(TARGET_HITBOX_PELVIS, pConfig->GetBool("targetselector", "checkpelvis"));
		pApp->TargetSelector()->SetCheckHitbox(TARGET_HITBOX_RIGHT_FOREARM, pConfig->GetBool("targetselector", "checkrightforearm"));
		pApp->TargetSelector()->SetCheckHitbox(TARGET_HITBOX_LEFT_FOREARM, pConfig->GetBool("targetselector", "checkleftforearm"));
		pApp->TargetSelector()->SetCheckHitbox(TARGET_HITBOX_RIGHT_CALF, pConfig->GetBool("targetselector", "checkrightcalf"));
		pApp->TargetSelector()->SetCheckHitbox(TARGET_HITBOX_LEFT_CALF, pConfig->GetBool("targetselector", "checkleftcalf"));

		// Ragebot
		pApp->Ragebot()->SetEnabled(pConfig->GetBool("ragebot", "enabled"));
		pApp->Ragebot()->SetAutoshoot(pConfig->GetBool("ragebot", "autoshoot"));
		pApp->Ragebot()->SetAutoscope(pConfig->GetBool("ragebot", "autoscope"));
		pApp->Ragebot()->SetSilentAim(pConfig->GetBool("ragebot", "silentaim"));
		pApp->Ragebot()->SetAutoReload(pConfig->GetBool("ragebot", "autoreload"));
		pApp->Ragebot()->SetAutoZeus(pConfig->GetBool("ragebot", "autozeus"));
		pApp->Ragebot()->SetTargetCriteria(pConfig->GetInt("ragebot", "targetcriteria"));
		pApp->Ragebot()->SetCalculateHitchance(pConfig->GetBool("ragebot", "hitchanceenabled"));
		pApp->Ragebot()->SetHitchance(pConfig->GetFloat("ragebot", "hitchance"));
		pApp->Ragebot()->SetNoSpread(pConfig->GetBool("ragebot", "nospread"));
		pApp->Ragebot()->SetAutoRevolver(pConfig->GetBool("ragebot", "autorevolver"));

		// Legitbot
		pApp->Legitbot()->SetEnabled(pConfig->GetBool("legitbot", "enabled"));
		pApp->Legitbot()->SetTriggerKey(pConfig->GetInt("legitbot", "key"));
		pApp->Legitbot()->SetAutoshoot(pConfig->GetBool("legitbot", "autoshoot"));
		pApp->Legitbot()->SetHelpAfterShots(pConfig->GetInt("legitbot", "helpaftershots"));
		pApp->Legitbot()->SetTimeToAim(pConfig->GetFloat("legitbot", "aimtime"));
		pApp->Legitbot()->SetCurve(pConfig->GetFloat("legitbot", "curve"));
		pApp->Legitbot()->SetFieldOfView(pConfig->GetFloat("legitbot", "fov"));
		pApp->Legitbot()->SetPointScale(pConfig->GetFloat("legitbot", "pointscale"));
		pApp->Legitbot()->SetDrawFieldOfView(pConfig->GetBool("legitbot", "drawfov"));
		pApp->Legitbot()->SetFieldOfViewColor(pConfig->GetColor("legitbot", "fovcolor"));
		pApp->Legitbot()->SetDrawPath(pConfig->GetBool("legitbot", "drawpath"));
		pApp->Legitbot()->SetPathColor(pConfig->GetColor("legitbot", "pathcolor"));
		pApp->Legitbot()->SetCheckHitbox(TARGET_HITBOX_HEAD, pConfig->GetBool("legitbot", "checkhead"));
		pApp->Legitbot()->SetCheckHitbox(TARGET_HITBOX_CHEST, pConfig->GetBool("legitbot", "checkchest"));
		pApp->Legitbot()->SetCheckHitbox(TARGET_HITBOX_PELVIS, pConfig->GetBool("legitbot", "checkpelvis"));
		pApp->Legitbot()->SetCheckHitbox(TARGET_HITBOX_RIGHT_FOREARM, pConfig->GetBool("legitbot", "checkrightforearm"));
		pApp->Legitbot()->SetCheckHitbox(TARGET_HITBOX_LEFT_FOREARM, pConfig->GetBool("legitbot", "checkleftforearm"));
		pApp->Legitbot()->SetCheckHitbox(TARGET_HITBOX_RIGHT_CALF, pConfig->GetBool("legitbot", "checkrightcalf"));
		pApp->Legitbot()->SetCheckHitbox(TARGET_HITBOX_LEFT_CALF, pConfig->GetBool("legitbot", "checkleftcalf"));
		pConfig->SetBool("legitbot", "drawfov", pApp->Legitbot()->GetDrawFieldOfView());
		pConfig->SetColor("legitbot", "fovcolor", pApp->Legitbot()->GetFieldOfViewColor());
		// Triggerbot
		pApp->Triggerbot()->SetEnabled(pConfig->GetBool("triggerbot", "enabled"));
		pApp->Triggerbot()->SetShootDelay(pConfig->GetInt("triggerbot", "shootdelay"));
		pApp->Triggerbot()->SetShootDelayJitter(pConfig->GetInt("triggerbot", "shootdelayjitter"));

		// Legit Lag Compensation


		// Antiaim
		pApp->AntiAim()->SetEnabled(pConfig->GetBool("antiaim", "enabled"));
		pApp->AntiAim()->SetDrawLbyIndicator(pConfig->GetBool("antiaim", "lbyindicator"));
		pApp->AntiAim()->SetLbyBreaker(pConfig->GetBool("antiaim", "lbybreaker"));

		// Standing
		pApp->AntiAim()->SetPitchSettingStanding(pConfig->GetInt("antiaim", "standingpitch"));
		pApp->AntiAim()->SetYawSettingStanding(pConfig->GetInt("antiaim", "standingyaw"));
		pApp->AntiAim()->SetYawOffsetStanding(pConfig->GetInt("antiaim", "standingyawoffset"));
		pApp->AntiAim()->SetYawFakeSettingStanding(pConfig->GetInt("antiaim", "standingfakeyaw"));
		pApp->AntiAim()->SetYawFakeOffsetStanding(pConfig->GetInt("antiaim", "standingfakeyawoffset"));
		// Moving
		pApp->AntiAim()->SetPitchSettingMoving(pConfig->GetInt("antiaim", "movingpitch"));
		pApp->AntiAim()->SetYawSettingMoving(pConfig->GetInt("antiaim", "movingyaw"));
		pApp->AntiAim()->SetYawOffsetMoving(pConfig->GetInt("antiaim", "movingyawoffset"));
		pApp->AntiAim()->SetYawFakeSettingMoving(pConfig->GetInt("antiaim", "movingfakeyaw"));
		pApp->AntiAim()->SetYawFakeOffsetMoving(pConfig->GetInt("antiaim", "movingfakeyawoffset"));

		// Edge AA
		pApp->AntiAim()->SetDoEdgeAntiAim(pConfig->GetBool("antiaim", "edgeantiaim"));
		pApp->AntiAim()->SetEdgeAntiAimCheckPointsAmount(pConfig->GetInt("antiaim", "edgeantiaimcheckpoints"));
		pApp->AntiAim()->SetDrawEdgeAntiAimPoints(pConfig->GetBool("antiaim", "edgeantiaimdrawpoints"));
		pApp->AntiAim()->SetDrawEdgeAntiAimLines(pConfig->GetBool("antiaim", "edgeantiaimdrawlines"));

		// Resolver
		pApp->Resolver()->SetEnabled(pConfig->GetBool("resolver", "enabled"));
		pApp->Resolver()->SetResolverType(pConfig->GetInt("resolver", "type"));

		// Bhop
		pApp->Bhop()->SetEnabled(pConfig->GetBool("bhop", "enabled"));

		// Esp
		pApp->Esp()->SetEnabled(pConfig->GetBool("esp", "enabled"));
		pApp->Esp()->SetDrawBoundingBox(pConfig->GetInt("esp", "boundingbox"));
		pApp->Esp()->SetFillBoundingBox(pConfig->GetBool("esp", "fillboundingbox"));
		pApp->Esp()->SetDrawOutline(pConfig->GetBool("esp", "outlineboundingbox"));
		pApp->Esp()->SetDrawSkeleton(pConfig->GetBool("esp", "skeleton"));
		pApp->Esp()->SetDrawNames(pConfig->GetBool("esp", "names"));
		pApp->Esp()->SetDrawHealthBar(pConfig->GetBool("esp", "healthbar"));
		pApp->Esp()->SetDrawHealthNumber(pConfig->GetBool("esp", "healthnumber"));
		pApp->Esp()->SetDrawArmorBar(pConfig->GetBool("esp", "armorbar"));
		pApp->Esp()->SetDrawActiveWeapon(pConfig->GetBool("esp", "activeweapon"));
		pApp->Esp()->SetDrawAmmoBar(pConfig->GetBool("esp", "ammobar"));
		pApp->Esp()->SetDrawAmmoNumber(pConfig->GetBool("esp", "ammonumber"));
		pApp->Esp()->SetDrawOwnTeam(pConfig->GetBool("esp", "team"));
		pApp->Esp()->SetDrawOwnModel(pConfig->GetBool("esp", "self"));
		pApp->Esp()->SetDrawOnlyVisible(pConfig->GetBool("esp", "onlyvisible"));
		pApp->Esp()->SetDrawOnlySpotted(pConfig->GetBool("esp", "onlyspotted"));
		pApp->Esp()->SetDrawViewangles(pConfig->GetBool("esp", "viewangles"));
		pApp->Esp()->SetViewanglesLength(pConfig->GetInt("esp", "viewanglelength"));
		pApp->Esp()->SetFadeoutEnabled(pConfig->GetBool("esp", "fadeout"));
		pApp->Esp()->SetFadeoutTime(pConfig->GetFloat("esp", "fadeouttime"));
		pApp->Esp()->SetColorCT(pConfig->GetColor("esp", "colorct"));
		pApp->Esp()->SetColorT(pConfig->GetColor("esp", "colort"));
		pApp->Esp()->SetColorSpotted(pConfig->GetColor("esp", "colorspotted"));

		// WeaponEsp
		pApp->WeaponEsp()->SetEnabled(pConfig->GetBool("weaponesp", "enabled"));
		pApp->WeaponEsp()->SetDrawWeaponName(pConfig->GetBool("weaponesp", "weaponname"));
		pApp->WeaponEsp()->SetDrawWeaponBoundingBox(pConfig->GetBool("weaponesp", "weaponboundingbox"));
		pApp->WeaponEsp()->SetDrawGrenadeName(pConfig->GetBool("weaponesp", "grenadename"));
		pApp->WeaponEsp()->SetDrawGrenadeBoundingBox(pConfig->GetBool("weaponesp", "grenadeboundingbox"));
		pApp->WeaponEsp()->SetDrawBombName(pConfig->GetBool("weaponesp", "bombname"));
		pApp->WeaponEsp()->SetDrawBombBoundingBox(pConfig->GetBool("weaponesp", "bombboundingbox"));
		pApp->WeaponEsp()->SetDrawBombTimer(pConfig->GetBool("weaponesp", "bombtimer"));
		pApp->WeaponEsp()->SetDrawBombDefuseTimer(pConfig->GetBool("weaponesp", "bombdefusetimer"));
		pApp->WeaponEsp()->SetDrawBombDamageIndicator(pConfig->GetBool("weaponesp", "bombdamageindicator"));

		// SoundEsp
		pApp->SoundEsp()->SetEnabled(pConfig->GetBool("soundesp", "enabled"));
		pApp->SoundEsp()->SetShowTime(pConfig->GetFloat("soundesp", "showtime"));
		pApp->SoundEsp()->SetFadeoutEnabled(pConfig->GetBool("soundesp", "fadeout"));
		pApp->SoundEsp()->SetFadeTime(pConfig->GetFloat("soundesp", "fadeouttime"));
		pApp->SoundEsp()->SetDrawOwnTeam(pConfig->GetBool("soundesp", "team"));
		pApp->SoundEsp()->SetDrawVisible(pConfig->GetBool("soundesp", "visible"));

		// Chams
		pApp->Chams()->SetEnabled(pConfig->GetBool("chams", "enabled"));
		pApp->Chams()->SetRenderTeam(pConfig->GetBool("chams", "team"));
		pApp->Chams()->SetRenderLocalplayer(pConfig->GetBool("chams", "self"));
		pApp->Chams()->SetOnlyVisible(pConfig->GetBool("chams", "onlyvisible"));
		pApp->Chams()->SetModelStyle(pConfig->GetInt("chams", "playerstyle"));
		pApp->Chams()->SetRenderFakeAngle(pConfig->GetBool("chams", "fake"));
		pApp->Chams()->SetColorHiddenCT(pConfig->GetColor("chams", "hiddenct"));
		pApp->Chams()->SetColorVisibleCT(pConfig->GetColor("chams", "visiblect"));
		pApp->Chams()->SetColorHiddenT(pConfig->GetColor("chams", "hiddent"));
		pApp->Chams()->SetColorVisibleT(pConfig->GetColor("chams", "visiblet"));
		pApp->Chams()->SetWeaponChamsStyle(pConfig->GetInt("chams", "weaponstyle"));

		// Misc	
		pApp->Misc()->SetNoRecoil(pConfig->GetBool("misc", "norecoil"));
		pApp->Fakelag()->SetEnabled(pConfig->GetBool("misc", "fakelag"));
		pApp->Fakelag()->SetOnlyInAir(pConfig->GetBool("misc", "fakelagonlyinair"));
		pApp->Fakelag()->SetChokeAmount(pConfig->GetInt("misc", "fakelagamount"));
		pApp->Fakelag()->SetLagType(pConfig->GetInt("misc", "fakelagtype"));
		pApp->Misc()->SetAutoStrafeMode(pConfig->GetInt("misc", "autostrafe"));
		pApp->Misc()->SetCircleStrafe(pConfig->GetBool("misc", "circlestrafe"));
		pApp->Misc()->SetAutoPistol(pConfig->GetBool("misc", "autopistol"));
		pApp->Misc()->SetShowSpectators(pConfig->GetBool("misc", "spectators"));
		pApp->Misc()->SetShowOnlyMySpectators(pConfig->GetBool("misc", "onlymyspectators"));
		pApp->Misc()->SetShowOnlyMyTeamSpectators(pConfig->GetBool("misc", "onlymyteamspectators"));
		pApp->Misc()->SetJumpScout(pConfig->GetBool("misc", "jumpscout"));
		pApp->Misc()->SetNoName(pConfig->GetBool("misc", "noname"));
		pApp->Misc()->SetAutoAccept(pConfig->GetBool("misc", "autoaccept"));
		pApp->Misc()->SetSpamName(pConfig->GetBool("misc", "spamname"));

		// TODO
		//// SkinChanger
		//this->m_skinchanger.SetEnabled(true);
		//// TODO: Config und sowas
		//this->LoadSkinChangerConfig();

		// Visuals
		pApp->GunHud()->SetCrosshair(pConfig->GetBool("visuals", "crosshair"));
		pApp->GunHud()->SetCrosshairShowRecoil(pConfig->GetBool("visuals", "recoilcrosshair"));
		pApp->GunHud()->SetSpreadCone(pConfig->GetBool("visuals", "spreadcone"));
		pApp->GunHud()->SetSpreadConeShowRecoil(pConfig->GetBool("visuals", "recoilspreadcone"));
		pApp->GunHud()->SetSpreadConeColor(pConfig->GetColor("visuals", "spreadconecolor"));
		pApp->GunHud()->SetHitmarker(pConfig->GetBool("visuals", "hitmarker"));
		pApp->GunHud()->SetHitmarkerSound(pConfig->GetBool("visuals", "hitmarkersound"));
		pApp->GunHud()->SetHitmarkerHitpoint(pConfig->GetBool("visuals", "hitmarkerhitpoint"));
		pApp->Visuals()->NoSmoke(pConfig->GetBool("visuals", "nosmoke"));

		pApp->Visuals()->SetNoFlash(pConfig->GetBool("visuals", "noflash"));
		pApp->Visuals()->SetFlashPercentage(pConfig->GetFloat("visuals", "flashpercentage"));

		pApp->Visuals()->SetHandsDrawStyle(pConfig->GetInt("visuals", "handsstyle"));
		pApp->Visuals()->SetNoVisualRecoil(pConfig->GetBool("visuals", "novisualrecoil"));
		pApp->Visuals()->DisablePostProcessing(pConfig->GetBool("visuals", "disablepostprocessing"));
		pApp->Visuals()->SetNoScope(pConfig->GetBool("visuals", "noscope"));
		pApp->MaterialVisuals()->SetNightmodeValue(pConfig->GetFloat("visuals", "nightmodevalue"));
		pApp->MaterialVisuals()->SetAsuswallsValue(pConfig->GetFloat("visuals", "asuswallsvalue"));
		pApp->MaterialVisuals()->SetSkychangerValue(pConfig->GetInt("visuals", "skychangervalue"));

		pApp->Visuals()->SetThirdperson(pConfig->GetBool("visuals", "thirdperson"));
		pApp->Visuals()->SetThirdpersonDistance(pConfig->GetInt("visuals", "thirdpersondistance"));

		pApp->Visuals()->SetFovChange(pConfig->GetBool("visuals", "fovenabled"));
		pApp->Visuals()->SetFovValue(pConfig->GetInt("visuals", "fov"));
		pApp->Visuals()->SetFovChangeScoped(pConfig->GetBool("visuals", "scopedfov"));

		pApp->Visuals()->SetViewmodelFov(pConfig->GetBool("visuals", "viewmodelfovenabled"));
		pApp->Visuals()->SetViewmodelFovValue(pConfig->GetInt("visuals", "viewmodelfov"));

		pApp->Visuals()->SetBulletTracer(pConfig->GetBool("visuals", "bullettracerenabled"));
		pApp->Visuals()->SetBulletTracerSelf(pConfig->GetBool("visuals", "bullettracerself"));
		pApp->Visuals()->SetBulletTracerTeam(pConfig->GetBool("visuals", "bullettracerteam"));

		// Mirror
		pApp->Mirror()->SetEnabled(pConfig->GetBool("visuals", "mirror"));

		// Radar
		pApp->Radar()->SetEnabled(pConfig->GetBool("visuals", "radar"));
		pApp->Radar()->SetType(pConfig->GetInt("visuals", "radartype"));

		// LagCompensation
		pApp->LagCompensation()->SetRageLagCompensationEnabled(pConfig->GetBool("lagcompensation", "rageenabled"));
		pApp->LagCompensation()->SetLegitLagCompensationEnabled(pConfig->GetBool("lagcompensation", "legitenabled"));
		pApp->LagCompensation()->SetLegitLagCompensationDuration(pConfig->GetInt("lagcompensation", "legitduration"));
		pApp->LagCompensation()->SetDrawStyle(pConfig->GetInt("lagcompensation", "drawstyle"));
		pApp->LagCompensation()->SetDrawFrequency(pConfig->GetInt("lagcompensation", "drawfrequency"));
		pApp->LagCompensation()->SetDrawOnlyVisible(pConfig->GetBool("lagcompensation", "drawonlyvisible"));
	}
	
	void FeaturesToConfig(CConfig* pConfig)
	{
		CApplication* pApp = CApplication::Instance();

		// Target Selector
		pConfig->SetBool("targetselector", "multipoint", pApp->TargetSelector()->GetMultipoint());
		pConfig->SetFloat("targetselector", "multipointscale", pApp->TargetSelector()->GetMultipointScale());
		pConfig->SetInt("targetselector", "visiblemode", pApp->TargetSelector()->GetVisibleMode());
		pConfig->SetBool("targetselector", "checkhead", pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_HEAD));
		pConfig->SetBool("targetselector", "checkchest", pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_CHEST));
		pConfig->SetBool("targetselector", "checkpelvis", pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_PELVIS));
		pConfig->SetBool("targetselector", "checkrightforearm", pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_RIGHT_FOREARM));
		pConfig->SetBool("targetselector", "checkleftforearm", pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_LEFT_FOREARM));
		pConfig->SetBool("targetselector", "checkrightcalf", pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_RIGHT_CALF));
		pConfig->SetBool("targetselector", "checkleftcalf", pApp->TargetSelector()->GetCheckHitbox(TARGET_HITBOX_LEFT_CALF));

		// Ragebot
		pConfig->SetBool("ragebot", "enabled", pApp->Ragebot()->GetEnabled());
		pConfig->SetBool("ragebot", "autoshoot", pApp->Ragebot()->GetAutoshoot());
		pConfig->SetBool("ragebot", "autoscope", pApp->Ragebot()->GetAutoscope());
		pConfig->SetBool("ragebot", "silentaim", pApp->Ragebot()->GetSilentAim());
		pConfig->SetBool("ragebot", "autoreload", pApp->Ragebot()->GetAutoReload());
		pConfig->SetBool("ragebot", "autozeus", pApp->Ragebot()->GetAutoZeus());
		pConfig->SetInt("ragebot", "targetcriteria", pApp->Ragebot()->GetTargetCriteria());
		pConfig->SetBool("ragebot", "hitchanceenabled", pApp->Ragebot()->GetCalculateHitchance());
		pConfig->SetFloat("ragebot", "hitchance", pApp->Ragebot()->GetHitchance());
		// TODO: NoRecoil
		pConfig->SetBool("ragebot", "nospread", pApp->Ragebot()->GetNoSpread());
		pConfig->SetBool("ragebot", "autorevolver", pApp->Ragebot()->GetAutoRevolver());

		// Legitbot
		pConfig->SetBool("legitbot", "enabled", pApp->Legitbot()->GetEnabled());
		pConfig->SetInt("legitbot", "key", pApp->Legitbot()->GetTriggerKey());
		pConfig->SetBool("legitbot", "autoshoot", pApp->Legitbot()->GetAutoshoot());
		pConfig->SetInt("legitbot", "helpaftershots", pApp->Legitbot()->GetHelpAfterShots());
		pConfig->SetFloat("legitbot", "aimtime", pApp->Legitbot()->GetTimeToAim());
		pConfig->SetFloat("legitbot", "curve", pApp->Legitbot()->GetCurve());
		pConfig->SetFloat("legitbot", "fov", pApp->Legitbot()->GetFieldOfView());
		pConfig->SetFloat("legitbot", "pointscale", pApp->Legitbot()->GetPointScale());
		pConfig->SetBool("legitbot", "drawfov", pApp->Legitbot()->GetDrawFieldOfView());
		pConfig->SetColor("legitbot", "fovcolor", pApp->Legitbot()->GetFieldOfViewColor());
		pConfig->SetBool("legitbot", "drawpath", pApp->Legitbot()->GetDrawPath());
		pConfig->SetColor("legitbot", "pathcolor", pApp->Legitbot()->GetPathColor());
		pConfig->SetBool("legitbot", "checkhead", pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_HEAD));
		pConfig->SetBool("legitbot", "checkchest", pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_CHEST));
		pConfig->SetBool("legitbot", "checkpelvis", pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_PELVIS));
		pConfig->SetBool("legitbot", "checkrightforearm", pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_RIGHT_FOREARM));
		pConfig->SetBool("legitbot", "checkleftforearm", pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_LEFT_FOREARM));
		pConfig->SetBool("legitbot", "checkrightcalf", pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_RIGHT_CALF));
		pConfig->SetBool("legitbot", "checkleftcalf", pApp->Legitbot()->GetCheckHitbox(TARGET_HITBOX_LEFT_CALF));

		// Triggerbot
		pConfig->SetBool("triggerbot", "enabled", pApp->Triggerbot()->GetEnabled());
		pConfig->SetInt("triggerbot", "shootdelay", pApp->Triggerbot()->GetShootDelay());
		pConfig->SetInt("triggerbot", "shootdelayjitter", pApp->Triggerbot()->GetShootDelayJitter());
		
		// Antiaim
		pConfig->SetBool("antiaim", "enabled", pApp->AntiAim()->GetEnabled());
		pConfig->SetBool("antiaim", "lbyindicator", pApp->AntiAim()->GetDrawLbyIndicator());
		pConfig->SetBool("antiaim", "lbybreaker", pApp->AntiAim()->GetLbyBreaker());

		// Standing
		pConfig->SetInt("antiaim", "standingpitch", pApp->AntiAim()->GetPitchSettingStanding());
		pConfig->SetInt("antiaim", "standingyaw", pApp->AntiAim()->GetYawSettingStanding());
		pConfig->SetInt("antiaim", "standingyawoffset", pApp->AntiAim()->GetYawOffsetStanding());
		pConfig->SetInt("antiaim", "standingfakeyaw", pApp->AntiAim()->GetYawFakeSettingStanding());
		pConfig->SetInt("antiaim", "standingfakeyawoffset", pApp->AntiAim()->GetYawFakeOffsetStanding());
		// Moving
		pConfig->SetInt("antiaim", "movingpitch", pApp->AntiAim()->GetPitchSettingMoving());
		pConfig->SetInt("antiaim", "movingyaw", pApp->AntiAim()->GetYawSettingMoving());
		pConfig->SetInt("antiaim", "movingyawoffset", pApp->AntiAim()->GetYawOffsetMoving());
		pConfig->SetInt("antiaim", "movingfakeyaw", pApp->AntiAim()->GetYawFakeSettingMoving());
		pConfig->SetInt("antiaim", "movingfakeyawoffset", pApp->AntiAim()->GetYawFakeOffsetMoving());

		// Edge AA
		pConfig->SetBool("antiaim", "edgeantiaim", pApp->AntiAim()->GetDoEdgeAntiAim());
		pConfig->SetInt("antiaim", "edgeantiaimcheckpoints", pApp->AntiAim()->GetEdgeAntiAimCheckPointsAmount());
		pConfig->SetBool("antiaim", "edgeantiaimdrawpoints", pApp->AntiAim()->GetDrawEdgeAntiAimPoints());
		pConfig->SetBool("antiaim", "edgeantiaimdrawlines", pApp->AntiAim()->GetDrawEdgeAntiAimLines());

		// Resolver
		pConfig->SetBool("resolver", "enabled", pApp->Resolver()->GetEnabled());
		pConfig->SetInt("resolver", "type", pApp->Resolver()->GetResolverType());

		// Bhop
		pConfig->SetBool("bhop", "enabled", pApp->Bhop()->GetEnabled());

		// Esp
		pConfig->SetBool("esp", "enabled", pApp->Esp()->GetEnabled());
		pConfig->SetInt("esp", "boundingbox", pApp->Esp()->GetDrawBoundingBox());
		pConfig->SetBool("esp", "fillboundingbox", pApp->Esp()->GetFillBoundingBox());
		pConfig->SetBool("esp", "outlineboundingbox", pApp->Esp()->GetDrawOutline());
		pConfig->SetBool("esp", "skeleton", pApp->Esp()->GetDrawSkeleton());
		pConfig->SetBool("esp", "names", pApp->Esp()->GetDrawNames());
		pConfig->SetBool("esp", "healthbar", pApp->Esp()->GetDrawHealthBar());
		pConfig->SetBool("esp", "healthnumber", pApp->Esp()->GetDrawHealthNumber());
		pConfig->SetBool("esp", "armorbar", pApp->Esp()->GetDrawArmorBar());
		pConfig->SetBool("esp", "activeweapon", pApp->Esp()->GetDrawActiveWeapon());
		pConfig->SetBool("esp", "ammobar", pApp->Esp()->GetDrawAmmoBar());
		pConfig->SetBool("esp", "ammonumber", pApp->Esp()->GetDrawAmmoNumber());
		pConfig->SetBool("esp", "team", pApp->Esp()->GetDrawOwnTeam());
		pConfig->SetBool("esp", "self", pApp->Esp()->GetDrawOwnModel());
		pConfig->SetBool("esp", "onlyvisible", pApp->Esp()->GetDrawOnlyVisible());
		pConfig->SetBool("esp", "onlyspotted", pApp->Esp()->GetDrawOnlySpotted());
		pConfig->SetBool("esp", "viewangles", pApp->Esp()->GetDrawViewangles());
		pConfig->SetInt("esp", "viewanglelength", pApp->Esp()->GetViewanglesLength());
		pConfig->SetBool("esp", "fadeout", pApp->Esp()->GetFadeoutEnabled());
		pConfig->SetFloat("esp", "fadeouttime", pApp->Esp()->GetFadeoutTime());
		pConfig->SetColor("esp", "colorct", pApp->Esp()->GetColorCT());
		pConfig->SetColor("esp", "colort", pApp->Esp()->GetColorT());
		pConfig->SetColor("esp", "colorspotted", pApp->Esp()->GetColorSpotted());

		// WeaponEsp
		pConfig->SetBool("weaponesp", "enabled", pApp->WeaponEsp()->GetEnabled());
		pConfig->SetBool("weaponesp", "weaponname", pApp->WeaponEsp()->GetDrawWeaponName());
		pConfig->SetBool("weaponesp", "weaponboundingbox", pApp->WeaponEsp()->GetDrawWeaponBoundingBox());
		pConfig->SetBool("weaponesp", "grenadename", pApp->WeaponEsp()->GetDrawGrenadeName());
		pConfig->SetBool("weaponesp", "grenadeboundingbox", pApp->WeaponEsp()->GetDrawGrenadeBoundingBox());
		pConfig->SetBool("weaponesp", "bombname", pApp->WeaponEsp()->GetDrawBombName());
		pConfig->SetBool("weaponesp", "bombboundingbox", pApp->WeaponEsp()->GetDrawBombBoundingBox());
		pConfig->SetBool("weaponesp", "bombtimer", pApp->WeaponEsp()->GetDrawBombTimer());
		pConfig->SetBool("weaponesp", "bombdefusetimer", pApp->WeaponEsp()->GetDrawBombDefuseTimer());
		pConfig->SetBool("weaponesp", "bombdamageindicator", pApp->WeaponEsp()->GetDrawBombDamageIndicator());

		// SoundEsp
		pConfig->SetBool("soundesp", "enabled", pApp->SoundEsp()->GetEnabled());
		pConfig->SetFloat("soundesp", "showtime", pApp->SoundEsp()->GetShowTime());
		pConfig->SetBool("soundesp", "fadeout", pApp->SoundEsp()->GetFadeoutEnabled());
		pConfig->SetFloat("soundesp", "fadeouttime", pApp->SoundEsp()->GetFadeTime());
		pConfig->SetBool("soundesp", "team", pApp->SoundEsp()->GetDrawOwnTeam());
		pConfig->SetBool("soundesp", "visible", pApp->SoundEsp()->GetDrawVisible());

		// Chams
		pConfig->SetBool("chams", "enabled", pApp->Chams()->GetEnabled());
		pConfig->SetBool("chams", "team", pApp->Chams()->GetRenderTeam());
		pConfig->SetBool("chams", "self", pApp->Chams()->GetRenderLocalplayer());
		pConfig->SetBool("chams", "onlyvisible", pApp->Chams()->GetOnlyVisible());
		pConfig->SetInt("chams", "playerstyle", pApp->Chams()->GetModelStyle());
		pConfig->SetBool("chams", "fake", pApp->Chams()->GetRenderFakeAngle());
		pConfig->SetColor("chams", "hiddenct", pApp->Chams()->GetColorHiddenCT());
		pConfig->SetColor("chams", "visiblect", pApp->Chams()->GetColorVisibleCT());
		pConfig->SetColor("chams", "hiddent", pApp->Chams()->GetColorHiddenT());
		pConfig->SetColor("chams", "visiblet", pApp->Chams()->GetColorVisibleT());
		pConfig->SetInt("chams", "weaponstyle", pApp->Chams()->GetWeaponChamsStyle());

		// Misc
		pConfig->SetBool("misc", "norecoil", pApp->Misc()->GetNoRecoil());
		pConfig->SetBool("misc", "fakelag", pApp->Fakelag()->GetEnabled());
		pConfig->SetBool("misc", "fakelagonlyinair", pApp->Fakelag()->GetOnlyInAir());
		pConfig->SetInt("misc", "fakelagamount", pApp->Fakelag()->GetChokeAmount());
		pConfig->SetInt("misc", "fakelagtype", pApp->Fakelag()->GetLagType());
		pConfig->SetInt("misc", "autostrafe", pApp->Misc()->GetAutoStrafeMode());
		pConfig->SetBool("misc", "circlestrafe", pApp->Misc()->GetCircleStrafe());
		// TODO: CircleStrafeDirection
		pConfig->SetBool("misc", "autopistol", pApp->Misc()->GetAutoPistol());
		pConfig->SetBool("misc", "spectators", pApp->Misc()->GetShowSpectators());
		pConfig->SetBool("misc", "onlymyspectators", pApp->Misc()->GetShowOnlyMySpectators());
		pConfig->SetBool("misc", "onlymyteamspectators", pApp->Misc()->GetShowOnlyMyTeamSpectators());
		pConfig->SetBool("misc", "jumpscout", pApp->Misc()->GetJumpScout());
		pConfig->SetBool("misc", "noname", pApp->Misc()->GetNoName());
		pConfig->SetBool("misc", "autoaccept", pApp->Misc()->GetAutoAccept());
		pConfig->SetBool("misc", "spamname", pApp->Misc()->GetSpamName());

		// Visuals
		pConfig->SetBool("visuals", "crosshair", pApp->GunHud()->GetCrosshair());
		pConfig->SetBool("visuals", "recoilcrosshair", pApp->GunHud()->GetCrosshairShowRecoil());
		pConfig->SetBool("visuals", "spreadcone", pApp->GunHud()->GetSpreadCone());
		pConfig->SetBool("visuals", "recoilspreadcone", pApp->GunHud()->GetSpreadConeShowRecoil());
		pConfig->SetColor("visuals", "spreadconecolor", pApp->GunHud()->GetSpreadConeColor());
		pConfig->SetBool("visuals", "hitmarker", pApp->GunHud()->GetHitmarker());
		pConfig->SetBool("visuals", "hitmarkersound", pApp->GunHud()->GetHitmarkerSound());
		pConfig->SetBool("visuals", "hitmarkerhitpoint", pApp->GunHud()->GetHitmarkerHitpoint());

		pConfig->SetBool("visuals", "nosmoke", pApp->Visuals()->GetNoSmoke());
		pConfig->SetBool("visuals", "noflash", pApp->Visuals()->GetNoFlash());
		pConfig->SetFloat("visuals", "flashpercentage", pApp->Visuals()->GetFlashPercentage());

		pConfig->SetInt("visuals", "handsstyle", pApp->Visuals()->GetHandsDrawStyle());
		pConfig->SetBool("visuals", "novisualrecoil", pApp->Visuals()->GetNoVisualRecoil());
		pConfig->SetBool("visuals", "disablepostprocessing", pApp->Visuals()->GetDisablePostProcessing());
		pConfig->SetBool("visuals", "noscope", pApp->Visuals()->GetNoScope());
		pConfig->SetFloat("visuals", "nightmodevalue", pApp->MaterialVisuals()->GetNightmodeValue());
		pConfig->SetFloat("visuals", "asuswallsvalue", pApp->MaterialVisuals()->GetAsuswallsValue());
		pConfig->SetInt("visuals", "skychangervalue", pApp->MaterialVisuals()->GetSkychangerValue());

		pConfig->SetBool("visuals", "thirdperson", pApp->Visuals()->GetThirdperson());
		pConfig->SetInt("visuals", "thirdpersondistance", pApp->Visuals()->GetThirdpersonDistance());

		pConfig->SetBool("visuals", "fovenabled", pApp->Visuals()->GetFovChange());
		pConfig->SetInt("visuals", "fov", pApp->Visuals()->GetFovValue());
		pConfig->SetBool("visuals", "scopedfov", pApp->Visuals()->GetFovChangeScoped());

		pConfig->SetBool("visuals", "viewmodelfovenabled", pApp->Visuals()->GetViewmodelFov());
		pConfig->SetInt("visuals", "viewmodelfov", pApp->Visuals()->GetViewmodelFovValue());

		pConfig->SetBool("visuals", "bullettracerenabled", pApp->Visuals()->GetBulletTracer());
		pConfig->SetBool("visuals", "bullettracerself", pApp->Visuals()->GetBulletTracerSelf());
		pConfig->SetBool("visuals", "bullettracerteam", pApp->Visuals()->GetBulletTracerTeam());

		// Mirror
		pConfig->SetBool("visuals", "mirror", pApp->Mirror()->GetEnabled());

		// Radar
		pConfig->SetBool("visuals", "radar", pApp->Radar()->GetEnabled());
		pConfig->SetInt("visuals", "radartype", pApp->Radar()->GetType());

		// LagCompensation
		pConfig->SetBool("lagcompensation", "rageenabled", pApp->LagCompensation()->GetRageLagCompensationEnabled());
		pConfig->SetBool("lagcompensation", "legitenabled", pApp->LagCompensation()->GetLegitLagCompensationEnabled());
		pConfig->SetInt("lagcompensation", "legitduration", pApp->LagCompensation()->GetLegitLagCompensationDuration());
		pConfig->SetInt("lagcompensation", "drawstyle", pApp->LagCompensation()->GetDrawStyle());
		pConfig->SetInt("lagcompensation", "drawfrequency", pApp->LagCompensation()->GetDrawFrequency());
		pConfig->SetInt("lagcompensation", "drawonlyvisible", pApp->LagCompensation()->GetDrawOnlyVisible());
	}
}