

#include "../includes.h"

#include "../UTILS/interfaces.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/IEngine.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ConVar.h"
#include "../SDK/ISurface.h"
#include "../SDK/IVDebugOverlay.h"
#include "../UTILS/render.h"
#include "NewEventLog.h"
#include "../FEATURES/Visuals.h"
#include <playsoundapi.h>
#include <sstream> 
#include "New\NewResolver.h"
#include <string>
#include "../SDK/IViewRenderBeams.h"
#include "New\NewResolver.h"

#pragma comment(lib, "Winmm.lib")
bool hurtcalled = false;			
int kill = 0;

typedef void(__cdecl* MsgFn)(const char* msg, va_list);
void ConMsg(const char* msg, ...) {

	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything

	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"); //This gets the address of export "Msg" in the dll "tier0.dll". The static keyword means it's only called once and then isn't called again (but the variable is still there)
	char buffer[989];
	va_list list; //Normal varargs stuff http://stackoverflow.com/questions/10482960/varargs-to-printf-all-arguments
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list); //Calls the function, we got the address above.

	if (!INTERFACES::cvar->FindVar("developer")->GetInt())
		INTERFACES::cvar->FindVar("developer")->SetValue(true);

	if (! INTERFACES::cvar->FindVar( "con_filter_enable" )->GetInt( ) )
		INTERFACES::cvar->FindVar( "con_filter_enable" )->SetValue( 2 );
}

/* sound esp */
soundesp g_soundesp;
bullettracers imapct;

/* sound logs */
std::vector<sound_info> sound_logs;

enum
{
	TE_BEAMPOINTS = 0x00,		// beam effect between two points
	TE_SPRITE = 0x01,	// additive sprite, plays 1 cycle
	TE_BEAMDISK = 0x02,	// disk that expands to max radius over lifetime
	TE_BEAMCYLINDER = 0x03,		// cylinder that expands to max radius over lifetime
	TE_BEAMFOLLOW = 0x04,		// create a line of decaying beam segments until entity stops moving
	TE_BEAMRING = 0x05,		// connect a beam ring to two entities
	TE_BEAMSPLINE = 0x06,
	TE_BEAMRINGPOINT = 0x07,
	TE_BEAMLASER = 0x08,		// Fades according to viewpoint
	TE_BEAMTESLA = 0x09,
};
enum
{
	FBEAM_STARTENTITY = 0x00000001,
	FBEAM_ENDENTITY = 0x00000002,
	FBEAM_FADEIN = 0x00000004,
	FBEAM_FADEOUT = 0x00000008,
	FBEAM_SINENOISE = 0x00000010,
	FBEAM_SOLID = 0x00000020,
	FBEAM_SHADEIN = 0x00000040,
	FBEAM_SHADEOUT = 0x00000080,
	FBEAM_ONLYNOISEONCE = 0x00000100,		// Only calculate our noise once
	FBEAM_NOTILE = 0x00000200,
	FBEAM_USE_HITBOXES = 0x00000400,		// Attachment indices represent hitbox indices instead when this is set.
	FBEAM_STARTVISIBLE = 0x00000800,		// Has this client actually seen this beam's start entity yet?
	FBEAM_ENDVISIBLE = 0x00001000,		    // Has this client actually seen this beam's end entity yet?
	FBEAM_ISACTIVE = 0x00002000,
	FBEAM_FOREVER = 0x00004000,
	FBEAM_HALOBEAM = 0x00008000,		    // When drawing a beam with a halo, don't ignore the segments and endwidth
	FBEAM_REVERSED = 0x00010000,
	NUM_BEAM_FLAGS = 17	                    // KEEP THIS UPDATED!
};

void soundesp::draw_circle(CColor color, Vector position)
{
	SDK::BeamInfo_t beamInfo;
	beamInfo.m_nType = TE_BEAMRINGPOINT;
	beamInfo.m_pszModelName = "sprites/purplelaser1.vmt";
	beamInfo.m_nModelIndex = INTERFACES::ModelInfo->GetModelIndex("sprites/purplelaser1.vmt");
	beamInfo.m_pszHaloName = "sprites/purplelaser1.vmt";
	beamInfo.m_nHaloIndex = INTERFACES::ModelInfo->GetModelIndex("sprites/purplelaser1.vmt");
	beamInfo.m_flHaloScale = 5;
	beamInfo.m_flLife = 1.50f;
	beamInfo.m_flWidth = 12.f;
	beamInfo.m_flFadeLength = 1.0f;
	beamInfo.m_flAmplitude = 0.f;
	beamInfo.m_flRed = Config::Visuals::Main::Visualsmain::get().soundesp_color_r;
	beamInfo.m_flGreen = Config::Visuals::Main::Visualsmain::get().soundesp_color_g;
	beamInfo.m_flBlue = Config::Visuals::Main::Visualsmain::get().soundesp_color_b;
	beamInfo.m_flBrightness = 255;
	beamInfo.m_flSpeed = beamInfo.m_flBlue = Config::Visuals::Main::Visualsmain::get().soundesp_speed;
	beamInfo.m_nStartFrame = 0;
	beamInfo.m_flFrameRate = 1;
	beamInfo.m_nSegments = 1;
	beamInfo.m_bRenderable = true;
	beamInfo.m_nFlags = 0;
	beamInfo.m_vecCenter = position + Vector(0, 0, 5);
	beamInfo.m_flStartRadius = 1;
	beamInfo.m_flEndRadius = beamInfo.m_flBlue = Config::Visuals::Main::Visualsmain::get().soundesp_size;

	auto beam = INTERFACES::ViewRenderBeams->CreateBeamRingPoint(beamInfo);

	if (beam) INTERFACES::ViewRenderBeams->DrawBeam(beam);
}
void bullettracers::draw_beam(Vector src, Vector end, CColor color)
{
	SDK::BeamInfo_t info;
	info.m_nType = TE_BEAMPOINTS;
	info.m_pszModelName = "sprites/purplelaser1.vmt";
	//info.m_pszHaloName = -1;
	info.m_nHaloIndex = -1;
	info.m_flHaloScale = 0.f;
	info.m_flLife = 1;
	info.m_flWidth = 4;
	info.m_flEndWidth = 6;
	info.m_flFadeLength = 0;
	info.m_flAmplitude = 0.f;
	info.m_flBrightness = 255;
	info.m_flSpeed = 20.f;
	info.m_nStartFrame = 0;
	info.m_flFrameRate = 0;
	info.m_flRed = 0;
	info.m_flGreen = 0;
	info.m_flBlue = 255;
	info.m_nSegments = -1;
	info.m_bRenderable = true;
	info.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE;
	info.m_vecStart = src; // Vector(0, 0, 5);
	info.m_vecEnd = end;


	auto beam = INTERFACES::ViewRenderBeams->CreateBeamRingPoint(info);

	if (beam) INTERFACES::ViewRenderBeams->DrawBeam(beam);
}
void bullettracers::draw_beam1(Vector src, Vector end, CColor color)
{
	SDK::BeamInfo_t info;
	info.m_nType = TE_BEAMPOINTS;
	info.m_pszModelName = "sprites/purplelaser1.vmt";
	//info.m_pszHaloName = -1;
	info.m_nHaloIndex = -1;
	info.m_flHaloScale = 0.f;
	info.m_flLife = 2;
	info.m_flWidth = 4;
	info.m_flEndWidth = 6;
	info.m_flFadeLength = 0;
	info.m_flAmplitude = 0.f;
	info.m_flBrightness = 255;
	info.m_flSpeed = 1.f;
	info.m_nStartFrame = 0;
	info.m_flFrameRate = 0;
	info.m_flRed = 255;
	info.m_flGreen = 0;
	info.m_flBlue = 0;
	info.m_nSegments = -1;
	info.m_bRenderable = true;
	info.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE;
	info.m_vecStart = src; // Vector(0, 0, 5);
	info.m_vecEnd = end;


	auto beam = INTERFACES::ViewRenderBeams->CreateBeamRingPoint(info);

	if (beam) INTERFACES::ViewRenderBeams->DrawBeam(beam);
}
void bullettracers::events(SDK::IGameEvent * event)
{
	if (!strcmp(event->GetName(), "bullet_impact"))
	{
		auto index = INTERFACES::Engine->GetPlayerForUserID(event->GetInt("userid"));

		auto shooter = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(event->GetInt("userid")));
		if (!shooter)
			return;


		auto local_player = static_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer()));
		if (!local_player) return;


		Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));



		if (position == Vector(0, 0, 0))
			return;

		static SDK::ConVar* sv_showimpacts = INTERFACES::cvar->FindVar("sv_showimpacts");
		if (Config::Visuals::Main::VisualsEffects::get().BulletImapcts  == 2)
			sv_showimpacts->SetValue(1);
		else
			sv_showimpacts->SetValue(0);
		if (Config::Visuals::Main::VisualsEffects::get().BulletImapcts > 0)
			{
				logs.push_back(trace_info(local_player->GetEyeAngles(), position, INTERFACES::Globals->realtime));
				draw_beam(local_player->GetEyeAngles() - Vector(0, 0, 1), position, CColor(0,0,0,0));
				switch (Config::Visuals::Main::VisualsEffects::get().BulletImapcts) {
				case 1:
					INTERFACES::DebugOverlay->AddBoxOverlay(position, Vector(-2, -2, -2), Vector(2, 2, 2), Vector(0, 0, 0), Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_r, Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_g, Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_b, Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_a, 4); break;
				case 2: {
						INTERFACES::DebugOverlay->AddBoxOverlay(position - Vector(-1, -1, -1), Vector(-2, -2, -2), Vector(2, 2, 2), Vector(0, 0, 0), 0, 0, 255, 210, 4);
				}break;
				}
		}
		if (index != INTERFACES::Engine->GetLocalPlayer() && shooter->GetTeam() != local_player->GetTeam())
		{

				Resolver::PlayerResolveRecord record;
				record.on_shot_eye_curtime = shooter->GetSimTime();
				record.on_shot_eye_postion = shooter->GetEyeAngles();

			if (Config::Visuals::Main::VisualsEffects::get().BulletImapcts > 0)
			{
				switch (Config::Visuals::Main::VisualsEffects::get().BulletImapcts) {
				case 1:
					INTERFACES::DebugOverlay->AddBoxOverlay(position, Vector(-2, -2, -2), Vector(2, 2, 2), Vector(0, 0, 0), Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_r, Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_g, Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_b, Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_a, 4); break;
				case 2: {
					INTERFACES::DebugOverlay->AddBoxOverlay(position, Vector(-2, -2, -2), Vector(2, 2, 2), Vector(0, 0, 0), 255, 0, 0, 255, 4);
					INTERFACES::DebugOverlay->AddBoxOverlay(position - Vector(-1, -1, -1), Vector(-2, -2, -2), Vector(2, 2, 2), Vector(0, 0, 0), 0, 0, 255, 255, 4);
				}break;
				}
			}
		}
	}
}
void soundesp::draw()
{
	/* if footstep esp is enabled */
	if (!Config::Visuals::Main::Visualsmain::get().SoundESP)
		return;

	/* if we are connected */
	if (!INTERFACES::Engine->IsInGame() || !INTERFACES::Engine->IsConnected())
		return;


	for (unsigned int i = 0; i < sound_logs.size(); i++)
	{
		/* draw the beam */
		g_soundesp.draw_circle(CColor(Config::Visuals::Main::Visualsmain::get().soundesp_color_r, Config::Visuals::Main::Visualsmain::get().soundesp_color_g, Config::Visuals::Main::Visualsmain::get().soundesp_color_b, Config::Visuals::Main::Visualsmain::get().soundesp_color_a), sound_logs[i].position);

		sound_logs.erase(sound_logs.begin() + i);
	}
}

namespace FEATURES
{
	namespace MISC
	{
		InGameLogger in_game_logger;
		void ColorLine::Draw(int x, int y, unsigned int font)
		{

			for (int i = 0; i < texts.size(); i++)
			{
				RENDER::DrawF(x, y, font, false, false, colors[i], texts[i]);
				x += RENDER::GetTextSize(font, texts[i]).x;
			}
		}

		void InGameLogger::Do()
		{
			if(!Config::Misc::MiscMain::get().log)
				return;
			if (log_queue.size() > max_lines_at_once)
				log_queue.erase(log_queue.begin() + max_lines_at_once, log_queue.end());

			for (int i = 0; i < log_queue.size(); i++)
			{
				auto log = log_queue[i];
				float time_delta = fabs(UTILS::GetCurtime() - log.time);

				int height = ideal_height + (16 * i);

				/// erase dead logs
				if (time_delta > text_time)
				{
					log_queue.erase(log_queue.begin() + i);
					break;
				}
				if (time_delta > text_time - slide_out_speed)
					height = height + (((slide_out_speed - (text_time - time_delta)) / slide_out_speed) * slide_out_distance);

				/// fade out
				if (time_delta > text_time - text_fade_out_time)
					log.color_line.ChangeAlpha(255 - (((time_delta - (text_time - text_fade_out_time)) / text_fade_out_time) * 255.f));
				/// fade in
				if (time_delta < text_fade_in_time)
					log.color_line.ChangeAlpha((time_delta / text_fade_in_time) * 255.f);

				int width = ideal_width;

				/// slide from left
				if (time_delta < text_fade_in_time)
					width = (time_delta / text_fade_in_time) * static_cast<float>(slide_in_distance) + (ideal_width - slide_in_distance);
				/// slider from right
				if (time_delta > text_time - text_fade_out_time)
					width = ideal_width + (((time_delta - (text_time - text_fade_out_time)) / text_fade_out_time) * static_cast<float>(slide_out_distance));

				
				log.color_line.Draw(width, height, FONTS::in_game_logging_font);
				
			}
		}
	}
}

bool did_hit = false;

namespace FEATURES
{
	namespace MISC
	{

		CREATE_EVENT_LISTENER(ItemPurchaseListener, "item_purchase");

		CREATE_EVENT_LISTENER(PlayerHurtListener, "player_hurt");

		CREATE_EVENT_LISTENER(BulletImpactListener, "bullet_impact");

		CREATE_EVENT_LISTENER(WeaponFireListener, "weapon_fire");

		CREATE_EVENT_LISTENER(RoundStartListener, "round_start");

		CREATE_EVENT_LISTENER(RoundPreStartListener, "round_prestart");

		CREATE_EVENT_LISTENER(PlayerDeathListener, "player_death");

		CREATE_EVENT_LISTENER(RoundEndListener, "round_end");

		CREATE_EVENT_LISTENER(Player_footstep_event, "player_footstep");

		CREATE_EVENT_LISTENER(Player_hurt_event, "player_hurt");

		ItemPurchaseListener item_purchase_listener;
		PlayerHurtListener player_hurt_listener;
		BulletImpactListener bullet_impact_listener;
		WeaponFireListener weapon_fire_listener;
		RoundStartListener round_start_listener;
		RoundPreStartListener round_prestart_listener;
		PlayerDeathListener player_death_listener;
		RoundEndListener round_end_listener;
		Player_footstep_event player_footstep_listener;
		Player_hurt_event player_hurt_listener2;

		void InitializeEventListeners()
		{

			INTERFACES::GameEventManager->AddListener(&item_purchase_listener, ("item_purchase"), false);
			INTERFACES::GameEventManager->AddListener(&player_hurt_listener, ("player_hurt"), false);
			INTERFACES::GameEventManager->AddListener(&bullet_impact_listener, ("bullet_impact"), false);
			INTERFACES::GameEventManager->AddListener(&weapon_fire_listener, ("weapon_fire"), false);
			INTERFACES::GameEventManager->AddListener(&round_start_listener, ("round_start"), false);
			INTERFACES::GameEventManager->AddListener(&player_death_listener, ("player_death"), false);
			INTERFACES::GameEventManager->AddListener(&round_prestart_listener, ("round_prestart"), false);
			INTERFACES::GameEventManager->AddListener(&round_end_listener, ("round_end"), false);
			INTERFACES::GameEventManager->AddListener(&player_footstep_listener, "player_footstep", false);
			INTERFACES::GameEventManager->AddListener(&player_hurt_listener2, "player_hurt", false);

		}

		void RemoveEventListeners()
		{
			INTERFACES::GameEventManager->RemoveListener(&item_purchase_listener);
			INTERFACES::GameEventManager->RemoveListener(&player_hurt_listener);
			INTERFACES::GameEventManager->RemoveListener(&bullet_impact_listener);
			INTERFACES::GameEventManager->RemoveListener(&weapon_fire_listener);
			INTERFACES::GameEventManager->RemoveListener(&round_start_listener);
			INTERFACES::GameEventManager->RemoveListener(&round_start_listener);
			INTERFACES::GameEventManager->RemoveListener(&round_end_listener);
			INTERFACES::GameEventManager->RemoveListener(&player_footstep_listener);
			INTERFACES::GameEventManager->RemoveListener(&player_hurt_listener2);
		}

		void ItemPurchaseListener::FireGameEvent(SDK::IGameEvent* game_event)
		{
			if (!game_event)
				return;

			SDK::CBaseEntity* local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			if (!local_player)
				return;

			SDK::CBaseEntity* entity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
			if (!entity)
				return;

			if (entity->GetTeam() == local_player->GetTeam())
				return;

			SDK::player_info_t player_info;
			if (!INTERFACES::Engine->GetPlayerInfo(entity->GetIndex(), &player_info))
				return;

			MISC::InGameLogger::Log log;

			log.color_line.PushBack("[stellarcheat.club] ", RED);
			log.color_line.PushBack(player_info.szName, WHITE);
			log.color_line.PushBack(" bought a ", WHITE);
			log.color_line.PushBack(game_event->GetString("weapon"), WHITE);

			MISC::in_game_logger.AddLog(log);
		};

		void RoundPreStartListener::FireGameEvent(SDK::IGameEvent* event) {

			if (!event)
				return;

			auto local_player = static_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer())); if (!local_player) return;

		}

	
		void RoundEndListener::FireGameEvent(SDK::IGameEvent* event) {

			if (!event)
				return;

			auto local_player = static_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer())); if (!local_player) return;


		}

		void Player_hurt_event::FireGameEvent(SDK::IGameEvent* game_event)
		{
			/* if we are connected */
			if (!INTERFACES::Engine->IsInGame() || !INTERFACES::Engine->IsConnected())
				return;

			/* check if we have the sound esp enabled */
			if (!Config::Visuals::Main::Visualsmain::get().SoundESP)
				return;

			/* return if not event */
			if (!game_event)
				return;

			/* local player */
			auto localplayer = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

			/* get the attacker */
			auto attacker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("attacker")));

			/* get the victim */
			auto victim = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));


			/* clock */
			static int timer;

			timer += 1;

			if (timer > 2)
				timer = 0;

			/* check if we are attacker */
			if (attacker == localplayer)
			{
				if (timer < 1)
					sound_logs.push_back(sound_info(victim->GetAbsOrigin(), INTERFACES::Globals->curtime, game_event->GetInt("userid")));
			}
		}

		void Player_footstep_event::FireGameEvent(SDK::IGameEvent* game_event)
		{
			/* check if we are in game */
			if (!INTERFACES::Engine->IsInGame() || !INTERFACES::Engine->IsConnected())
				return;

			/* check if we have the sound esp enabled */
			if (!Config::Visuals::Main::Visualsmain::get().SoundESP)
				return;

			/* return if not event */
			if (!game_event)
				return;

			/* local player */
			auto localplayer = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

			/* footstep */
			if (strstr(game_event->GetName(), "player_footstep"))
			{
				/* get the walker */
				auto walker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));

				/* return if walker is nullptr */
				if (walker == nullptr)
					return;

				/* return if walker is dormant */
				if (walker->GetIsDormant())
					return;

				/* clock */
				static int timer;

				timer += 1;

				if (timer > 1)
					timer = 0;

				if (walker->GetTeam() != localplayer->GetTeam())
				{
					if (walker && timer < 1)
					{
						sound_logs.push_back(sound_info(walker->GetAbsOrigin(), INTERFACES::Globals->curtime, game_event->GetInt("userid")));
					}
				}
			}
		}

		void PlayerHurtListener::FireGameEvent(SDK::IGameEvent* game_event)
		{
			if (!game_event)
				return;
			
			resolver.EventCallback(game_event);

			SDK::CBaseEntity* entity = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
			SDK::CBaseEntity* local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			if (!local_player)
				return;
			if (!strcmp(game_event->GetName(), "player_hurt")) {
			
			}

			int index = INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid"));

			SDK::CBaseEntity* attacker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("attacker")));
			if (!attacker)
				return;

			if (attacker != local_player)
				return;

			SDK::player_info_t player_info;
			INTERFACES::Engine->GetPlayerInfo(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")), &player_info);

			INTERFACES::Engine->GetPlayerInfo(index, &player_info);
			
			SDK::CBaseEntity* hurt = (SDK::CBaseEntity*) INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));


			MISC::InGameLogger::Log log;

			std::string name = player_info.szName;
			std::transform(name.begin(), name.end(), name.begin(), ::toupper);

			int hitgroup = game_event->GetInt("hitgroup");
			if (hitgroup > 0 && hitgroup <= 7)
			{
				log.color_line.PushBack(" in the ", WHITE);
				log.color_line.PushBack(UTILS::GetHitgroupName(hitgroup), WHITE);
			}
			log.color_line.PushBack("[stellarcheat.club] ", RED);
			log.color_line.PushBack(" for ", WHITE);
			log.color_line.PushBack(std::to_string(game_event->GetInt("dmg_health")), WHITE);
			log.color_line.PushBack(" damage.", WHITE);

			MISC::in_game_logger.AddLog(log);

			hitmarkers.AddHitmarker(game_event->GetInt("dmg_health"));

			if(Config::Visuals::Main::VisualsEffects::get().HitEffect[1])
				INTERFACES::Surface->IPlaySound("buttons\\arena_switch_press_02.wav");

			shots_hit[attacker->GetIndex()]++;



		};

		void BulletImpactListener::FireGameEvent(SDK::IGameEvent* game_event)
		{
			bullettracers per;
			if (!game_event)
				return;

			resolver.EventCallback(game_event);

			if (!(INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame()))
				return;

			SDK::CBaseEntity* local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

			int iUser = INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid"));
			auto entity = reinterpret_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(iUser));
			if (!entity) return;

			if (entity->GetIsDormant()) return;



			float x, y, z;
			x = game_event->GetFloat("x");
			y = game_event->GetFloat("y");
			z = game_event->GetFloat("z");

			per.events(game_event);

			if (entity == local_player || entity->GetTeam() == local_player->GetTeam())
				return;
			UTILS::BulletImpact_t impact(entity, Vector(x, y, z), INTERFACES::Globals->curtime, iUser == INTERFACES::Engine->GetLocalPlayer() ? GREEN : RED);

			visuals->Impacts.push_back(impact);


		}

		void PlayerDeathListener::FireGameEvent(SDK::IGameEvent* event) {


			if (!event)
				return;
			SDK::CBaseEntity* local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			if (!local_player)
				return;

			SDK::CBaseEntity* e = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(event->GetInt("userid")));

			if (e = local_player) {
				for (int i = 0; i < 65; i++) {
					global::missedshots[i] = 0;
					global::fired[i] = 0;
					global::hit[i] = 0;
				}
			}

			SDK::CBaseEntity* attacker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(event->GetInt("attacker")));
			if (!attacker)
				return;




			if (attacker != local_player)
				return;
			visuals->set_hitmarker_timekill(INTERFACES::Globals->curtime);
		


			if (Config::Visuals::Main::VisualsEffects::get().HitEffect[0])
				local_player->HealthShotBoostExpirationTime(INTERFACES::Globals->curtime + 1.f);

			SDK::player_info_t player_info;
			INTERFACES::Engine->GetPlayerInfo(INTERFACES::Engine->GetPlayerForUserID(event->GetInt("userid")), &player_info);

			std::string name = player_info.szName;
			std::transform(name.begin(), name.end(), name.begin(), ::toupper);

		}

		void WeaponFireListener::FireGameEvent(SDK::IGameEvent* game_event)
		{
			if (!game_event)
				return;

			resolver.EventCallback(game_event);


		}

		void RoundStartListener::FireGameEvent(SDK::IGameEvent* game_event)
		{
			if (!game_event)
				return;

			if (!(INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame()))
				return;
			global::should_buy = true;
			global::should_load_materials = true;
			SDK::CBaseEntity* local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
			if (!local_player)
				return;

			global::preservedelete = true;
		}
	}
}
NEL* nel = new NEL();