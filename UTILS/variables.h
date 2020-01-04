#pragma once

/*
global variables and other stuff that is probably too small to put in another 
file (and i'm too lazy to make files for all of them) would go in here.
Stuff like fonts and shit
*/

namespace global
{
	extern HWND csgo_hwnd;
	extern bool render_init;
	extern bool Should_Desync;
	extern int packets_choked;
	extern bool isautpwall;
	extern bool preservedelete;
	extern bool hasclantagged;
	extern bool DesyncEnabledSpecialChecks;
	extern bool Shot[65];
	//extern bool MissedShot;
	extern float flHurtTimeKill;
	extern float flHurtTimeKillmulti;
	extern bool Hited[65];
	extern int missedshots[65];
	extern int hit[65];
	extern 	bool  Hit2[65];
	extern bool should_buy;
	extern int fired[65];
	//extern int  MissedShots[65];
	extern int freestandingsite;

	extern std::string reason;
	extern bool can_shoot_someone;

	extern bool should_send_packet;
	extern bool m_round_changed;
	extern bool is_fakewalking;
	extern bool breaking_lby;
	extern int choke_amount;
	extern bool desync;
	extern float cheat_start_time;
	extern bool is_autorevolving;
	extern int killcount;
	extern int menual;
	extern int menualrlb;
	extern int autowalkkdmg;
	extern int desyncchoke;
	extern bool should_load_materials;

	extern Vector real_angles;
	extern Vector cmd_angles;
	extern Vector angles;
	extern Vector fake_angles;
	extern Vector fake_angelsdesync;
	extern Vector strafe_angle;
	extern Vector aim_point;

	extern bool should_choke_packets;
	extern bool modulate;
	extern int randomnumber;
	extern float flHurtTime;
	extern bool DisableAA;
	extern bool Aimbotting;
	extern int missed_shots[65];

	using msg_t = void(__cdecl*)(const char*, ...);
	extern msg_t		Msg;

	extern Vector FakePosition;
	extern int ground_tickz;
	extern bool CircleStraferActive;
	extern SDK::CUserCmd originalCMD;
}
namespace FONTS
{
	extern unsigned int menu_tab_font;
	extern unsigned int menu_checkbox_font;
	extern unsigned int menu_slider_font;
	extern unsigned int menu_slider_value_font;
	extern unsigned int menu_groupbox_font;
	extern unsigned int menu_combobox_name_font;
	extern unsigned int menu_combobox_value_font;
	extern unsigned int menu_window_font;
	extern unsigned int menu_separator_font;
	extern unsigned int menu_window_blurr_font;
	extern unsigned int menu_keybind_value_font;
	extern unsigned int menu_keybind_name_font;
	extern unsigned int menu_text_input_name_font;
	extern unsigned int menu_text_input_value_font;
	extern unsigned int menu_button_font;
	extern unsigned int menu_colorpicker_font;
	extern unsigned int menu_tool_tip_font;

	extern unsigned int visuals_esp_font;
	extern unsigned int visuals_xhair_font;
	extern unsigned int visuals_headshot_font;
	extern unsigned int visuals_side_font;
	extern unsigned int visuals_name_font;
	extern unsigned int welcome_font;
	extern unsigned int visuals_esp_font_de;
	extern unsigned int visuals_icon_font;
	extern unsigned int visuals_lby_font;
	extern unsigned int visuals_other_font;
	extern unsigned int visuals_grenade_pred_font;
	extern unsigned int  in_game_logging_font;
	extern unsigned int visuals_indector;
	extern unsigned int killindicator;
	extern unsigned int watermarkalyal;
	extern unsigned int watermarktextfps;
	extern unsigned int watermarkclock;
	extern unsigned int watermarkfps;
	bool ShouldReloadFonts();
	void InitFonts();
}
namespace SETTINGS
{
	class Sticker_t
	{
	public:
		int   iID = 0;
		float flWear = 0.f;
		float flScale = 1.f;
		int   iRotation = 0;
	};
	class Weapon_tTT
	{
	public:
		int SkinsWeapon;
		int SkinsKnife;
		int VremennyiWeapon;
		int VremennyiKnife;
		int Stikers1;
		int Stikers2;
		int Stikers3;
		int Stikers4;
		float ChangerWear = 0;
		int ChangerStatTrak = 0;
		char ChangerName[32] = "";
		bool ChangerEnabled;
	};
	class RagebotConfig
	{
	public:
		/* AIMBOT */
		bool aimbot_enabled = false;
		bool aimbot_prefer_baim_enabled = false;
		bool aimbot_baim_if_unplugged = false;
		bool aimbot_autostop_enabled = false;

		float aimbot_pointscale = 0.f;
		float aimbot_minimum_damage = 0.f;
		float aimbot_minimum_autowall_damage = 0.f;
		float aimbot_delay_shot = 0.f;

		int aimbot_accuracy_type = 0;
		float aimbot_accuracy = 0.f;

		bool aimbot_selected_multipoint_hitgroups[4];
		bool aimbot_selected_hitgroups[5];
		float aimbot_hitscan_pointscale = 0.f;
		float aimbot_bodyaim_if_x_damage = 100.f;

		bool fakelatency_enabled = false;
		float fakelatency_amount = 0.f;


		bool body_if_lethal = false;
	};
	extern RagebotConfig ragebot_configs;

	class CSettings
	{
	public:
		// returns true/false whether the function succeeds, usually returns false if file doesn't exist
		bool Save(int cfgnum);
		bool Load(int cfgnum);
		bool LoadLua(std::string file_name);

		bool Remove(std::string file_name);

		void CreateConfig(std::string name); // creates a blank config
		void RefreshConfigs();
		void ReadConfigs(LPCTSTR lpszFileName);
		std::vector<std::string> GetConfigs();
		std::vector<std::string> GetLuas();

		bool skinenabled;
		int Knife;

		Weapon_tTT weapons[520];

		struct wpnz
		{
			bool  EnabledStickers;
			bool  EnabledSkins;
			int   ChangerSkin;
			char  ChangerName[32] = "";
			int   ChangerStatTrak;
			int   ChangerSeed;
			float ChangerWear;
			Sticker_t Stickers[5];
		}Weapons[519];
		//visuals
			//skybox
				bool skybox_modulate_enable;
				bool skybox_color_modulate_enable;
				float skybox_color_modulate_color[4] = { 1.0f,1.0f,1.0f,1.0f };
				int skybox_chnager;
			//main
				//enemy
					bool enemy_box;
					float enemy_box_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool enemy_ammo_bar;
					float enemy_ammo_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool enemy_armor_bar;
					float enemy_armor_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool enemy_name;
					float enemy_name_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool enemy_health;
					bool enemy_skeleton;
					float enemy_skeleton_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					int enemy_weapon_combo;
					float enemy_weapon_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool enemy_money_flag;
					bool enemy_armor_flag;
					bool enemy_fakeduck_flag;
					bool enemy_zoom_flag;
				//team
					bool team_box;
					float team_box_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool team_ammo_bar;
					float team_ammo_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool team_armor_bar;
					float team_armor_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool team_name;
					float team_name_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool team_health;
					bool team_skeleton;
					float team_skeleton_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					int team_weapon_combo;
					float team_weapon_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool team_money_flag;
					bool team_armor_flag;
					bool team_fakeduck_flag;
					bool team_zoom_flag;
				//local
					bool local_box;
					float local_box_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool local_ammo_bar;
					float local_ammo_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool local_armor_bar;
					float local_armor_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool local_name;
					float local_name_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool local_health;
					bool local_skeleton;
					float local_skeleton_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					int local_weapon_combo;
					float local_weapon_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					bool local_money_flag;
					bool local_armor_flag;
					bool local_fakeduck_flag;
					bool local_zoom_flag;
				//end
			//chams
				//entity
					bool entity_chams_enable;
					bool entity_chams_invisble;
					bool entity_chams_wireframe;
					int entity_chams;
					float entity_visible_chams_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					float entity_invisible_chams_col[4] = { 1.0f,1.0f,1.0f,1.0f };
				//team
					bool team_chams_enable;
					bool team_chams_invisble;
					bool team_chams_wireframe;
					int team_chams;
					float team_visible_chams_col[4] = { 1.0f,1.0f,1.0f,1.0f };
					float team_invisible_chams_col[4] = { 1.0f,1.0f,1.0f,1.0f };
				//local
					bool fakelag_chams;
					float local_fakelag_chams_col[4] = { 1.0f,1.0f,1.0f,1.0f };

		//end









		//keybinds
		int overridekey;
		int circlestrafekey;
		int slowalkkey;
		int menual_left;
		int menual_right;
		int menual_backwards;
		int desync_switch;
		int fakeduck_key;
		int baim_key;
		//slider-ints
		int backtrack_ticks;
		//slider-floats
		int aspect_ratio_x;
		int aspect_ratio_y;
		//bools
		bool auto_revolver;
		bool baim_on_slowwalk;
		bool on_shot_aa;
		bool dropped_weapon_boxes;
		bool dropped_weapon_ammo;
		bool dropped_grenades;
		bool draw_lag_compensation;
		bool kill_effect;
		bool watermark;
		bool keystorks;
		bool aspect_ratio;
		bool armorbar;
		//colors
		float AmmoColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		float SkeletonColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		float DroppedWeaponColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		float DroppedWeaponBoxColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		float DroppedWeaponAmmoColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		float BulletImpactColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		float indicator_backround_color[4] = { 1.0f,1.0f,1.0f,1.0f };
		float indicator_choosed_color[4] = { 1.0f,1.0f,1.0f,1.0f };
		float armor_bar_color[4] = { 1.0f,1.0f,1.0f,1.0f };
		//combos
		int desync_cham_combo;
		int bullet_tracer_combo;
		int bullet_impacts_combo;
		int player_hitmarker_combo;
		int clan_tag_combo;
		//menu






		bool fix;
		bool backaimhead;
		int rage_lagcompensation_type=0;
		bool rage_lagcompensation;

				//=========================
		bool legit_bot_aimbot;
		float legit_bot_fov;
		float legit_bot_smooth;
		bool lbot_hitscan;
		bool rcsbool;
		//=========================


		bool hitscan_head;
		bool hitscan_neck;
		bool hitscan_chest;
		bool hitscan_pelvis;
		bool hitscan_arms;
		bool hitscan_legs;
		bool smartbaim;
		bool weapon_esp;

		int
			viewmodel_x = 6,
			viewmodel_y = 5,
			viewmodel_z = 4;

		bool Viewmodelchanger;

		int gloves;
		int skingloves;
		int newconfigs;
		bool desync_yaw;
		bool desync_yaw1;
		bool desync_yaw2;
		float glovewear;
		bool glovesenabled;
		bool rankchanger;
		int KnifeSkin;
		int rank_id = 9;
		int wins = 100;
		int level = 40;
		int friendly = 999;
		int teaching = 999;
		int leader = 999;
		int AK47Skin;
		int GalilSkin;
		int M4A1SSkin;
		bool zeusrange;
		int M4A4Skin;
		int AUGSkin;
		int FAMASSkin;
		int AWPSkin;
		int SSG08Skin;
		bool fakelatency_enabled;
		bool fixshit;
		float fakelatency_amount;
		char clantag_name[64] = { 0 };
		int SCAR20Skin;
		int P90Skin;
		int Mp7Skin;
		int NovaSkin;
		int UMP45Skin;
		int GlockSkin;
		int SawedSkin;
		int USPSkin;
		int MagSkin;
		bool aa_lbybreaker;
		bool aa_lbybreaker1;
		bool aa_lbybreaker2;
		float aa_lbybreaker_float;
		float aa_lbybreaker_float1;
		float aa_lbybreaker_float2;
		int XmSkin;
		int DeagleSkin;
		int DualSkin;
		int FiveSkin;
		int RevolverSkin;
		int Mac10Skin;
		int tec9Skin;
		int Cz75Skin;
		int NegevSkin;
		int M249Skin;
		int Mp9Skin;
		int P2000Skin;
		int BizonSkin;
		int Sg553Skin;
		int P250Skin;
		int G3sg1Skin;
		bool Enabled ;
		bool Visuals_BoxEnabled ;
		int Visuals_BoxType;
		int Visuals_HealthBarType;
		bool Visuals_Scoped ;
		bool Visuals_AmmoESP ;
		int Visuals_AmmoESPType ;

		bool Visuals_EspTeam ;
		bool Visuals_VisableOnly ;

		float BoxColorPickCTVIS[4] = { 1.0f,0.0f,0.0f,1.0f };
		float BoxColorPickCTINVIS[4] = { 1.0f,1.0f,0.0f,1.0f };
		float aa_ccolor[4] = { 1.0f,1.0f,0.0f,1.0f };

		float BoxColorPickTVIS[4] = { 0.0f,1.0f,0.0f,1.0f };
		float BoxColorPickTINVIS[4] = { 0.0f,1.0f,1.0f,1.0f };


		float LagCompHitboxes[4] = { 0.0f,1.0f,0.0f,1.0f };



		float SkeletonCTVIS[4] = { 1.0f,0.0f,0.0f,1.0f };
		float SkeletonCTINVIS[4] = { 1.0f,1.0f,0.0f,1.0f };

		float SkeletonTVIS[4] = { 0.0f,1.0f,0.0f,1.0f };
		float SkeletonTINVIS[4] = { 0.0f,1.0f,1.0f,1.0f };




		float CTChamsVisable[4] = { 1.0f,0.0f,0.0f,1.0f };

		float TChamsVisable[4] = { 1.0f,1.0f,0.0f,1.0f };


		float CTChamsInVisable[4] = { 0.0f,1.0f,0.0f,1.0f };
		float TChamsInVisable[4] = { 1.0f,1.0f,0.0f,1.0f };

		float beamtrace[4] = { 1.0f,0.0f,0.0f,1.0f };
		//int Hitsound;
		bool tp_bool;
		bool Monitor;
		bool Visuals_HealthBar;
		bool Visuals_Name;
		bool Visuals_ArmorBar = false;
		bool Visuals_Flashed = false;
		bool Visuals_Defuser = false;
		bool Visuals_Weapons = false;
		int Visuals_WeaponsType ;
		bool Visuals_AimLines = false;
		bool Visuals_Skeltal = false;
		bool Visuals_EngineRadar = false;
		bool Visuals_DrawLinesAA = false;
		bool Visuals_DroppedWeapons = false;
		bool Visuals_NoRecoil = false;
		bool Visuals_NoFlash = false;
		bool Visuals_NoSmoke = false;
		bool Visuals_NoSmokeVar = false;
		bool Visuals_NoScope = false;
		bool Visuals_NoZoom = false;
		bool Visuals_NoPostProcess = false;


		struct Visualsxd
		{
			bool Enabled = true;
			bool Visuals_BoxEnabled = true;
			int Visuals_BoxType = 1;
			bool Visuals_DrawDropp = false;
			int Visuals_DrawDropped = 1;
			int Visuals_HealthBarType = 1;
			bool Visuals_Scoped = false;
			bool Visuals_AmmoESP = true;
			bool Backtrackshow = false;
			int Visuals_AmmoESPType = 1;
			int Visuals_WeaponType = 1;
			bool Visuals_EspTeam = false;
			bool Visuals_VisableOnly = false;

			float BoxColorPickCTVIS[4] = { 1.0f,0.0f,0.0f,1.0f };
			float BoxColorPickCTINVIS[4] = { 1.0f,1.0f,0.0f,1.0f };


			float BoxColorPickTVIS[4] = { 0.0f,1.0f,0.0f,1.0f };
			float BoxColorPickTINVIS[4] = { 0.0f,1.0f,1.0f,1.0f };


			float LagCompHitboxes[4] = { 0.0f,1.0f,0.0f,1.0f };



			float SkeletonCTVIS[4] = { 1.0f,0.0f,0.0f,1.0f };
			float SkeletonCTINVIS[4] = { 1.0f,1.0f,0.0f,1.0f };

			float SkeletonTVIS[4] = { 0.0f,1.0f,0.0f,1.0f };
			float SkeletonTINVIS[4] = { 0.0f,1.0f,1.0f,1.0f };




			float CTChamsVisable[4] = { 1.0f,0.0f,0.0f,1.0f };

			float TChamsVisable[4] = { 1.0f,1.0f,0.0f,1.0f };


			float CTChamsInVisable[4] = { 0.0f,1.0f,0.0f,1.0f };
			float TChamsInVisable[4] = { 1.0f,1.0f,0.0f,1.0f };

			float beamtrace[4] = { 1.0f,0.0f,0.0f,1.0f };
			int Hitsound;
			bool tp_bool;
			bool Monitor;
			bool Visuals_HealthBar = true;
			bool Visuals_Name = true;
			int BoxHigh;
			int BoxWidth;
			bool Visuals_ArmorBar = false;
			bool Visuals_Flashed = false;
			bool Visuals_Defuser = false;
			bool Visuals_Weapons = false;
			int Visuals_WeaponsType = 0;
			//int Visuals_WeaponType = 1;
			bool Visuals_AimLines = false;
			bool Visuals_Skeltal = false;
			bool Visuals_AWCROSSHAIR = false;
			bool Visuals_EngineRadar = false;
			bool Visuals_DrawLinesAA = false;
			bool Visuals_DroppedWeapons = false;
			bool Visuals_NoRecoil = false;
			bool Visuals_NoFlash = false;
			bool Visuals_NoSmoke = false;
			bool Visuals_NoSmokeVar = false;
			bool Visuals_NoScope = false;
			bool Visuals_NoZoom = false;
			bool Visuals_NoPostProcess = false;


			bool Visuals_Chams = true;
			bool Visuals_ChamsTeam = false;
			bool Visuals_ChamsXQZ = false;
			int  Visuals_ChamsMaterial = 1;
			bool Visuals_GhostAngle = false;
			bool Visuals_ChamsGuns = false;
			bool Visuals_Crosshair = false;
			bool Visuals_CrosshairDynamic = false;


			float CTGlowVisable[4] = { 1.f,1.0f,1.0f,1.0f };
			float TGlowVisable[4] = { 1.f,1.0f,1.0f,1.0f };
			float CTGlowInVisable[4] = { 1.f,1.0f,1.0f,1.0f };
			float TGlowInVisable[4] = { 1.f,1.0f,1.0f,1.0f };

			bool Visuals_Hitmarker = false;
			bool Visuals_Spread = false;
			bool Visuals_GrenadePrediction = false;
			bool Visuals_DamageESP;
			bool Visuals_C4 = false;
			bool eventlog_enabled = false;
			float SpreadColor[4] = { 1.0f,0.0f,0.0f,1.0f };

			float DamageESPColor[4] = { 1.0f,0.0f,0.0f,1.0f };
			bool Vis_Glow = true;
			bool Vis_Glow_Team = false;
			bool Vis_Glow_Vis = false;


			bool Visuals_NightMode = false;
			bool Visuals_Asus = false;

			bool Visuals_DrawBeams = false;
			float Visuals_DrawBeamsDuration = 3.f;
			float Visuals_DrawBeamsSize = 3.f;

			bool Visuals_DrawCapsules = false;
			bool Visuals_DrawEventLog = false;
			float capsulecolor[4] = { 1.0,0.0f,0.0f,1.0f };
			float Visuals_DrawCapsulesDuration = 5.f;
			int Visuals_Spread_Type = 0;
			struct
			{
				bool Enabled = false;
				float Distance = 800.f;
				int type = 0;
				float Radius = 10.f;
				bool Animated = false;
				bool visonly = true;
			}SoundESP;
			struct
			{
				struct
				{
					bool	ExternalRadar;
					int		RadarStyle;
					float	RadarDistance;
					bool	RadarVisibleOnly;
					bool	RadarSmoke;
				}Radar;
				bool Monitor = false;
			}Panels;

			bool lbyIndicator = false;
			bool strelkiIndicator = false;
		}Visuals;

		bool lbyIndicator = false;
		bool strelkiIndicator = false;


		bool friendfire;
		bool bhop_bool;
		bool strafe_bool;
		bool esp_bool;
		int xhair_type;
		bool skeleton;
		bool aim_bool;
		int aim_type;
		bool aa_bool;
		int aa_pitch;
		int aa_type;
		int thirdperson_int;
		bool ragdoll_bool;
		int ragdoollol;
		int acc_type;
		bool playerhitmarker;
		bool nocoruchdelay;
		bool multiHitboxes[16] =
		{
			true, true, true, true, true,
			true, true, true, true, true,
			true, true, true, true, true,
			true
		};
		bool up_bool;
		bool misc_bool;
		int config_sel;
		int luas_sel;
		bool beam_bool;
		bool stop_bool;
		int fakewalk_lag;
		bool box_bool;
		bool name_bool;
		bool health_bool;
		bool info_bool;
		bool back_bool;
		bool lag_bool;
		int box_type;
		bool reverse_bool;
		bool multi_bool=true;
		bool fakefix_bool;
		bool angle_bool;
		bool tp_angle_bool;
		bool glow_bool;
		bool dist_bool;
		bool fov_bool;
		bool smoke_bool;
		bool scope_bool;
		bool predict_bool;
		bool fake_bool;
		int media_type;
		bool novis_bool;
		bool localglow_bool;
		bool duck_bool;
		bool money_bool;
		int delay_shot;
		int lag_type;
		bool cham_bool;
		bool resolve_bool;
		bool ammo_bool;
		bool spread_bool;


		float spreadcirclecol[4] = { 1.0f,0.0f,0.0f,1.0f };

		int stand_lag;
		int move_lag;
		int jump_lag;

		bool debug_bool;


		float vmodel_col[4] = { 1.0f,1.0f,0.0f,1.0f };

		float imodel_col[4] = { 0.0f,1.0f,0.0f,1.0f };
		
	
		
		float box_col[4] = { 1.0f,1.0f,0.0f,1.0f };
		float name_col[4] = { 1.0f,1.0f,0.0f,1.0f };
		float weapon_col[4] = { 1.0f,1.0f,0.0f,1.0f };
		float distance_col[4] = { 1.0f,1.0f,0.0f,1.0f };

		
		float localchams_col[4] = { 1.0f,0.0f,1.0f,1.0f };
		float grenadepredline_col[4] = { 1.0f,0.0f,1.0f,1.0f };
		int m4a4smodel;
		float bulletlocal_col[4] = { 1.0f,0.0f,1.0f,1.0f };
		float bulletenemy_col[4] = { 0.0f,1.0f,1.0f,1.0f };
		float bulletteam_col[4]{ 1.0f,1.0f,1.0f,1.0f };

		float bulletlife = 3.0f;
		float bulletsize = 2.0f;
	
		CColor menu_col = CColor(60, 60, 60);
		CColor checkbox_col = CColor(5, 135, 5);
		CColor slider_col = CColor(5, 135, 5);
		CColor tab_col = CColor(91, 91, 91);
		
		bool no_flash;

		float glow_col[4] = { 0.5f,0.5f,1.0f,1.0f };

		float transparency_amnt = .5f;

		float glowlocal_col[4] = { 0.3f,0.3f,0.9f,1.0f };
		
		float fov_col[4] = { 0.3f,0.3f,0.9f,1.0f };

		bool cradle;

		int chance_val;
		int damage_val = 1;
		int delta_val;
		float point_val;
		//bool hitsound;
		float body_val;
		bool misc_clantag;

		bool localesp;
		int localchams;
		float fov_val= 90;
		float viewfov_val= 68;

		bool fakechams;
		float fakechamscol[4] = { 1.0f,1.0f,1.0f,1.0f };


		bool flip_bool;
		int aa_side;

		bool legit_bool;
		int legit_key;
		bool rcs_bool;
		float legitfov_val;
		int legitbone_int;
		float rcsamount_min;
		float rcsamount_max;
		float legitaim_val;
		bool legittrigger_bool;
		int legittrigger_key;
		int legittrigger_hitchance;
		bool legittrigger_smoke_check;
		bool legittrigger_flash_check;
		int legitbothitboxes;
		bool legitbot_omouse;
		bool legitbot_silent;
		bool legitbot_smoke_check_aimbot;
		bool legitbot_wall_check_aimbot;
		bool legitbot_flash_check_aimbot;


		char thirdperson;
		int flip_int;

		bool glowenable;
		int glowstyle;
		bool glowlocal;
		int glowstylelocal;
		int hcmode;
		int autohc;
		int automd;
		int scouthc;
		int scouthcinair;
		int scoutmd;
		int pistolhc;
		int pistolmd;
		int revolverhc;
		int revolvermd;
		int otherhc;
		int othermd;
		int awphc;
		int awpmd;
		float automh;
		float automb;
		float scoutmh;
		float scoutmb;
		float pistolmh;
		float pistolmb;
		float revolvermh;
		float revolvermb;
		float othermh;
		float othermb;
		float awpmh;
		float awpmb;

		int hitmarker_val;

		int aa_mode;
		bool backjitter, backjitter1, backjitter2, backjitter3;

		int aa_real_type;
		int aa_real1_type;
		int aa_real2_type;
		int aa_real3_type;

		int aa_fake_type;
		int aa_fake1_type;
		int aa_fake2_type;

		int aa_pitch_type;
		int aa_pitch1_type;
		int aa_pitch2_type;
		int scope_type;
		int aa_pitch3_type;

		int aa_realadditive_val;
		float aa_fakeadditive_val;

		int aa_realadditive1_val;
		float aa_fakeadditive1_val;
		int delta1_val;

		int aa_realadditive2_val;
		float aa_fakeadditive2_val;
		int delta2_val;

		int aa_realadditive3_val;
		int delta3_val;

		int spinangle;
		int spinspeed;

		int spinangle1;
		int spinspeed1;
		int spinspeed3;

		int spinangle2;
		int spinspeed2;
		int spinangle3;

		float spinanglefake;
		float spinspeedfake;

		float spinanglefake1;
		float spinspeedfake1;

		float spinanglefake2;
		float spinspeedfake2;

		bool lbyflickup;
		bool lbyflickup1;
		bool lbyflickup2;

		bool aa_fakeangchams_bool;

		int chamstype;
		float fov_time;
		bool rifk_arrow;

		int glowteamselection;
		bool glowteam;

		int espteamselection;
		int espteamcolourselection;
		bool boxteam;
		bool weaponteam;
		bool flagsteam;
		bool healthteam;
		bool moneyteam;
		bool ammoteam;
		bool arrowteam;
		float boxteam_col[4] = { 1.0f,0.5f,0.5f,1.0f };
		float nameteam_col[4] = { 1.0f,0.5f,0.5f,1.0f };
		float weaponteam_col[4] = { 1.0f,0.5f,0.5f,1.0f };
		float  arrowteam_col[4] = { 1.0f,0.5f,0.5f,1.0f };


		float teamvis_color[4] = { 1.0f,0.5f,0.5f,1.0f };
		float teaminvis_color[4] = { 1.0f,1.0f,0.5f,1.0f };


	
		


		float teamglow_color[4] = { 1.0f,1.0f,0.5f,1.0f };

		bool matpostprocessenable;
		bool removescoping;
		bool fixscopesens;
		bool forcecrosshair;

		bool killfeed;
		bool full_bright;

		bool stop_flip;
		bool chamsmetallic;
		int autostopmethod;
		int overridemethod;
		bool overridething;
		bool overrideenable;
		bool lbyenable;
		float circlstraferetract;
		int slowwalkspeed = 1;
		bool fakewalk;
		float daytimevalue = 100;
		bool footsteps;

		bool test;

		float circlemin;
		float circlemax;
		float circlevel;
		float circlenormalizemultiplier;

		bool skinchangerenable;

		int knifeToUse;
		int bayonetID, karambitID, flipID, gutID, m9ID, huntsmanID;
		int gloveToUse;
		int bloodhoundID, driverID, handwrapsID, motoID, specialistID, sportID, hydraID;

		int uspID, p2000ID, glockID, dualberettaID, p250ID, fivesevenID, tech9ID, r8ID, deagleID;
		int novaID, xm1014ID, mag7ID, sawedoffID, m249ID, negevID;
		int mp9ID, mac10ID, mp7ID, ump45ID, p90ID, ppbizonID;
		int famasID, galilID, ak47ID, m4a4ID, m4a1sID, ssg08ID, augID, sg553ID, awpID, scar20ID, g3sg1ID;

//		//--------------------------------------------\\
//	   ||			(most of all the) new shit         ||
//		\\--------------------------------------------//

		int hitsound2;
		bool slowalk;
		bool impacts;
		bool render_mode;
		int hitsound;
		bool autoscope;
		bool radio_paused = false;
		int radio_selected = 0;

		bool CUSTOMMODEL;
		bool autozeus_bool, autoknife_bool;
		int customct;
		int customtt;
		int m4a4model;
		int customlocal;

		bool forcehair;
		bool SkeetBar = true;
		bool baim_if_unplugged;
		bool preferbaim;
		bool baimiflethal;
		float baim_if_x_damage;
		bool hitmarkerscreen;
		float tpfov_val = 100;

		bool skinchanger_enabled;
		int skinchanger_gloves;
		int skinchanger_gloves_skin;
		int skinchanger_knifes;
		int currentWeapon;

		int chamsteamselection;
		bool chamsteam;
		bool backtrack_chams;
		int chamstypeselfcolor;
		float btvis_col[4] = { 1.0f,1.0f,0.5f,1.0f };
		float dmg_ccolor[4] = { 1.0f,1.0f,0.5f,1.0f };
		float rainbowenemy;
		float rainbowself;
		float rainbowteam;
		bool chams_type;
		int chamstypeteam;
		int chamstypebacktrack;
		bool cvarsoff = true;
		struct
		{
			bool ChangerEnabled = false;
			int ChangerSkin = 0;
			char ChangerName[32] = "";
			bool stat;
			int statamount;
			bool customstat;
			int customstatamount;
			char Name;
		} skinchanger_weapons[519];

		bool indicator_bool;
		bool indicator_bool2;
		float aa_arrows_width = 7, aa_arrow_height;
		//END
		//END
		//NEW DME CHAMS
		bool weaponchams;
		bool handchams;
		bool handwireframe;
		bool weaponwireframe;
		//Color Raimbow
		bool weaponraimbow;
		bool handraimbow;
		// Colors
		float weaponcol[4] = { 1.0f,1.0f,1.0f,1.0f };
		float handcol[4] = { 1.0f,1.0f,1.0f,1.0f };
		//int
		int handchamgb;
		int weaponchamghb;

		bool antiaim_arrow;
		bool showlbydelta;
		bool nameteam;
		bool NoFlash;
		bool night_bool = 100;
		bool weap_bool;
		bool weap_ammo;
		int arrowtrypes;
		bool lby_bar;
		bool dmg_bool;
		int arroewstyle = 1;
		int chamstypeself;
		int chamstypebacktrackcolor, chamstypeteamcolor, chamstypecolor;
		//bool chams_type;
		bool antiaim_enable;
		float aa_arrows_height = 15;
		//bool autoknife_bool;
		//bool autoscope;
		//int arrowtrypes;
		float randoslider;
		float arrowsizesh = 16;
		int arrowradius;
		float cSound[4] = { 1.0f,1.0f,0.5f,1.0f };
		float fontsizedmg_bool = 40;
		bool bSound;
		float asus_props_value = 100;
		float slider1 = 2, slider2 = 16, slider3, slider4 = 16, slider5;
		//newregbot

	private:
	}; extern CSettings settings;
}

extern bool using_fake_angles[65];
extern bool full_choke;
extern bool is_shooting;

extern bool in_tp;
extern bool fake_walk;

extern int resolve_type[65];

extern int target;
extern int shots_fired[65];
extern int shots_hit[65];
extern int shots_missed[65];
extern int backtrack_missed[65];

extern bool didShot;
extern bool didMiss;

extern float tick_to_back[65];
extern float lby_to_back[65];
extern bool backtrack_tick[65];
extern bool in_slowalk;
extern bool flipaa;

extern float lby_delta;
extern float update_time[65];
extern float walking_time[65];

extern float local_update;

extern int hitmarker_time;
extern int random_number;

extern bool menu_hide;

extern int oldest_tick[65];
extern float compensate[65][12];
extern Vector backtrack_hitbox[65][20][12];
extern float backtrack_simtime[65][12];