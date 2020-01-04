#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/ConVar.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/IViewRenderBeams.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseWeapon.h"
#include "../FEATURES/AutoWall.h"
#include "../SDK/CTrace.h"	
#include "../HOOKS/hooks.h"
#include "../SDK/CGlobalVars.h"
#include "../FEATURES/AntiAim.h"
#include "../FEATURES/Aimbot.h"
#include "../SDK/NetChannel.h"
#include "../FEATURES/Visuals.h"
#include "../UTILS/render.h"
#include "../SDK/IVDebugOverlay.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/Collideable.h"
#include "../SDK/CInput.h"
#include "New\NewResolver.h"
#include "NewEventLog.h"
#include "../SDK/INetChannelInfo.h"
#include <string.h>
//--- Misc Variable Initalization ---//
#ifdef ENABLE_XOR
#define XorStr _xor_ 
#else
#define XorStr
#endif
int alpha[65];

float flPlayerAlpha[255];
CColor breaking;
CColor bt;
static bool bPerformed = false, bLastSetting;
float fade_alpha[65];
float dormant_time[65];
CColor main_color;
CColor ammo;
SDK::CBaseEntity *BombCarrier;

bool IsInGame()
{
	return INTERFACES::Engine->IsConnected() && INTERFACES::Engine->IsInGame();
}
int getping()
{
	if (!IsInGame())
		return 0;

	auto nci = INTERFACES::Engine->GetNetChannelInfo();
	if (!nci)
		return 0;

	float m_AvgLatency = nci->GetAvgLatency(0);

	return (int)(m_AvgLatency * 1000.0f);
}

int getfps()
{
	return static_cast<int>(1.f / INTERFACES::Globals->frametime);
}
void CVisuals::set_hitmarker_time(float time)
{
	global::flHurtTime = time;
}
void CVisuals::set_hitmarker_timekill(float time)
{
	global::flHurtTimeKill = time;
}
void CVisuals::set_hitmarker_timekillmulti(float time)
{
	global::flHurtTimeKillmulti = time;
}
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
void CVisuals::Draw()
{
	static SDK::ClientClass* pClass;
	if (!INTERFACES::Engine->IsInGame()) {
		global::flHurtTime = 0.f;
		global::flHurtTimeKill = 0.f;

		return;
	}
	if (SETTINGS::settings.Viewmodelchanger)
		viewmodelxyz();
	DrawCrosshair();
	for (int i = 1; i <= INTERFACES::Globals->maxclients; i++)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!entity) continue;
		if (!local_player) continue;

		bool is_local_player = entity == local_player;
		bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

		if (entity->GetHealth() <= 0) continue;
		if (entity->GetVecOrigin() == Vector(0, 0, 0)) continue;
		if(is_local_player) continue;

		//--- Colors ---//
		if (entity->GetIsDormant() && flPlayerAlpha[i] > 0) flPlayerAlpha[i] -= .3;
		else if (!entity->GetIsDormant() && flPlayerAlpha[i] < 255) flPlayerAlpha[i] = 255;

		float playerAlpha = flPlayerAlpha[i];
		int enemy_hp = entity->GetHealth();
		int hp_red = 255 - (enemy_hp * 2.55);
		int hp_green = enemy_hp * 2.55;
		CColor health_color = CColor(hp_red, hp_green, 1, playerAlpha);
		CColor dormant_color = CColor(100, 100, 100, playerAlpha);
		CColor alt_color, still_health, zoom_color;
		CColor enemy_box, enemy_ammo, enemy_armor, enemy_name ,enemy_skeleton, enemy_btskeleton, enemy_weapon_icon, enemy_weapon_text, enemy_weapon_dist;
		CColor team_box, team_ammo, team_armor, team_name, team_skeleton, team_weapon;
		CColor local_box, local_ammo, local_armor, local_name, local_skeleton, local_weapon;
		EntityBoxInfo_t info;

		static auto alpha = 0.f; static auto plus_or_minus = false;
		if (alpha <= 0.f || alpha >= 255.f) plus_or_minus = !plus_or_minus;
		alpha += plus_or_minus ? (255.f / 7 * 0.015) : -(255.f / 7 * 0.015); alpha = clamp(alpha, 0.f, 255.f);

		if (entity->GetIsDormant())
		{
			enemy_box = dormant_color;
			enemy_ammo = dormant_color;
			enemy_armor = dormant_color;
			enemy_name = dormant_color;
			enemy_skeleton = CColor(0,0,0,0);
			enemy_btskeleton = CColor(0, 0, 0, 0);
			enemy_weapon_icon = dormant_color;
			enemy_weapon_text = dormant_color;
			enemy_weapon_dist = dormant_color;


			still_health = health_color;
			alt_color = CColor(20, 20, 20, playerAlpha);
			main_color = dormant_color;
			zoom_color = dormant_color;
			bt = dormant_color;
		}
		else if (!entity->GetIsDormant())
		{
			enemy_box = CColor(Config::Visuals::Main::Visualsmain::get().box_color_r, Config::Visuals::Main::Visualsmain::get().box_color_g, Config::Visuals::Main::Visualsmain::get().box_color_b, Config::Visuals::Main::Visualsmain::get().box_color_a);
			enemy_ammo = CColor(Config::Visuals::Main::Visualsmain::get().ammobar_color_r, Config::Visuals::Main::Visualsmain::get().ammobar_color_g, Config::Visuals::Main::Visualsmain::get().ammobar_color_b, Config::Visuals::Main::Visualsmain::get().ammobar_color_a);
			enemy_armor = CColor(Config::Visuals::Main::Visualsmain::get().armorbar_color_r, Config::Visuals::Main::Visualsmain::get().armorbar_color_g, Config::Visuals::Main::Visualsmain::get().armorbar_color_b, Config::Visuals::Main::Visualsmain::get().armorbar_color_a);
			enemy_name = CColor(Config::Visuals::Main::Visualsmain::get().name_color_r, Config::Visuals::Main::Visualsmain::get().name_color_g, Config::Visuals::Main::Visualsmain::get().name_color_b, Config::Visuals::Main::Visualsmain::get().name_color_a);
			enemy_skeleton = CColor(Config::Visuals::Main::Visualsmain::get().skeleton_color_r, Config::Visuals::Main::Visualsmain::get().skeleton_color_g, Config::Visuals::Main::Visualsmain::get().skeleton_color_b, Config::Visuals::Main::Visualsmain::get().skeleton_color_a);
			enemy_btskeleton = CColor(Config::Visuals::Main::Visualsmain::get().btskeleton_color_r, Config::Visuals::Main::Visualsmain::get().btskeleton_color_g, Config::Visuals::Main::Visualsmain::get().btskeleton_color_b, Config::Visuals::Main::Visualsmain::get().btskeleton_color_a);
			enemy_weapon_icon = CColor(CColor(Config::Visuals::Main::Visualsmain::get().weaponicon_color_r, Config::Visuals::Main::Visualsmain::get().weaponicon_color_g, Config::Visuals::Main::Visualsmain::get().weaponicon_color_b, Config::Visuals::Main::Visualsmain::get().weaponicon_color_a));
			enemy_weapon_text = CColor(CColor(Config::Visuals::Main::Visualsmain::get().weaponname_color_r, Config::Visuals::Main::Visualsmain::get().weaponname_color_g, Config::Visuals::Main::Visualsmain::get().weaponname_color_b, Config::Visuals::Main::Visualsmain::get().weaponname_color_a));
			enemy_weapon_dist = CColor(CColor(Config::Visuals::Main::Visualsmain::get().distance_color_r, Config::Visuals::Main::Visualsmain::get().distance_color_g, Config::Visuals::Main::Visualsmain::get().distance_color_b, Config::Visuals::Main::Visualsmain::get().distance_color_a));

			main_color = CColor(255, 255, 255, playerAlpha);
			still_health = health_color;
			alt_color = CColor(0, 0, 0, 165);
			zoom_color = CColor(150, 150, 220, 165);
			breaking = CColor(220, 150, 150, 165);
			bt = CColor(155, 220, 150, 165);
		}
		Vector min, max, pos, pos3D, top, top3D; entity->GetRenderBounds(min, max);
		pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10); top3D = pos3D + Vector(0, 0, max.z + 10);

		if (RENDER::WorldToScreen(pos3D, pos) && RENDER::WorldToScreen(top3D, top))
		{
			if (!is_teammate && !is_local_player)
			{
				if (Config::Visuals::Main::Visualsmain::get().Enable) {
					if (Config::Visuals::Main::Visualsmain::get().Box) DrawBox(entity, enemy_box, pos, top);
					if (Config::Visuals::Main::Visualsmain::get().Name) DrawName(entity, enemy_name, i, pos, top);
					if (Config::Visuals::Main::Visualsmain::get().HealthBar) DrawHealth(entity, still_health, alt_color, pos, top);
					if (Config::Visuals::Main::Visualsmain::get().AmmoBar) DrawAmmo(entity, enemy_ammo, alt_color, pos, top);
					if (Config::Visuals::Main::Visualsmain::get().ArmorBar) DrawArmor(entity, enemy_armor, alt_color, pos, top);
					if (Config::Visuals::Main::Visualsmain::get().Skeleton) DrawSkeleton(entity, enemy_skeleton);
					if (Config::Visuals::Main::Visualsmain::get().BacktrackSkeleton) {
						auto backtrack_records = backtracking.GetPriorityRecords(entity);
						for (const auto& record : backtrack_records)
						{
							/// skeleton
							DrawBacktrackSkeleton(entity, enemy_btskeleton, record.bone_matrix);
						}
					}
					DrawWeaponName(entity, enemy_weapon_dist, enemy_weapon_icon, enemy_weapon_text, i, pos, top);
					DrawFovArrows(entity, CColor(Config::Visuals::Main::Visualsmain::get().arrows_color_r, Config::Visuals::Main::Visualsmain::get().arrows_color_g, Config::Visuals::Main::Visualsmain::get().arrows_color_b, Config::Visuals::Main::Visualsmain::get().arrows_color_a));
				}
			}
			if (is_teammate && !is_local_player)
			{
				if (Config::Visuals::Main::Visualsmain::get().Enable && Config::Visuals::Main::Visualsmain::get().Team) {
					if (Config::Visuals::Main::Visualsmain::get().Box) DrawBox(entity, enemy_box, pos, top);
					if (Config::Visuals::Main::Visualsmain::get().Name) DrawName(entity, enemy_name, i, pos, top);
					if (Config::Visuals::Main::Visualsmain::get().HealthBar) DrawHealth(entity, still_health, alt_color, pos, top);
					if (Config::Visuals::Main::Visualsmain::get().AmmoBar) DrawAmmo(entity, enemy_ammo, alt_color, pos, top);
					if (Config::Visuals::Main::Visualsmain::get().ArmorBar) DrawArmor(entity, enemy_armor, alt_color, pos, top);
					if (Config::Visuals::Main::Visualsmain::get().Skeleton) DrawSkeleton(entity, enemy_skeleton);

					DrawWeaponName(entity, enemy_weapon_dist, enemy_weapon_icon, enemy_weapon_text, i, pos, top);
					DrawFovArrows(entity, CColor(Config::Visuals::Main::Visualsmain::get().arrows_color_r, Config::Visuals::Main::Visualsmain::get().arrows_color_g, Config::Visuals::Main::Visualsmain::get().arrows_color_b, Config::Visuals::Main::Visualsmain::get().arrows_color_a));
				}
			}
			DrawInfo(entity, main_color, zoom_color, pos, top);

		}

		if (SETTINGS::settings.bullet_tracer_combo == 8)
			DrawBulletTracers();
	}

	if (Config::Visuals::Main::VisualsEffects::get().DrawLC == 2) {
		for(int i = 0; i < nel->lag_comp.size(); i++)
			DrawLagCompSkeleton(nel->lag_comp[i].Player, CColor(255, 255, 255, 255), nel->lag_comp[i].pmatrix, i);
	}


	WorldDraw();

	if(Config::Visuals::Main::Visualsworld::get().Spectators)
		speclist();
}
void CVisuals::WorldDraw() {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	for (int i = 0; i < INTERFACES::ClientEntityList->GetHighestEntityIndex(); i++) {
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (!entity)continue;
		auto client_class = entity->GetClientClass();
		Vector min, max, pos, pos3D, top, top3D; entity->GetRenderBounds(min, max);
		pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10); top3D = pos3D + Vector(0, 0, max.z + 10);
		if (RENDER::WorldToScreen(pos3D, pos) && RENDER::WorldToScreen(top3D, top))
		{
			if (client_class->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && client_class->m_ClassID != (int)CSGOClassID::CCSPlayer && ((strstr(client_class->m_pNetworkName, "Weapon") || client_class->m_ClassID == (int)CSGOClassID::CDEagle || client_class->m_ClassID == (int)CSGOClassID::CAK47))) {
				DrawDropped(entity, pos, top);
			}
		}
		if (Config::Visuals::Main::Visualsworld::get().GranadeWaring) {
			DrawGranades(entity);
			DrawFireGranades(entity, pos, pos3D, top);
			DrawSmokeGranades(entity, pos, pos3D, top);
		}
		if (Config::Visuals::Main::Visualsworld::get().DroppedGrenades || Config::Visuals::Main::Visualsworld::get().Bomb) {
			DrawDroppedGranades(entity);
			BombPlanted(entity);
		}	


	}

}
void CVisuals::ClientDraw()
{
	if (SETTINGS::settings.lby_bar) DrawCircleLBY(CColor(255, 255, 255, 255));
	if (Config::Visuals::Main::VisualsEffects::get().RemoveEffect[2] == 1) DrawBorderLines();
	if (Config::Visuals::Main::Visualsworld::get().Crosshair)
		INTERFACES::cvar->FindVar("crosshair")->SetValue(1);
	else
		INTERFACES::cvar->FindVar("crosshair")->SetValue(0);
	if (Config::Visuals::Main::Visualsworld::get().SpreadCircle)
		DrawInaccuracy();

	DrawIndicator();
	if (Config::Visuals::Main::VisualsEffects::get().HitEffect[2])
		hitmarkers.Do();

	if (Config::Visuals::Main::Visualsmain::get().screen[3]) {
		DrawCircleWithoutTimer(CColor(255, 255, 255), INTERFACES::Engine->GetNetChannel()->m_nChokedPackets, 16, 50, 500, "Chocked");
		DrawCircleWithoutTimer(CColor(0, 255, 21), getping(), 100, 50, 580, "Ping");
	}

	if(Config::Visuals::Main::Visualsmain::get().screen[0])
		KeyStorks();
}
void CVisuals::Footstep()
{
	SDK::BeamInfo_t beam_info;
	beam_info.m_flStartRadius = 10;
	beam_info.m_flEndRadius = 285;

	beam_info.m_pszModelName = "sprites/physbeam.vmt";
	beam_info.m_nModelIndex = -1;

	beam_info.m_nStartFrame = 0;
	beam_info.m_flFrameRate = 10;

	beam_info.m_flLife = 0.6;
	beam_info.m_flWidth = 5;
	beam_info.m_flAmplitude = 0;

	beam_info.m_flSpeed = 10;
	beam_info.m_nFlags = 0;
	beam_info.m_flFadeLength = 0;
	beam_info.m_bRenderable = true;


	auto beam = INTERFACES::ViewRenderBeams->CreateBeamPoints(beam_info);
	if (beam) INTERFACES::ViewRenderBeams->DrawBeam(beam);


}

std::string str_to_upper(std::string strToConvert)
{
	std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);

	return strToConvert;
}

void CVisuals::DrawBox(SDK::CBaseEntity* entity, CColor color, Vector pos, Vector top)
{
	float alpha = flPlayerAlpha[entity->GetIndex()];
	int height = (pos.y - top.y), width = height / 2;

	RENDER::DrawEmptyRect(pos.x - width / 2, top.y, (pos.x - width / 2) + width, top.y + height, color);
	RENDER::DrawEmptyRect((pos.x - width / 2) + 1, top.y + 1, (pos.x - width / 2) + width - 1, top.y + height - 1, CColor(20, 20, 20, alpha));
	RENDER::DrawEmptyRect((pos.x - width / 2) - 1, top.y - 1, (pos.x - width / 2) + width + 1, top.y + height + 1, CColor(20, 20, 20, alpha));
}

void CVisuals::DrawName(SDK::CBaseEntity* entity, CColor color, int index, Vector pos, Vector top)
{
	SDK::player_info_t ent_info; INTERFACES::Engine->GetPlayerInfo(index, &ent_info);

	int height = (pos.y - top.y), width = height / 2;
	RENDER::DrawF(pos.x, top.y - 7, FONTS::visuals_esp_font, true, true, color, ent_info.szName);
}
void CVisuals::DrawBacktrackSkeleton(SDK::CBaseEntity* entity, CColor color, const matrix3x4_t* bone_matrix)
{
	auto studio_model = INTERFACES::ModelInfo->GetStudioModel(entity->GetModel());
	if (!studio_model)
	return;

	for (int i = 0; i < studio_model->numbones; i++)
	{
		auto bone = studio_model->pBone(i);
		if (!bone || bone->parent < 0 || !(bone->flags & BONE_USED_BY_HITBOX))
			continue;

		/// 2 bone matrices, 1 for the child and 1 for the parent bone
		matrix3x4_t bone_matrix_1, bone_matrix_2;
		if (bone_matrix)
		{
			bone_matrix_1 = bone_matrix[i];
			bone_matrix_2 = bone_matrix[bone->parent];
		}
		else
		{
			bone_matrix_1 = entity->GetBoneMatrix(i);
			bone_matrix_2 = entity->GetBoneMatrix(bone->parent);
		}

		Vector bone_position_1 = Vector(bone_matrix_1[0][3], bone_matrix_1[1][3], bone_matrix_1[2][3]),
			bone_position_2 = Vector(bone_matrix_2[0][3], bone_matrix_2[1][3], bone_matrix_2[2][3]);
		Vector screen1, screen2;
		if (RENDER::WorldToScreen(bone_position_1, screen1) && RENDER::WorldToScreen(bone_position_2, screen2))
			RENDER::DrawLine(screen1.x, screen1.y, screen2.x, screen2.y, color);
	}
}
void CVisuals::DrawLagCompSkeleton(SDK::CBaseEntity* entity, CColor color, const matrix3x4_t* bone_matrix, int index)
{



	if (!entity || entity == nullptr || !(entity->GetClientClass()->m_ClassID == (int)CSGOClassID::CCSPlayer))
		return;


	auto local_player = static_cast<SDK::CBaseEntity*>(INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer()));
	if (!local_player)
		return;


	float CurrentTime = local_player->GetTickBase() * INTERFACES::Globals->interval_per_tick;

	if (nel->lag_comp[index].flEraseTime < CurrentTime) {
		nel->lag_comp[index].flEraseTime = 0;
		return;
	}

	auto studio_model = INTERFACES::ModelInfo->GetStudioModel(entity->GetModel());
	if (!studio_model)
		return;

	CColor c1 = CColor(255, 255, 255, (nel->lag_comp[index].flEraseTime - CurrentTime) * 50);

	for (int i = 0; i < studio_model->numbones; i++)
	{
		auto bone = studio_model->pBone(i);
		if (!bone || bone->parent < 0 || !(bone->flags & BONE_USED_BY_HITBOX))
			continue;

		/// 2 bone matrices, 1 for the child and 1 for the parent bone
		matrix3x4_t bone_matrix_1, bone_matrix_2;
		if (bone_matrix)
		{
			bone_matrix_1 = bone_matrix[i];
			bone_matrix_2 = bone_matrix[bone->parent];
		}
		else
		{
			bone_matrix_1 = entity->GetBoneMatrix(i);
			bone_matrix_2 = entity->GetBoneMatrix(bone->parent);
		}

		Vector bone_position_1 = Vector(bone_matrix_1[0][3], bone_matrix_1[1][3], bone_matrix_1[2][3]),
			bone_position_2 = Vector(bone_matrix_2[0][3], bone_matrix_2[1][3], bone_matrix_2[2][3]);
		Vector screen1, screen2;
		if (RENDER::WorldToScreen(bone_position_1, screen1) && RENDER::WorldToScreen(bone_position_2, screen2))
			RENDER::DrawLine(screen1.x, screen1.y, screen2.x, screen2.y, c1);
	}
}
void CVisuals::DrawSkeleton(SDK::CBaseEntity* entity, CColor color) //wut hek
{
	matrix3x4_t bone_matrix[128];
	if (!entity->SetupBones2(bone_matrix, 128, BONE_USED_BY_ANYTHING, 0))
		return;

	auto studio_model = INTERFACES::ModelInfo->GetStudioModel(entity->GetModel());
	if (!studio_model)
		return;

	for (int i = 0; i < studio_model->numbones; i++)
	{
		auto bone = studio_model->pBone(i);
		if (!bone || bone->parent < 0 || !(bone->flags & BONE_USED_BY_HITBOX))
			continue;

		/// 2 bone matrices, 1 for the child and 1 for the parent bone
		matrix3x4_t bone_matrix_1, bone_matrix_2;
		if (bone_matrix)
		{
			bone_matrix_1 = bone_matrix[i];
			bone_matrix_2 = bone_matrix[bone->parent];
		}
		else
		{
			bone_matrix_1 = entity->GetBoneMatrix(i);
			bone_matrix_2 = entity->GetBoneMatrix(bone->parent);
		}

		Vector bone_position_1 = Vector(bone_matrix_1[0][3], bone_matrix_1[1][3], bone_matrix_1[2][3]),
			bone_position_2 = Vector(bone_matrix_2[0][3], bone_matrix_2[1][3], bone_matrix_2[2][3]);

		Vector screen1, screen2;
		if (RENDER::WorldToScreen(bone_position_1, screen1) && RENDER::WorldToScreen(bone_position_2, screen2))
			RENDER::DrawLine(screen1.x, screen1.y, screen2.x, screen2.y, color);
	}
	SDK::studiohdr_t* pStudioModel = INTERFACES::ModelInfo->GetStudioModel(entity->GetModel());
	if (!pStudioModel)
		return;

	matrix3x4_t pBoneToWorldOut[128];
	if (!entity->SetupBones2(pBoneToWorldOut, 128, BONE_USED_BY_ANYTHING, 0))
		return;

	SDK::mstudiohitboxset_t* pHitboxSet = pStudioModel->pHitboxSet(0);
	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioModel->numbones; j++)
	{
		SDK::mstudiobone_t* pBone = pStudioModel->pBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = entity->GetBonePosition(j);
			vParent = entity->GetBonePosition(pBone->parent);

			int iChestBone = 6;  // Parameter of relevant Bone number
			Vector vBreastBone; // New reference Point for connecting many bones
			Vector vUpperDirection = entity->GetBonePosition(iChestBone + 1) - entity->GetBonePosition(iChestBone); // direction vector from chest to neck
			vBreastBone = entity->GetBonePosition(iChestBone) + vUpperDirection / 2;
			Vector vDeltaChild = vChild - vBreastBone; // Used to determine close bones to the reference point
			Vector vDeltaParent = vParent - vBreastBone;

			// Eliminating / Converting all disturbing bone positions in three steps.
			if ((vDeltaParent.Length() < 9 && vDeltaChild.Length() < 9))
				vParent = vBreastBone;

			if (j == iChestBone - 1)
				vChild = vBreastBone;

			if (abs(vDeltaChild.z) < 5 && (vDeltaParent.Length() < 5 && vDeltaChild.Length() < 5) || j == iChestBone)
				continue;

			INTERFACES::DebugOverlay->ScreenPosition(vParent, sParent);
			INTERFACES::DebugOverlay->ScreenPosition(vChild, sChild);

			INTERFACES::Surface->DrawSetColor(color);
			INTERFACES::Surface->DrawLine(sParent[0], sParent[1], sChild[0], sChild[1]);
		}
	}
	for (int i = 0; i < pStudioModel->numbones; i++)
	{
		SDK::mstudiobone_t* pBone = pStudioModel->pBone(i);
		if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
			continue;

		Vector vBonePos1;
		if (!RENDER::WorldToScreen(Vector(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1))
			continue;

		Vector vBonePos2;
		if (!RENDER::WorldToScreen(Vector(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
			continue;

		RENDER::DrawLine((int)vBonePos1.x, (int)vBonePos1.y, (int)vBonePos2.x, (int)vBonePos2.y, main_color);
	}
}
void CVisuals::DrawIndicator1() {

}
std::vector<int> GetObservervators1(int playerId)
{
	std::vector<int> SpectatorList;

	SDK::CBaseEntity* pPlayer = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntity(playerId);

	if (!pPlayer)
		return SpectatorList;

	if (!pPlayer->IsAlive())
	{
		SDK::CBaseEntity* pObserverTarget = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntityFromHandle(pPlayer->GetObserverTargetHandle());

		if (!pObserverTarget)
			return SpectatorList;

		pPlayer = pObserverTarget;
	}

	for (int PlayerIndex = 0; PlayerIndex < 65; PlayerIndex++)
	{
		SDK::CBaseEntity* pCheckPlayer = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntity(PlayerIndex);

		if (!pCheckPlayer)
			continue;

		if (pCheckPlayer->GetIsDormant() || pCheckPlayer->IsAlive())
			continue;

		SDK::CBaseEntity* pObserverTarget = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntityFromHandle(pCheckPlayer->GetObserverTargetHandle());

		if (!pObserverTarget)
			continue;

		if (pPlayer != pObserverTarget)
			continue;

		SpectatorList.push_back(PlayerIndex);
	}

	return SpectatorList;
}
void CVisuals::speclist()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	int speclistx = 1300, speclisty = 5; //speclist pos
	if (!INTERFACES::Engine->IsInGame() && !INTERFACES::Engine->IsConnected())
		return;

	int specs = 0;
	int modes = 0;
	std::string spect = "";
	std::string mode = "";
	int DrawIndex = 1;

	for (int playerId : GetObservervators1(INTERFACES::Engine->GetLocalPlayer()))
	{
		if (playerId == INTERFACES::Engine->GetLocalPlayer())
			continue;

		SDK::CBaseEntity* pPlayer = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntity(playerId);

		if (!pPlayer)
			continue;

		SDK::player_info_t Pinfo;
		INTERFACES::Engine->GetPlayerInfo(playerId, &Pinfo);

		char player_name[255] = { 0 };

		RENDER::DrawF(1300, speclisty, FONTS::visuals_esp_font, false, false, CColor(255, 255, 255, 150), Pinfo.szName);
		speclisty += 15;
	}
}
void CVisuals::LagCompHitbox(SDK::CBaseEntity* entity, int index)
{
	float duration = 3;


	if (index < 0)
		return;

	if (!entity)
		return;

	SDK::studiohdr_t* pStudioModel = INTERFACES::ModelInfo->GetStudioModel(entity->GetModel());

	if (!pStudioModel)
		return;

	SDK::mstudiohitboxset_t* pHitboxSet = pStudioModel->pHitboxSet(0);

	if (!pHitboxSet)
		return;

	for (int i = 0; i < pHitboxSet->numhitboxes; i++)
	{
		SDK::mstudiobbox_t* pHitbox = pHitboxSet->GetHitbox(i);

		if (!pHitbox)
			continue;

		auto bone_matrix = entity->GetBoneMatrix(pHitbox->bone);

		Vector vMin, vMax;

		MATH::VectorTransform(pHitbox->bbmin, bone_matrix, vMin);
		MATH::VectorTransform(pHitbox->bbmax, bone_matrix, vMax);

		if (pHitbox->radius > -1)
		{
			INTERFACES::DebugOverlay->AddCapsuleOverlay(vMin, vMax, pHitbox->radius, 255, 255, 255, 80, duration); // color green 
		}
	}
}
float CVisuals::resolve_distance(Vector src, Vector dest)
{
	Vector delta = src - dest;
	float fl_dist = ::sqrtf((delta.Length()));
	if (fl_dist < 1.0f) return 1.0f;
	return fl_dist;
}
void CVisuals::DrawDistance(SDK::CBaseEntity* entity, CColor color, Vector pos, Vector top)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	SDK::player_info_t ent_info;
	Vector vecOrigin = entity->GetVecOrigin(), vecOriginLocal = local_player->GetVecOrigin();

	char dist_to[32]; int height = (pos.y - top.y), width = height / 2;

	sprintf_s(dist_to, "%.0f ft", resolve_distance(vecOriginLocal, vecOrigin));
	RENDER::DrawF(pos.x, SETTINGS::settings.ammo_bool ? pos.y + 12 : pos.y + 8, FONTS::visuals_esp_font, true, true, color, dist_to);
}
void DrawLBYCircleTimer(int x, int y, int size, float amount_full, CColor fill)
{
	int texture = INTERFACES::Surface->CreateNewTextureID(true);
	INTERFACES::Surface->DrawSetTexture(texture);
	INTERFACES::Surface->DrawSetColor(fill);

	SDK::Vertex_t vertexes[100];
	for (int i = 0; i < 100; i++) {
		float angle = ((float)i / -100) * (M_PI * (2 * amount_full));
		vertexes[i].Init(Vector2D(x + (size * sin(angle)), y + (size * cos(angle))));
	}

	INTERFACES::Surface->DrawTexturedPolygon(100, vertexes, true);
}
void CVisuals::DrawCircleLBY(CColor color)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	int screen_width, screen_height;
	INTERFACES::Engine->GetScreenSize(screen_width, screen_height);

	static float next_lby_update[65];

	const float curtime = UTILS::GetCurtime();

	if (local_player->GetVelocity().Length2D() > 0.1)
		return;

	auto animstate = local_player->GetAnimState();
	if (!animstate)
		return;
	static float last_lby[65];
	if (local_player->GetHealth() > 0)
	{
		if (last_lby[local_player->GetIndex()] != local_player->GetLowerBodyYaw())
		{
			last_lby[local_player->GetIndex()] = local_player->GetLowerBodyYaw();
			next_lby_update[local_player->GetIndex()] = curtime + 1.1 + INTERFACES::Globals->interval_per_tick;
		}

		if (next_lby_update[local_player->GetIndex()] < curtime)
		{
			next_lby_update[local_player->GetIndex()] = curtime + 1.1;
		}

		float time_remain_to_update = next_lby_update[local_player->GetIndex()] - local_player->GetSimTime();
		float time_update = next_lby_update[local_player->GetIndex()];

		float fill;
		fill = (((time_remain_to_update)));
		static float add = 0.000f;
		add = 1.1 - fill;

		int iY = 88;

		iY += 6;

		RENDER::DrawFilledCircle(96, screen_height - iY, 12, 60, CColor(30, 30, 30));
		DrawLBYCircleTimer(96, screen_height - iY, 10, add, CColor(0, 190, 0, 255));
		RENDER::DrawFilledCircle(96, screen_height - iY, 7, 60, CColor(40, 40, 40));
	}
}
void CVisuals::DrawCircleTimer(int x, int y, int size,int full, float amount_full, CColor fill)
{;
	INTERFACES::Surface->DrawSetColor(fill);

	int amount = amount_full / 100;

	SDK::Vertex_t vertexes[100];
	for (int i = 0; i < 100; i++) {
		float angle = ((float)i / -100) * (M_PI * ((full / amount_full) * 2));
		vertexes[i].Init(Vector2D(x + (size * sin(angle)), y + (size * cos(angle))));
	}

	INTERFACES::Surface->DrawTexturedPolygon(100, vertexes, true);
}
void CVisuals::DrawCircleWithoutTimer(CColor color, float value, float full_value, int x , int y, std::string name )
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	int screen_width, screen_height;
	INTERFACES::Engine->GetScreenSize(screen_width, screen_height);

	static float last_lby[65];
	if (local_player->GetHealth() > 0)
	{

		float time_remain_to_update = value;

		float fill;
		fill = (((time_remain_to_update)));
		static float add = 0.000f;
		add = fill;


		int yey = (int)value;

		std::string wreee = ": ";
		std::string lol = name + wreee + std::to_string(yey);

		Vector2D textsize = RENDER::GetTextSize(FONTS::visuals_esp_font, lol);

		int value_x = (textsize.x / 2);

		RENDER::DrawF(x, y - 38, FONTS::visuals_esp_font, true, true, CColor(255, 255, 255, 255), lol);


		RENDER::DrawFilledCircle(x, y, 32, 60, CColor(30, 30, 30));
		DrawCircleTimer(x, y, 30, add, full_value, color);
		RENDER::DrawFilledCircle(x, y, 25, 60, CColor(40, 40, 40));
	}
}


std::string fix_item_name(std::string name)
{
	if (name[0] == 'C')
		name.erase(name.begin());

	auto startOfWeap = name.find("Weapon");
	if (startOfWeap != std::string::npos)
		name.erase(name.begin() + startOfWeap, name.begin() + startOfWeap + 6);

	return name;
}
void CVisuals::DrawWeaponName(SDK::CBaseEntity* entity, CColor dist,CColor icon, CColor text, int index, Vector pos, Vector top) {
	SDK::player_info_t ent_info; INTERFACES::Engine->GetPlayerInfo(index, &ent_info);

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	Vector vecOrigin = entity->GetVecOrigin(), vecOriginLocal = local_player->GetVecOrigin();
	char dist_to[32];

	sprintf_s(dist_to, "%.0fft", resolve_distance(vecOriginLocal, vecOrigin));

	auto weapon = INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex());
	if (!weapon) return;

	auto c_baseweapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));

	int addtional_distance = 0;

	if (Config::Visuals::Main::Visualsmain::get().ArmorBar)
		addtional_distance = addtional_distance + 12;
	if (Config::Visuals::Main::Visualsmain::get().AmmoBar) {
		if (Config::Visuals::Main::Visualsmain::get().ArmorBar)
			addtional_distance = addtional_distance + 6;
		else
			addtional_distance = addtional_distance + 12;
	}

	if (!Config::Visuals::Main::Visualsmain::get().AmmoBar && !Config::Visuals::Main::Visualsmain::get().ArmorBar)
		addtional_distance = addtional_distance + 6;

	if (Config::Visuals::Main::Visualsmain::get().Distance) 
		RENDER::DrawF(pos.x, pos.y + addtional_distance, FONTS::visuals_esp_font, true, true, dist, dist_to);
	if (Config::Visuals::Main::Visualsmain::get().WeaponText) {
		if (Config::Visuals::Main::Visualsmain::get().Distance) addtional_distance = addtional_distance + 10;
		RENDER::DrawF(pos.x, pos.y + addtional_distance, FONTS::visuals_esp_font, true, true, text, fix_item_name(weapon->GetClientClass()->m_pNetworkName));
	}
	if (Config::Visuals::Main::Visualsmain::get().WeaponIcon) {
		if (Config::Visuals::Main::Visualsmain::get().Distance) addtional_distance = addtional_distance + 10;
		if (Config::Visuals::Main::Visualsmain::get().Distance) addtional_distance = addtional_distance + 3; else if(Config::Visuals::Main::Visualsmain::get().WeaponText) addtional_distance = addtional_distance + 10; else addtional_distance = addtional_distance;
		RENDER::DrawF(pos.x, pos.y + addtional_distance, FONTS::visuals_icon_font, true, true, icon, weapon->GetGunIcon());
	}


}

void CVisuals::DrawHealth(SDK::CBaseEntity* entity, CColor color, CColor dormant, Vector pos, Vector top)
{
	int enemy_hp = entity->GetHealth(),
		hp_red = 255 - (enemy_hp * 2.55),
		hp_green = enemy_hp * 2.55,
		height = (pos.y - top.y),
		width = height / 2;

	float offset = (height / 4.f) + 5;
	UINT hp = height - (UINT)((height * enemy_hp) / 100);

	RENDER::DrawEmptyRect((pos.x - width / 2) - 6, top.y, (pos.x - width / 2) - 3, top.y + height, dormant);
	RENDER::DrawLine((pos.x - width / 2) - 4, top.y + hp, (pos.x - width / 2) - 4, top.y + height, color);
	RENDER::DrawLine((pos.x - width / 2) - 5, top.y + hp, (pos.x - width / 2) - 5, top.y + height, color);

	if (entity->GetHealth() < 100)
		RENDER::DrawF((pos.x - width / 2) - 4, top.y + hp, FONTS::visuals_esp_font, true, true, main_color, std::to_string(enemy_hp));
}

void CVisuals::BombPlanted(SDK::CBaseEntity* entity)
{	
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	int sizex,sizey;
	INTERFACES::Engine->GetScreenSize(sizex,sizey);
	if (entity->GetClientClass()->m_ClassID == (int)CSGOClassID::CPlantedC4) {
		SDK::CCSBomb* Bomb = (SDK::CCSBomb*)(entity);
		float flBlow = Bomb->GetC4BlowTime();
		float ttb = flBlow - (local_player->GetTickBase() * INTERFACES::Globals->interval_per_tick);
		float max = 40.f;
		float width = (((sizex * ttb) / max));


		RENDER::DrawFilledRect(0, 0, width, 10, CColor(255, 255, 100));
		RENDER::DrawF(width, 5, FONTS::visuals_lby_font, false, false, CColor(124, 195, 13, 255), std::to_string(ttb));
	}
}
bool GetBox(SDK::CBaseEntity* pEntity, int& x, int& y, int& w, int& h) {
	// Variables
	Vector vOrigin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = pEntity->GetVecOrigin();
	
	min = pEntity->GetCollideable()->Mins() + vOrigin;
	max = pEntity->GetCollideable()->Maxs() + vOrigin;
	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (INTERFACES::DebugOverlay->ScreenPosition(points[3], flb) || INTERFACES::DebugOverlay->ScreenPosition(points[5], brt)
		|| INTERFACES::DebugOverlay->ScreenPosition(points[0], blb) || INTERFACES::DebugOverlay->ScreenPosition(points[4], frt)
		|| INTERFACES::DebugOverlay->ScreenPosition(points[2], frb) || INTERFACES::DebugOverlay->ScreenPosition(points[1], brb)
		|| INTERFACES::DebugOverlay->ScreenPosition(points[6], blt) || INTERFACES::DebugOverlay->ScreenPosition(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	x = (int)left;
	y = (int)top;
	w = (int)(right - left);
	h = (int)(bottom - top);

	return true;
}
void DrawEntityFont(SDK::CBaseEntity* pEntity, std::string name, int font) {
	int x, y, w, h;
	if (!GetBox(pEntity, x, y, w, h))
		return;

	Vector2D nameSize = RENDER::GetTextSize(font, name.c_str());
	RENDER::DrawF(x + w / 2 - nameSize.x / 2, y + h / 2, font,false,false,CColor(255, 255, 255, 255), name);
}

void CVisuals::DrawDropped(SDK::CBaseEntity* entity, Vector pos, Vector top) {
		if (entity->GetIsDormant())
			return;

		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		if (!local_player)
			return;

		int width, width2;
		SDK::CBaseWeapon* weapon = (SDK::CBaseWeapon*)(entity);
		if (!weapon)return;
		auto cClass = entity->GetClientClass();
		int owner = weapon->GetOwnerWep();
		int Aditional_dist = 0;
		CColor dormant = CColor(100, 100, 100, 255);
		CColor color = CColor(Config::Visuals::Main::Visualsworld::get().droppedweapon_ammo_color_r, Config::Visuals::Main::Visualsworld::get().droppedweapon_ammo_color_g, Config::Visuals::Main::Visualsworld::get().droppedweapon_ammo_color_b, Config::Visuals::Main::Visualsworld::get().droppedweapon_ammo_color_a);
		width = weapon->GetLoadedAmmo();
		width2 = weapon->get_full_info()->max_clip;
		Vector vecOrigin = entity->GetVecOrigin(), vecOriginLocal = local_player->GetVecOrigin();

		if (owner > -1)
			return;
		if (strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == 39 || cClass->m_ClassID == 1 || cClass->m_ClassID == 29 || cClass->m_ClassID == 2 || cClass->m_ClassID == 108 || cClass->m_ClassID == (int)CSGOClassID::CAK47 || cClass->m_ClassID == (int)CSGOClassID::CDEagle)
		{
				char dist_to[32];
				sprintf_s(dist_to, "%.0fft", resolve_distance(vecOriginLocal, vecOrigin));

				int ClassID = cClass->m_ClassID;
				if (strstr(entity->GetClientClass()->m_pNetworkName, XorStr("CWeapon")))
				{
					if (Config::Visuals::Main::Visualsworld::get().DroppedWeapons[0]) {
						RENDER::DrawF(pos.x, pos.y - 6, FONTS::visuals_icon_font, true, true, CColor(Config::Visuals::Main::Visualsworld::get().droppedweapon_color_r, Config::Visuals::Main::Visualsworld::get().droppedweapon_color_g, Config::Visuals::Main::Visualsworld::get().droppedweapon_color_b), entity->GetGunIcon());
						Aditional_dist = Aditional_dist - 12;
					}
					if (Config::Visuals::Main::Visualsworld::get().DroppedWeapons[1]) {
						RENDER::DrawF(pos.x, pos.y - 6 + Aditional_dist, FONTS::visuals_esp_font, true, true, CColor(Config::Visuals::Main::Visualsworld::get().droppedweapon_color_r, Config::Visuals::Main::Visualsworld::get().droppedweapon_color_g, Config::Visuals::Main::Visualsworld::get().droppedweapon_color_b), fix_item_name(entity->GetClientClass()->m_pNetworkName));
						Aditional_dist = Aditional_dist - 12;
					}
					if (Config::Visuals::Main::Visualsworld::get().DroppedWeapons[2])
						RENDER::DrawF(pos.x, pos.y - 6 + Aditional_dist, FONTS::visuals_esp_font, true, true, CColor(Config::Visuals::Main::Visualsworld::get().droppedweapon_color_r, Config::Visuals::Main::Visualsworld::get().droppedweapon_color_g, Config::Visuals::Main::Visualsworld::get().droppedweapon_color_b), dist_to);
					if (Config::Visuals::Main::Visualsworld::get().DroppedWeaponAmmo) {
						int dist = resolve_distance(vecOriginLocal, vecOrigin);
						int height = (pos.y - top.y);
						float offset = (height / 4.f) + 5;
						int iClip = weapon->GetLoadedAmmo();
						int iClipMax = weapon->get_full_info()->max_clip;
						Vector2D angle = RENDER::GetTextSize(FONTS::visuals_esp_font, fix_item_name(entity->GetClientClass()->m_pNetworkName));
						float width;
						width = (((angle.x * iClip) / iClipMax));

						RENDER::DrawFilledRect((pos.x - angle.x / 2), top.y + height + 3, (pos.x - angle.x / 2) + angle.x + 2, top.y + height + 7, CColor(0, 0, 0, 165)); //outline
						RENDER::DrawFilledRect((pos.x - angle.x / 2) + 1, top.y + height + 4, (pos.x - angle.x / 2) + width + 1, top.y + height + 6, color); //ammo
					}
				}
		}
}
void CVisuals::DrawFireGranades(SDK::CBaseEntity * entity, Vector pos, Vector pos3D, Vector top) {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;
	auto inferno = reinterpret_cast<inferno_t*>(entity);

	auto cClass = entity->GetClientClass();

	if (cClass->m_ClassID == (int)CSGOClassID::CInferno)
	{

		const auto spawn_time = inferno->get_spawn_time();
		const auto factor = ((spawn_time + inferno_t::get_expiry_time()) - INTERFACES::Globals->curtime) / inferno_t::get_expiry_time();

		RENDER::DrawF(pos.x, pos.y, FONTS::visuals_esp_font, true, true, RED, "MOLOTOV");
		Vector2D angle = RENDER::GetTextSize(FONTS::visuals_esp_font, "MOLOTOV");
		RENDER::DrawFilledRect((pos.x - (angle.x / 2)), pos.y - 8 - 5 ,pos.x + (angle.x / 2), pos.y - 4 - 5, GREY); //outline
		RENDER::DrawFilledRect((pos.x - (angle.x / 2)) , pos.y - 8 - 5, (pos.x - (angle.x / 2) ) + factor * 30, pos.y - 4 - 5, RED);
		RENDER::DrawEmptyRect((pos.x - (angle.x / 2)), pos.y - 8 - 5, pos.x + (angle.x / 2), pos.y - 4 - 5, GREY);//ammo
	}
}
void CVisuals::DrawSmokeGranades(SDK::CBaseEntity * entity, Vector pos, Vector pos3D, Vector top) {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;
	auto inferno = reinterpret_cast<smoke_t*>(entity);

	auto cClass = entity->GetClientClass();

	if (cClass->m_ClassID == (int)CSGOClassID::CSmokeGrenadeProjectile)
	{

		const auto spawn_time = inferno->get_spawn_time();
		const auto factor = ((spawn_time + smoke_t::get_expiry_time()) - INTERFACES::Globals->curtime) / smoke_t::get_expiry_time();

		if (spawn_time <= 0)
			return;

		RENDER::DrawF(pos.x, pos.y, FONTS::visuals_esp_font, true, true, CYAN, "SMOKE");
		Vector2D angle = RENDER::GetTextSize(FONTS::visuals_esp_font, "SMOKE");
		RENDER::DrawFilledRect((pos.x - (angle.x / 2)), pos.y - 8 - 5, pos.x + (angle.x / 2), pos.y - 4 - 5, GREY); //outline
		RENDER::DrawFilledRect((pos.x - (angle.x / 2)), pos.y - 8 - 5, (pos.x - (angle.x / 2)) + factor * 25, pos.y - 4 - 5, CYAN);
		RENDER::DrawEmptyRect((pos.x - (angle.x / 2)), pos.y - 8 - 5, pos.x + (angle.x / 2), pos.y - 4 - 5, GREY);//ammo
	}
}
float dist(Vector src, Vector dest)
{
	Vector delta = src - dest;
	float fl_dist = ::sqrtf((delta.Length()));
	if (fl_dist < 1.0f) return 1.0f;
	return fl_dist;
}
void CVisuals::DrawDroppedGranades(SDK::CBaseEntity* entity){
	if (entity->GetIsDormant())
		return;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	Vector pos, pos3D;

	pos3D = entity->GetAbsOrigin();


	int owner = entity->GetOwnerWep();

	auto cClass = entity->GetClientClass();

	if (owner > -1)
		return;

	if (!RENDER::WorldToScreen(pos3D, pos))
		return;

	if (Config::Visuals::Main::Visualsworld::get().DroppedGrenades) {
		if (cClass->m_ClassID == (int)CSGOClassID::CFlashbang)
			RENDER::DrawF(pos.x, pos.y, FONTS::visuals_esp_font, true, true, CColor(Config::Visuals::Main::Visualsworld::get().droppedgranades_color_r, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_g, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_b, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_a), "Flashbang");
		if (cClass->m_ClassID == (int)CSGOClassID::CIncendiaryGrenade)
			RENDER::DrawF(pos.x, pos.y, FONTS::visuals_esp_font, true, true, CColor(Config::Visuals::Main::Visualsworld::get().droppedgranades_color_r, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_g, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_b, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_a), "Incendiary Grenade");
		if (cClass->m_ClassID == (int)CSGOClassID::CMolotovGrenade)
			RENDER::DrawF(pos.x, pos.y, FONTS::visuals_esp_font, true, true, CColor(Config::Visuals::Main::Visualsworld::get().droppedgranades_color_r, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_g, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_b, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_a), "Molotov");
		if (cClass->m_ClassID == (int)CSGOClassID::CDecoyGrenade)
			RENDER::DrawF(pos.x, pos.y, FONTS::visuals_esp_font, true, true, CColor(Config::Visuals::Main::Visualsworld::get().droppedgranades_color_r, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_g, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_b, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_a), "Decoy");
		if (cClass->m_ClassID == (int)CSGOClassID::CHEGrenade)
			RENDER::DrawF(pos.x, pos.y, FONTS::visuals_esp_font, true, true, CColor(Config::Visuals::Main::Visualsworld::get().droppedgranades_color_r, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_g, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_b, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_a), "HE Grenade");
		if (cClass->m_ClassID == (int)CSGOClassID::CSmokeGrenade)
			RENDER::DrawF(pos.x, pos.y, FONTS::visuals_esp_font, true, true, CColor(Config::Visuals::Main::Visualsworld::get().droppedgranades_color_r, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_g, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_b, Config::Visuals::Main::Visualsworld::get().droppedgranades_color_a), "Smoke");
	}
	if (Config::Visuals::Main::Visualsworld::get().Bomb) {
		if (cClass->m_ClassID == (int)CSGOClassID::CC4)
			RENDER::DrawF(pos.x, pos.y, FONTS::visuals_esp_font, true, true, CColor(Config::Visuals::Main::Visualsworld::get().bomb_color_r, Config::Visuals::Main::Visualsworld::get().bomb_color_g, Config::Visuals::Main::Visualsworld::get().bomb_color_b, Config::Visuals::Main::Visualsworld::get().bomb_color_a), "BOMB");
	}

}
void CVisuals::DrawGranades(SDK::CBaseEntity* entity) {
	if (entity->GetIsDormant())
		return;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	Vector pos, pos3D;
	pos3D = entity->GetAbsOrigin();
	auto cClass = entity->GetClientClass();

	if (!RENDER::WorldToScreen(pos3D, pos))
		return;
	Vector vecOrigin = entity->GetVecOrigin(), vecOriginLocal = local_player->GetVecOrigin();
	char dist_to[32];
	std::string model_name = INTERFACES::ModelInfo->GetModelName(entity->GetModel());
	if (!model_name.empty()) {

		if (cClass->m_ClassID == (int)CSGOClassID::CBaseCSGrenadeProjectile) {

			if (model_name.find("fraggrenade") != std::string::npos) {
				if(entity->ExplodeEffectTickBegin() < 1){
					/*
					if(dist(vecOriginLocal, vecOrigin) < 15)
					RENDER::DrawFilledCircle(pos.x, pos.y, 40 - (dist(vecOriginLocal, vecOrigin) / 3), 60, CColor(255, 20, 20, 180));
					else 
						RENDER::DrawFilledCircle(pos.x, pos.y, 40 - (dist(vecOriginLocal, vecOrigin) / 3), 60, CColor(46, 45, 45, 180));
					sprintf_s(dist_to, "%.0f ft", dist(vecOriginLocal, vecOrigin));
					RENDER::DrawF(pos.x, pos.y, FONTS::visuals_indector, true, true, CColor(238, 255, 0), "!");
					RENDER::DrawF(pos.x, pos.y + 20, FONTS::visuals_esp_font, true, true, CColor(255, 255, 255), dist_to);
					*/
					RENDER::DrawF(pos.x, pos.y, FONTS::visuals_esp_font, true, true, CColor(255, 255, 255), "he grenade");
				}
			}

		}
	}
}
void CVisuals::DrawDroppedIcons(SDK::CBaseEntity* entity)
{
	Vector min, max;
	entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 10);

	SDK::CBaseWeapon* weapon_cast = (SDK::CBaseWeapon*)entity;

	if (!weapon_cast)
		return;

	auto weapon = INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex());
	auto c_baseweapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));

	if (!c_baseweapon)
		return;

	if (!weapon)
		return;

	SDK::CBaseEntity* plr = INTERFACES::ClientEntityList->GetClientEntityFromHandle((HANDLE)weapon_cast->GetOwnerHandle());
	if (!plr && RENDER::WorldToScreen(pos3D, pos) && RENDER::WorldToScreen(top3D, top))
	{
		std::string ItemName = fix_item_name(weapon->GetClientClass()->m_pNetworkName);
		int height = (pos.y - top.y);
		int width = height / 2;
		RENDER::DrawF(plr->GetAbsOrigin().x, plr->GetAbsOrigin().y, FONTS::visuals_esp_font, true, true, WHITE, ItemName.c_str()); //numpad_menu_font
	}
}

void CVisuals::DrawAmmo(SDK::CBaseEntity* entity, CColor color, CColor dormant, Vector pos, Vector top)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player && local_player->GetHealth() < 1) return;


	auto c_baseweapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));
	if (!c_baseweapon) return;
	int height = (pos.y - top.y);
	UINT hp = height - (UINT)((height * 3) / 100);
	float offset = (height / 4.f) + 5;
	int iClip = c_baseweapon->GetLoadedAmmo();
	int iClipMax = c_baseweapon->GetWeaponInfo()->max_clip;
	auto animLayer = entity->GetAnimOverlay(1);
	auto activity = 967;
	if (!animLayer.m_pOwner)
		return;
	float box_w = (float)fabs(height / 2);
	float width;
	if (activity == 967 && animLayer.m_flWeight != 0.f)
	{
		float cycle = animLayer.m_flCycle;
		width = (((box_w * cycle) / 1.f));
	}
	else
		width = (((box_w * iClip) / iClipMax));

	RENDER::DrawFilledRect((pos.x - box_w / 2), top.y + height + 3, (pos.x - box_w / 2) + box_w + 2, top.y + height + 7, dormant); //outline
	RENDER::DrawFilledRect((pos.x - box_w / 2) + 1, top.y + height + 4, (pos.x - box_w / 2) + width + 1, top.y + height + 6, color); //ammo
}
void CVisuals::DrawArmor(SDK::CBaseEntity* entity, CColor color, CColor dormant, Vector pos, Vector top)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	int height = (pos.y - top.y);
	float offset = (height / 4.f) + 5;
	int iClip = entity->GetArmor();
	int iClipMax = 100;
	float box_w = (float)fabs(height / 2);
	float width;
	width = (((box_w * iClip) / iClipMax));
	int w = 0;
	bool is_teammate = local_player->GetTeam() == entity->GetTeam();
	bool is_local_player = entity == local_player;

	if (Config::Visuals::Main::Visualsmain::get().AmmoBar)
		w = 5;

	RENDER::DrawFilledRect((pos.x - box_w / 2), top.y + height + 3 + w, (pos.x - box_w / 2) + box_w + 2, top.y + height + 7 + w, dormant); //outline
	RENDER::DrawFilledRect((pos.x - box_w / 2) + 1, top.y + height + 4 + w, (pos.x - box_w / 2) + width + 1, top.y + height + 6 + w, color); //ammo
}
void CVisuals::DrawInfo(SDK::CBaseEntity* entity, CColor color, CColor alt, Vector pos, Vector top)
{
	std::vector<std::pair<std::string, CColor>> stored_info;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	bool is_local_player = entity == local_player;
	bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;
	if (is_local_player) return;

	if (Config::Visuals::Main::Visualsmain::get().Flags && !is_teammate)
		stored_info.push_back(std::pair<std::string, CColor>("$" + std::to_string(entity->GetMoney()), bt));
	else if (Config::Visuals::Main::Visualsmain::get().Flags && Config::Visuals::Main::Visualsmain::get().Team && is_teammate)
		stored_info.push_back(std::pair<std::string, CColor>("$" + std::to_string(entity->GetMoney()), bt));

	if (Config::Visuals::Main::Visualsmain::get().Flags && !is_teammate)
	{
		if (entity->GetArmor() > 0)
			stored_info.push_back(std::pair<std::string, CColor>(entity->GetArmorName(), color));

		static int crouched_ticks[65] = { 0 };
		static int stored_tick = 0;
		if (*entity->flDuckAmount() <= 0.9f && *entity->flDuckAmount() > 0.1f && entity->GetFlags() & 1 && *entity->flDuckSpeed() == 8) {
			if (stored_tick != INTERFACES::Globals->tickcount) {
				crouched_ticks[entity->GetIndex()]++;
				stored_tick = INTERFACES::Globals->tickcount;
			}

			if (crouched_ticks[entity->GetIndex()] >= 5)
				stored_info.push_back(std::pair<std::string, CColor>("FD", color));
		}
		else
			crouched_ticks[entity->GetIndex()] = 0;


		if (entity->GetIsScoped())
			stored_info.push_back(std::pair<std::string, CColor>("zoom", alt));

		//stored_info.push_back(std::pair<std::string, CColor>("delta time: " + std::to_string(resolver.GetDeltaTime(resolver.GetPlayerResolveInfo(entity).on_shot_eye_curtime)), color));
		stored_info.push_back(std::pair<std::string, CColor>("RM: " + std::to_string(resolver.GetPlayerResolveInfo(entity).resolve_type), color));
		stored_info.push_back(std::pair<std::string, CColor>("OS: " + std::to_string(resolver.GetPlayerResolveInfo(entity).can_onshot), color));
		/*for (int i = 0; i <= 15; i++) {
			stored_info.push_back(std::pair<std::string, CColor>("Sequence: " + std::to_string(resolver.GetPlayerResolveInfo(entity).anim_layers[i].m_nSequence), color));
			stored_info.push_back(std::pair<std::string, CColor>("Cycle: " + std::to_string(resolver.GetPlayerResolveInfo(entity).anim_layers[i].m_flCycle), color));
		}*/

	}
	else if (Config::Visuals::Main::Visualsmain::get().Flags && Config::Visuals::Main::Visualsmain::get().Team && is_teammate)
	{
		if (entity->GetArmor() > 0)
			stored_info.push_back(std::pair<std::string, CColor>(entity->GetArmorName(), color));

		static int crouched_ticks[65] = { 0 };
		static int stored_tick = 0;
		if (*entity->flDuckAmount() <= 0.9f && *entity->flDuckAmount() > 0.1f && entity->GetFlags() & 1 && *entity->flDuckSpeed() == 8) {
			if (stored_tick != INTERFACES::Globals->tickcount) {
				crouched_ticks[entity->GetIndex()]++;
				stored_tick = INTERFACES::Globals->tickcount;
			}

			if (crouched_ticks[entity->GetIndex()] >= 5)
				stored_info.push_back(std::pair<std::string, CColor>("FD", color));
		}
		else
			crouched_ticks[entity->GetIndex()] = 0;


		if (entity->GetIsScoped())
			stored_info.push_back(std::pair<std::string, CColor>("zoom", alt));
	}

	int height = (pos.y - top.y), width = height / 2, i = 0;
	for (auto Text : stored_info)
	{
		RENDER::DrawF((pos.x + width / 2) + 5, top.y + i, FONTS::visuals_esp_font, false, false, Text.second, Text.first);
		i += 8;
	}
}
inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}
void CVisuals::DrawInaccuracy(){
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon) return;

	int W, H, cW, cH;
	INTERFACES::Engine->GetScreenSize(W, H);
	cW = W / 2; cH = H / 2;
	if (local_player->IsAlive())
	{
		auto accuracy = (weapon->GetInaccuracy() + weapon->GetSpreadCone()) * 500.f;
		if (!weapon->is_grenade() && !weapon->is_knife())
			RENDER::DrawFilledCircle(cW, cH, accuracy + 3, 30, CColor(0, 0, 0, 85));
	}
	
}
void CVisuals::DrawBulletTracers() /// [censored] bullet tracers
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	if (!INTERFACES::Engine->IsInGame() || !local_player) { Impacts.clear(); return; }
	if (Impacts.size() > 30) Impacts.pop_back();

	for (int i = 0; i < Impacts.size(); i++)
	{
		auto current = visuals->Impacts.at(i);


		bool is_local_player = current.pPlayer == local_player;
		bool is_teammate = local_player->GetTeam() == current.pPlayer->GetTeam() && !is_local_player;

		if (is_local_player)
		{                                                                                                           // i  divide so the bullet beam isnt exactly where our eyes are
			INTERFACES::DebugOverlay->AddLineOverlay(current.pPlayer->GetVecOrigin() + current.pPlayer->GetViewOffset() / 1.1, current.vecImpactPos, SETTINGS::settings.bulletlocal_col[0] * 255, SETTINGS::settings.bulletlocal_col[1] * 255, SETTINGS::settings.bulletlocal_col[2] * 255, SETTINGS::settings.bulletlocal_col[3] * 255, true, SETTINGS::settings.bulletlife);
		}
		if (!is_local_player && !is_teammate)
		{
			INTERFACES::DebugOverlay->AddLineOverlay(current.pPlayer->GetVecOrigin() + current.pPlayer->GetViewOffset(), current.vecImpactPos, SETTINGS::settings.bulletenemy_col[0] * 255, SETTINGS::settings.bulletenemy_col[1] * 255, SETTINGS::settings.bulletenemy_col[2] * 255, SETTINGS::settings.bulletenemy_col[3] * 255, true, SETTINGS::settings.bulletlife);

		}


		Impacts.erase(Impacts.begin() + i);
	}



}
void CVisuals::DrawBulletBeams()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	if (!INTERFACES::Engine->IsInGame() || !local_player) { Impacts.clear(); return; }
	if (Impacts.size() > 30) Impacts.pop_back();

	for (int i = 0; i < Impacts.size(); i++)
	{
		auto current = Impacts.at(i);
		if (!current.pPlayer) continue;
		if (current.pPlayer->GetIsDormant()) continue;

		bool is_local_player = current.pPlayer == local_player;
		bool is_teammate = local_player->GetTeam() == current.pPlayer->GetTeam() && !is_local_player;


		SDK::BeamInfo_t beamInfo;
		beamInfo.m_pszModelName = "sprites/physbeam.vmt"; 
		beamInfo.m_nModelIndex = -1;
		beamInfo.m_flHaloScale = 0.0f;
		beamInfo.m_flLife = 2;
		beamInfo.m_flWidth = 2;
		beamInfo.m_flEndWidth = 2.0f;
		beamInfo.m_flFadeLength = 0.0f;
		beamInfo.m_flAmplitude = 2.0f;
		beamInfo.m_flBrightness = 255.f;
		beamInfo.m_flSpeed = 0.2f;
		beamInfo.m_nStartFrame = 0;
		beamInfo.m_flFrameRate = 0.f;
		beamInfo.m_flRed = Config::Visuals::Main::VisualsEffects::get().bullettracers_color_r;
		beamInfo.m_flGreen = Config::Visuals::Main::VisualsEffects::get().bullettracers_color_g;
		beamInfo.m_flBlue = Config::Visuals::Main::VisualsEffects::get().bullettracers_color_b;
		beamInfo.m_nSegments = 2;
		beamInfo.m_bRenderable = true;
		beamInfo.m_nFlags = SDK::FBEAM_ONLYNOISEONCE | SDK::FBEAM_NOTILE | SDK::FBEAM_HALOBEAM;

		beamInfo.m_vecStart = current.pPlayer->GetVecOrigin() + current.pPlayer->GetViewOffset();
		beamInfo.m_vecEnd = current.vecImpactPos;

		auto beam = INTERFACES::ViewRenderBeams->CreateBeamPoints(beamInfo);
		if (beam) INTERFACES::ViewRenderBeams->DrawBeam(beam);

		Impacts.erase(Impacts.begin() + i);
	}
}

void CVisuals::DrawCrosshair()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	auto crosshair = INTERFACES::cvar->FindVar("crosshair");
	if (SETTINGS::settings.xhair_type == 0)
	{
		crosshair->SetValue("1");
		return;
	}
	else
		crosshair->SetValue("0");

	int W, H, cW, cH;
	INTERFACES::Engine->GetScreenSize(W, H);

	cW = W / 2; cH = H / 2;

	int dX = W / 120.f, dY = H / 120.f;
	int drX, drY;

	if (SETTINGS::settings.xhair_type == 2)
	{
		drX = cW - (int)(dX * (((local_player->GetPunchAngles().y * 2.f) * 0.45f) + local_player->GetPunchAngles().y));
		drY = cH + (int)(dY * (((local_player->GetPunchAngles().x * 2.f) * 0.45f) + local_player->GetPunchAngles().x));
	}
	else
	{
		drX = cW;
		drY = cH;
	}

	INTERFACES::Surface->DrawSetColor(BLACK);
	INTERFACES::Surface->DrawFilledRect(drX - 4, drY - 2, drX - 4 + 8, drY - 2 + 4);
	INTERFACES::Surface->DrawFilledRect(drX - 2, drY - 4, drX - 2 + 4, drY - 4 + 8);

	INTERFACES::Surface->DrawSetColor(WHITE);
	INTERFACES::Surface->DrawFilledRect(drX - 3, drY - 1, drX - 3 + 6, drY - 1 + 2);
	INTERFACES::Surface->DrawFilledRect(drX - 1, drY - 3, drX - 1 + 2, drY - 3 + 6);
}

void CVisuals::DrawFovArrows(SDK::CBaseEntity* entity, CColor color)
{

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	if (entity->GetIsDormant()) return;

	Vector screenPos, client_viewangles;
	int screen_width = 0, screen_height = 0;
	auto radius = Config::Visuals::Main::Visualsmain::get().arrows_rad;
	float size = Config::Visuals::Main::Visualsmain::get().arrows_size / 10;

	//if (UTILS::IsOnScreen(aimbot->get_hitbox_pos(entity, SDK::HitboxList::HITBOX_HEAD), screenPos)) return;

	INTERFACES::Engine->GetViewAngles(client_viewangles);
	INTERFACES::Engine->GetScreenSize(screen_width, screen_height);

	const auto screen_center = Vector(screen_width / 2.f, screen_height / 2.f, 0);
	const auto rot = DEG2RAD(client_viewangles.y - UTILS::CalcAngle(local_player->GetEyePosition(), aimbot->get_hitbox_pos(entity, SDK::HitboxList::HITBOX_HEAD)).y - 90);

	std::vector<SDK::Vertex_t> vertices;
	bool is_teammate = local_player->GetTeam() == entity->GetTeam() && entity != local_player;


	vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
	vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot + DEG2RAD(2 * size)) * (radius - 16 * size), screen_center.y + sinf(rot + DEG2RAD(2 * size)) * (radius - 16 * size))));
	vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot - DEG2RAD(2 * size)) * (radius - 16 * size), screen_center.y + sinf(rot - DEG2RAD(2 * size)) * (radius - 16 * size))));

	RENDER::TexturedPolygon(3, vertices, color);
}
void CVisuals::DrawInaccuracy1()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon) return;

	int spread_Col[3] = { SETTINGS::settings.spreadcirclecol[0], SETTINGS::settings.spreadcirclecol[1], SETTINGS::settings.spreadcirclecol[2] };


	int W, H, cW, cH;
	INTERFACES::Engine->GetScreenSize(W, H);
	cW = W / 2; cH = H / 2;

	if (local_player->IsAlive())
	{
		auto accuracy = (weapon->GetInaccuracy() + weapon->GetSpreadCone()) * 500.f;

		float r;
		float alpha, newAlpha;

		for (r = accuracy; r > 0; r--)
		{
			if (!weapon->is_grenade() && !weapon->is_knife())


				alpha = r / accuracy;
			newAlpha = pow(alpha, 5);

			RENDER::DrawCircle(cW, cH, r, 60, CColor(spread_Col[0], spread_Col[1], spread_Col[2], newAlpha * 130));
		}
	}
}
void CVisuals::DrawIndicator()
{
	auto crosshair = INTERFACES::cvar->FindVar("crosshair");
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	if (local_player->GetHealth() <= 0) return;
	if (Config::Visuals::Main::Visualsworld::get().PenetrationCrosshair)
	{
		crosshair->SetValue("0");

		int screen_width, screen_height;
		INTERFACES::Engine->GetScreenSize(screen_width, screen_height);
		int center_width = screen_width / 2;
		int center_height = screen_height / 2;
		Vector EyeAng;
		INTERFACES::Engine->GetViewAngles(EyeAng);

		Vector dst, forward;
		MATH::AngleVectors(EyeAng, &forward);
		dst = forward * 8196.f;



		float dmg;
		if (AutoWall->CanWallbang(dmg))
			RENDER::DrawCircle(center_width, center_height, 1, 100, CColor(0, 255, 0, 225));
		else
			RENDER::DrawCircle(center_width, center_height, 1, 100, CColor(255, 0, 0, 225));
	}
	else
	{
		crosshair->SetValue("1");
	}

	float breaking_lby_fraction = fabs(MATH::NormalizeYaw(global::real_angles.y - local_player->GetLowerBodyYaw())) / 180.f;
	float lby_delta = abs(MATH::NormalizeYaw(global::real_angles.y - local_player->GetLowerBodyYaw()));

	CColor fake;
	int screen_width, screen_height;
	INTERFACES::Engine->GetScreenSize(screen_width, screen_height);
	Vector loclapos;
	INTERFACES::Engine->GetViewAngles(loclapos);
	if (Config::Visuals::Main::Visualsmain::get().screen[1]) {
		int iY = 350;

		if (Config::AntiAim::AADesync::get().desync > 0 && Config::AntiAim::AADesync::get().angle > 0) {
			if (antiaim->ShallReturn(&global::originalCMD))
				fake = CColor(255, 0, 0);
			else if (!local_player->GetFlags() & FL_ONGROUND)
				fake = CColor(246, 255, 0);
			else if (local_player->GetVelocity().Length2D() > 100)
				fake = CColor(246, 255, 0);
			else
				fake = CColor(26, 255, 0);

			iY += 22;
			RENDER::DrawF(10, screen_height - iY, FONTS::visuals_lby_font, false, false, fake, "FAKE");

		}
		if (global::menualrlb < 3) {
			iY += 22;
			if (global::menualrlb == 0)
				RENDER::DrawF(10, screen_height - iY, FONTS::visuals_lby_font, false, false, fake, "BACK");
			else if (global::menualrlb == 1)
				RENDER::DrawF(10, screen_height - iY, FONTS::visuals_lby_font, false, false, fake, "RIGHT");
			else if (global::menualrlb == 2)
				RENDER::DrawF(10, screen_height - iY, FONTS::visuals_lby_font, false, false, fake, "LEFT");
		}
		if (getping() > 80) {
			iY += 22;
			RENDER::DrawF(10, screen_height - iY, FONTS::visuals_lby_font, false, false, CColor(255, 0, 0), "PING");
		}
		if (GetAsyncKeyState(UTILS::INPUT::input_handler.keyBindings(Config::Misc::MiscMain::get().fakeduck))) {
			iY += 22;
			RENDER::DrawF(10, screen_height - iY, FONTS::visuals_lby_font, false, false, CColor(0, 255, 0), "FD");

		}
		iY += 22;
		if (local_player->GetVelocity().Length2D() > 450)
			RENDER::DrawF(10, screen_height - iY, FONTS::visuals_lby_font, false, false, CColor(255, 255, 0), "LC");
		else if (local_player->GetVelocity().Length2D() > 350)
			RENDER::DrawF(10, screen_height - iY, FONTS::visuals_lby_font, false, false, CColor(246, 255, 0), "LC");
		else
			RENDER::DrawF(10, screen_height - iY, FONTS::visuals_lby_font, false, false, CColor(255, 0, 0), "LC");

	}
	if (Config::Visuals::Main::Visualsmain::get().screen[4]) {
		{

			auto client_viewangles = Vector();
			INTERFACES::Engine->GetViewAngles(client_viewangles);
			const auto screen_center = Vector2D(screen_width / 2.f, screen_height / 2.f);

			/*
			if (Config::AntiAim::AA::get().yaw == 2) //freestanding
			{
				float radius = 65.61;
				auto draw_arrow = [&](float rot, CColor color) -> void
				{
					std::vector<SDK::Vertex_t> vertices;
					vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
					vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot + DEG2RAD(13)) * (radius - 12), screen_center.y + sinf(rot + DEG2RAD(13)) * (radius - 12)))); //25
					vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot - DEG2RAD(13)) * (radius - 12), screen_center.y + sinf(rot - DEG2RAD(13)) * (radius - 12)))); //25
					RENDER::TexturedPolygon(3, vertices, color);
				};

				static auto alpha = 0.f; static auto plus_or_minus = false;
				if (alpha <= 0.f || alpha >= 255.f) plus_or_minus = !plus_or_minus;
				alpha += plus_or_minus ? (255.f / 7 * 0.015) : -(255.f / 7 * 0.015); alpha = clamp(alpha, 0.f, 255.f);

				auto real_color = CColor(0, 255, 0, alpha);
				const auto real_rot = DEG2RAD(client_viewangles.y - 90);
				float rl = 0;
				for (int i = 0; rl <= 1100; i++)
				{
					draw_arrow(rl, CColor(1, 1, 1, 90));
					rl = rl + 151.229996;
				}
				if (global::freestandingsite == 1) {
					draw_arrow(-73.649999, CColor(117, 172, 255, 150));
				}
				if (global::freestandingsite == 4) {
					draw_arrow(-62.82, CColor(117, 172, 255, 150));
				}
				if (global::freestandingsite == 5) {
					draw_arrow(-55.67, CColor(117, 172, 255, 150));
				}
				if (global::freestandingsite == 2) {
					draw_arrow(-78.63998, CColor(117, 172, 255, 150));
				}
				if (global::freestandingsite == 3) {
					draw_arrow(-60.659909, CColor(117, 172, 255, 150));
				}
				if (global::freestandingsite == 6) {
					draw_arrow(-80.370002, CColor(117, 172, 255, 150));
				}
			}
			if (Config::AntiAim::AA::get().yaw == 1) { //manual
				float radius = 69;
				auto draw_arrow = [&](float rot, CColor color) -> void
				{
					std::vector<SDK::Vertex_t> vertices;
					vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
					vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot + DEG2RAD(11)) * (radius - 18), screen_center.y + sinf(rot + DEG2RAD(11)) * (radius - 18)))); //25
					vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot - DEG2RAD(11)) * (radius - 18), screen_center.y + sinf(rot - DEG2RAD(11)) * (radius - 18)))); //25
					RENDER::TexturedPolygon(3, vertices, color);
				};

				static auto alpha = 0.f; static auto plus_or_minus = false;
				if (alpha <= 0.f || alpha >= 255.f) plus_or_minus = !plus_or_minus;
				alpha += plus_or_minus ? (255.f / 7 * 0.015) : -(255.f / 7 * 0.015); alpha = clamp(alpha, 0.f, 255.f);

				auto real_color = CColor(0, 255, 0, alpha);
				const auto real_rot = DEG2RAD(client_viewangles.y - 90);
				float rl = 0;
				if (global::menualrlb < 3) {
					for (int i = 0; rl <= 470.779999; i++)
					{
						draw_arrow(rl, CColor(1, 1, 1, 90));
						rl = rl + 317.260010;
					}
					draw_arrow(403.68, CColor(1, 1, 1, 90));
					if (global::menual == 1) {
						draw_arrow(65.949997, CColor(117, 172, 255, 150));

					}
					if (global::menual == 0) {
						draw_arrow(403.68, CColor(117, 172, 255, 150));

					}
					if (global::menual == 2) {
						draw_arrow(0, CColor(117, 172, 255, 150));

					}
				}
			}
			*/
			if (Config::AntiAim::AADesync::get().desync > 0) {
				float radius = 110;
				auto draw_arrow = [&](float rot, CColor color) -> void
				{
					std::vector<SDK::Vertex_t> vertices;
					vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
					vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot + DEG2RAD(5)) * (radius - 18), screen_center.y + sinf(rot + DEG2RAD(5)) * (radius - 18)))); //25
					vertices.push_back(SDK::Vertex_t(Vector2D(screen_center.x + cosf(rot - DEG2RAD(5)) * (radius - 18), screen_center.y + sinf(rot - DEG2RAD(5)) * (radius - 18)))); //25
					RENDER::TexturedPolygon(3, vertices, color);
				};

				static auto alpha = 0.f; static auto plus_or_minus = false;
				if (alpha <= 0.f || alpha >= 255.f) plus_or_minus = !plus_or_minus;
				alpha += plus_or_minus ? (255.f / 7 * 0.015) : -(255.f / 7 * 0.015); alpha = clamp(alpha, 0.f, 255.f);

				auto real_color = CColor(0, 255, 0, alpha);
				const auto real_rot = DEG2RAD(client_viewangles.y - 90);
				float rl = 0;
				draw_arrow(65.949997, CColor(255, 255, 255, 90));
				draw_arrow(0, CColor(255, 255, 255, 90));

				if (!global::desync) {
					draw_arrow(65.949997, CColor(30, 255, 0, 150));
				}
				else {
					draw_arrow(0, CColor(30, 255, 0, 150));
				}
			}
		}
	}
}

bool PrecacheModel(const char* szModelName)
{
	return false;
}

void CVisuals::LogEvents()
{
	static bool convar_performed = false, convar_lastsetting;

	if (convar_lastsetting != SETTINGS::settings.info_bool)
	{
		convar_lastsetting = SETTINGS::settings.info_bool;
		convar_performed = false;
	}

	if (!convar_performed)
	{
		static auto developer = INTERFACES::cvar->FindVar("developer");
		developer->SetValue(1);
		static auto con_filter_text_out = INTERFACES::cvar->FindVar("con_filter_text_out");
		static auto con_filter_enable = INTERFACES::cvar->FindVar("con_filter_enable");
		static auto con_filter_text = INTERFACES::cvar->FindVar("con_filter_text");

		con_filter_text->SetValue(".     ");
		con_filter_text_out->SetValue("");
		con_filter_enable->SetValue(2);
		convar_performed = true;
	}
}

void CVisuals::ModulateWorld()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	static auto sv_skyname = INTERFACES::cvar->FindVar("sv_skyname");
	sv_skyname->nFlags &= ~FCVAR_CHEAT;

	static auto static_val = 100;
	static auto static_val_asus = 255;
	auto night_val = SETTINGS::settings.daytimevalue;
	auto asus_val = SETTINGS::settings.asus_props_value;

	if (static_val_asus != asus_val)
		{
			static_val = asus_val;
			auto staticdrop = INTERFACES::cvar->FindVar("r_DrawSpecificStaticProp");
			staticdrop->SetValue(0);

			for (auto i = INTERFACES::MaterialSystem->FirstMaterial(); i != INTERFACES::MaterialSystem->InvalidMaterial(); i = INTERFACES::MaterialSystem->NextMaterial(i))
			{
				auto pMaterial = INTERFACES::MaterialSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				if (!strcmp(pMaterial->GetTextureGroupName(), "StaticProp textures"))
				{
					pMaterial->AlphaModulate(SETTINGS::settings.asus_props_value);
				}
			}
	}

	if (static_val != night_val)
	{
		static_val = night_val;
		for (auto i = INTERFACES::MaterialSystem->FirstMaterial(); i != INTERFACES::MaterialSystem->InvalidMaterial(); i = INTERFACES::MaterialSystem->NextMaterial(i))
		{
			auto pMaterial = INTERFACES::MaterialSystem->GetMaterial(i);
			if (!pMaterial) continue;

			auto finalnightval = night_val / 100.f;
			if (strstr(pMaterial->GetTextureGroupName(), "World"))
				pMaterial->ColorModulate(finalnightval, finalnightval, finalnightval);
			else if (strstr(pMaterial->GetTextureGroupName(), "SkyBox"))
				sv_skyname->SetValue("sky_csgo_night02");
		}
	}
}
void CVisuals::viewmodelxyz()
{
	if (SETTINGS::settings.Viewmodelchanger)
	{
		INTERFACES::cvar->get_convar("viewmodel_offset_x")->set_value(SETTINGS::settings.viewmodel_x);
		INTERFACES::cvar->get_convar("viewmodel_offset_y")->set_value(SETTINGS::settings.viewmodel_y);
		INTERFACES::cvar->get_convar("viewmodel_offset_z")->set_value(SETTINGS::settings.viewmodel_z - 10);
	}
}
void CVisuals::KeyStorks() {
	bool a = false, w = false, s = false, d = false, space = false, m1 = false, m2 = false;
	int screen_width, screen_height;
	if (GetAsyncKeyState(VK_SPACE))
		space = true;
	if (GetAsyncKeyState(0x41)) 
		a = true;
	if (GetAsyncKeyState(0x53)) 
		s = true;
	if (GetAsyncKeyState(0x44)) 
		d = true;
	if (GetAsyncKeyState(0x57)) 
		w = true;
	if (GetAsyncKeyState(VK_LBUTTON))
		m1 = true;
	if (GetAsyncKeyState(VK_RBUTTON))
		m2 = true;

	auto draw_stroke = [&](bool active, std::string key, Vector2D pos,Vector2D pos2, Vector2D screen) -> void
	{
		RENDER::FillRectangle(screen.x + pos.x , screen.y + pos.y, pos2.x, pos2.y, CColor(80, 80, 80, active ? 175 : 80));
		RENDER::DrawF(screen.x + pos.x + pos2.x / 2, screen.y + pos.y + pos2.y / 2, FONTS::visuals_indector, true, true, CColor(255, 255, 255), key);
	};

	INTERFACES::Engine->GetScreenSize(screen_width, screen_height);
	const auto screen_center = Vector2D(screen_width / 2.f, screen_height / 2.f);
	draw_stroke(m1, "M1", Vector2D(700, -170), Vector2D(94.5, 60), screen_center);
	draw_stroke(m2, "M2", Vector2D(797, -170), Vector2D(97, 60), screen_center);
	draw_stroke(space, "SPACE", Vector2D(700,-235), Vector2D(194,60), screen_center);
	draw_stroke(a, "A", Vector2D(700, -300), Vector2D(60, 60), screen_center);
	draw_stroke(s, "S", Vector2D(766, -300), Vector2D(60, 60), screen_center);
	draw_stroke(d, "D", Vector2D(832, -300), Vector2D(60, 60), screen_center);
	draw_stroke(w, "W", Vector2D(766, -365), Vector2D(60, 60), screen_center);

}
int getvalocity() {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto velocity = local_player->GetVelocity();
	return (int)velocity.Length2D();

}
std::string getchock() {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	//if (!local_player) return;
	auto net_channel = INTERFACES::Engine->GetNetChannelInfo();
	std::string outgoing = local_player ? std::to_string((int)(net_channel->GetLatency(FLOW_OUTGOING) * 1000)) : "0";
	return outgoing;

}
const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X", &tstruct);

	return buf;
}
std::string get_time()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X", &tstruct);
	return buf;

}
void CVisuals::watermark() {
	int w_s, h_s;
	int x, y, w, h;
	int w0, h0;

	INTERFACES::Engine->GetScreenSize(w_s, h_s);
	w = w_s / 2; h = h_s / 2;

	x = 1700, y = 10, w = 197, h = 20;

	auto watermark = [](int x, int y, int w, int h,CColor c) {
		RENDER::DrawLine(x, y, x, y + h, c);
		RENDER::DrawLine(x, y + h, x + w + 1, y + h, c);
		RENDER::DrawLine(x + w, y, x + w, y + h, c);
		RENDER::DrawLine(x, y, x + w, y, c);
	};

	static int u = 0;
	float watermarkScrollSpeed = 1.5f;

	RENDER::DrawFilledRect(x, y,x + w, y + h, CColor(11, 11, 11));
	watermark(x - 0, y - 0, w + 0 * 2, h + 0 * 2, BLACK);
	watermark(x - 1, y - 1, w + 1 * 2, h + 1 * 2, CColor(53, 53, 53));
	watermark(x - 2, y - 2, w + 2 * 2, h + 2 * 2, CColor(28, 28, 28));
	watermark(x - 3, y - 3, w + 3 * 2, h + 3 * 2, CColor(28, 28, 28));
	watermark(x - 4, y - 4, w + 4 * 2, h + 4 * 2, CColor(28, 28, 28));
	watermark(x - 5, y - 5, w + 5 * 2, h + 5 * 2, CColor(53, 53, 53));
	watermark(x - 6, y - 6, w + 6 * 2, h + 6 * 2, BLACK);

	std::string thingy = "| ";

	RENDER::DrawF(1708, 14,FONTS::visuals_esp_font,false,false, CColor(255, 255, 255, 255), "ste");
	RENDER::DrawF(1853, 14, FONTS::visuals_esp_font, false, false,CColor(255, 255, 255, 255), thingy + get_time().c_str());
	RENDER::DrawF(1745, 14, FONTS::visuals_esp_font, false, false, CColor(255, 255, 255, 255), thingy + std::to_string(getfps()));
	RENDER::DrawF(1775, 14, FONTS::visuals_esp_font, false, false, CColor(255, 255, 255, 255), thingy + std::to_string(getping()));
	RENDER::DrawF(1800, 14, FONTS::visuals_esp_font, false, false, CColor(255, 255, 255, 255), thingy + "ur name");
	RENDER::DrawF(1724, 14, FONTS::visuals_esp_font, false, false, CColor(255, 64, 64), "lar");
}
void CVisuals::DrawTaserDist()
{
		if (!INTERFACES::Engine->IsConnected() || !INTERFACES::Engine->IsInGame())
			return;

		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player) return;
		if (local_player->GetHealth() <= 0) return;


		auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
		if (!weapon) return;

		if (weapon->GetItemDefenitionIndex() != ItemDefinitionIndex::WEAPON_ZEUS)return;

		float step = M_PI * 2.0 / 1023;
		float rad = 130.f;
		Vector origin = local_player->GetEyePosition();

		static double rainbow;

		Vector screenPos;
		static Vector prevScreenPos;

		for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
		{
			Vector pos(rad * cos(rotation) + origin.x, rad * sin(rotation) + origin.y, origin.z);

			SDK::CTraceFilter filter;
			SDK::trace_t trace;
			filter.pSkip1 = local_player;
			SDK::Ray_t t;
			t.Init(origin, pos);

			INTERFACES::Trace->TraceRay(t, MASK_SHOT_BRUSHONLY, &filter, &trace);

			if (INTERFACES::DebugOverlay->ScreenPosition(trace.end, screenPos))
				continue;

			if (!prevScreenPos.IsZero() && !screenPos.IsZero() && screenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f && prevScreenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f) {
				rainbow += 0.00001;
				if (rainbow > 1.f)
					rainbow = 0;
				CColor color = CColor::FromHSB(rainbow, 1.f, 1.f);
				RENDER::DrawLine(prevScreenPos.x, prevScreenPos.y, screenPos.x, screenPos.y, color);
			}
			prevScreenPos = screenPos;
		}

}
Hitmarkers hitmarkers;
void Hitmarkers::Do()
{
	if (!hitmarker_vector.size())
		return;

	if (fabs(UTILS::GetCurtime() - hitmarker_vector.back().m_time) > hitmarker_damage_fade_time)
	{
		hitmarker_vector.pop_back();

		if (!hitmarker_vector.size())
			return;
	}

	int width, height;
	INTERFACES::Engine->GetScreenSize(width, height);

	int center_width = width * 0.5f, center_height = height * 0.5f;

	if (Config::Visuals::Main::VisualsEffects::get().HitEffect[4]) {
		for (const auto hitmarker : hitmarker_vector)
		{
			const float time_delta = fabs(UTILS::GetCurtime() - hitmarker.m_time);
			const float extra_height = (time_delta / hitmarker_damage_fade_time) * hitmarker_damage_float_height;

			const int alpha = 255 - ((time_delta / hitmarker_damage_fade_time) * 255.f);
			const CColor color = CColor(min(255, (hitmarker.m_damage * 0.01f) * 255.f), max(0, 255 - ((hitmarker.m_damage * 0.01f) * 255.f)), 0, alpha);

			RENDER::DrawF(center_width, center_height - extra_height, FONTS::visuals_esp_font, true, true, color, std::to_string(hitmarker.m_damage));
			//RENDER::DrawF(center_width + 75 + (sin(time_delta * 2.5f) * 60.f), center_height - extra_height, FONTS::visuals_esp_font, true, true, color, std::to_string(hitmarker.m_damage));
		}
	}

	const auto hitmarker = hitmarker_vector.front();
	static int lineSize = 6;
	int screenSizeX, screenCenterX;
	int screenSizeY, screenCenterY;
	INTERFACES::Engine->GetScreenSize(screenSizeX, screenSizeY);
	const float time_delta = fabs(UTILS::GetCurtime() - hitmarker.m_time);
	const int alpha = 255 - ((time_delta / hitmarker_damage_fade_time) * 255.f);
	screenCenterX = screenSizeX / 2;
	screenCenterY = screenSizeY / 2;
	CColor col = CColor(255, 255, 255, alpha);

	RENDER::DrawLine(screenCenterX - lineSize * 2, screenCenterY - lineSize * 2, screenCenterX - (lineSize), screenCenterY - (lineSize), col);
	RENDER::DrawLine(screenCenterX - lineSize * 2, screenCenterY + lineSize * 2, screenCenterX - (lineSize), screenCenterY + (lineSize), col);
	RENDER::DrawLine(screenCenterX + lineSize * 2, screenCenterY + lineSize * 2, screenCenterX + (lineSize), screenCenterY + (lineSize), col);
	RENDER::DrawLine(screenCenterX + lineSize * 2, screenCenterY - lineSize * 2, screenCenterX + (lineSize), screenCenterY - (lineSize), col);

	RENDER::DrawLine(center_width + 4, center_height + 4, center_width + 7, center_height + 7, col);
	RENDER::DrawLine(center_width - 4, center_height + 4, center_width - 7, center_height + 7, col);
	RENDER::DrawLine(center_width + 4, center_height - 4, center_width + 7, center_height - 7, col);
	RENDER::DrawLine(center_width - 4, center_height - 4, center_width - 7, center_height - 7, col);
}
void Hitmarkers::AddHitmarker(int damage)
{
	hitmarker_vector.insert(hitmarker_vector.begin(), Hitmarker(UTILS::GetCurtime(), damage));
}


void CVisuals::DrawBorderLines()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	auto weapon = INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex());
	if (!weapon) return;

	int screen_x;
	int screen_y;
	int center_x;
	int center_y;
	INTERFACES::Engine->GetScreenSize(screen_x, screen_y);
	INTERFACES::Engine->GetScreenSize(center_x, center_y);
	center_x /= 2; center_y /= 2;

	if (local_player->GetIsScoped())
	{
		RENDER::DrawLine(0, center_y, screen_x, center_y, CColor(0, 0, 0, 255));
		RENDER::DrawLine(center_x, 0, center_x, screen_y, CColor(0, 0, 0, 255));
	}
}
void setClanTag(const char* tag, const char* name)//190% paste
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)UTILS::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));
	pSetClanTag(tag, name);
}
int kek = 0;
int autism = 0;
const char* clantagAnim[12] = {
	" hawkware ", "  hawkware ", "e  hawkwar", //3
	"re  hawkwa", "are  hawkw", "ware  hawk", //6
	"kware  haw", "wkware  ha", "awkware  h", //9
	"hawkware  ", " hawkware ", " hawkware " //12
};
void CVisuals::Clantag()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	static size_t lastTime = 0;
	if (GetTickCount() > lastTime)
	{
		kek++;
		if (kek > 10) {
			autism++; if (autism >= 12) autism = 0;
			setClanTag(clantagAnim[autism], "hawkware");
			lastTime = GetTickCount() + 500;
		}

		if (kek > 11) kek = 0;
	}
}

CVisuals* visuals = new CVisuals();
