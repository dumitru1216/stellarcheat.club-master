#pragma once

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}
struct EntityBoxInfo_t
{
	int x, y, w, h, alpha, alpha_anim;
	float dormancy = 0.f;

	int hp = -1;
	int hpDifference = 0;

	float hpDiffTime = 0.f;

	SDK::CBaseEntity* pEnt;
};
class Box;
class CVisuals
{
public:
	//void Drawmodels();
	void DrawTaserDist();
	void Draw();
	void WorldDraw();
	bool PrecacheModel(const char* szModelName);
	void ClientDraw();
	void Drawmodels();
	void Drawmodelsweapons();
	void CustomModels(SDK::CBaseEntity* entity);
	void CustomModelsweapons(SDK::CBaseWeapon* entity);
	void apply_clantag();
	void DrawInaccuracy();
	void DrawBulletTracers();
	void DrawBulletBeams();
	void ModulateWorld();
	void DrawDroppedGranades(SDK::CBaseEntity* entity);
	void ModulateSky();
	struct inferno_t : public SDK::CBaseEntity
	{
		float get_spawn_time()
		{
			return *reinterpret_cast<float*>(uintptr_t(this) + 0x20);
		}

		static float get_expiry_time()
		{
			return 7.f;
		}
	};
	struct smoke_t : public SDK::CBaseEntity
	{
		float get_spawn_time()
		{
			return *reinterpret_cast<float*>(uintptr_t(this) + 0x20);
		}

		static float get_expiry_time()
		{
			return 18.f;
		}
	};
	struct damage_indicator_t {
		int dmg;
		bool initializes;
		float earse_time;
		float last_update;
		SDK::CBaseEntity * player;
		Vector Position;
	};
	std::vector<damage_indicator_t> dmg_indicator;
	struct headshot_indicator {
		bool initializes;
		float earse_time;
		float last_update;
		SDK::CBaseEntity * player;
		Vector Position;
	};
	std::vector<headshot_indicator> hs_indicator;
	void viewmodelxyz();
	void LagCompHitbox(SDK::CBaseEntity* entity, int index);
	void fakeanglehitbox(SDK::CBaseEntity * pEntity, int r, int g, int b, int a, float duration);
	void DrawBoundingBox(EntityBoxInfo_t inf);
	void DrawWeaponName(EntityBoxInfo_t inf);
	void set_hitmarker_time(float time);
	void set_hitmarker_timekill(float time);
	void set_hitmarker_timekillmulti(float time);
	void LogEvents();
	void DrawDamageIndicator();
	void DrawHSIndicator();
	void watermark();
	void Clantag();
private:
	void idk(SDK::CBaseEntity * entity);
	void DrawBox(SDK::CBaseEntity* entity, CColor color, Vector pos, Vector top);
	void KillNotfication();
	void KillNotficationMulti();
	void DrawSkeleton(SDK::CBaseEntity* entity, CColor color);
	void DrawName(SDK::CBaseEntity* entity, CColor color, int index, Vector pos, Vector top);
	void DrawBacktrackSkeleton(SDK::CBaseEntity * entity, CColor color, const matrix3x4_t * bone_matrix);
	void DrawLagCompSkeleton(SDK::CBaseEntity * entity, CColor color, const matrix3x4_t * bone_matrix, int index);
	void DrawLagCompSkel();
	void DrawWeapon(SDK::CBaseEntity * entity, CColor color, int index, Vector pos, Vector top);
	void DrawPlantedBob(SDK::CBaseEntity * entity);
	void DrawWeaponIcon(SDK::CBaseEntity* entity, CColor color, int index, Vector pos, Vector top);
	void GrenadePred(SDK::CBaseEntity* entity);
	void DrawWeapon1(SDK::CBaseEntity * entity, CColor color, int index, Vector pos, Vector top);
	void DrawWeaponName(SDK::CBaseEntity * entity, CColor dist, CColor icon, CColor text, int index, Vector pos, Vector top);
	void DrawHealth(SDK::CBaseEntity * entity, CColor color, CColor dormant, Vector pos, Vector top);
	void BombPlanted(SDK::CBaseEntity * entity);
	void espPreview();
	void KeyStorks();
	void DrawLBY(SDK::CBaseEntity * entity, CColor color, CColor dormant);
	void DrawCircleLBY(CColor color);
	void DrawCircleTimer(int x, int y, int size, int full, float amount_full, CColor fill);
	void DrawDroppedIcons(SDK::CBaseEntity* entity);
	void DrawDropped(SDK::CBaseEntity * entity, Vector pos, Vector top);
	void DrawFireGranades(SDK::CBaseEntity * entity, Vector pos, Vector pos3D, Vector top);
	void DrawSmokeGranades(SDK::CBaseEntity * entity, Vector pos, Vector pos3D, Vector top);
	void DrawGranades(SDK::CBaseEntity* entity);
	void awDrawCrosshair();
	void idk();
	void FreeLook(float frametime, SDK::CUserCmd * cmd, SDK::CBaseEntity * pEntity);
	void Footstep();
	void DrawAmmo(SDK::CBaseEntity * entity, CColor color, CColor dormant, Vector pos, Vector top);
	void DrawArmor(SDK::CBaseEntity * entity, CColor color, CColor dormant, Vector pos, Vector top);
	float resolve_distance(Vector src, Vector dest);
	void DrawDistance(SDK::CBaseEntity * entity, CColor color, Vector pos, Vector top);
	void DrawWeaponIcon1(SDK::CBaseEntity* entity, CColor color, int index, Vector pos, Vector top);
	void DrawWeaponIcon2(SDK::CBaseEntity * entity, CColor color, int index, Vector pos, Vector top);
	void DrawInfo(SDK::CBaseEntity * entity, CColor color, CColor alt, Vector pos, Vector top);
	void DrawFovArrows(SDK::CBaseEntity* entity, CColor color);
	void DroppedAmmo(SDK::CBaseWeapon* entity, CColor color, CColor dormant);
	void DrawInaccuracy1();
	void DrawCrosshair();
	void DrawIndicator();	
	void DrawIndicator1();
	void speclist();
	void DrawHitmarker();

	void DrawCircleWithoutTimer(CColor color, float value, float full_value, int x, int y, std::string name);




	void DrawBorderLines();
public:
	std::vector<std::pair<int, float>>				Entities;
	std::deque<UTILS::BulletImpact_t>				Impacts;
};
class Hitmarkers
{
public:
	void Do();
	void AddHitmarker(int damage);

private:
	struct Hitmarker
	{
		Hitmarker(float time, int damage)
		{
			m_time = time;
			m_damage = damage;
		}

		float m_time;
		int m_damage;
	};

private:
	std::vector<Hitmarker> hitmarker_vector;
private:
	const float hitmarker_fade_time = 0.2f;
	const float hitmarker_damage_fade_time = 2.f;
	const int hitmarker_damage_float_height = 150;
};

extern Hitmarkers hitmarkers;

extern CVisuals* visuals;
