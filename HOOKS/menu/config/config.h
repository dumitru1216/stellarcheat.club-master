#include "../../../singleton.h"
namespace Config {
	namespace Visuals {
		namespace Main
		{
			class Visualsmain : public singleton< Visualsmain > {
			public:
				bool Enable;
				bool Team;
				bool Name;
				bool Box;
				bool Skeleton;
				bool BacktrackSkeleton;
				bool HealthBar;
				bool Flags;
				bool Distance;
				bool WeaponIcon;
				bool WeaponText;
				bool AmmoBar;
				bool ArmorBar;
				bool Arrows;
				int arrows_size;
				int arrows_rad;
				bool SoundESP;
				int soundesp_size;
				int soundesp_speed;
				bool screen[5] = { false,false,true,false,false };
				
				float box_color_r = 255, box_color_g = 255, box_color_b = 255, box_color_a = 255;
				float name_color_r = 255, name_color_g = 255, name_color_b = 255, name_color_a = 255;
				float skeleton_color_r = 255, skeleton_color_g = 255, skeleton_color_b = 255, skeleton_color_a = 255;
				float btskeleton_color_r = 255, btskeleton_color_g = 255, btskeleton_color_b = 255, btskeleton_color_a = 255;
				float distance_color_r = 255, distance_color_g = 255, distance_color_b = 255, distance_color_a = 255;
				float weaponicon_color_r = 255, weaponicon_color_g = 255, weaponicon_color_b = 255, weaponicon_color_a = 255;
				float weaponname_color_r = 255, weaponname_color_g = 255, weaponname_color_b = 255, weaponname_color_a = 255;
				float ammobar_color_r = 255, ammobar_color_g = 255, ammobar_color_b = 255, ammobar_color_a = 255;
				float armorbar_color_r = 255, armorbar_color_g = 255, armorbar_color_b = 255, armorbar_color_a = 255;
				float arrows_color_r = 255, arrows_color_g = 255, arrows_color_b = 255, arrows_color_a = 255;
				float soundesp_color_r = 255, soundesp_color_g = 255, soundesp_color_b = 255, soundesp_color_a = 255;
			};
			class Visualsworld : public singleton< Visualsworld > {
			public:
				bool Radar;
				bool DroppedWeapons[4] = { false,false,false,false };
				bool DroppedWeaponAmmo;
				bool DroppedGrenades;
				bool Bomb;
				bool GranadeTrajectory;
				bool Crosshair;
				bool GranadeWaring;
				bool Spectators;
				bool PenetrationCrosshair;
				bool VisualizeTaserRange;
				int slider1;
				bool SpreadCircle;

				float droppedweapon_color_r = 255, droppedweapon_color_g = 255, droppedweapon_color_b = 255, droppedweapon_color_a = 255;
				float droppedweapon_ammo_color_r = 255, droppedweapon_ammo_color_g = 255, droppedweapon_ammo_color_b = 255, droppedweapon_ammo_color_a = 255;

				float droppedgranades_color_r = 255, droppedgranades_color_g = 255, droppedgranades_color_b = 255, droppedgranades_color_a = 255;
				float bomb_color_r = 255, bomb_color_g = 255, bomb_color_b = 255, bomb_color_a = 255;
				float granadetrajectory_color_r = 255, granadetrajectory_color_g = 255, granadetrajectory_color_b = 255, granadetrajectory_color_a = 255;
			};
			class VisualsEffects : public singleton< VisualsEffects > {
			public:
				bool RemoveEffect[5] = { false,false,false,false,false };
				bool HitEffect[5] = { false,false,false,false, false };
				int Skybox;
				int BulletImapcts;
				bool BulletTracers;
				float AsusWalls = 100;
				float AsusProps = 100;
				float Brightness = 100;
				bool FullBright;
				bool DisablePostProcessing;
				bool KillEfect;
				int ThirdPerson = -1;
				int DrawLC;

				float world_color_r = 255, world_color_g = 255, world_color_b = 255, world_color_a = 255;
				float bulletimapcts_color_r = 255, bulletimapcts_color_g = 255, bulletimapcts_color_b = 255, bulletimapcts_color_a = 255;
				float bullettracers_color_r = 255, bullettracers_color_g = 255, bullettracers_color_b = 255, bullettracers_color_a = 255;
			};
			class Visualschams : public singleton< Visualschams > {
			public:
				bool Chams[4] = { false,false,false,false };
				int local_chams;
				int desync_chams;
				int fakelag_chams;
				bool local_glow;
				int enemy_chams;
				bool enemy_xyz_chams;
				bool backtrack_chams;
				bool enemy_glow;
				int team_chams;
				bool team_xyz_chams;
				bool team_glow;
				int hand_chams;
				bool hand_chams_wireframe;
				int weapon_chams;
				bool weapon_chams_wireframe;


				float backtrack_chams_r = 255, backtrack_chams_g = 255, backtrack_chams_b = 255, backtrack_chams_a = 255;
				float enemy_chams_r = 255, enemy_chams_g = 255, enemy_chams_b = 255, enemy_chams_a = 255, enemy_chams_w = 255;
				float enemy_chams_xqz_r = 255, enemy_chams_xqz_g = 255, enemy_chams_xqz_b = 255, enemy_chams_xqz_a = 255, enemy_chams_xqz_w = 255;
				float enemy_glow_r = 255, enemy_glow_g = 255, enemy_glow_b = 255, enemy_glow_a = 255;

				float team_chams_r = 255, team_chams_g = 255, team_chams_b = 255, team_chams_a = 255, team_chams_w = 255;
				float team_chams_xqz_r = 255, team_chams_xqz_g = 255, team_chams_xqz_b = 255, team_chams_xqz_a = 255, team_chams_xqz_w = 255;
				float team_glow_r = 255, team_glow_g = 255, team_glow_b = 255, team_glow_a = 255;

				float desync_chams_r = 255, desync_chams_g = 255, desync_chams_b = 255, desync_chams_a = 255, desync_chams_w = 255;
				float local_chams_r = 255, local_chams_g = 255, local_chams_b = 255, local_chams_a = 255, local_chams_w = 255;
				float fakelag_chams_r = 255, fakelag_chams_g = 255, fakelag_chams_b = 255, fakelag_chams_a = 255;
				float local_glow_r = 255, local_glow_g = 255, local_glow_b = 255, local_glow_a = 255;

				float weapon_chams_r = 255, weapon_chams_g = 255, weapon_chams_b = 255, weapon_chams_a = 255, weapon_chams_w = 255;
				float hand_chams_r = 255, hand_chams_g = 255, hand_chams_b = 255, hand_chams_a = 255, hand_chams_w = 255;

			};

		};


	}
	namespace Legitbot {
		class LegitGlobal : public singleton< LegitGlobal > {
		public:
			bool enable;
			int enemyscan;
			int trigerbot_key = -1;
			int legitbot_key = -1;
			bool smoke_check;
			bool flash_check;
			bool wall_check;
			bool legit_aa;
			bool show_legit_aa;
			float show_legit_aa_r = 225, show_legit_aa_g = 225, show_legit_aa_b = 225 , show_legit_aa_a = 225;
			bool backtrack_enable;
			int backtrack_method;
			int backtrack_ticks;
			int extend_time;
			bool trigerbot;
			int triggerbot_hitchance;
			bool rcs_enable;
			float rcs_x;
			float rcs_y;
		};
		class LegitPistol : public singleton< LegitPistol > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int fov;
			int smooth;
			bool silent;
			int delay = 0;
			int triggerbot_hitchance;
			bool rcs_enable;
			float rcs_x;
			float rcs_y;
		};
		class LegitHeavyPistol : public singleton< LegitHeavyPistol > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int fov;
			int smooth;
			bool silent;
			int delay = 0;
			int triggerbot_hitchance;
			bool rcs_enable;
			float rcs_x;
			float rcs_y;
		};
		class LegitSMG : public singleton< LegitSMG > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int fov;
			int smooth;
			bool silent;
			int delay = 0;
			int triggerbot_hitchance;
			bool rcs_enable;
			float rcs_x;
			float rcs_y;
		};
		class LegitAR : public singleton< LegitAR > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int fov;
			int smooth;
			bool silent;
			int delay = 0;
			int triggerbot_hitchance;
			bool rcs_enable;
			float rcs_x;
			float rcs_y;
		};
		class LegitAuto : public singleton< LegitAuto > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int fov;
			int smooth;
			bool silent;
			int delay = 0;
			int triggerbot_hitchance;
			bool rcs_enable;
			float rcs_x;
			float rcs_y;
		};
		class LegitScout : public singleton< LegitScout > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int fov;
			int smooth;
			bool silent;
			int delay = 0;
			int triggerbot_hitchance;
			bool rcs_enable;
			float rcs_x;
			float rcs_y;
		};
		class LegitAWP : public singleton< LegitAWP > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int fov;
			int smooth;
			bool silent;
			int delay = 0;
			int triggerbot_hitchance;
			bool rcs_enable;
			float rcs_x;
			float rcs_y;
		};
		class LegitShotGuns : public singleton< LegitShotGuns > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int fov;
			int smooth;
			bool silent;
			int delay = 0;
			int triggerbot_hitchance;
			bool rcs_enable;
			float rcs_x;
			float rcs_y;
		};
		class LegitLMG : public singleton< LegitLMG > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int fov;
			int smooth;
			bool silent;
			int delay = 0;
			int triggerbot_hitchance;
			bool rcs_enable;
			float rcs_x;
			float rcs_y;
		};
	
	
	
	};
	namespace Ragebot {
		class RageGlobal : public singleton< RageGlobal > {
		public:
			bool enable;
			int enemyscan;
			bool silent;
			bool autoshoot;
			bool auto_scope;
			bool remove_recoil;
			bool autowall;
			bool low_fps;
			int max_fov;
			bool backtrack_enable;
			int backtrack_method;
			int backtrack_ticks;
			bool extened_backtrack_ticks;
			float extend_time;
			bool resolver;
			int fakelag_cor;
			int acc_bost;
			int bodyaim = -1;
			bool remove_spread;
			int dubletap;
			int dubletap_key;
		};
		class RagePistol : public singleton< RagePistol > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int hitchance;
			int min_dmg;
			int min_dmg_autowall;
			int prefer_hitbox;
			int prefer;
			int auto_stop_method;
			int multipoint_head;
			int multipoint_body;
			int multipoint_arms;
			int multipoint_neck;
			int multipoint_legs;
		};
		class RageHeavyPistol : public singleton< RageHeavyPistol > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int hitchance;
			int min_dmg;
			int min_dmg_autowall;
			int prefer_hitbox;
			int prefer;
			int auto_stop_method;
			int multipoint_head;
			int multipoint_body;
			int multipoint_arms;
			int multipoint_neck;
			int multipoint_legs;
		};
		class RageSMG : public singleton< RageSMG > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int hitchance;
			int min_dmg;
			int min_dmg_autowall;
			int prefer_hitbox;
			int prefer;
			int auto_stop_method;
			int multipoint_head;
			int multipoint_body;
			int multipoint_arms;
			int multipoint_neck;
			int multipoint_legs;
		};
		class RageAR : public singleton< RageAR > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int hitchance;
			int min_dmg;
			int min_dmg_autowall;
			int prefer_hitbox;
			int prefer;
			int auto_stop_method;
			int multipoint_head;
			int multipoint_body;
			int multipoint_arms;
			int multipoint_neck;
			int multipoint_legs;
		};
		class RageAuto : public singleton< RageAuto > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int hitchance;
			int min_dmg;
			int min_dmg_autowall;
			int prefer_hitbox;
			int prefer;
			int auto_stop_method;
			int multipoint_head;
			int multipoint_body;
			int multipoint_arms;
			int multipoint_neck;
			int multipoint_legs;
		};
		class RageScout : public singleton< RageScout > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int hitchance;
			int min_dmg;
			int min_dmg_autowall;
			int prefer_hitbox;
			int prefer;
			int auto_stop_method;
			int multipoint_head;
			int multipoint_body;
			int multipoint_arms;
			int multipoint_neck;
			int multipoint_legs;
		};
		class RageAWP : public singleton< RageAWP > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int hitchance;
			int min_dmg;
			int min_dmg_autowall;
			int prefer_hitbox;
			int prefer;
			int auto_stop_method;
			int multipoint_head;
			int multipoint_body;
			int multipoint_arms;
			int multipoint_neck;
			int multipoint_legs;
		};
		class RageShotGuns : public singleton< RageShotGuns > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int hitchance;
			int min_dmg;
			int min_dmg_autowall;
			int prefer_hitbox;
			int prefer;
			int auto_stop_method;
			int multipoint_head;
			int multipoint_body;
			int multipoint_arms;
			int multipoint_neck;
			int multipoint_legs;
		};
		class RageLMG : public singleton< RageLMG > {
		public:
			bool enable;
			bool hitboxes[5] = { false,false,false,false,false };
			int hitchance;
			int min_dmg;
			int min_dmg_autowall;
			int prefer_hitbox;
			int prefer;
			int auto_stop_method;
			int multipoint_head;
			int multipoint_body;
			int multipoint_arms;
			int multipoint_neck;
			int multipoint_legs;
		};



	};
	namespace AntiAim {
		class AA : public singleton< AA > {
		public:
			int pitch;
			int yawbase;
			int yaw;
			int jitterspeed;
			int jitterangle;
			int jitterwidth;
		};
		class AADesync : public singleton< AADesync > {
		public:
			int desyncbase;
			int desync;
			int angle;
			int desync_flip = -1;

		};
		class AAFakelag : public singleton< AAFakelag > {
		public:
			int fakelag;
			int fakelagticks;
			bool triggers[3] = { false,false,false };
			int fakelagtickstrigger;

		};
		class AAMisc : public singleton< AAMisc > {
		public:
			int menual_left = -1;
			int menual_right = -1;
			int menual_back = -1;
			int slowwwalk = -1;
			int slowwwalk_speed;
			bool hide_shots;

		};


	};
	namespace Misc {
		class MiscMain : public singleton< MiscMain > {
		public:
			int view_model_fov;
			int view_fov;
			bool viewmodelchanger;
			int viewmodel_x;
			int viewmodel_y;
			int viewmodel_z;
			bool auto_bunnyHop;
			bool autostrafer;
			bool auto_zeus;
			bool auto_knife;
			bool walk_bot;
			bool block_bot;
			bool buy_bot;
			int weapon_main;
			int weapon_sec;
			bool granade[4] = { false,false,false,false };
			bool other[4] = { false,false,false,false };
			bool anti_untrasted;
			bool name_stealer;
			bool log;
			bool clantag_spamer;
			bool infduck;
			int fakeduck = -1;
			int screen_x = 1920;
			int screen_y = 1080;

		};
		class RadioMain : public singleton< RadioMain > {
		public:
			float volume;
			int radio_station;
			bool Enable;

		};

	};
	namespace SkinChanger {
		class Knifes : public singleton< Knifes > {
		public:
			int knifemodel;
		};
		class Model : public singleton< Model > {
		public:
			int model_changer;
		};
	};
	namespace ConfigMenu {
		class ConfigMain : public singleton< ConfigMain > {
		public:
			float menu_color_r = 255, menu_color_g = 243, menu_color_b = 72, menu_color_a = 255;
			int slider1 = 40;
			int slider2 = 14;
			int slider3;
			int slider4;
			int slider5;
			int slider6;
		};

	};
}
