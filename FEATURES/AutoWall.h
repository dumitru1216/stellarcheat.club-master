#pragma once

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
	struct Weapon_Info;
	class CSWeaponInfo;
}

class CAutowall
{
public:

	struct FireBulletData
	{
		FireBulletData(const Vector &eye_pos) : src(eye_pos)
		{
		}

		Vector						src;
		SDK::trace_t       enter_trace;
		Vector						direction;
		SDK::CTraceFilter  filter;
		float						trace_length;
		float						trace_length_remaining;
		float						current_damage;
		int							penetrate_count;
	};
	struct returninfo {
		int damage = -1;
		int hitgroup = -1;
		int walls = 4;
		bool did_penetrate_wall = false;
		float thickness = 1.f;

		SDK::CBaseEntity* ent = nullptr;
		Vector end = Vector();
	};
	returninfo autowall(Vector start, Vector end, SDK::CBaseEntity * from_ent, SDK::CBaseEntity * to_ent, int hitgroup);
	void TraceLine(Vector & absStart, Vector & absEnd, unsigned int mask, SDK::CBaseEntity * ignore, SDK::trace_t * ptr);
	void ClipTraceToPlayers(Vector & absStart, Vector absEnd, unsigned int mask, SDK::ITraceFilter * filter, SDK::trace_t * tr);
	void GetBulletTypeParameters(float & maxRange, float & maxDistance, char * bulletType, bool sv_penetration_type);
	bool IsBreakableEntity(SDK::CBaseEntity * entity);
	void ScaleDamage(SDK::trace_t & enterTrace, SDK::CSWeaponInfo * weaponData, float & currentDamage);
	bool TraceToExit(SDK::trace_t & enterTrace, SDK::trace_t & exitTrace, Vector startPosition, Vector direction);
	bool HandleBulletPenetration(SDK::CSWeaponInfo * weaponData, SDK::trace_t & enterTrace, Vector & eyePosition, Vector direction, int & possibleHitsRemaining, float & currentDamage, float penetrationPower, bool sv_penetration_type, float ff_damage_reduction_bullets, float ff_damage_bullet_penetration);
	bool FireBullet(SDK::CBaseWeapon * pWeapon, Vector & direction, float & currentDamage);
	float CanHit(Vector & point);
	float CanHit1(Vector & point, SDK::CBaseEntity * pBaseEntity);
	bool SimulateFireBullet(FireBulletData & data);
	bool PenetrateWall(SDK::CBaseEntity * pBaseEntity, Vector & vecPoint);
	bool CanWallbang(float & dmg);
};

extern CAutowall* AutoWall;
//extern CAutowall g_Autowall;