#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ConVar.h"
#include "../FEATURES/AutoWall.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Movement.h"

#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
#define CheckIfNonValidNumber(x) (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || fpclassify(x) == FP_SUBNORMAL)

void CMovement::bunnyhop(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	if (cmd->buttons & IN_JUMP)
	{
		int local_flags = local_player->GetFlags();
		if (!(local_flags & FL_ONGROUND))
			cmd->buttons &= ~IN_JUMP;
	}
}


bool get_closest_plane(Vector* plane)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return false;

	SDK::trace_t trace; SDK::CTraceWorldOnly filter; SDK::Ray_t ray;

	auto start = local_player->GetVecOrigin(), mins = local_player->GetVecMins(), maxs = local_player->GetVecMaxs();

	Vector planes;
	auto count = 0;

	for (auto step = 0.f; step <= M_PI * 2.f; step += M_PI / 10.f)
	{
		auto end = start;

		end.x += cos(step) * 64.f;
		end.y += sin(step) * 64.f;

		ray.Init(start, end, mins, maxs);
		INTERFACES::Trace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &trace);

		if (trace.flFraction < 1.f)
		{
			planes += trace.plane.normal;
			count++;
		}
	}

	planes /= count;

	if (planes.z < 0.1f) { *plane = planes; return true; }
	return false;
}
float get_move_angle(float speed)
{
	auto move_angle = RAD2DEG(asin(15.f / speed));

	if (!isfinite(move_angle) || move_angle > 90.f)
		move_angle = 90.f;
	else if (move_angle < 0.f)
		move_angle = 0.f;

	return move_angle;
}
bool will_hit_obstacle_in_future(float predict_time, float step)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return false;

	static auto sv_gravity = INTERFACES::cvar->FindVar("sv_gravity");
	static auto sv_jump_impulse = INTERFACES::cvar->FindVar("sv_jump_impulse");

	bool ground = local_player->GetFlags() & FL_ONGROUND;
	auto gravity_per_tick = sv_gravity->GetFloat() * INTERFACES::Globals->interval_per_tick;

	auto start = local_player->GetVecOrigin(), end = start;
	auto velocity = local_player->GetVelocity();

	auto min = local_player->GetVecMins(), maxs = local_player->GetVecMaxs();

	SDK::trace_t trace;
	SDK::CTraceWorldOnly filter;
	SDK::Ray_t ray;

	auto predicted_ticks_needed = TIME_TO_TICKS(predict_time);
	auto velocity_rotation_angle = RAD2DEG(atan2(velocity.y, velocity.x));
	auto ticks_done = 0;

	if (predicted_ticks_needed <= 0)
		return false;

	while (true)
	{
		auto rotation_angle = velocity_rotation_angle + step;

		velocity.x = cos(DEG2RAD(rotation_angle)) * velocity.Length2D();
		velocity.y = sin(DEG2RAD(rotation_angle)) * velocity.Length2D();
		velocity.z = ground ? sv_jump_impulse->GetFloat() : velocity.z - gravity_per_tick;

		end += velocity * INTERFACES::Globals->interval_per_tick;

		ray.Init(start, end, min, maxs);
		INTERFACES::Trace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &trace);

		if (trace.flFraction != 1.f && trace.plane.normal.z <= 0.9f || trace.startSolid || trace.allsolid)
			break;

		end = trace.end;
		end.z -= 2.f;

		ray.Init(trace.end, end, min, maxs);
		INTERFACES::Trace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &trace);

		ground = (trace.flFraction < 1.f || trace.allsolid || trace.startSolid) && trace.plane.normal.z >= 0.7f;

		if (++ticks_done >= predicted_ticks_needed)
			return false;

		velocity_rotation_angle = rotation_angle;
	}
	return true;
}

void CMovement::circle_strafe(SDK::CUserCmd* cmd, float* circle_yaw)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	const auto min_step = 2.25f;
	const auto max_step = 5.f;

	auto velocity_2d = local_player->GetVelocity().Length2D();
	auto velocity = local_player->GetVelocity();
	velocity.z = 0.f;

	auto circlemin = SETTINGS::settings.circlemin / 10;
	auto circlemax = SETTINGS::settings.circlemax / 10;
	auto circlevel = SETTINGS::settings.circlevel;
	auto circlemulti = SETTINGS::settings.circlenormalizemultiplier / 1000;

	auto ideal_strafe = clamp(get_move_angle(velocity_2d) * 2.f, min_step, max_step);
	auto predict_time = clamp(295.f / velocity_2d, 0.25f, 1.15f);
	//auto predict_time = clamp(circlevel / velocity_2d, circlemin, circlemax);

	auto step = ideal_strafe;

	while (true)
	{
		if (!will_hit_obstacle_in_future(predict_time, step) || step > max_step)
			break;

		step += 0.2f;
	}

	if (step > max_step)
	{
		step = ideal_strafe;

		while (true)
		{
			if (!will_hit_obstacle_in_future(predict_time, step) || step < -min_step)
				break;

			step -= 0.2f;
		}

		if (step < -min_step)
		{
			Vector plane;
			if (get_closest_plane(&plane))
				//step = -MATH::NormalizeAngle(*circle_yaw - RAD2DEG(atan2(plane.y, plane.x))) * circlemulti;
				step = -MATH::NormalizeAngle(*circle_yaw - RAD2DEG(atan2(plane.y, plane.x))) * 0.03f;
		}
		else
			step -= 0.2f;
	}
	else
		step += 0.2f;

	global::originalCMD.viewangles.y = *circle_yaw = MATH::NormalizeAngle(*circle_yaw + step);
	global::originalCMD.move.y = copysign(450.f, -step);
}
bool is_bhopping;
float calculated_direction;
bool in_transition;
float true_direction;
float wish_direction;
float step;
float rough_direction;
void CMovement::autostrafer(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;


	enum directions {
		FORWARDS = 0,
		BACKWARDS = 180,
		LEFT = 90,
		RIGHT = -90
	};

	// Reset direction when player is not strafing
	is_bhopping = cmd->buttons & IN_JUMP;
	if (!is_bhopping && local_player->GetFlags() & FL_ONGROUND) {
		calculated_direction = directions::FORWARDS;
		in_transition = false;
		return;
	}

	// Get true view angles
	Vector base{ };
	INTERFACES::Engine->GetViewAngles(base);

	// Calculate the rough direction closest to the player's true direction
	auto get_rough_direction = [&](float true_direction) -> float {
		// Make array with our four rough directions
		std::array< float, 4 > minimum = { directions::FORWARDS, directions::BACKWARDS, directions::LEFT, directions::RIGHT };
		float best_angle, best_delta = 181.f;

		// Loop through our rough directions and find which one is closest to our true direction
		for (size_t i = 0; i < minimum.size(); ++i) {
			float rough_direction = base.y + minimum.at(i);
			float delta = fabsf(MATH::NormalizeYaw(true_direction - rough_direction));

			// Only the smallest delta wins out
			if (delta < best_delta) {
				best_angle = rough_direction;
				best_delta = delta;
			}
		}

		return best_angle;
	};

	// Get true direction based on player velocity
	true_direction = local_player->GetVelocity().Angle2().y;

	// Detect wish direction based on movement keypresses
	if (cmd->buttons & IN_FORWARD) {
		wish_direction = base.y + directions::FORWARDS;
	}
	else if (cmd->buttons & IN_BACK) {
		wish_direction = base.y + directions::BACKWARDS;
	}
	else if (cmd->buttons & IN_MOVELEFT) {
		wish_direction = base.y + directions::LEFT;
	}
	else if (cmd->buttons & IN_MOVERIGHT) {
		wish_direction = base.y + directions::RIGHT;
	}
	else {
		// Reset to forward when not pressing a movement key, then fix anti-aim strafing by setting IN_FORWARD
		cmd->buttons |= IN_FORWARD;
		wish_direction = base.y + directions::FORWARDS;
	}
	// Calculate the ideal strafe rotation based on player speed (c) navewindre
	float speed_rotation = min(RAD2DEG(std::asinf(30.f / local_player->GetVelocity().Length2D())) * 0.5f, 45.f);
	if (in_transition) {
		// Get value to rotate by via calculated speed rotation
		float ideal_step = speed_rotation + calculated_direction;
		step = fabsf(MATH::NormalizeYaw(calculated_direction - ideal_step)); // 15.f is a good alternative, but here is where you would do your "speed" slider value for the autostrafer

		// Check when the calculated direction arrives close to the wish direction
		if (fabsf(MATH::NormalizeYaw(wish_direction - calculated_direction)) > step) {
			float add = MATH::NormalizeYaw(calculated_direction + step);
			float sub = MATH::NormalizeYaw(calculated_direction - step);

			// Step in direction that gets us closer to our wish direction
			if (fabsf(MATH::NormalizeYaw(wish_direction - add)) >= fabsf(MATH::NormalizeYaw(wish_direction - sub))) {
				calculated_direction -= step;
			}
			else {
				calculated_direction += step;
			}
		}
		else {
			// Stop transitioning when we meet our wish direction
			in_transition = false;
		}
	}
	else {
		// Get rough direction and setup calculated direction only when not transitioning
		rough_direction = get_rough_direction(true_direction);
		calculated_direction = rough_direction;

		// When we have a difference between our current (rough) direction and our wish direction, then transition
		if (rough_direction != wish_direction) {
			in_transition = true;
		}
	}

	// Set movement up to be rotated
	global::originalCMD.move.x = 0.f;
	global::originalCMD.move.y = cmd->command_number % 2 ? 450.f : -450.f;

	// Calculate ideal rotation based on our newly calculated direction
	float direction = (cmd->command_number % 2 ? speed_rotation : -speed_rotation) + calculated_direction;

	// Rotate our direction based on our new, defininite direction
	float rotation =DEG2RAD(base.y - direction);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float forwardmove = (cos_rot * global::originalCMD.move.x) - (sin_rot * global::originalCMD.move.y);
	float sidemove = (sin_rot * global::originalCMD.move.x) + (cos_rot * global::originalCMD.move.y);

	// Apply newly rotated movement
	global::originalCMD.move.x = forwardmove;
	global::originalCMD.move.y = sidemove;
}
void CMovement::duckinair(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;

	int local_flags = local_player->GetFlags();
	if (!(local_flags & FL_ONGROUND))
		cmd->buttons |= IN_DUCK;
}

CMovement* movement = new CMovement();