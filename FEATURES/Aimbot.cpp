#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ConVar.h"
#include "../FEATURES/AutoWall.h"
#include "../SDK/NetChannel.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Movement.h"
#include "../FEATURES/NewEventLog.h"
#include "../UTILS/render.h"

#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

int bestHitbox = -1, mostDamage, multipointCount = 0;;
bool lag_comp;
bool quickstopped;
bool resolved[64];
Vector multipoints[128];

int lerped_ticks() {

	static bool 2;

	static const auto cl_interp_ratio = INTERFACES::cvar->FindVar("cl_interp_ratio");
	static const auto cl_updaterate = INTERFACES::cvar->FindVar("cl_updaterate");
	static const auto cl_interp = INTERFACES::cvar->FindVar("cl_interp");

	return TIME_TO_TICKS(max(cl_interp->GetFloat(), cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat()));
}

static SDK::ConVar *big_ud_rate = nullptr,
*min_ud_rate = nullptr, *max_ud_rate = nullptr,
*interp_ratio = nullptr, *cl_interp = nullptr,
*cl_min_interp = nullptr, *cl_max_interp = nullptr;

float LerpTime() {

	static SDK::ConVar* updaterate = INTERFACES::cvar->FindVar("cl_updaterate");
	static SDK::ConVar* minupdate = INTERFACES::cvar->FindVar("sv_minupdaterate");
	static SDK::ConVar* maxupdate = INTERFACES::cvar->FindVar("sv_maxupdaterate");
	static SDK::ConVar* lerp = INTERFACES::cvar->FindVar("cl_interp");
	static SDK::ConVar* cmin = INTERFACES::cvar->FindVar("sv_client_min_interp_ratio");
	static SDK::ConVar* cmax = INTERFACES::cvar->FindVar("sv_client_max_interp_ratio");
	static SDK::ConVar* ratio = INTERFACES::cvar->FindVar("cl_interp_ratio");

	float lerpurmom = lerp->GetFloat(), maxupdateurmom = maxupdate->GetFloat(),
		ratiourmom = ratio->GetFloat(), cminurmom = cmin->GetFloat(), cmaxurmom = cmax->GetFloat();
	int updaterateurmom = updaterate->GetInt(),
		sv_maxupdaterate = maxupdate->GetInt(), sv_minupdaterate = minupdate->GetInt();

	if (sv_maxupdaterate && sv_minupdaterate)
		updaterateurmom = maxupdateurmom;

	if (ratiourmom == 0)
		ratiourmom = 1.0f;

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratiourmom = clamp(ratiourmom, cminurmom, cmaxurmom);

	return max(lerpurmom, ratiourmom / updaterateurmom);
}

bool CAimbot::good_backtrack_tick(int tick)
{
	auto nci = INTERFACES::Engine->GetNetChannelInfo();
	if (!nci) return false;

	float correct = clamp(nci->GetLatency(FLOW_OUTGOING) + LerpTime(), 0.f, 1.f);
	float delta_time = correct - (INTERFACES::Globals->curtime - TICKS_TO_TIME(tick));
	return fabsf(delta_time) < 0.2f;
}
float CAimbot::lag_fix()
{
	float updaterate = INTERFACES::cvar->FindVar("cl_updaterate")->GetFloat();
	static auto minupdate = INTERFACES::cvar->FindVar("sv_minupdaterate");
	static auto maxupdate = INTERFACES::cvar->FindVar("sv_maxupdaterate");

	if (minupdate && maxupdate)
		updaterate = maxupdate->GetFloat();

	float ratio = INTERFACES::cvar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = INTERFACES::cvar->FindVar("cl_interp")->GetFloat();
	static auto cmin = INTERFACES::cvar->FindVar("sv_client_min_interp_ratio");
	static auto cmax = INTERFACES::cvar->FindVar("sv_client_max_interp_ratio");

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratio = clamp(ratio, cmin->GetFloat(), cmax->GetFloat());

	return max(lerp, ratio / updaterate);
}
inline Vector ExtrapolateTick1(Vector p0, Vector v0)
{
	return p0 + (v0 * INTERFACES::Globals->interval_per_tick);
}
SDK::mstudiobbox_t* GetHitbox1(SDK::CBaseEntity* entity, int hitbox_index)
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
Vector GetHitboxPosition1(SDK::CBaseEntity* entity, int Hitbox)
{
	auto hitbox = GetHitbox1(entity, Hitbox);
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
float FovToPlayer1(Vector ViewOffSet, Vector View, SDK::CBaseEntity* pEntity, int aHitBox)
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
	Vector AimPos = GetHitboxPosition1(pEntity, aHitBox);
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
float distance_to_player(Vector src, Vector dest)
{
	Vector delta = src - dest;
	float fl_dist = ::sqrtf((delta.Length()));
	if (fl_dist < 1.0f) return 1.0f;
	return fl_dist;
}
void CAimbot::GunCheck() {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	enable = false;
	hitchance = 0;
	min_dmg = 0;
	min_dmg_autowall = 0;
	prefer_hitbox = 0;
	prefer = 0;
	auto_stop_method = 0;
	multipoint_head = 0;
	multipoint_body = 0;
	multipoint_arms = 0;
	multipoint_neck = 0;
	multipoint_legs = 0;
	hitboxes[5] = false, false, false, false, false;


	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon)return;

	if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_CZ75A || weapon->GetItemDefenitionIndex() == SDK::WEAPON_ELITE || weapon->GetItemDefenitionIndex() == SDK::WEAPON_FIVESEVEN || weapon->GetItemDefenitionIndex() == SDK::WEAPON_GLOCK || weapon->GetItemDefenitionIndex() == SDK::WEAPON_HKP2000 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_P250 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_TEC9 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_USP_SILENCER) {

		enable = Config::Ragebot::RagePistol::get().enable;
		hitchance = Config::Ragebot::RagePistol::get().hitchance;
		min_dmg = Config::Ragebot::RagePistol::get().min_dmg;
		min_dmg_autowall = Config::Ragebot::RagePistol::get().min_dmg_autowall;
		prefer_hitbox = Config::Ragebot::RagePistol::get().prefer_hitbox;
		prefer = Config::Ragebot::RagePistol::get().prefer;
		auto_stop_method = Config::Ragebot::RagePistol::get().auto_stop_method;
		multipoint_head = Config::Ragebot::RagePistol::get().multipoint_head;
		multipoint_body = Config::Ragebot::RagePistol::get().multipoint_body;
		multipoint_arms = Config::Ragebot::RagePistol::get().multipoint_arms;
		multipoint_neck = Config::Ragebot::RagePistol::get().multipoint_neck;
		multipoint_legs = Config::Ragebot::RagePistol::get().multipoint_legs;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Ragebot::RagePistol::get().hitboxes[i];

	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_DEAGLE || weapon->GetItemDefenitionIndex() == SDK::WEAPON_REVOLVER) {
		enable = Config::Ragebot::RageHeavyPistol::get().enable;
		hitchance = Config::Ragebot::RageHeavyPistol::get().hitchance;
		min_dmg = Config::Ragebot::RageHeavyPistol::get().min_dmg;
		min_dmg_autowall = Config::Ragebot::RageHeavyPistol::get().min_dmg_autowall;
		prefer_hitbox = Config::Ragebot::RageHeavyPistol::get().prefer_hitbox;
		prefer = Config::Ragebot::RageHeavyPistol::get().prefer;
		auto_stop_method = Config::Ragebot::RageHeavyPistol::get().auto_stop_method;
		multipoint_head = Config::Ragebot::RageHeavyPistol::get().multipoint_head;
		multipoint_body = Config::Ragebot::RageHeavyPistol::get().multipoint_body;
		multipoint_arms = Config::Ragebot::RageHeavyPistol::get().multipoint_arms;
		multipoint_neck = Config::Ragebot::RageHeavyPistol::get().multipoint_neck;
		multipoint_legs = Config::Ragebot::RageHeavyPistol::get().multipoint_legs;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Ragebot::RageHeavyPistol::get().hitboxes[i];
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_P90 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_BIZON || weapon->GetItemDefenitionIndex() == SDK::WEAPON_MP9 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_MP7 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_MP5SD || weapon->GetItemDefenitionIndex() == SDK::WEAPON_MAC10) {
		enable = Config::Ragebot::RageSMG::get().enable;
		hitchance = Config::Ragebot::RageSMG::get().hitchance;
		min_dmg = Config::Ragebot::RageSMG::get().min_dmg;
		min_dmg_autowall = Config::Ragebot::RageSMG::get().min_dmg_autowall;
		prefer_hitbox = Config::Ragebot::RageSMG::get().prefer_hitbox;
		prefer = Config::Ragebot::RageSMG::get().prefer;
		auto_stop_method = Config::Ragebot::RageSMG::get().auto_stop_method;
		multipoint_head = Config::Ragebot::RageSMG::get().multipoint_head;
		multipoint_body = Config::Ragebot::RageSMG::get().multipoint_body;
		multipoint_arms = Config::Ragebot::RageSMG::get().multipoint_arms;
		multipoint_neck = Config::Ragebot::RageSMG::get().multipoint_neck;
		multipoint_legs = Config::Ragebot::RageSMG::get().multipoint_legs;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Ragebot::RageSMG::get().hitboxes[i];
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_AK47 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_M4A1 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_M4A1_SILENCER || weapon->GetItemDefenitionIndex() == SDK::WEAPON_FAMAS || weapon->GetItemDefenitionIndex() == SDK::WEAPON_GALILAR || weapon->GetItemDefenitionIndex() == SDK::WEAPON_SG553 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_AUG) {
		enable = Config::Ragebot::RageAR::get().enable;
		hitchance = Config::Ragebot::RageAR::get().hitchance;
		min_dmg = Config::Ragebot::RageAR::get().min_dmg;
		min_dmg_autowall = Config::Ragebot::RageAR::get().min_dmg_autowall;
		prefer_hitbox = Config::Ragebot::RageAR::get().prefer_hitbox;
		prefer = Config::Ragebot::RageAR::get().prefer;
		auto_stop_method = Config::Ragebot::RageAR::get().auto_stop_method;
		multipoint_head = Config::Ragebot::RageAR::get().multipoint_head;
		multipoint_body = Config::Ragebot::RageAR::get().multipoint_body;
		multipoint_arms = Config::Ragebot::RageAR::get().multipoint_arms;
		multipoint_neck = Config::Ragebot::RageAR::get().multipoint_neck;
		multipoint_legs = Config::Ragebot::RageAR::get().multipoint_legs;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Ragebot::RageAR::get().hitboxes[i];
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_SCAR20 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_G3SG1) {
		enable = Config::Ragebot::RageAuto::get().enable;
		hitchance = Config::Ragebot::RageAuto::get().hitchance;
		min_dmg = Config::Ragebot::RageAuto::get().min_dmg;
		min_dmg_autowall = Config::Ragebot::RageAuto::get().min_dmg_autowall;
		prefer_hitbox = Config::Ragebot::RageAuto::get().prefer_hitbox;
		prefer = Config::Ragebot::RageAuto::get().prefer;
		auto_stop_method = Config::Ragebot::RageAuto::get().auto_stop_method;
		multipoint_head = Config::Ragebot::RageAuto::get().multipoint_head;
		multipoint_body = Config::Ragebot::RageAuto::get().multipoint_body;
		multipoint_arms = Config::Ragebot::RageAuto::get().multipoint_arms;
		multipoint_neck = Config::Ragebot::RageAuto::get().multipoint_neck;
		multipoint_legs = Config::Ragebot::RageAuto::get().multipoint_legs;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Ragebot::RageAuto::get().hitboxes[i];
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_SSG08) {
		enable = Config::Ragebot::RageScout::get().enable;
		hitchance = Config::Ragebot::RageScout::get().hitchance;
		min_dmg = Config::Ragebot::RageScout::get().min_dmg;
		min_dmg_autowall = Config::Ragebot::RageScout::get().min_dmg_autowall;
		prefer_hitbox = Config::Ragebot::RageScout::get().prefer_hitbox;
		prefer = Config::Ragebot::RageScout::get().prefer;
		auto_stop_method = Config::Ragebot::RageScout::get().auto_stop_method;
		multipoint_head = Config::Ragebot::RageScout::get().multipoint_head;
		multipoint_body = Config::Ragebot::RageScout::get().multipoint_body;
		multipoint_arms = Config::Ragebot::RageScout::get().multipoint_arms;
		multipoint_neck = Config::Ragebot::RageScout::get().multipoint_neck;
		multipoint_legs = Config::Ragebot::RageScout::get().multipoint_legs;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Ragebot::RageScout::get().hitboxes[i];
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_AWP) {
		enable = Config::Ragebot::RageAWP::get().enable;
		hitchance = Config::Ragebot::RageAWP::get().hitchance;
		min_dmg = Config::Ragebot::RageAWP::get().min_dmg;
		min_dmg_autowall = Config::Ragebot::RageAWP::get().min_dmg_autowall;
		prefer_hitbox = Config::Ragebot::RageAWP::get().prefer_hitbox;
		prefer = Config::Ragebot::RageAWP::get().prefer;
		auto_stop_method = Config::Ragebot::RageAWP::get().auto_stop_method;
		multipoint_head = Config::Ragebot::RageAWP::get().multipoint_head;
		multipoint_body = Config::Ragebot::RageAWP::get().multipoint_body;
		multipoint_arms = Config::Ragebot::RageAWP::get().multipoint_arms;
		multipoint_neck = Config::Ragebot::RageAWP::get().multipoint_neck;
		multipoint_legs = Config::Ragebot::RageAWP::get().multipoint_legs;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Ragebot::RageAWP::get().hitboxes[i];
	}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_NOVA || weapon->GetItemDefenitionIndex() == SDK::WEAPON_MAG7 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_SAWEDOFF || weapon->GetItemDefenitionIndex() == SDK::WEAPON_XM1014) {
		enable = Config::Ragebot::RageShotGuns::get().enable;
		hitchance = Config::Ragebot::RageShotGuns::get().hitchance;
		min_dmg = Config::Ragebot::RageShotGuns::get().min_dmg;
		min_dmg_autowall = Config::Ragebot::RageShotGuns::get().min_dmg_autowall;
		prefer_hitbox = Config::Ragebot::RageShotGuns::get().prefer_hitbox;
		prefer = Config::Ragebot::RageShotGuns::get().prefer;
		auto_stop_method = Config::Ragebot::RageShotGuns::get().auto_stop_method;
		multipoint_head = Config::Ragebot::RageShotGuns::get().multipoint_head;
		multipoint_body = Config::Ragebot::RageShotGuns::get().multipoint_body;
		multipoint_arms = Config::Ragebot::RageShotGuns::get().multipoint_arms;
		multipoint_neck = Config::Ragebot::RageShotGuns::get().multipoint_neck;
		multipoint_legs = Config::Ragebot::RageShotGuns::get().multipoint_legs;
		for (int i = 0; i <= 5; i++)
			hitboxes[i] = Config::Ragebot::RageShotGuns::get().hitboxes[i];
		}
	else if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_NEGEV || weapon->GetItemDefenitionIndex() == SDK::WEAPON_M249) {
	enable = Config::Ragebot::RageLMG::get().enable;
	hitchance = Config::Ragebot::RageLMG::get().hitchance;
	min_dmg = Config::Ragebot::RageLMG::get().min_dmg;
	min_dmg_autowall = Config::Ragebot::RageLMG::get().min_dmg_autowall;
	prefer_hitbox = Config::Ragebot::RageLMG::get().prefer_hitbox;
	prefer = Config::Ragebot::RageLMG::get().prefer;
	auto_stop_method = Config::Ragebot::RageLMG::get().auto_stop_method;
	multipoint_head = Config::Ragebot::RageLMG::get().multipoint_head;
	multipoint_body = Config::Ragebot::RageLMG::get().multipoint_body;
	multipoint_arms = Config::Ragebot::RageLMG::get().multipoint_arms;
	multipoint_neck = Config::Ragebot::RageLMG::get().multipoint_neck;
	multipoint_legs = Config::Ragebot::RageLMG::get().multipoint_legs;
	for (int i = 0; i <= 5; i++)
		hitboxes[i] = Config::Ragebot::RageLMG::get().hitboxes[i];
	}


}
bool readyToFire() {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

	// match time. ( client_current_tick * tick_interval )
	if (local_player->GetNextAttack() > INTERFACES::Globals->curtime)
		return false;

	if (weapon->GetNextPrimaryAttack() > INTERFACES::Globals->curtime)
		return false;

	return true;
}
void restore(){
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	for (int i = 1; i <= INTERFACES::Globals->maxclients; i++)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		if (!entity || entity->GetImmunity() || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || !entity->IsAlive())
			continue;

		backtracking.RestoreToCurrentRecord(entity);
	}
}
void CAimbot::run_aimbot(SDK::CUserCmd* cmd)
{
	if (Config::Ragebot::RageGlobal::get().enable) {
		GunCheck();
		SelectTarget(cmd);
		shoot_enemy(cmd);
		if (Config::Ragebot::RageGlobal::get().backtrack_enable)
			restore();
	}
	if (Config::Misc::MiscMain::get().auto_zeus || Config::Misc::MiscMain::get().auto_knife) {
		SelectTarget2();
		Entities.clear();
		if (Config::Misc::MiscMain::get().auto_zeus)
			autozeus(cmd);

		if (Config::Misc::MiscMain::get().auto_knife)
			autoknife(cmd);
	}
}
SDK::CBaseEntity* bestentity = nullptr; //best entity so we dont need to call SelectTarget twice
int CAimbot::SelectTarget(SDK::CUserCmd* cmd)
{

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return -1;

	int index = -1;
	float lowest = INT_MAX;
	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();
	Vector vecOriginLocal = local_player->GetVecOrigin();

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon || weapon->is_grenade() || weapon->is_knife() || weapon->GetItemDefenitionIndex() == SDK::WEAPON_TASER || !readyToFire()) return -1;

	Vector angles;
	INTERFACES::Engine->GetViewAngles(angles);

	bestentity = nullptr; //lets set entity to nullpointer

	for (int i = 1; i <= INTERFACES::Globals->maxclients; i++)
	{
		SDK::CBaseEntity *entity = INTERFACES::ClientEntityList->GetClientEntity(i);

		if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || entity->GetImmunity() || entity->GetIsDormant() || entity == local_player || !entity->IsAlive() )
			continue;

		if (Config::Ragebot::RageGlobal::get().backtrack_enable) {
			for (const auto& rec : backtracking.GetPriorityRecords(entity))
			{

				const int tick = backtracking.GetTickCount(rec);
				int random_ticks = 0;
				if (rec.is_exploiting)
					random_ticks = TIME_TO_TICKS(UTILS::GetLatency());

				backtracking.Restore(entity, rec, random_ticks);
				backtracking.ApplyRestore(entity, TICKS_TO_TIME(tick));

				player_aimbot_info[i].tick = tick;
				player_aimbot_info[i].backtrack_record = rec;
				player_aimbot_info[i].extrapolted_ticks = random_ticks;
			}
		}

		if (Config::Ragebot::RageGlobal::get().fakelag_cor == 2 && resolver.GetDeltaTime(resolver.GetPlayerResolveInfo(entity).on_shot_eye_curtime) > 0.2)
			continue;

		player_aimbot_info[i].multipoint = aimbot->multipoint(entity, cmd);//multpoint
		if (player_aimbot_info[i].multipoint == Vector(0, 0, 0)) continue; ///checking if its actually right


		player_aimbot_info[i].aim_angles = MATH::NormalizeAngle(UTILS::CalcAngle(local_player->GetEyePosition(), player_aimbot_info[i].multipoint));
		if (player_aimbot_info[i].aim_angles == Vector(0, 0, 0)) continue;

		if (Config::Ragebot::RageGlobal::get().auto_scope && !local_player->GetIsScoped() && UTILS::IsWeaponSniper(weapon)) /// auto scope
			cmd->buttons |= IN_ATTACK2;

		if (!weapon->is_knife() && !(weapon->GetItemDefenitionIndex() == SDK::WEAPON_TASER) && auto_stop_method > 0 && local_player->GetFlags() & FL_ONGROUND && readyToFire() && !local_player->GetVelocity().Length2D() == 0) { /// autostop not fully done
			switch (auto_stop_method){
			case 1:
			{
				Vector Velocity = local_player->GetVelocity();

				static float Speed = 450.f;

				Vector Direction;
				Vector RealView;
				MATH::VectorAngles(Velocity, Direction);
				INTERFACES::Engine->GetViewAngles(RealView);
				Direction.y = RealView.y - Direction.y;

				Vector Forward;
				MATH::AngleVectors(Direction, &Forward);
				Vector NegativeDirection = Forward * -Speed;

				global::originalCMD.move.x = NegativeDirection.x;
				global::originalCMD.move.y = NegativeDirection.y;
			}break;
			case 2: {
				global::originalCMD.move.x = 0;
				global::originalCMD.move.y = 0;
			}break;
			case 3: {
				global::originalCMD.move.x = 0;
				global::originalCMD.move.y = Config::AntiAim::AAMisc::get().slowwwalk_speed;
			}break;
			}

		}


		if (accepted_inaccuracy(weapon) < hitchance)continue;


		float fov = FovToPlayer1(local_position, angles, entity, 0);
		Vector vecOrigin = entity->GetVecOrigin();
		float dsit = distance_to_player(vecOriginLocal, vecOrigin);
		float hp = entity->GetHealth();
		switch (Config::Ragebot::RageGlobal::get().enemyscan) {
		case 0: {
			if (fov < lowest && fov < Config::Ragebot::RageGlobal::get().max_fov)
			{
				lowest = fov;
				index = i; 
				bestentity = entity;
			}
		} break;
		case 1: {
			if (dsit < lowest && fov < Config::Ragebot::RageGlobal::get().max_fov)
			{
				lowest = fov;
				index = i;
				bestentity = entity;
			}

		}break;
		case 2: {
			if (hp < lowest && fov < Config::Ragebot::RageGlobal::get().max_fov)
			{
				lowest = fov;
				index = i;
				bestentity = entity;
			}

		} break;
		}
	}

	return index;
}
inline float FastSqrt(float x)
{
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}
#define square( x ) ( x * x )

void CAimbot::shoot_enemy(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0) return;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon || weapon->GetLoadedAmmo() == 0) return;
	if (weapon->get_full_info()->m_WeaponType == 9) return;
	if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_C4 || weapon->is_grenade() || weapon->is_knife()) return;
	if (!can_shoot(cmd)) { cmd->buttons &= ~IN_ATTACK;	return; }
	if (!enable) return;

	int best_tick, last_extrapolated_ticks;
	Backtracking_Record last_backtrack_record;

	if (bestentity == nullptr)return;

	auto entity = bestentity;

	
	best_tick = player_aimbot_info[entity->GetIndex()].tick;
	last_backtrack_record = player_aimbot_info[entity->GetIndex()].backtrack_record;
	last_extrapolated_ticks = player_aimbot_info[entity->GetIndex()].extrapolted_ticks;
	


	SDK::player_info_t ent_info; INTERFACES::Engine->GetPlayerInfo(entity->GetIndex(), &ent_info);


	BestEntityI = entity->GetIndex();

	if (player_aimbot_info[entity->GetIndex()].aim_angles == Vector(0, 0, 0))
		return;

	const bool is_exploiting = player_aimbot_info[entity->GetIndex()].backtrack_record.is_exploiting;
	if (is_exploiting || (!(entity->GetFlags() & FL_ONGROUND)))
	{
		FEATURES::MISC::InGameLogger::Log log;
		log.color_line.PushBack(("Shot at exploiting player!"), BLUE);
		FEATURES::MISC::in_game_logger.AddLog(log);
	}

	if (Config::Ragebot::RageGlobal::get().backtrack_enable && !resolver.IsSlowwalking(entity)) {
		backtracking.Restore(entity, last_backtrack_record, last_extrapolated_ticks);
		backtracking.ApplyRestore(entity, best_tick);
	}



	if (Config::Ragebot::RageGlobal::get().auto_scope && !local_player->GetIsScoped() && UTILS::IsWeaponSniper(weapon)) /// auto scope
		cmd->buttons |= IN_ATTACK2;
	else if(Config::Ragebot::RageGlobal::get().autoshoot)
		cmd->buttons |= IN_ATTACK;


	Vector Aimpunch = local_player->GetPunchAngles();
	if (cmd->buttons & IN_ATTACK) {
		if (Config::Ragebot::RageGlobal::get().silent) {
			player_aimbot_info[entity->GetIndex()].aim_angles -= Aimpunch * 2; cmd->viewangles = player_aimbot_info[entity->GetIndex()].aim_angles;
		}
		else {
			player_aimbot_info[entity->GetIndex()].aim_angles -= Aimpunch * 2; cmd->viewangles = player_aimbot_info[entity->GetIndex()].aim_angles;
		}
		if (Config::Ragebot::RageGlobal::get().backtrack_enable && !resolver.IsSlowwalking(entity))
			cmd->tick_count = best_tick;

		resolver.AddShotSnapshot(bestentity, resolver.GetPlayerResolveInfo(bestentity));
	}

	
}

float CAimbot::accepted_inaccuracy(SDK::CBaseWeapon* weapon)
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
void CAimbot::linear_extrapolations()
{

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	for (int i = 1; i < INTERFACES::Globals->maxclients; i++)
	{
		auto m_entity = (SDK::CBaseEntity*)INTERFACES::ClientEntityList->GetClientEntity(i);
		if (!m_entity || m_entity == local_player || m_entity->GetClientClass()->m_ClassID != 40 || m_entity->GetHealth() <= 0) continue;
		SDK::CBaseEntity* m_player = m_entity;

		float simtime_delta = m_entity->GetSimulationTime() - m_entity->GetOldSimulationTime();
		int choked_ticks = clamp(TIME_TO_TICKS(simtime_delta), 1, 15);
		static auto last_origin = m_player->GetVecOrigin();
		if (last_origin.Length() != m_entity->GetVecOrigin().Length())
			last_origin = m_entity->GetVecOrigin();

		float delta_distance = (m_entity->GetVecOrigin() - last_origin).LengthSqr();
		if (delta_distance > 4096.f)
		{
			Vector velocity_per_tick = m_entity->GetVelocity() * INTERFACES::Globals->interval_per_tick;
			auto new_origin = m_entity->GetVecOrigin() + (velocity_per_tick * choked_ticks);
			m_entity->SetAbsOrigin(new_origin);
		}
	}


}

std::vector<Vector> CAimbot::GetMultiplePointsForHitbox(SDK::CBaseEntity* local, SDK::CBaseEntity* entity, int iHitbox, VMatrix BoneMatrix[128])
{
	auto VectorTransform_Wrapper = [](const Vector& in1, const VMatrix &in2, Vector &out)
	{
		auto VectorTransform = [](const float *in1, const VMatrix& in2, float *out)
		{
			auto DotProducts = [](const float *v1, const float *v2)
			{
				return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
			};
			out[0] = DotProducts(in1, in2[0]) + in2[0][3];
			out[1] = DotProducts(in1, in2[1]) + in2[1][3];
			out[2] = DotProducts(in1, in2[2]) + in2[2][3];
		};
		VectorTransform(&in1.x, in2, &out.x);
	};

	SDK::studiohdr_t* pStudioModel = INTERFACES::ModelInfo->GetStudioModel(entity->GetModel());
	SDK::mstudiohitboxset_t* set = pStudioModel->pHitboxSet(0);
	SDK::mstudiobbox_t *hitbox = set->GetHitbox(iHitbox);

	std::vector<Vector> vecArray;

	Vector max;
	Vector min;
	VectorTransform_Wrapper(hitbox->bbmax, BoneMatrix[hitbox->bone], max);
	VectorTransform_Wrapper(hitbox->bbmin, BoneMatrix[hitbox->bone], min);

	auto center = (min + max) * 0.5f;

	Vector CurrentAngles = UTILS::CalcAngle(center, local->GetEyePosition());

	Vector Forward;
	MATH::AngleVectors(CurrentAngles, &Forward);

	Vector Right = Forward.Cross(Vector(0, 0, 1));
	Vector Left = Vector(-Right.x, -Right.y, Right.z);

	Vector Top = Vector(0, 0, 1);
	Vector Bot = Vector(0, 0, -1);

	if (iHitbox == SDK::HITBOX_HEAD) {
		for (auto i = 0; i < 4; ++i)
			vecArray.emplace_back(center);
		vecArray[1] += Top * (hitbox->radius * multipoint_head );
		vecArray[2] += Right * (hitbox->radius * multipoint_head);
		vecArray[3] += Left * (hitbox->radius * multipoint_head);
	}
	if (iHitbox == SDK::HITBOX_BODY || iHitbox == SDK::HITBOX_CHEST || iHitbox == SDK::HITBOX_PELVIS) {
		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);
		vecArray[1] += Right * (hitbox->radius * multipoint_body);
		vecArray[2] += Left * (hitbox->radius * multipoint_body);
	}
	if (iHitbox == SDK::HITBOX_NECK || iHitbox == SDK::HITBOX_UPPER_CHEST || iHitbox == SDK::HITBOX_THORAX) {
		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);
		vecArray[1] += Right * (hitbox->radius * multipoint_body);
		vecArray[2] += Left * (hitbox->radius * multipoint_body );
	}
	if (iHitbox == SDK::HITBOX_LEFT_FOOT || iHitbox == SDK::HITBOX_RIGHT_FOOT || iHitbox == SDK::HITBOX_LEFT_THIGH || iHitbox == SDK::HITBOX_RIGHT_THIGH || iHitbox == SDK::HITBOX_LEFT_CALF || iHitbox == SDK::HITBOX_RIGHT_CALF)
	{
		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);
		vecArray[1] += Right * (hitbox->radius * multipoint_legs);
		vecArray[2] += Left * (hitbox->radius * multipoint_legs);
	}
	if (iHitbox == SDK::HITBOX_LEFT_HAND || iHitbox == SDK::HITBOX_RIGHT_HAND || iHitbox == SDK::HITBOX_LEFT_FOREARM || iHitbox == SDK::HITBOX_RIGHT_FOREARM || iHitbox == SDK::HITBOX_RIGHT_UPPER_ARM || iHitbox == SDK::HITBOX_LEFT_UPPER_ARM)
	{
		for (auto i = 0; i < 3; ++i)
			vecArray.emplace_back(center);
		vecArray[1] += Right * (hitbox->radius * multipoint_arms);
		vecArray[2] += Left * (hitbox->radius * multipoint_arms);
	}

	return vecArray;
}

SDK::mstudiobbox_t* GetHitbox2(SDK::CBaseEntity* entity, int hitbox_index)
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
Vector CAimbot::get_hitbox_pos(SDK::CBaseEntity* entity, int hitbox_id)
{
	auto hitbox = GetHitbox2(entity, hitbox_id);
	if (!hitbox)
		return Vector(0, 0, 0);

	auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax, vCenter;
	MATH::VectorTransform(hitbox->bbmin, bone_matrix, bbmin);
	MATH::VectorTransform(hitbox->bbmax, bone_matrix, bbmax);

	vCenter = (bbmin + bbmax) * 0.5f;

	return vCenter;
}
Vector CAimbot::multipoint(SDK::CBaseEntity* entity, SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return Vector(0, 0, 0);

	Vector vector_best_point = Vector(0, 0, 0);
	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	int maxDamage;
	maxDamage = min_dmg;

	VMatrix matrix[128];
	if (!entity->SetupBones(matrix, 128, 256, 0)) return Vector(0, 0, 0);

	std::vector<int> HitBoxesToScan;
	// Get the hitboxes to scan
	if (GetAsyncKeyState(Config::Ragebot::RageGlobal::get().bodyaim)) {
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_BODY));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_CHEST));
		HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_PELVIS));
	}
	else if (prefer == 1 && (resolver.IsSlowwalking(entity) || global::missedshots[entity->GetIndex()] > 2 || entity->GetHealth() < 30)) {
			HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_BODY));
			HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_CHEST));
			HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_PELVIS));
	}
	else if (prefer == 2 && (entity->GetHealth() < 40 || resolver.IsSlowwalking(entity) || global::missedshots[entity->GetIndex()] > 1)) {
			HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_BODY));
			HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_CHEST));
			HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_PELVIS));
	}
	else if (prefer == 3 && (resolver.IsSlowwalking(entity) || global::missedshots[entity->GetIndex()] > 0)) {
			HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_BODY));
			HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_CHEST));
			HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_PELVIS));
	}
	else {

		if (hitboxes[0]) {
			HitBoxesToScan.push_back(static_cast<int>(SDK::HITBOX_HEAD));
		}
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
	}

	
	for (int i = 0; i < HitBoxesToScan.size(); i++)
	{
		for (auto point : GetMultiplePointsForHitbox(local_player, entity, HitBoxesToScan[i], matrix))
		{
			int damage = AutoWall->CanHit(point);
			
			if (damage > maxDamage)
			{
				bestHitbox = HitBoxesToScan[i];
				maxDamage = damage;
				vector_best_point = point;

				if (maxDamage >= entity->GetHealth())
					return vector_best_point;
			}
		}
	}

	HitBoxesToScan.clear();

	return vector_best_point;
}

bool CAimbot::can_shoot(SDK::CUserCmd* cmd)
{

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return false;
	if (local_player->GetHealth() <= 0) return false;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon || weapon->GetLoadedAmmo() == 0) return false;

	return (weapon->GetNextPrimaryAttack() < UTILS::GetCurtime()) && (local_player->GetNextAttack() < UTILS::GetCurtime());

}

void CAimbot::SelectTarget2()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	for (int index = 1; index <= 65; index++)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(index);
		if (!entity) continue;
		if (entity->GetTeam() == local_player->GetTeam()) continue;
		if (entity->GetHealth() <= 0) continue;
		if (entity->GetClientClass()->m_ClassID != 40) continue;
		if (entity->GetVecOrigin() == Vector(0, 0, 0)) continue;
		if (entity->GetImmunity()) continue;
		if (entity->GetIsDormant())	continue;
		AimbotData_t data = AimbotData_t(entity, index);
		Entities.push_back(data);
	}
}
void CAimbot::autoknife(SDK::CUserCmd *cmd) {
	for (int i = 1; i < 65; i++)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		if (!entity)
			continue;

		if (!local_player)
			continue;

		bool is_local_player = entity == local_player;
		bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

		if (is_local_player)
			continue;

		if (!entity->IsAlive())
			continue;

		if (is_teammate)
			continue;

		if (!local_player->IsAlive())
			continue;

		auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

		if (!weapon)
			continue;

		if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_BAYONET ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_FLIP ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_GUT ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_KARAMBIT ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_M9_BAYONET ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_TACTICAL ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_FALCHION ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_SURVIVAL_BOWIE ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_BUTTERFLY ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_PUSH ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_URSUS ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_GYPSY_JACKKNIFE ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_STILETTO ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_WIDOWMAKER ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_T ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE_CT ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFE ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_KNIFEGG)
		{
			if (can_shoot(cmd))
			{
				int bone = knife_hitbox(entity); //you can change this but keep in mind this has range stuff. it only has pelvis as a bone but why do other stuff really it will make it inaccurate shooting at arms and legs if they arent resolved right

				if (bone != 1)
				{
					Vector fucknigga = get_hitbox_pos(entity, bone);
					Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

					if (fucknigga != Vector(0, 0, 0))
					{
						SDK::trace_t trace;

						AutoWall->TraceLine(local_position, fucknigga, MASK_SOLID, local_player, &trace);

						SDK::player_info_t info;
						if(trace.m_pEnt == nullptr)							continue;

						if (!(INTERFACES::Engine->GetPlayerInfo(trace.m_pEnt->GetIndex(), &info)))
							continue;

						if (fucknigga != Vector(0, 0, 0))
						{
							cmd->viewangles = MATH::NormalizeAngle(UTILS::CalcAngle(local_position, fucknigga));
							global::should_send_packet = true;
							Vector point2 = get_hitbox_pos(entity, SDK::HitboxList::HITBOX_PELVIS);
							float distance = fabs((point2 - local_position).Length());
							float primary_attack = 80.f; // left click
							float secondary_attack = 50.f; // right click
							if (entity->GetArmor() > 0 && entity->HasHelmet())
							{
								if (entity->GetHealth() <= 55)
								{
									cmd->buttons |= IN_ATTACK2;
								}
								else
								{
									cmd->buttons |= IN_ATTACK;
								}
							}
							else
							{
								if (entity->GetHealth() <= 65)
								{
									cmd->buttons |= IN_ATTACK2;
								}
								else
								{
									cmd->buttons |= IN_ATTACK;
								}
							}
						}
					}
				}
			}
			continue;
		}

	}
}


int CAimbot::knife_hitbox(SDK::CBaseEntity* entity)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	float primary_attack = 80.f; // left click
	float secondary_attack = 50.f; // right click

	bestHitbox = -1;

	Vector point = get_hitbox_pos(entity, SDK::HitboxList::HITBOX_PELVIS);

	if (point != Vector(0, 0, 0))
	{
		float distance = fabs((point - local_position).Length());

		if (entity->GetArmor() > 0 && entity->HasHelmet())
		{
			if (entity->GetHealth() <= 55 && (distance <= secondary_attack))
			{
				bestHitbox = SDK::HitboxList::HITBOX_PELVIS;
				secondary_attack = distance;
			}
			else if (distance <= primary_attack)
			{
				bestHitbox = SDK::HitboxList::HITBOX_PELVIS;
				primary_attack = distance;
			}
		}
		else
		{
			if (entity->GetHealth() <= 65 && (distance <= secondary_attack))
			{
				bestHitbox = SDK::HitboxList::HITBOX_PELVIS;
				secondary_attack = distance;
			}
			else if (distance <= primary_attack)
			{
				bestHitbox = SDK::HitboxList::HITBOX_PELVIS;
				primary_attack = distance;
			}
		}

	}

	return bestHitbox;
}

int CAimbot::zeus_hitbox(SDK::CBaseEntity* entity)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	float closest = 160.f;

	bestHitbox = -1;

	Vector point = get_hitbox_pos(entity, SDK::HitboxList::HITBOX_BODY);

	if (point != Vector(0, 0, 0))
	{
		if ((entity->GetVecOrigin() - local_player->GetVecOrigin()).Length() < weapon->get_full_info()->m_Range)
		{
			bestHitbox = SDK::HitboxList::HITBOX_BODY;
		}
	}

	return bestHitbox;
}

void CAimbot::autozeus(SDK::CUserCmd *cmd) {
	for (int i = 1; i < 65; i++)
	{
		auto entity = INTERFACES::ClientEntityList->GetClientEntity(i);
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		if (!entity)
			continue;

		if (!local_player)
			continue;

		bool is_local_player = entity == local_player;
		bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

		if (is_local_player)
			continue;

		if (!entity->IsAlive())
			continue;

		if (is_teammate)
			continue;

		if (!local_player->IsAlive())
			continue;

		auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

		if (!weapon)
			continue;

		if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_TASER) //if we have a taser men!1!!1 
		{
			if (can_shoot(cmd))
			{
				int bone = zeus_hitbox(entity); //you can change this but keep in mind this has range stuff. it only has pelvis as a bone but why do other stuff really it will make it inaccurate shooting at arms and legs if they arent resolved right 

				if (bone != 1)
				{
					Vector hitboxp = get_hitbox_pos(entity, bone);
					Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

					if (hitboxp != Vector(0, 0, 0))
					{
						SDK::trace_t trace;

						AutoWall->TraceLine(local_position, hitboxp, MASK_SOLID, local_player, &trace);

						SDK::player_info_t info;

						if (trace.m_pEnt == nullptr) return;

						if (!(INTERFACES::Engine->GetPlayerInfo(trace.m_pEnt->GetIndex(), &info)))
							continue;

						if (hitboxp != Vector(0, 0, 0))
						{
							cmd->viewangles = MATH::NormalizeAngle(UTILS::CalcAngle(local_position, hitboxp));
							global::should_send_packet = true;
							cmd->buttons |= IN_ATTACK;
						}
					}
				}
			}
			continue;
		}

	}
}

CAimbot* aimbot = new CAimbot();