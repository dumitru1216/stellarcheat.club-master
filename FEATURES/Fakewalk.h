#pragma once

namespace SDK
{

}

class CSlowwalk
{
public:
	int choked;
	void DoSlowWalk(SDK::CUserCmd *cmd);
	void MinWalk(SDK::CUserCmd* cmd, float get_speed);
};

extern CSlowwalk* slowwalk;