#pragma once
#include <vector>
#include "../includes.h"

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}
struct LAimbotData_t
{
	LAimbotData_t(SDK::CBaseEntity* player, const int& idx)
	{
		this->pPlayer = player;
		this->index = idx;
	}
	SDK::CBaseEntity*	pPlayer;
	int					index;
};
class LegitBot
{
public:
	void Run(SDK::CUserCmd * cmd);
	void TriggerBot(SDK::CUserCmd * cmd);
	void Smooth(SDK::CBaseEntity * player, Vector angle, SDK::CUserCmd * cmd);
	void SelectTarget();
	int closest_to_crosshair();
	int GetClosest();
	int GetBestTarget();
private:
	std::vector<LAimbotData_t> Entities;
	void DoAimbot(SDK::CUserCmd *pCmd);
	void GunCheck();
	Vector point(SDK::CBaseEntity * entity, Vector & vec_out);
	void RCS(SDK::CUserCmd* cmd);
	void Move(SDK::CUserCmd * pCmd);
	bool Enable;
	bool Silent;
	int FOV;
	int Smoth;
	int Delay;
	int hitchance;
	int rcs_x;
	int rcs_y;
	bool rcs;
	bool hitboxes[5];

};

extern LegitBot* legit;


struct legit_backtrackdata
{
	float simtime;
	Vector hitboxPos;
};
extern legit_backtrackdata headPositions[64][12];