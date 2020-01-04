#pragma once
namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}

class CAntiAim
{
public:
	float antiaim_yaw_real(SDK::CUserCmd * cmd);
	void antiaim_pitch_real(int choose, SDK::CUserCmd* cmd);
	bool ShallReturn(SDK::CUserCmd * cmd);
	void do_antiaim(SDK::CUserCmd* cmd);
	void fix_movement(SDK::CUserCmd* cmd);
	Vector fix_movement(SDK::CUserCmd* cmd, SDK::CUserCmd orignal);
	void manipulate(SDK::CUserCmd* cmd);
	float TimeUntilNextLBYUpdate()
	{
		return local_update - INTERFACES::Globals->curtime;
	}
	float m_next_lby_update_time = 0.f;
private:
	bool lby_update(SDK::CUserCmd* cmd);
};

extern CAntiAim* antiaim;