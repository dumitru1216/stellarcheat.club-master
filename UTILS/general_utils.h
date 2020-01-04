#pragma once

#include "logging.h"
#include "../SDK/CBaseWeapon.h"

class Vector2D;
class Vector;
namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}
namespace UTILS
{
	template<class T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi)
	{
		return (v >= lo && v <= hi) ? v : (v < lo ? lo : hi);
	}
    template<unsigned int IIdx, typename TRet, typename ... TArgs>
    static auto CallVFunc(void* thisptr, TArgs ... argList) -> TRet
    {
        using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
        return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
    }

	bool is_point_in_range(Vector2D, Vector2D, Vector2D);
	float GetFraction(float, float, float);
	float GetValueFromFraction(float, float, float);
	unsigned int GetNumberOfDigits(int);
	float RoundToDecimalPlaces(float, int);
	std::string FloatToString(float, int);
	Vector CalcAngle(Vector src, Vector dst);
	bool IsOnScreen(Vector origin, Vector& screen);
	float GetCurtime();
	float Lerp(float fraction, float min, float max);
	uint64_t FindSignature(const char* szModule, const char* szSignature);
	DWORD WaitOnModuleHandle(std::string moduleName);
	bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask);
	DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask);
	bool ClampLemon(Vector& angles);
	bool IsVisibleOld(Vector &Src, Vector &Dst, SDK::CBaseEntity* skip);
	bool IsVisiblePos(Vector &bonepos);
	bool IsPlayerVisible(SDK::CBaseEntity* target);
	unsigned int FNVHashEx(const char* const data, const unsigned int value = 2166136261);
	void SetClanTag(const char* tag, const char* name);
	Vector TraceToEnd(Vector start, Vector end);
	const char* GetHitgroupName(int hitgroup);
	bool ConsolePrint(bool logToFile, const char *fmt, ...);
	SDK::CBaseEntity * LocalPlayer();
	bool CanShoot(bool right_click = false);
	bool IsWeaponGrenade(SDK::CBaseWeapon* weapon);
	bool IsWeaponSniper(SDK::CBaseWeapon* weapon);
	bool IsWeaponKnife(SDK::CBaseWeapon* weapon);
	bool IsWeaponTaser(SDK::CBaseWeapon* weapon);
	float GetLatency();
	bool IsPressingMovementKeys(SDK::CUserCmd* cmd);

	void RotateMovement(SDK::CUserCmd* cmd, float yaw);

	int TicksToStop(Vector velocity);

	SDK::CBaseWeapon* GetWeapon(SDK::CBaseEntity* entity);

	Vector TraceToEnd(Vector start, Vector end);
}
