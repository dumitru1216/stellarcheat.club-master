#include "../includes.h"

#include "general_utils.h"

#include "interfaces.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/IEngine.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CUserCmd.h"
#include "../UTILS/render.h"
#include "../SDK/CTrace.h"
#include "../SDK/ConVar.h"


#define enc_char(s) enc_str(s).c_str()
#include <corecrt.h>
#include <iosfwd>
namespace UTILS
{
	HANDLE _out = NULL;

	bool ConsolePrint(bool logToFile, const char *fmt, ...)
	{
		char buf[1024] = { 0 };
		va_list va;

		va_start(va, fmt);
		_vsnprintf_s(buf, 1024, fmt, va);
		va_end(va);

		std::string outRes = buf;

		OutputDebugString(outRes.c_str());

		

		if (!_out)
			return false;

		return !!WriteConsoleA(_out, outRes.c_str(), static_cast<DWORD>(strlen(outRes.c_str())), nullptr, nullptr);
	}

	Vector TraceToEnd(Vector start, Vector end)
	{
		SDK::trace_t trace;
		SDK::CTraceWorldOnly filter;
		SDK::Ray_t ray;

		ray.Init(start, end);
		INTERFACES::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);

		return trace.end;
	}

	const char* GetHitgroupName(int hitgroup)
	{
		switch (hitgroup)
		{
		case 1:
			return "HEAD";
		case 2:
			return "CHEST";
		case 3:
			return "PELVIS";
		case 4:
			return "LEFT ARM";
		case 5:
			return "RIGHT ARM";
		case 6:
			return "LEFT LEG";
		case 7:
			return "RIGHT LEG";
		default:
			return "";
		}
	}
	bool is_point_in_range(Vector2D point, Vector2D position, Vector2D size)
	{
		return (point.x >= position.x && point.x <= position.x + size.x)
			&& (point.y >= position.y && point.y <= position.y + size.y);
	}
	float GetFraction(float value, float min, float max)
	{
		float ratio = 1.f / (max - min);
		return ratio * (value - min);
	}
	float GetValueFromFraction(float fraction, float min, float max)
	{
		return ((max - min) * fraction) + min;
	}
	unsigned int GetNumberOfDigits(int i)
	{
		i = i < 0 ? -(i * 10) : i;
		return i > 0 ? (int)log10((double)i) + 1 : 1;
	}
	float GetLatency()
	{
		auto nci = INTERFACES::Engine->GetNetChannelInfo();
		if (!nci)
			return 0.f;

		return nci->GetLatency(FLOW_OUTGOING);
	}

	float RoundToDecimalPlaces(float value, int decimals)
	{
		float yeet = pow(10.f, decimals);
		return roundf(value * yeet) / yeet;
	}
	std::string FloatToString(float value, int decimals) // this shit is ugly as fOK
	{
		std::string ret = std::to_string(value);

		ret.erase(ret.begin() + clamp<unsigned int>(GetNumberOfDigits(value)
			+ decimals + 1, 0, ret.size()), ret.end());

		if (decimals <= 0)
			ret.pop_back();

		return ret;
	}
	Vector CalcAngle(Vector src, Vector dst)
	{
		Vector ret;
		MATH::VectorAngles(dst - src, ret);
		return ret;
	}

	bool IsOnScreen(Vector origin, Vector& screen)
	{
		if (!RENDER::WorldToScreen(origin, screen)) return false;
		int iScreenWidth, iScreenHeight;
		INTERFACES::Engine->GetScreenSize(iScreenWidth, iScreenHeight);
		bool xOk = iScreenWidth > screen.x > 0, yOk = iScreenHeight > screen.y > 0;
		return xOk && yOk;
	}
	float GetCurtime()
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		if (!local_player)
			return -1;

		return static_cast<float>(local_player->GetTickBase()) * INTERFACES::Globals->interval_per_tick;
	}
	float Lerp(float fraction, float min, float max)
	{
		return min + (fraction * (max - min));
	}
	uint64_t FindSignature(const char* szModule, const char* szSignature)
	{
		MODULEINFO modInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
		DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
		DWORD endAddress = startAddress + modInfo.SizeOfImage;
		const char* pat = szSignature;
		DWORD firstMatch = 0;
		for (DWORD pCur = startAddress; pCur < endAddress; pCur++) {
			if (!*pat) return firstMatch;
			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
				if (!firstMatch) firstMatch = pCur;
				if (!pat[2]) return firstMatch;
				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
				else pat += 2;    //one ?
			}
			else {
				pat = szSignature;
				firstMatch = 0;
			}
		}
		return NULL;
	}

	DWORD WaitOnModuleHandle(std::string moduleName)
	{
		DWORD ModuleHandle = NULL;
		while (!ModuleHandle)
		{
			ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
			if (!ModuleHandle)
				Sleep(50);
		}
		return ModuleHandle;
	}
	bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++Mask, ++Data)
		{
			if (*szMask == 'x' && *Mask != *Data)
			{
				return false;
			}
		}
		return (*szMask) == 0;
	}
	DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
	{
		DWORD Address = WaitOnModuleHandle(moduleName.c_str());
		MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
		DWORD Length = ModInfo.SizeOfImage;
		for (DWORD c = 0; c < Length; c += 1)
		{
			if (bCompare((BYTE*)(Address + c), Mask, szMask))
			{
				return (DWORD)(Address + c);
			}
		}
		std::cout << red << "Faild to find petter aka memory adress!" << "wrong adress: " << Address << std::endl;
		return 0;
	}

	void NormalizeLemon(Vector& angle)
	{
		while (angle.x > 89.0f) {
			angle.x -= 180.f;
		}
		while (angle.x < -89.0f) {
			angle.x += 180.f;
		}
		while (angle.y > 180.f) {
			angle.y -= 360.f;
		}
		while (angle.y < -180.f) {
			angle.y += 360.f;
		}
	}
	bool IsVisibleOld(Vector &Src, Vector &Dst, SDK::CBaseEntity* skip) //skip = pLocal 99% of the time
	{
		SDK::Ray_t ray;
		ray.Init(Src, Dst);

		SDK::trace_t tr;
		SDK::CTraceFilter filter;
		filter.pSkip1 = skip;

		INTERFACES::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

		return tr.m_pEnt != nullptr && tr.m_pEnt != INTERFACES::ClientEntityList->GetClientEntity(0);
	}
	bool IsVisiblePos(Vector &bonepos)
	{
		auto pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

		if (!pLocal)
			return false;

		Vector localEyePos = pLocal->GetEyePosition();
		return UTILS::IsVisibleOld(localEyePos, bonepos, pLocal);
	}
	bool IsBoneVisible(SDK::CBaseEntity* Target, int Bone)
	{
		auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		Vector vEnd;
		vEnd = Target->GetBonePosition(Bone);

		SDK::trace_t Trace;
		SDK::Ray_t Ray;

		SDK::CTraceFilter Filter;
		Filter.pSkip1 = local_player;

		Ray.Init(local_player->GetEyePosition(), vEnd);
		INTERFACES::Trace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

		if (Trace.m_pEnt && Trace.m_pEnt->GetTeam() != local_player->GetTeam() && !(Trace.m_pEnt->GetHealth() <= 0) && !(Trace.m_pEnt->GetIsDormant()))
		{
			if (Trace.physicsBone <= 128 && Trace.physicsBone > 0)
			{
				return true;
			}
		}

		return false;
	}

	/* IsPlayerVisible */
	bool IsPlayerVisible(SDK::CBaseEntity* target)
	{
		for (int i = 0; i < 128; i++)
		{
			if (IsBoneVisible(target, i))
			{
				return true;
			}
		}
		return false;
	}

	void ClampAnglesLemon(Vector &angles)
	{
		if (angles.y > 180.0f)
			angles.y = 180.0f;
		else if (angles.y < -180.0f)
			angles.y = -180.0f;

		if (angles.x > 89.0f)
			angles.x = 89.0f;
		else if (angles.x < -89.0f)
			angles.x = -89.0f;

		angles.z = 0;
	}

	bool ClampLemon(Vector &angles)
	{
		Vector a = angles;
		NormalizeLemon(a);
		ClampAnglesLemon(a);

		if (isnan(a.x) || isinf(a.x) ||
			isnan(a.y) || isinf(a.y) ||
			isnan(a.z) || isinf(a.z)) {
			return false;
		}
		else {
			angles = a;
			return true;
		}
	}
	void SetClanTag(const char* tag, const char* name)
	{
		static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(reinterpret_cast<uintptr_t>(GetModuleHandleA("engine.dll")) + 0x87060);
		pSetClanTag(tag, name);
	}
	SDK::CBaseEntity * LocalPlayer()
	{
		return INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	}
	bool IsWeaponGrenade(SDK::CBaseWeapon* weapon)
	{
		if (!weapon)
			return false;

		auto weapon_info = weapon->GetWeaponInfo();
		if (!weapon_info)
			return false;

		if (weapon->is_grenade())
			return true;

		return false;
	}

	bool IsWeaponSniper(SDK::CBaseWeapon* weapon)
	{
		if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_AWP || weapon->GetItemDefenitionIndex() == SDK::WEAPON_SSG08 ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_SCAR20 || weapon->GetItemDefenitionIndex() == SDK::WEAPON_G3SG1 ||
			weapon->GetItemDefenitionIndex() == SDK::WEAPON_AUG || weapon->GetItemDefenitionIndex() == SDK::WEAPON_SG553)
			return true;
		else
			return false;
	}

	bool IsWeaponKnife(SDK::CBaseWeapon* weapon)
	{
		if (!weapon)
			return false;

		auto weapon_info = weapon->GetWeaponInfo();
		if (!weapon_info)
			return false;

		if (weapon->is_knife())
			return true;

		return false;
	}

	bool IsWeaponTaser(SDK::CBaseWeapon* weapon)
	{
		if (!weapon)
			return false;

		if (weapon->GetItemDefenitionIndex() == SDK::WEAPON_TASER)
			return true;

		return false;
	}

	bool IsPressingMovementKeys(SDK::CUserCmd* cmd)
	{
		if (!cmd)
			return false;

		if (cmd->buttons & IN_FORWARD ||
			cmd->buttons & IN_BACK || cmd->buttons & IN_LEFT || cmd->buttons & IN_RIGHT ||
			cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT)
			return true;

		return false;
	}

	void RotateMovement(SDK::CUserCmd* cmd, float yaw)
	{
		Vector viewangles;
		INTERFACES::Engine->GetViewAngles(viewangles);

		float rotation = DEG2RAD(viewangles.y - yaw);

		float cos_rot = cos(rotation);
		float sin_rot = sin(rotation);

		float new_forwardmove = (cos_rot * cmd->move.x) - (sin_rot * cmd->move.y);
		float new_sidemove = (sin_rot * cmd->move.x) + (cos_rot * cmd->move.y);

		cmd->move.x = new_forwardmove;
		cmd->move.y = new_sidemove;
	}

	int TicksToStop(Vector velocity)
	{
		static auto sv_maxspeed = INTERFACES::cvar->FindVar("sv_maxspeed");

		const float max_speed = 320.f;
		const float acceleration = 5.5f;
		const float max_accelspeed = acceleration * max_speed * INTERFACES::Globals->interval_per_tick;

		return velocity.Length() / max_accelspeed;
	}

	SDK::CBaseWeapon* GetWeapon(SDK::CBaseEntity* entity)
	{
		if (!entity)
			return nullptr;

		return reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));
	}

}