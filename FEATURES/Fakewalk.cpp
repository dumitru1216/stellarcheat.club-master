#include "../includes.h"

#include "../UTILS/variables.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/IClient.h"
#include "../SDK/ConVar.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CBaseAnimState.h"

#include "Fakewalk.h"
#include <iostream>
#include <algorithm>
#include <math.h>
inline float FastSqrt(float x)
{
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}
#define Square(x) ((x)*(x))
void CSlowwalk::MinWalk(SDK::CUserCmd* cmd, float get_speed)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto animstate = local_player->GetAnimState();
	if (get_speed <= 0.f)
		return;

	float min_speed = (float)(FastSqrt(Square(global::originalCMD.move.x) + Square(global::originalCMD.move.y) + Square(global::originalCMD.move.z)));
	if (min_speed <= 0.f)
		return;

	if (cmd->buttons & IN_DUCK)
		get_speed *= 2.94117647f;

	if (min_speed <= get_speed)
		return;

	float kys = get_speed / min_speed;

	global::originalCMD.move.x *= kys;
	global::originalCMD.move.y *= kys;
	global::originalCMD.move.z *= kys;
	animstate->m_flUnknownFraction = 0.f;

}


void CSlowwalk::DoSlowWalk(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (GetAsyncKeyState(Config::AntiAim::AAMisc::get().slowwwalk)) {
		slowwalk->MinWalk(cmd, Config::AntiAim::AAMisc::get().slowwwalk_speed);
	}
}
CSlowwalk* slowwalk = new CSlowwalk();
