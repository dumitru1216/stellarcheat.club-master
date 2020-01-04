#include "..\includes.h"
#include "../UTILS/interfaces.h"
#include "../UTILS/offsets.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/CInput.h"
#include "../SDK/IClient.h"
#include "../SDK/CPanel.h"
#include "../UTILS/render.h"
#include "../SDK/ConVar.h"
#include "../SDK/CGlowObjectManager.h"
#include "../SDK/IEngine.h"
#include "../SDK/CTrace.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/ModelInfo.h"
#include "../SDK/ModelRender.h"
#include "../SDK/RenderView.h"
#include "../SDK/CTrace.h"
#include "../SDK/CViewSetup.h"
#include "../SDK/CGlobalVars.h"
#include "../UTILS/NetvarHookManager.h"
#include "../SDK/CBaseEntity.h"

#include "../SDK/RecvData.h"
#include "SkinChanger.h"
#include "StickerChanger.h"



#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
std::unordered_map<char*, char*> killIcons = {};
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
HANDLE worldmodel_handle;
SDK::CBaseWeapon* worldmodel;
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))

void xdSkinchanger()
{
	SDK::CBaseEntity *pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto weapons = pLocal->m_hMyWeapons();
	for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
	{
		SDK::CBaseEntity *pEntity = INTERFACES::ClientEntityList->GetClientEntityFromHandle(weapons[i]);
		if (pEntity)
		{
			SDK::CBaseWeapon* pWeapon = (SDK::CBaseWeapon*)pEntity;
			int weapon = pWeapon->GetItemDefenitionIndex();

			if (weapon == WEAPON_SSG08)
			{
				//*pWeapon->FallbackPaintKit() = 624;

				switch (Config::SkinChanger::Knifes::get().ssg08_skin)
				{
				case 0: break;
				case 1: *pWeapon->FallbackPaintKit() = 624; break;//dragon fire
				case 2: *pWeapon->FallbackPaintKit() = 269; break;//fuskia
				case 3: *pWeapon->FallbackPaintKit() = 102; break;//whiteout
				case 4: *pWeapon->FallbackPaintKit() = 253; break;//acid fade
				case 5: *pWeapon->FallbackPaintKit() = 409; break;//tiger thoot
				case 6: *pWeapon->FallbackPaintKit() = 38; break;//fade
				}

			}				
			else if (weapon == WEAPON_REVOLVER)
				*pWeapon->FallbackPaintKit() = 701;
			else if (weapon == WEAPON_SCAR20)
				*pWeapon->FallbackPaintKit() = 597;
			else if (weapon == WEAPON_G3SG1)
				*pWeapon->FallbackPaintKit() = 712;

			*pWeapon->OwnerXuidLow() = 0;
			*pWeapon->OwnerXuidHigh() = 0;
			*pWeapon->FallbackWear() = 0.001f;
			*pWeapon->ItemIDHigh() = 1;

		}
	}
}
int GetKnifeDefinitionIndex(int si)
{
	// 0 - Bayonet, 1 - Flip, 2 - Gut, 3 - Karambit, 4 - M9Bayonet, 5 - Huntsman, 6 - Falchion, 7 - Bowie, 8 - Butterfly, 9 - Daggers, 10 - Golden
	switch (si)
	{
	case 0:
		return 500;
	case 1:
		return 505;
	case 2:
		return 506;
	case 3:
		return 507;
	case 4:
		return 508;
	case 5:
		return 509;
	case 6:
		return 512;
	case 7:
		return 514;
	case 8:
		return 515;
	case 9:
		return 516;
	case 10:
		return 523;
	case 11:
		return 520;
	case 12:
		return 522;
	case 13:
		return 519;
	case 14:
		return 503;
	case 15:
		return 525;
	case 16:
		return 518;
	case 17:
		return 517;
	case 18:
		return 521;
	default:
		return -1;
	}

}

int GetCurrentKnifeModel(int currentKnife)
{
	switch (currentKnife)
	{
	case 0:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl"); //Bayo
	case 1:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl"); //Flip
	case 2:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl"); //Gut
	case 3:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl"); //Karambit
	case 4:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl"); //M9 Bayo
	case 5:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl"); //Huntsman
	case 6:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl"); //Falchion
	case 7:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl"); //Bowie
	case 8:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl"); //Butterfly
	case 9:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl"); //Daggers
	case 10:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl"); //talon
	case 11:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl"); //navaja
	case 12:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl"); //stiletto
	case 13:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl"); //ursus
	case 14:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_css.mdl"); //golden knife
	case 15:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_skeleton.mdl"); //golden knife
	case 16:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_canis.mdl"); //golden knife
	case 17:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_cord.mdl"); //golden knife
	case 18:
		return INTERFACES::ModelInfo->GetModelIndex("models/weapons/v_knife_outdoor.mdl"); //golden knife
	default:
		return 0;
	}
}
void KnifeChanger() {
	SDK::CBaseEntity *pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto weapons = pLocal->m_hMyWeapons();
	for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
	{
		SDK::CBaseEntity *pEntity = INTERFACES::ClientEntityList->GetClientEntityFromHandle(weapons[i]);
		if (pEntity)
		{
			SDK::CBaseWeapon* pWeapon = (SDK::CBaseWeapon*)pEntity;
			if (Config::SkinChanger::Knifes::get().knifemodel > 0)
			{
				if (Config::SkinChanger::Knifes::get().knifemodel > 0)
				{
					if (pEntity->GetClientClass()->m_ClassID)
					{
						if (pEntity->GetClientClass()->m_ClassID == (int)CSGOClassID::CKnife)
						{
							worldmodel_handle = pWeapon->m_hWeaponWorldModel();
							if (worldmodel_handle) worldmodel = (SDK::CBaseWeapon*)INTERFACES::ClientEntityList->GetClientEntityFromHandle(worldmodel_handle);

							*pWeapon->GetEntityQuality() = 3;

							*pWeapon->ModelIndex() = GetCurrentKnifeModel(Config::SkinChanger::Knifes::get().knifemodel - 1); // m_nModelIndex
							*pWeapon->ViewModelIndex() = GetCurrentKnifeModel(Config::SkinChanger::Knifes::get().knifemodel - 1);
							if (worldmodel) *worldmodel->ModelIndex() = GetCurrentKnifeModel(Config::SkinChanger::Knifes::get().knifemodel - 1) + 1;
							*pWeapon->fixskins() = GetKnifeDefinitionIndex(Config::SkinChanger::Knifes::get().knifemodel - 1);

						}
					}
				}


				*pWeapon->OwnerXuidLow() = 0;
				*pWeapon->OwnerXuidHigh() = 0;
				*pWeapon->FallbackWear() = 0.001f;
				*pWeapon->ItemIDHigh() = 1;
			}
		}

	}
}