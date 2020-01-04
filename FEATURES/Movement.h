#pragma once
#include "../XorStr.hpp"
namespace SDK
{
	class CUserCmd;
	class CBaseWeapon;
}
const double M_U_DEG = 360.0 / 65536;
const double M_U_RAD = M_PI / 32768;
#define PI_2 M_PI/2
#define PI_4 M_PI/4
class AutoStrafer
{
	void VectorAngles3(const Vector& forward, QAngle &angles)
	{
		float	tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}
	double anglemod_deg(double a)
	{
		return M_U_DEG * ((int)(a / M_U_DEG) & 0xffff);
	}

	double anglemod_rad(double a)
	{
		return M_U_RAD * ((int)(a / M_U_RAD) & 0xffff);
	}

	double point2line_distsq(const double pos[2],
		const double line_origin[2],
		const double line_dir[2])
	{
		double tmp[2] = { line_origin[0] - pos[0], line_origin[1] - pos[1] };
		double dotprod = line_dir[0] * tmp[0] + line_dir[1] * tmp[1];
		tmp[0] -= line_dir[0] * dotprod;
		tmp[1] -= line_dir[1] * dotprod;
		return tmp[0] * tmp[0] + tmp[1] * tmp[1];
	}

	double strafe_theta_opt(double speed, double L, double tauMA)
	{
		double tmp = L - tauMA;
		if (tmp <= 0)
			return PI_2;
		if (tmp < speed)
			return std::acos(tmp / speed);
		return 0;
	}

	double strafe_theta_const(double speed, double nofric_speed, double L,
		double tauMA)
	{
		double sqdiff = nofric_speed * nofric_speed - speed * speed;
		double tmp = sqdiff / tauMA;
		if (tmp + tauMA < 2 * L && 2 * speed >= std::fabs(tmp - tauMA))
			return std::acos((tmp - tauMA) / (2 * speed));
		tmp = std::sqrt(L * L - sqdiff);
		if (tauMA - L > tmp && speed >= tmp)
			return std::acos(-tmp / speed);
		return strafe_theta_opt(speed, L, tauMA);
	}

	void strafe_fme_vec(double vel[2], const double avec[2], double L,
		double tauMA)
	{
		double tmp = L - vel[0] * avec[0] - vel[1] * avec[1];
		if (tmp < 0)
			return;
		if (tauMA < tmp)
			tmp = tauMA;
		vel[0] += avec[0] * tmp;
		vel[1] += avec[1] * tmp;
	}

	void strafe_fric(double vel[2], double E, double ktau)
	{
		double speed = std::hypot(vel[0], vel[1]);
		if (speed >= E)
		{
			vel[0] *= 1 - ktau;
			vel[1] *= 1 - ktau;
			return;
		}

		double tmp = E * ktau;
		if (speed > tmp)
		{
			tmp /= speed;
			vel[0] -= tmp * vel[0];
			vel[1] -= tmp * vel[1];
			return;
		}

		vel[0] = 0;
		vel[1] = 0;
	}

	double strafe_fric_spd(double spd, double E, double ktau)
	{
		if (spd >= E)
			return spd * (1 - ktau);
		double tmp = E * ktau;
		if (spd > tmp)
			return spd - tmp;
		return 0;
	}

	void strafe_side(double& yaw, int& Sdir, int& Fdir, double vel[2],
		double theta, double L, double tauMA, int dir)
	{
		double phi;
		// This is to reduce the overall shaking.
		if (theta >= PI_2 * 0.75)
		{
			Sdir = dir;
			Fdir = 0;
			phi = std::copysign(PI_2, dir);
		}
		else if (PI_2 * 0.25 <= theta && theta <= PI_2 * 0.75)
		{
			Sdir = dir;
			Fdir = 1;
			phi = std::copysign(PI_4, dir);
		}
		else
		{
			Sdir = 0;
			Fdir = 1;
			phi = 0;
		}

		if (std::fabs(vel[0]) > 0.1 || std::fabs(vel[1]) > 0.1)
			yaw = std::atan2(vel[1], vel[0]);
		yaw += phi - std::copysign(theta, dir);
		double yawcand[2] = {
			anglemod_rad(yaw), anglemod_rad(yaw + std::copysign(M_U_RAD, yaw))
		};
		double avec[2] = { std::cos(yawcand[0] - phi), std::sin(yawcand[0] - phi) };
		double tmpvel[2] = { vel[0], vel[1] };
		strafe_fme_vec(vel, avec, L, tauMA);
		avec[0] = std::cos(yawcand[1] - phi);
		avec[1] = std::sin(yawcand[1] - phi);
		strafe_fme_vec(tmpvel, avec, L, tauMA);

		if (tmpvel[0] * tmpvel[0] + tmpvel[1] * tmpvel[1] >
			vel[0] * vel[0] + vel[1] * vel[1])
		{
			vel[0] = tmpvel[0];
			vel[1] = tmpvel[1];
			yaw = yawcand[1];
		}
		else
			yaw = yawcand[0];
	}

public:
	void strafe_side_opt(double& yaw, int& Sdir, int& Fdir, double* vel,
		double L, double tauMA, int dir)
	{
		double speed = std::hypot(vel[0], vel[1]);
		double theta = strafe_theta_opt(speed, L, tauMA);
		strafe_side(yaw, Sdir, Fdir, vel, theta, L, tauMA, dir);
	}

	void strafe_side_const(double& yaw, int& Sdir, int& Fdir, double vel[2],
		double nofricspd, double L, double tauMA, int dir)
	{
		double speed = std::hypot(vel[0], vel[1]);
		double theta = strafe_theta_const(speed, nofricspd, L, tauMA);
		strafe_side(yaw, Sdir, Fdir, vel, theta, L, tauMA, dir);
	}

	void strafe_line_opt(double& yaw, int& Sdir, int& Fdir, double vel[2],
		double pos[2], double L, double tau, double MA,
		double line_origin[2], double line_dir[2])
	{
		double tauMA = tau * MA;
		double speed = std::hypot(vel[0], vel[1]);
		double theta = strafe_theta_opt(speed, L, tauMA);
		double ct = std::cos(theta);
		double tmp = L - speed * ct;
		if (tmp < 0)
		{
			strafe_side(yaw, Sdir, Fdir, vel, theta, L, tauMA, 1);
			return;
		}

		if (tauMA < tmp)
			tmp = tauMA;
		tmp /= speed;
		double st = std::sin(theta);
		double newpos_right[2], newpos_left[2];
		double avec[2];

		avec[0] = (vel[0] * ct + vel[1] * st) * tmp;
		avec[1] = (-vel[0] * st + vel[1] * ct) * tmp;
		newpos_right[0] = pos[0] + tau * (vel[0] + avec[0]);
		newpos_right[1] = pos[1] + tau * (vel[1] + avec[1]);

		avec[0] = (vel[0] * ct - vel[1] * st) * tmp;
		avec[1] = (vel[0] * st + vel[1] * ct) * tmp;
		newpos_left[0] = pos[0] + tau * (vel[0] + avec[0]);
		newpos_left[1] = pos[1] + tau * (vel[1] + avec[1]);

		bool rightgt = point2line_distsq(newpos_right, line_origin, line_dir) <
			point2line_distsq(newpos_left, line_origin, line_dir);
		strafe_side(yaw, Sdir, Fdir, vel, theta, L, tauMA, rightgt ? 1 : -1);
	}

	void strafe_back(double& yaw, int& Sdir, int& Fdir, double vel[2],
		double tauMA)
	{
		Sdir = 0;
		Fdir = -1;

		yaw = std::atan2(vel[1], vel[0]);
		float frac = yaw / M_U_RAD;
		frac -= std::trunc(frac);
		if (frac > 0.5)
			yaw += M_U_RAD;
		else if (frac < -0.5)
			yaw -= M_U_RAD;
		yaw = anglemod_rad(yaw);

		double avec[2] = { std::cos(yaw), std::sin(yaw) };
		vel[0] -= tauMA * avec[0];
		vel[1] -= tauMA * avec[1];
	}

	double strafe_opt_spd(double spd, double L, double tauMA)
	{
		double tmp = L - tauMA;
		if (tmp < 0)
			return std::sqrt(spd * spd + L * L);
		if (tmp < spd)
			return std::sqrt(spd * spd + tauMA * (L + tmp));
		return spd + tauMA;
	}

	void MovementFix(SDK::CUserCmd* cmd, QAngle& realvec)
	{
		Vector vMove(cmd->move.x, cmd->move.y, cmd->move.z);
		float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
		QAngle vMove2;
		VectorAngles3(vMove, vMove2);
		flYaw = DEG2RAD(cmd->viewangles.y - realvec.yaw + vMove2.yaw);
		cmd->move.x = cos(flYaw) * flSpeed;
		cmd->move.y = sin(flYaw) * flSpeed;
		if (90 < abs(cmd->viewangles.x)) cmd->move.x *= -1;
	}

	static void BhopMovement(SDK::CUserCmd* pCmd, QAngle& OrigAngles)
	{
		static AutoStrafer strafer;

		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player) return;
		static QAngle LastOrigAng = OrigAngles;
		auto airaccel = INTERFACES::cvar->FindVar(XorStr("sv_airaccelerate"))->GetFloat();
		auto maxspeed = INTERFACES::cvar->FindVar(XorStr("sv_maxspeed"))->GetFloat();

		double yawrad = MATH::NormalizeYawInPlace(OrigAngles.yaw) * M_PI / 180.0f;

		Vector velocity = local_player->GetVelocity();
		float speed = velocity.Length();

		//if (pCmd->buttons & IN_DUCK) 
		//	speed *= 0.333f;
		//double tau = I::pGlobals->interval_per_tick, MA = (maxspeed  * airaccel) / tau;
		//double tau = I::pGlobals->interval_per_tick, MA = (2000  * 1000) / tau;

		double tau = /*I::pGlobals->absoluteframetime*/INTERFACES::Globals->interval_per_tick, MA = (maxspeed * airaccel) / tau;

		int Sdir = 0, Fdir = 0;

		double vel[3] = { velocity[0], velocity[1], velocity[2] };
		double pos[2] = { 0, 0 };
		double dir[2] = { std::cos(OrigAngles[1] * M_PI / 180), std::sin(OrigAngles[1] * M_PI / 180) };

		strafer.strafe_line_opt(yawrad, Sdir, Fdir, vel, pos, 30.0, tau, MA, pos, dir);

		OrigAngles.yaw = MATH::NormalizeYawInPlace((float)yawrad * 180.0f / M_PI);

		pCmd->move.y = Sdir * 450.0f;
		pCmd->move.x = Fdir * 450.0f;

		strafer.MovementFix(pCmd, OrigAngles);
		MATH::NormalizeYaw(LastOrigAng.yaw);
		LastOrigAng = OrigAngles;
	}
};
class CMovement
{
public:
	void bunnyhop(SDK::CUserCmd* cmd);
	void AutoStrafer1(SDK::CUserCmd * pCmd, QAngle & OrigAngles);
	void autostrafer(SDK::CUserCmd* cmd);
	void duckinair(SDK::CUserCmd* cmd);
	void circle_strafe(SDK::CUserCmd* cmd, float* circle_yaw);
};

extern CMovement* movement;