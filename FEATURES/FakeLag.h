#pragma once
#pragma once

namespace SDK
{
	class CUserCmd;
}

class CFakeLag
{
public:
	int lag_comp_break();
	int adaptive();
	int passive();
	int step();
	void do_fakelag(SDK::CUserCmd* cmd);
};

extern CFakeLag* fakelag;