#pragma once

namespace SDK
{
	class CRenderView
	{
	public:
		void SetBlend(float blend)
		{
			typedef void(__thiscall* Fn)(void*, float);
			VMT::VMTHookManager::GetFunction<Fn>(this, 4)(this, blend);
		}
		float GetBlend()
		{
			typedef float(__thiscall* Fn)(void*);
			return VMT::VMTHookManager::GetFunction<Fn>(this, 5)(this);
		}
		void SetColorModulation(float const* blend)
		{
			typedef void(__thiscall* Fn)(void*, float const*);
			return VMT::VMTHookManager::GetFunction<Fn>(this, 6)(this, blend);
		}
		void SetColorModulation2(float const* colors)
		{
			typedef void(__thiscall* oDrawModelExecute)(PVOID, float const*);
			return VMT::VMTHookManager::GetFunction<oDrawModelExecute>(this, 6)(this, colors);
		}
		void GetColorModulation(float* blend)
		{
			typedef void(__thiscall* Fn)(void*, float*);
			VMT::VMTHookManager::GetFunction<Fn>(this, 7)(this, blend);
		}
	};
}