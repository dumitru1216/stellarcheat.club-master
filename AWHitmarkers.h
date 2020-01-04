#pragma once

#include "includes.h"
#include "SDK/GameEvents.h"

struct pImpactInfo {

	float x, y, z;
	long long time;
};

struct pHitmarkerInfo {

	pImpactInfo pImpact;
	int alpha;
	int hsHit;
	int dmg_health;
};
struct DamageIndicator_t
{
	int iDamage;
	bool bInitialized;
	float flEraseTime;
	float flLastUpdate;
	SDK::CBaseEntity * Player;
	Vector Position;
};

class Hitmarker : SDK::IGameEventListener {

public:
	void Initialize();
	void Paint();

	void PaintDMG();

private:
	void FireGameEvent(SDK::IGameEvent* pEvent) override;
	int GetEventDebugID(void) override;
	void PlayerHurt(SDK::IGameEvent* pEvent);
	void BulletImpact(SDK::IGameEvent* pEvent);
	SDK::CBaseEntity* GetPlayer(int userid);
	std::vector<pImpactInfo> pImpacts;
	std::vector<pHitmarkerInfo> pHitmarkers;
	std::vector<DamageIndicator_t> DamageIndicator;
};

extern Hitmarker* pHitmarker;