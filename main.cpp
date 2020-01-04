#include "includes.h"
#include "BASS\bass.h"
#include "UTILS\interfaces.h"
#include "HOOKS\hooks.h"
#include "UTILS\offsets.h"
#include "SDK\RecvData.h"
#include "SDK\CClientEntityList.h"
#include "SDK\IEngine.h"
#include "FEATURES/recv.h"
#include "AWHitmarkers.h"
#include "FEATURES/NewEventLog.h"
#include <chrono>
#include <thread>
#include <map>
#include "SDK/ConVar.h"
#include "SDK/IClient.h"
#include <string>
#include <sstream>
#include "radionstuff.h"

#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <iostream>
#include <sapi.h>
using namespace std::literals::chrono_literals;

bool using_fake_angles[65];
bool full_choke;
bool is_shooting;

bool in_tp;
bool fake_walk;
bool in_slowalk;
bool flipaa;

int resolve_type[65];

int target;
int shots_fired[65];
int shots_hit[65];
int shots_missed[65];
bool didMiss = true;
bool didShot = false;
int backtrack_missed[65];

float tick_to_back[65];
float lby_to_back[65];
bool backtrack_tick[65];

float lby_delta;
float update_time[65];
float walking_time[65];

float local_update;

int hitmarker_time;
int random_number;

bool menu_hide;

int oldest_tick[65];
float compensate[65][12];
Vector backtrack_hitbox[65][20][12];
float backtrack_simtime[65][12];
bool fake = false;
DWORD stream;

HMODULE get_module_handle(wchar_t* name) {

	static std::map<wchar_t*, HMODULE> modules = { };

	if (modules.count(name) == 0)
		modules[name] = GetModuleHandleW(name);

	return modules[name];
}


void patchLogs() {

	if (!INTERFACES::Engine->Con_BoneIsVisible()) //seems to cause crashes, so commented it. Use it if you want
		INTERFACES::Engine->ExecuteClientCmd("toggleconsole");
	INTERFACES::Engine->ExecuteClientCmd("clear");
	INTERFACES::cvar->ConsoleColorPrintf(CColor(255, 20, 255), "\n[zigaDEV's hook] ");
	INTERFACES::cvar->ConsoleColorPrintf(CColor(255, 255, 255), "Welcome back, ");
	INTERFACES::cvar->ConsoleColorPrintf(CColor(255, 255, 255), "Hvh trap");
	INTERFACES::cvar->ConsoleColorPrintf(CColor(255, 255, 255), "\n");

}
void radio()
{
	BASS::bass_lib_handle = BASS::bass_lib.LoadFromMemory(bass_dll_image, sizeof(bass_dll_image));
	static bool once;
	if (!once)
	{
		if (BASS_Init(-1, 44100, 0, nullptr, nullptr))
		{
			BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 1);
			BASS_SetConfig(BASS_CONFIG_NET_PREBUF, 0);
			once = true;
		}
	}
}
void Start() {

	global::cheat_start_time = GetTickCount() * 0.001f;
	INTERFACES::InitInterfaces();
	OFFSETS::InitOffsets();
	NetvarHook();
	UTILS::INPUT::input_handler.Init();
	pHitmarker->Initialize();
	FONTS::InitFonts();
	HOOKS::InitHooks();
	HOOKS::InitNetvarHooks();
	FEATURES::MISC::InitializeEventListeners();

	radio();
}
std::string username;
std::string password;
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {

	switch (dwReason)
	{

	case DLL_PROCESS_ATTACH:
	{
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		std::cout << green << "Hack is curntly in development! " << "DATE:" << __DATE__ << __TIME__ << "\n" ;
		/*std::cout << white << "Type your username: \n";
		std::getline(std::cin, username);
		std::cout << white << "Type your password: \n";
		std::getline(std::cin, password);
		if (username == "ziga" && password == "test")
		*/
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Start, NULL, NULL, NULL);
		//else
		//	std::exit(0);
	
	}
	break;
	case DLL_PROCESS_DETACH:
	{
		LOG("DETACHING");
	}
	break;
	}

	return true;
}