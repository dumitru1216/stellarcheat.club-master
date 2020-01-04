#pragma once
#pragma once
#include "../../SDK/CBaseEntity.h"


namespace SDK
{
	class IGameEvent;
}

class Resolver
{
public:
	Resolver();

	void DoFSN();
	void DoCM();

public:
	/// resolve types, they're "flags" so to speak since a player can have more than 1 resolve type at once
	/// if angles overlap
	static const unsigned int RESOLVE_TYPE_NUM = 8,
		RESOLVE_TYPE_BRUTEFORCE = 0b1,
		RESOLVE_TYPE_SLOWWALK = 0b10,
		RESOLVE_TYPE_DESYNC_STATIC = 0b100,
		RESOLVE_TYPE_DESYNC_JITTER = 0b1000,
		RESOLVE_TYPE_NO_FAKE = 0b10000,
		RESOLVE_TYPE_ON_SHOT_BT = 0b100000;

public:
	/// a struct holding info the resolver needs, updated every frame for every player
	class PlayerResolveRecord
	{
	public:
		PlayerResolveRecord()
		{
			resolve_type = 0;

			resolved_eye_pos = 0.f;
			slowwalk_resolved_eye_pos = 0.f;
			resolved_goal_feet_yaw = 0.f; 
			on_shot_eye_curtime = 0.f;


			is_desyncing_static = false;
			is_desyncing_jitter = false;
			is_dormant = false, is_last_moving_lby_delta_valid = false;
			is_last_moving_lby_valid = false, is_slowwalking = false;
			is_balance_adjust_triggered = false, is_balance_adjust_playing = false;
			did_lby_flick = false, did_predicted_lby_flick = false;
			can_onshot = false;


			for (int i = 0; i < RESOLVE_TYPE_NUM; i++)
			{
				shots_hit[i] = 0;
				shots_fired[i] = 0;
			}
		}

	public:

	public:
		SDK::CAnimationLayer anim_layers[15];
		Vector resolved_angles, networked_angles, on_shot_eye_postion;;

		Vector velocity, origin;

		int shots_hit[RESOLVE_TYPE_NUM], shots_fired[RESOLVE_TYPE_NUM];
		int shots_missed_moving_lby, shots_missed_moving_lby_delta;
		unsigned short resolve_type;

		float lower_body_yaw;
		float last_moving_lby;
		float last_moving_lby_delta;
		float last_balance_adjust_trigger_time;
		float last_time_moving;
		float last_time_down_pitch;
		float next_predicted_lby_update;
		float resolved_eye_pos;
		float slowwalk_resolved_eye_pos;
		float resolved_goal_feet_yaw;
		float on_shot_eye_curtime;

		bool can_onshot;
		bool is_dormant;
		bool is_last_moving_lby_valid;
		bool is_slowwalking;
		bool is_last_moving_lby_delta_valid;
		bool is_balance_adjust_triggered, is_balance_adjust_playing;
		bool did_lby_flick, did_predicted_lby_flick;
		bool is_desyncing_static, is_desyncing_jitter;
	};

	/// a snapshot holding info about the moment you shot, used to count shots missed / hit
	struct ShotSnapshot
	{
		SDK::CBaseEntity* entity; /// person we shot at
		PlayerResolveRecord resolve_record; /// their resolve record when we shot

		float time; /// time when snapshot was created
		float first_processed_time; /// time when the shot was first processed
		bool was_shot_processed;
		int hitgroup_hit;
	};

private:
	PlayerResolveRecord player_resolve_records[64];
	std::vector<ShotSnapshot> shot_snapshots;
	std::vector<Vector> last_eye_positions;

public:
	PlayerResolveRecord& GetPlayerResolveInfo(SDK::CBaseEntity* entity)
	{
		return player_resolve_records[entity->GetIndex()];
	}

	std::string TranslateResolveRecord(unsigned short resolve_type);
	CColor GetResolveColor(unsigned short resolve_type);
	int GetResolveTypeIndex(unsigned short resolve_type);
	int GetShotsMissed(SDK::CBaseEntity* entity, unsigned short resolve_type);

	void UpdateEntityAnimtions(SDK::CBaseEntity * entity);

	bool IsResolved(const unsigned short& resolve_type)
	{
		if (resolve_type & RESOLVE_TYPE_NO_FAKE ||
			resolve_type & RESOLVE_TYPE_ON_SHOT_BT ||
			resolve_type & RESOLVE_TYPE_DESYNC_STATIC)
			return true;

		return false;
	}

	bool IsSlowwalking(SDK::CBaseEntity* entity)
	{
		return player_resolve_records[entity->GetIndex()].is_slowwalking;
	}

	bool IsMovingOnGround(SDK::CBaseEntity* entity)
	{
		return player_resolve_records[entity->GetIndex()].velocity.Length2D() > 0.5f && entity->GetFlags() & FL_ONGROUND;
	}
	float GetDeltaTime(float simtime);
	bool IsYawSideways(SDK::CBaseEntity* entity, float yaw);
	float ResolveSlowwalk(SDK::CBaseEntity * entity);
	bool IsFakingYawStatic(SDK::CBaseEntity * entity);
	bool IsFakingYawJitter(SDK::CBaseEntity * entity);

	/// pushback a record onto the shot snapshot queue
	void AddShotSnapshot(SDK::CBaseEntity* entity, PlayerResolveRecord resolve_record);

	void CheckForOnShot(SDK::CBaseEntity * entity, SDK::IGameEvent * game_event);

	void EventCallback(SDK::IGameEvent* game_event);

private:
	void ProcessSnapShots();

	void UpdateResolveRecord(SDK::CBaseEntity* entity);

	void ResolverFeetYaw(SDK::CBaseEntity* entity);
	void ResolveYaw(SDK::CBaseEntity* entity);
	float ResolverDesyncStatic(SDK::CBaseEntity * entity);
	float ResolverDesyncJitter(SDK::CBaseEntity * entity);
	void ResolvePitch(SDK::CBaseEntity* entity);

	void ResolveYawBruteforce(SDK::CBaseEntity* entity);
};

extern Resolver resolver;

