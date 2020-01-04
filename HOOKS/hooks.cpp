#include "..\includes.h"
#include "hooks.h"
#include "../SDK/imdlcache.h"
#include "../UTILS/interfaces.h"
#include "../UTILS/offsets.h"
#include "../UTILS/NetvarHookManager.h"
#include "../UTILS/render.h"
#include "../FEATURES/NewEventLog.h"
#include "../SDK/CInput.h"
#include "../SDK/IClient.h"
#include "../SDK/CPanel.h"
#include "../SDK/ConVar.h"
#include "../SDK/CGlowObjectManager.h"
#include "../SDK/IEngine.h"
#include "../SDK/CTrace.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/RecvData.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/ModelInfo.h"
#include "../SDK/ModelRender.h"
#include "../FEATURES/Flashlight.h"
#include "../SDK/RenderView.h"
#include "../SDK/CTrace.h"
#include "../AWHitmarkers.h"
#include "../SDK/CViewSetup.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CPrediction.h"
#include "../FEATURES/Movement.h"
#include "../FEATURES/Visuals.h"
#include "../FEATURES/AntiAim.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/New/Backtrackingcpp.h"
#include "../FEATURES/FakeWalk.h"
#include "../cheats/exploits/exploits.h"
#include "../SDK/IMemAlloc.h"
#include "../FEATURES/FakeLag.h"
#include "../FEATURES/EnginePred.h"
#include "../FEATURES/GrenadePrediction.h"
#include "../SDK/NetChannel.h"
#include "../FEATURES/Flashlight.h"
#include "../FEATURES/GloveChanger.h"
#include "../FEATURES/SkinChanger.h"
#include "../SDK/CBaseAnimating.h"
#include "../SDK/CBaseAnimState.h"
#include "../FEATURES/LegitBot.h"
#include "..\FEATURES\custom_font.h"
#include "../shit.h"
#include "ColorVar.h"
#include "bytearray.h"
#include <intrin.h>
#include "../SDK/CClinetstate.h"
#include "../UTILS/qangle.h"
#include "convar_mkurtl.h"
#include "../BASS/bass.h"
#include "../BASS/API.h"
#include "../SDK/imdlcache.h"
#include "../FEATURES/New/NewResolver.h"
#include "../radionstuff.h"
#include "../XorStr.hpp"
#include "menu\Menu.h"
#include "NewMenu\Menu.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

CMenu* menu = new CMenu();

typedef MDLHandle_t(__thiscall* iFindMdl)(void*, char*);
iFindMdl oFindMDL;
MDLHandle_t __fastcall hkFindMDL(void*, void*, char*);

namespace INIT
{
	HMODULE Dll;
	HWND Window;
	WNDPROC OldWindow;
}

namespace MenuTabs
{
	void Tab1();
	void Tab2();
	void Tab3();
	void Tab4();
	void Tab5();
}

enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	//	MATERIAL_VAR_UNUSED					  = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17), // OBSOLETE
	//	MATERIAL_VAR_UNUSED					  = (1 << 18),
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19), // OBSOLETE
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23), // OBSOLETE
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25), // OBSOLETE
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
	MATERIAL_VAR_VERTEXFOG = (1 << 31),

	// NOTE: Only add flags here that either should be read from
	// .vmts or can be set directly from client code. Other, internal
	// flags should to into the flag enum in IMaterialInternal.h
};
template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(UTILS::FindSignature("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(UTILS::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}
int fliuP = 0;
static bool menu_open = false;
static bool tabsOnTop = true;
static bool d3d_init = false;
bool oof = false;
bool _done = false;
bool PressedKeys[256] = {};
const char* merixids[] =
{
	"1","2","3","4","5","6", "7", "8", "9",
	"Q","W","E","R","T","Y","U","I","O","P",
	"A","S","D","F","G","H","J","K","L",
	"Z","X","C","V","B","N","M",".","\\","|", "/","}","{","[","]",
	"<",">","?","'"
};
static char ConfigNamexd[64] = { 0 };
static char clantag_name[64] = { 0 };
static bool should_reset = false;

Vector fakedesync;
//--- Other Globally Used Variables ---///
static bool tick = false;
static int ground_tick;
static bool hold;
static bool omg;
static bool desync_switch;
Vector legitaa;
Vector vecAimPunch, vecViewPunch;
Vector* pAimPunch = nullptr;
Vector* pViewPunch = nullptr;
int freecamx;
int freecamy;
matrix3x4_t fakelagmatrix[128];
int freecamz;
//--- Declare Signatures and Patterns Here ---///


static auto CAM_THINK = UTILS::FindSignature("client_panorama.dll", "85 C0 75 30 38 86");
static auto linegoesthrusmoke = UTILS::FindPattern("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");


template<class Type>
static Type HookManual(uintptr_t* vftable, uint32_t index, Type fnNew) {
	DWORD Dummy;
	Type fnOld = (Type)vftable[index];
	VirtualProtect((void*)(vftable + index * 0x4), 0x4, PAGE_EXECUTE_READWRITE, &Dummy);
	vftable[index] = (uintptr_t)fnNew;
	VirtualProtect((void*)(vftable + index * 0x4), 0x4, Dummy, &Dummy);
	return fnOld;
}

struct player_anims
{
	SDK::CAnimationLayer m_layers[15];
	std::array< float, 24 > pose_params;
	SDK::CAnimationLayer* animstate;
};
bool           shoot[65];
bool           hit[65];
SDK::CAnimationLayer* animstate;
std::array< bool, 64 > on_ground{ false };
std::array< bool, 64 > last_on_ground{ false };
std::array< Vector, 64 > speed{ Vector() };
std::array< Vector, 64 > last_speed{ Vector() };
player_anims player_data[64];


void re_work(int stage) {

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());


	if (!local_player || !local_player->GetAnimState())
		return;

	if (stage == ClientFrameStage_t::FRAME_START)
		local_player->SetAbsAngles(Vector(0.f, local_player->GetAnimState()->m_flGoalFeetYaw, 0.f));


	for (int i = 1; i <= INTERFACES::Globals->maxclients; i++) {
		SDK::CBaseEntity * entity = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntity(i);


		static auto set_interpolation_flags = [](SDK::CBaseEntity* e, int flag) {
			const auto var_map = (uintptr_t)e + 36;
			const auto sz_var_map = *(int*)(var_map + 20);

			for (auto index = 0; index < sz_var_map; index++)
				*(uintptr_t*)((*(uintptr_t*)var_map) + index * 12) = flag;
		};

		if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END)
			set_interpolation_flags(entity, 0);


	}
}

float NormalizeVector(Vector& v) {
	float l = v.Length();
	if (l != 0.0f) {
		v /= l;
	}
	else {
		v.x = v.y = v.z = 0.0f;
	}
	return l;
}

//--- Tick Counting ---//
void ground_ticks()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (local_player->GetFlags() & FL_ONGROUND)
		ground_tick++;
	else
		ground_tick = 0;
}

namespace HOOKS
{
	CreateMoveFn original_create_move;
	PaintTraverseFn original_paint_traverse;
	PaintFn original_paint;
	FrameStageNotifyFn original_frame_stage_notify;
	DrawModelExecuteFn original_draw_model_execute;
	SceneEndFn original_scene_end;
	TraceRayFn original_trace_ray;
	OverrideViewFn original_override_view;
	SetupBonesFn original_setup_bones;
	RenderViewFn original_render_view;
	SvCheatsGetBoolFn original_get_bool;
	GetViewmodelFOVFn original_viewmodel_fov;
	SendDatagramFn original_send_datagram = 0;
	FireBullets_t o_FireBullets;
	ScreenAspectRatio original_screen_aspect_ratio;
	DoPostScreenEffects_t orginal_post_screen_effect;

	vfunc_hook fireevent;
	vfunc_hook directz;
	vfunc_hook HookMethod;
	vfunc_hook screen_aspect;

	VMT::VMTHookManager iclient_hook_manager;
	VMT::VMTHookManager panel_hook_manager;
	VMT::VMTHookManager paint_hook_manager;
	VMT::VMTHookManager model_render_hook_manager;
	VMT::VMTHookManager scene_end_hook_manager;
	VMT::VMTHookManager render_view_hook_manager;
	VMT::VMTHookManager trace_hook_manager;
	VMT::VMTHookManager net_channel_hook_manager;
	VMT::VMTHookManager override_view_hook_manager;
	VMT::VMTHookManager input_table_manager;
	VMT::VMTHookManager get_bool_manager;
	VMT::VMTHookManager setup_bones_hook_manager;
	VMT::VMTHookManager screen_aspect_ratio;

	std::string sPanel = ("FocusOverlayPanel");
	void AutoRevolver(SDK::CUserCmd* cmd) {
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player || local_player->GetHealth() <= 0)
			return;
		auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
		if (!weapon || weapon->GetItemDefenitionIndex() != SDK::WEAPON_REVOLVER)
			return;
		static float delay = 0.f;
		if (delay < 0.20f)
		{
			delay += INTERFACES::Globals->interval_per_tick;
			cmd->buttons |= IN_ATTACK;
		}
		else
			delay = 0.f;
	}


	template<class T, class U>
	T fine(T in, U low, U high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}
	struct hud_weapons_t {
		std::int32_t* get_weapon_count() {
			return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
		}
	};
	void PreserveKillFeed()
	{
		static void(__thiscall *ClearDeathNotices)(DWORD);
		static DWORD* deathNotice;
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		if (local_player && INTERFACES::Engine->IsInGame() && INTERFACES::Engine->IsConnected())
		{
			if (local_player) {
				if (!deathNotice) deathNotice = FindHudElement<DWORD>("CCSGO_HudDeathNotice");
				if (deathNotice) {
					float* localDeathNotice = (float*)((DWORD)deathNotice + 0x50);
					if (localDeathNotice) *localDeathNotice = SETTINGS::settings.killfeed ? FLT_MAX : 1.5f;
					if (global::preservedelete && deathNotice - 20) {
						if (!ClearDeathNotices) ClearDeathNotices = (void(__thiscall*)(DWORD))UTILS::FindSignature("client_panorama.dll", "55 8B EC 83 EC 0C 53 56 8B 71 58");
						if (ClearDeathNotices) { ClearDeathNotices(((DWORD)deathNotice - 20)); global::preservedelete = false; }

					}
				}
			}
		}
	}


	void KnifeApplyCallbk()
	{

		static auto clear_hud_weapon_icon_fn =
			reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
				UTILS::FindSignature("client_panorama.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC"));

		auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

		auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);
		if (hud_weapons == nullptr)
			return;

		if (!*hud_weapons->get_weapon_count())
			return;

		for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
			i = clear_hud_weapon_icon_fn(hud_weapons, i);

		static SDK::ConVar* Meme = INTERFACES::cvar->FindVar("cl_fullupdate");
		Meme->nFlags &= ~FCVAR_CHEAT;
		INTERFACES::Engine->ClientCmd_Unrestricted("cl_fullupdate");

	}

	bool __fastcall HookedSetupBones(void* ECX, void* EDX, matrix3x4_t* matrix, int num_bones, int bone_mask, float current_time)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		auto animstate = local_player->GetAnimState();
		auto old_absorigin = local_player->GetAbsOrigin();
		if (!ECX || !matrix || !local_player || local_player->GetHealth() <= 0 || !animstate)
			return original_setup_bones(ECX, matrix, num_bones, bone_mask, current_time);

		static auto last_networked_origin = Vector(0, 0, 0);
		if (global::should_send_packet)
			last_networked_origin = local_player->GetVecOrigin();

		return original_setup_bones(ECX, matrix, num_bones, bone_mask, current_time);
	}
	void setup_bones()
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player || local_player->GetHealth() <= 0) return;

		auto renderable = reinterpret_cast<void*>(uintptr_t(local_player) + 0x4);
		if (!renderable) return;

		static bool is_init = false;
		if (!is_init)
		{
			is_init = true;
			HOOKS::setup_bones_hook_manager.Init(renderable);
			HOOKS::original_setup_bones = HOOKS::setup_bones_hook_manager.HookFunction<HOOKS::SetupBonesFn>(13, HookedSetupBones);
		}
	}

	int __fastcall HookedSendDatagram(SDK::NetChannel* ecx, void* edx, void* data)
	{
	
		if (!ecx)
			return 0;

		if (data || !INTERFACES::Engine->IsInGame() || !Config::Ragebot::RageGlobal::get().extened_backtrack_ticks)
			return original_send_datagram(ecx, data);

		backtracking.netchan = ecx;

		int in_reliable_state = ecx->m_nInReliableState;
		int in_sequence_num = ecx->m_nInSequenceNr;

		auto lag_s = Config::Ragebot::RageGlobal::get().extend_time / 1000.f;
		auto lag_delta = lag_s - INTERFACES::Engine->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);

		backtracking.AddLatency(ecx, lag_delta);
		int ret = original_send_datagram(ecx, data);

		ecx->m_nInReliableState = in_reliable_state;
		ecx->m_nInSequenceNr = in_sequence_num;

		return ret;
	}
	void buybot() {
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		bool is_ct = local_player->GetTeam() == 3;
		if (global::should_buy){
			switch (Config::Misc::MiscMain::get().weapon_main)
			{
			case 0: is_ct ? (INTERFACES::Engine->ExecuteClientCmd("buy scar20;")) : (INTERFACES::Engine->ExecuteClientCmd("buy g3sg1;"));
				break;
			case 1: INTERFACES::Engine->ExecuteClientCmd("buy ssg08;");
				break;
			case 2: INTERFACES::Engine->ExecuteClientCmd("buy awp;");
				break;

			}
			switch (Config::Misc::MiscMain::get().weapon_sec)
			{
			case 0: INTERFACES::Engine->ExecuteClientCmd("buy elite;");
				break;
			case 1: INTERFACES::Engine->ExecuteClientCmd("buy deagle;buy revolver");
				break;

			}

			if(Config::Misc::MiscMain::get().granade[0] == true)
				INTERFACES::Engine->ExecuteClientCmd("buy flashbang;");
			if (Config::Misc::MiscMain::get().granade[1] == true)
				INTERFACES::Engine->ExecuteClientCmd("buy smokegrenade;");
			if (Config::Misc::MiscMain::get().granade[2] == true)
				INTERFACES::Engine->ExecuteClientCmd("buy hegrenade;");
			if (Config::Misc::MiscMain::get().granade[3] == true) {
				INTERFACES::Engine->ExecuteClientCmd("buy molotov; buy incgrenade;");
			}
			if (Config::Misc::MiscMain::get().other[0] == true)
				INTERFACES::Engine->ExecuteClientCmd("buy vest;");
			if (Config::Misc::MiscMain::get().other[1] == true)
				INTERFACES::Engine->ExecuteClientCmd("buy vesthelm;");
			if (Config::Misc::MiscMain::get().other[2] == true)
				INTERFACES::Engine->ExecuteClientCmd("buy taser;");
			if (Config::Misc::MiscMain::get().other[3] == true)
				INTERFACES::Engine->ExecuteClientCmd("buy defuser;");
		global::should_buy = false;
	}

	}
	void set_name(const char* name)
	{
		SDK::ConVar* nameConvar = INTERFACES::cvar->FindVar(("name"));
		*(int*)((DWORD)&nameConvar->fnChangeCallback + 0xC) = NULL;
		nameConvar->SetValueChar(name);
	}
	void namespam()
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player)
			return;
		static clock_t start_t = clock();
		double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
		if (timeSoFar < .5)
			return;
		const char* result;
		std::vector <std::string> names;
		if (INTERFACES::Engine->IsInGame() && INTERFACES::Engine->IsConnected()) {
			for (int i = 1; i < 64; i++)
			{
				SDK::CBaseEntity *entity = INTERFACES::ClientEntityList->GetClientEntity(i);

				SDK::player_info_t pInfo;

				if (entity && local_player->GetTeam() == entity->GetTeam())
				{
					SDK::ClientClass* cClass = (SDK::ClientClass*)entity->GetClientClass();

					if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
					{
						if (INTERFACES::Engine->GetPlayerInfo(i, &pInfo))
						{
							if (!strstr(pInfo.szName, "GOTV"))
								names.push_back(pInfo.szName);
						}
					}
				}
			}
		}

		set_name("\n\xAD\xAD\xAD");
		int randomIndex = rand() % names.size();
		char buffer[128];
		sprintf_s(buffer, "%s ", names[randomIndex].c_str());
		result = buffer;



		set_name(result);
		start_t = clock();

	}
	bool __stdcall HookedCreateMove(float sample_input_frametime, SDK::CUserCmd* cmd)
	{
		if (!cmd || cmd->command_number == 0)
			return false;

		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame() && INTERFACES::Engine->GetNetChannel() && !original_send_datagram)
		{
			net_channel_hook_manager.Init(INTERFACES::Engine->GetNetChannel());
			original_send_datagram = reinterpret_cast<SendDatagramFn>(net_channel_hook_manager.HookFunction<SendDatagramFn>(46, HookedSendDatagram));
		}

		backtracking.UpdateIncomingSequences();

		auto nci = INTERFACES::Engine->GetNetChannel();
		bool bSendpacket = false;

		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player)
			return false;
		uintptr_t* FPointer; __asm { MOV FPointer, EBP }
		byte* SendPacket = (byte*)(*FPointer - 0x1C);


		global::originalCMD = *cmd;

		
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			GrenadePrediction::get().Tick(cmd->buttons);

			if (global::modulate) {
				SDK::ConVar* staticdrop = INTERFACES::cvar->FindVar("r_DrawSpecificStaticProp");
				staticdrop->SetValue(0);
				SDK::ConVar* NightSkybox1 = INTERFACES::cvar->FindVar("sv_skyname");
				*(float*)((DWORD)&NightSkybox1->fnChangeCallback + 0xC) = NULL;
				for (SDK::MaterialHandle_t i = INTERFACES::MaterialSystem->FirstMaterial(); i != INTERFACES::MaterialSystem->InvalidMaterial(); i = INTERFACES::MaterialSystem->NextMaterial(i))
				{
					SDK::IMaterial *pMaterial = INTERFACES::MaterialSystem->GetMaterial(i);

					if (!pMaterial)
						continue;

					float asus_props = Config::Visuals::Main::VisualsEffects::get().AsusProps / 100;
					float asus_walls = Config::Visuals::Main::VisualsEffects::get().AsusWalls / 100;
					float night = Config::Visuals::Main::VisualsEffects::get().Brightness / 100;
					float red = Config::Visuals::Main::VisualsEffects::get().world_color_r / 100;
					float green = Config::Visuals::Main::VisualsEffects::get().world_color_g / 100;
					float blue = Config::Visuals::Main::VisualsEffects::get().world_color_b / 100;

					switch (Config::Visuals::Main::VisualsEffects::get().Skybox)
					{
					case 1:
						NightSkybox1->SetValue("cs_baggage_skybox"); break;
					case 2:
						NightSkybox1->SetValue("cs_tibet"); break;
					case 3:
						NightSkybox1->SetValue("clearsky"); break;
					case 4:
						NightSkybox1->SetValue("clearsky_hdr"); break;
					case 5:
						NightSkybox1->SetValue("embassy"); break;
					case 6:
						NightSkybox1->SetValue("italy"); break;
					case 7:
						NightSkybox1->SetValue("jungle"); break;
					case 8:
						NightSkybox1->SetValue("nukeblank"); break;
					case 9:
						NightSkybox1->SetValue("office"); break;
					case 10:
						NightSkybox1->SetValue("sky_cs15_daylight01_hdr"); break;
					case 11:
						NightSkybox1->SetValue("sky_cs15_daylight02_hdr"); break;
					case 12:
						NightSkybox1->SetValue("sky_cs15_daylight03_hdr"); break;
					case 13:
						NightSkybox1->SetValue("sky_cs15_daylight04_hdr"); break;
					case 14:
						NightSkybox1->SetValue("sky_csgo_cloudy01"); break;
					case 15:
						NightSkybox1->SetValue("sky_csgo_night02"); break;
					case 16:
						NightSkybox1->SetValue("sky_csgo_night02b"); break;
					case 17:
						NightSkybox1->SetValue("sky_csgo_night_flat"); break;
					case 18:
						NightSkybox1->SetValue("sky_day02_05_hdr"); break;
					case 19:
						NightSkybox1->SetValue("sky_day02_05"); break;
					case 20:
						NightSkybox1->SetValue("sky_dust"); break;
					case 21:
						NightSkybox1->SetValue("sky_l4d_rural02_ldr"); break;
					case 22:
						NightSkybox1->SetValue("vertigo_hdr"); break;
					case 23:
						NightSkybox1->SetValue("vertigoblue_hdr"); break;
					case 24:
						NightSkybox1->SetValue("vertigo"); break;
					case 25:
						NightSkybox1->SetValue("vietnam"); break;
					}
					if (strstr(pMaterial->GetTextureGroupName(), ("SkyBox")))
					{
						pMaterial->ColorModulate(red, green, blue);
					}

					if (!strcmp(pMaterial->GetTextureGroupName(), "World textures") && Config::Visuals::Main::VisualsEffects::get().AsusProps < 1)
					{
						pMaterial->ColorModulate(night, night, night);
					}
					if (!strcmp(pMaterial->GetTextureGroupName(), "World textures") && Config::Visuals::Main::VisualsEffects::get().AsusProps > 0)
					{
						pMaterial->AlphaModulate(asus_props);
						pMaterial->ColorModulate(night, night, night);
					}
					if (!strcmp(pMaterial->GetTextureGroupName(), "StaticProp textures"))
					{
						pMaterial->AlphaModulate(asus_walls);
						pMaterial->ColorModulate(night, night, night);
					}

				}
				global::modulate = false;
			}

			slowwalk->DoSlowWalk(cmd);

			if (GetAsyncKeyState(VK_F1))
			{
				cmd->tick_count = INT_MAX;
				cmd->command_number = INT_MAX;
			}


			if (Config::Misc::MiscMain::get().infduck)
				exploits->duck(cmd);

			if(Config::Legitbot::LegitGlobal::get().enable)
				legit->Run(cmd);


			if (Config::Misc::MiscMain::get().auto_bunnyHop)
				movement->bunnyhop(cmd);




			if (GetAsyncKeyState(Config::Misc::MiscMain::get().fakeduck) && local_player->IsAlive() && local_player->GetFlags() & FL_ONGROUND)
			{
				bSendpacket = (nci->m_nChokedPackets > 13);
				local_player->GetAnimState()->m_fDuckAmount = 1.f;
				((nci->m_nChokedPackets >= 7) || bSendpacket || INTERFACES::clientstate->next_cmd_time <= 0.1f) ? cmd->buttons |= IN_DUCK : cmd->buttons &= ~IN_DUCK;
			}
			if (Config::Misc::MiscMain::get().buy_bot)
				buybot();


			if (Config::Misc::MiscMain::get().autostrafer)
				movement->autostrafer(cmd);
			if (Config::Misc::MiscMain::get().anti_untrasted) {
				Config::Visuals::Main::VisualsEffects::get().FullBright = false;
				Config::Visuals::Main::VisualsEffects::get().DisablePostProcessing = false;
			}
			if (Config::Misc::MiscMain::get().name_stealer)
				namespam();
			if (Config::Legitbot::LegitGlobal::get().legit_aa && !(cmd->buttons & IN_ATTACK)) {
				if(*SendPacket){
					cmd->viewangles.y -= global::desync ? local_player->MaxDelta(local_player) : -local_player->MaxDelta(local_player);
					legitaa.y = global::desync ? local_player->MaxDelta(local_player) : -local_player->MaxDelta(local_player);
				}

			}

			Vector ViewAngle = cmd->viewangles;
			Vector view;
			INTERFACES::Engine->GetViewAngles(view);
			global::real_angles.y = view.y;


			prediction->run_prediction(cmd);


			aimbot->run_aimbot(cmd);

			if (Config::Ragebot::RageGlobal::get().resolver)
				resolver.DoCM();


			prediction->end_prediction(cmd);

			fakelag->do_fakelag(cmd);

		
			if (Config::Misc::MiscMain::get().clantag_spamer) {
				static std::string cur_clantag = " $ stellarcheat.club $ ";
				int iLastTime;
				if (int(INTERFACES::Globals->curtime) != iLastTime)
				{
					global::hasclantagged = true;

					static int old_time, i = 0;

					if (i > 32)
					{
						auto marquee = [](std::string& panicova_zlomena_noha) -> void
						{
							std::string temp_string = panicova_zlomena_noha;
							panicova_zlomena_noha.erase(0, 1);
							panicova_zlomena_noha += temp_string[0];
						};

						marquee(cur_clantag);

						auto setclantag = [](const char* tag) -> void
						{
							static auto ClanTagOffset = UTILS::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", "xxxxxxxxx");
							if (ClanTagOffset)
							{
								auto tag_ = std::string(tag);
								if (strlen(tag) > 0) {
									auto newline = tag_.find("\\n");
									auto tab = tag_.find("\\t");
									if (newline != std::string::npos) {
										tag_.replace(newline, newline + 2, "\n");
									}
									if (tab != std::string::npos) {
										tag_.replace(tab, tab + 2, "\t");
									}
								}
								static auto dankesttSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(ClanTagOffset);
								dankesttSetClanTag(tag_.data(), tag_.data());
							}
						};
						setclantag(cur_clantag.c_str());

						i = 0;

					}
					else
					{
						i++;
					}
				}

				iLastTime = int(INTERFACES::Globals->curtime);

			}


			if (GetAsyncKeyState(VK_RCONTROL)) {

				if (cmd->command_number % 3)
					cmd->tick_count = INT_MAX;
			}


		}
			
		global::cmd_angles = cmd->viewangles;


		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame()) {

			antiaim->do_antiaim(cmd);
			antiaim->fix_movement(cmd);

			auto animstate = local_player->GetAnimState();
			if (animstate->m_bInHitGroundAnimation)
				global::real_angles.x = -10;
			else
				global::real_angles.x = cmd->viewangles.x;

			AutoRevolver(cmd);

			setup_bones();
		}



		*SendPacket = global::should_send_packet;
		cmd->move = antiaim->fix_movement(cmd, global::originalCMD);

		UTILS::ClampLemon(cmd->viewangles);

		return false;
	}
	float __fastcall GetScreenAspectRatio(void *pEcx, void *pEdx, int32_t iWidth, int32_t iHeight)
	{
		return ((float)Config::Misc::MiscMain::get().screen_x / (float)Config::Misc::MiscMain::get().screen_y);
	}

	void __stdcall HookedPaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce)

	{
		static unsigned int panelID = 0;
		std::string panel_name = INTERFACES::Panel->GetName(VGUIPanel);
		auto panelName = INTERFACES::Panel->GetName(VGUIPanel);


		if (!strcmp("HudZoom", INTERFACES::Panel->GetName(VGUIPanel)) && Config::Visuals::Main::VisualsEffects::get().RemoveEffect[2] == 1)return;
		if (Config::Visuals::Main::VisualsEffects::get().HitEffect[3])
			pHitmarker->Paint();

		auto local_player = static_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer()));
		if (!panelID)
		{
			if (strstr(panelName, "MatSystemTopPanel"))
				panelID = VGUIPanel;

			
		}
		SDK::ConVar* mat_fullbright = INTERFACES::cvar->FindVar("mat_fullbright");

		mat_fullbright->SetValue(Config::Visuals::Main::VisualsEffects::get().FullBright ? true : false);

		if (panel_name == "FocusOverlayPanel")
		{
			if (FONTS::ShouldReloadFonts())
				FONTS::InitFonts();

			if (MENU_FONTS::ShouldReloadFonts())
				MENU_FONTS::InitFonts();

			if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			{
				if (Config::Visuals::Main::Visualsworld::get().GranadeTrajectory)
					GrenadePrediction::get().Paint();
				auto matpostprocess = INTERFACES::cvar->FindVar("mat_postprocess_enable");
				matpostprocess->fnChangeCallback = 0;
				matpostprocess->SetValue(Config::Visuals::Main::VisualsEffects::get().DisablePostProcessing ? false : true);
				FEATURES::MISC::in_game_logger.Do();
				if (Config::Visuals::Main::Visualsworld::get().VisualizeTaserRange) visuals->DrawTaserDist();
				visuals->Draw();
				visuals->ClientDraw();
				Flashlight.RunFrame();
			}


		}
		if (panelID == VGUIPanel) {

			g_Menu.Render();
			//PPGUI::MENU::Do();
			if (Config::Visuals::Main::Visualsmain::get().screen[2])
				visuals->watermark();


			int screen_width, screen_height;
			INTERFACES::Engine->GetScreenSize(screen_width, screen_height);
			int w = screen_width;
			int h = screen_height;

			auto DoBamewareInto = [screen_width, screen_height]()
			{
				static constexpr float intro_length_time = 5.f, intro_max_height_offset = -25.f, intro_max_width_offset = -15.f;

				const float current_time = GetTickCount() * 0.001f, time_delta = current_time - global::cheat_start_time;
				if (time_delta < intro_length_time)
				{
					static constexpr char bameware_text[] = "Happy New Year 2020";

					const int text_width = RENDER::GetTextSize(FONTS::welcome_font, bameware_text).x, text_len = strlen(bameware_text);
					const int center_width = screen_width * 0.5, center_height = screen_height * 0.5;
					const int current_letter = ((time_delta / intro_length_time) * text_len) + 1;
					const float time_per_letter = intro_length_time / static_cast<float>(text_len);
					for (int i = 0; i < text_len; i++)
					{
						if (i >= current_letter)
							continue;

						const float frac = ((fmod(time_delta, time_per_letter) * 0.5f + (i == current_letter - 2 ? time_per_letter * 0.5f : 0)) / time_per_letter);

						int alpha = 0;
						if (current_letter > i + 2)
							alpha = 255;
						else
							alpha = frac * 255.f;

						const int cur_width = center_width + (text_width * 0.5f) - (text_width / static_cast<float>(text_len)) * static_cast<float>(text_len - i);
						if (i == current_letter - 1 || i == current_letter - 2)
						{
							const float frac_2 = (frac > 0.5f ? 1.f - frac : frac) * 2.f;
							RENDER::DrawF(cur_width + (frac_2 * intro_max_width_offset), center_height + (frac_2 * intro_max_height_offset), FONTS::welcome_font, false, true, CColor(0, 157, 255, alpha), std::string(1, bameware_text[i]));
						}
						else
							RENDER::DrawF(cur_width, center_height, FONTS::welcome_font, false, true, CColor(0, 157, 255, alpha), std::string(1, bameware_text[i]));
					}
				}
			};

			DoBamewareInto();
		}


		original_paint_traverse(INTERFACES::Panel, VGUIPanel, ForceRepaint, AllowForce);

		const char* pszPanelName = INTERFACES::Panel->GetName(VGUIPanel);

		if (!strstr(pszPanelName, sPanel.data()))
			return;

		INTERFACES::Panel->SetMouseInputEnabled(VGUIPanel, g_Menu.menuOpened);

	}

	void __fastcall HookedFrameStageNotify(void* ecx, void* edx, int stage)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player)
			return;
		static ClientFrameStage_t sn;

		Vector vecAimPunch, vecViewPunch;
		Vector* pAimPunch = nullptr, *pViewPunch = nullptr;

		auto GetCorrectDistance = [&local_player](float ideal_distance) -> float
		{

			Vector inverse_angles;
			INTERFACES::Engine->GetViewAngles(inverse_angles);

			inverse_angles.x *= -1.f, inverse_angles.y += 180.f;

			Vector direction;
			MATH::AngleVectors(inverse_angles, &direction);

			SDK::CTraceWorldOnly filter;
			SDK::trace_t trace;
			SDK::Ray_t ray;

			ray.Init(local_player->GetVecOrigin() + local_player->GetViewOffset(), (local_player->GetVecOrigin() + local_player->GetViewOffset()) + (direction * (ideal_distance + 5.f)));
			INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);

			return ideal_distance * trace.flFraction;
		};
		switch (stage)
		{

		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		{
			switch (Config::SkinChanger::Model::get().model_changer)
			{
			case 0: break;
			case 1: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_phoenix.mdl")); break;
			case 2: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/ctm_sas.mdl")); break;
			case 3: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_balkan_variantj.mdl")); break;
			case 4: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_balkan_variantg.mdl")); break;
			case 5: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_balkan_varianti.mdl")); break;
			case 6: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_balkan_variantf.mdl")); break;
			case 7: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/ctm_st6_varianti.mdl")); break;
			case 8: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/ctm_st6_variantm.mdl")); break;
			case 9: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/ctm_st6_variantg.mdl")); break;
			case 10: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/ctm_st6_variante.mdl")); break;
			case 11: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/ctm_st6_variantk.mdl")); break;
			case 12: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_balkan_varianth.mdl")); break;
			case 13: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/ctm_fbi_varianth.mdl")); break;
			case 14: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/ctm_fbi_variantg.mdl")); break;
			case 15: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/ctm_fbi_variantf.mdl")); break;
			case 16: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_phoenix_variantg.mdl")); break;
			case 17: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_phoenix_variantf.mdl")); break;
			case 18: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_phoenix_varianth.mdl")); break;
			case 19: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_leet_variantf.mdl")); break;
			case 20: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_leet_varianti.mdl")); break;
			case 21: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_leet_varianth.mdl")); break;
			case 22: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_leet_varianth.mdl")); break;
			case 23: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/tm_leet_variantg.mdl")); break;
			case 24: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/ctm_fbi_variantb.mdl")); break;
			case 25: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/ctm_sas_variantf.mdl")); break;
			case 26: local_player->SetModelIndex(INTERFACES::ModelInfo->GetModelIndex("models/player/custom_player/legacy/ctm_idf.mdl")); break;
			}

			GloveChanger();
			xdSkinchanger();
			KnifeChanger();
			if(Config::Ragebot::RageGlobal::get().resolver)
				resolver.DoFSN();

		}			break;

		case FRAME_NET_UPDATE_POSTDATAUPDATE_END: {
				backtracking.Store();
		}break;

		case FRAME_RENDER_START: {
			if (in_tp)
			{
				*(Vector*)((DWORD)local_player + 0x31D8) = Vector(global::real_angles.x, global::real_angles.y, 0.f);
				INTERFACES::pPrediction->SetLocalViewAngles(Vector(global::real_angles.x, global::real_angles.y, 0));

			}
			for (int i = 1; i <= 65; i++)
			{
				auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
				if (!entity) continue;
				if (entity == local_player) continue;

				*(int*)((uintptr_t)entity + 0xA30) = INTERFACES::Globals->framecount;
				*(int*)((uintptr_t)entity + 0xA28) = 0;
			}
		} break;
		case FRAME_NET_UPDATE_START: {
			if(Config::Visuals::Main::VisualsEffects::get().BulletTracers)
				visuals->DrawBulletBeams();

			for (int i = 1; i <= 12; i++)
				g_soundesp.draw();
		}
			break;
		case FRAME_NET_UPDATE_END: {
			for (int i = 1; i < 65; i++)
			{
				auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);

				if (!entity)
					continue;

				if (!local_player)
					continue;

				bool is_local_player = entity == local_player;
				bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

				if (is_local_player)
					continue;

				if (is_teammate)
					continue;

				if (entity->GetHealth() <= 0)
					continue;
			}
		}
			break;

		case FRAME_RENDER_END: {
			break;
		}
		}
		original_frame_stage_notify(ecx, stage);
	}
	void __fastcall Hooked_Postscreeneffects(void* thisptr, void* edx, SDK::CViewSetup* setup) {
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player) return;
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			for (auto i = 0; i < INTERFACES::GlowObjManager->GetSize(); i++)
			{
				auto &glowObject = INTERFACES::GlowObjManager->m_GlowObjectDefinitions[i];
				auto entity = reinterpret_cast<SDK::CBaseEntity*>(glowObject.m_pEntity);

				if (!entity) continue;
				if (!local_player) continue;

				if (glowObject.IsUnused()) continue;

				bool is_local_player = entity == local_player;
				bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;
				if (entity->GetClientClass()->m_ClassID == 1 || entity->GetClientClass()->m_ClassID == 66 || entity->GetClientClass()->m_ClassID == 87 || entity->GetClientClass()->m_ClassID == 98 || entity->GetClientClass()->m_ClassID == 108 || entity->GetClientClass()->m_ClassID == 134 || entity->GetClientClass()->m_ClassID == 1 || entity->GetClientClass()->m_ClassID == 9 || entity->GetClientClass()->m_ClassID == 29 || entity->GetClientClass()->m_ClassID == 39 || entity->GetClientClass()->m_ClassID == 41 || entity->GetClientClass()->m_ClassID == WEAPON_CPlantedC4 || entity->GetClientClass()->m_ClassID == WEAPON_C4 || entity->GetClientClass()->m_ClassID == WEAPON_MOLOTOV || entity->GetClientClass()->m_ClassID == WEAPON_INC || entity->GetClientClass()->m_ClassID == WEAPON_HEGRENADE || entity->GetClientClass()->m_ClassID == WEAPON_SMOKEGRENADE || entity->GetClientClass()->m_ClassID == WEAPON_FLASHBANG || entity->GetClientClass()->m_ClassID == WEAPON_CC4 || entity->GetClientClass()->m_ClassID == CMolotovGrenade || entity->GetClientClass()->m_ClassID == CSmokeGrenade || entity->GetClientClass()->m_ClassID == WEAPON_DECOY || strstr(entity->GetClientClass()->m_pNetworkName, "Weapon"))
				{
					if (Config::Visuals::Main::Visualsworld::get().DroppedWeapons[3]) {
						glowObject.m_flRed = 255 / 255.0f;
						glowObject.m_flGreen = 144 / 255.0f;
						glowObject.m_flBlue = 165 / 255.0f;
						glowObject.m_flAlpha = 255/ 255.0f;
						glowObject.m_bRenderWhenOccluded = true;
						glowObject.m_bRenderWhenUnoccluded = false;
					}
				}
				if (is_local_player && in_tp && Config::Visuals::Main::Visualschams::get().local_glow)
				{
					glowObject.m_nGlowStyle = 3;
					glowObject.m_flRed = SETTINGS::settings.glowlocal_col[0];
					glowObject.m_flGreen = SETTINGS::settings.glowlocal_col[1];
					glowObject.m_flBlue = SETTINGS::settings.glowlocal_col[2];
					glowObject.m_flAlpha = SETTINGS::settings.glowlocal_col[3];
					glowObject.m_bRenderWhenOccluded = true;
					glowObject.m_bRenderWhenUnoccluded = false;
					continue;
				}
				else if (!Config::Visuals::Main::Visualschams::get().local_glow && is_local_player)
					continue;

				if (entity->GetHealth() <= 0) continue;
				if (entity->GetIsDormant())	continue;
				if (entity->GetClientClass()->m_ClassID != 40) continue;

				if (is_teammate && Config::Visuals::Main::Visualschams::get().team_glow)
				{
					glowObject.m_nGlowStyle = 0; //0;
					glowObject.m_flRed = SETTINGS::settings.teamglow_color[0];
					glowObject.m_flGreen = SETTINGS::settings.teamglow_color[1];
					glowObject.m_flBlue = SETTINGS::settings.teamglow_color[2];
					glowObject.m_flAlpha = SETTINGS::settings.teamglow_color[3];
					glowObject.m_bRenderWhenOccluded = true;
					glowObject.m_bRenderWhenUnoccluded = false;
					continue;
				}
				else if (is_teammate && !Config::Visuals::Main::Visualschams::get().team_glow)
					continue;

				if (Config::Visuals::Main::Visualschams::get().enemy_glow)
				{
					glowObject.m_nGlowStyle = SETTINGS::settings.glowstyle;//0;
					glowObject.m_flRed = Config::Visuals::Main::Visualschams::get().enemy_glow_r / 255;
					glowObject.m_flGreen = Config::Visuals::Main::Visualschams::get().enemy_glow_g / 255;
					glowObject.m_flBlue = Config::Visuals::Main::Visualschams::get().enemy_glow_b / 255;
					glowObject.m_flAlpha = Config::Visuals::Main::Visualschams::get().enemy_glow_a / 255;
					glowObject.m_bRenderWhenOccluded = true;
					glowObject.m_bRenderWhenUnoccluded = false;
					continue;
				}
			}

		}


		return orginal_post_screen_effect(thisptr, setup);
	}
	void zAngleMatrix(const Vector angles, matrix3x4_t& matrix)
	{
		float sr, sp, sy, cr, cp, cy;

		sy = sin(DEG2RAD(angles[1]));
		cy = cos(DEG2RAD(angles[1]));

		sp = sin(DEG2RAD(angles[0]));
		cp = cos(DEG2RAD(angles[0]));

		sr = sin(DEG2RAD(angles[2]));
		cr = cos(DEG2RAD(angles[2]));

		//matrix = (YAW * PITCH) * ROLL
		matrix[0][0] = cp * cy;
		matrix[1][0] = cp * sy;
		matrix[2][0] = -sp;

		float crcy = cr * cy;
		float crsy = cr * sy;
		float srcy = sr * cy;
		float srsy = sr * sy;

		matrix[0][1] = sp * srcy - crsy;
		matrix[1][1] = sp * srsy + crcy;
		matrix[2][1] = sr * cp;

		matrix[0][2] = (sp * crcy + srsy);
		matrix[1][2] = (sp * crsy - srcy);
		matrix[2][2] = cr * cp;

		matrix[0][3] = 0.0f;
		matrix[1][3] = 0.0f;
		matrix[2][3] = 0.0f;
	}

	void MatrixSetColumn(const Vector& in, int column, matrix3x4_t& out)
	{
		out[0][column] = in.x;
		out[1][column] = in.y;
		out[2][column] = in.z;
	}

	void AngleMatrix(const QAngle& angles, const Vector& position, matrix3x4_t& matrix_out)
	{
		zAngleMatrix(Vector(angles.pitch, angles.yaw, angles.roll), matrix_out);
		MatrixSetColumn(position, 3, matrix_out);
	}

	void MatrixCopy(const matrix3x4_t& source, matrix3x4_t& target)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				target[i][j] = source[i][j];
			}
		}
	}

	void MatrixMultiply(matrix3x4_t& in1, const matrix3x4_t& in2)
	{
		matrix3x4_t out;
		if (&in1 == &out)
		{
			matrix3x4_t in1b;
			MatrixCopy(in1, in1b);
			MatrixMultiply(in1b, in2);
			return;
		}
		if (&in2 == &out)
		{
			matrix3x4_t in2b;
			MatrixCopy(in2, in2b);
			MatrixMultiply(in1, in2b);
			return;
		}
		out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
			in1[0][2] * in2[2][0];
		out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
			in1[0][2] * in2[2][1];
		out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
			in1[0][2] * in2[2][2];
		out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
			in1[0][2] * in2[2][3] + in1[0][3];
		out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
			in1[1][2] * in2[2][0];
		out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
			in1[1][2] * in2[2][1];
		out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
			in1[1][2] * in2[2][2];
		out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
			in1[1][2] * in2[2][3] + in1[1][3];
		out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
			in1[2][2] * in2[2][0];
		out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
			in1[2][2] * in2[2][1];
		out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
			in1[2][2] * in2[2][2];
		out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
			in1[2][2] * in2[2][3] + in1[2][3];

		in1 = out;
	}
	FORCEINLINE float DotProduct(const float* v1, const float* v2) {
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	}
	void zzVectorRotate(const float* in1, const matrix3x4_t& in2, float* out)
	{
		out[0] = DotProduct(in1, in2[0]);
		out[1] = DotProduct(in1, in2[1]);
		out[2] = DotProduct(in1, in2[2]);
	}

	void zVectorRotate(const Vector& in1, const matrix3x4_t& in2, Vector& out)
	{
		zzVectorRotate(&in1.x, in2, &out.x);
	}

	void VectorRotate(const Vector& in1, const QAngle& in2, Vector& out)
	{
		matrix3x4_t matRotate;
		zAngleMatrix(Vector(in2.pitch, in2.yaw, in2.roll), matRotate);
		zVectorRotate(in1, matRotate, out);
	}
	Vector BonePos;
	Vector OutPos;
	Vector BonePos1;
	Vector OutPos2;
	matrix3x4_t oldBoneMatrix[128];
	matrix3x4_t oldFakelag[128];
	void manage_local_fake_animstate(matrix3x4_t* matrix, const SDK::ModelRenderInfo_t& render_info)
	{
		auto local_player = static_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer()));
		if (local_player->GetFlags() & FL_ONGROUND) {
			for (int i = 0; i < 128; i++)
			{
				zAngleMatrix(Vector(0, global::fake_angles.y, 0), oldBoneMatrix[i]);
				MatrixMultiply(oldBoneMatrix[i], matrix[i]);
				BonePos = Vector(matrix[i][0][3], matrix[i][1][3], matrix[i][2][3]) - render_info.origin;
				VectorRotate(BonePos, QAngle(0, global::fake_angles.y, 0), OutPos);
				OutPos += render_info.origin;
				oldBoneMatrix[i][0][3] = OutPos.x;
				oldBoneMatrix[i][1][3] = OutPos.y;
				oldBoneMatrix[i][2][3] = OutPos.z;

			}
		}
		else {
			//idk bro
		}

	}

	void manage_fakelag(matrix3x4_t* matrix, const SDK::ModelRenderInfo_t& render_info)
	{
		auto local_player = static_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer()));
		if (!global::should_send_packet && local_player->GetVelocity().Length2D() > 20) {
			for (int i = 0; i < 128; i++)
			{
				zAngleMatrix(Vector(0, global::fake_angles.y, 0), oldFakelag[i]);
				MatrixMultiply(oldFakelag[i], matrix[i]);
				BonePos1 = Vector(matrix[i][0][3], matrix[i][1][3], matrix[i][2][3]) - render_info.origin;
				VectorRotate(BonePos1, QAngle(0, global::fake_angles.y, 0), OutPos2);
				OutPos2 += render_info.origin;
				oldFakelag[i][0][3] = OutPos2.x;
				oldFakelag[i][1][3] = OutPos2.y;
				oldFakelag[i][2][3] = OutPos2.z;
			}
		}


	}
	bool loaded_chams = false;
	SDK::IMaterial* GetMat(int material) {
		if (!loaded_chams) {
			std::ofstream("csgo\\materials\\textured_virt.vmt") << R"("VertexLitGeneric"
				{
					"$basetexture"	"vgui/white"
					"$model"		"1"
					"$flat"			"0"
					"$nocull"		"1"
					"$halflambert"	"1"
					"$nofog"		"1"
					"$ignorez"		"0"
					"$znearer"		"0"
					"$wireframe"	"0"
				})";
			std::ofstream("csgo\\materials\\custom_virt.vmt") << R"("VertexLitGeneric" 
				{
					"$basetexture" "vgui/white_additive"
					"$ignorez" "1"
					"$envmap" "env_cubemap"
					"$normalmapalphaenvmapmask" "1"
					"$envmapcontrast"  "1"
					"$nofog" "1"
					"$model" "1"
					"$nocull" "0"
					"$selfillum" "1"
					"$halflambert" "1"
					"$znearer" "0"
					"$flat" "1" 
				})";
			std::ofstream("csgo\\materials\\flat_virt.vmt") << R"("UnlitGeneric"
				{
					"$basetexture"	"vgui/white"
					"$model"		"1"
					"$flat"			"1"
					"$nocull"		"1"
					"$selfillum"	"1"
					"$halflambert"	"1"
					"$nofog"		"1"
					"$ignorez"		"0"
					"$znearer"		"0"
					"$wireframe"	"0"
				})";
			std::ofstream("csgo/materials/glowOverlay.vmt") << R"#("VertexLitGeneric" {
				"$additive" "1"
				"$envmap" "models/effects/cube_white"
				"$envmaptint" "[0.2 0.2 0.7]"
				"$envmapfresnel" "1"
				"$envmapfresnelminmaxexp" "[0 1 2]"
				"$alpha" "0.8"
			})#";
			loaded_chams = true;
		}


		switch (material) {
		case 1: default: return INTERFACES::MaterialSystem->FindMaterial("textured_virt", TEXTURE_GROUP_OTHER);
		case 2: return INTERFACES::MaterialSystem->FindMaterial("flat_virt", TEXTURE_GROUP_OTHER);
		case 3: return INTERFACES::MaterialSystem->FindMaterial("custom_virt", TEXTURE_GROUP_OTHER);
		case 4: return INTERFACES::MaterialSystem->FindMaterial("dev/glow_armsrace", TEXTURE_GROUP_OTHER);
		case 5: return INTERFACES::MaterialSystem->FindMaterial("glowOverlay.vmt", TEXTURE_GROUP_OTHER);
		case 6: return INTERFACES::MaterialSystem->FindMaterial("regular_reflective.vmt", TEXTURE_GROUP_OTHER);
		}
	}
	bool isHand(std::string modelName) {
		if ((modelName.find("arms") != std::string::npos || modelName.find("v_models") != std::string::npos) && !(modelName.find("uid") != std::string::npos || modelName.find("stattrack") != std::string::npos)) {
			return true;
		}

		return false;
	}
	void modulate(SDK::IMaterial* material, float color[3], const float alpha, const float width) {
		const auto tint = material->find_var("$envmaptint");
		if (tint)
			tint->set_vector(Vector(color[0], color[1], color[2]));

		const auto transform = material->find_var("$envmapfresnelminmaxexp");
		if (transform)
			transform->set_vector_component(width * alpha, 1);

	}

	bool isWeapon(std::string modelName) {
		if ((modelName.find("v_") != std::string::npos || modelName.find("uid") != std::string::npos || modelName.find("stattrack") != std::string::npos) && modelName.find("arms") == std::string::npos) {
			return true;
		}

		return false;
	}
	static SDK::IMaterial* flag = nullptr;
	static SDK::IMaterial* entity = nullptr;
	static SDK::IMaterial* team = nullptr;
	static SDK::IMaterial* desync = nullptr;
	static SDK::IMaterial* local = nullptr;
	static SDK::IMaterial* weapon = nullptr;
	static SDK::IMaterial* fuck = nullptr;
	static SDK::IMaterial* hand = nullptr;
	static SDK::IMaterial* bt = nullptr;

	bool load_mat = false;
	void __fastcall HookedDrawModelExecute(void* ecx, void* edx, SDK::IMatRenderContext* context, const SDK::DrawModelState_t& state, const SDK::ModelRenderInfo_t& render_info, matrix3x4_t* matrix)
	{
		const char* ModelName = INTERFACES::ModelInfo->GetModelName((SDK::model_t*)render_info.model);
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			const char* ModelName = INTERFACES::ModelInfo->GetModelName((SDK::model_t*)render_info.model);
			auto local_player = static_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer()));
			SDK::CBaseEntity* pentity = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntity(render_info.entity_index);
			SDK::CBaseEntity* pLocal = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

			Vector view;
			INTERFACES::Engine->GetViewAngles(view);


			if (in_tp && render_info.entity_index == local_player->GetIndex() && local_player->GetIsScoped())
				INTERFACES::RenderView->SetBlend(SETTINGS::settings.transparency_amnt);

			if (!load_mat || global::should_load_materials) { /// gheto way for fps drops
				local = GetMat(Config::Visuals::Main::Visualschams::get().local_chams) ;
				desync = GetMat(Config::Visuals::Main::Visualschams::get().desync_chams);
				entity = GetMat(Config::Visuals::Main::Visualschams::get().enemy_chams);
				bt = GetMat(2);

				team = GetMat(Config::Visuals::Main::Visualschams::get().team_chams);
				flag = GetMat(Config::Visuals::Main::Visualschams::get().fakelag_chams);
				weapon = GetMat(Config::Visuals::Main::Visualschams::get().weapon_chams);
				hand = GetMat(Config::Visuals::Main::Visualschams::get().hand_chams);
				fuck = GetMat(1);
				global::should_load_materials = false;
				load_mat = true;
			}

			//ent
			float entity_invisible[3] = { Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_r / 255, Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_g / 255, Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_b / 255 };
			float entity_visible[3] = { Config::Visuals::Main::Visualschams::get().enemy_chams_r / 255, Config::Visuals::Main::Visualschams::get().enemy_chams_g / 255, Config::Visuals::Main::Visualschams::get().enemy_chams_b / 255 };
			float entity_bt[3] = { Config::Visuals::Main::Visualschams::get().backtrack_chams_r / 255, Config::Visuals::Main::Visualschams::get().backtrack_chams_g / 255, Config::Visuals::Main::Visualschams::get().backtrack_chams_b / 255 };

			//team
			float team_invisible[3] = { Config::Visuals::Main::Visualschams::get().team_chams_xqz_r / 255, Config::Visuals::Main::Visualschams::get().team_chams_xqz_g / 255, Config::Visuals::Main::Visualschams::get().team_chams_xqz_b / 255 };
			float team_visible[3] = { Config::Visuals::Main::Visualschams::get().team_chams_r / 255, Config::Visuals::Main::Visualschams::get().team_chams_g / 255, Config::Visuals::Main::Visualschams::get().team_chams_b / 255 };
			//local
			float local_col[3] = { Config::Visuals::Main::Visualschams::get().local_chams_r / 255, Config::Visuals::Main::Visualschams::get().local_chams_g / 255, Config::Visuals::Main::Visualschams::get().local_chams_b / 255 };
			float desync_col[3] = { Config::Visuals::Main::Visualschams::get().desync_chams_r / 255, Config::Visuals::Main::Visualschams::get().desync_chams_g / 255, Config::Visuals::Main::Visualschams::get().desync_chams_b / 255 };
			float ncol[3] = {1,1,1};
			float fakelag_col[3] = { Config::Visuals::Main::Visualschams::get().fakelag_chams_r / 255, Config::Visuals::Main::Visualschams::get().fakelag_chams_g / 255, Config::Visuals::Main::Visualschams::get().fakelag_chams_b / 255 };
			float legitaaa[3] = { Config::Legitbot::LegitGlobal::get().show_legit_aa_r / 255,Config::Legitbot::LegitGlobal::get().show_legit_aa_g / 255,Config::Legitbot::LegitGlobal::get().show_legit_aa_b / 255 };
			//hand
			float hand_col[3] = { Config::Visuals::Main::Visualschams::get().hand_chams_r / 255, Config::Visuals::Main::Visualschams::get().hand_chams_g / 255, Config::Visuals::Main::Visualschams::get().hand_chams_b / 255 };
			//weapon
			float weapon_col[3] = { Config::Visuals::Main::Visualschams::get().weapon_chams_r / 255, Config::Visuals::Main::Visualschams::get().weapon_chams_g / 255, Config::Visuals::Main::Visualschams::get().weapon_chams_b / 255 };

			if (pentity && (strstr(ModelName, "models/player"))) {
				if (pentity == pLocal) {
					if (INTERFACES::ModelRender->IsForcedMaterialOverride()) {
						original_draw_model_execute(ecx, context, state, render_info, matrix);
						return;
					}
					if (Config::Visuals::Main::Visualschams::get().fakelag_chams > 0) {
						manage_fakelag(matrix, render_info);
						flag->IncrementReferenceCount();
						INTERFACES::ModelRender->ForcedMaterialOverride(flag);
						if (Config::Visuals::Main::Visualschams::get().fakelag_chams < 4) {
							INTERFACES::RenderView->SetColorModulation2(fakelag_col);
							INTERFACES::RenderView->SetBlend(Config::Visuals::Main::Visualschams::get().fakelag_chams_a / 255);
						}
						else
							modulate(flag, fakelag_col, Config::Visuals::Main::Visualschams::get().fakelag_chams_a / 255, 60.f);
						original_draw_model_execute(ecx, context, state, render_info, oldFakelag);
						INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
					}
					if (Config::Visuals::Main::Visualschams::get().desync_chams > 0 && !antiaim->ShallReturn(&global::originalCMD)) {
						if (Config::AntiAim::AADesync::get().desync > 0 && Config::AntiAim::AADesync::get().desync2 > 0) {
							desync->Set_Flag(MATERIAL_VAR_WIREFRAME, false);
							manage_local_fake_animstate(matrix, render_info);
								INTERFACES::ModelRender->ForcedMaterialOverride(desync);
								if (Config::Visuals::Main::Visualschams::get().desync_chams < 4) {
									INTERFACES::RenderView->SetColorModulation2(desync_col);
									INTERFACES::RenderView->SetBlend(Config::Visuals::Main::Visualschams::get().desync_chams_a / 255);
								}
								else
									modulate(desync, desync_col, Config::Visuals::Main::Visualschams::get().desync_chams_a / 255, Config::Visuals::Main::Visualschams::get().desync_chams_w);
								original_draw_model_execute(ecx, context, state, render_info,oldBoneMatrix);
								INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
						}
					}
					if (Config::Legitbot::LegitGlobal::get().show_legit_aa) {
						matrix3x4_t BoneMatrix[128];
						Vector OutPosition;
						Vector bonePostion;
						for (int i = 0; i < 128; i++)
						{
							zAngleMatrix(Vector(0, legitaa.y, 0), BoneMatrix[i]);
							MatrixMultiply(BoneMatrix[i], matrix[i]);
							bonePostion = Vector(matrix[i][0][3], matrix[i][1][3], matrix[i][2][3]) - render_info.origin;
							VectorRotate(bonePostion, QAngle(0, legitaa.y, 0), OutPosition);
							OutPos += render_info.origin;
							BoneMatrix[i][0][3] = OutPosition.x;
							BoneMatrix[i][1][3] = OutPosition.y;
							BoneMatrix[i][2][3] = OutPosition.z;

							INTERFACES::ModelRender->ForcedMaterialOverride(desync);
							INTERFACES::RenderView->SetBlend(Config::Legitbot::LegitGlobal::get().show_legit_aa_a / 255);
							INTERFACES::RenderView->SetColorModulation2(legitaaa);
							original_draw_model_execute(ecx, context, state, render_info, BoneMatrix);
							INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
						}

					}

					if (Config::Visuals::Main::Visualschams::get().local_chams > 0) {
						local->Set_Flag(MATERIAL_VAR_WIREFRAME, false);
						local->IncrementReferenceCount();
						INTERFACES::ModelRender->ForcedMaterialOverride(local);
						if (Config::Visuals::Main::Visualschams::get().local_chams < 4) {
							INTERFACES::RenderView->SetColorModulation2(local_col);
							INTERFACES::RenderView->SetBlend(Config::Visuals::Main::Visualschams::get().local_chams_a / 255);
						}
						else
							modulate(local, local_col, Config::Visuals::Main::Visualschams::get().local_chams_a / 255, Config::Visuals::Main::Visualschams::get().local_chams_w);
						original_draw_model_execute(ecx, context, state, render_info, matrix);
						INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
						return;
					}
					else {
						if (Config::Visuals::Main::Visualschams::get().desync_chams > 0 || Config::Legitbot::LegitGlobal::get().show_legit_aa) {
							INTERFACES::RenderView->SetBlend(1);
							INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
							INTERFACES::RenderView->SetColorModulation(ncol);
							original_draw_model_execute(ecx, context, state, render_info, matrix);
							return;
						}
					}

				}
				if (pentity != local_player && pentity->GetTeam() != local_player->GetTeam() && pentity->IsAlive()) {
					if (INTERFACES::ModelRender->IsForcedMaterialOverride()) {
						original_draw_model_execute(ecx, context, state, render_info, matrix);
						return;
					}
					entity->IncrementReferenceCount();
					if (Config::Visuals::Main::Visualschams::get().enemy_chams > 0) {
						if (Config::Visuals::Main::Visualschams::get().backtrack_chams && !pentity->GetIsDormant() ) {
							bt->Set_Flag(MATERIAL_VAR_IGNOREZ, true);
								auto backtrack_records = backtracking.GetPriorityRecords(pentity);
								if (pentity->GetVelocity().Length2D() > 2 && backtrack_records.size() > 0) { //ghetto but work :P
									for (auto record : backtrack_records)
									{
										INTERFACES::ModelRender->ForcedMaterialOverride(bt);
										INTERFACES::RenderView->SetColorModulation2(entity_bt);
										INTERFACES::RenderView->SetBlend(Config::Visuals::Main::Visualschams::get().backtrack_chams_a / 255);
										original_draw_model_execute(ecx, context, state, render_info, record.bone_matrix);
										INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);

									}
								}
						}
						if (Config::Visuals::Main::Visualschams::get().enemy_xyz_chams) {
							entity->Set_Flag(MATERIAL_VAR_IGNOREZ, true);
							INTERFACES::ModelRender->ForcedMaterialOverride(entity);
							if (Config::Visuals::Main::Visualschams::get().enemy_chams < 4) {
								INTERFACES::RenderView->SetColorModulation2(entity_invisible);
								INTERFACES::RenderView->SetBlend(Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_a / 255);
							}
							else
								modulate(entity, entity_invisible, Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_a / 255, Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_w);
							original_draw_model_execute(ecx, context, state, render_info, matrix);
							INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
						}
						entity->Set_Flag(MATERIAL_VAR_IGNOREZ, false);
						INTERFACES::ModelRender->ForcedMaterialOverride(entity);
						if (Config::Visuals::Main::Visualschams::get().enemy_chams < 4) {
							INTERFACES::RenderView->SetColorModulation2(entity_visible);
							INTERFACES::RenderView->SetBlend(Config::Visuals::Main::Visualschams::get().enemy_chams_a / 255);
						}
						else
							modulate(entity, entity_visible, Config::Visuals::Main::Visualschams::get().enemy_chams_a / 255, Config::Visuals::Main::Visualschams::get().enemy_chams_w);
						original_draw_model_execute(ecx, context, state, render_info, matrix);
						INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
						return;
					}
				}
				if (pentity != local_player && pentity->GetTeam() == local_player->GetTeam()) {

					if (INTERFACES::ModelRender->IsForcedMaterialOverride()) {
						original_draw_model_execute(ecx, context, state, render_info, matrix);
						return;
					}
					team->IncrementReferenceCount();
					if (Config::Visuals::Main::Visualschams::get().team_chams > 0) {
						if (Config::Visuals::Main::Visualschams::get().team_xyz_chams) {
							team->Set_Flag(MATERIAL_VAR_IGNOREZ, true);
							INTERFACES::ModelRender->ForcedMaterialOverride(team);
							if (Config::Visuals::Main::Visualschams::get().team_chams < 4) {
								INTERFACES::RenderView->SetColorModulation2(team_invisible);
								INTERFACES::RenderView->SetBlend(Config::Visuals::Main::Visualschams::get().team_chams_a / 255);
							}
							else
								modulate(team, team_invisible, Config::Visuals::Main::Visualschams::get().team_chams_a / 255, Config::Visuals::Main::Visualschams::get().team_chams_w);
							original_draw_model_execute(ecx, context, state, render_info, matrix);
							INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
						}
						team->Set_Flag(MATERIAL_VAR_IGNOREZ, false);
						INTERFACES::ModelRender->ForcedMaterialOverride(team);
						if (Config::Visuals::Main::Visualschams::get().team_chams < 4) {
							INTERFACES::RenderView->SetColorModulation2(team_visible);
							INTERFACES::RenderView->SetBlend(Config::Visuals::Main::Visualschams::get().team_chams_a / 255);
						}
						else
							modulate(team, team_visible, Config::Visuals::Main::Visualschams::get().team_chams_a / 255, Config::Visuals::Main::Visualschams::get().team_chams_w);
						original_draw_model_execute(ecx, context, state, render_info, matrix);
						INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
						return;
					}
				}
			}
			if (Config::Visuals::Main::Visualschams::get().hand_chams > 0) {
				if (Config::Visuals::Main::Visualschams::get().hand_chams_wireframe)
					hand->Set_Flag(MATERIAL_VAR_WIREFRAME, true);
				else
					hand->Set_Flag(MATERIAL_VAR_WIREFRAME, false);
				if (isHand(ModelName)) {

					hand->IncrementReferenceCount();
					if (Config::Visuals::Main::Visualschams::get().hand_chams < 4) {
						INTERFACES::RenderView->SetColorModulation2(hand_col);
						INTERFACES::RenderView->SetBlend(Config::Visuals::Main::Visualschams::get().hand_chams_a / 255);
					}
					else
						modulate(hand, hand_col, Config::Visuals::Main::Visualschams::get().hand_chams_a / 255, Config::Visuals::Main::Visualschams::get().hand_chams_w);

					INTERFACES::ModelRender->ForcedMaterialOverride(hand);
					original_draw_model_execute(ecx, context, state, render_info, matrix);
					INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
					return;
				}
			}
			if (Config::Visuals::Main::Visualschams::get().weapon_chams > 0) {
				if (Config::Visuals::Main::Visualschams::get().weapon_chams_wireframe)
					weapon->Set_Flag(MATERIAL_VAR_WIREFRAME, true);
				else
					weapon->Set_Flag(MATERIAL_VAR_WIREFRAME, false);
				if (isWeapon(ModelName)) {

					weapon->IncrementReferenceCount();
					if (Config::Visuals::Main::Visualschams::get().weapon_chams < 4) {
						INTERFACES::RenderView->SetColorModulation2(weapon_col);
						INTERFACES::RenderView->SetBlend(Config::Visuals::Main::Visualschams::get().weapon_chams_a / 255);
					}
					else
						modulate(weapon, hand_col, Config::Visuals::Main::Visualschams::get().weapon_chams_a / 255, Config::Visuals::Main::Visualschams::get().weapon_chams_w);
					INTERFACES::ModelRender->ForcedMaterialOverride(weapon);

					original_draw_model_execute(ecx, context, state, render_info, matrix);
					INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
					return;
				}
			}
			if (Config::Visuals::Main::VisualsEffects::get().RemoveEffect[0] == 1)
			{
				SDK::IMaterial* Flash = INTERFACES::MaterialSystem->FindMaterial("effects\\flashbang", "ClientEffect textures");
				SDK::IMaterial* FlashWhite = INTERFACES::MaterialSystem->FindMaterial("effects\\flashbang_white", "ClientEffect textures");
				Flash->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
				FlashWhite->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
			}
		}
		original_draw_model_execute(ecx, context, state, render_info, matrix);

		if (Config::Visuals::Main::VisualsEffects::get().DrawLC == 1 && INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame() ) {
			SDK::CBaseEntity* pLocal = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			SDK::CBaseEntity* pentity = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntity(render_info.entity_index);
			auto local_player = static_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer())); //idk why I Did this

			if (!pentity)
				return;

			if (!pentity->GetClientClass()->m_ClassID == (int)CSGOClassID::CCSPlayer || !strstr(ModelName, "models/player"))
				return;

			for (int i = 0; i < nel->lag_comp.size(); i++) {


				static SDK::IMaterial* lagcomp = nullptr;
				lagcomp = GetMat(5);
				float entity_lc[3] = { 255,255,255 };
				float CurrentTime = local_player->GetTickBase() * INTERFACES::Globals->interval_per_tick;


				if (nel->lag_comp[i].flEraseTime <= CurrentTime) {
					nel->lag_comp.erase(nel->lag_comp.begin() + i); // lets clear the vector, so we dont get memory leaking
					continue;
				}

				lagcomp->Set_Flag(MATERIAL_VAR_IGNOREZ, true);
				INTERFACES::ModelRender->ForcedMaterialOverride(lagcomp);
				INTERFACES::RenderView->SetColorModulation2(entity_lc);
				INTERFACES::RenderView->SetBlend((nel->lag_comp[i].flEraseTime - CurrentTime) / 10);
				original_draw_model_execute(ecx, context, state, render_info, nel->lag_comp[i].pmatrix);

			}
			INTERFACES::ModelRender->ForcedMaterialOverride(nullptr);
			return;
		}



		INTERFACES::RenderView->SetBlend(1.f);
	}
	void __fastcall HookedSceneEnd(void* ecx, void* edx)
	{

		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			original_scene_end(ecx);

				auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
				if (!local_player) return;
				CColor color = CColor(SETTINGS::settings.glow_col[0] * 255, SETTINGS::settings.glow_col[1] * 255, SETTINGS::settings.glow_col[2] * 255, SETTINGS::settings.glow_col[3] * 255), colorTeam = CColor(SETTINGS::settings.teamglow_color[0] * 255, SETTINGS::settings.teamglow_color[1] * 255, SETTINGS::settings.teamglow_color[2] * 255, SETTINGS::settings.teamglow_color[3] * 255), colorlocal = CColor(SETTINGS::settings.glowlocal_col[0] * 255, SETTINGS::settings.glowlocal_col[1] * 255, SETTINGS::settings.glowlocal_col[2] * 255, SETTINGS::settings.glowlocal_col[3] * 255);
				for (int i = 1; i < 65; i++)
				{

				if (Config::Visuals::Main::VisualsEffects::get().RemoveEffect[1] == 1)
				{
					std::vector<const char*> vistasmoke_wireframe = { "particle/vistasmokev1/vistasmokev1_smokegrenade" };

					std::vector<const char*> vistasmoke_nodraw =
					{
						"particle/vistasmokev1/vistasmokev1_fire",
						"particle/vistasmokev1/vistasmokev1_emods",
						"particle/vistasmokev1/vistasmokev1_emods_impactdust",
					};

					for (auto mat_s : vistasmoke_wireframe)
					{
						SDK::IMaterial* mat = INTERFACES::MaterialSystem->FindMaterial(mat_s, TEXTURE_GROUP_OTHER);
						mat->SetMaterialVarFlag(SDK::MATERIAL_VAR_WIREFRAME, true); //wireframe
					}

					for (auto mat_n : vistasmoke_nodraw)
					{
						SDK::IMaterial* mat = INTERFACES::MaterialSystem->FindMaterial(mat_n, TEXTURE_GROUP_OTHER);
						mat->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
					}

					static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
					*(int*)(smokecout) = 0;
				}
			}

		}
	}
	void __fastcall HookedOverrideView(void* ecx, void* edx, SDK::CViewSetup* pSetup)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player) return;

		auto animstate = local_player->GetAnimState();
		if (!animstate) return;

		if (GetAsyncKeyState(Config::Visuals::Main::VisualsEffects::get().ThirdPerson) & 1)
			in_tp = !in_tp;

		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			if (Config::Visuals::Main::VisualsEffects::get().RemoveEffect[4] == 1)
			{
				pSetup->angles -= *local_player->GetAimPunchAngle() * 0.9f + *local_player->GetViewPunchAngle();
			}
			GrenadePrediction::get().View(pSetup);
			auto GetCorrectDistance = [&local_player](float ideal_distance) -> float //lambda
			{
				Vector inverse_angles;
				INTERFACES::Engine->GetViewAngles(inverse_angles);

				inverse_angles.x *= -1.f, inverse_angles.y += 180.f;

				Vector direction;
				MATH::AngleVectors(inverse_angles, &direction);

				SDK::CTraceWorldOnly filter;
				SDK::trace_t trace;
				SDK::Ray_t ray;

				ray.Init(local_player->GetVecOrigin() + local_player->GetViewOffset(), (local_player->GetVecOrigin() + local_player->GetViewOffset()) + (direction * (ideal_distance + 5.f)));
				INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);

				return ideal_distance * trace.flFraction;
			};
			auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			if (GetAsyncKeyState(Config::Misc::MiscMain::get().fakeduck) && local_player->IsAlive() && local_player->GetFlags() & FL_ONGROUND ) {
				pSetup->origin.z = local_player->GetAbsOrigin().z + 64;
			}

			if (in_tp)
			{
				if (local_player->GetHealth() <= 0)
					local_player->SetObserverMode(5);

				if (!INTERFACES::Input->m_fCameraInThirdPerson)
				{
					INTERFACES::Input->m_fCameraInThirdPerson = true;
					INTERFACES::Input->m_vecCameraOffset = Vector(global::real_angles.x, global::real_angles.y, GetCorrectDistance(100));

					Vector camForward;
					MATH::AngleVectors(Vector(INTERFACES::Input->m_vecCameraOffset.x, INTERFACES::Input->m_vecCameraOffset.y, 0), &camForward);
				}
			}
			else
			{
				INTERFACES::Input->m_fCameraInThirdPerson = false;
				INTERFACES::Input->m_vecCameraOffset = Vector(global::real_angles.x, global::real_angles.y, 0);
			}

			auto zoomsensration = INTERFACES::cvar->FindVar("zoom_sensitivity_ratio_mouse");
			if (SETTINGS::settings.fixscopesens)
				zoomsensration->SetValue("0");
			else
				zoomsensration->SetValue("1");

			if (local_player->GetIsScoped() && Config::Visuals::Main::VisualsEffects::get().RemoveEffect[3] == 1)
				pSetup->fov = Config::Misc::MiscMain::get().view_fov;
			else {
				if (!local_player->GetIsScoped())
					pSetup->fov = Config::Misc::MiscMain::get().view_fov;

			}
		}
		original_override_view(ecx, pSetup);
	}
	void __fastcall HookedTraceRay(void *thisptr, void*, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter *pTraceFilter, SDK::trace_t *pTrace)
	{
		original_trace_ray(thisptr, ray, fMask, pTraceFilter, pTrace);
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
			pTrace->surface.flags |= SURF_SKY;
	}
	bool __fastcall HookedGetBool(void* pConVar, void* edx)
	{
		if ((uintptr_t)_ReturnAddress() == CAM_THINK)
			return true;

		return original_get_bool(pConVar);
	}
	float __fastcall GetViewmodelFOV()
	{
		if (INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame())
		{
			float player_fov = original_viewmodel_fov();
			player_fov = Config::Misc::MiscMain::get().view_model_fov;

			return player_fov;
		}
	}




	LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg) {
		case WM_LBUTTONDOWN:
			PressedKeys[VK_LBUTTON] = true;
			break;
		case WM_LBUTTONUP:
			PressedKeys[VK_LBUTTON] = false;
			break;
		case WM_RBUTTONDOWN:
			PressedKeys[VK_RBUTTON] = true;
			break;
		case WM_RBUTTONUP:
			PressedKeys[VK_RBUTTON] = false;
			break;
		case WM_MBUTTONDOWN:
			PressedKeys[VK_MBUTTON] = true;
			break;
		case WM_MBUTTONUP:
			PressedKeys[VK_MBUTTON] = false;
			break;
		case WM_XBUTTONDOWN:
		{
			UINT button = GET_XBUTTON_WPARAM(wParam);
			if (button == XBUTTON1)
			{
				PressedKeys[VK_XBUTTON1] = true;
			}
			else if (button == XBUTTON2)
			{
				PressedKeys[VK_XBUTTON2] = true;
			}
			break;
		}
		case WM_XBUTTONUP:
		{
			UINT button = GET_XBUTTON_WPARAM(wParam);
			if (button == XBUTTON1)
			{
				PressedKeys[VK_XBUTTON1] = false;
			}
			else if (button == XBUTTON2)
			{
				PressedKeys[VK_XBUTTON2] = false;
			}
			break;
		}
		case WM_KEYDOWN:
			PressedKeys[wParam] = true;
			break;
		case WM_KEYUP:
			PressedKeys[wParam] = false;
			break;
		default: break;
		}


		if (g_Menu.menuOpened)
		return true;

		return CallWindowProc(INIT::OldWindow, hWnd, uMsg, wParam, lParam);
	}


	SDK::CBaseEntity* Target;

	typedef bool(__thiscall *FireEventClientSide)(void*, SDK::IGameEvent*);

	bool __fastcall Hooked_FireEventClientSide(void *ecx, void* edx, SDK::IGameEvent* pEvent) {

		static auto ofunct = fireevent.get_original<FireEventClientSide>(9);

		if (!pEvent)
			return ofunct(ecx, pEvent);

		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		auto userID = pEvent->GetInt("userid");
		auto attacker = INTERFACES::Engine->GetPlayerForUserID(userID);
		global::Hited[attacker] = false;
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(aimbot->BestEntityI);
		if (local_player) {
			if (!strcmp(pEvent->GetName(), "weapon_fire") && entity != nullptr) {
				auto userID = pEvent->GetInt("userid");
				auto attacker = INTERFACES::Engine->GetPlayerForUserID(userID);
				if (Config::AntiAim::AAMisc::get().hide_shots) {
						auto nci = INTERFACES::Engine->GetNetChannel();
						if (nci->m_nChokedPackets) {
							// suppress command, so it wont be simulated
							global::originalCMD.tick_count = INT_MAX;
						}

						// tickbase shift amount
						if (nci->m_nChokedPackets < 6) {
							global::should_send_packet = false;
						}
						else {
							global::should_send_packet = true;
						}


				}

				if (attacker) {
					if (attacker == INTERFACES::Engine->GetLocalPlayer()) {
						auto nci = INTERFACES::Engine->GetNetChannel();
						NEL::LagComp lc;
						switch (Config::Visuals::Main::VisualsEffects::get().DrawLC) {
						case 1: {
								entity->SetupBones2(lc.pmatrix, 128, 256, INTERFACES::Globals->curtime);

								lc.flEraseTime = local_player->GetTickBase() * INTERFACES::Globals->interval_per_tick + 5.f;
								lc.Player = entity;

								nel->lag_comp.push_back(lc);
						}break;
						case 2: {

							entity->SetupBones2(lc.pmatrix, 128, 256, INTERFACES::Globals->curtime);

							lc.flEraseTime = local_player->GetTickBase() * INTERFACES::Globals->interval_per_tick + 5.f;
							lc.Player = entity;

							nel->lag_comp.push_back(lc);
						}break;
						case 3: {
							visuals->LagCompHitbox(entity, entity->GetIndex());

						}
						}
						global::fired[entity->GetIndex()]++;
					}
				}
			}

			if (!strcmp(pEvent->GetName(), "player_hurt") && entity != nullptr) {
				int deadfag = pEvent->GetInt("userid");
				int attackingfag = pEvent->GetInt("attacker");
				if (INTERFACES::Engine->GetPlayerForUserID(deadfag) != INTERFACES::Engine->GetLocalPlayer() && INTERFACES::Engine->GetPlayerForUserID(attackingfag) == INTERFACES::Engine->GetLocalPlayer()) {
					global::hit[entity->GetIndex()]++;
					global::Hited[entity->GetIndex()] = true;

				}
			}
			if (entity != nullptr)
				global::missedshots[entity->GetIndex()] = global::fired[entity->GetIndex()] - global::hit[entity->GetIndex()];
		}

		return ofunct(ecx, pEvent);
	}

	void InitHooks()
	{
		iclient_hook_manager.Init(INTERFACES::Client);
		original_frame_stage_notify = reinterpret_cast<FrameStageNotifyFn>(iclient_hook_manager.HookFunction<FrameStageNotifyFn>(37, HookedFrameStageNotify));
		
		panel_hook_manager.Init(INTERFACES::Panel);
		original_paint_traverse = reinterpret_cast<PaintTraverseFn>(panel_hook_manager.HookFunction<PaintTraverseFn>(41, HookedPaintTraverse));
		
		scene_end_hook_manager.Init(INTERFACES::RenderView);
		original_scene_end = reinterpret_cast<SceneEndFn>(scene_end_hook_manager.HookFunction<SceneEndFn>(9, HookedSceneEnd));

		trace_hook_manager.Init(INTERFACES::Trace);
		original_trace_ray = reinterpret_cast<TraceRayFn>(trace_hook_manager.HookFunction<TraceRayFn>(5, HookedTraceRay));
		

		override_view_hook_manager.Init(INTERFACES::ClientMode);
		original_override_view = reinterpret_cast<OverrideViewFn>(override_view_hook_manager.HookFunction<OverrideViewFn>(18, HookedOverrideView));
		orginal_post_screen_effect = reinterpret_cast<DoPostScreenEffects_t>(override_view_hook_manager.HookFunction<CreateMoveFn>(44, Hooked_Postscreeneffects));
		original_create_move = reinterpret_cast<CreateMoveFn>(override_view_hook_manager.HookFunction<CreateMoveFn>(24, HookedCreateMove));
		original_viewmodel_fov = reinterpret_cast<GetViewmodelFOVFn>(override_view_hook_manager.HookFunction<GetViewmodelFOVFn>(35, GetViewmodelFOV));


		model_render_hook_manager.Init(INTERFACES::ModelRender);
		original_draw_model_execute = reinterpret_cast<DrawModelExecuteFn>(model_render_hook_manager.HookFunction<DrawModelExecuteFn>(21, HookedDrawModelExecute));

		screen_aspect.setup(INTERFACES::Engine);
		screen_aspect.hook_index(101, GetScreenAspectRatio);
		


		auto sv_cheats = INTERFACES::cvar->FindVar("sv_cheats");
		get_bool_manager = VMT::VMTHookManager(reinterpret_cast<DWORD**>(sv_cheats));
		original_get_bool = reinterpret_cast<SvCheatsGetBoolFn>(get_bool_manager.HookFunction<SvCheatsGetBoolFn>(13, HookedGetBool));

		INTERFACES::cvar->get_convar("viewmodel_offset_x")->callbacks.SetSize(0);
		INTERFACES::cvar->get_convar("viewmodel_offset_y")->callbacks.SetSize(0);
		INTERFACES::cvar->get_convar("viewmodel_offset_z")->callbacks.SetSize(0);

		fireevent.setup(INTERFACES::GameEventManager);
		fireevent.hook_index(9, Hooked_FireEventClientSide);


		
		if (!(INIT::Window = FindWindowA("Valve001", nullptr)))
			Sleep(100);

		if (INIT::Window)
			INIT::OldWindow = (WNDPROC)SetWindowLongPtr(INIT::Window, GWL_WNDPROC, (LONG_PTR)Hooked_WndProc);

		

	}
	void EyeAnglesPitchHook(const SDK::CRecvProxyData *pData, void *pStruct, void *pOut)
	{
		*reinterpret_cast<float*>(pOut) = pData->m_Value.m_Float;

		auto entity = reinterpret_cast<SDK::CBaseEntity*>(pStruct);
		if (!entity)
			return;


		resolver.GetPlayerResolveInfo(entity).networked_angles.x = pData->m_Value.m_Float;

	}
	void EyeAnglesYawHook(const SDK::CRecvProxyData *pData, void *pStruct, void *pOut)
	{
		*reinterpret_cast<float*>(pOut) = pData->m_Value.m_Float;

		auto entity = reinterpret_cast<SDK::CBaseEntity*>(pStruct);
		if (!entity)
			return;

		resolver.GetPlayerResolveInfo(entity).networked_angles.y = pData->m_Value.m_Float;
	}
	void InitNetvarHooks()
	{
		UTILS::netvar_hook_manager.Hook("DT_CSPlayer", "m_angEyeAngles[0]", EyeAnglesPitchHook);
		UTILS::netvar_hook_manager.Hook("DT_CSPlayer", "m_angEyeAngles[1]", EyeAnglesYawHook);
	}
}



#define ARRAY_SIZE(X)   (sizeof(array_size(X)) ? (sizeof(X) / sizeof((X)[0])) : -1)




