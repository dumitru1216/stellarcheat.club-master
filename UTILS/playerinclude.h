#pragma once

#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)

namespace SDK
{
	class CBaseEntity;
}

namespace UTILS
{
	struct BulletImpact_t
	{
		float			flImpactTime;
		Vector			vecImpactPos;
		CColor			color;
		SDK::CBaseEntity*	pPlayer;



		__forceinline BulletImpact_t()
		{
			vecImpactPos = { 0.0f, 0.0f, 0.0f };
			flImpactTime = 0.0f;
			color = WHITE;
			pPlayer = nullptr;
		}

		__forceinline BulletImpact_t(SDK::CBaseEntity* player, Vector pos, float time, CColor col = WHITE)
		{
			pPlayer = player;
			flImpactTime = time;
			vecImpactPos = pos;
			color = col;
		}
	};
	template <class T>
	constexpr const T& Min(const T& x, const T& y)
	{
		return (x > y) ? y : x;
	}

	template <class T>
	constexpr const T& Max(const T& x, const T& y)
	{
		return (x < y) ? y : x;
	}
}