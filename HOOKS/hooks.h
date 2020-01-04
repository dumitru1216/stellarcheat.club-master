#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "..\SDK\IEngineSound.h"
#include "..\SDK\IMaterial.h"
#include "../UTILS/qangle.h"

namespace SDK
{
	class IMatRenderContext;
	class Ray_t;
	class ITraceFilter;

	struct CViewSetup;
	struct DrawModelState_t;
	struct ModelRenderInfo_t;
	struct trace_t;
}

class C_TEFireBullets
{
public:
	char pad[12];
	int		m_iPlayer; //12
	int _m_iItemDefinitionIndex;
	Vector	_m_vecOrigin;
	QAngle	m_vecAngles;
	int		_m_iWeaponID;
	int		m_iMode;
	int		m_iSeed;
	float	m_flSpread;
};

enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED
};


namespace HOOKS
{

	typedef void(__thiscall* FireBullets_t)(C_TEFireBullets*, DataUpdateType_t);
	typedef bool(__stdcall* CreateMoveFn)(float , SDK::CUserCmd*);
	typedef void(__thiscall* FrameStageNotifyFn)(void*, int);
	typedef void(__thiscall* PaintFn)(void*, void*, int);
	typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
	typedef void(__thiscall* DoPostScreenEffects_t)(void*,SDK::CViewSetup* pSetup);
	typedef void(__thiscall* DrawModelExecuteFn)(void*, SDK::IMatRenderContext*, const SDK::DrawModelState_t&, const SDK::ModelRenderInfo_t&, matrix3x4_t*);
	typedef void(__thiscall* SceneEndFn)(void*);
	typedef void(__thiscall* TraceRayFn)(void *thisptr, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter* pTraceFilter, SDK::trace_t *pTrace);
	typedef void(__thiscall* OverrideViewFn)(void* ecx, SDK::CViewSetup* pSetup);
	typedef void(__thiscall* RenderViewFn)(void*, SDK::CViewSetup&, SDK::CViewSetup&, unsigned int nClearFlags, int whatToDraw);
	typedef int(__thiscall* SendDatagramFn)(void*, void*);
	typedef bool(__thiscall* SetupBonesFn)(void*, matrix3x4_t*, int, int, float);
	typedef bool(__thiscall* SvCheatsGetBoolFn)(void*);
	typedef float(__thiscall* GetViewmodelFOVFn)();
	typedef float(__thiscall* EngineFN)();
	typedef float (__thiscall* ScreenAspectRatio)(void* pEcx, void* pEdx, int32_t iWidth, int32_t iHeight);
	typedef SDK::IMaterial*(__fastcall* FindMaterialFn)(void*, const char*, const char*, bool, const char*);
	typedef void(*EmitSound1Fn) (void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int, float, int, int, const Vector*, const Vector*, void*, bool, float, int);
	typedef void(*EmitSound2Fn) (void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int, soundlevel_t, int, int, const Vector*, const Vector*, void*, bool, float, int);

	typedef long(__stdcall *EndSceneFn)(IDirect3DDevice9* device);
	typedef long(__stdcall *EndSceneResetFn)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	typedef long(__stdcall *hooked_presentFn)(IDirect3DDevice9* device, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);

	extern CreateMoveFn original_create_move;
	extern PaintFn original_paint;
	extern PaintTraverseFn original_paint_traverse;
	extern FrameStageNotifyFn original_frame_stage_notify;
	extern DrawModelExecuteFn original_draw_model_execute;
	extern SceneEndFn original_scene_end;
	extern TraceRayFn original_trace_ray;
	extern SendDatagramFn original_send_datagram;
	extern OverrideViewFn original_override_view;
	extern RenderViewFn original_render_view;
	extern SvCheatsGetBoolFn original_get_bool;
	extern SetupBonesFn original_setup_bones;
	extern GetViewmodelFOVFn original_viewmodel_fov;
	extern FindMaterialFn original_find_material;
	extern FireBullets_t o_FireBullets;
	extern ScreenAspectRatio original_screen_aspect_ratio;
	extern DoPostScreenEffects_t orginal_post_screen_effect;

	extern VMT::VMTHookManager iclient_hook_manager;
	extern VMT::VMTHookManager panel_hook_manager;
	extern VMT::VMTHookManager paint_hook_manager;
	extern VMT::VMTHookManager model_render_hook_manager;
	extern VMT::VMTHookManager scene_end_hook_manager;
	extern VMT::VMTHookManager render_view_hook_manager;
	extern VMT::VMTHookManager trace_hook_manager;
	extern VMT::VMTHookManager screen_aspect_ratio;
	extern VMT::VMTHookManager net_channel_hook_manager;
	extern VMT::VMTHookManager override_view_hook_manager;
	extern VMT::VMTHookManager input_table_manager;
	extern VMT::VMTHookManager get_bool_manager;
	extern VMT::VMTHookManager sound_hook_manager;
	extern VMT::VMTHookManager find_material_hook_manager;
	extern VMT::VMTHookManager setup_bones_hook_manager;
	extern VMT::VMTHookManager post_screen_effect;

	void InitHooks();

	// virtual function hooks
	bool __stdcall   HookedCreateMove(float sample_input_frametime, SDK::CUserCmd* cmd);
	void __fastcall Hooked_Postscreeneffects(void* thisptr, void* edx, SDK::CViewSetup* setup);
	void __fastcall  HookedFrameStageNotify(void*, void*, int);
	void __stdcall   HookedPaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce);
	void __fastcall  HookedDrawModelExecute(void*, void*, SDK::IMatRenderContext*, const SDK::DrawModelState_t&, const SDK::ModelRenderInfo_t&, matrix3x4_t*);
	void __fastcall  HookedSceneEnd(void*, void*);
	void __fastcall  HookedTraceRay(void *thisptr, void*, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter *pTraceFilter, SDK::trace_t *pTrace);
	//void __fastcall  HookedSendDatagram(void* ecx, void* data);
	void __fastcall  HookedOverrideView(void* ecx, void* edx, SDK::CViewSetup* pSetup);
	void __fastcall  HookedRenderView(void* thisptr, void*, SDK::CViewSetup& setup, SDK::CViewSetup& hudViewSetup, unsigned int nClearFlags, int whatToDraw);
	float __fastcall Hook_GetScreenAspectRatio(void* pEcx, void* pEdx, int32_t iWidth, int32_t iHeight);
	bool __fastcall  HookedGetBool(void* pConVar, void* edx);
	void __stdcall FireBullets_PostDataUpdate(C_TEFireBullets* thisptr, DataUpdateType_t updateType);
	float __fastcall Hook_GetScreenAspectRatio(void* pEcx, void* pEdx, int32_t iWidth, int32_t iHeight);
	SDK::IMaterial* __fastcall hooked_findmaterial(void* thisptr, void*, const char* pMaterialName, const char* pTextureGroupName, bool complain, const char* pComplainPrefix);
	float __fastcall GetViewmodelFOV();
	void EmitSound1(void* thisptr, IRecipientFilter& filter, int iGetIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity);
	void EmitSound2(void* thisptr, IRecipientFilter& filter, int iGetIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, int nSeed, soundlevel_t iSoundLevel, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity);
	void hookKnife();
	// netvar hooks
	void InitNetvarHooks();
}
namespace RecVarShit
{
	void hookKnife();

}

class CMenu
{
public:
	void Menu();

private:
	void DrawMenu();

	void MenuStyle();
	void SetupVariables();
	void CheckForResolutionChange();

	void RagebotTab();
	void LegitbotTab();

	void VisualsTab();
	void MiscTab();
	void SkinsTab();
	void LuaTab();
	void ConfigTab();
	void SPEC();
	void radioTab();



	/*MENU VARS*/

	int currentTab = 0;
	int curntr = 0;
	int curntv = 0;
	int curntm = 0;
	int curntra = 0;
	int curntaa = 0;
	int curntvm = 0;
	int curntvch = 0;
	int curntvmi = 0;
	int curntc = 0;
	//?Visuals tab?//
	int tabvisualsmain = 0;
	int tabvisualsmainplayers = 0;

	//END
	//? MENU ?//
	int tabrb;
	int tabv;
	int tabm;
	int tabsk;
	int tablegit;
	//END
	bool rt = 1;
	bool vt = 1;
	bool p = 0;
	bool mt = 1;
	bool st = 1;
	int menuTabStyle = 0;

	float rainbow = 0.0f;



	struct menuVars {
		//this determines the size of the menu
		Vector2D scale = Vector2D(0.4, 0.5);

		Vector2D screenSize;
		Vector2D size;
		Vector2D position;

		float alpha = 0.0f;
		float alphaSpeed = 0.02f;
		bool set = false;
	}menuVars;

	struct tabMenuVars {
		Vector2D size;
		Vector2D position;
		float speed = 5.0f;
	}tabMenuVars;

}; 
extern CMenu* menu;

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);