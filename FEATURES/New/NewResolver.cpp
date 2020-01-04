#include "../../includes.h"

#include "../../UTILS/interfaces.h"
#include "../../UTILS/render.h"
#include "../../SDK/CClientEntityList.h"
#include "../../SDK/IEngine.h"
#include "../../SDK/CBaseAnimState.h"
#include "../../SDK/CGlobalVars.h"
#include "../../SDK/GameEvents.h"
#include "../../SDK/CTrace.h"
#include "../../SDK/Collideable.h"

#include "../../FEATURES/Autowall.h"
#include "../AutoWall.h"
#include "../../UTILS/math.h"
#include "../NewEventLog.h"

#include "NewResolver.h"
#include "Backtrackingcpp.h"
#include "../Aimbot.h"
#include "../../SDK/ConVar.h"

Resolver resolver;

Resolver::Resolver() { }

void Resolver::DoFSN()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0)
		return;

		auto entity = INTERFACES::ClientEntityList->GetClientEntity(aimbot->BestEntityI);
		if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || !entity->IsAlive() || entity == nullptr)
			return;


		UpdateEntityAnimtions(entity);
		UpdateResolveRecord(entity);

		ResolveYaw(entity);
		ResolvePitch(entity);

		const auto resolve_record = player_resolve_records[entity->GetIndex()];
		entity->SetEyeAngles(resolve_record.resolved_angles);
		entity->GetAnimState()->m_flGoalFeetYaw = resolve_record.resolved_angles.y;
}

void Resolver::DoCM()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0)
		return;

	ProcessSnapShots();

	last_eye_positions.insert(last_eye_positions.begin(), local_player->GetVecOrigin() + local_player->GetViewOffset());
	if (last_eye_positions.size() > 128)
		last_eye_positions.pop_back();

	auto nci = INTERFACES::Engine->GetNetChannelInfo();
	if (!nci)
		return;

	const int latency_ticks = TIME_TO_TICKS(nci->GetLatency(FLOW_OUTGOING));
	const auto latency_based_eye_pos = last_eye_positions.size() <= latency_ticks ? last_eye_positions.back() : last_eye_positions[latency_ticks];
	//RENDER::render_queue.AddDrawCircle(latency_based_eye_pos, 2, 50, RED);
	auto entity = INTERFACES::ClientEntityList->GetClientEntity(aimbot->BestEntityI);
	if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || !entity->IsAlive() || entity == nullptr)
		return;

	auto& resolve_record = player_resolve_records[entity->GetIndex()];

	const float height = 64;

	resolve_record.did_predicted_lby_flick = false;
	resolve_record.did_lby_flick = false;
}

void Resolver::AddShotSnapshot(SDK::CBaseEntity* entity, PlayerResolveRecord resolve_record)
{
	ShotSnapshot snapshot;

	snapshot.entity = entity;
	snapshot.time = UTILS::GetCurtime();
	snapshot.resolve_record = resolve_record;
	snapshot.first_processed_time = 0.f;
	snapshot.was_shot_processed = false;
	snapshot.hitgroup_hit = -1;

	shot_snapshots.push_back(snapshot);
}
void Resolver::CheckForOnShot(SDK::CBaseEntity* entity, SDK::IGameEvent* game_event) {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0 || !game_event)
		return;



}
void Resolver::EventCallback(SDK::IGameEvent* game_event)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0 || !game_event)
		return;

	if (!strstr(game_event->GetName(), "weapon_fire"))
	{
		/// it wasn't us who shot

		std::cout << "weapon_fire \n";
		SDK::CBaseEntity* attacker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt(("userid"))));

		auto& record = player_resolve_records[attacker->GetIndex()];
		record.on_shot_eye_curtime = attacker->GetSimTime();
		record.on_shot_eye_postion = attacker->GetEyeAngles();
	}

	if (shot_snapshots.size() <= 0)
		return;

	auto& snapshot = shot_snapshots.front();
	if (strstr(game_event->GetName(), "player_hurt"))
	{
		/// it wasn't us who shot
		if (INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("attacker")) != INTERFACES::Engine->GetLocalPlayer())
			return;

		if (!snapshot.was_shot_processed)
		{
			snapshot.was_shot_processed = true;
			snapshot.first_processed_time = UTILS::GetCurtime();
		}

		snapshot.hitgroup_hit = game_event->GetInt("hitgroup");

	}
	if (strstr(game_event->GetName(), "bullet_impact"))
	{
		

	}
	if (strstr(game_event->GetName(), "weapon_fire"))
	{
		/// it wasn't us who shot

		SDK::CBaseEntity* attacker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt(("userid"))));

		if (INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt(("userid")) != INTERFACES::Engine->GetLocalPlayer()) && attacker->GetTeam() != local_player->GetTeam()) {
			auto& record = player_resolve_records[attacker->GetIndex()];
			record.on_shot_eye_curtime = attacker->GetSimTime();
			record.on_shot_eye_postion = attacker->GetEyeAngles();
		}


		if (INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt(("userid"))) != INTERFACES::Engine->GetLocalPlayer())
			return;

		
		if (!snapshot.was_shot_processed)
		{
			snapshot.was_shot_processed = true;
			snapshot.first_processed_time = UTILS::GetCurtime();
		}


	}
}

void Resolver::ProcessSnapShots()
{
	if (shot_snapshots.size() <= 0)
		return;

	const auto snapshot = shot_snapshots.front();
	if (fabs(UTILS::GetCurtime() - snapshot.time) > 1.f)
	{

		shot_snapshots.erase(shot_snapshots.begin());
		return;
	}

	const int player_index = snapshot.entity->GetIndex();
	if (snapshot.hitgroup_hit != -1) /// hit
	{
		/// increment shots fired and shots hit
		for (int i = 0; i < RESOLVE_TYPE_NUM; i++)
		{
			if (snapshot.resolve_record.resolve_type & (1 << i))
			{
				player_resolve_records[player_index].shots_fired[i]++;
				player_resolve_records[player_index].shots_hit[i]++;
			}
		}

		FEATURES::MISC::in_game_logger.AddLog(FEATURES::MISC::InGameLogger::Log("[RESOLVER] Hit shot at " + TranslateResolveRecord(snapshot.resolve_record.resolve_type), GREEN));
	}
	else if (snapshot.first_processed_time != 0.f && fabs(UTILS::GetCurtime() - snapshot.first_processed_time) > 0.1f) /// missed
	{
		/// increment shots fired but not shots hit
		for (int i = 0; i < RESOLVE_TYPE_NUM; i++)
		{
			if (snapshot.resolve_record.resolve_type & (1 << i))
				player_resolve_records[player_index].shots_fired[i]++;
		}




		FEATURES::MISC::in_game_logger.AddLog(FEATURES::MISC::InGameLogger::Log("[RESOLVER] Missed shot at " + TranslateResolveRecord(snapshot.resolve_record.resolve_type), RED));
	}
	else
		return;

	shot_snapshots.erase(shot_snapshots.begin());
}
inline float NormalizePitch(float pitch)
{
	while (pitch > 89.f)
		pitch -= 180.f;
	while (pitch < -89.f)
		pitch += 180.f;

	return pitch;
}
int GetLerpTicks()
{
	static const auto cl_interp_ratio = INTERFACES::cvar->FindVar("cl_interp_ratio");
	static const auto cl_updaterate = INTERFACES::cvar->FindVar("cl_updaterate");
	static const auto cl_interp = INTERFACES::cvar->FindVar("cl_interp");

	return TIME_TO_TICKS(max(cl_interp->GetFloat(), cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat()));
}
float Resolver::GetDeltaTime(float simtime)
{
	auto nci = INTERFACES::Engine->GetNetChannelInfo();
	if (!nci)
		return FLT_MAX;

	float correct = 0.f;
	correct += nci->GetLatency(FLOW_OUTGOING);
	correct += nci->GetLatency(FLOW_INCOMING);
	correct += TICKS_TO_TIME(GetLerpTicks());
	correct = UTILS::clamp<float>(correct, 0.f, 1.f);

	return fabs(correct - (UTILS::GetCurtime() - simtime));
}
void Resolver::UpdateResolveRecord(SDK::CBaseEntity* entity)
{
	/// a copy of the previous record for comparisons and shit
	const auto previous_record = player_resolve_records[entity->GetIndex()];
	auto& record = player_resolve_records[entity->GetIndex()];

	record.resolved_angles = record.networked_angles;
	record.velocity = entity->GetVelocity();
	record.origin = entity->GetVecOrigin();
	record.lower_body_yaw = entity->GetLowerBodyYaw();
	record.is_desyncing_static = IsFakingYawStatic(entity);
	record.is_desyncing_jitter = IsFakingYawJitter(entity);
	record.resolve_type = 0;

	record.is_balance_adjust_triggered = false, record.is_balance_adjust_playing = false;
	for (int i = 0; i < 15; i++) /// go through each animation layer 
	{
		record.anim_layers[i] = entity->GetAnimOverlay(i);

		/// balanceadjust act aka 979
		if (entity->GetSequenceActivity(record.anim_layers[i].m_nSequence) == SDK::CSGO_ACTS::ACT_CSGO_IDLE_TURN_BALANCEADJUST)
		{
			record.is_balance_adjust_playing = true;

			/// balance adjust is being triggered
			if (record.anim_layers[i].m_flWeight == 1 || record.anim_layers[i].m_flCycle > previous_record.anim_layers[i].m_flCycle)
				record.last_balance_adjust_trigger_time = UTILS::GetCurtime();
			if (fabs(UTILS::GetCurtime() - record.last_balance_adjust_trigger_time) < 0.5f)
				record.is_balance_adjust_triggered = true;
		}
	}
	if (GetDeltaTime(record.on_shot_eye_curtime) <= 0.2f)
		record.can_onshot = true;
	
	else {
		record.can_onshot = false;
		record.on_shot_eye_curtime = 0;
		record.on_shot_eye_postion = Vector(0,0,0);
	}


	const auto bt_recs = backtracking.GetRecords(entity);
	/// if lby updated
	/// slowwlak
	const bool is_moving_on_ground = record.velocity.Length2D() > 0.34f && entity->GetFlags() & FL_ONGROUND;
	if (entity->GetVelocity().Length2D() > 5 && entity->GetVelocity().Length2D() < 50 && entity->GetAnimState()->m_fDuckAmount > 0.5)
		record.is_slowwalking = true;
	else
		record.is_slowwalking = false;

	if (record.is_slowwalking) //lets just use Ghetto way slowwalk resolver
		record.slowwalk_resolved_eye_pos = ResolveSlowwalk(entity);
	if (record.is_desyncing_jitter && !record.is_slowwalking && entity->GetFlags() & FL_ONGROUND)
		record.resolved_eye_pos = ResolverDesyncJitter(entity);
	if (record.is_desyncing_static && !record.is_slowwalking && entity->GetFlags() & FL_ONGROUND) 
		record.resolved_eye_pos = ResolverDesyncStatic(entity);


	if (NormalizePitch(record.networked_angles.x) > 5.f)
		record.last_time_down_pitch = UTILS::GetCurtime();
}
void Resolver::ResolverFeetYaw(SDK::CBaseEntity* entity) {
	auto animstate = entity->GetAnimState();
	auto& resolve_record = player_resolve_records[entity->GetIndex()];
	animstate->m_flGoalFeetYaw = resolve_record.resolved_angles.y; ///ezz ghetto way!

}

void Resolver::ResolveYaw(SDK::CBaseEntity* entity)
{
	auto& resolve_record = player_resolve_records[entity->GetIndex()];
	const auto bt_recs = backtracking.GetRecords(entity);
	if (global::missedshots[entity->GetIndex()] >= 3) /// bruteforce as last resort
	{
		ResolveYawBruteforce(entity);
		resolve_record.resolve_type |= RESOLVE_TYPE_BRUTEFORCE;
	}
	else if (resolve_record.can_onshot) {
		resolve_record.resolved_angles = resolve_record.on_shot_eye_postion;
		resolve_record.resolve_type |= RESOLVE_TYPE_ON_SHOT_BT;

	}
	else if (resolve_record.is_slowwalking) /// moving
	{
		resolve_record.resolved_angles.y = resolve_record.slowwalk_resolved_eye_pos;
		resolve_record.resolve_type |= RESOLVE_TYPE_SLOWWALK;
	}
	else if (resolve_record.is_desyncing_static && entity->GetVelocity().Length2D() < 0.5f) {
		resolve_record.resolved_angles.y = resolve_record.resolved_eye_pos;
		resolve_record.resolve_type |= RESOLVE_TYPE_DESYNC_STATIC;
	}
	else if (resolve_record.is_desyncing_jitter && entity->GetVelocity().Length2D() < 0.5f) {
		resolve_record.resolved_angles.y = resolve_record.resolved_eye_pos;
		resolve_record.resolve_type |= RESOLVE_TYPE_DESYNC_JITTER;
	}
	else if (bt_recs.size() >= 1 && !bt_recs.front().resolve_record.is_desyncing_static && !bt_recs.front().resolve_record.is_desyncing_jitter)
	{
		resolve_record.resolved_angles.y = resolve_record.networked_angles.y;
		resolve_record.resolve_type |= RESOLVE_TYPE_NO_FAKE;
	}
}
float Resolver::ResolverDesyncStatic(SDK::CBaseEntity* entity) {
	auto animstate = entity->GetAnimState();
	float calculated_delta = 0.f;
	auto lbyYaw = animstate->m_flGoalFeetYaw;
	auto eyeYaw = animstate->m_flEyeYaw;
	float eye_feet_delta = fabs(eyeYaw - lbyYaw);
	float newFeetYaw = 0.f;
	float flMaxYawModifier = animstate->pad10[516] * 58;
	float flMinYawModifier = animstate->pad10[512] * -58;


	if (eye_feet_delta <= flMaxYawModifier)
	{
		if (flMinYawModifier > eye_feet_delta)
		{
			newFeetYaw = fabs(flMinYawModifier) + eyeYaw;
		}
	}
	else
		newFeetYaw = eyeYaw - fabs(flMaxYawModifier);

	calculated_delta = (newFeetYaw - animstate->body_yaw) * 2; //geting ABS angle and than multiplaying it by 2 to get the real angel

	return calculated_delta;
}
float Resolver::ResolverDesyncJitter(SDK::CBaseEntity* entity) {
	auto animstate = entity->GetAnimState();
	float calculated_delta = 0.f;
	auto lbyYaw = animstate->m_flGoalFeetYaw;
	auto eyeYaw = animstate->m_flEyeYaw;
	float eye_feet_delta = fabs(eyeYaw - lbyYaw);
	float newFeetYaw = 0.f;
	float flMaxYawModifier = animstate->pad10[516] * 58;
	float flMinYawModifier = animstate->pad10[512] * -58;


	if (eye_feet_delta <= flMaxYawModifier)
	{
		if (flMinYawModifier > eye_feet_delta)
		{
			newFeetYaw = fabs(flMinYawModifier) + eyeYaw;
		}
	}
	else
		newFeetYaw = eyeYaw - fabs(flMaxYawModifier);

	calculated_delta = (newFeetYaw - animstate->body_yaw) * 2; //geting ABS angle and than multiplaying it by 2 to get the real angel

	return calculated_delta;
}

//Ghetto ResolvePitch
void Resolver::ResolvePitch(SDK::CBaseEntity* entity)
{
	auto& resolve_record = player_resolve_records[entity->GetIndex()];

	if (resolve_record.resolve_type & RESOLVE_TYPE_NO_FAKE)
		resolve_record.resolved_angles.x = resolve_record.networked_angles.x;
	else if(resolve_record.resolve_type & RESOLVE_TYPE_ON_SHOT_BT)
		resolve_record.resolved_angles.x = resolve_record.on_shot_eye_postion.x;
	else
	{
		/// super fucking ghetto fix to stop their pitch from going to 0 when they're shooting (only neccesary when they're using psilent)
		if (fabs(UTILS::GetCurtime() - resolve_record.last_time_down_pitch) < 0.5f)
			resolve_record.resolved_angles.x = 70.f;
		else
			resolve_record.resolved_angles.x = resolve_record.networked_angles.x;
	}

	resolve_record.resolved_angles.x = NormalizePitch(resolve_record.resolved_angles.x);
}

void Resolver::ResolveYawBruteforce(SDK::CBaseEntity* entity)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	auto& resolve_record = player_resolve_records[entity->GetIndex()];
	resolve_record.resolve_type |= RESOLVE_TYPE_BRUTEFORCE;

	const float at_target_yaw = MATH::CalcAngle(entity->GetVecOrigin(), local_player->GetVecOrigin()).y;

	const int shots_missed = resolve_record.shots_fired[GetResolveTypeIndex(resolve_record.resolve_type)] -
		resolve_record.shots_hit[GetResolveTypeIndex(resolve_record.resolve_type)];
	switch (global::missedshots[entity->GetIndex()] % 3)
	{
	case 0:
		resolve_record.resolved_angles.y = at_target_yaw + 58;
		break;
	case 1:
		resolve_record.resolved_angles.y = at_target_yaw - 58;
		break;
	case 2:
		resolve_record.resolved_angles.y = at_target_yaw - 30;
		break;
	case 3:
		resolve_record.resolved_angles.y = at_target_yaw + 30;
		break;
	}

}


bool Resolver::IsYawSideways(SDK::CBaseEntity* entity, float yaw)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return false;

	const auto at_target_yaw = MATH::CalcAngle(local_player->GetVecOrigin(), entity->GetVecOrigin()).y;
	const float delta = fabs(MATH::NormalizeYaw(at_target_yaw - yaw));

	return delta > 20.f && delta < 160.f;
}

float Resolver::ResolveSlowwalk(SDK::CBaseEntity * entity) {
	auto animstate = entity->GetAnimState();
	float speed = animstate->m_flFeetSpeedForwardsOrSideWays;
		return fminf(speed, 1.0); // we are returning x spped any y will be just 1 like now
}

bool Resolver::IsFakingYawStatic(SDK::CBaseEntity* entity)
{
	const auto bt_records = backtracking.GetRecords(entity); //geting all record for players
	if (bt_records.size() < 2) 
		return true;

	if (bt_records.front().is_exploiting)
		return true;

	if((entity->GetAnimState()->m_flGoalFeetYaw - entity->GetAnimState()->body_yaw) < 58 || (entity->GetAnimState()->m_flGoalFeetYaw + entity->GetAnimState()->body_yaw) < -58) // calcualting 
		return true;

	if (fabs(bt_records[0].simulation_time - bt_records[1].simulation_time) == INTERFACES::Globals->interval_per_tick)
		return false;

	return true;
}

bool Resolver::IsFakingYawJitter(SDK::CBaseEntity* entity)
{
	const auto bt_records = backtracking.GetRecords(entity); //geting all record for players
	if (bt_records.size() < 2)
		return true;

	if (bt_records.front().is_exploiting)
		return true;

	if ((entity->GetAnimState()->m_flGoalFeetYaw - entity->GetAnimState()->body_yaw) < 58 || (entity->GetAnimState()->m_flGoalFeetYaw + entity->GetAnimState()->body_yaw) < -58) // calcualting 
		return true;

	if (fabs(bt_records[0].simulation_time - bt_records[1].simulation_time) == INTERFACES::Globals->interval_per_tick)
		return false;

	return true;
}

std::string Resolver::TranslateResolveRecord(unsigned short resolve_type)
{
	std::vector<std::string> resolve_strings;

	/// gonna have to use lorge if statements cuz fuck you
	if (resolve_type & RESOLVE_TYPE_NO_FAKE)
		resolve_strings.push_back(("NO FAKE"));
	if (resolve_type & RESOLVE_TYPE_BRUTEFORCE)
		resolve_strings.push_back(("BRUTEFORCE"));
	if (resolve_type & RESOLVE_TYPE_SLOWWALK)
		resolve_strings.push_back(("SLOWWALK"));
	if (resolve_type & RESOLVE_TYPE_DESYNC_STATIC)
		resolve_strings.push_back(("DESYNC STATIC"));
	if (resolve_type & RESOLVE_TYPE_DESYNC_JITTER)
		resolve_strings.push_back(("DESYNC JITTER"));
	if (resolve_type & RESOLVE_TYPE_ON_SHOT_BT)
		resolve_strings.push_back(("ON SHOT BACKTRACK"));
	if (resolve_strings.size() <= 0)
		return "SPREAD";

	std::string buffer;
	for (const auto& str : resolve_strings)
		buffer.size() ? (buffer += " / " + str) : (buffer = str);

	return buffer;
}

CColor Resolver::GetResolveColor(unsigned short resolve_type)
{
	/// gonna have to use lorge if statements cuz fuck you
	if (resolve_type & RESOLVE_TYPE_NO_FAKE)
		return GREEN;
	else if (resolve_type & RESOLVE_TYPE_BRUTEFORCE)
		return RED;
	else if (resolve_type & RESOLVE_TYPE_SLOWWALK)
		return YELLOW;
	else if (resolve_type & RESOLVE_TYPE_DESYNC_STATIC)
		return YELLOW;
	else if (resolve_type & RESOLVE_TYPE_DESYNC_JITTER)
		return YELLOW;
	else if (resolve_type & RESOLVE_TYPE_ON_SHOT_BT)
		return GREEN;

	return WHITE;
}

int Resolver::GetResolveTypeIndex(unsigned short resolve_type)
{
	/// gonna have to use lorge if statements cuz fuck you
	if (resolve_type & RESOLVE_TYPE_NO_FAKE)
		return 1;
	else if (resolve_type & RESOLVE_TYPE_DESYNC_STATIC)
		return 2;
	else if (resolve_type & RESOLVE_TYPE_DESYNC_JITTER)
		return 3;
	else if (resolve_type & RESOLVE_TYPE_BRUTEFORCE)
		return 4;
	else if (resolve_type & RESOLVE_TYPE_SLOWWALK)
		return 5;
	else if (resolve_type & RESOLVE_TYPE_ON_SHOT_BT)
		return 6;
	return 0;
}

int Resolver::GetShotsMissed(SDK::CBaseEntity* entity, unsigned short resolve_type)
{
	const int resolve_index = GetResolveTypeIndex(resolve_type);

	return player_resolve_records[entity->GetIndex()].shots_fired[resolve_index] - player_resolve_records[entity->GetIndex()].shots_hit[resolve_index];
}

int old_curtime;
int old_frametime;
int realtime;
void Resolver::UpdateEntityAnimtions(SDK::CBaseEntity* entity) {
	int simtime = entity->GetSimTime();


	//We dont really need a animfix for tick records as we are already setupbonening ticks
	entity->m_bClientSideAnimation() = true; //update Client Side animation
	auto curtime = INTERFACES::Globals->curtime;
	auto frametime = INTERFACES::Globals->frametime;
	auto frametimne = DWORD(INTERFACES::Globals->frametime);
	auto absoluteframtime = DWORD(INTERFACES::Globals->absoluteframetime);
	auto interp_amount = DWORD(INTERFACES::Globals->interpolation_amount);
	auto framecount = INTERFACES::Globals->framecount;
	auto tickcount = INTERFACES::Globals->tickcount;
	INTERFACES::Globals->realtime = simtime;
	INTERFACES::Globals->curtime = simtime;
	INTERFACES::Globals->frametime = INTERFACES::Globals->interval_per_tick;
	INTERFACES::Globals->absoluteframetime = INTERFACES::Globals->interval_per_tick;
	INTERFACES::Globals->framecount = TIME_TO_TICKS(simtime);
	INTERFACES::Globals->tickcount = TIME_TO_TICKS(simtime);
	INTERFACES::Globals->interpolation_amount = 0.0f;
	INTERFACES::Globals->curtime = entity->GetSimulationTime();
	INTERFACES::Globals->frametime = INTERFACES::Globals->interval_per_tick;

	if (entity->GetAnimState()->m_iLastClientSideAnimationUpdateFramecount >= TIME_TO_TICKS(simtime))
		entity->GetAnimState()->m_iLastClientSideAnimationUpdateFramecount = TIME_TO_TICKS(simtime) - 1;

	entity->UpdateClientSideAnimation();
	entity->m_bClientSideAnimation() = false;
	INTERFACES::Globals->curtime = old_curtime;
	INTERFACES::Globals->frametime = old_frametime;
	INTERFACES::Globals->realtime = realtime;
	INTERFACES::Globals->curtime = curtime;
	INTERFACES::Globals->frametime = frametimne;
	INTERFACES::Globals->absoluteframetime = absoluteframtime;
	INTERFACES::Globals->interpolation_amount = interp_amount;
	INTERFACES::Globals->framecount = framecount;
	INTERFACES::Globals->tickcount = tickcount;

	old_curtime = INTERFACES::Globals->curtime;
	old_frametime = INTERFACES::Globals->frametime;
	realtime = INTERFACES::Globals->realtime;
} 