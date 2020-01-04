#include <iomanip>
#include <ctime>
#include "Menu-Config.h"
#include "../../includes.h"
#include <Windows.h>
#include <io.h>
#include <ShlObj.h>
/*
Credits: A5
Side Notes: Baby you're cute for letting me use your config shit <3
*/

void CConfig::Setup()
{
	//legitbot
	SetupValue(Config::Legitbot::LegitGlobal::get().backtrack_enable, false, "Legit_Global", "Legit_Global_backtrack_enable");
	SetupValue(Config::Legitbot::LegitGlobal::get().backtrack_method, 0, "Legit_Global", "Legit_Global_backtrack_method");
	SetupValue(Config::Legitbot::LegitGlobal::get().backtrack_ticks, 0, "Legit_Global", "Legit_Global_backtrack_ticks");
	SetupValue(Config::Legitbot::LegitGlobal::get().enable, false, "Legit_Global", "Legit_Global_enable");
	SetupValue(Config::Legitbot::LegitGlobal::get().enemyscan, 0, "Legit_Global", "Legit_Global_enemyscan");
	SetupValue(Config::Legitbot::LegitGlobal::get().extend_time, 0, "Legit_Global", "Legit_Global_extend_time");
	SetupValue(Config::Legitbot::LegitGlobal::get().flash_check, false, "Legit_Global", "Legit_Global_flash_check");
	SetupValue(Config::Legitbot::LegitGlobal::get().legitbot_key, 0, "Legit_Global", "Legit_Global_flash_check");
	SetupValue(Config::Legitbot::LegitGlobal::get().legit_aa, false, "Legit_Global", "Legit_Global_legit_aa");
	SetupValue(Config::Legitbot::LegitGlobal::get().show_legit_aa, false, "Legit_Global", "Legit_Global_show_legit_aa");
	SetupValue(Config::Legitbot::LegitGlobal::get().show_legit_aa_r, 0, "Legit_Global", "Legit_Global_show_legit_aa_r");
	SetupValue(Config::Legitbot::LegitGlobal::get().show_legit_aa_g, 0, "Legit_Global", "Legit_Global_show_legit_aa_g");
	SetupValue(Config::Legitbot::LegitGlobal::get().show_legit_aa_b, 0, "Legit_Global", "Legit_Global_show_legit_aa_b");
	SetupValue(Config::Legitbot::LegitGlobal::get().show_legit_aa_a, 0, "Legit_Global", "Legit_Global_show_legit_aa_a");
	SetupValue(Config::Legitbot::LegitGlobal::get().rcs_enable, false, "Legit_Global", "Legit_Global_rcs_enable");
	SetupValue(Config::Legitbot::LegitGlobal::get().rcs_x, 0, "Legit_Global", "Legit_Global_rcs_x");
	SetupValue(Config::Legitbot::LegitGlobal::get().rcs_y, 0, "Legit_Global", "Legit_Global_rcs_y");
	SetupValue(Config::Legitbot::LegitGlobal::get().smoke_check, false, "Legit_Global", "Legit_Global_smoke_check");
	SetupValue(Config::Legitbot::LegitGlobal::get().trigerbot, false, "Legit_Global", "Legit_Global_trigerbot");
	SetupValue(Config::Legitbot::LegitGlobal::get().trigerbot_key, 0, "Legit_Global", "Legit_Global_trigerbot_key");
	SetupValue(Config::Legitbot::LegitGlobal::get().triggerbot_hitchance, 0, "Legit_Global", "Legit_Global_triggerbot_hitchance");
	SetupValue(Config::Legitbot::LegitGlobal::get().wall_check, false, "Legit_Global", "Legit_Global_wall_check");


	SetupValue(Config::Legitbot::LegitPistol::get().hitboxes, 5,false, "Legit_Pistol", "Legit_Pistol_hitboxes");
	SetupValue(Config::Legitbot::LegitPistol::get().silent, false, "Legit_Pistol", "Legit_Pistol_silent");
	SetupValue(Config::Legitbot::LegitPistol::get().smooth, 0, "Legit_Pistol", "Legit_Pistol_smooth");
	SetupValue(Config::Legitbot::LegitPistol::get().fov, 0, "Legit_Pistol", "Legit_Pistol_fov");
	SetupValue(Config::Legitbot::LegitPistol::get().delay, 0, "Legit_Pistol", "Legit_Pistol_delay");
	SetupValue(Config::Legitbot::LegitPistol::get().enable, false, "Legit_Pistol", "Legit_Pistol_enable");
	SetupValue(Config::Legitbot::LegitPistol::get().triggerbot_hitchance, 0, "Legit_Pistol", "Legit_Pistol_triggerbot_hitchance");
	SetupValue(Config::Legitbot::LegitPistol::get().rcs_enable, false, "Legit_Pistol", "Legit_Pistol_rcs_enable");
	SetupValue(Config::Legitbot::LegitPistol::get().rcs_x, 0, "Legit_Pistol", "Legit_Pistol_rcs_x");
	SetupValue(Config::Legitbot::LegitPistol::get().rcs_y, 0, "Legit_Pistol", "Legit_Pistol_rcs_y");

	SetupValue(Config::Legitbot::LegitHeavyPistol::get().hitboxes, false, 5, "Legit_HeavyPistol", "Legit_HeavyPistol_hitboxes");
	SetupValue(Config::Legitbot::LegitHeavyPistol::get().silent, false, "Legit_HeavyPistol", "Legit_HeavyPistol_silent");
	SetupValue(Config::Legitbot::LegitHeavyPistol::get().smooth, 0, "Legit_HeavyPistol", "Legit_HeavyPistol_smooth");
	SetupValue(Config::Legitbot::LegitHeavyPistol::get().fov, 0, "Legit_HeavyPistol", "Legit_HeavyPistol_fov");
	SetupValue(Config::Legitbot::LegitHeavyPistol::get().delay, 0, "Legit_HeavyPistol", "Legit_HeavyPistol_delay");
	SetupValue(Config::Legitbot::LegitHeavyPistol::get().enable, false, "Legit_HeavyPistol", "Legit_HeavyPistol_enable");
	SetupValue(Config::Legitbot::LegitHeavyPistol::get().triggerbot_hitchance, 0, "Legit_HeavyPistol", "Legit_HeavyPistol_triggerbot_hitchance");
	SetupValue(Config::Legitbot::LegitHeavyPistol::get().rcs_enable, false, "Legit_HeavyPistol", "Legit_HeavyPistol_rcs_enable");
	SetupValue(Config::Legitbot::LegitHeavyPistol::get().rcs_x, 0, "Legit_HeavyPistol", "Legit_HeavyPistol_rcs_x");
	SetupValue(Config::Legitbot::LegitHeavyPistol::get().rcs_y, 0, "Legit_HeavyPistol", "Legit_HeavyPistol_rcs_y");

	SetupValue(Config::Legitbot::LegitSMG::get().hitboxes, false, 5, "Legit_SMG", "Legit_SMG_hitboxes");
	SetupValue(Config::Legitbot::LegitSMG::get().silent, false, "Legit_SMG", "Legit_SMG_silent");
	SetupValue(Config::Legitbot::LegitSMG::get().smooth, 0, "Legit_SMG", "Legit_SMG_smooth");
	SetupValue(Config::Legitbot::LegitSMG::get().fov, 0, "Legit_SMG", "Legit_SMG_fov");
	SetupValue(Config::Legitbot::LegitSMG::get().delay, 0, "Legit_SMG", "Legit_SMG_delay");
	SetupValue(Config::Legitbot::LegitSMG::get().enable, false, "Legit_SMG", "Legit_SMG_enable");
	SetupValue(Config::Legitbot::LegitSMG::get().triggerbot_hitchance, 0, "Legit_SMG", "Legit_SMG_triggerbot_hitchance");
	SetupValue(Config::Legitbot::LegitSMG::get().rcs_enable, false, "Legit_SMG", "Legit_SMG_rcs_enable");
	SetupValue(Config::Legitbot::LegitSMG::get().rcs_x, 0, "Legit_SMG", "Legit_SMG_rcs_x");
	SetupValue(Config::Legitbot::LegitSMG::get().rcs_y, 0, "Legit_SMG", "Legit_SMG_rcs_y");

	SetupValue(Config::Legitbot::LegitAR::get().hitboxes, false, 5, "Legit_AR", "Legit_AR_hitboxes");
	SetupValue(Config::Legitbot::LegitAR::get().silent, false, "Legit_AR", "Legit_AR_silent");
	SetupValue(Config::Legitbot::LegitAR::get().smooth, 0, "Legit_AR", "Legit_AR_smooth");
	SetupValue(Config::Legitbot::LegitAR::get().fov, 0, "Legit_AR", "Legit_AR_fov");
	SetupValue(Config::Legitbot::LegitAR::get().delay, 0, "Legit_AR", "Legit_AR_delay");
	SetupValue(Config::Legitbot::LegitAR::get().enable, false, "Legit_AR", "Legit_AR_enable");
	SetupValue(Config::Legitbot::LegitAR::get().triggerbot_hitchance, 0, "Legit_AR", "Legit_AR_triggerbot_hitchance");
	SetupValue(Config::Legitbot::LegitAR::get().rcs_enable, false, "Legit_AR", "Legit_AR_rcs_enable");
	SetupValue(Config::Legitbot::LegitAR::get().rcs_x, 0, "Legit_AR", "Legit_AR_rcs_x");
	SetupValue(Config::Legitbot::LegitAR::get().rcs_y, 0, "Legit_AR", "Legit_AR_rcs_y");

	SetupValue(Config::Legitbot::LegitAuto::get().hitboxes, false, 5, "Legit_Auto", "Legit_Auto_hitboxes");
	SetupValue(Config::Legitbot::LegitAuto::get().silent, false, "Legit_Auto", "Legit_Auto_silent");
	SetupValue(Config::Legitbot::LegitAuto::get().smooth, 0, "Legit_Auto", "Legit_Auto_smooth");
	SetupValue(Config::Legitbot::LegitAuto::get().fov, 0, "Legit_Auto", "Legit_Auto_fov");
	SetupValue(Config::Legitbot::LegitAuto::get().delay, 0, "Legit_Auto", "Legit_Auto_delay");
	SetupValue(Config::Legitbot::LegitAuto::get().enable, false, "Legit_Auto", "Legit_Auto_enable");
	SetupValue(Config::Legitbot::LegitAuto::get().triggerbot_hitchance, 0, "Legit_Auto", "Legit_Auto_triggerbot_hitchance");
	SetupValue(Config::Legitbot::LegitAuto::get().rcs_enable, false, "Legit_Auto", "Legit_Auto_rcs_enable");
	SetupValue(Config::Legitbot::LegitAuto::get().rcs_x, 0, "Legit_Auto", "Legit_Auto_rcs_x");
	SetupValue(Config::Legitbot::LegitAuto::get().rcs_y, 0, "Legit_Auto", "Legit_Auto_rcs_y");

	SetupValue(Config::Legitbot::LegitScout::get().hitboxes, false, 5, "Legit_Scout", "Legit_Scout_hitboxes");
	SetupValue(Config::Legitbot::LegitScout::get().silent, false, "Legit_Scout", "Legit_Scout_silent");
	SetupValue(Config::Legitbot::LegitScout::get().smooth, 0, "Legit_Scout", "Legit_Scout_smooth");
	SetupValue(Config::Legitbot::LegitScout::get().fov, 0, "Legit_Scout", "Legit_Scout_fov");
	SetupValue(Config::Legitbot::LegitScout::get().delay, 0, "Legit_Scout", "Legit_Scout_delay");
	SetupValue(Config::Legitbot::LegitScout::get().enable, false, "Legit_Scout", "Legit_Scout_enable");
	SetupValue(Config::Legitbot::LegitScout::get().triggerbot_hitchance, 0, "Legit_Scout", "Legit_Scout_triggerbot_hitchance");
	SetupValue(Config::Legitbot::LegitScout::get().rcs_enable, false, "Legit_Scout", "Legit_Scout_rcs_enable");
	SetupValue(Config::Legitbot::LegitScout::get().rcs_x, 0, "Legit_Scout", "Legit_Scout_rcs_x");
	SetupValue(Config::Legitbot::LegitScout::get().rcs_y, 0, "Legit_Scout", "Legit_Scout_rcs_y");

	SetupValue(Config::Legitbot::LegitAWP::get().hitboxes, false, 5, "Legit_AWP", "Legit_AWP_hitboxes");
	SetupValue(Config::Legitbot::LegitAWP::get().silent, false, "Legit_AWP", "Legit_AWP_silent");
	SetupValue(Config::Legitbot::LegitAWP::get().smooth, 0, "Legit_AWP", "Legit_AWP_smooth");
	SetupValue(Config::Legitbot::LegitAWP::get().fov, 0, "Legit_AWP", "Legit_AWP_fov");
	SetupValue(Config::Legitbot::LegitAWP::get().delay, 0, "Legit_AWP", "Legit_AWP_delay");
	SetupValue(Config::Legitbot::LegitAWP::get().enable, false, "Legit_AWP", "Legit_AWP_enable");
	SetupValue(Config::Legitbot::LegitAWP::get().triggerbot_hitchance, 0, "Legit_AWP", "Legit_AWP_triggerbot_hitchance");
	SetupValue(Config::Legitbot::LegitAWP::get().rcs_enable, false, "Legit_AWP", "Legit_AWP_rcs_enable");
	SetupValue(Config::Legitbot::LegitAWP::get().rcs_x, 0, "Legit_AWP", "Legit_AWP_rcs_x");
	SetupValue(Config::Legitbot::LegitAWP::get().rcs_y, 0, "Legit_AWP", "Legit_AWP_rcs_y");

	SetupValue(Config::Legitbot::LegitShotGuns::get().hitboxes, false, 5, "Legit_ShotGuns", "Legit_ShotGuns_hitboxes");
	SetupValue(Config::Legitbot::LegitShotGuns::get().silent, false, "Legit_ShotGuns", "Legit_ShotGuns_silent");
	SetupValue(Config::Legitbot::LegitShotGuns::get().smooth, 0, "Legit_ShotGuns", "Legit_ShotGuns_smooth");
	SetupValue(Config::Legitbot::LegitShotGuns::get().fov, 0, "Legit_ShotGuns", "Legit_ShotGuns_fov");
	SetupValue(Config::Legitbot::LegitShotGuns::get().delay, 0, "Legit_ShotGuns", "Legit_ShotGuns_delay");
	SetupValue(Config::Legitbot::LegitShotGuns::get().enable, false, "Legit_ShotGuns", "Legit_ShotGuns_enable");
	SetupValue(Config::Legitbot::LegitShotGuns::get().triggerbot_hitchance, 0, "Legit_ShotGuns", "Legit_ShotGuns_triggerbot_hitchance");
	SetupValue(Config::Legitbot::LegitShotGuns::get().rcs_enable, false, "Legit_ShotGuns", "Legit_ShotGuns_rcs_enable");
	SetupValue(Config::Legitbot::LegitShotGuns::get().rcs_x, 0, "Legit_ShotGuns", "Legit_ShotGuns_rcs_x");
	SetupValue(Config::Legitbot::LegitShotGuns::get().rcs_y, 0, "Legit_ShotGuns", "Legit_ShotGuns_rcs_y");

	SetupValue(Config::Legitbot::LegitLMG::get().hitboxes, false, 5, "Legit_LMG", "Legit_LMG_hitboxes");
	SetupValue(Config::Legitbot::LegitLMG::get().silent, false, "Legit_LMG", "Legit_LMG_silent");
	SetupValue(Config::Legitbot::LegitLMG::get().smooth, 0, "Legit_LMG", "Legit_LMG_smooth");
	SetupValue(Config::Legitbot::LegitLMG::get().fov, 0, "Legit_LMG", "Legit_LMG_fov");
	SetupValue(Config::Legitbot::LegitLMG::get().delay, 0, "Legit_LMG", "Legit_LMG_delay");
	SetupValue(Config::Legitbot::LegitLMG::get().enable, false, "Legit_LMG", "Legit_LMG_enable");
	SetupValue(Config::Legitbot::LegitLMG::get().triggerbot_hitchance, 0, "Legit_LMG", "Legit_LMG_triggerbot_hitchance");
	SetupValue(Config::Legitbot::LegitLMG::get().rcs_enable, false, "Legit_LMG", "Legit_LMG_rcs_enable");
	SetupValue(Config::Legitbot::LegitLMG::get().rcs_x, 0, "Legit_LMG", "Legit_LMG_rcs_x");
	SetupValue(Config::Legitbot::LegitLMG::get().rcs_y, 0, "Legit_LMG", "Legit_LMG_rcs_y");
	//rage
	SetupValue(Config::Ragebot::RageGlobal::get().enable, false, "Rage_Global", "Rage_Global_Enable");
	SetupValue(Config::Ragebot::RageGlobal::get().low_fps, false, "Rage_Global", "Rage_Global_low_fps");
	SetupValue(Config::Ragebot::RageGlobal::get().acc_bost, 0, "Rage_Global", "Rage_Global_acc_bost");
	SetupValue(Config::Ragebot::RageGlobal::get().autoshoot, false, "Rage_Global", "Rage_Global_autoshoot");
	SetupValue(Config::Ragebot::RageGlobal::get().auto_scope, false, "Rage_Global", "Rage_Global_auto_scope");
	SetupValue(Config::Ragebot::RageGlobal::get().autowall, false, "Rage_Global", "Rage_Global_auto_autowall");
	SetupValue(Config::Ragebot::RageGlobal::get().backtrack_enable, false, "Rage_Global", "Rage_Global_backtrack_enable");
	SetupValue(Config::Ragebot::RageGlobal::get().backtrack_method, 0, "Rage_Global", "Rage_Global_backtrack_method");
	SetupValue(Config::Ragebot::RageGlobal::get().backtrack_ticks, 0, "Rage_Global", "Rage_Global_backtrack_ticks");
	SetupValue(Config::Ragebot::RageGlobal::get().bodyaim, 0, "Rage_Global", "Rage_Global_bodyaim");
	SetupValue(Config::Ragebot::RageGlobal::get().dubletap, 0, "Rage_Global", "Rage_Global_dubletap");
	SetupValue(Config::Ragebot::RageGlobal::get().dubletap_key, 0, "Rage_Global", "Rage_Global_dubletap_key");
	SetupValue(Config::Ragebot::RageGlobal::get().enemyscan, 0, "Rage_Global", "Rage_Global_enemyscan");
	SetupValue(Config::Ragebot::RageGlobal::get().extend_time, 0, "Rage_Global", "Rage_Global_extend_time");
	SetupValue(Config::Ragebot::RageGlobal::get().extened_backtrack_ticks, false, "Rage_Global", "Rage_Global_extened_backtrack_ticks");
	SetupValue(Config::Ragebot::RageGlobal::get().fakelag_cor, 0, "Rage_Global", "Rage_Global_fakelag_cor");
	SetupValue(Config::Ragebot::RageGlobal::get().max_fov, 0, "Rage_Global", "Rage_Global_max_fov");
	SetupValue(Config::Ragebot::RageGlobal::get().remove_recoil, false, "Rage_Global", "Rage_Global_remove_recoil");
	SetupValue(Config::Ragebot::RageGlobal::get().remove_spread, false, "Rage_Global", "Rage_Global_remove_spread");
	SetupValue(Config::Ragebot::RageGlobal::get().resolver, false, "Rage_Global", "Rage_Global_resolver");
	SetupValue(Config::Ragebot::RageGlobal::get().silent, false, "Rage_Global", "Rage_Global_silent");

	SetupValue(Config::Ragebot::RagePistol::get().auto_stop_method, 0, "Rage_Pistol", "Rage_Pistol_autostop");
	SetupValue(Config::Ragebot::RagePistol::get().enable, false, "Rage_Pistol", "Rage_Pistol_enable");
	SetupValue(Config::Ragebot::RagePistol::get().hitboxes, false, 5, "Rage_Pistol", "Rage_Pistol_hitboxes");
	SetupValue(Config::Ragebot::RagePistol::get().hitchance, 0, "Rage_Pistol", "Rage_Pistol_hitchance");
	SetupValue(Config::Ragebot::RagePistol::get().min_dmg, 0, "Rage_Pistol", "Rage_Pistol_min_dmg");
	SetupValue(Config::Ragebot::RagePistol::get().min_dmg_autowall, 0, "Rage_Pistol", "Rage_Pistol_min_dmg_autowall");
	SetupValue(Config::Ragebot::RagePistol::get().multipoint_arms, 0, "Rage_Pistol", "Rage_Pistol_multipoint_arms");
	SetupValue(Config::Ragebot::RagePistol::get().multipoint_body, 0, "Rage_Pistol", "Rage_Pistol_multipoint_body");
	SetupValue(Config::Ragebot::RagePistol::get().multipoint_head, 0, "Rage_Pistol", "Rage_Pistol_multipoint_head");
	SetupValue(Config::Ragebot::RagePistol::get().multipoint_legs, 0, "Rage_Pistol", "Rage_Pistol_multipoint_legs");
	SetupValue(Config::Ragebot::RagePistol::get().multipoint_neck, 0, "Rage_Pistol", "Rage_Pistol_multipoint_neck");
	SetupValue(Config::Ragebot::RagePistol::get().prefer_hitbox, 0, "Rage_Pistol", "Rage_Pistol_prefer_hitbox");
	SetupValue(Config::Ragebot::RagePistol::get().prefer, 0, "Rage_Pistol", "Rage_Pistol_prefer");

	SetupValue(Config::Ragebot::RageHeavyPistol::get().auto_stop_method, 0, "Rage_HeavyPistol", "Rage_HeavyPistol_autostop");
	SetupValue(Config::Ragebot::RageHeavyPistol::get().enable, false, "Rage_HeavyPistol", "Rage_HeavyPistol_enable");
	SetupValue(Config::Ragebot::RageHeavyPistol::get().hitboxes, false, 5, "Rage_HeavyPistol", "Rage_HeavyPistol_hitboxes");
	SetupValue(Config::Ragebot::RageHeavyPistol::get().hitchance, 0, "Rage_HeavyPistol", "Rage_HeavyPistol_hitchance");
	SetupValue(Config::Ragebot::RageHeavyPistol::get().min_dmg, 0, "Rage_HeavyPistol", "Rage_HeavyPistol_min_dmg");
	SetupValue(Config::Ragebot::RageHeavyPistol::get().min_dmg_autowall, 0, "Rage_HeavyPistol", "Rage_HeavyPistol_min_dmg_autowall");
	SetupValue(Config::Ragebot::RageHeavyPistol::get().multipoint_arms, 0, "Rage_HeavyPistol", "Rage_HeavyPistol_multipoint_arms");
	SetupValue(Config::Ragebot::RageHeavyPistol::get().multipoint_body, 0, "Rage_HeavyPistol", "Rage_HeavyPistol_multipoint_body");
	SetupValue(Config::Ragebot::RageHeavyPistol::get().multipoint_head, 0, "Rage_HeavyPistol", "Rage_HeavyPistol_multipoint_head");
	SetupValue(Config::Ragebot::RageHeavyPistol::get().multipoint_legs, 0, "Rage_HeavyPistol", "Rage_HeavyPistol_multipoint_legs");
	SetupValue(Config::Ragebot::RageHeavyPistol::get().multipoint_neck, 0, "Rage_HeavyPistol", "Rage_HeavyPistol_multipoint_neck");
	SetupValue(Config::Ragebot::RageHeavyPistol::get().prefer_hitbox, 0, "Rage_HeavyPistol", "Rage_HeavyPistol_prefer_hitbox");
	SetupValue(Config::Ragebot::RageHeavyPistol::get().prefer, 0, "Rage_HeavyPistol", "Rage_HeavyPistol_prefer");

	SetupValue(Config::Ragebot::RageSMG::get().auto_stop_method, 0, "Rage_SMG", "Rage_SMG_autostop");
	SetupValue(Config::Ragebot::RageSMG::get().enable, false, "Rage_SMG", "Rage_SMG_enable");
	SetupValue(Config::Ragebot::RageSMG::get().hitboxes, false, 5, "Rage_SMG", "Rage_SMG_hitboxes");
	SetupValue(Config::Ragebot::RageSMG::get().hitchance, 0, "Rage_SMG", "Rage_SMG_hitchance");
	SetupValue(Config::Ragebot::RageSMG::get().min_dmg, 0, "Rage_SMG", "Rage_SMG_min_dmg");
	SetupValue(Config::Ragebot::RageSMG::get().min_dmg_autowall, 0, "Rage_SMG", "Rage_SMG_min_dmg_autowall");
	SetupValue(Config::Ragebot::RageSMG::get().multipoint_arms, 0, "Rage_SMG", "Rage_SMG_multipoint_arms");
	SetupValue(Config::Ragebot::RageSMG::get().multipoint_body, 0, "Rage_SMG", "Rage_SMG_multipoint_body");
	SetupValue(Config::Ragebot::RageSMG::get().multipoint_head, 0, "Rage_SMG", "Rage_SMG_multipoint_head");
	SetupValue(Config::Ragebot::RageSMG::get().multipoint_legs, 0, "Rage_SMG", "Rage_SMG_multipoint_legs");
	SetupValue(Config::Ragebot::RageSMG::get().multipoint_neck, 0, "Rage_SMG", "Rage_SMG_multipoint_neck");
	SetupValue(Config::Ragebot::RageSMG::get().prefer_hitbox, 0, "Rage_SMG", "Rage_SMG_prefer_hitbox");
	SetupValue(Config::Ragebot::RageSMG::get().prefer, 0, "Rage_SMG", "Rage_SMG_prefer");

	SetupValue(Config::Ragebot::RageAR::get().auto_stop_method, 0, "Rage_AR", "Rage_AR_autostop");
	SetupValue(Config::Ragebot::RageAR::get().enable, false, "Rage_AR", "Rage_AR_enable");
	SetupValue(Config::Ragebot::RageAR::get().hitboxes, false, 5, "Rage_AR", "Rage_AR_hitboxes");
	SetupValue(Config::Ragebot::RageAR::get().hitchance, 0, "Rage_AR", "Rage_AR_hitchance");
	SetupValue(Config::Ragebot::RageAR::get().min_dmg, 0, "Rage_AR", "Rage_AR_min_dmg");
	SetupValue(Config::Ragebot::RageAR::get().min_dmg_autowall, 0, "Rage_AR", "Rage_AR_min_dmg_autowall");
	SetupValue(Config::Ragebot::RageAR::get().multipoint_arms, 0, "Rage_AR", "Rage_AR_multipoint_arms");
	SetupValue(Config::Ragebot::RageAR::get().multipoint_body, 0, "Rage_AR", "Rage_AR_multipoint_body");
	SetupValue(Config::Ragebot::RageAR::get().multipoint_head, 0, "Rage_AR", "Rage_AR_multipoint_head");
	SetupValue(Config::Ragebot::RageAR::get().multipoint_legs, 0, "Rage_AR", "Rage_AR_multipoint_legs");
	SetupValue(Config::Ragebot::RageAR::get().multipoint_neck, 0, "Rage_AR", "Rage_AR_multipoint_neck");
	SetupValue(Config::Ragebot::RageAR::get().prefer_hitbox, 0, "Rage_AR", "Rage_AR_prefer_hitbox");
	SetupValue(Config::Ragebot::RageAR::get().prefer, 0, "Rage_AR", "Rage_AR_prefer");

	SetupValue(Config::Ragebot::RageAuto::get().auto_stop_method, 0, "Rage_Auto", "Rage_Auto_autostop");
	SetupValue(Config::Ragebot::RageAuto::get().enable, false, "Rage_Auto", "Rage_Auto_enable");
	SetupValue(Config::Ragebot::RageAuto::get().hitboxes, false, 5, "Rage_Auto", "Rage_Auto_hitboxes");
	SetupValue(Config::Ragebot::RageAuto::get().hitchance, 0, "Rage_Auto", "Rage_Auto_hitchance");
	SetupValue(Config::Ragebot::RageAuto::get().min_dmg, 0, "Rage_Auto", "Rage_Auto_min_dmg");
	SetupValue(Config::Ragebot::RageAuto::get().min_dmg_autowall, 0, "Rage_Auto", "Rage_Auto_min_dmg_autowall");
	SetupValue(Config::Ragebot::RageAuto::get().multipoint_arms, 0, "Rage_Auto", "Rage_Auto_multipoint_arms");
	SetupValue(Config::Ragebot::RageAuto::get().multipoint_body, 0, "Rage_Auto", "Rage_Auto_multipoint_body");
	SetupValue(Config::Ragebot::RageAuto::get().multipoint_head, 0, "Rage_Auto", "Rage_Auto_multipoint_head");
	SetupValue(Config::Ragebot::RageAuto::get().multipoint_legs, 0, "Rage_Auto", "Rage_Auto_multipoint_legs");
	SetupValue(Config::Ragebot::RageAuto::get().multipoint_neck, 0, "Rage_Auto", "Rage_Auto_multipoint_neck");
	SetupValue(Config::Ragebot::RageAuto::get().prefer_hitbox, 0, "Rage_Auto", "Rage_Auto_prefer_hitbox");
	SetupValue(Config::Ragebot::RageAuto::get().prefer, 0, "Rage_Auto", "Rage_Auto_prefer");

	SetupValue(Config::Ragebot::RageScout::get().auto_stop_method, 0, "Rage_Scout", "Rage_Scout_autostop");
	SetupValue(Config::Ragebot::RageScout::get().enable, false, "Rage_Scout", "Rage_Scout_enable");
	SetupValue(Config::Ragebot::RageScout::get().hitboxes, false, 5, "Rage_Scout", "Rage_Scout_hitboxes");
	SetupValue(Config::Ragebot::RageScout::get().hitchance, 0, "Rage_Scout", "Rage_Scout_hitchance");
	SetupValue(Config::Ragebot::RageScout::get().min_dmg, 0, "Rage_Scout", "Rage_Scout_min_dmg");
	SetupValue(Config::Ragebot::RageScout::get().min_dmg_autowall, 0, "Rage_Scout", "Rage_Scout_min_dmg_autowall");
	SetupValue(Config::Ragebot::RageScout::get().multipoint_arms, 0, "Rage_Scout", "Rage_Scout_multipoint_arms");
	SetupValue(Config::Ragebot::RageScout::get().multipoint_body, 0, "Rage_Scout", "Rage_Scout_multipoint_body");
	SetupValue(Config::Ragebot::RageScout::get().multipoint_head, 0, "Rage_Scout", "Rage_Scout_multipoint_head");
	SetupValue(Config::Ragebot::RageScout::get().multipoint_legs, 0, "Rage_Scout", "Rage_Scout_multipoint_legs");
	SetupValue(Config::Ragebot::RageScout::get().multipoint_neck, 0, "Rage_Scout", "Rage_Scout_multipoint_neck");
	SetupValue(Config::Ragebot::RageScout::get().prefer_hitbox, 0, "Rage_Scout", "Rage_Scout_prefer_hitbox");
	SetupValue(Config::Ragebot::RageScout::get().prefer, 0, "Rage_Scout", "Rage_Scout_prefer");

	SetupValue(Config::Ragebot::RageAWP::get().auto_stop_method, 0, "Rage_AWP", "Rage_AWP_autostop");
	SetupValue(Config::Ragebot::RageAWP::get().enable, false, "Rage_AWP", "Rage_AWP_enable");
	SetupValue(Config::Ragebot::RageAWP::get().hitboxes, false, 5, "Rage_AWP", "Rage_AWP_hitboxes");
	SetupValue(Config::Ragebot::RageAWP::get().hitchance, 0, "Rage_AWP", "Rage_AWP_hitchance");
	SetupValue(Config::Ragebot::RageAWP::get().min_dmg, 0, "Rage_AWP", "Rage_AWP_min_dmg");
	SetupValue(Config::Ragebot::RageAWP::get().min_dmg_autowall, 0, "Rage_AWP", "Rage_AWP_min_dmg_autowall");
	SetupValue(Config::Ragebot::RageAWP::get().multipoint_arms, 0, "Rage_AWP", "Rage_AWP_multipoint_arms");
	SetupValue(Config::Ragebot::RageAWP::get().multipoint_body, 0, "Rage_AWP", "Rage_AWP_multipoint_body");
	SetupValue(Config::Ragebot::RageAWP::get().multipoint_head, 0, "Rage_AWP", "Rage_AWP_multipoint_head");
	SetupValue(Config::Ragebot::RageAWP::get().multipoint_legs, 0, "Rage_AWP", "Rage_AWP_multipoint_legs");
	SetupValue(Config::Ragebot::RageAWP::get().multipoint_neck, 0, "Rage_AWP", "Rage_AWP_multipoint_neck");
	SetupValue(Config::Ragebot::RageAWP::get().prefer_hitbox, 0, "Rage_AWP", "Rage_AWP_prefer_hitbox");
	SetupValue(Config::Ragebot::RageAWP::get().prefer, 0, "Rage_AWP", "Rage_AWP_prefer");

	SetupValue(Config::Ragebot::RageShotGuns::get().auto_stop_method, 0, "Rage_ShotGuns", "Rage_ShotGuns_autostop");
	SetupValue(Config::Ragebot::RageShotGuns::get().enable, false, "Rage_ShotGuns", "Rage_ShotGuns_enable");
	SetupValue(Config::Ragebot::RageShotGuns::get().hitboxes, false, 5, "Rage_ShotGuns", "Rage_ShotGuns_hitboxes");
	SetupValue(Config::Ragebot::RageShotGuns::get().hitchance, 0, "Rage_ShotGuns", "Rage_ShotGuns_hitchance");
	SetupValue(Config::Ragebot::RageShotGuns::get().min_dmg, 0, "Rage_ShotGuns", "Rage_ShotGuns_min_dmg");
	SetupValue(Config::Ragebot::RageShotGuns::get().min_dmg_autowall, 0, "Rage_ShotGuns", "Rage_ShotGuns_min_dmg_autowall");
	SetupValue(Config::Ragebot::RageShotGuns::get().multipoint_arms, 0, "Rage_ShotGuns", "Rage_ShotGuns_multipoint_arms");
	SetupValue(Config::Ragebot::RageShotGuns::get().multipoint_body, 0, "Rage_ShotGuns", "Rage_ShotGuns_multipoint_body");
	SetupValue(Config::Ragebot::RageShotGuns::get().multipoint_head, 0, "Rage_ShotGuns", "Rage_ShotGuns_multipoint_head");
	SetupValue(Config::Ragebot::RageShotGuns::get().multipoint_legs, 0, "Rage_ShotGuns", "Rage_ShotGuns_multipoint_legs");
	SetupValue(Config::Ragebot::RageShotGuns::get().multipoint_neck, 0, "Rage_ShotGuns", "Rage_ShotGuns_multipoint_neck");
	SetupValue(Config::Ragebot::RageShotGuns::get().prefer_hitbox, 0, "Rage_ShotGuns", "Rage_ShotGuns_prefer_hitbox");
	SetupValue(Config::Ragebot::RageShotGuns::get().prefer, 0, "Rage_ShotGuns", "Rage_ShotGuns_prefer");

	SetupValue(Config::Ragebot::RageShotGuns::get().auto_stop_method, 0, "Rage_LMG", "Rage_LMG_autostop");
	SetupValue(Config::Ragebot::RageShotGuns::get().enable, false, "Rage_LMG", "Rage_LMG_enable");
	SetupValue(Config::Ragebot::RageShotGuns::get().hitboxes, false, 5, "Rage_LMG", "Rage_LMG_hitboxes");
	SetupValue(Config::Ragebot::RageShotGuns::get().hitchance, 0, "Rage_LMG", "Rage_LMG_hitchance");
	SetupValue(Config::Ragebot::RageShotGuns::get().min_dmg, 0, "Rage_LMG", "Rage_LMG_min_dmg");
	SetupValue(Config::Ragebot::RageShotGuns::get().min_dmg_autowall, 0, "Rage_LMG", "Rage_LMG_min_dmg_autowall");
	SetupValue(Config::Ragebot::RageShotGuns::get().multipoint_arms, 0, "Rage_LMG", "Rage_LMG_multipoint_arms");
	SetupValue(Config::Ragebot::RageShotGuns::get().multipoint_body, 0, "Rage_LMG", "Rage_LMG_multipoint_body");
	SetupValue(Config::Ragebot::RageShotGuns::get().multipoint_head, 0, "Rage_LMG", "Rage_LMG_multipoint_head");
	SetupValue(Config::Ragebot::RageShotGuns::get().multipoint_legs, 0, "Rage_LMG", "Rage_LMG_multipoint_legs");
	SetupValue(Config::Ragebot::RageShotGuns::get().multipoint_neck, 0, "Rage_LMG", "Rage_LMG_multipoint_neck");
	SetupValue(Config::Ragebot::RageShotGuns::get().prefer_hitbox, 0, "Rage_LMG", "Rage_LMG_prefer_hitbox");
	SetupValue(Config::Ragebot::RageShotGuns::get().prefer, 0, "Rage_LMG", "Rage_LMG_prefer");
	//antiaim

	SetupValue(Config::AntiAim::AA::get().jitterspeed, 0, "AntiAim_AA", "AntiAim_AA_jitterspeed");
	SetupValue(Config::AntiAim::AA::get().jitterangle, 0, "AntiAim_AA", "AntiAim_AA_jitterangle");
	SetupValue(Config::AntiAim::AA::get().jitterwidth, 0, "AntiAim_AA", "AntiAim_AA_jitterwidth");
	SetupValue(Config::AntiAim::AA::get().pitch, 0, "AntiAim_AA", "AntiAim_AA_pitch");
	SetupValue(Config::AntiAim::AA::get().yaw, 0, "AntiAim_AA", "AntiAim_AA_yaw");
	SetupValue(Config::AntiAim::AA::get().yawbase, 0, "AntiAim_AA", "AntiAim_AA_yawbase");

	SetupValue(Config::AntiAim::AADesync::get().desync, 0, "AntiAim_Desync", "AntiAim_Desync_desync");
	SetupValue(Config::AntiAim::AADesync::get().desyncbase, 0, "AntiAim_Desync", "AntiAim_Desync_desyncbase");
	SetupValue(Config::AntiAim::AADesync::get().desync_flip, 0, "AntiAim_Desync", "AntiAim_Desync_desync_flip");
	SetupValue(Config::AntiAim::AADesync::get().angle, 0, "AntiAim_Desync", "AntiAim_Desync_angle");


	SetupValue(Config::AntiAim::AAFakelag::get().fakelag, 0, "AntiAim_Fakelag", "AntiAim_Fakelag_fakelag");
	SetupValue(Config::AntiAim::AAFakelag::get().fakelagticks, 0, "AntiAim_Fakelag", "AntiAim_Fakelag_fakelagticks");
	SetupValue(Config::AntiAim::AAFakelag::get().fakelagtickstrigger, 0, "AntiAim_Fakelag", "AntiAim_Fakelag_fakelagtickstrigger");
	SetupValue(Config::AntiAim::AAFakelag::get().triggers, 3, false, "AntiAim_Fakelag", "AntiAim_Fakelag_triggers");

	SetupValue(Config::AntiAim::AAMisc::get().menual_left, 0, "AntiAim_Misc", "AntiAim_Misc_menual_left");
	SetupValue(Config::AntiAim::AAMisc::get().menual_right, 0, "AntiAim_Misc", "AntiAim_Misc_menual_right");
	SetupValue(Config::AntiAim::AAMisc::get().menual_back, 0, "AntiAim_Misc", "AntiAim_Misc_menual_back"); 
	SetupValue(Config::AntiAim::AAMisc::get().slowwwalk, 0, "AntiAim_Misc", "AntiAim_Misc_slowwwalk");
	SetupValue(Config::AntiAim::AAMisc::get().slowwwalk_speed, 0, "AntiAim_Misc", "AntiAim_Misc_slowwwalk_speed");
	SetupValue(Config::AntiAim::AAMisc::get().hide_shots, false, "AntiAim_Misc", "AntiAim_Misc_hide_shots");
	//visuals
	SetupValue(Config::Visuals::Main::Visualsmain::get().Enable, false, "Visuals_Main", "Visuals_Main_Enable");
	SetupValue(Config::Visuals::Main::Visualsmain::get().Team, false, "Visuals_Main", "Visuals_Main_Team");
	SetupValue(Config::Visuals::Main::Visualsmain::get().Name, false, "Visuals_Main", "Visuals_Main_Name");
	SetupValue(Config::Visuals::Main::Visualsmain::get().Box, false, "Visuals_Main", "Visuals_Main_Box");
	SetupValue(Config::Visuals::Main::Visualsmain::get().Distance, false, "Visuals_Main", "Visuals_Main_Distance");
	SetupValue(Config::Visuals::Main::Visualsmain::get().Skeleton, false, "Visuals_Main", "Visuals_Main_Skeleton");
	SetupValue(Config::Visuals::Main::Visualsmain::get().BacktrackSkeleton, false, "Visuals_Main", "Visuals_Main_BacktrackSkeleton");
	SetupValue(Config::Visuals::Main::Visualsmain::get().ArmorBar, false, "Visuals_Main", "Visuals_Main_Armor");
	SetupValue(Config::Visuals::Main::Visualsmain::get().AmmoBar, false, "Visuals_Main", "Visuals_Main_Ammo");
	SetupValue(Config::Visuals::Main::Visualsmain::get().HealthBar, false, "Visuals_Main", "Visuals_Main_Health");
	SetupValue(Config::Visuals::Main::Visualsmain::get().Flags, false, "Visuals_Main", "Visuals_Main_Flags");
	SetupValue(Config::Visuals::Main::Visualsmain::get().WeaponIcon, false, "Visuals_Main", "Visuals_Main_Icon");
	SetupValue(Config::Visuals::Main::Visualsmain::get().WeaponText, false, "Visuals_Main", "Visuals_Main_Text");
	SetupValue(Config::Visuals::Main::Visualsmain::get().SoundESP, false, "Visuals_Main", "Visuals_Main_SoundESP");
	SetupValue(Config::Visuals::Main::Visualsmain::get().Arrows, false, "Visuals_Main", "Visuals_Main_Arrows");
	SetupValue(Config::Visuals::Main::Visualsmain::get().arrows_size, 0, "Visuals_Main", "Visuals_Main_arrows_size");
	SetupValue(Config::Visuals::Main::Visualsmain::get().arrows_rad, 0, "Visuals_Main", "Visuals_Main_arrows_rad");
	SetupValue(Config::Visuals::Main::Visualsmain::get().soundesp_size, 0, "Visuals_Main", "Visuals_Main_soundesp_size");
	SetupValue(Config::Visuals::Main::Visualsmain::get().soundesp_speed, 0, "Visuals_Main", "Visuals_Main_soundesp_speed");
	SetupValue(Config::Visuals::Main::Visualsmain::get().box_color_r, 255, "Visuals_Main", "Visuals_Main_box_r");
	SetupValue(Config::Visuals::Main::Visualsmain::get().box_color_g, 255, "Visuals_Main", "Visuals_Main_box_g");
	SetupValue(Config::Visuals::Main::Visualsmain::get().box_color_b, 255, "Visuals_Main", "Visuals_Main_box_b");
	SetupValue(Config::Visuals::Main::Visualsmain::get().box_color_a, 255, "Visuals_Main", "Visuals_Main_box_a");
	SetupValue(Config::Visuals::Main::Visualsmain::get().name_color_r, 255, "Visuals_Main", "Visuals_Main_name_r");
	SetupValue(Config::Visuals::Main::Visualsmain::get().name_color_g, 255, "Visuals_Main", "Visuals_Main_name_g");
	SetupValue(Config::Visuals::Main::Visualsmain::get().name_color_b, 255, "Visuals_Main", "Visuals_Main_name_b");
	SetupValue(Config::Visuals::Main::Visualsmain::get().name_color_a, 255, "Visuals_Main", "Visuals_Main_name_a");
	SetupValue(Config::Visuals::Main::Visualsmain::get().skeleton_color_r, 255, "Visuals_Main", "Visuals_Main_skeleton_r");
	SetupValue(Config::Visuals::Main::Visualsmain::get().skeleton_color_g, 255, "Visuals_Main", "Visuals_Main_skeleton_g");
	SetupValue(Config::Visuals::Main::Visualsmain::get().skeleton_color_b, 255, "Visuals_Main", "Visuals_Main_skeleton_b");
	SetupValue(Config::Visuals::Main::Visualsmain::get().skeleton_color_a, 255, "Visuals_Main", "Visuals_Main_skeleton_a");
	SetupValue(Config::Visuals::Main::Visualsmain::get().armorbar_color_r, 255, "Visuals_Main", "Visuals_Main_armor_r");
	SetupValue(Config::Visuals::Main::Visualsmain::get().armorbar_color_g, 255, "Visuals_Main", "Visuals_Main_armor_g");
	SetupValue(Config::Visuals::Main::Visualsmain::get().armorbar_color_b, 255, "Visuals_Main", "Visuals_Main_armor_b");
	SetupValue(Config::Visuals::Main::Visualsmain::get().armorbar_color_a, 255, "Visuals_Main", "Visuals_Main_armor_a");
	SetupValue(Config::Visuals::Main::Visualsmain::get().ammobar_color_r, 255, "Visuals_Main", "Visuals_Main_ammobar_r");
	SetupValue(Config::Visuals::Main::Visualsmain::get().ammobar_color_g, 255, "Visuals_Main", "Visuals_Main_ammobar_g");
	SetupValue(Config::Visuals::Main::Visualsmain::get().ammobar_color_b, 255, "Visuals_Main", "Visuals_Main_ammobar_b");
	SetupValue(Config::Visuals::Main::Visualsmain::get().ammobar_color_a, 255, "Visuals_Main", "Visuals_Main_ammobar_a");
	SetupValue(Config::Visuals::Main::Visualsmain::get().distance_color_r, 255, "Visuals_Main", "Visuals_Main_distance_r");
	SetupValue(Config::Visuals::Main::Visualsmain::get().distance_color_g, 255, "Visuals_Main", "Visuals_Main_distance_g");
	SetupValue(Config::Visuals::Main::Visualsmain::get().distance_color_b, 255, "Visuals_Main", "Visuals_Main_distance_b");
	SetupValue(Config::Visuals::Main::Visualsmain::get().distance_color_a, 255, "Visuals_Main", "Visuals_Main_distance_a");
	SetupValue(Config::Visuals::Main::Visualsmain::get().weaponicon_color_r, 255, "Visuals_Main", "Visuals_Main_weaponicon_r");
	SetupValue(Config::Visuals::Main::Visualsmain::get().weaponicon_color_g, 255, "Visuals_Main", "Visuals_Main_weaponicon_g");
	SetupValue(Config::Visuals::Main::Visualsmain::get().weaponicon_color_b, 255, "Visuals_Main", "Visuals_Main_weaponicon_b");
	SetupValue(Config::Visuals::Main::Visualsmain::get().weaponicon_color_a, 255, "Visuals_Main", "Visuals_Main_weaponicon_a");
	SetupValue(Config::Visuals::Main::Visualsmain::get().weaponname_color_r, 255, "Visuals_Main", "Visuals_Main_weaponname_r");
	SetupValue(Config::Visuals::Main::Visualsmain::get().weaponname_color_g, 255, "Visuals_Main", "Visuals_Main_weaponname_g");
	SetupValue(Config::Visuals::Main::Visualsmain::get().weaponname_color_b, 255, "Visuals_Main", "Visuals_Main_weaponname_b");
	SetupValue(Config::Visuals::Main::Visualsmain::get().weaponname_color_a, 255, "Visuals_Main", "Visuals_Main_weaponname_a");
	SetupValue(Config::Visuals::Main::Visualsmain::get().arrows_color_r, 255, "Visuals_Main", "Visuals_Main_arrows_color_r");
	SetupValue(Config::Visuals::Main::Visualsmain::get().arrows_color_g, 255, "Visuals_Main", "Visuals_Main_arrows_color_g");
	SetupValue(Config::Visuals::Main::Visualsmain::get().arrows_color_b, 255, "Visuals_Main", "Visuals_Main_arrows_color_b");
	SetupValue(Config::Visuals::Main::Visualsmain::get().arrows_color_a, 255, "Visuals_Main", "Visuals_Main_arrows_color_a");
	SetupValue(Config::Visuals::Main::Visualsmain::get().soundesp_color_r, 255, "Visuals_Main", "Visuals_Main_soundesp_color_r");
	SetupValue(Config::Visuals::Main::Visualsmain::get().soundesp_color_g, 255, "Visuals_Main", "Visuals_Main_soundesp_color_g");
	SetupValue(Config::Visuals::Main::Visualsmain::get().soundesp_color_b, 255, "Visuals_Main", "Visuals_Main_soundesp_color_b");
	SetupValue(Config::Visuals::Main::Visualsmain::get().soundesp_color_a, 255, "Visuals_Main", "Visuals_Main_soundesp_color_a");
	SetupValue(Config::Visuals::Main::Visualsmain::get().btskeleton_color_r, 255, "Visuals_Main", "Visuals_Main_btskeleton_color_r");
	SetupValue(Config::Visuals::Main::Visualsmain::get().btskeleton_color_g, 255, "Visuals_Main", "Visuals_Main_btskeleton_color_g");
	SetupValue(Config::Visuals::Main::Visualsmain::get().btskeleton_color_b, 255, "Visuals_Main", "Visuals_Main_btskeleton_color_b");
	SetupValue(Config::Visuals::Main::Visualsmain::get().btskeleton_color_a, 255, "Visuals_Main", "Visuals_Main_btskeleton_color_a");


	SetupValue(Config::Visuals::Main::Visualsworld::get().Radar, false, "Visuals_World", "Visuals_World_Radar");
	SetupValue(Config::Visuals::Main::Visualsworld::get().Bomb, false, "Visuals_World", "Visuals_World_Bomb");
	SetupValue(Config::Visuals::Main::Visualsworld::get().Crosshair, false, "Visuals_World", "Visuals_World_Crosshair");
	SetupValue(Config::Visuals::Main::Visualsworld::get().DroppedGrenades, false, "Visuals_World", "Visuals_World_DroppedGranades");
	SetupValue(Config::Visuals::Main::Visualsworld::get().DroppedWeaponAmmo, false, "Visuals_World", "Visuals_World_DroppedWeaponAmmo");
	SetupValue(Config::Visuals::Main::Visualsworld::get().GranadeTrajectory, false, "Visuals_World", "Visuals_World_GranadeTrajectory");
	SetupValue(Config::Visuals::Main::Visualsworld::get().GranadeWaring, false, "Visuals_World", "Visuals_World_GranadeWaring");
	SetupValue(Config::Visuals::Main::Visualsworld::get().PenetrationCrosshair, false, "Visuals_World", "Visuals_World_PenetrationCrosshair");
	SetupValue(Config::Visuals::Main::Visualsworld::get().Spectators, false, "Visuals_World", "Visuals_World_Spectators");
	SetupValue(Config::Visuals::Main::Visualsworld::get().SpreadCircle, false, "Visuals_World", "Visuals_World_SpreadCircle");
	SetupValue(Config::Visuals::Main::Visualsworld::get().VisualizeTaserRange, false, "Visuals_World", "Visuals_World_VisualizeTaserRange");
	SetupValue(Config::Visuals::Main::Visualsworld::get().DroppedWeapons,false, 3, "Visuals_World", "Visuals_World_DroppedWeapons");
	SetupValue(Config::Visuals::Main::Visualsworld::get().droppedweapon_color_a, 255, "Visuals_World", "Visuals_World_droppedweapon_color_r");
	SetupValue(Config::Visuals::Main::Visualsworld::get().droppedweapon_color_g, 255, "Visuals_World", "Visuals_World_droppedweapon_color_g");
	SetupValue(Config::Visuals::Main::Visualsworld::get().droppedweapon_color_b, 255, "Visuals_World", "Visuals_World_droppedweapon_color_b");
	SetupValue(Config::Visuals::Main::Visualsworld::get().droppedweapon_color_a, 255, "Visuals_World", "Visuals_World_droppedweapon_color_a");
	SetupValue(Config::Visuals::Main::Visualsmain::get().screen, false, 5,  "Visuals_World", "Visuals_World_Screen");


	SetupValue(Config::Visuals::Main::Visualsworld::get().droppedweapon_ammo_color_a, 255, "Visuals_World", "Visuals_World_droppedweapon_ammo_color_r");
	SetupValue(Config::Visuals::Main::Visualsworld::get().droppedweapon_ammo_color_g, 255, "Visuals_World", "Visuals_World_droppedweapon_ammo_color_g");
	SetupValue(Config::Visuals::Main::Visualsworld::get().droppedweapon_ammo_color_b, 255, "Visuals_World", "Visuals_World_droppedweapon_ammo_color_b");
	SetupValue(Config::Visuals::Main::Visualsworld::get().droppedweapon_ammo_color_a, 255, "Visuals_World", "Visuals_World_droppedweapon_ammo_color_a");
	SetupValue(Config::Visuals::Main::Visualsworld::get().bomb_color_a, 255, "Visuals_World", "Visuals_World_bomb_color_r");
	SetupValue(Config::Visuals::Main::Visualsworld::get().bomb_color_g, 255, "Visuals_World", "Visuals_World_bomb_color_g");
	SetupValue(Config::Visuals::Main::Visualsworld::get().bomb_color_b, 255, "Visuals_World", "Visuals_World_bomb_color_b");
	SetupValue(Config::Visuals::Main::Visualsworld::get().bomb_color_a, 255, "Visuals_World", "Visuals_World_bomb_color_a");
	SetupValue(Config::Visuals::Main::Visualsworld::get().droppedgranades_color_a, 255, "Visuals_World", "Visuals_World_droppedgranades_color_r");
	SetupValue(Config::Visuals::Main::Visualsworld::get().droppedgranades_color_g, 255, "Visuals_World", "Visuals_World_droppedgranades_color_g");
	SetupValue(Config::Visuals::Main::Visualsworld::get().droppedgranades_color_b, 255, "Visuals_World", "Visuals_World_droppedgranades_color_b");
	SetupValue(Config::Visuals::Main::Visualsworld::get().droppedgranades_color_a, 255, "Visuals_World", "Visuals_World_droppedgranades_color_a");
	SetupValue(Config::Visuals::Main::Visualsworld::get().granadetrajectory_color_a, 255, "Visuals_World", "Visuals_World_granadetrajectory_color_r");
	SetupValue(Config::Visuals::Main::Visualsworld::get().granadetrajectory_color_g, 255, "Visuals_World", "Visuals_World_granadetrajectory_color_g");
	SetupValue(Config::Visuals::Main::Visualsworld::get().granadetrajectory_color_b, 255, "Visuals_World", "Visuals_World_granadetrajectory_color_b");
	SetupValue(Config::Visuals::Main::Visualsworld::get().granadetrajectory_color_a, 255, "Visuals_World", "Visuals_World_granadetrajectory_color_a");


	SetupValue(Config::Visuals::Main::VisualsEffects::get().AsusProps, 100, "Visuals_Effects", "Visuals_Effects_AsusProps");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().AsusWalls, 100, "Visuals_Effects", "Visuals_Effects_AsusWalls");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().ThirdPerson, 0, "Visuals_Effects", "Visuals_Effects_ThirdPerson");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().BulletImapcts, 0, "Visuals_Effects", "Visuals_Effects_BulletImapcts");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().BulletTracers, false, "Visuals_Effects", "Visuals_Effects_BulletTracers");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().Brightness, 100, "Visuals_Effects", "Visuals_Effects_Brightness");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().DisablePostProcessing, false, "Visuals_Effects", "Visuals_Effects_DPP");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().RemoveEffect, false, 5, "Visuals_Effects", "Visuals_Effects_RemoveEffect");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().HitEffect, false, 5, "Visuals_Effects", "Visuals_Effects_HitEffect");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().FullBright, false, "Visuals_Effects", "Visuals_Effects_FullBright");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().KillEfect, false, "Visuals_Effects", "Visuals_Effects_KillEfect");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().Skybox, 0, "Visuals_Effects", "Visuals_Effects_Skybox");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().world_color_r, 255, "Visuals_Effects", "Visuals_Effects_world_color_r");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().world_color_g, 255, "Visuals_Effects", "Visuals_Effects_world_color_b");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().world_color_b, 255, "Visuals_Effects", "Visuals_Effects_world_color_b");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().world_color_a, 255, "Visuals_Effects", "Visuals_Effects_world_color_a");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_r, 255, "Visuals_Effects", "Visuals_Effects_bulletimapcts_color_r");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_g, 255, "Visuals_Effects", "Visuals_Effects_bulletimapcts_color_g");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_b, 255, "Visuals_Effects", "Visuals_Effects_bulletimapcts_color_b");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_a, 255, "Visuals_Effects", "Visuals_Effects_bulletimapcts_color_a");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().bullettracers_color_r, 255, "Visuals_Effects", "Visuals_Effects_bullettracers_color_r");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().bullettracers_color_g, 255, "Visuals_Effects", "Visuals_Effects_bullettracers_color_g");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().bullettracers_color_b, 255, "Visuals_Effects", "Visuals_Effects_bullettracers_color_b");
	SetupValue(Config::Visuals::Main::VisualsEffects::get().bullettracers_color_a, 255, "Visuals_Effects", "Visuals_Effects_bullettracers_color_a");


	SetupValue(Config::Visuals::Main::Visualschams::get().Chams, false, 4, "Visuals_Chams", "Visuals_Chams_chams");
	SetupValue(Config::Visuals::Main::Visualschams::get().local_chams, 0, "Visuals_Chams", "Visuals_Chams_local_chams");
	SetupValue(Config::Visuals::Main::Visualschams::get().desync_chams, 0, "Visuals_Chams", "Visuals_Chams_desync_chams");
	SetupValue(Config::Visuals::Main::Visualschams::get().local_glow, false, "Visuals_Chams", "Visuals_Chams_local_glow");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_chams, 0, "Visuals_Chams", "Visuals_Chams_enemy_chams");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_xyz_chams, false, "Visuals_Chams", "Visuals_Chams_enemy_xyz");
	SetupValue(Config::Visuals::Main::Visualschams::get().backtrack_chams, false, "Visuals_Chams", "Visuals_Chams_backtrack_chams");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_glow, false, "Visuals_Chams", "Visuals_Chams_enemy_glow");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_chams, 0, "Visuals_Chams", "Visuals_Chams_team_chams");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_xyz_chams, false, "Visuals_Chams", "Visuals_Chams_team_xyz");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_glow, false, "Visuals_Chams", "Visuals_Chams_team_glow");
	SetupValue(Config::Visuals::Main::Visualschams::get().weapon_chams, 0, "Visuals_Chams", "Visuals_Chams_weapon_chams");
	SetupValue(Config::Visuals::Main::Visualschams::get().weapon_chams_wireframe, false, "Visuals_Chams", "Visuals_Chams_weapon_chams_wireframe");
	SetupValue(Config::Visuals::Main::Visualschams::get().hand_chams, 0, "Visuals_Chams", "Visuals_Chams_hand_chams");
	SetupValue(Config::Visuals::Main::Visualschams::get().hand_chams_wireframe, false, "Visuals_Chams", "Visuals_Chams_hand_chams_wireframe");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_chams_r, 255, "Visuals_Chams", "Visuals_Chams_enemy_chams_r");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_chams_g, 255, "Visuals_Chams", "Visuals_Chams_enemy_chams_g");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_chams_b, 255, "Visuals_Chams", "Visuals_Chams_enemy_chams_b");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_chams_a, 255, "Visuals_Chams", "Visuals_Chams_enemy_chams_a");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_chams_w, 255, "Visuals_Chams", "Visuals_Chams_enemy_chams_w");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_r, 255, "Visuals_Chams", "Visuals_Chams_enemy_chams_xqz_r");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_g, 255, "Visuals_Chams", "Visuals_Chams_enemy_chams_xqz_g");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_b, 255, "Visuals_Chams", "Visuals_Chams_enemy_chams_xqz_b");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_a, 255, "Visuals_Chams", "Visuals_Chams_enemy_chams_xqz_a");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_w, 255, "Visuals_Chams", "Visuals_Chams_enemy_chams_xqz_w");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_glow_r, 255, "Visuals_Chams", "Visuals_Chams_enemy_glow_r");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_glow_g, 255, "Visuals_Chams", "Visuals_Chams_enemy_glow_g");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_glow_b, 255, "Visuals_Chams", "Visuals_Chams_enemy_glow_b");
	SetupValue(Config::Visuals::Main::Visualschams::get().enemy_glow_a, 255, "Visuals_Chams", "Visuals_Chams_enemy_enemy_glow_a");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_chams_r, 255, "Visuals_Chams", "Visuals_Chams_team_chams_r");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_chams_g, 255, "Visuals_Chams", "Visuals_Chams_team_chams_g");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_chams_b, 255, "Visuals_Chams", "Visuals_Chams_team_chams_b");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_chams_a, 255, "Visuals_Chams", "Visuals_Chams_team_chams_a");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_chams_w, 255, "Visuals_Chams", "Visuals_Chams_team_chams_w");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_chams_xqz_r, 255, "Visuals_Chams", "Visuals_Chams_team_chams_xqz_r");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_chams_xqz_g, 255, "Visuals_Chams", "Visuals_Chams_team_chams_xqz_g");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_chams_xqz_b, 255, "Visuals_Chams", "Visuals_Chams_team_chams_xqz_b");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_chams_xqz_a, 255, "Visuals_Chams", "Visuals_Chams_team_chams_xqz_a");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_chams_xqz_w, 255, "Visuals_Chams", "Visuals_Chams_team_chams_xqz_w");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_glow_r, 255, "Visuals_Chams", "Visuals_Chams_team_glow_r");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_glow_g, 255, "Visuals_Chams", "Visuals_Chams_team_glow_g");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_glow_b, 255, "Visuals_Chams", "Visuals_Chams_team_glow_b");
	SetupValue(Config::Visuals::Main::Visualschams::get().team_glow_a, 255, "Visuals_Chams", "Visuals_Chams_team_glow_a");
	SetupValue(Config::Visuals::Main::Visualschams::get().desync_chams_r, 255, "Visuals_Chams", "Visuals_Chams_desync_chams_r");
	SetupValue(Config::Visuals::Main::Visualschams::get().desync_chams_g, 255, "Visuals_Chams", "Visuals_Chams_desync_chams_g");
	SetupValue(Config::Visuals::Main::Visualschams::get().desync_chams_b, 255, "Visuals_Chams", "Visuals_Chams_desync_chams_b");
	SetupValue(Config::Visuals::Main::Visualschams::get().desync_chams_a, 255, "Visuals_Chams", "Visuals_Chams_desync_chams_a");
	SetupValue(Config::Visuals::Main::Visualschams::get().desync_chams_w, 255, "Visuals_Chams", "Visuals_Chams_desync_chams_w");
	SetupValue(Config::Visuals::Main::Visualschams::get().local_chams_r, 255, "Visuals_Chams", "Visuals_Chams_local_chams_r");
	SetupValue(Config::Visuals::Main::Visualschams::get().local_chams_g, 255, "Visuals_Chams", "Visuals_Chams_local_chams_g");
	SetupValue(Config::Visuals::Main::Visualschams::get().local_chams_b, 255, "Visuals_Chams", "Visuals_Chams_local_chams_b");
	SetupValue(Config::Visuals::Main::Visualschams::get().local_chams_a, 255, "Visuals_Chams", "Visuals_Chams_local_chams_a");
	SetupValue(Config::Visuals::Main::Visualschams::get().local_chams_w, 255, "Visuals_Chams", "Visuals_Chams_local_chams_w");
	SetupValue(Config::Visuals::Main::Visualschams::get().fakelag_chams_r, 255, "Visuals_Chams", "Visuals_Chams_fakelag_chams_r");
	SetupValue(Config::Visuals::Main::Visualschams::get().fakelag_chams_g, 255, "Visuals_Chams", "Visuals_Chams_fakelag_chams_g");
	SetupValue(Config::Visuals::Main::Visualschams::get().fakelag_chams_b, 255, "Visuals_Chams", "Visuals_Chams_fakelag_chams_b");
	SetupValue(Config::Visuals::Main::Visualschams::get().fakelag_chams_a, 255, "Visuals_Chams", "Visuals_Chams_fakelag_chams_a");
	SetupValue(Config::Visuals::Main::Visualschams::get().local_glow_r, 255, "Visuals_Chams", "Visuals_Chams_local_glow_r");
	SetupValue(Config::Visuals::Main::Visualschams::get().local_glow_g, 255, "Visuals_Chams", "Visuals_Chams_local_glow_g");
	SetupValue(Config::Visuals::Main::Visualschams::get().local_glow_b, 255, "Visuals_Chams", "Visuals_Chams_local_glow_b");
	SetupValue(Config::Visuals::Main::Visualschams::get().local_glow_a, 255, "Visuals_Chams", "Visuals_Chams_local_glow_a");
	SetupValue(Config::Visuals::Main::Visualschams::get().hand_chams_r, 255, "Visuals_Chams", "Visuals_Chams_hand_chams_r");
	SetupValue(Config::Visuals::Main::Visualschams::get().hand_chams_g, 255, "Visuals_Chams", "Visuals_Chams_hand_chams_g");
	SetupValue(Config::Visuals::Main::Visualschams::get().hand_chams_b, 255, "Visuals_Chams", "Visuals_Chams_hand_chams_b");
	SetupValue(Config::Visuals::Main::Visualschams::get().hand_chams_a, 255, "Visuals_Chams", "Visuals_Chams_hand_chams_a");
	SetupValue(Config::Visuals::Main::Visualschams::get().hand_chams_w, 255, "Visuals_Chams", "Visuals_Chams_hand_chams_w");
	SetupValue(Config::Visuals::Main::Visualschams::get().weapon_chams_r, 255, "Visuals_Chams", "Visuals_Chams_weapon_chams_r");
	SetupValue(Config::Visuals::Main::Visualschams::get().weapon_chams_g, 255, "Visuals_Chams", "Visuals_Chams_weapon_chams_g");
	SetupValue(Config::Visuals::Main::Visualschams::get().weapon_chams_b, 255, "Visuals_Chams", "Visuals_Chams_weapon_chams_b");
	SetupValue(Config::Visuals::Main::Visualschams::get().weapon_chams_a, 255, "Visuals_Chams", "Visuals_Chams_weapon_chams_a");
	SetupValue(Config::Visuals::Main::Visualschams::get().weapon_chams_w, 255, "Visuals_Chams", "Visuals_Chams_weapon_chams_w");
	SetupValue(Config::Visuals::Main::Visualschams::get().backtrack_chams_r, 255, "Visuals_Chams", "Visuals_Chams_backtrack_chams_r");
	SetupValue(Config::Visuals::Main::Visualschams::get().backtrack_chams_g, 255, "Visuals_Chams", "Visuals_Chams_backtrack_chams_g");
	SetupValue(Config::Visuals::Main::Visualschams::get().backtrack_chams_b, 255, "Visuals_Chams", "Visuals_Chams_backtrack_chams_b");
	SetupValue(Config::Visuals::Main::Visualschams::get().backtrack_chams_a, 255, "Visuals_Chams", "Visuals_Chams_backtrack_chams_a");

	//misc
	SetupValue(Config::Misc::MiscMain::get().anti_untrasted, false, "Misc_main", "Misc_main_anti_untrasted");
	SetupValue(Config::Misc::MiscMain::get().autostrafer, false, "Misc_main", "Misc_main_autostrafer");
	SetupValue(Config::Misc::MiscMain::get().auto_bunnyHop, false, "Misc_main", "Misc_main_auto_bunnyHop");
	SetupValue(Config::Misc::MiscMain::get().auto_knife, false, "Misc_main", "Misc_main_auto_knife");
	SetupValue(Config::Misc::MiscMain::get().auto_zeus, false, "Misc_main", "Misc_main_auto_zeus");
	SetupValue(Config::Misc::MiscMain::get().block_bot, false, "Misc_main", "Misc_main_block_bot");
	SetupValue(Config::Misc::MiscMain::get().buy_bot, false, "Misc_main", "Misc_main_auto_buy_bot");
	SetupValue(Config::Misc::MiscMain::get().clantag_spamer, false, "Misc_main", "Misc_main_clantag_spamer");
	SetupValue(Config::Misc::MiscMain::get().granade, false, 4, "Misc_main", "Misc_main_granade");
	SetupValue(Config::Misc::MiscMain::get().log, false, "Misc_main", "Misc_main_log");
	SetupValue(Config::Misc::MiscMain::get().name_stealer, false, "Misc_main", "Misc_main_name_stealer");
	SetupValue(Config::Misc::MiscMain::get().other, false, 4, "Misc_main", "Misc_main_other");
	SetupValue(Config::Misc::MiscMain::get().viewmodelchanger, false, "Misc_main", "Misc_main_viewmodelchanger");
	SetupValue(Config::Misc::MiscMain::get().viewmodel_x, 0, "Misc_main", "Misc_main_viewmodel_x");
	SetupValue(Config::Misc::MiscMain::get().viewmodel_y, 0, "Misc_main", "Misc_main_viewmodel_y");
	SetupValue(Config::Misc::MiscMain::get().viewmodel_z, 0, "Misc_main", "Misc_viewmodel_z");
	SetupValue(Config::Misc::MiscMain::get().view_fov, 90, "Misc_main", "Misc_main_view_fov");
	SetupValue(Config::Misc::MiscMain::get().view_model_fov, 68, "Misc_main", "Misc_main_view_model_fov");
	SetupValue(Config::Misc::MiscMain::get().walk_bot, false, "Misc_main", "Misc_main_walk_bot");
	SetupValue(Config::Misc::MiscMain::get().weapon_main, 0, "Misc_main", "Misc_main_weapon_main");
	SetupValue(Config::Misc::MiscMain::get().weapon_sec, false, "Misc_main", "Misc_main_weapon_sec");
	SetupValue(Config::Misc::MiscMain::get().infduck, false, "Misc_main", "Misc_main_infduck");
	SetupValue(Config::Misc::MiscMain::get().fakeduck, 0, "Misc_main", "Misc_main_fakeduck");
	SetupValue(Config::Misc::MiscMain::get().screen_x, 1920, "Misc_main", "Misc_main_screen_x");
	SetupValue(Config::Misc::MiscMain::get().screen_y, 1080, "Misc_main", "Misc_main_screen_y");

	SetupValue(Config::SkinChanger::Knifes::get().knifemodel, 0, "Misc_main", "Misc_main_knifemodel");

	SetupValue(Config::ConfigMenu::ConfigMain::get().menu_color_r, 255, "Visuals_Chams", "Visuals_Chams_menu_color_r");
	SetupValue(Config::ConfigMenu::ConfigMain::get().menu_color_g, 255, "Visuals_Chams", "Visuals_Chams_menu_color_g");
	SetupValue(Config::ConfigMenu::ConfigMain::get().menu_color_b, 255, "Visuals_Chams", "Visuals_Chams_menu_color_b");
	SetupValue(Config::ConfigMenu::ConfigMain::get().menu_color_a, 255, "Visuals_Chams", "Visuals_Chams_menu_color_a");

}

void CConfig::export_to_clipboard(std::string file) {

	static TCHAR path[MAX_PATH];
	std::string folder, file_directory;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\stellarcheat.club\\";
		file_directory = std::string(path) + "\\stellarcheat.club\\" + file + ".ini";
	}

	Save(file);

	std::ifstream input_file = std::ifstream(file_directory);
	std::string str((std::istreambuf_iterator< char >(input_file)), std::istreambuf_iterator< char >());

	const char* output = str.c_str();
	const size_t len = strlen(output) + 1;

	HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(mem), output, len);

	GlobalUnlock(mem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, mem);
	CloseClipboard();
}


bool CConfig::import_from_clipboard(std::string file) {
	const auto get_clipboard_data = []() -> std::string {
		OpenClipboard(0);
		HANDLE hData = GetClipboardData(CF_TEXT);
		char *data = static_cast<char*>(GlobalLock(hData));
		std::string text(data);
		GlobalUnlock(hData);
		CloseClipboard();

		return text;
	};

	static TCHAR path[MAX_PATH];
	std::string folder, file_directory;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\stellarcheat.club\\";
		file_directory = std::string(path) + "\\stellarcheat.club\\" + file + ".ini";
	}

	std::string clipboard = get_clipboard_data();

	std::ofstream output = std::ofstream(file_directory);
	if (!output.good())
		return false;

	// clear our config
	output.clear();

	// write clipboard data to the file
	output << clipboard;


	// close the file after writing
	output.close();

	// re-load the imported config
	Load(file);

	return true;
}

void CConfig::SetupValue(int& value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue< int >(category, name, &value));
}

void CConfig::SetupValue(float& value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue< float >(category, name, &value));
}

void CConfig::SetupValue(bool& value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue< bool >(category, name, &value));
}

void CConfig::SetupValue(bool* value, bool def, int size, std::string category, std::string name) // for multiboxes
{
	for (int c = 0; c < size; c++)
	{
		value[c] = def;

		name += std::to_string(c);

		bools.push_back(new ConfigValue< bool >(category, name, &value[c]));
	}
}

void CConfig::Save(std::string ConfigName)
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\stellarcheat.club\\";
		file = std::string(path) + "\\stellarcheat.club\\" + ConfigName + ".ini";
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load(std::string ConfigName)
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + "\\stellarcheat.club\\";
		file = std::string(path) + "\\stellarcheat.club\\" + ConfigName + ".ini";
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

CConfig* Config2 = new CConfig();
