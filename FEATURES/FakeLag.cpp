#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/NetChannel.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/ConVar.h"
#include "../FEATURES/Fakewalk.h"
#include "../FEATURES/AutoWall.h"
#include "../FEATURES/FakeLag.h"
#include "../SDK/Collideable.h"
#include "../SDK/CTrace.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseEntity.h"
#include "Aimbot.h"

int CFakeLag::adaptive()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player || local_player->GetHealth() <= 0)
		return 1;	

	auto velocity = local_player->GetVelocity();
	velocity.z = 0.f;
	auto speed = velocity.Length();
	auto distance_per_tick = speed * INTERFACES::Globals->interval_per_tick;
	int choked_ticks = std::ceilf(64.f / distance_per_tick);
	return std::min<int>(choked_ticks, 14.f);
}
int CFakeLag::passive()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player || local_player->GetHealth() <= 0)
		return 1;

	srand(time(NULL));
	if (rand() % 12 + 1)
		return Config::AntiAim::AAFakelag::get().fakelagticks;
	else
		return 1;
}

int CFakeLag::step()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player || local_player->GetHealth() <= 0)
		return 1;
	if (local_player->GetVelocity().Length2D() > 0)
		return Config::AntiAim::AAFakelag::get().fakelagticks;
	else
		return 1;
}
bool IntersectionBoundingBox(const Vector& src, const Vector& dir, const Vector& min, const Vector& max, Vector* hit_point)
{
	/*
	Fast Ray-Box Intersection
	by Andrew Woo
	from "Graphics Gems", Academic Press, 1990
	*/

	constexpr int NUMDIM = 3;
	constexpr int RIGHT = 0;
	constexpr int LEFT = 1;
	constexpr int MIDDLE = 2;

	bool inside = true;
	char quadrant[NUMDIM];
	int i;

	// Rind candidate planes; this loop can be avoided if
	// rays cast all from the eye(assume perpsective view)
	Vector candidatePlane;
	for (i = 0; i < NUMDIM; i++)
	{
		if (src[i] < min[i])
		{
			quadrant[i] = LEFT;
			candidatePlane[i] = min[i];
			inside = false;
		}
		else if (src[i] > max[i])
		{
			quadrant[i] = RIGHT;
			candidatePlane[i] = max[i];
			inside = false;
		}
		else
		{
			quadrant[i] = MIDDLE;
		}
	}

	// Ray origin inside bounding box
	if (inside)
	{
		if (hit_point)
			*hit_point = src;
		return true;
	}

	// Calculate T distances to candidate planes
	Vector maxT;
	for (i = 0; i < NUMDIM; i++)
	{
		if (quadrant[i] != MIDDLE && dir[i] != 0.f)
			maxT[i] = (candidatePlane[i] - src[i]) / dir[i];
		else
			maxT[i] = -1.f;
	}

	// Get largest of the maxT's for final choice of intersection
	int whichPlane = 0;
	for (i = 1; i < NUMDIM; i++)
	{
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;
	}

	// Check final candidate actually inside box
	if (maxT[whichPlane] < 0.f)
		return false;

	for (i = 0; i < NUMDIM; i++)
	{
		if (whichPlane != i)
		{
			float temp = src[i] + maxT[whichPlane] * dir[i];
			if (temp < min[i] || temp > max[i])
			{
				return false;
			}
			else if (hit_point)
			{
				(*hit_point)[i] = temp;
			}
		}
		else if (hit_point)
		{
			(*hit_point)[i] = candidatePlane[i];
		}
	}

	// ray hits box
	return true;
}
bool peek(SDK::CUserCmd* pCmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player)
		return false;


	float speed = local_player->GetVelocity().Length();

	if (speed > 300.0f)
		return false;


	for (int i = 0; i <= INTERFACES::ClientEntityList->GetHighestEntityIndex(); i++)
	{
		auto pEntity = INTERFACES::ClientEntityList->GetClientEntity(i);

		if (!pEntity)
			continue;

		if (!pEntity->IsAlive() || pEntity->GetIsDormant())
			continue;

		if (pEntity == local_player || local_player->GetTeam() == pEntity->GetTeam())
			continue;

		auto pWeapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(pEntity->GetActiveWeaponIndex()));

		if (!pWeapon)
			continue;

		if (pWeapon->GetLoadedAmmo() <= 0)
			continue;

		Vector lol = aimbot->multipoint(pEntity, &global::originalCMD);
			
		if (lol != Vector(0, 0, 0))
			return true;
	}
}
void CFakeLag::do_fakelag(SDK::CUserCmd* cmd)
{
	global::should_send_packet = true;
	int choke_amount;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0)
		return;

	auto net_channel = INTERFACES::Engine->GetNetChannel();
	if (!net_channel)
		return;


	choke_amount = 1;
	if (GetAsyncKeyState(Config::Misc::MiscMain::get().fakeduck))
		choke_amount = 14;
	else {
		if (Config::AntiAim::AAFakelag::get().triggers[0] && local_player->GetFlags() & FL_ONGROUND)
			choke_amount = Config::AntiAim::AAFakelag::get().fakelagtickstrigger;
		else if (Config::AntiAim::AAFakelag::get().triggers[1] && !(local_player->GetFlags() & FL_ONGROUND))
			choke_amount = Config::AntiAim::AAFakelag::get().fakelagtickstrigger;
		else if (Config::AntiAim::AAFakelag::get().triggers[3] && peek(cmd))
			choke_amount = Config::AntiAim::AAFakelag::get().fakelagtickstrigger;
		else {
			switch (Config::AntiAim::AAFakelag::get().fakelag)
			{
			case 1: choke_amount = passive(); break;
			case 2: choke_amount = Config::AntiAim::AAFakelag::get().fakelagticks; break;
			case 3: choke_amount = adaptive(); break;
			case 4: choke_amount = step(); break;
			}
		}
	}


	global::packets_choked = choke_amount;


	if (net_channel->m_nChokedPackets >= min(14, choke_amount))
		global::should_send_packet = true;
	else
		global::should_send_packet = false;
}

CFakeLag* fakelag = new CFakeLag();