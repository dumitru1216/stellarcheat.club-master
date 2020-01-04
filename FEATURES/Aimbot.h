#pragma once
#include "New\Backtrackingcpp.h"
#include "../includes.h"
namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
	class CBaseWeapon;
	struct mstudiobbox_t;
}

struct AimbotData_t
{
	AimbotData_t(SDK::CBaseEntity* player, const int& idx)
	{
		this->pPlayer = player;
		this->index = idx;
	}
	SDK::CBaseEntity*	pPlayer;
	int					index;
};
struct PlayerAimbotInfo
{
	int tick, extrapolted_ticks;
	Vector head_position, hitscan_position;
	Backtracking_Record backtrack_record;
	Vector multipoint;
	Vector aim_angles;
};

class CAimbot
{
public:
	int knife_hitbox(SDK::CBaseEntity* entity);
	void SelectTarget2();
	void autoknife(SDK::CUserCmd *cmd);
	int zeus_hitbox(SDK::CBaseEntity * entity);
	void autozeus(SDK::CUserCmd * cmd);
	void shoot_enemy(SDK::CUserCmd* cmd);
	bool good_backtrack_tick(int tick);
	float lag_fix();
	void GunCheck();
	void run_aimbot(SDK::CUserCmd * cmd);
	int SelectTarget(SDK::CUserCmd * cmd);
	void lby_backtrack(SDK::CUserCmd * pCmd, SDK::CBaseEntity * pLocal, SDK::CBaseEntity * pEntity);
	void aimware_is_shit();
	float accepted_inaccuracy(SDK::CBaseWeapon * weapon);
	void linear_extrapolations();
	bool can_shoot(SDK::CUserCmd * cmd);
	int BestEntityI;
	Vector get_hitbox_pos(SDK::CBaseEntity* entity, int hitbox_id);
	Vector multipoint(SDK::CBaseEntity * entity, SDK::CUserCmd * cmd);

	std::vector<AimbotData_t>	Entities;
private:
	std::vector<Vector> GetMultiplePointsForHitbox(SDK::CBaseEntity * local, SDK::CBaseEntity * entity, int iHitbox, VMatrix BoneMatrix[128]);
	PlayerAimbotInfo player_aimbot_info[64];
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

extern CAimbot* aimbot;