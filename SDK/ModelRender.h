#pragma once

inline void**& getvtable(void* inst, size_t offset = 0)
{
	return *reinterpret_cast<void***>((size_t)inst + offset);
}

inline const void** getvtable(const void* inst, size_t offset = 0)
{
	return *reinterpret_cast<const void***>((size_t)inst + offset);
}

template< typename Fn >

inline Fn getvfunc(const void* inst, size_t index, size_t offset = 0)
{
	return reinterpret_cast<Fn>(getvtable(inst, offset)[index]);
}

namespace SDK
{
	typedef unsigned short ModelInstanceHandle_t;
	class IMaterial;
	class IMatRenderContext;
	class IClientRenderable;
	struct model_t;
	struct studiohdr_t;
	struct studiohwdata_t;
	struct StudioDecalHandle_t { int unused; };

	struct DrawModelState_t
	{
		studiohdr_t*			m_pStudioHdr;
		studiohwdata_t*			m_pStudioHWData;
		IClientRenderable*		m_pRenderable;
		const matrix3x4_t		*m_pModelToWorld;
		StudioDecalHandle_t		m_decals;
		int						m_drawFlags;
		int						m_lod;
	};

	enum OverrideType_t
	{
		OVERRIDE_NORMAL = 0,
		OVERRIDE_BUILD_SHADOWS,
		OVERRIDE_DEPTH_WRITE,
	};

	/*struct ModelRenderInfo_t
	{
		Vector origin;
		Vector angles;
		IClientRenderable *pRenderable;
		const model_t *pModel;
		const matrix3x4_t *pModelToWorld;
		const matrix3x4_t *pLightingOffset;
		const Vector *pLightingOrigin;
		int flags;
		int entity_index;
		int skin;
		int body;
		int hitboxset;
		ModelInstanceHandle_t instance;

		ModelRenderInfo_t()
		{
			pModelToWorld = NULL;
			pLightingOffset = NULL;
			pLightingOrigin = NULL;
		}

	};*/

	//--- 3/30/18 ---//
	using model_instance_handle_t = uint32_t;
	struct zmodel_t {
		void* handle;
		char name[255];
	};

	struct ModelRenderInfo_t 
	{
		Vector origin;
		QAngle angles;
		char pad[0x4];
		void* renderable;
		const zmodel_t* model;
		const matrix3x4_t* model_to_world;
		const matrix3x4_t* lighting_offset;
		const Vector* lighting_origin;
		int flags;
		int entity_index;
		int skin;
		int body;
		int hitboxset;
		model_instance_handle_t instance;
	};

	class CModelRender
	{
	public:
		void DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
		{
			typedef void(__thiscall* DrawModelExecuteFn)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
			VMT::VMTHookManager::GetFunction<DrawModelExecuteFn>(this, 21)(this, ctx, state, pInfo, pCustomBoneToWorld);
		}
		void ForcedMaterialOverride(IMaterial *newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL)
		{
			typedef void(__thiscall* OriginalFn)(void*, IMaterial*, OverrideType_t, int);
			VMT::VMTHookManager::GetFunction<OriginalFn>(this, 1)(this, newMaterial, nOverrideType, NULL);
		}
		bool IsForcedMaterialOverride() {
			typedef bool(__thiscall* oForce)(PVOID);
			return getvfunc<oForce>(this, 2)(this);
		}

	};
}