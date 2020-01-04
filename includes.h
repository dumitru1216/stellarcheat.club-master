#pragma once

#include <Windows.h>
#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <assert.h>
#include <Psapi.h>
#include <thread>
#include <chrono>

#define DEV_MODE 1

#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)), 'A', 'F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x, '0', '9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

#define IN_ATTACK					(1 << 0)
#define IN_JUMP						(1 << 1)
#define IN_DUCK						(1 << 2)
#define IN_FORWARD					(1 << 3)
#define IN_BACK						(1 << 4)
#define IN_USE						(1 << 5)
#define IN_CANCEL					(1 << 6)
#define IN_LEFT						(1 << 7)
#define IN_RIGHT					(1 << 8)
#define IN_MOVELEFT					(1 << 9)
#define IN_MOVERIGHT				(1 << 10)
#define IN_ATTACK2					(1 << 11)
#define IN_RUN						(1 << 12)
#define IN_RELOAD					(1 << 13)
#define IN_ALT1						(1 << 14)
#define IN_ALT2						(1 << 15)
#define IN_SCORE					(1 << 16)   
#define IN_SPEED					(1 << 17)	
#define IN_WALK						(1 << 18)	
#define IN_ZOOM						(1 << 19)	
#define IN_WEAPON1					(1 << 20)	
#define IN_WEAPON2					(1 << 21)
#define IN_BULLRUSH					(1 << 22)
#define IN_GRENADE1					(1 << 23)
#define IN_GRENADE2					(1 << 24)	
#define	IN_ATTACK3					(1 << 25)

#define	FL_ONGROUND					(1 << 0)	
#define FL_DUCKING					(1 << 1)	
#define	FL_WATERJUMP				(1 << 2)	
#define FL_ONTRAIN					(1 << 3)
#define FL_INRAIN					(1 << 4)	
#define FL_FROZEN					(1 << 5)
#define FL_ATCONTROLS				(1 << 6)
#define	FL_CLIENT					(1 << 7)	
#define FL_FAKECLIENT				(1 << 8)	
#define	FL_INWATER					(1 << 9)										
#define	FL_FLY						(1 << 10)	
#define	FL_SWIM						(1 << 11)
#define	FL_CONVEYOR					(1 << 12)
#define	FL_NPC						(1 << 13)
#define	FL_GODMODE					(1 << 14)
#define	FL_NOTARGET					(1 << 15)
#define	FL_AIMTARGET				(1 << 16)	
#define	FL_PARTIALGROUND			(1 << 17)	
#define FL_STATICPROP				(1 << 18)
#define FL_GRAPHED					(1 << 19) 
#define FL_GRENADE					(1 << 20)
#define FL_STEPMOVEMENT				(1 << 21)	
#define FL_DONTTOUCH				(1 << 22)
#define FL_BASEVELOCITY				(1 << 23)
#define FL_WORLDBRUSH				(1 << 24)	
#define FL_OBJECT					(1 << 25) 
#define FL_KILLME					(1 << 26)
#define FL_ONFIRE					(1 << 27)	
#define FL_DISSOLVING				(1 << 28)
#define FL_TRANSRAGDOLL				(1 << 29)
#define FL_UNBLOCKABLE_BY_PLAYER	(1 << 30) 

#define PLAYER_FLAG_BITS			10

#define DISPSURF_FLAG_SURFACE		(1<<0)
#define DISPSURF_FLAG_WALKABLE		(1<<1)
#define DISPSURF_FLAG_BUILDABLE		(1<<2)
#define DISPSURF_FLAG_SURFPROP1		(1<<3)
#define DISPSURF_FLAG_SURFPROP2		(1<<4)

#define HITGROUP_GENERIC		0
#define HITGROUP_HEAD			1
#define HITGROUP_CHEST			2
#define HITGROUP_STOMACH		3
#define HITGROUP_LEFTARM		4    
#define HITGROUP_RIGHTARM		5
#define HITGROUP_LEFTLEG		6
#define HITGROUP_RIGHTLEG		7
#define HITGROUP_GEAR			10
#define SURF_HITBOX				0x8000

#define MAX_STUDIOBONES			128
#define MAXSTUDIOSKINS			32

#define	SURF_SKY				0x0004	
#define BONE_USED_BY_HITBOX		0x00000100
#define BONE_USED_BY_ANYTHING   0x0007FF00

#define TIME_TO_TICKS(dt)		((int)(0.5 + (float)(dt) / INTERFACES::Globals->interval_per_tick))
#define TICKS_TO_TIME(t)		(INTERFACES::Globals->interval_per_tick * (t))

#include "UTILS\vmt.h"
#include "UTILS\vector3D.h"
#include "UTILS\vector2D.h"
#include "UTILS\vmatrix.h"
#include "UTILS\quaternion.h"
#include "UTILS\math.h"
#include "UTILS\general_utils.h"
#include "UTILS\color.h"
#include "UTILS\input_handler.h"
#include "UTILS\variables.h"
#include "HOOKS\menu\Menu-Fonts.h"
#include "UTILS\playerinclude.h"
#include "HOOKS\menu\config\config.h"
#include "HOOKS\menu\Menu-Draw.h"
#include "ConsoleColor.h"

enum ClientFrameStage_t {

	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};
enum ItemDefinitionIndex
{
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALIL = 13,
	WEAPON_M249,
	WEAPON_M4A4 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_ZEUS,
	WEAPON_P2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG553,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INC,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1S,
	WEAPON_USPS,
	WEAPON_CZ75 = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035,
	WEAPON_SG556 = 5039,
	WEAPON_KNIFE_BOWIE,
	/**/
	CMolotovGrenade = 110,
	WEAPON_CPlantedC4 = 126,
	WEAPON_CC4 = 127,
	CSmokeGrenade = 152
};

enum class CSGOClassID
{
	CAI_BaseNPC = 0,
	CAK47,
	CBaseAnimating,
	CBaseAnimatingOverlay,
	CBaseAttributableItem,
	CBaseButton,
	CBaseCombatCharacter,
	CBaseCombatWeapon,
	CBaseCSGrenade,
	CBaseCSGrenadeProjectile,
	CBaseDoor,
	CBaseEntity,
	CBaseFlex,
	CBaseGrenade,
	CBaseParticleEntity,
	CBasePlayer,
	CBasePropDoor,
	CBaseTeamObjectiveResource,
	CBaseTempEntity,
	CBaseToggle,
	CBaseTrigger,
	CBaseViewModel,
	CBaseVPhysicsTrigger,
	CBaseWeaponWorldModel,
	CBeam,
	CBeamSpotlight,
	CBoneFollower,
	CBRC4Target,
	CBreachCharge,
	CBreachChargeProjectile,
	CBreakableProp,
	CBreakableSurface,
	CBumpMine,
	CBumpMineProjectile,
	CC4,
	CCascadeLight,
	CChicken,
	CColorCorrection,
	CColorCorrectionVolume,
	CCSGameRulesProxy,
	CCSPlayer,
	CCSPlayerResource,
	CCSRagdoll,
	CCSTeam,
	CDangerZone,
	CDangerZoneController,
	CDEagle,
	CDecoyGrenade,
	CDecoyProjectile,
	CDrone,
	CDronegun,
	CDynamicLight,
	CDynamicProp,
	CEconEntity,
	CEconWearable,
	CEmbers,
	CEntityDissolve,
	CEntityFlame,
	CEntityFreezing,
	CEntityParticleTrail,
	CEnvAmbientLight,
	CEnvDetailController,
	CEnvDOFController,
	CEnvGasCanister,
	CEnvParticleScript,
	CEnvProjectedTexture,
	CEnvQuadraticBeam,
	CEnvScreenEffect,
	CEnvScreenOverlay,
	CEnvTonemapController,
	CEnvWind,
	CFEPlayerDecal,
	CFireCrackerBlast,
	CFireSmoke,
	CFireTrail,
	CFish,
	CFists,
	CFlashbang,
	CFogController,
	CFootstepControl,
	CFunc_Dust,
	CFunc_LOD,
	CFuncAreaPortalWindow,
	CFuncBrush,
	CFuncConveyor,
	CFuncLadder,
	CFuncMonitor,
	CFuncMoveLinear,
	CFuncOccluder,
	CFuncReflectiveGlass,
	CFuncRotating,
	CFuncSmokeVolume,
	CFuncTrackTrain,
	CGameRulesProxy,
	CGrassBurn,
	CHandleTest,
	CHEGrenade,
	CHostage,
	CHostageCarriableProp,
	CIncendiaryGrenade,
	CInferno,
	CInfoLadderDismount,
	CInfoMapRegion,
	CInfoOverlayAccessor,
	CItem_Healthshot,
	CItemCash,
	CItemDogtags,
	CKnife,
	CKnifeGG,

	CLightGlow,

	CMaterialModifyControl,

	CMelee,

	CMolotovGrenade,

	CMolotovProjectile,

	CMovieDisplay,

	CParadropChopper,

	CParticleFire,

	CParticlePerformanceMonitor,

	CParticleSystem,

	CPhysBox,

	CPhysBoxMultiplayer,

	CPhysicsProp,

	CPhysicsPropMultiplayer,

	CPhysMagnet,

	CPhysPropAmmoBox,

	CPhysPropLootCrate,

	CPhysPropRadarJammer,

	CPhysPropWeaponUpgrade,

	CPlantedC4,

	CPlasma,

	CPlayerPing,

	CPlayerResource,

	CPointCamera,

	CPointCommentaryNode,

	CPointWorldText,

	CPoseController,

	CPostProcessController,

	CPrecipitation,

	CPrecipitationBlocker,

	CPredictedViewModel,

	CProp_Hallucination,

	CPropCounter,

	CPropDoorRotating,

	CPropJeep,

	CPropVehicleDriveable,

	CRagdollManager,

	CRagdollProp,

	CRagdollPropAttached,

	CRopeKeyframe,

	CSCAR17,

	CSceneEntity,

	CSensorGrenade,

	CSensorGrenadeProjectile,

	CShadowControl,

	CSlideshowDisplay,

	CSmokeGrenade,

	CSmokeGrenadeProjectile,

	CSmokeStack,

	CSnowball,

	CSnowballPile,

	CSnowballProjectile,

	CSpatialEntity,

	CSpotlightEnd,

	CSprite,

	CSpriteOriented,

	CSpriteTrail,

	CStatueProp,

	CSteamJet,

	CSun,

	CSunlightShadowControl,

	CSurvivalSpawnChopper,

	CTablet,

	CTeam,

	CTeamplayRoundBasedRulesProxy,

	CTEArmorRicochet,

	CTEBaseBeam,

	CTEBeamEntPoint,

	CTEBeamEnts,

	CTEBeamFollow,

	CTEBeamLaser,

	CTEBeamPoints,

	CTEBeamRing,

	CTEBeamRingPoint,

	CTEBeamSpline,

	CTEBloodSprite,

	CTEBloodStream,

	CTEBreakModel,

	CTEBSPDecal,

	CTEBubbles,

	CTEBubbleTrail,

	CTEClientProjectile,

	CTEDecal,

	CTEDust,

	CTEDynamicLight,

	CTEEffectDispatch,

	CTEEnergySplash,

	CTEExplosion,

	CTEFireBullets,

	CTEFizz,

	CTEFootprintDecal,

	CTEFoundryHelpers,

	CTEGaussExplosion,

	CTEGlowSprite,

	CTEImpact,

	CTEKillPlayerAttachments,

	CTELargeFunnel,

	CTEMetalSparks,

	CTEMuzzleFlash,

	CTEParticleSystem,

	CTEPhysicsProp,

	CTEPlantBomb,

	CTEPlayerAnimEvent,

	CTEPlayerDecal,

	CTEProjectedDecal,

	CTERadioIcon,

	CTEShatterSurface,

	CTEShowLine,

	CTesla,

	CTESmoke,

	CTESparks,

	CTESprite,

	CTESpriteSpray,

	CTest_ProxyToggle_Networkable,

	CTestTraceline,

	CTEWorldDecal,

	CTriggerPlayerMovement,

	CTriggerSoundOperator,

	CVGuiScreen,

	CVoteController,

	CWaterBullet,

	CWaterLODControl,

	CWeaponAug,

	CWeaponAWP,

	CWeaponBaseItem,

	CWeaponBizon,

	CWeaponCSBase,

	CWeaponCSBaseGun,

	CWeaponCycler,

	CWeaponElite,

	CWeaponFamas,

	CWeaponFiveSeven,

	CWeaponG3SG1,

	CWeaponGalil,

	CWeaponGalilAR,

	CWeaponGlock,

	CWeaponHKP2000,

	CWeaponM249,

	CWeaponM3,

	CWeaponM4A1,

	CWeaponMAC10,

	CWeaponMag7,

	CWeaponMP5Navy,

	CWeaponMP7,

	CWeaponMP9,

	CWeaponNegev,

	CWeaponNOVA,

	CWeaponP228,

	CWeaponP250,

	CWeaponP90,

	CWeaponSawedoff,

	CWeaponSCAR20,

	CWeaponScout,

	CWeaponSG550,

	CWeaponSG552,

	CWeaponSG556,

	CWeaponShield,

	CWeaponSSG08,

	CWeaponTaser,

	CWeaponTec9,

	CWeaponTMP,

	CWeaponUMP45,

	CWeaponUSP,

	CWeaponXM1014,

	CWorld,

	CWorldVguiText,

	DustTrail,

	MovieExplosion,

	ParticleSmokeGrenade,

	RocketTrail,

	SmokeTrail,

	SporeExplosion,

	SporeTrail,
};