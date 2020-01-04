#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/AntiAim.h"
#include "../FEATURES/AutoWall.h"
#include "../UTILS/render.h"
#include "../SDK/NetChannel.h"
#include "../SDK/IClient.h"
#include "../SDK/CClinetstate.h"
#include <intrin.h>


#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
bool r = false;
bool l = false;
bool b = false;
int choose_yaw;
int choose_pitch;

static bool desync_switch;
static bool hold;
static bool omg;
float randnum(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}
inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}

bool next_lby_update(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return false;

	static float next_lby_update_time = 0;
	float curtime = INTERFACES::Globals->curtime;
	local_update = next_lby_update_time;

	auto animstate = local_player->GetAnimState();

	if (!animstate)
		return false;

	if (!(local_player->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1f)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}

float fov_player(Vector ViewOffSet, Vector View, SDK::CBaseEntity* entity, int hitbox)
{
	const float MaxDegrees = 180.0f;
	Vector Angles = View, Origin = ViewOffSet;
	Vector Delta(0, 0, 0), Forward(0, 0, 0);
	Vector AimPos = aimbot->get_hitbox_pos(entity, hitbox);

	MATH::AngleVectors(Angles, &Forward);
	MATH::VectorSubtract(AimPos, Origin, Delta);
	MATH::NormalizeNum(Delta, Delta);

	float DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / M_PI));
}

int closest_to_crosshair()
{
	int index = -1;
	float lowest_fov = INT_MAX;

	SDK::CBaseEntity* local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	Vector angles;
	INTERFACES::Engine->GetViewAngles(angles);

	for (int i = 1; i <= INTERFACES::Globals->maxclients; i++)
	{
		SDK::CBaseEntity *entity = INTERFACES::ClientEntityList->GetClientEntity(i);

		if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || entity->GetIsDormant() || entity == local_player)
			continue;

		float fov = fov_player(local_position, angles, entity, 0);

		if (fov < lowest_fov)
		{
			lowest_fov = fov;
			index = i;
		}
	}

	return index;
}
void AngleVectorsp(const Vector angles, Vector& forward, Vector& right, Vector& up)
{
	float angle;
	static float sp, sy, cp, cy;

	angle = angles[0] * (M_PI / 180.f);
	sp = sin(angle);
	cp = cos(angle);

	angle = angles[1] * (M_PI / 180.f);
	sy = sin(angle);
	cy = cos(angle);


	forward[0] = cp * cy;
	forward[1] = cp * sy;
	forward[2] = -sp;


	static float sr, cr;

	angle = angles[2] * (M_PI / 180.f);
	sr = sin(angle);
	cr = cos(angle);


	right[0] = -1 * sr * sp * cy + -1 * cr * -sy;
	right[1] = -1 * sr * sp * sy + -1 * cr *cy;
	right[2] = -1 * sr * cp;

	up[0] = cr * sp *cy + -sr * -sy;
	up[1] = cr * sp *sy + -sr * cy;
	up[2] = cr * cp;


}


float freestanding(SDK::CUserCmd* cmd) {
	float Back, Right, Left;

	Vector src3D, dst3D, forward, right, up, src, dst;
	SDK::trace_t tr;
	SDK::Ray_t ray, ray2, ray3, ray4, ray5;
	SDK::CTraceFilter filter;

	Vector viewangles;
	INTERFACES::Engine->GetViewAngles(viewangles);

	Vector view;
	INTERFACES::Engine->GetViewAngles(view);

	AngleVectorsp(viewangles, forward, right, up);
	int index = closest_to_crosshair();
	auto entity = INTERFACES::ClientEntityList->GetClientEntity(index);
	auto local = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	filter.pSkip1 = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	src3D = local->GetEyePosition();
	dst3D = src3D + (forward * 384);

	ray.Init(src3D, dst3D);

	INTERFACES::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	Back = (tr.end - tr.start).Length();

	ray2.Init(src3D + right * 35, dst3D + right * 35);

	INTERFACES::Trace->TraceRay(ray2, MASK_SHOT, &filter, &tr);

	Right = (tr.end - tr.start).Length();

	ray3.Init(src3D - right * 35, dst3D - right * 35);

	INTERFACES::Trace->TraceRay(ray3, MASK_SHOT, &filter, &tr);

	Left = (tr.end - tr.start).Length();

	if (Back < Right && Back < Left && Left == Right && !(entity == nullptr)) {
		cmd->viewangles.y = cmd->viewangles.y;
		global::freestandingsite = 0;
	}
	if (Back > Right && Back > Left) {
		return 180; //if back is the best angle
		global::freestandingsite = 1;
	}
	else if (Right > Left && Right > Back) {
		return 90; //if right is the best angle
		global::freestandingsite = 2;
	}
	else if (Right > Left && Right == Back) {
		return 135;
		global::freestandingsite = 3;
	}
	else if (Left > Right && Left > Back) {
		return -90; //if left is the best angle
		global::freestandingsite = 4;
	}
	else if (Left > Right && Left == Back) {
		return -135; //if left is equal to back
		global::freestandingsite = 5;
	}
	else {
		return 180;
		global::freestandingsite = 6;
	}
}
bool flip;
int last_set_tick;
float CAntiAim::antiaim_yaw_real(SDK::CUserCmd* cmd) {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (last_set_tick <= local_player->GetTickBase()) {
		if (flip == true)
			flip = false;
		else
			flip = true;

		last_set_tick = local_player->GetTickBase() + Config::AntiAim::AA::get().jitterspeed;
	}
	if (l == true) {
		global::menualrlb = 2;
		return -90;
	}
	else if (r == true) {
		global::menualrlb = 1;
		return 90;
	}
	else if (b == true) {
		global::menualrlb = 0;
		return 179.f;
	}
	else {
		global::menualrlb = 3;
		switch (Config::AntiAim::AA::get().yaw) {
		case 1: {
			if (flip == false)
				return 179.f;
			else
				return 0;
		}break;
		case 2: {
			if (flip == false)
				return 179.f + Config::AntiAim::AA::get().jitterwidth;
			else
				return 179.f - Config::AntiAim::AA::get().jitterwidth;
		}break;
		case 3: {
			if (flip == false)
				return Config::AntiAim::AA::get().jitterangle + Config::AntiAim::AA::get().jitterwidth;
			else
				return Config::AntiAim::AA::get().jitterangle - Config::AntiAim::AA::get().jitterwidth;
		}break;
		case 4: return freestanding(cmd); break;

		}

	}
	return 0.f;

}
void CAntiAim::antiaim_pitch_real(int choose, SDK::CUserCmd* cmd) {

	switch (choose) {
	case 1: {
		cmd->viewangles.x = 89.f;

	}break;
	case 2: {
		cmd->viewangles.x = -89.f;
	}break;
	case 3: {
		cmd->viewangles.x = 0.f + RandomFloat(-89.f, 89.f);
	}break;
	case 4: {
		cmd->viewangles.x = 90.f;
	}break;
	}

}
bool CAntiAim::ShallReturn(SDK::CUserCmd* cmd) {

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon) return true;

	if (cmd->buttons & IN_USE || local_player->GetMoveType() == SDK::MOVETYPE_LADDER) return true;
	if (cmd->buttons & IN_ATTACK && aimbot->can_shoot(cmd)) return true;
	if (weapon->is_knife() && cmd->buttons & IN_ATTACK2) return true;
	if (weapon->get_full_info()->m_WeaponType == 9) return true;
	if (weapon->is_grenade()) return true;

	return false;
}
float getmaxdesync() {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto animstate = uintptr_t(local_player->GetAnimState());

	float duckammount = *(float *)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0)
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

	unk3 = *(float *)(animstate + 0x334) * unk2;

	return unk3;
}
bool canDesync(SDK::CUserCmd* cmd, bool sedpac) {
	switch (Config::AntiAim::AADesync::get().desync) {
	case 0: {
		return false;
	}
	case 1: {
		if (!sedpac)
			return true;
	}break;
	case 2: {
		if (cmd->command_number % 3 && !sedpac)
			return true;
	}break;
	case 3: {
		if (!sedpac && next_lby_update(cmd)) {
			return true;
		}
	}break;
	}
	return false;
}

bool jit;
static float LBY = 0;
void CAntiAim::do_antiaim(SDK::CUserCmd* cmd) {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	if (local_player->GetHealth() <= 0) return;
	if(ShallReturn(cmd))  return;
	int real = 0;

	if (GetAsyncKeyState(Config::AntiAim::AADesync::get().desync_flip))
		desync_switch = !desync_switch;
	global::desync = desync_switch;

	if (GetAsyncKeyState(Config::AntiAim::AAMisc::get().menual_right)) {
		l = !l;
		r = false;
		b = false;
	}
	else if (GetAsyncKeyState(Config::AntiAim::AAMisc::get().menual_left)) {
		l = false;
		r = !r;
		b = false;

	}
	else if (GetAsyncKeyState(Config::AntiAim::AAMisc::get().menual_back)) {
		r = false;
		l = false;
		b = !b;
	}

	float yaw = 0;

	Vector view;
	INTERFACES::Engine->GetViewAngles(view);

	int angle = Config::AntiAim::AADesync::get().angle;

	antiaim_pitch_real(Config::AntiAim::AA::get().pitch, cmd);


		float Desync = local_player->MaxDelta(local_player) / 100 * angle + 30.0f; float MM = 1.10f;
		if (cmd->tick_count % 2) MM = -MM;

		if (INTERFACES::Globals->curtime < LBY && global::originalCMD.move.x < 4 && global::originalCMD.move.x > -4 && local_player->GetFlags() & FL_ONGROUND) global::originalCMD.move.x = MM;

		if (canDesync(cmd, global::should_send_packet) && local_player->GetFlags() & FL_ONGROUND) {
			if (INTERFACES::Globals->curtime >= LBY && local_player->GetVelocity().Length() < 4)
			{
				LBY = INTERFACES::Globals->curtime + 0.22f;
			}
			else {
				if (desync_switch)
					cmd->viewangles.y += antiaim_yaw_real(cmd) + Desync;
				else
					cmd->viewangles.y += antiaim_yaw_real(cmd) - Desync;
				global::fake_angles.y = desync_switch ? Desync : -Desync;
			}

		}
		else {
			yaw = antiaim_yaw_real(cmd);

		}

		MATH::NormalizeYaw(yaw);
	auto entity = INTERFACES::ClientEntityList->GetClientEntity(closest_to_crosshair());
	if (Config::AntiAim::AA::get().yawbase == 0) {
		cmd->viewangles.y += yaw;
		global::real_angles.y = view.y + antiaim_yaw_real(cmd);
	}
	else if (Config::AntiAim::AA::get().yawbase == 1) {
		if (entity != nullptr) {
			cmd->viewangles.y = UTILS::CalcAngle(local_player->GetEyePosition(), entity->GetEyePosition()).y + yaw;
			global::real_angles.y = UTILS::CalcAngle(local_player->GetEyePosition(), entity->GetEyePosition()).y + antiaim_yaw_real(cmd);
		}
		else {
			cmd->viewangles.y += yaw;
			global::real_angles.y = view.y + antiaim_yaw_real(cmd);
		}
	}
	
	
	auto nci = INTERFACES::Engine->GetNetChannel();

	if (GetAsyncKeyState(VK_F2)) {
		static bool shifting = false;

		if (!shifting) {
			// send another packet after shifting tickbase
			shifting = true;
			global::should_send_packet = true;
		}
		else {
			// fake angles, place angles manipulation here
			cmd->viewangles.y += MATH::NormalizeAngle(real + (desync_switch ? angle: -angle) * 2);
			global::fake_angles.y = desync_switch ? angle : -angle;

			if (INTERFACES::clientstate->choked_commands) {
				// suppress command, so it wont be simulated
				cmd->tick_count = INT_MAX;
			}

			// tickbase shift amount
			if (INTERFACES::clientstate->choked_commands < 6) {
				global::should_send_packet = false;
			}
			else {
				global::should_send_packet = true;
				shifting = false;
			}
			return;
		}
	}

}




void CAntiAim::fix_movement(SDK::CUserCmd* cmd)
{

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (SETTINGS::settings.aa_pitch > 1 || SETTINGS::settings.aa_pitch > 1 || SETTINGS::settings.aa_pitch > 1)
	{
		cmd->move.x = clamp(cmd->move.x, -450.f, 450.f);
		cmd->move.y = clamp(cmd->move.y, -450.f, 450.f);
		cmd->move.z = clamp(cmd->move.z, -320.f, 320.f);

		cmd->viewangles.x = clamp(cmd->viewangles.x, -89.f, 89.f);
	}

	Vector real_viewangles;
	INTERFACES::Engine->GetViewAngles(real_viewangles);

	Vector vecMove(cmd->move.x, cmd->move.y, cmd->move.z);
	float speed = sqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);

	Vector angMove;
	MATH::VectorAngles(vecMove, angMove);

	float yaw = DEG2RAD(cmd->viewangles.y - real_viewangles.y + angMove.y);

	cmd->move.x = cos(yaw) * speed;
	cmd->move.y = sin(yaw) * speed;

	cmd->viewangles = MATH::NormalizeAngle(cmd->viewangles);

	if (cmd->viewangles.x < -89.f || cmd->viewangles.x > 89.f) cmd->move.x *= -1;
}

Vector CAntiAim::fix_movement(SDK::CUserCmd* cmd, SDK::CUserCmd originalCMD)
{

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return Vector(0, 0, 0);

	Vector wish_forward, wish_right, wish_up, cmd_forward, cmd_right, cmd_up;

	auto viewangles = cmd->viewangles;
	auto movedata = originalCMD.move;
	viewangles.Normalize();

	if (!(local_player->GetFlags() & FL_ONGROUND) && viewangles.z != 0.f)
		movedata.y = 0.f;

	MATH::AngleVectors2(originalCMD.viewangles, &wish_forward, &wish_right, &wish_up);
	MATH::AngleVectors2(viewangles, &cmd_forward, &cmd_right, &cmd_up);

	auto v8 = sqrt(wish_forward.x * wish_forward.x + wish_forward.y * wish_forward.y), v10 = sqrt(wish_right.x * wish_right.x + wish_right.y * wish_right.y), v12 = sqrt(wish_up.z * wish_up.z);

	Vector wish_forward_norm(1.0f / v8 * wish_forward.x, 1.0f / v8 * wish_forward.y, 0.f),
		wish_right_norm(1.0f / v10 * wish_right.x, 1.0f / v10 * wish_right.y, 0.f),
		wish_up_norm(0.f, 0.f, 1.0f / v12 * wish_up.z);

	auto v14 = sqrt(cmd_forward.x * cmd_forward.x + cmd_forward.y * cmd_forward.y), v16 = sqrt(cmd_right.x * cmd_right.x + cmd_right.y * cmd_right.y), v18 = sqrt(cmd_up.z * cmd_up.z);

	Vector cmd_forward_norm(1.0f / v14 * cmd_forward.x, 1.0f / v14 * cmd_forward.y, 1.0f / v14 * 0.0f),
		cmd_right_norm(1.0f / v16 * cmd_right.x, 1.0f / v16 * cmd_right.y, 1.0f / v16 * 0.0f),
		cmd_up_norm(0.f, 0.f, 1.0f / v18 * cmd_up.z);

	auto v22 = wish_forward_norm.x * movedata.x, v26 = wish_forward_norm.y * movedata.x, v28 = wish_forward_norm.z * movedata.x, v24 = wish_right_norm.x * movedata.y, v23 = wish_right_norm.y * movedata.y, v25 = wish_right_norm.z * movedata.y, v30 = wish_up_norm.x * movedata.z, v27 = wish_up_norm.z * movedata.z, v29 = wish_up_norm.y * movedata.z;

	Vector correct_movement;
	correct_movement.x = cmd_forward_norm.x * v24 + cmd_forward_norm.y * v23 + cmd_forward_norm.z * v25
		+ (cmd_forward_norm.x * v22 + cmd_forward_norm.y * v26 + cmd_forward_norm.z * v28)
		+ (cmd_forward_norm.y * v30 + cmd_forward_norm.x * v29 + cmd_forward_norm.z * v27);
	correct_movement.y = cmd_right_norm.x * v24 + cmd_right_norm.y * v23 + cmd_right_norm.z * v25
		+ (cmd_right_norm.x * v22 + cmd_right_norm.y * v26 + cmd_right_norm.z * v28)
		+ (cmd_right_norm.x * v29 + cmd_right_norm.y * v30 + cmd_right_norm.z * v27);
	correct_movement.z = cmd_up_norm.x * v23 + cmd_up_norm.y * v24 + cmd_up_norm.z * v25
		+ (cmd_up_norm.x * v26 + cmd_up_norm.y * v22 + cmd_up_norm.z * v28)
		+ (cmd_up_norm.x * v30 + cmd_up_norm.y * v29 + cmd_up_norm.z * v27);

	correct_movement.x = clamp(correct_movement.x, -450.f, 450.f);
	correct_movement.y = clamp(correct_movement.y, -450.f, 450.f);
	correct_movement.z = clamp(correct_movement.z, -320.f, 320.f);

	return correct_movement;
}

CAntiAim* antiaim = new CAntiAim();