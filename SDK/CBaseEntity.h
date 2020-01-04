#pragma once

#include "../UTILS/offsets.h"
#include "../UTILS/interfaces.h"
#include "../SDK/ModelInfo.h"
#include "../UTILS/qangle.h"
#include "../UTILS/NetvarHookManager.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CClientEntityList.h"

#include "AnimLayer.h"
#include "RecvData.h"

template<class T>
T GetValue(const int offset)
{
	return reinterpret_cast<T>(reinterpret_cast<std::uintptr_t>(this) + offset);
}
enum DataUpdateType_txd
{
	DATA_UPDATE_CREATEDxd = 0,
	DATA_UPDATE_DATATABLE_CHANGEDxd,
};
namespace SDK
{
	class CBaseAnimState;
	class Collideable;
	typedef unsigned long CBaseHandle;
	struct studiohdr_t;
	struct model_t;
	class CBoneBitList;
	class CIKContext;

	enum MoveType_t
	{
		MOVETYPE_NONE = 0,
		MOVETYPE_ISOMETRIC,
		MOVETYPE_WALK,
		MOVETYPE_STEP,
		MOVETYPE_FLY,
		MOVETYPE_FLYGRAVITY,
		MOVETYPE_VPHYSICS,
		MOVETYPE_PUSH,
		MOVETYPE_NOCLIP,
		MOVETYPE_LADDER,
		MOVETYPE_OBSERVER,
		MOVETYPE_CUSTOM,
		MOVETYPE_LAST = MOVETYPE_CUSTOM,
		MOVETYPE_MAX_BITS = 4
	};

	class CBaseAnimating
	{
	public:
		std::array<float, 24>* m_flPoseParameter()
		{
			static int offset = 0;
			if (!offset)
				offset = OFFSETS::m_flPoseParameter;
			return (std::array<float, 24>*)((uintptr_t)this + offset);
		}
		model_t* GetModel()
		{
			void* pRenderable = reinterpret_cast<void*>(uintptr_t(this) + 0x4);
			typedef model_t* (__thiscall* fnGetModel)(void*);

			return VMT::VMTHookManager::GetFunction<fnGetModel>(pRenderable, 8)(pRenderable);
		}
		void SetBoneMatrix(matrix3x4_t* boneMatrix)
		{
			//Offset found in C_BaseAnimating::GetBoneTransform, string search ankle_L and a function below is the right one
			const auto model = this->GetModel();
			if (!model)
				return;

			matrix3x4_t* matrix = *(matrix3x4_t**)((DWORD)this + 9880);
			studiohdr_t *hdr = INTERFACES::ModelInfo->GetStudioModel(model);
			if (!hdr)
				return;
			int size = hdr->numbones;
			if (matrix) {
				for (int i = 0; i < size; i++)
					memcpy(matrix + i, boneMatrix + i, sizeof(matrix3x4_t));
			}
		}
		void GetDirectBoneMatrix(matrix3x4_t* boneMatrix)
		{
			const auto model = this->GetModel();
			if (!model)
				return;

			matrix3x4_t* matrix = *(matrix3x4_t**)((DWORD)this + 9880);
			studiohdr_t *hdr = INTERFACES::ModelInfo->GetStudioModel(model);
			if (!hdr)
				return;
			int size = hdr->numbones;
			if (matrix) {
				for (int i = 0; i < size; i++)
					memcpy(boneMatrix + i, matrix + i, sizeof(matrix3x4_t));
			}
		}
	};

	class CBaseEntity
	{
	public:
		int GetHealth()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iHealth);
		}
		int GetFlags()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_fFlags);
		}
		short GetItemDefenitionIndex()
		{
			return *reinterpret_cast<short*>(uintptr_t(this) + 0x2FAA);
		}
		char* GetGunIcon() {
			int WeaponId = this->GetItemDefenitionIndex();

			switch (WeaponId) {
			case  SDK::definition_index::WEAPON_KNIFEGG:
				return "r";
			case  SDK::definition_index::WEAPON_KNIFE:
				return "r";
			case  SDK::definition_index::WEAPON_KNIFE_T:
				return "r";
			case  SDK::definition_index::WEAPON_KNIFE_CT:
				return "r";
			case SDK::definition_index::WEAPON_DEAGLE:
				return "F";
			case SDK::definition_index::WEAPON_ELITE:
				return "G";
			case SDK::definition_index::WEAPON_FIVESEVEN:
				return "I";
			case SDK::definition_index::WEAPON_GLOCK:
				return "g";
			case  SDK::definition_index::WEAPON_HKP2000:
				return "e";
			case SDK::definition_index::WEAPON_P250:
				return "f";
			case  SDK::definition_index::WEAPON_USP_SILENCER:
				return "c";
			case SDK::definition_index::WEAPON_TEC9:
				return "a";
			case  SDK::definition_index::WEAPON_CZ75A:
				return "E";
			case  SDK::definition_index::WEAPON_REVOLVER:
				return "j";
			case  SDK::definition_index::WEAPON_MAC10:
				return "O";
			case  SDK::definition_index::WEAPON_UMP45:
				return "b";
			case  SDK::definition_index::WEAPON_BIZON:
				return "D";
			case  SDK::definition_index::WEAPON_MP7:
				return "N";
			case SDK::definition_index::WEAPON_MP9:
				return "R";
			case  SDK::definition_index::WEAPON_P90:
				return "U";
			case  SDK::definition_index::WEAPON_GALILAR:
				return "K";
			case  SDK::definition_index::WEAPON_FAMAS:
				return "H";
			case SDK::definition_index::WEAPON_M4A1_SILENCER:
				return "N";
			case SDK::definition_index::WEAPON_M4A1:
				return "M";
			case SDK::definition_index::WEAPON_AUG:
				return "B";
			case SDK::definition_index::WEAPON_SG553:
				return "X";
			case SDK::definition_index::WEAPON_AK47:
				return "A";
			case SDK::definition_index::WEAPON_G3SG1:
				return "J";
			case SDK::definition_index::WEAPON_SCAR20:
				return "W";
			case  SDK::definition_index::WEAPON_AWP:
				return "C";
			case  SDK::definition_index::WEAPON_SSG08:
				return "Y";
			case SDK::definition_index::WEAPON_XM1014:
				return "d";
			case  SDK::definition_index::WEAPON_SAWEDOFF:
				return "V";
			case SDK::definition_index::WEAPON_MAG7:
				return "P";
			case SDK::definition_index::WEAPON_NOVA:
				return "T";
			case SDK::definition_index::WEAPON_NEGEV:
				return "S";
			case SDK::definition_index::WEAPON_M249:
				return "L";
			case SDK::definition_index::WEAPON_TASER:
				return "Z";
			case SDK::definition_index::WEAPON_FLASHBANG:
				return "k";
			case SDK::definition_index::WEAPON_HEGRENADE:
				return "h";
			case SDK::definition_index::WEAPON_SMOKEGRENADE:
				return "k";
			case SDK::definition_index::WEAPON_MOLOTOV:
				return "i";
			case SDK::definition_index::WEAPON_DECOY:
				return "k";
			case SDK::definition_index::WEAPON_INCGRENADE:
				return "i";
			case SDK::definition_index::WEAPON_C4:
				return "v";
			default:
				return " ";
			}
		}
		void SetFlags(int flags)
		{
			*reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_fFlags) = flags;
		}
		int GetTeam()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iTeamNum);
		}
		int GetObserverMode()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iObserverMode);
		}
		int SetObserverMode(int mode)
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iObserverMode) = mode;
		}
		bool GetIsScoped()
		{
			return *reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bIsScoped);
		}
		Vector GetVelocity()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_vecVelocity);
		}
		void SetVelocity(Vector velocity)
		{
			*reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_vecVelocity) = velocity;
		}
		int GetMoney()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iAccount);
		}
		int GetLifeState()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_lifeState);
		}
		float GetLowerBodyYaw()
		{
			return *reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flLowerBodyYawTarget);
		}
		int ExplodeEffectTickBegin()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_nExplodeEffectTickBegin);
		}
		Vector GetVecOrigin()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_vecOrigin);
		}
		/*float m_flDuckAmount() {

			return *reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flDuckAmount);
		}*/
		bool IsAlive() 
		{ 
			return this->GetLifeState() == 0;
		}
		Vector& GetAbsOrigin()
		{
			typedef Vector& (__thiscall* OriginalFn)(void*);
			return ((OriginalFn)VMT::VMTHookManager::GetFunction<OriginalFn>(this, 10))(this);
		}
		int GetTickBase()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_nTickBase);
		}
		bool GetIsDormant()
		{
			return *reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bDormant);
		}
		CBaseAnimState* GetAnimState()
		{
			return *reinterpret_cast<CBaseAnimState**>(uintptr_t(this) + OFFSETS::animstate);
		}
		CAnimationLayer* GetAnimOverlaysModifiable() 
		{ 
			return (*reinterpret_cast< CAnimationLayer** >(reinterpret_cast< std::uintptr_t >(this) + OFFSETS::animlayer));
		}
		Collideable* GetCollideable()
		{
			return (Collideable*)((DWORD)this + OFFSETS::m_Collision);
		}
		void GetRenderBounds(Vector& mins, Vector& maxs)
		{
			void* pRenderable = (void*)(this + 0x4);
			typedef void(__thiscall* Fn)(void*, Vector&, Vector&);
			VMT::VMTHookManager::GetFunction<Fn>(pRenderable, 17)(pRenderable, mins, maxs);
		}
		Vector GetRenderAngles()
		{
			return *(Vector*)((DWORD)this + 0x128);
		}
		void SetRenderAngles(Vector angles)
		{
			*(Vector*)((DWORD)this + 0x128) = angles;
		}

		int GetIndex()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + 0x64);
		}
		int GetMoveType()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_MoveType);
		}
		CAnimationLayer& GetAnimOverlay(int Index)
		{
			return (*(CAnimationLayer**)((DWORD)this + OFFSETS::animlayer))[Index];
		}
		/*	Vector CBaseEntity::GetHitboxPosition(SDK::CBaseEntity* entity, int hitbox_id)
		{
			auto getHitbox = [](SDK::CBaseEntity* entity, int hitboxIndex) -> SDK::mstudiobbox_t*
			{
				if (entity->GetIsDormant() || entity->GetHealth() <= 0) return NULL;

				const auto pModel = entity->GetModel();
				if (!pModel) return NULL;

				auto pStudioHdr = INTERFACES::ModelInfo->GetStudioModel(pModel);
				if (!pStudioHdr) return NULL;

				auto pSet = pStudioHdr->pHitboxSet(0);
				if (!pSet) return NULL;

				if (hitboxIndex >= pSet->numhitboxes || hitboxIndex < 0) return NULL;

				return pSet->GetHitbox(hitboxIndex);
			};

			auto hitbox = getHitbox(entity, hitbox_id);
			if (!hitbox) return Vector(0, 0, 0);

			auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

			Vector bbmin, bbmax;
			MATH::VectorTransform(hitbox->bbmin, bone_matrix, bbmin);
			MATH::VectorTransform(hitbox->bbmax, bone_matrix, bbmax);

			return (bbmin + bbmax) * 0.5f;
		}
		*/
		Vector				GetPredicted(Vector p0);
		SDK::CAnimationLayer* GetAnimOverlays()
		{
			// to find offset: use 9/12/17 dll
			// sig: 55 8B EC 51 53 8B 5D 08 33 C0
			return *(SDK::CAnimationLayer**)((DWORD)this + 0x2980);
		}
		QAngle& GetAbsAnglesQang()//
		{
			typedef QAngle&(__thiscall* oGetAbsAngles)(PVOID);
			return ((oGetAbsAngles)VMT::VMTHookManager::GetFunction<oGetAbsAngles>(this, 11))(this);
		}
		void SetAbsAnglesVec(Vector angle)//
		{
			using SetAbsAnglesFn = void(__thiscall*)(void*, const Vector &angle);
			static SetAbsAnglesFn SetAbsAngles;

			if (!SetAbsAngles)
				SetAbsAngles = (SetAbsAnglesFn)(UTILS::FindPattern("client_panorama.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx"));

			SetAbsAngles(this, angle);
		}
		void SetAbsOriginal(Vector origin)//
		{
			using SetAbsOriginFn = void(__thiscall*)(void*, const Vector &origin);
			static SetAbsOriginFn SetAbsOrigin;
			if (!SetAbsOrigin)
				SetAbsOrigin = (SetAbsOriginFn)((DWORD)UTILS::FindPattern("client_panorama.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8\x00\x00", "xxxxxxxxxxxxx??"));
			SetAbsOrigin(this, origin);
		}
		void SetAbsAnglesQang(QAngle angle)//
		{
			using SetAbsAnglesFn = void(__thiscall*)(void*, const QAngle &angle);
			static SetAbsAnglesFn SetAbsAngles;

			if (!SetAbsAngles)
				SetAbsAngles = (SetAbsAnglesFn)(UTILS::FindPattern("client_panorama.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx"));

			SetAbsAngles(this, angle);
		}
		Vector& GetAbsAnglesVec()
		{
			typedef Vector& (__thiscall* OriginalFn)(void*);
			return ((OriginalFn)VMT::VMTHookManager::GetFunction<OriginalFn>(this, 11))(this);
		}
		void SetAnimOverlay(int Index, CAnimationLayer layer)
		{
			(*(CAnimationLayer**)((DWORD)this + OFFSETS::animlayer))[Index] = layer;
		}
		int CBaseEntity::GetSequenceActivity(int sequence)
		{
			const auto model = GetModel();
			if (!model)
				return -1;

			const auto hdr = INTERFACES::ModelInfo->GetStudioModel(model);
			if (!hdr)
				return -1;

			static auto offset = (DWORD)UTILS::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83");
			static auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, SDK::studiohdr_t*, int)>(offset);

			return GetSequenceActivity(this, hdr, sequence);
		}
		Vector GetEyeAngles()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_angEyeAngles);
		}
		Vector GetVecMins()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x320);
		}
		Vector GetVecMaxs()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x32C);
		}
		QAngle* EasyEyeAngles()
		{
			return (QAngle*)((uintptr_t)this + OFFSETS::m_angEyeAngles);
		}
		void SetModelIndex(int index)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, int);
			return VMT::VMTHookManager::GetFunction<OriginalFn>(this, 75)(this, index);
		}
		IClientNetworkable* CBaseEntity::GetNetworkable()
		{
			return reinterpret_cast<IClientNetworkable*>(reinterpret_cast<uintptr_t>(this) + 0x8);
		}
		void PreDataUpdate(DataUpdateType_txd updateType)
		{
			PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
			typedef void(__thiscall* OriginalFn)(PVOID, int);
			return VMT::VMTHookManager::GetFunction<OriginalFn>(pNetworkable, 6)(pNetworkable, updateType);
		}
		CBaseHandle * m_hMyWeapons()
		{
			return (CBaseHandle*)((uintptr_t)this + OFFSETS::m_hMyWeapons);
		}
		void SetEyeAngles(Vector angles)
		{
			*reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_angEyeAngles) = angles;
		}
		float GetSimTime()
		{
			return *reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flSimulationTime);
		}
		Vector GetViewOffset()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_vecViewOffset);
		}
		model_t* GetModel()
		{
			void* pRenderable = reinterpret_cast<void*>(uintptr_t(this) + 0x4);
			typedef model_t* (__thiscall* fnGetModel)(void*);

			return VMT::VMTHookManager::GetFunction<fnGetModel>(pRenderable, 8)(pRenderable);
		}
		matrix3x4_t GetBoneMatrix(int BoneID)
		{
			matrix3x4_t matrix;

			auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + OFFSETS::m_dwBoneMatrix);
			if (offset)
				matrix = *reinterpret_cast<matrix3x4_t*>(offset + 0x30 * BoneID);

			return matrix;
		}
		Vector GetBonePosition(int i)
		{
			VMatrix matrix[128];
			if (this->SetupBones(matrix, 128, BONE_USED_BY_HITBOX, GetTickCount64()))
			{
				return Vector(matrix[i][0][3], matrix[i][1][3], matrix[i][2][3]);
			}
			return Vector(0, 0, 0);
		}
		Vector GetEyePosition(void)
		{
			return GetVecOrigin() + *(Vector*)((DWORD)this + OFFSETS::m_vecViewOffset);
		}
		Vector GetPunchAngles()
		{
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_aimPunchAngle);
		}
		Vector* GetAimPunchAngle()
		{
			return (Vector*)((uintptr_t)this + OFFSETS::m_aimPunchAngle);
		}
		Vector* GetViewPunchAngle()
		{
			return (Vector*)((uintptr_t)this + OFFSETS::m_viewPunchAngle);
		}
		bool GetImmunity()
		{
			return *reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bGunGameImmunity);
		}
		bool SetupBones(VMatrix *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
        {
            __asm
            {
                mov edi, this
                lea ecx, dword ptr ds : [edi + 0x4]
                mov edx, dword ptr ds : [ecx]
                push currentTime
                push boneMask
                push nMaxBones
                push pBoneToWorldOut
                call dword ptr ds : [edx + 0x34]
            }
        }
		void UpdateClientSideAnimation()
		{
			typedef void(__thiscall* original)(void*);
			VMT::VMTHookManager::GetFunction<original>(this, 223)(this);
		}
		bool& GetClientSideAnimation2()
		{
			return *reinterpret_cast<bool*>((uintptr_t)this + OFFSETS::m_bClientSideAnimation);
		}
		void ClientAnimations(bool value)
		{
			static int m_bClientSideAnimation = UTILS::netvar_hook_manager.GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");
			*reinterpret_cast<bool*>(uintptr_t(this) + m_bClientSideAnimation) = value;
		}
		void SetAbsOrigin(Vector ArgOrigin)
		{
			using Fn = void(__thiscall*)(CBaseEntity*, const Vector &origin);
			static Fn func;

			if (!func)
				func = (Fn)(UTILS::FindPattern("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8\x00\x00", "xxxxxxxxxxxxx??"));

			func(this, ArgOrigin);
		}
		void SetOriginz(Vector wantedpos)
		{
			typedef void(__thiscall* SetOriginFn)(void*, const Vector &);
			static SetOriginFn SetOriginze = (SetOriginFn)(UTILS::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
			SetOriginze(this, wantedpos);
		}
		Vector& GetAbsAngles()
		{
			typedef Vector& (__thiscall* OriginalFn)(void*);
			return ((OriginalFn)VMT::VMTHookManager::GetFunction<OriginalFn>(this, 11))(this);
		}
		void SetAbsAngles(Vector angles)
		{
			using Fn = void(__thiscall*)(CBaseEntity*, const Vector &angles);
			static Fn func;

			if (!func)
				func = (Fn)(UTILS::FindPattern("client_panorama.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx"));

			func(this, angles);
		}
		float* GetPoseParamaters()
		{
			return reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flPoseParameter);
		}
		int DrawModel(int flags, uint8_t alpha)
		{
			void* pRenderable = (void*)(this + 0x4);

			using fn = int(__thiscall*)(void*, int, uint8_t);
			return VMT::VMTHookManager::GetFunction< fn >(pRenderable, 9)(pRenderable, flags, alpha);
		}
		float* flDuckSpeed()
		{
			return (float*)((DWORD)this + OFFSETS::m_fduckspeed);
		}
		float* flDuckAmount()
		{
			return (float*)((DWORD)this + OFFSETS::m_fduckamount);
		}
		auto& get_flags(void) {
			return *(byte*)(this + 0x30);
		}
		bool& get_clientside_animation()
		{
			static int m_bClientSideAnimation = OFFSETS::m_bClientSideAnimation;
			return *reinterpret_cast<bool*>(uintptr_t(this) + m_bClientSideAnimation);
		}
		bool IsWeapon()
		{
			if (!this)
				return false;

			return VMT::VMTHookManager::GetFunction<bool(__thiscall*)(CBaseEntity*)>(this, 161)(this);
		}
		int& get_eflags()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + 0x44);
		}
		Vector &get_abs_velocity()
		{
			typedef Vector& (__thiscall * OriginalFn)(void*);

			if (this)
				return *(Vector*)((DWORD)this + 0x94);
		}
		ClientClass* GetClientClass()
		{
			void* Networkable = (void*)(this + 0x8);
			typedef ClientClass*(__thiscall* OriginalFn)(void*);
			return VMT::VMTHookManager::GetFunction<OriginalFn>(Networkable, 2)(Networkable);
		}
		float GetNextAttack()
		{
			return *reinterpret_cast<float*>(uint32_t(this) + OFFSETS::m_flNextAttack);
		}
		int GetNumAnimOverlays()
		{
			return (int)((DWORD)this + 0x298C);
		}
		void InvalidateBoneCache()
		{
			typedef void(__thiscall * OriginalFn)(void*);
			static auto func = (OriginalFn)UTILS::FindSignature("client_panorama.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");

			if (this && func)
				func(this);
		}
		bool SetupBones2(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
		{
			void* pRenderable = reinterpret_cast<void*>(uintptr_t(this) + 0x4);
			if (!pRenderable)
				false;

			typedef bool(__thiscall* Fn)(void*, matrix3x4_t*, int, int, float);
			return VMT::VMTHookManager::GetFunction<Fn>(pRenderable, 13)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
		}
		bool& m_bClientSideAnimation() { return *reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bClientSideAnimation); }
		Vector get_ragdoll_pos() {
			return *reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::get_ragdoll_pos);
		}
		HANDLE GetObserverTargetHandle()
		{
			return *(HANDLE*)((uintptr_t)this + OFFSETS::m_hObserverTarget);
		}
		int GetActiveWeaponIndex()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_hActiveWeapon) & 0xFFF;
		}
		float GetSimulationTime()
		{
			return *reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flSimulationTime);
		}
		float GetOldSimulationTime()
		{
			return *reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flSimulationTime + 0x4);
		}
		int GetArmor()
		{
			return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_ArmorValue);
		}
		void HealthShotBoostExpirationTime(float value)
		{
			static int m_flHealthShotBoostExpirationTime = UTILS::netvar_hook_manager.GetOffset("DT_CSPlayer", "m_flHealthShotBoostExpirationTime");
			*reinterpret_cast<float*>(uintptr_t(this) + m_flHealthShotBoostExpirationTime) = value;
		}
		bool HasHelmet()
		{
			return *reinterpret_cast<bool*>(uintptr_t(this) + OFFSETS::m_bHasHelmet);
		}
		int GetOwner()
		{
			return *(int*)((uintptr_t)this + OFFSETS::m_hOwner);
		}
		int GetOwnerEnt()
		{
			return *(int*)((uintptr_t)this + OFFSETS::m_hOwnerEnt);
		}
		char* GetArmorName()
		{
			if (GetArmor() > 0)
			{
				if (HasHelmet())
					return "hk";
				else
					return "k";
			}
			else
				return " ";
		}
		void SetLowerBodyYaw(float value)
		{
			static int m_flLowerBodyYawTarget = UTILS::netvar_hook_manager.GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
			*reinterpret_cast<float*>(uintptr_t(this) + m_flLowerBodyYawTarget) = value;
		}
		int GetOwnerWep()
		{
			return *(int*)((uintptr_t)this + OFFSETS::m_hOwnerEnt);
		}


		float GetLastShotTime()
		{
			static int m_fLastShotTime = UTILS::netvar_hook_manager.GetOffset("DT_WeaponCSBase", "m_fLastShotTime");
			return *reinterpret_cast<float*>(uint32_t(this) + m_fLastShotTime);
		}
		Vector* CBaseEntity::GetEyeAnglesPointer()
		{
			return (Vector*)((uintptr_t)this + UTILS::netvar_hook_manager.GetOffset("DT_CSPlayer", "m_angEyeAngles"));
		}



		float MaxDelta(SDK::CBaseEntity* pEnt) {

			auto animstate = uintptr_t(pEnt->GetAnimState());

			float duckammount = *(float*)(animstate + 0xA4);
			float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

			float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

			float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001f) - 0.19999999f) * speedfraction;
			float unk2 = unk1 + 1.f;
			float unk3;

			if (duckammount > 0) {
				unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
			}

			unk3 = *(float*)(animstate + 0x334) * unk2;

			return unk3;
		}

		bool HasC4()
		{
			int iBombIndex = *(int*)(*(DWORD*)(OFFSETS::dwPlayerResource) + OFFSETS::m_iPlayerC4);
			if (iBombIndex == this->GetIndex())
				return true;
			else
				return false;
		}
		void SetAngle2(Vector wantedang) {
			typedef void(__thiscall* oSetAngle)(void*, const Vector &);
			static oSetAngle _SetAngle = (oSetAngle)((uintptr_t)UTILS::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
			_SetAngle(this, wantedang);
		}
		bool IsVisibleVector(Vector bone);
		bool IsVisible(int bone);
};

class CBaseViewModel : public CModelInfo
{
public:

	inline DWORD GetOwner() {

		return *(PDWORD)((DWORD)this + OFFSETS::m_hOwner);
	}

	inline int GetModelIndex() {

		return *(int*)((DWORD)this + OFFSETS::m_nModelIndex);
	}
};
}
