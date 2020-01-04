
#include "LegitBot.h"
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
#include "../SDK/CTrace.h"	
#include "../HOOKS/hooks.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/NetChannel.h"
#include "../UTILS/render.h"
#include "../SDK/IVDebugOverlay.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/Collideable.h"
#include "../SDK/CInput.h"
#include "Aimbot.h"
#include "AutoWall.h"
#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

void LegitBot::GunCheck() {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	Enable = false;
	Silent = false;
	rcs = false;
	FOV = 0;
	Smoth = 0;
	hitchance = 0;
	Delay = 0;
	rcs_x = 0;
	rcs_y = 0;
	hitboxes[5] = false, false, false, false, false;


	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon)return;

	if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_CZ75A || weapon->GetItemDefenitionIndex() == SDK::WEAPON_ELITE || weapon->GetItemDefenitionIndex() == SDK::WEAPON_FIVESEVEN || weapon->GetItemDefenitionIndex() == SDK::WEAPON_GLOCK || weapon->GetItemDefenitionIndex() == SDK::WEAPON_HKP2000 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_P250 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_TEC9 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_USP_SILENCER) {
		Enable = Config::Legitbot::LegitPistol::get().enable;
		Silent = Config::Legitbot::LegitPistol::get().silent;
		rcs = Config::Legitbot::LegitPistol::get().rcs_enable;
		Smoth = Config::Legitbot::LegitPistol::get().smooth;
		FOV = Config::Legitbot::LegitPistol::get().fov;
		hitchance = Config::Legitbot::LegitPistol::get().triggerbot_hitchance;
		Delay = Config::Legitbot::LegitPistol::get().delay;
		rcs_x = Config::Legitbot::LegitPistol::get().rcs_x;
		rcs_y = Config::Legitbot::LegitPistol::get().rcs_y;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Legitbot::LegitPistol::get().hitboxes[i];

	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_DEAGLE || weapon->GetItemDefenitionIndex() == SDK::WEAPON_REVOLVER) {
		Enable = Config::Legitbot::LegitHeavyPistol::get().enable;
		Silent = Config::Legitbot::LegitHeavyPistol::get().silent;
		rcs = Config::Legitbot::LegitHeavyPistol::get().rcs_enable;
		Smoth = Config::Legitbot::LegitHeavyPistol::get().smooth;
		FOV = Config::Legitbot::LegitHeavyPistol::get().fov;
		hitchance = Config::Legitbot::LegitHeavyPistol::get().triggerbot_hitchance;
		Delay = Config::Legitbot::LegitHeavyPistol::get().delay;
		rcs_x = Config::Legitbot::LegitHeavyPistol::get().rcs_x;
		rcs_y = Config::Legitbot::LegitHeavyPistol::get().rcs_y;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Legitbot::LegitHeavyPistol::get().hitboxes[i];
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_P90 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_BIZON || weapon->GetItemDefenitionIndex() == SDK::WEAPON_MP9 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_MP7 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_MP5SD || weapon->GetItemDefenitionIndex() == SDK::WEAPON_MAC10) {
		Enable = Config::Legitbot::LegitSMG::get().enable;
		Silent = Config::Legitbot::LegitSMG::get().silent;
		rcs = Config::Legitbot::LegitSMG::get().rcs_enable;
		Smoth = Config::Legitbot::LegitSMG::get().smooth;
		FOV = Config::Legitbot::LegitSMG::get().fov;
		hitchance = Config::Legitbot::LegitSMG::get().triggerbot_hitchance;
		Delay = Config::Legitbot::LegitSMG::get().delay;
		rcs_x = Config::Legitbot::LegitSMG::get().rcs_x;
		rcs_y = Config::Legitbot::LegitSMG::get().rcs_y;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Legitbot::LegitSMG::get().hitboxes[i];
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_AK47 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_M4A1 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_M4A1_SILENCER || weapon->GetItemDefenitionIndex() == SDK::WEAPON_FAMAS || weapon->GetItemDefenitionIndex() == SDK::WEAPON_GALILAR || weapon->GetItemDefenitionIndex() == SDK::WEAPON_SG553 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_AUG) {
		Enable = Config::Legitbot::LegitAR::get().enable;
		Silent = Config::Legitbot::LegitAR::get().silent;
		rcs = Config::Legitbot::LegitAR::get().rcs_enable;
		Smoth = Config::Legitbot::LegitAR::get().smooth;
		FOV = Config::Legitbot::LegitAR::get().fov;
		hitchance = Config::Legitbot::LegitAR::get().triggerbot_hitchance;
		Delay = Config::Legitbot::LegitAR::get().delay;
		rcs_x = Config::Legitbot::LegitAR::get().rcs_x;
		rcs_y = Config::Legitbot::LegitAR::get().rcs_y;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Legitbot::LegitAR::get().hitboxes[i];
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_SCAR20 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_G3SG1) {
		Enable = Config::Legitbot::LegitAuto::get().enable;
		Silent = Config::Legitbot::LegitAuto::get().silent;
		rcs = Config::Legitbot::LegitAuto::get().rcs_enable;
		Smoth = Config::Legitbot::LegitAuto::get().smooth;
		FOV = Config::Legitbot::LegitAuto::get().fov;
		hitchance = Config::Legitbot::LegitAuto::get().triggerbot_hitchance;
		Delay = Config::Legitbot::LegitAuto::get().delay;
		rcs_x = Config::Legitbot::LegitAuto::get().rcs_x;
		rcs_y = Config::Legitbot::LegitAuto::get().rcs_y;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Legitbot::LegitAuto::get().hitboxes[i];
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_SSG08) {
		Enable = Config::Legitbot::LegitScout::get().enable;
		Silent = Config::Legitbot::LegitScout::get().silent;
		rcs = Config::Legitbot::LegitScout::get().rcs_enable;
		Smoth = Config::Legitbot::LegitScout::get().smooth;
		FOV = Config::Legitbot::LegitScout::get().fov;
		hitchance = Config::Legitbot::LegitScout::get().triggerbot_hitchance;
		Delay = Config::Legitbot::LegitScout::get().delay;
		rcs_x = Config::Legitbot::LegitScout::get().rcs_x;
		rcs_y = Config::Legitbot::LegitScout::get().rcs_y;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Legitbot::LegitScout::get().hitboxes[i];
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_AWP) {
		Enable = Config::Legitbot::LegitAWP::get().enable;
		Silent = Config::Legitbot::LegitAWP::get().silent;
		rcs = Config::Legitbot::LegitAWP::get().rcs_enable;
		Smoth = Config::Legitbot::LegitAWP::get().smooth;
		FOV = Config::Legitbot::LegitAWP::get().fov;
		hitchance = Config::Legitbot::LegitAWP::get().triggerbot_hitchance;
		Delay = Config::Legitbot::LegitAWP::get().delay;
		rcs_x = Config::Legitbot::LegitAWP::get().rcs_x;
		rcs_y = Config::Legitbot::LegitAWP::get().rcs_y;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Legitbot::LegitAWP::get().hitboxes[i];
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_NOVA || weapon->GetItemDefenitionIndex() == SDK::WEAPON_MAG7 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_SAWEDOFF || weapon->GetItemDefenitionIndex() == SDK::WEAPON_XM1014) {
		Enable = Config::Legitbot::LegitShotGuns::get().enable;
		Silent = Config::Legitbot::LegitShotGuns::get().silent;
		rcs = Config::Legitbot::LegitShotGuns::get().rcs_enable;
		Smoth = Config::Legitbot::LegitShotGuns::get().smooth;
		FOV = Config::Legitbot::LegitShotGuns::get().fov;
		hitchance = Config::Legitbot::LegitShotGuns::get().triggerbot_hitchance;
		Delay = Config::Legitbot::LegitShotGuns::get().delay;
		rcs_x = Config::Legitbot::LegitShotGuns::get().rcs_x;
		rcs_y = Config::Legitbot::LegitShotGuns::get().rcs_y;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Legitbot::LegitShotGuns::get().hitboxes[i];
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_NEGEV || weapon->GetItemDefenitionIndex() == SDK::WEAPON_M249) {
		Enable = Config::Legitbot::LegitLMG::get().enable;
		Silent = Config::Legitbot::LegitLMG::get().silent;
		rcs = Config::Legitbot::LegitLMG::get().rcs_enable;
		Smoth = Config::Legitbot::LegitLMG::get().smooth;
		FOV = Config::Legitbot::LegitLMG::get().fov;
		hitchance = Config::Legitbot::LegitLMG::get().triggerbot_hitchance;
		Delay = Config::Legitbot::LegitLMG::get().delay;
		rcs_x = Config::Legitbot::LegitLMG::get().rcs_x;
		rcs_y = Config::Legitbot::LegitLMG::get().rcs_y;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Legitbot::LegitLMG::get().hitboxes[i];
	}


}
static Vector CalcVecSD(Vector src, Vector dst)
{
	Vector angles;
	Vector delta = src - dst;
	angles.x = (asinf(delta.z / delta.Length()) * 57.295779513082f);
	angles.y = (atanf(delta.y / delta.x) * 57.295779513082f);
	angles.z = 0.0f;
	if (delta.x >= 0.0) { angles.y += 180.0f; }

	return angles;
}
//got from uc
void angleVectors(const Vector& angles, Vector& forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}
static float GetFov(const Vector& viewAngle, const Vector& aimAngle)
{
	Vector ang, aim;

	angleVectors(viewAngle, aim);
	angleVectors(aimAngle, ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}
inline Vector ExtrapolateTick(Vector p0, Vector v0)
{
	return p0 + (v0 * INTERFACES::Globals->interval_per_tick);
}
Vector SDK::CBaseEntity::GetPredicted(Vector p0)
{
	return ExtrapolateTick(p0, this->GetVelocity());
}
SDK::mstudiobbox_t* GetHitbox(SDK::CBaseEntity* entity, int hitbox_index)
{
	if (entity->GetIsDormant() || entity->GetHealth() <= 0)
		return nullptr;

	const auto pModel = entity->GetModel();
	if (!pModel)
		return nullptr;

	auto pStudioHdr = INTERFACES::ModelInfo->GetStudioModel(pModel);
	if (!pStudioHdr)
		return nullptr;

	auto pSet = pStudioHdr->pHitboxSet(0);
	if (!pSet)
		return nullptr;

	if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
		return nullptr;

	return pSet->GetHitbox(hitbox_index);
}
Vector GetHitboxPosition(SDK::CBaseEntity* entity, int Hitbox)
{
	auto hitbox = GetHitbox(entity, Hitbox);
	if (!hitbox)
		return Vector(0, 0, 0);

	auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax, vCenter;
	MATH::VectorTransform(hitbox->bbmin, bone_matrix, bbmin);
	MATH::VectorTransform(hitbox->bbmax, bone_matrix, bbmax);

	vCenter = (bbmin + bbmax) * 0.5f;

	return vCenter;
}
static void Normalize(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}
float FovToPlayer(Vector ViewOffSet, Vector View, SDK::CBaseEntity* pEntity, int aHitBox)
{
	Vector out[9];

	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 180.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	//Vector Origin(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	MATH::AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);
	// Get delta vector between our local eye position and passed vector
	MATH::VectorSubtract(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	Normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / M_PI));
}
inline Vector angle_vector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(M_PI));
	auto sp = sin(meme.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(M_PI));
	return Vector(cp*cy, cp*sy, -sp);
}



inline float point_to_line(Vector Point, Vector LineOrigin, Vector Dir)

{
	auto PointDir = Point - LineOrigin;
	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();

}
void run_legit(SDK::CUserCmd* cmd) //phook backtrack muahhahahahaaha

{
	int bestTargetIndex = -1;
	float bestFov = FLT_MAX;
	SDK::player_info_t info;
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	for (int i = 1; i < 65; i++)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (!entity) continue;
		if (!INTERFACES::Engine->GetPlayerInfo(i, &info)) continue;
		if (entity->GetIsDormant()) continue;
		if (entity->GetTeam() == local_player->GetTeam()) continue;
		if (entity->GetHealth() <= 0) continue;
		if (local_player->GetHealth() <= 0) return;
		float simtime = entity->GetSimTime(); Vector hitboxPos = aimbot->get_hitbox_pos(entity, 0);
		headPositions[i][cmd->command_number % 12] = legit_backtrackdata{ simtime, hitboxPos };
		Vector ViewDir = angle_vector(cmd->viewangles + (local_player->GetPunchAngles() * 2.f));
		float FOVDistance = point_to_line(hitboxPos, local_player->GetEyePosition(), ViewDir);
		if (bestFov > FOVDistance)
		{
			bestFov = FOVDistance;
			bestTargetIndex = i;
		}
	}

	float bestTargetSimTime;
	if (bestTargetIndex != -1)
	{
		float tempFloat = FLT_MAX;
		Vector ViewDir = angle_vector(cmd->viewangles + (local_player->GetPunchAngles() * 2.f));
		for (int t = 0; t < 12; ++t)
		{
			float tempFOVDistance = point_to_line(headPositions[bestTargetIndex][t].hitboxPos, local_player->GetEyePosition(), ViewDir);
			if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > local_player->GetSimTime() - 1)
			{
				tempFloat = tempFOVDistance;
				bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
			}
		}
		if (cmd->buttons & IN_ATTACK)
			cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
	}
}

void LegitBot::Run(SDK::CUserCmd* cmd) {
	if (INTERFACES::Engine->IsInGame() && INTERFACES::Engine->IsConnected()) {
		GunCheck();
		if (Config::Legitbot::LegitGlobal::get().backtrack_enable)
			run_legit(cmd);
		if (GetAsyncKeyState(Config::Legitbot::LegitGlobal::get().trigerbot_key) && Config::Legitbot::LegitGlobal::get().trigerbot)
			TriggerBot(cmd);

		if (rcs)
			RCS(cmd);
		if (Enable) {
			Move(cmd);
		}
	}

}
float inaccuracy(SDK::CBaseWeapon* weapon)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return 0;

	if (!weapon) return 0;
	if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_TASER) return 0;

	float inaccuracy = weapon->GetInaccuracy();
	if (inaccuracy == 0) inaccuracy = 0.0000001;
	inaccuracy = 1 / inaccuracy;
	return inaccuracy;
}
inline bool LineGoesThroughSmoke(const Vector& start, const Vector& end, bool bUnknown = true)
{
	typedef bool(*LineGoesThroughSmokeFn) (Vector, Vector, int16_t);
	static LineGoesThroughSmokeFn dw_fnLineGoesThroughSmoke;

	if (!dw_fnLineGoesThroughSmoke)
		dw_fnLineGoesThroughSmoke = (LineGoesThroughSmokeFn)UTILS::FindSignature("client_panorama.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");

	return dw_fnLineGoesThroughSmoke(start, end, bUnknown);
}


void LegitBot::TriggerBot(SDK::CUserCmd* cmd) {

	Vector src, dst, forward;
	SDK::trace_t tr;
	SDK::Ray_t ray;
	SDK::CTraceFilter filter;
	Vector viewangle = cmd->viewangles;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon || weapon->is_grenade() || weapon->is_knife() || weapon->GetItemDefenitionIndex() == ItemDefinitionIndex::WEAPON_C4) return;

	viewangle += local_player->GetPunchAngles() * 2.f;
	MATH::AngleVectors(viewangle, &forward);
	forward *= weapon->get_full_info()->m_Range;
	filter.pSkip1 = local_player;

	src = local_player->GetEyePosition();
	dst = src + forward;

	ray.Init(src, dst);

	INTERFACES::Trace->TraceRay(ray, 0x46004003, &filter, &tr);

	if (!tr.m_pEnt)
		return;
	
	if (inaccuracy(weapon) < hitchance)
		return;

	if (Config::Legitbot::LegitGlobal::get().smoke_check) {
		if (LineGoesThroughSmoke(src, dst))
			return;
	}


	int hitgroup = tr.hitGroup;
	bool didHit = false;
	didHit = false;

	if (hitgroup == HITGROUP_HEAD || hitgroup == HITGROUP_CHEST || hitgroup == HITGROUP_STOMACH || hitgroup == HITGROUP_LEFTLEG || hitgroup == HITGROUP_RIGHTLEG)
		didHit = true;
	else
		didHit = false;

	if (tr.m_pEnt->GetTeam() == local_player->GetTeam()) return;
	if (tr.m_pEnt->GetHealth() <= 0) return;
	if (tr.m_pEnt->GetImmunity()) return;
	if (didHit) cmd->buttons |= IN_ATTACK;
}
float distance(Vector src, Vector dest)
{
	Vector delta = src - dest;
	float fl_dist = ::sqrtf((delta.Length()));
	if (fl_dist < 1.0f) return 1.0f;
	return fl_dist;
}
void LegitBot::Smooth(SDK::CBaseEntity* player, Vector angle, SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;
	if (!player)
		return;

	Vector viewAngles = Vector(0.f, 0.f, 0.f);
	INTERFACES::Engine->GetViewAngles(viewAngles);

	Vector delta = angle - viewAngles;
	MATH::NormalizeAngle(delta);

	float smoothx = Smoth;
	float smoothy = Smoth;

#undef min
	smoothx = std::min(0.99f, smoothx);
	smoothy = std::min(0.99f, smoothy);


	Vector toChange = Vector();

	toChange.x = delta.x - delta.x * smoothx;
	toChange.y = delta.y - delta.y * smoothy;

	angle = viewAngles + toChange;

	INTERFACES::Engine->SetViewAngles(Vector(angle.x, angle.y, 0));
	cmd->viewangles = angle;
}
int LegitBot::GetBestTarget() {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return -1;

	int index = -1;
	float lowest = INT_MAX;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	Vector angles;
	INTERFACES::Engine->GetViewAngles(angles);

	for (int i = 1; i <= INTERFACES::Globals->maxclients; i++)
	{
		SDK::CBaseEntity *entity = INTERFACES::ClientEntityList->GetClientEntity(i);

		if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || entity->GetIsDormant() || entity == local_player)
			continue;

		if (Config::Legitbot::LegitGlobal::get().wall_check) {
			if (!UTILS::IsPlayerVisible(entity)) //autowall check
				continue;
		}
		if (Config::Legitbot::LegitGlobal::get().smoke_check) {
			if (LineGoesThroughSmoke(local_player->GetEyePosition(), angles)) //smokecheck
				continue;
		}


		float fov = FovToPlayer(local_position, angles, entity, 0);
		Vector vecOrigin = entity->GetVecOrigin(), vecOriginLocal = local_player->GetVecOrigin();
		float dsit = distance(vecOriginLocal, vecOrigin);
		float hp = entity->GetHealth();


		switch (Config::Legitbot::LegitGlobal::get().enemyscan) {
		case 0: {
			if (fov < lowest && fov < FOV)
			{
				lowest = fov;
				index = i;
			}
		} break;
		case 1: {
			if (dsit < lowest && fov < FOV)
			{
				lowest = fov;
				index = i;
			}

		}break;
		case 2: {
			if (hp < lowest && fov < FOV)
			{
				lowest = fov;
				index = i;
			}
		
		} break;
		}
	}

	return index;
}
inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}
void LegitBot::DoAimbot(SDK::CUserCmd *pCmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	if (local_player->GetHealth() <= 0)
		return;

	if (local_player->GetFlags() & FL_ATCONTROLS)
		return;

	auto pWeapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!pWeapon)
		return;


	if (GetBestTarget() == -1)return;
	auto entity = INTERFACES::ClientEntityList->GetClientEntity(GetBestTarget());

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();
	Vector shoot_here;
	point(entity, shoot_here);
	if (shoot_here == Vector(0, 0, 0))
		return;
	Vector AimAngle;
	AimAngle = MATH::NormalizeAngle(UTILS::CalcAngle(local_player->GetEyePosition(), entity->GetPredicted(shoot_here)));
	if (AimAngle == Vector(0, 0, 0))
		return;
	Vector Aimpunch = local_player->GetPunchAngles();
	if (GetAsyncKeyState(Config::Legitbot::LegitGlobal::get().legitbot_key))
	{
		//AimAngle.x -= Aimpunch.x * 2;
		if (!Silent)
			Smooth(entity, AimAngle, pCmd);
		else {
			pCmd->viewangles = AimAngle;
			global::should_send_packet = false;
		}
	}
}
Vector LegitBot::point(SDK::CBaseEntity* entity, Vector& vec_out) {
	std::vector<int> HitBoxesToScan;
	// Get the hitboxes to scan

	if (hitboxes[0])		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_HEAD));
	if (hitboxes[1]) {
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_BODY));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_CHEST));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_PELVIS));
	}
	if (hitboxes[2]) {
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_NECK));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_UPPER_CHEST));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_THORAX));
	}
	if (hitboxes[3]) { 
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_LEFT_FOOT));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_LEFT_THIGH));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_LEFT_CALF));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_RIGHT_FOOT));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_RIGHT_THIGH));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_RIGHT_CALF));
	}
	if (hitboxes[4]) {
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_LEFT_HAND));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_LEFT_FOREARM));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_LEFT_UPPER_ARM));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_RIGHT_HAND));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_RIGHT_FOREARM));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_RIGHT_UPPER_ARM));
	}

	// check hits
	for (auto HitBoxID : HitBoxesToScan)
	{
		Vector pos = GetHitboxPosition(entity, SDK::HitboxList(HitBoxID));
		if (UTILS::IsVisiblePos(pos))
		{
			vec_out = pos;

			if (vec_out.IsValid() && !vec_out.IsZero())
				return vec_out;
			else
				vec_out = Vector(0, 0, 0);
		}
	}



}
void LegitBot::RCS(SDK::CUserCmd* cmd)
{
	if (!(cmd->buttons & IN_ATTACK))
		return;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return;
	Vector punch = local_player->GetPunchAngles();


	cmd->viewangles.x -= punch.x * (rcs_x / 50);
	cmd->viewangles.y -= punch.y * (rcs_y / 50);
}
void LegitBot::Move(SDK::CUserCmd * pCmd)
{
	DoAimbot(pCmd);
}
LegitBot* legit = new LegitBot();

legit_backtrackdata headPositions[64][12];