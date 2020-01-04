#include "Menu.h"
#include "../../includes.h"
#include "../../UTILS/render.h"
#include <iostream>
#include "../../singleton.h"
//#include "Menu-Draw.h"
#include "../../XorStr.hpp"
#include "../../SDK/CInput.h"
#include "../../SDK/CClientEntityList.h"
#include "../../SDK/IEngine.h"
#include "../../SDK/CInputSystem.h"
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
Menu g_Menu;
bool manually_set_key = false;
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
/*
*
*     CREDITS TO : people from xy0 / p$ilent
*
*	based on their framework, with some modifications to pretty much everything ;)
*
*/
enum TABS
{
	Aimbot = 0,
	Visuals = 1,
	Misc = 2
};

#define or ||
#define and &&
int TopLeftGroupBox_X = 5;
int TopLeftGroupBox_Y = 52 + (309 / 1.5) + 5;

int GroupBoxSize_Width = 170;
int GroupBoxSize_Height = 455 /*- 6*//*this is for if i want the bottom text to show not under the groupbox*/;
int _width = 694; //514
int _height = 558; //437

void GradientV(int x, int y, int w, int h, CColor c1, CColor c2)
{
	RENDER::FillRectangle(x, y, w, h, c1);
	BYTE first = c2.RGBA[0];
	BYTE second = c2.RGBA[1];
	BYTE third = c2.RGBA[2];
	for (int i = 0; i < h; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		RENDER::FillRectangle(x, y + i, w, 1, CColor(first, second, third, ia));
	}
}

void GradientVEnd(int x, int y, int w, int h, CColor c1, CColor c2)
{
	//RENDER::FillRectangle(x, y, w, h, c1);
	BYTE first = c2.RGBA[0];
	BYTE second = c2.RGBA[1];
	BYTE third = c2.RGBA[2];
	for (float i = 0; i < h / 1.5; i++)
	{
		float fi = i, fh = h / 1.5;
		float a = fi / fh;
		DWORD ia = a * 255;
		RENDER::FillRectangle(x, y + i, w, 2, CColor(first, second, third, ia));
	}
}


void DrawESPPlayersSubTab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main)
{
	int in_sizew_esp_player_subtabs = (375 / 1.5) - 8;
	static std::string ESP_Player_SubTabs_Names[3] = { "Player", "World", "Model" };

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size2 = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, ESP_Player_SubTabs_Names[i].c_str());

		int tab_area[4] = {
			_pos.x + 9 + (i * (in_sizew_esp_player_subtabs / tab_amount)), _pos.y + 52 + 5,
			(in_sizew_esp_player_subtabs / tab_amount), 20
		};

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(tab_area[0], tab_area[1], tab_area[2],
			tab_area[3]))
			current_players_esp_subtab = i;

		if (current_players_esp_subtab == i)
		{
			//RENDER::FillRectangle(tab_area[0], tab_area[1], tab_area[2], tab_area[3], CColor(37, 37, 45, MenuAlpha_Main));

			RENDER::FillRectangle(tab_area[0] + 3, tab_area[1] + tab_area[3] + 1.5, tab_area[2] - 6, 1.5, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main));

			MENU_DRAW::DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
		else
		{
			//RENDER::FillRectangle(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(27, 27, 35, MenuAlpha_Main));
			//RENDER::FillRectangle(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3, Color(39, 39, 47, MenuAlpha_Main));

			MENU_DRAW::DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
	}
}

void DrawAimbotTargetSubTab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main)
{
	int in_sizew_esp_player_subtabs = (375 / 1.5) - 8;
	static std::string ESP_Player_SubTabs_Names[2] = { "Target", "Body-Aim" };

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size2 = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, ESP_Player_SubTabs_Names[i].c_str());

		int tab_area[4] = {
			_pos.x + 9 + (i * (in_sizew_esp_player_subtabs / tab_amount)), _pos.y + TopLeftGroupBox_Y + 5,
			(in_sizew_esp_player_subtabs / tab_amount), 20
		};

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(tab_area[0], tab_area[1], tab_area[2],
			tab_area[3]))
			current_players_esp_subtab = i;


		//	RENDER::FillRectangle(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(23, 23, 25, MenuAlpha_Main));

		if (current_players_esp_subtab == i)
		{

			//RENDER::FillRectangle(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(37, 37, 45, MenuAlpha_Main));

			RENDER::FillRectangle(tab_area[0] + 3, tab_area[1] + tab_area[3] + 1.5, tab_area[2] - 6, 1.5, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main));

			MENU_DRAW::DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
		else
		{
			//RENDER::FillRectangle(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(27, 27, 35, MenuAlpha_Main));
			//RENDER::FillRectangle(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3, Color(39, 39, 47, MenuAlpha_Main));

			MENU_DRAW::DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}

	}
} //

void DrawAimbotAccuracy_Subtab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main)
{
	int in_sizew_esp_player_subtabs = GroupBoxSize_Width - 8;
	static std::string ESP_Player_SubTabs_Names[5] = { "Auto", "Scout", "Awp", "H. Pistol", "Other" };

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size2 = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, ESP_Player_SubTabs_Names[i].c_str());

		int tab_area[4] = {
			_pos.x + TopLeftGroupBox_X + GroupBoxSize_Width + 5 + 4 + (i * (in_sizew_esp_player_subtabs / tab_amount)),
			_pos.y + 52 + 5, (in_sizew_esp_player_subtabs / tab_amount), 20
		};

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(tab_area[0], tab_area[1], tab_area[2],
			tab_area[3]))
			current_players_esp_subtab = i;

		if (current_players_esp_subtab == i)
		{
			//RENDER::FillRectangle(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(37, 37, 45, MenuAlpha_Main));

			RENDER::FillRectangle(tab_area[0] + 3, tab_area[1] + tab_area[3] + 1.5, tab_area[2] - 6, 1.5, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main));

			MENU_DRAW::DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
		else
		{
			//RENDER::FillRectangle(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(27, 27, 35, MenuAlpha_Main));
			//RENDER::FillRectangle(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3, Color(39, 39, 47, MenuAlpha_Main));

			MENU_DRAW::DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
	}
}

void DrawAntiAim_Subtab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main)
{
	int in_sizew_esp_player_subtabs = GroupBoxSize_Width - 8;
	static std::string ESP_Player_SubTabs_Names[4] = { "Standing", "Moving", "Air", "Misc" };

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size2 = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, ESP_Player_SubTabs_Names[i].c_str());

		int tab_area[4] = {
			_pos.x + 9 + (i * (in_sizew_esp_player_subtabs / tab_amount)), _pos.y + 65,
			(in_sizew_esp_player_subtabs / tab_amount), 20
		};

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(tab_area[0], tab_area[1], tab_area[2],
			tab_area[3]))
			current_players_esp_subtab = i;

		if (current_players_esp_subtab == i)
		{
			//RENDER::FillRectangle(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(37, 37, 45, MenuAlpha_Main));

			RENDER::FillRectangle(tab_area[0] + 3, tab_area[1] + tab_area[3] + 1.5, tab_area[2] - 6, 1.5, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main));

			MENU_DRAW::DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
		else
		{
			//RENDER::FillRectangle(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(27, 27, 35, MenuAlpha_Main));
			//RENDER::FillRectangle(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3, Color(39, 39, 47, MenuAlpha_Main));

			MENU_DRAW::DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
				ESP_Player_SubTabs_Names[i].c_str());
		}
	}
} //
#include <tuple>

class vars : public singleton< vars > {
private:

public:
	bool visuals_box = false;
};

void DrawRageMain_Subtab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main)
{
	int in_sizew_esp_player_subtabs = 483;
	static std::string ESP_Player_SubTabs_Names[10] = { "v", "G", "J", "L" , "S" , "Y" , "a" , "Z" , "e" , "f" };

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size2 = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, ESP_Player_SubTabs_Names[i].c_str());

		int tab_area[4] = {
			_pos.x + 41, _pos.y + 72 + (i * (in_sizew_esp_player_subtabs / tab_amount)),
			(in_sizew_esp_player_subtabs / tab_amount), text_size2.bottom + 8
		};

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(tab_area[0], tab_area[1], tab_area[2], tab_area[3]))
			current_players_esp_subtab = i;

		INTERFACES::Surface->DrawSetColor(CColor(255, 255, 255, 255));
		INTERFACES::Surface->DrawOutlinedRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3]);

		if (current_players_esp_subtab == i)
		{
			RENDER::FillRectangle(tab_area[0] + 67, tab_area[1], 1, tab_area[2] - 11, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main));
			RENDER::FillRectangle(tab_area[0] + 66, tab_area[1], 1, tab_area[2] - 11, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 115));

			MENU_DRAW::DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main), MENU_FONTS::weapontabicon, true,
				ESP_Player_SubTabs_Names[i].c_str());
		}
		else
		{
			//RENDER::FillRectangle(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(27, 27, 35, MenuAlpha_Main));
			//RENDER::FillRectangle(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3, Color(39, 39, 47, MenuAlpha_Main));

			MENU_DRAW::DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::weapontabicon, true,
				ESP_Player_SubTabs_Names[i].c_str());
		}

		//	MENU_DRAW::OutlinedRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(40, 44, 53, MenuAlpha_Main));
	}
} //

void DrawLegitMain_Subtab(int& current_players_esp_subtab, int tab_amount, Vector _pos, int MenuAlpha_Main)
{
	int in_sizew_esp_player_subtabs = 483;
	static std::string ESP_Player_SubTabs_Names[10] = { "v", "G", "J", "L" , "S" , "Y" , "a" , "Z" , "e" , "f" };

	for (int i = 0; i < tab_amount; i++)
	{
		RECT text_size2 = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, ESP_Player_SubTabs_Names[i].c_str());

		int tab_area[4] = {
			_pos.x + 41, _pos.y + 72 + (i * (in_sizew_esp_player_subtabs / tab_amount)),
			(in_sizew_esp_player_subtabs / tab_amount), text_size2.bottom + 8
		};

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(tab_area[0], tab_area[1], tab_area[2], tab_area[3]))
			current_players_esp_subtab = i;

		INTERFACES::Surface->DrawSetColor(CColor(255, 255, 255, 255));
		INTERFACES::Surface->DrawOutlinedRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3]);


		if (current_players_esp_subtab == i)
		{
			RENDER::FillRectangle(tab_area[0] + 67, tab_area[1], 1, tab_area[2] - 11, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main));
			RENDER::FillRectangle(tab_area[0] + 66, tab_area[1], 1, tab_area[2] - 11, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 115));

			MENU_DRAW::DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main), MENU_FONTS::weapontabicon, true,
				ESP_Player_SubTabs_Names[i].c_str());
		}
		else
		{
			//RENDER::FillRectangle(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(27, 27, 35, MenuAlpha_Main));
			//RENDER::FillRectangle(tab_area[0], tab_area[1] + tab_area[3], tab_area[2], 3, Color(39, 39, 47, MenuAlpha_Main));

			MENU_DRAW::DrawT(tab_area[0] + (((in_sizew_esp_player_subtabs / tab_amount) / 2) - (text_size2.right / 2)),
				tab_area[1] + (tab_area[3] / 2) - (text_size2.bottom / 2),
				CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::weapontabicon, true,
				ESP_Player_SubTabs_Names[i].c_str());
		}

		//	MENU_DRAW::OutlinedRect(tab_area[0], tab_area[1], tab_area[2], tab_area[3], Color(40, 44, 53, MenuAlpha_Main));
	}
} //
#include "Menu-Config.h"
std::string config_slots[10] = {
	"HvH-Spread1", "HvH-Spread2", "HvH-Spread3", "HvH-Spread4", "HvH-NoSpread1", "HvH-NoSpread2", "Rage1", "Rage2", "Legit1", "Legit1"
};

static int save_slot = 0;
void Save_Config()
{
	Config2->Save(config_slots[save_slot]);


}

void modelchanger_plus()//testing some shit
{
	static bool next = false;//bool for [+] define

	if (next == true)//check when button get clicked
	{
		Config::SkinChanger::Model::get().model_changer == 1;//move to model numer 1 
		next == false;//uncheck button[when clicked] // == false [fixing]
	}
	else if (next == true)//againg for next model
	{
		Config::SkinChanger::Model::get().model_changer == 2;//move to model number 2 
		next == false;//uncheck button[when clicked]
	}

}

void modelchanger_minus()//testing some shit#2
{
	static bool back = false;//define [-] buttton

	if (back == true)//check when button get click
	{
		Config::SkinChanger::Model::get().model_changer == 0;
		back == false;
	}
	else if (back == true)
	{
		Config::SkinChanger::Model::get().model_changer == 1;
		back == false;
	}


}

void Load_Config()
{
	Config2->Load(config_slots[save_slot]);

}

void Export_Config()
{
	Config2->export_to_clipboard("export");
}

void Import_Config()
{
	Config2->import_from_clipboard("import");
}

void Save()
{
	std::string ConfigName;

	switch (2)
	{
	case 0: ConfigName = "1";
		break;
	case 1: ConfigName = "2";
		break;
	case 2: ConfigName = "3";
		break;
	case 3: ConfigName = "4";
		break;
	case 4: ConfigName = "5";
		break;
	}
	Config2->Save(ConfigName);
}

void GradientH(int x, int y, int w, int h, CColor c1, CColor c2)
{
	RENDER::FillRectangle(x, y, w, h, c1);
	BYTE first = c2.RGBA[0];
	BYTE second = c2.RGBA[1];
	BYTE third = c2.RGBA[2];
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		RENDER::FillRectangle(x + i, y, 1, h, CColor(first, second, third, ia));
	}
}

void Gradientuptodown(int x, int y, int w, int h, CColor c1, CColor c2)
{
	RENDER::FillRectangle(x, y, w, h, c1);
	BYTE first = c2.RGBA[0];
	BYTE second = c2.RGBA[1];
	BYTE third = c2.RGBA[2];
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		RENDER::FillRectangle(x, y + i, w, 1, CColor(first, second, third, ia));
	}
}

#include <experimental/filesystem>
#include <filesystem>
#include <iostream>

#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;




DWORD FindSig(DWORD dwAddress, DWORD dwLength, const char* szPattern)
{
	if (!dwAddress || !dwLength || !szPattern)
		return 0;

	const char* pat = szPattern;
	DWORD firstMatch = NULL;

	for (DWORD pCur = dwAddress; pCur < dwLength; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == GET_BYTE(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else pat += 2;
		}
		else
		{
			pat = szPattern;
			firstMatch = 0;
		}
	}

	return 0;
}
DWORD FindSignaturenew(const char* szModuleName, const char* PatternName, char* szPattern)
{
	HMODULE hModule = GetModuleHandleA(szModuleName);
	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)(((DWORD)hModule) + pDOSHeader->e_lfanew);

	DWORD ret = FindSig(((DWORD)hModule) + pNTHeaders->OptionalHeader.BaseOfCode, ((DWORD)hModule) + pNTHeaders->OptionalHeader.SizeOfCode, szPattern);;

	return ret;
}



template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**> (UTILS::FindSignature("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1); //find element signature

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(UTILS::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}

struct hud_weapons_t
{
	std::int32_t* get_weapon_count()
	{
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};

void KnifeApplyCallbk()
{
	static auto clear_hud_weapon_icon_fn = reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(UTILS::FindSignature(("client_panorama.dll"), ("55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C")));

	auto element = FindHudElement<std::uintptr_t*>(("CCSGO_HudWeaponSelection"));

	auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);
	if (hud_weapons == nullptr)
		return;

	if (!*hud_weapons->get_weapon_count())
		return;

	for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
		i = clear_hud_weapon_icon_fn(hud_weapons, i);

	INTERFACES::Engine->ForceFullUpdate();
}



std::string text_uwu;
bool dont_recieve_input = false;

void Menu::repeatythingy(int subtabname, int totaltabs)
{

	/*for (int i = 0; i > totaltabs; i++)
	{
		if (subtabname == i)
		{
			groupbox(138, 77, GroupBoxSize_Width, GroupBoxSize_Height, "Tab : General");
			groupbox(411, 77, GroupBoxSize_Width, GroupBoxSize_Height, "Misc");
		}
		else
			subtabname += 1;
	}*/

	/*if (aimbot_main_subtab == 0)//settings
	{
		groupbox(138, 77, GroupBoxSize_Width, GroupBoxSize_Height, "Tab : General");
		groupbox(411, 77, GroupBoxSize_Width, GroupBoxSize_Height, "Misc");
		slider(1000, "Slider3", &Config::ConfigMenu::ConfigMain::get().slider3, "", 10);
		slider(100, "Slider4", &Config::ConfigMenu::ConfigMain::get().slider4, "", 1);
	}

	else if (aimbot_main_subtab == 1)//pistol
	{
		groupbox(138, 77, GroupBoxSize_Width, GroupBoxSize_Height, "Tab : Pistol");
		groupbox(411, 77, GroupBoxSize_Width, GroupBoxSize_Height, "Misc");
	}*/
}
int lmao = 0;
void mod() {
	global::modulate = true;
}
void reload()
{
	global::should_load_materials = true;
}
void Menu::Render()
{
	static bool _pressed = true;

	if (!_pressed && GetAsyncKeyState(VK_INSERT))
		_pressed = true;
	else if (_pressed && !GetAsyncKeyState(VK_INSERT))
	{
		_pressed = false;
		menuOpened = !menuOpened;
	}
	INTERFACES::Engine->GetScreenSize(screen_width, screen_width);
	static Vector2D _mouse_pos;
	static int groupbox_bottom;
	static int _drag_x = 300;
	static int _drag_y = 300;
	static Vector2D oldPos;
	static Vector2D mousePos;
	static bool _dragging = false;
	bool _click = false;
	static bool _resizing = false;

	INTERFACES::InputSystem->EnableInput(!menuOpened);
	INTERFACES::Surface->clipping_enabled() = true;

	if (menuOpened)
	{


		MenuAlpha_Main = min(MenuAlpha_Main + 15, 255);
		MenuAlpha_Text = min(MenuAlpha_Text + 5, 255);

		//MenuAlpha_Main = 255;
		//MenuAlpha_Text = 255;

		if (GetAsyncKeyState(VK_LBUTTON))
			_click = true;

		Vector2D _mouse_pos = MENU_DRAW::GetMousePosition();

		if (_dragging and !_click)
			_dragging = false;

		if (_resizing and !_click)
			_resizing = false;

		if (_dragging and _click)
		{
			_pos.x = _mouse_pos.x - _drag_x;
			_pos.y = _mouse_pos.y - _drag_y;
		}


		//if (_resizing && _click) {
		//	_width = _mouse_pos.x - _pos.x;
		//	_height = _mouse_pos.y - _pos.y;

		//	if (_width < 480) _width = 480;
		//	if (_height < 340) _height = 340;

		//	if (_width > 960) _width = 960;
		//	if (_height > 680) _height = 680;
		//}


		if (MENU_DRAW::MouseInRegion(_pos.x, _pos.y, _width, _height) && !(MENU_DRAW::MouseInRegion(
			_pos.x + 10, _pos.y + 20, _width - 10, _height - 10)))
		{
			_dragging = true;
			_drag_x = _mouse_pos.x - _pos.x;
			_drag_y = _mouse_pos.y - _pos.y;
		}

		//	if (mouse_in_params(_pos.x + 20, _pos.y + 20, _width - 10, _height - 10))
		//	{
		//		_resizing = true;
		//	}
	}
	else
	{
		MenuAlpha_Main = max(MenuAlpha_Main - 15, 0);
		MenuAlpha_Text = max(MenuAlpha_Text - 25, 0);
		//MenuAlpha_Main = 0;
		//MenuAlpha_Text = 0;
	}

	if (menuOpened)//maybe save fps?
	{
		MENU_DRAW::OutlinedRect(_pos.x, _pos.y, _width, _height, CColor(18, 18, 18, 255));

		MENU_DRAW::OutlinedRect(_pos.x + 1, _pos.y + 1, _width - 2, _height - 2, CColor(57, 57, 57, 255));

		RENDER::FillRectangle(_pos.x + 2, _pos.y + 2, _width - 4, _height - 4, CColor(29, 29, 29, 255));

		MENU_DRAW::OutlinedRect(_pos.x + 6, _pos.y + 6, _width - 12, _height - 12, CColor(57, 57, 57, 255));

		RENDER::FillRectangle(_pos.x + 7, _pos.y + 7, _width - 14, _height - 14, CColor(19, 19, 19, 255));

		//color line aa boi
		RENDER::FillRectangle(_pos.x + 8, _pos.y + 8, _width - 16, 1, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 255));
		//GradientH(_pos.x + 8, _pos.y + 8, _width / 2, 1, Color(12, 12, 12, MenuAlpha_Main), Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main));
		//GradientH(_width / 2, _pos.y + 8, _width - 16, 1, Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main), Color(12, 12, 12, MenuAlpha_Main));


		// container window background
		//MENU_DRAW::OutlinedRect(_pos.x + 23, _pos.y + 23, _width - 46, _height - 46, Color(46, 46, 46, MenuAlpha_Main));
		//RENDER::FillRectangle(_pos.x + 24, _pos.y + 24, _width - 48, _height - 48, Color(24, 24, 24, MenuAlpha_Main));

		//tabpart
		MENU_DRAW::OutlinedRect(_pos.x + 17, _pos.y + 17, _width - 17 * 2, 35, CColor(57, 56, 57, 200));
		RENDER::FillRectangle(_pos.x + 18, _pos.y + 18, _width - 18 * 2, 33, CColor(24, 24, 24, 255));

	}

	int sum = 0;
	int x;
	ifstream inFile;

	string  path;
	static string username;
	static string rent;

	static bool GrabName = true;

	if (GrabName) {
		path = getenv("appdata");
		//cout << "appdata=" << path << endl;
		path += "\\xy0_login";

		inFile.open(path);

		inFile >> username >> rent;

		GrabName = false;
	}

	//int in_sizew = _width - Config::ConfigMenu::ConfigMain::get().slider1;

	static int tab_selected = 0;
	static const int tab_amount = 9;
	static std::string tab_names[tab_amount] = { "Legitbot", "Ragebot", "Anti-aim", "Players", "Visuals", "Miscellaneous", "Player-list", "Skin Changer", "Configurations" };
	if (menuOpened)//maybe save fps? 
	{
		for (int i = 0; i < tab_amount; i++)
		{

			RECT text_size = MENU_DRAW::GetTextSizeRect(MENU_FONTS::TabFont, tab_names[i].c_str());

			int in_sizew = _width - Config::ConfigMenu::ConfigMain::get().slider1;

			int tab_area[4] = { _pos.x + Config::ConfigMenu::ConfigMain::get().slider2 + (i * (in_sizew / tab_amount)), _pos.y + 35, (in_sizew / tab_amount), 20 };

			if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(tab_area[0], tab_area[1] - 11, tab_area[2],
				tab_area[3]))
				tab_selected = i;

			if (tab_selected == i)
			{
				/*switch (tab_selected)
				{
					case 0: {
						RENDER::FillRectangle(text_size.left + tab_area[0] - 26, tab_area[1] + tab_area[3] - 37 + 1, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 155));
						RENDER::FillRectangle(text_size.left + tab_area[0] - 26, tab_area[1] + tab_area[3] - 37, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 225));
					}break;
					case 1: {
						RENDER::FillRectangle(text_size.left + tab_area[0] - 29, tab_area[1] + tab_area[3] - 37 + 1, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 155));
						RENDER::FillRectangle(text_size.left + tab_area[0] - 29, tab_area[1] + tab_area[3] - 37, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 225));
					}break;
					case 2: {
						RENDER::FillRectangle(text_size.left + tab_area[0] - 31, tab_area[1] + tab_area[3] - 37 + 1, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 155));
						RENDER::FillRectangle(text_size.left + tab_area[0] - 31, tab_area[1] + tab_area[3] - 37, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 225));
					}break;
					case 3: {
						RENDER::FillRectangle(text_size.left + tab_area[0] - 23, tab_area[1] + tab_area[3] - 37 + 1, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 155));
						RENDER::FillRectangle(text_size.left + tab_area[0] - 23, tab_area[1] + tab_area[3] - 37, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 225));
					}break;
					case 4: {
						RENDER::FillRectangle(text_size.left + tab_area[0] - 20, tab_area[1] + tab_area[3] - 37 + 1, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 155));
						RENDER::FillRectangle(text_size.left + tab_area[0] - 20, tab_area[1] + tab_area[3] - 37, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 225));
					}break;
					case 5: {
						RENDER::FillRectangle(text_size.left + tab_area[0] + 4, tab_area[1] + tab_area[3] - 37 + 1, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 155));
						RENDER::FillRectangle(text_size.left + tab_area[0] + 4, tab_area[1] + tab_area[3] - 37, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 225));
					}break;
					case 6: {
						RENDER::FillRectangle(text_size.left + tab_area[0] - 5, tab_area[1] + tab_area[3] - 37 + 1, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 155));
						RENDER::FillRectangle(text_size.left + tab_area[0] - 5, tab_area[1] + tab_area[3] - 37, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 225));
					}break;
					case 7: {
						RENDER::FillRectangle(text_size.left + tab_area[0] - 14, tab_area[1] + tab_area[3] - 37 + 1, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 155));
						RENDER::FillRectangle(text_size.left + tab_area[0] - 14, tab_area[1] + tab_area[3] - 37, text_size.right, 1,
							Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 225));
					}break;
				}*/
				/*
				tab_area[0] + 3, tab_area[1] + tab_area[3] + 1.5, tab_area[2] - 6, 1.5
				*/
				RENDER::FillRectangle(tab_area[0] + 10, tab_area[1] + tab_area[3] - 36, tab_area[2] - 21, 1,
					CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 115));
				RENDER::FillRectangle(tab_area[0] + 10, tab_area[1] + tab_area[3] - 37, tab_area[2] - 21, 1,
					CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 255));

				MENU_DRAW::DrawT(tab_area[0] + (((in_sizew / tab_amount) / 2) - (text_size.right / 2)),
					tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2 + 11), CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 255),
					MENU_FONTS::TabFont, false, tab_names[i].c_str());
			}
			else
			{

				MENU_DRAW::DrawT(tab_area[0] + (((in_sizew / tab_amount) / 2) - (text_size.right / 2)),
					tab_area[1] + (tab_area[3] / 2) - (text_size.bottom / 2 + 11), CColor(143, 143, 143, 255),
					MENU_FONTS::TabFont, false, tab_names[i].c_str());
			}

			if (tab_selected == 0 || tab_selected == 1)
			{
				/*
				MENU_DRAW::OutlinedRect(_pos.x + 6, _pos.y + 6, _width - 12, _height - 12, Color(57, 57, 57, 255));

		RENDER::FillRectangle(_pos.x + 7, _pos.y + 7, _width - 14, _height - 14, Color(19, 19, 19, 255));

		//tabpart
		MENU_DRAW::OutlinedRect(_pos.x + 23, _pos.y + 23, _width - 46, 35, Color(57, 56, 57, 255));
		RENDER::FillRectangle(_pos.x + 24, _pos.y + 24, _width - 48, 33, Color(24, 24, 24, 255));
				*/

				MENU_DRAW::OutlinedRect(_pos.x + 17, _pos.y + 62, 93, _height - 77, CColor(57, 56, 57, 200));
				RENDER::FillRectangle(_pos.x + 17 + 1, _pos.y + 63, 91, _height - 77 - 2, CColor(24, 24, 24, 255));

				MENU_DRAW::OutlinedRect(_pos.x + 127, _pos.y + 62, _width - 144, _height - 77, CColor(57, 56, 57, 200));
				RENDER::FillRectangle(_pos.x + 128, _pos.y + 63, _width - 144 - 2, _height - 77 - 2, CColor(24, 24, 24, 255));
			}
			else
			{
				MENU_DRAW::OutlinedRect(_pos.x + 17, _pos.y + 62, _width - 34, _height - 77, CColor(57, 56, 57, 200));
				RENDER::FillRectangle(_pos.x + 17 + 1, _pos.y + 63, _width - 34 - 2, _height - 77 - 2, CColor(24, 24, 24, 255));
			}

			//MENU_DRAW::Line(tab_area[0] - 10, tab_area[1] + 5, tab_area[0] - 13, tab_area[1] + tab_area[3] - 10, Color(91, 95, 97, MenuAlpha_Main));
		}
	}
	static int aimbot_main_subtab = 0;
	static int ragebot_main_subtab = 0;
	std::string pitch[5] = { "Disabled", "Down", "Up", "Origin", "Offset" };

	std::string BodyAimOptions[6] = { "In Air", "Slow Walk", "High Inaccuracy", "Vulnerable", "Lethal", "weapon = awp" };
	std::string model_changer[27] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26" };

	std::string HitboxOptionz[2] = { "Hitscan", "Head Only" };

	std::string AutoStopOptions[4] = { "Minimum Speed", "Full Stop" };

	std::string BodyAimModeOptions[2] = { "Prefer", "Force" };
	//5UWWINGS
	if (menuOpened)//maybe save fps?
	{
		std::string hitboxes[5] = {"head", "body", "neck", "legs", "arms"};
		std::string AutoStop[4] = { "None", "Quick Stop", "Full Stop", "Half Stop" };
		std::string prefer[4] = { "None", "Body-Aim", "Agressive Body-Aim", "Advance Body-Aim" };
		//std::string nnware;
		//nnware += "dambbruh.gq";
		//std::time_t result = std::time(nullptr);

		//MENU_DRAW::DrawT(_pos.x + Config::ConfigMenu::ConfigMain::get().slider3, _pos.y + 527, Color(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 255), MENU_FONTS::SPECIALSmallText, false,
			//"%s - Released at: %s - Local time: %s", nnware.c_str(), __DATE__, std::asctime(std::localtime(&result)));

		if (tab_selected == 0)//LEGITBOT
		{

			DrawLegitMain_Subtab(aimbot_main_subtab, 10, _pos, MenuAlpha_Main);
			if (aimbot_main_subtab == 0)//settings
			{
				std::string btmethod[3] = {"last tick", "all ticks", "resolved ticks"};
				std::string scan[3] = { "Closest", "Closest to crosshair", "Health" };
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "Tab : Main");
				checkbox("Enable", &Config::Legitbot::LegitGlobal::get().enable);
				keybind(&Config::Legitbot::LegitGlobal::get().legitbot_key, "b");
				checkbox("Check Smoke", &Config::Legitbot::LegitGlobal::get().smoke_check);
				checkbox("Check Wall", &Config::Legitbot::LegitGlobal::get().wall_check);
				combobox(3, "Enemy Scan", scan, &Config::Legitbot::LegitGlobal::get().enemyscan);
				checkbox("Legit AntiAim", &Config::Legitbot::LegitGlobal::get().legit_aa);
				color_selector("SLAA", &Config::Legitbot::LegitGlobal::get().show_legit_aa_r, &Config::Legitbot::LegitGlobal::get().show_legit_aa_g, &Config::Legitbot::LegitGlobal::get().show_legit_aa_b, &Config::Legitbot::LegitGlobal::get().show_legit_aa_a);
				checkbox("Show Legit AA", &Config::Legitbot::LegitGlobal::get().show_legit_aa);

				groupbox(137, 312.5, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "Backtrack");
				checkbox("Enable Backtrack ", &Config::Legitbot::LegitGlobal::get().backtrack_enable);
				slider(13, "Backtrack Ticks", &Config::Legitbot::LegitGlobal::get().backtrack_ticks, "t", 0);

				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "TrigerBot");
				checkbox("Enable TrigerBot", &Config::Legitbot::LegitGlobal::get().trigerbot);
				keybind(&Config::Legitbot::LegitGlobal::get().trigerbot_key, "c");
				slider(100, "Trigerbot hitchance", &Config::Legitbot::LegitGlobal::get().triggerbot_hitchance,"%", 0);


				groupbox(412, 312.5, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "RCS");
				checkbox("RCS Control", &Config::Legitbot::LegitGlobal::get().rcs_enable);
				sliderfloat(100, "RCS X", &Config::Legitbot::LegitGlobal::get().rcs_x, "%", 0);
				sliderfloat(100, "RCS Y", &Config::Legitbot::LegitGlobal::get().rcs_x, "%", 0);
			}

			else if (aimbot_main_subtab == 1)//pistol
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : Pistol");
				checkbox("Enable", &Config::Legitbot::LegitPistol::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Legitbot::LegitPistol::get().hitboxes);
				slider(180, "FOV", &Config::Legitbot::LegitPistol::get().fov, "°", 0);
				slider(100, "Smothness", &Config::Legitbot::LegitPistol::get().smooth, "%", 0);
				checkbox("Silent", &Config::Legitbot::LegitPistol::get().silent);
				slider(300, "Shot Delay", &Config::Legitbot::LegitPistol::get().delay, "t", 0);


				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "TrigerBot");
				checkbox("Enable", &Config::Legitbot::LegitGlobal::get().trigerbot);
				keybind(&Config::Legitbot::LegitGlobal::get().trigerbot_key, "c");
				slider(100, "Trigerbot hitchance", &Config::Legitbot::LegitPistol::get().triggerbot_hitchance, "%", 0);


				groupbox(412, 312.5, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "RCS");
				checkbox("RCS Control", &Config::Legitbot::LegitPistol::get().rcs_enable);
				sliderfloat(100, "RCS X", &Config::Legitbot::LegitPistol::get().rcs_x, "%", 0);
				sliderfloat(100, "RCS Y", &Config::Legitbot::LegitPistol::get().rcs_x, "%", 0);
			}
			else if (aimbot_main_subtab == 2)//heavypistol
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : Heavy Pistol");
				checkbox("Enable", &Config::Legitbot::LegitHeavyPistol::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Legitbot::LegitHeavyPistol::get().hitboxes);
				slider(180, "FOV", &Config::Legitbot::LegitHeavyPistol::get().fov, "°", 0);
				slider(100, "Smothness", &Config::Legitbot::LegitHeavyPistol::get().smooth, "%", 0);
				checkbox("Silent", &Config::Legitbot::LegitHeavyPistol::get().silent);
				slider(300, "Shot Delay", &Config::Legitbot::LegitHeavyPistol::get().delay, "t", 0);


				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "TrigerBot");
				checkbox("Enable", &Config::Legitbot::LegitGlobal::get().trigerbot);
				keybind(&Config::Legitbot::LegitGlobal::get().trigerbot_key, "c");
				slider(100, "Trigerbot hitchance", &Config::Legitbot::LegitHeavyPistol::get().triggerbot_hitchance, "%", 0);


				groupbox(412, 312.5, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "RCS");
				checkbox("RCS Control", &Config::Legitbot::LegitHeavyPistol::get().rcs_enable);
				sliderfloat(100, "RCS X", &Config::Legitbot::LegitHeavyPistol::get().rcs_x, "%", 0);
				sliderfloat(100, "RCS Y", &Config::Legitbot::LegitHeavyPistol::get().rcs_x, "%", 0);
			}
			else if (aimbot_main_subtab == 3)//smg
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : SMG");
				checkbox("Enable", &Config::Legitbot::LegitSMG::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Legitbot::LegitSMG::get().hitboxes);
				slider(180, "FOV", &Config::Legitbot::LegitSMG::get().fov, "°", 0);
				slider(100, "Smothness", &Config::Legitbot::LegitSMG::get().smooth, "%", 0);
				checkbox("Silent", &Config::Legitbot::LegitSMG::get().silent);
				slider(300, "Shot Delay", &Config::Legitbot::LegitSMG::get().delay, "t", 0);


				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "TrigerBot");
				checkbox("Enable", &Config::Legitbot::LegitGlobal::get().trigerbot);
				keybind(&Config::Legitbot::LegitGlobal::get().trigerbot_key, "c");
				slider(100, "Trigerbot hitchance", &Config::Legitbot::LegitSMG::get().triggerbot_hitchance, "%", 0);


				groupbox(412, 312.5, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "RCS");
				checkbox("RCS Control", &Config::Legitbot::LegitSMG::get().rcs_enable);
				sliderfloat(100, "RCS X", &Config::Legitbot::LegitSMG::get().rcs_x, "%", 0);
				sliderfloat(100, "RCS Y", &Config::Legitbot::LegitSMG::get().rcs_x, "%", 0);

			}
			else if (aimbot_main_subtab == 4)//ar
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : Asult Rifle");
				checkbox("Enable", &Config::Legitbot::LegitAR::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Legitbot::LegitAR::get().hitboxes);
				slider(180, "FOV", &Config::Legitbot::LegitAR::get().fov, "°", 0);
				slider(100, "Smothness", &Config::Legitbot::LegitAR::get().smooth, "%", 0);
				checkbox("Silent", &Config::Legitbot::LegitAR::get().silent);
				slider(300, "Delay", &Config::Legitbot::LegitAR::get().delay, "t", 0);


				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "TrigerBot");
				checkbox("Enable", &Config::Legitbot::LegitGlobal::get().trigerbot);
				keybind(&Config::Legitbot::LegitGlobal::get().trigerbot_key, "c");
				slider(100, "Trigerbot hitchance", &Config::Legitbot::LegitAR::get().triggerbot_hitchance, "%", 0);


				groupbox(412, 312.5, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "RCS");
				checkbox("RCS Enable", &Config::Legitbot::LegitAR::get().rcs_enable);
				sliderfloat(100, "RCS X", &Config::Legitbot::LegitAR::get().rcs_x, "%", 0);
				sliderfloat(100, "RCS Y", &Config::Legitbot::LegitAR::get().rcs_x, "%", 0);
			}
			else if (aimbot_main_subtab == 5)//auto
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : Auto Sniper");
				checkbox("Enable", &Config::Legitbot::LegitAuto::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Legitbot::LegitAuto::get().hitboxes);
				slider(180, "FOV", &Config::Legitbot::LegitAuto::get().fov, "°", 0);
				slider(100, "Smothness", &Config::Legitbot::LegitAuto::get().smooth, "%", 0);
				checkbox("Silent", &Config::Legitbot::LegitAuto::get().silent);
				slider(300, "Delay", &Config::Legitbot::LegitAuto::get().delay, "t", 0);


				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "TrigerBot");
				checkbox("Enable", &Config::Legitbot::LegitGlobal::get().trigerbot);
				keybind(&Config::Legitbot::LegitGlobal::get().trigerbot_key, "c");
				slider(100, "Trigerbot hitchance", &Config::Legitbot::LegitAuto::get().triggerbot_hitchance, "%", 0);


				groupbox(412, 312.5, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "RCS");
				checkbox("RCS Enable", &Config::Legitbot::LegitAuto::get().rcs_enable);
				sliderfloat(100, "RCS X", &Config::Legitbot::LegitAuto::get().rcs_x, "%", 0);
				sliderfloat(100, "RCS Y", &Config::Legitbot::LegitAuto::get().rcs_x, "%", 0);
			}
			else if (aimbot_main_subtab == 6)//scoot
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : Scout");
				checkbox("Enable", &Config::Legitbot::LegitScout::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Legitbot::LegitScout::get().hitboxes);
				slider(180, "FOV", &Config::Legitbot::LegitScout::get().fov, "°", 0);
				slider(100, "Smothness", &Config::Legitbot::LegitScout::get().smooth, "%", 0);
				checkbox("Silent", &Config::Legitbot::LegitScout::get().silent);
				slider(300, "Delay", &Config::Legitbot::LegitScout::get().delay, "t", 0);


				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "TrigerBot");
				checkbox("Enable", &Config::Legitbot::LegitGlobal::get().trigerbot);
				keybind(&Config::Legitbot::LegitGlobal::get().trigerbot_key, "c");
				slider(100, "Trigerbot hitchance", &Config::Legitbot::LegitScout::get().triggerbot_hitchance, "%", 0);


				groupbox(412, 312.5, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "RCS");
				checkbox("RCS Enable", &Config::Legitbot::LegitScout::get().rcs_enable);
				sliderfloat(100, "RCS X", &Config::Legitbot::LegitScout::get().rcs_x, "%", 0);
				sliderfloat(100, "RCS Y", &Config::Legitbot::LegitScout::get().rcs_x, "%", 0);
			}
			else if (aimbot_main_subtab == 7)//a v p
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : AWP");
				checkbox("Enable", &Config::Legitbot::LegitAWP::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Legitbot::LegitAWP::get().hitboxes);
				slider(180, "FOV", &Config::Legitbot::LegitAWP::get().fov, "°", 0);
				slider(100, "Smothness", &Config::Legitbot::LegitAWP::get().smooth, "%", 0);
				checkbox("Silent", &Config::Legitbot::LegitAWP::get().silent);
				slider(300, "Delay", &Config::Legitbot::LegitAWP::get().delay, "t", 0);


				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "TrigerBot");
				checkbox("Enable", &Config::Legitbot::LegitGlobal::get().trigerbot);
				keybind(&Config::Legitbot::LegitGlobal::get().trigerbot_key, "c");
				slider(100, "Trigerbot hitchance", &Config::Legitbot::LegitAWP::get().triggerbot_hitchance, "%", 0);


				groupbox(412, 312.5, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "RCS");
				checkbox("RCS Enable", &Config::Legitbot::LegitAWP::get().rcs_enable);
				sliderfloat(100, "RCS X", &Config::Legitbot::LegitAWP::get().rcs_x, "%", 0);
				sliderfloat(100, "RCS Y", &Config::Legitbot::LegitAWP::get().rcs_x, "%", 0);
			}
			else if (aimbot_main_subtab == 8)// shooty
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : ShotGun");
				checkbox("Enable", &Config::Legitbot::LegitShotGuns::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Legitbot::LegitShotGuns::get().hitboxes);
				slider(180, "FOV", &Config::Legitbot::LegitShotGuns::get().fov, "°", 0);
				slider(100, "Smothness", &Config::Legitbot::LegitShotGuns::get().smooth, "%", 0);
				checkbox("Silent", &Config::Legitbot::LegitShotGuns::get().silent);
				slider(300, "Delay", &Config::Legitbot::LegitShotGuns::get().delay, "t", 0);


				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "TrigerBot");
				checkbox("Enable", &Config::Legitbot::LegitGlobal::get().trigerbot);
				keybind(&Config::Legitbot::LegitGlobal::get().trigerbot_key, "c");
				slider(100, "Trigerbot hitchance", &Config::Legitbot::LegitShotGuns::get().triggerbot_hitchance, "%", 0);


				groupbox(412, 312.5, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "RCS");
				checkbox("RCS Enable", &Config::Legitbot::LegitShotGuns::get().rcs_enable);
				sliderfloat(100, "RCS X", &Config::Legitbot::LegitShotGuns::get().rcs_x, "%", 0);
				sliderfloat(100, "RCS Y", &Config::Legitbot::LegitShotGuns::get().rcs_x, "%", 0);
			}
			else if (aimbot_main_subtab == 9)//lmg
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : LMG");
				checkbox("Enable", &Config::Legitbot::LegitLMG::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Legitbot::LegitLMG::get().hitboxes);
				slider(180, "FOV", &Config::Legitbot::LegitLMG::get().fov, "°", 0);
				slider(100, "Smothness", &Config::Legitbot::LegitLMG::get().smooth, "%", 0);
				checkbox("Silent", &Config::Legitbot::LegitLMG::get().silent);
				slider(300, "Delay", &Config::Legitbot::LegitLMG::get().delay, "t", 0);


				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "TrigerBot");
				checkbox("Enable", &Config::Legitbot::LegitGlobal::get().trigerbot);
				keybind(&Config::Legitbot::LegitGlobal::get().trigerbot_key, "c");
				slider(100, "Trigerbot hitchance", &Config::Legitbot::LegitLMG::get().triggerbot_hitchance, "%", 0);


				groupbox(412, 312.5, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "RCS");
				checkbox("RCS Enable", &Config::Legitbot::LegitLMG::get().rcs_enable);
				sliderfloat(100, "RCS X", &Config::Legitbot::LegitLMG::get().rcs_x, "%", 0);
				sliderfloat(100, "RCS Y", &Config::Legitbot::LegitLMG::get().rcs_x, "%", 0);
			}
		}


		if (tab_selected == 1)//RAGEBOT
		{
			DrawRageMain_Subtab(ragebot_main_subtab, 10, _pos, MenuAlpha_Main);

			if (ragebot_main_subtab == 0)//settings
			{
				std::string btmethod[3] = { "Last-tick", "All ticks", "Resolved ticks" };
				std::string scan[3] = { "Closest", "Closest to crosshair", "Health" };
				std::string fakelagcor[3] = { "None", "Fake-Lag correct", "Wait for on Shot" };
				std::string accbost[5] = {"None", "Low", "Medium", "High", "Maximum" };
				std::string dtap[3] = { "None", "On Shot", "On Key" };
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "Tab : Main");
				checkbox("Enable", &Config::Ragebot::RageGlobal::get().enable);
				combobox(3, "Target Selection", scan, &Config::Ragebot::RageGlobal::get().enemyscan);
				checkbox("Automatic Fire", &Config::Ragebot::RageGlobal::get().autoshoot);
				checkbox("Automatic Pentration", &Config::Ragebot::RageGlobal::get().autowall);
				checkbox("Silent Aim", &Config::Ragebot::RageGlobal::get().silent);
				checkbox("Remove Recoil", &Config::Ragebot::RageGlobal::get().remove_recoil);
				checkbox("Auto Scope", &Config::Ragebot::RageGlobal::get().auto_scope);
				slider(180, "Maximum FOV", &Config::Ragebot::RageGlobal::get().max_fov, "°", 0);


				groupbox(137, 312.5, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "Backtrack");
				checkbox("Backtrack Enable", &Config::Ragebot::RageGlobal::get().backtrack_enable);
				slider(13, "Backtrack Ticks", &Config::Ragebot::RageGlobal::get().backtrack_ticks, "t", 0);
				combobox(3, "Backtrack Method", btmethod, &Config::Ragebot::RageGlobal::get().backtrack_method);
				checkbox("Extended Backtrack Enable", &Config::Ragebot::RageGlobal::get().extened_backtrack_ticks);
				sliderfloat(1000, "Extende Backtrack Time", &Config::Ragebot::RageGlobal::get().extend_time, "ms", 0);

				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "Misc");
				checkbox("Anti-aim Correction", &Config::Ragebot::RageGlobal::get().resolver);
				combobox(3, "Fake-Lag Correction", fakelagcor, &Config::Ragebot::RageGlobal::get().fakelag_cor);
				combobox(5, "Accauracy boost", accbost, &Config::Ragebot::RageGlobal::get().acc_bost);
				text("Body-Aim Key");
				keybind(&Config::Ragebot::RageGlobal::get().bodyaim, "body-aim");
				checkbox("Low FPS optimization", &Config::Ragebot::RageGlobal::get().low_fps);

				groupbox(412, 312.5, GroupBoxSize_Width * 1.5, GroupBoxSize_Height / 2, "Exploits");
				checkbox("Remove Spread", &Config::Ragebot::RageGlobal::get().remove_spread);
				combobox(3, "Dubletap", dtap, &Config::Ragebot::RageGlobal::get().dubletap);
				if(Config::Ragebot::RageGlobal::get().dubletap == 2)
				keybind(&Config::Ragebot::RageGlobal::get().dubletap_key, "duble tap");
				


			}

			else if (ragebot_main_subtab == 1)//pistol
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : Pistol");
				checkbox("Enable", &Config::Ragebot::RagePistol::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Ragebot::RagePistol::get().hitboxes);
				slider(100, "Hit Chance", &Config::Ragebot::RagePistol::get().hitchance, "%", 0);
				slider(150, "Minimum Damage", &Config::Ragebot::RagePistol::get().min_dmg, "%", 0);
				slider(150, "Minimum Penetration Damage", &Config::Ragebot::RagePistol::get().min_dmg_autowall, "%", 0);
				combobox(4, "Auto Stop", AutoStop, &Config::Ragebot::RagePistol::get().auto_stop_method);
				combobox(4, "Prefer:", prefer, &Config::Ragebot::RagePistol::get().prefer);

				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Scales");
				if (Config::Ragebot::RagePistol::get().hitboxes[0])
					slider(100, "Hit-Box Scale Head", &Config::Ragebot::RagePistol::get().multipoint_head, "%", 0);
				if (Config::Ragebot::RagePistol::get().hitboxes[1])
					slider(100, "Hit-Box Scale Body", &Config::Ragebot::RagePistol::get().multipoint_body, "%", 0);
				if (Config::Ragebot::RagePistol::get().hitboxes[2])
					slider(100, "Hit-Box Scale Neck", &Config::Ragebot::RagePistol::get().multipoint_neck, "%", 0);
				if (Config::Ragebot::RagePistol::get().hitboxes[3])
					slider(100, "Hit-Box Scale Legs", &Config::Ragebot::RagePistol::get().multipoint_legs, "%", 0);
				if (Config::Ragebot::RagePistol::get().hitboxes[4])
					slider(100, "Hit-Box Scale Arms", &Config::Ragebot::RagePistol::get().multipoint_arms, "%", 0);
			}
			else if (ragebot_main_subtab == 2)//heavypistol
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : HeavyPistol ");
				checkbox("Enable", &Config::Ragebot::RageHeavyPistol::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Ragebot::RageHeavyPistol::get().hitboxes);
				slider(100, "Hit Chance", &Config::Ragebot::RageHeavyPistol::get().hitchance, "%", 0);
				slider(150, "Minimum Damage", &Config::Ragebot::RageHeavyPistol::get().min_dmg, "%", 0);
				slider(150, "Minimum Penetration Damage", &Config::Ragebot::RageHeavyPistol::get().min_dmg_autowall, "%", 0);
				combobox(4, "Auto Stop", AutoStop, &Config::Ragebot::RageHeavyPistol::get().auto_stop_method);
				combobox(4, "Prefer:", prefer, &Config::Ragebot::RageHeavyPistol::get().prefer);

				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Scales");
				if (Config::Ragebot::RageHeavyPistol::get().hitboxes[0])
					slider(100, "Hit-Box Scale Head", &Config::Ragebot::RageHeavyPistol::get().multipoint_head, "%", 0);
				if (Config::Ragebot::RageHeavyPistol::get().hitboxes[1])
					slider(100, "Hit-Box Scale Body", &Config::Ragebot::RageHeavyPistol::get().multipoint_body, "%", 0);
				if (Config::Ragebot::RageHeavyPistol::get().hitboxes[2])
					slider(100, "Hit-Box Scale Neck", &Config::Ragebot::RageHeavyPistol::get().multipoint_neck, "%", 0);
				if (Config::Ragebot::RageHeavyPistol::get().hitboxes[3])
					slider(100, "Hit-Box Scale Legs", &Config::Ragebot::RageHeavyPistol::get().multipoint_legs, "%", 0);
				if (Config::Ragebot::RageHeavyPistol::get().hitboxes[4])
					slider(100, "Hit-Box Scale Arms", &Config::Ragebot::RageHeavyPistol::get().multipoint_arms, "%", 0);
			}
			else if (ragebot_main_subtab == 3)//smg
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : SMG");
				checkbox("Enable", &Config::Ragebot::RageSMG::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Ragebot::RageSMG::get().hitboxes);
				slider(100, "Hit Chance", &Config::Ragebot::RageSMG::get().hitchance, "%", 0);
				slider(150, "Minimum Damage", &Config::Ragebot::RageSMG::get().min_dmg, "%", 0);
				slider(150, "Minimum Penetration Damage", &Config::Ragebot::RageSMG::get().min_dmg_autowall, "%", 0);
				combobox(4, "Auto Stop", AutoStop, &Config::Ragebot::RageSMG::get().auto_stop_method);
				combobox(4, "Prefer:", prefer, &Config::Ragebot::RageSMG::get().prefer);

				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Scales");
				if (Config::Ragebot::RageSMG::get().hitboxes[0])
					slider(100, "Hit-Box Scale Head", &Config::Ragebot::RageSMG::get().multipoint_head, "%", 0);
				if (Config::Ragebot::RageSMG::get().hitboxes[1])
					slider(100, "Hit-Box Scale Body", &Config::Ragebot::RageSMG::get().multipoint_body, "%", 0);
				if (Config::Ragebot::RageSMG::get().hitboxes[2])
					slider(100, "Hit-Box Scale Neck", &Config::Ragebot::RageSMG::get().multipoint_neck, "%", 0);
				if (Config::Ragebot::RageSMG::get().hitboxes[3])
					slider(100, "Hit-Box Scale Legs", &Config::Ragebot::RageSMG::get().multipoint_legs, "%", 0);
				if (Config::Ragebot::RageSMG::get().hitboxes[4])
					slider(100, "Hit-Box Scale Arms", &Config::Ragebot::RageSMG::get().multipoint_arms, "%", 0);

			}
			else if (ragebot_main_subtab == 4)//ar
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : AR");
				checkbox("Enable", &Config::Ragebot::RageAR::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Ragebot::RageAR::get().hitboxes);
				slider(100, "Hit Chance", &Config::Ragebot::RageAR::get().hitchance, "%", 0);
				slider(150, "Minimum Damage", &Config::Ragebot::RageAR::get().min_dmg, "%", 0);
				slider(150, "Minimum Penetration Damage", &Config::Ragebot::RageAR::get().min_dmg_autowall, "%", 0);
				combobox(4, "Auto Stop", AutoStop, &Config::Ragebot::RageAR::get().auto_stop_method);
				combobox(4, "Prefer:", prefer, &Config::Ragebot::RageAR::get().prefer);

				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Scales");
				if (Config::Ragebot::RageAR::get().hitboxes[0])
					slider(100, "Hit-Box Scale Head", &Config::Ragebot::RageAR::get().multipoint_head, "%", 0);
				if (Config::Ragebot::RageAR::get().hitboxes[1])
					slider(100, "Hit-Box Scale Body", &Config::Ragebot::RageAR::get().multipoint_body, "%", 0);
				if (Config::Ragebot::RageAR::get().hitboxes[2])
					slider(100, "Hit-Box Scale Neck", &Config::Ragebot::RageAR::get().multipoint_neck, "%", 0);
				if (Config::Ragebot::RageAR::get().hitboxes[3])
					slider(100, "Hit-Box Scale Legs", &Config::Ragebot::RageAR::get().multipoint_legs, "%", 0);
				if (Config::Ragebot::RageAR::get().hitboxes[4])
					slider(100, "Hit-Box Scale Arms", &Config::Ragebot::RageAR::get().multipoint_arms, "%", 0);
			}
			else if (ragebot_main_subtab == 5)//auto
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : Auto");
				checkbox("Enable", &Config::Ragebot::RageAuto::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Ragebot::RageAuto::get().hitboxes);
				slider(100, "Hit Chance", &Config::Ragebot::RageAuto::get().hitchance, "%", 0);
				slider(150, "Minimum Damage", &Config::Ragebot::RageAuto::get().min_dmg, "%", 0);
				slider(150, "Minimum Penetration Damage", &Config::Ragebot::RageAuto::get().min_dmg_autowall, "%", 0);
				combobox(4, "Auto Stop", AutoStop, &Config::Ragebot::RageAuto::get().auto_stop_method);
				combobox(4, "Prefer:", prefer, &Config::Ragebot::RageAuto::get().prefer);

				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Scales");
				if (Config::Ragebot::RageAuto::get().hitboxes[0])
					slider(100, "Hit-Box Scale Head", &Config::Ragebot::RageAuto::get().multipoint_head, "%", 0);
				if (Config::Ragebot::RageAuto::get().hitboxes[1])
					slider(100, "Hit-Box Scale Body", &Config::Ragebot::RageAuto::get().multipoint_body, "%", 0);
				if (Config::Ragebot::RageAuto::get().hitboxes[2])
					slider(100, "Hit-Box Scale Neck", &Config::Ragebot::RageAuto::get().multipoint_neck, "%", 0);
				if (Config::Ragebot::RageAuto::get().hitboxes[3])
					slider(100, "Hit-Box Scale Legs", &Config::Ragebot::RageAuto::get().multipoint_legs, "%", 0);
				if (Config::Ragebot::RageAuto::get().hitboxes[4])
					slider(100, "Hit-Box Scale Arms", &Config::Ragebot::RageAuto::get().multipoint_arms, "%", 0);
			}
			else if (ragebot_main_subtab == 6)//scoot
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : Scout");
				checkbox("Enable", &Config::Ragebot::RageScout::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Ragebot::RageScout::get().hitboxes);
				slider(100, "Hit Chance", &Config::Ragebot::RageScout::get().hitchance, "%", 0);
				slider(150, "Minimum Damage", &Config::Ragebot::RageScout::get().min_dmg, "%", 0);
				slider(150, "Minimum Penetration Damage", &Config::Ragebot::RageScout::get().min_dmg_autowall, "%", 0);
				combobox(4, "Auto Stop", AutoStop, &Config::Ragebot::RageScout::get().auto_stop_method);
				combobox(4, "Prefer:", prefer, &Config::Ragebot::RageScout::get().prefer);

				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Scales");
				if (Config::Ragebot::RageScout::get().hitboxes[0])
					slider(100, "Hit-Box Scale Head", &Config::Ragebot::RageScout::get().multipoint_head, "%", 0);
				if (Config::Ragebot::RageScout::get().hitboxes[1])
					slider(100, "Hit-Box Scale Body", &Config::Ragebot::RageScout::get().multipoint_body, "%", 0);
				if (Config::Ragebot::RageScout::get().hitboxes[2])
					slider(100, "Hit-Box Scale Neck", &Config::Ragebot::RageScout::get().multipoint_neck, "%", 0);
				if (Config::Ragebot::RageScout::get().hitboxes[3])
					slider(100, "Hit-Box Scale Legs", &Config::Ragebot::RageScout::get().multipoint_legs, "%", 0);
				if (Config::Ragebot::RageScout::get().hitboxes[4])
					slider(100, "Hit-Box Scale Arms", &Config::Ragebot::RageScout::get().multipoint_arms, "%", 0);
			}
			else if (ragebot_main_subtab == 7)//a v p
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : AWP");
				checkbox("Enable", &Config::Ragebot::RageAWP::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Ragebot::RageAWP::get().hitboxes);
				slider(100, "Hit Chance", &Config::Ragebot::RageAWP::get().hitchance, "%", 0);
				slider(150, "Minimum Damage", &Config::Ragebot::RageAWP::get().min_dmg, "%", 0);
				slider(150, "Minimum Penetration Damage", &Config::Ragebot::RageAWP::get().min_dmg_autowall, "%", 0);
				combobox(4, "Auto Stop", AutoStop, &Config::Ragebot::RageAWP::get().auto_stop_method);
				combobox(4, "Prefer:", prefer, &Config::Ragebot::RageAWP::get().prefer);

				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Scales");
				if (Config::Ragebot::RageAWP::get().hitboxes[0])
					slider(100, "Hit-Box Scale Head", &Config::Ragebot::RageAWP::get().multipoint_head, "%", 0);
				if (Config::Ragebot::RageAWP::get().hitboxes[1])
					slider(100, "Hit-Box Scale Body", &Config::Ragebot::RageAWP::get().multipoint_body, "%", 0);
				if (Config::Ragebot::RageAWP::get().hitboxes[2])
					slider(100, "Hit-Box Scale Neck", &Config::Ragebot::RageAWP::get().multipoint_neck, "%", 0);
				if (Config::Ragebot::RageAWP::get().hitboxes[3])
					slider(100, "Hit-Box Scale Legs", &Config::Ragebot::RageAWP::get().multipoint_legs, "%", 0);
				if (Config::Ragebot::RageAWP::get().hitboxes[4])
					slider(100, "Hit-Box Scale Arms", &Config::Ragebot::RageAWP::get().multipoint_arms, "%", 0);
			}
			else if (ragebot_main_subtab == 8)// shooty
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : ShotGun");
				checkbox("Enable", &Config::Ragebot::RageShotGuns::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Ragebot::RageShotGuns::get().hitboxes);
				slider(100, "Hit Chance", &Config::Ragebot::RageShotGuns::get().hitchance, "%", 0);
				slider(150, "Minimum Damage", &Config::Ragebot::RageShotGuns::get().min_dmg, "%", 0);
				slider(150, "Minimum Penetration Damage", &Config::Ragebot::RageShotGuns::get().min_dmg_autowall, "%", 0);
				combobox(4, "Auto Stop", AutoStop, &Config::Ragebot::RageShotGuns::get().auto_stop_method);
				combobox(4, "Prefer:", prefer, &Config::Ragebot::RageShotGuns::get().prefer);

				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Scales");
				if (Config::Ragebot::RageShotGuns::get().hitboxes[0])
					slider(100, "Hit-Box Scale Head", &Config::Ragebot::RageShotGuns::get().multipoint_head, "%", 0);
				if (Config::Ragebot::RageShotGuns::get().hitboxes[1])
					slider(100, "Hit-Box Scale Body", &Config::Ragebot::RageShotGuns::get().multipoint_body, "%", 0);
				if (Config::Ragebot::RageShotGuns::get().hitboxes[2])
					slider(100, "Hit-Box Scale Neck", &Config::Ragebot::RageShotGuns::get().multipoint_neck, "%", 0);
				if (Config::Ragebot::RageShotGuns::get().hitboxes[3])
					slider(100, "Hit-Box Scale Legs", &Config::Ragebot::RageShotGuns::get().multipoint_legs, "%", 0);
				if (Config::Ragebot::RageShotGuns::get().hitboxes[4])
					slider(100, "Hit-Box Scale Arms", &Config::Ragebot::RageShotGuns::get().multipoint_arms, "%", 0);
			}
			else if (aimbot_main_subtab == 9)//lmg
			{
				groupbox(137, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Tab : LMG");
				checkbox("Enable", &Config::Ragebot::RageLMG::get().enable);
				MultiComboBox(5, "Hitboxes", hitboxes, Config::Ragebot::RageLMG::get().hitboxes);
				slider(100, "Hit Chance", &Config::Ragebot::RageLMG::get().hitchance, "%", 0);
				slider(150, "Minimum Damage", &Config::Ragebot::RageLMG::get().min_dmg, "%", 0);
				slider(150, "Minimum Penetration Damage", &Config::Ragebot::RageLMG::get().min_dmg_autowall, "%", 0);
				combobox(4, "Auto Stop", AutoStop, &Config::Ragebot::RageLMG::get().auto_stop_method);
				combobox(4, "Prefer:", prefer, &Config::Ragebot::RageLMG::get().prefer);

				groupbox(412, 77, GroupBoxSize_Width * 1.5, GroupBoxSize_Height, "Scales");
				if (Config::Ragebot::RageLMG::get().hitboxes[0])
					slider(100, "Hit-Box Scale Head", &Config::Ragebot::RageLMG::get().multipoint_head, "%", 0);
				if (Config::Ragebot::RageLMG::get().hitboxes[1])
					slider(100, "Hit-Box Scale Body", &Config::Ragebot::RageLMG::get().multipoint_body, "%", 0);
				if (Config::Ragebot::RageLMG::get().hitboxes[2])
					slider(100, "Hit-Box Scale Neck", &Config::Ragebot::RageLMG::get().multipoint_neck, "%", 0);
				if (Config::Ragebot::RageLMG::get().hitboxes[3])
					slider(100, "Hit-Box Scale Legs", &Config::Ragebot::RageLMG::get().multipoint_legs, "%", 0);
				if (Config::Ragebot::RageLMG::get().hitboxes[4])
					slider(100, "Hit-Box Scale Arms", &Config::Ragebot::RageLMG::get().multipoint_arms, "%", 0);
				}
		}
		std::string pitch[5] = {"none", "down", "up", "random", "fake up"};
		std::string base[2] = { "local view", "at target" };
		std::string dbase[2] = { "menual", "beta" };
		std::string desync[4] = { "none", "static", "jitter", "lby" };
		std::string desync2[2] = { "none", "180 + jitter" };
		std::string yaw[5] = { "none", "180z jitter","backwards jitter", "custom jitter", "freestanding" };
		std::string yawjitter[6] = { "none", "Offset", "Center", "Random" };
		std::string fakelag[5] = { "none", "passive", "factor", "adaptive", "step" };
		std::string trigers[3] = { "on ground", "in air", "on peek" };
		if (tab_selected == 2) {
			groupbox(30, 77, GroupBoxSize_Width * 1.8, GroupBoxSize_Height / 1.8, "Anti Aim");
			combobox(5, "Pitch", pitch, &Config::AntiAim::AA::get().pitch);
			combobox(2, "Yaw Base", base, &Config::AntiAim::AA::get().yawbase);
			combobox(5, "Yaw", yaw, &Config::AntiAim::AA::get().yaw);
			if (Config::AntiAim::AA::get().yaw == 1) 
				slider(25, "jitter speed", &Config::AntiAim::AA::get().jitterspeed, "u", 0);
			else if (Config::AntiAim::AA::get().yaw == 2) {
				slider(25, "jitter speed", &Config::AntiAim::AA::get().jitterspeed, "u", 0);
				slider(25, "jitter width", &Config::AntiAim::AA::get().jitterwidth, "*", 0);
			}
			else if (Config::AntiAim::AA::get().yaw == 3) {
				slider(25, "jitter speed", &Config::AntiAim::AA::get().jitterspeed, "u", 0);
				slider(25, "jitter width", &Config::AntiAim::AA::get().jitterwidth, "*", 0);
				slider(180, "jitter angle", &Config::AntiAim::AA::get().jitterangle, "*", -180);
			}

			groupbox(30, 342.5, GroupBoxSize_Width * 1.8, GroupBoxSize_Height / 2.4, "AntiAim KeyBind's:");
			static int keybind2 = 0;
			std::string keybind3[5] = { "manual left", "manual right", "manual back", "slowwalk", "desync switch" };
			combobox(5, "keybind list", keybind3, &keybind2);
			if (keybind2 == 0)
			{
				text("Menual AA left key:");
				keybind(&Config::AntiAim::AAMisc::get().menual_left, "left key");
			}
			if (keybind2 == 1)
			{
				text("Menual AA right key:");
				keybind(&Config::AntiAim::AAMisc::get().menual_right, "right key");
			}
			if (keybind2 == 2)
			{
				text("Menual AA backwards key:");
				keybind(&Config::AntiAim::AAMisc::get().menual_back, "back key");
			}
			if (keybind2 == 3)
			{
				text("Slowwalk key:");
				keybind(&Config::AntiAim::AAMisc::get().slowwwalk, "slowwalk");
			}
			if (keybind2 == 4)
			{
				text("Desync Flip Key");
				keybind(&Config::AntiAim::AADesync::get().desync_flip, "flip key");
			}



			groupbox(349, 77, GroupBoxSize_Width * 1.8, GroupBoxSize_Height / 2.4, "Fakelag");
			combobox(5, "Fakelag mode", fakelag, &Config::AntiAim::AAFakelag::get().fakelag);
			slider(16, "Fakelag Ticks", &Config::AntiAim::AAFakelag::get().fakelagticks, "t", 0);
			MultiComboBox(3, "Fakelag triggers", trigers, Config::AntiAim::AAFakelag::get().triggers);
			slider(16, "Fakelag Triggers Ticks", &Config::AntiAim::AAFakelag::get().fakelagtickstrigger, "t", 0);


			groupbox(349, 272, GroupBoxSize_Width * 1.8, GroupBoxSize_Height / 1.8, "Desync");
			combobox(2, "Desync Base", dbase, &Config::AntiAim::AADesync::get().desyncbase);
			//combobox(2, "Yaw Configure", desync, &Config::AntiAim::AADesync::get().desync);
			//combobox(4, "Desync", desync, &Config::AntiAim::AADesync::get().desync);
			if (Config::AntiAim::AADesync::get().desyncbase == 0)
			{
				combobox(4, "Desync", desync, &Config::AntiAim::AADesync::get().desync);
				slider(58, "Angle", &Config::AntiAim::AADesync::get().angle, "°", 0);
			}
			if (Config::AntiAim::AADesync::get().desyncbase == 1)
			{
				combobox(2, "Yaw Configure", desync2, &Config::AntiAim::AADesync::get().desync);
			}

			slider(50, "Slowwalk speed", &Config::AntiAim::AAMisc::get().slowwwalk_speed, "units", 0);
			checkbox("Hide Shots", &Config::AntiAim::AAMisc::get().hide_shots);

		}
		if (tab_selected == 3)//NONONO, this is Player stuff, dummy, like esp, chams, n glow, smh
		{
			groupbox(30, 77, GroupBoxSize_Width * 1.8, GroupBoxSize_Height, "General ESP");
			checkbox("Enable", &Config::Visuals::Main::Visualsmain::get().Enable);
			checkbox("Teammates", &Config::Visuals::Main::Visualsmain::get().Team);
			color_selector("box", &Config::Visuals::Main::Visualsmain::get().box_color_r, &Config::Visuals::Main::Visualsmain::get().box_color_g, &Config::Visuals::Main::Visualsmain::get().box_color_b, &Config::Visuals::Main::Visualsmain::get().box_color_a);
			checkbox("Box", &Config::Visuals::Main::Visualsmain::get().Box);
			color_selector("name", &Config::Visuals::Main::Visualsmain::get().name_color_r, &Config::Visuals::Main::Visualsmain::get().name_color_g, &Config::Visuals::Main::Visualsmain::get().name_color_b, &Config::Visuals::Main::Visualsmain::get().name_color_a);
			checkbox("Name", &Config::Visuals::Main::Visualsmain::get().Name);
			checkbox("HealthBar", &Config::Visuals::Main::Visualsmain::get().HealthBar);
			color_selector("skeleton", &Config::Visuals::Main::Visualsmain::get().skeleton_color_r, &Config::Visuals::Main::Visualsmain::get().skeleton_color_g, &Config::Visuals::Main::Visualsmain::get().skeleton_color_b, &Config::Visuals::Main::Visualsmain::get().skeleton_color_a);
			checkbox("Skeleton", &Config::Visuals::Main::Visualsmain::get().Skeleton);
			color_selector("btskeleton", &Config::Visuals::Main::Visualsmain::get().btskeleton_color_r, &Config::Visuals::Main::Visualsmain::get().btskeleton_color_g, &Config::Visuals::Main::Visualsmain::get().btskeleton_color_b, &Config::Visuals::Main::Visualsmain::get().btskeleton_color_a);
			checkbox("Backtrack Skeleton", &Config::Visuals::Main::Visualsmain::get().BacktrackSkeleton);
			color_selector("dist", &Config::Visuals::Main::Visualsmain::get().distance_color_r, &Config::Visuals::Main::Visualsmain::get().distance_color_g, &Config::Visuals::Main::Visualsmain::get().distance_color_b, &Config::Visuals::Main::Visualsmain::get().distance_color_a);
			checkbox("Distance", &Config::Visuals::Main::Visualsmain::get().Distance);
			color_selector("tex", &Config::Visuals::Main::Visualsmain::get().weaponname_color_r, &Config::Visuals::Main::Visualsmain::get().weaponname_color_g, &Config::Visuals::Main::Visualsmain::get().weaponname_color_b, &Config::Visuals::Main::Visualsmain::get().weaponname_color_a);
			checkbox("Weapon Text", &Config::Visuals::Main::Visualsmain::get().WeaponText);
			color_selector("icon", &Config::Visuals::Main::Visualsmain::get().weaponicon_color_r, &Config::Visuals::Main::Visualsmain::get().weaponicon_color_g, &Config::Visuals::Main::Visualsmain::get().weaponicon_color_b, &Config::Visuals::Main::Visualsmain::get().weaponicon_color_a);
			checkbox("Weapon Icon", &Config::Visuals::Main::Visualsmain::get().WeaponIcon);
			color_selector("ammo", &Config::Visuals::Main::Visualsmain::get().ammobar_color_r, &Config::Visuals::Main::Visualsmain::get().ammobar_color_g, &Config::Visuals::Main::Visualsmain::get().ammobar_color_b, &Config::Visuals::Main::Visualsmain::get().ammobar_color_a);
			checkbox("Ammo Bar", &Config::Visuals::Main::Visualsmain::get().AmmoBar);
			color_selector("armor", &Config::Visuals::Main::Visualsmain::get().armorbar_color_r, &Config::Visuals::Main::Visualsmain::get().armorbar_color_g, &Config::Visuals::Main::Visualsmain::get().armorbar_color_b, &Config::Visuals::Main::Visualsmain::get().armorbar_color_a);
			checkbox("Armor Bar", &Config::Visuals::Main::Visualsmain::get().ArmorBar);
			checkbox("Flags", &Config::Visuals::Main::Visualsmain::get().Flags);
			color_selector("arrowss", &Config::Visuals::Main::Visualsmain::get().arrows_color_r, &Config::Visuals::Main::Visualsmain::get().arrows_color_g, &Config::Visuals::Main::Visualsmain::get().arrows_color_b, &Config::Visuals::Main::Visualsmain::get().arrows_color_a);
			checkbox("Arrows", &Config::Visuals::Main::Visualsmain::get().Arrows);
			slider(30, "Arrow-Size", &Config::Visuals::Main::Visualsmain::get().arrows_size, "px", 0);
			slider(400, "Arrow-Radius", &Config::Visuals::Main::Visualsmain::get().arrows_rad, "px", 0);
			color_selector("sound", &Config::Visuals::Main::Visualsmain::get().soundesp_color_r, &Config::Visuals::Main::Visualsmain::get().soundesp_color_g, &Config::Visuals::Main::Visualsmain::get().soundesp_color_b, &Config::Visuals::Main::Visualsmain::get().soundesp_color_a);
			checkbox("SoundESP", &Config::Visuals::Main::Visualsmain::get().SoundESP);
			slider(1500, "SoundESP Size", &Config::Visuals::Main::Visualsmain::get().soundesp_size, "px", 0);
			slider(150, "SoundESP Speed", &Config::Visuals::Main::Visualsmain::get().soundesp_speed, "u", 0);
			groupbox(349, 77, GroupBoxSize_Width * 1.8, GroupBoxSize_Height, "Player Models");
			std::string chams[4] = { "Enemy","Team","Local","Other" };
			std::string chams_selector[6] = { "none","textured","flat","metalic","armspace","devglow" };
			button(reload, "Reload Materials");
			MultiComboBox(4, "Chams", chams, Config::Visuals::Main::Visualschams::get().Chams);
			if (Config::Visuals::Main::Visualschams::get().Chams[0]) {
				color_selector_chams("enemychams", &Config::Visuals::Main::Visualschams::get().enemy_chams_r, &Config::Visuals::Main::Visualschams::get().enemy_chams_g, &Config::Visuals::Main::Visualschams::get().enemy_chams_b, &Config::Visuals::Main::Visualschams::get().enemy_chams_a, &Config::Visuals::Main::Visualschams::get().enemy_chams_w);
				combobox(6, "Enemy Chams", chams_selector, &Config::Visuals::Main::Visualschams::get().enemy_chams);
				color_selector_chams("enemychamsxqz", &Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_r, &Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_g, &Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_b, &Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_a, &Config::Visuals::Main::Visualschams::get().enemy_chams_xqz_w);
				checkbox("Enemy XQZ Chams", &Config::Visuals::Main::Visualschams::get().enemy_xyz_chams);
				color_selector("btchams", &Config::Visuals::Main::Visualschams::get().backtrack_chams_r, &Config::Visuals::Main::Visualschams::get().backtrack_chams_g, &Config::Visuals::Main::Visualschams::get().backtrack_chams_b, &Config::Visuals::Main::Visualschams::get().backtrack_chams_a);
				checkbox("Backtrack chams", &Config::Visuals::Main::Visualschams::get().backtrack_chams);
				color_selector("enemyglow", &Config::Visuals::Main::Visualschams::get().enemy_glow_r, &Config::Visuals::Main::Visualschams::get().enemy_glow_g, &Config::Visuals::Main::Visualschams::get().enemy_glow_b, &Config::Visuals::Main::Visualschams::get().enemy_glow_a);
				checkbox("Enemy Glow", &Config::Visuals::Main::Visualschams::get().enemy_glow);
			}
			if (Config::Visuals::Main::Visualschams::get().Chams[1]) {
				color_selector_chams("teamchams", &Config::Visuals::Main::Visualschams::get().team_chams_r, &Config::Visuals::Main::Visualschams::get().team_chams_g, &Config::Visuals::Main::Visualschams::get().team_chams_b, &Config::Visuals::Main::Visualschams::get().team_chams_a, &Config::Visuals::Main::Visualschams::get().team_chams_w);
				combobox(6, "Team Chams", chams_selector, &Config::Visuals::Main::Visualschams::get().team_chams);
				color_selector_chams("teamchamsxqz", &Config::Visuals::Main::Visualschams::get().team_chams_xqz_r, &Config::Visuals::Main::Visualschams::get().team_chams_xqz_g, &Config::Visuals::Main::Visualschams::get().team_chams_xqz_b, &Config::Visuals::Main::Visualschams::get().team_chams_xqz_a, &Config::Visuals::Main::Visualschams::get().team_chams_xqz_w);
				checkbox("Team XQZ Chams", &Config::Visuals::Main::Visualschams::get().team_xyz_chams);
				color_selector("teamglow", &Config::Visuals::Main::Visualschams::get().team_glow_r, &Config::Visuals::Main::Visualschams::get().team_glow_g, &Config::Visuals::Main::Visualschams::get().team_glow_b, &Config::Visuals::Main::Visualschams::get().team_glow_a);
				checkbox("Team Glow", &Config::Visuals::Main::Visualschams::get().team_glow);
			}
			if (Config::Visuals::Main::Visualschams::get().Chams[2]) {
				color_selector_chams("localchams", &Config::Visuals::Main::Visualschams::get().local_chams_r, &Config::Visuals::Main::Visualschams::get().local_chams_g, &Config::Visuals::Main::Visualschams::get().local_chams_b, &Config::Visuals::Main::Visualschams::get().local_chams_a, &Config::Visuals::Main::Visualschams::get().local_chams_w);
				combobox(6, "Local Chams", chams_selector, &Config::Visuals::Main::Visualschams::get().local_chams);
				color_selector_chams("desyncchams", &Config::Visuals::Main::Visualschams::get().desync_chams_r, &Config::Visuals::Main::Visualschams::get().desync_chams_g, &Config::Visuals::Main::Visualschams::get().desync_chams_b, &Config::Visuals::Main::Visualschams::get().desync_chams_a, &Config::Visuals::Main::Visualschams::get().desync_chams_w);
				combobox(6, "Desync Chams", chams_selector, &Config::Visuals::Main::Visualschams::get().desync_chams);
				color_selector("fakelagchams", &Config::Visuals::Main::Visualschams::get().fakelag_chams_r, &Config::Visuals::Main::Visualschams::get().fakelag_chams_g, &Config::Visuals::Main::Visualschams::get().fakelag_chams_b, &Config::Visuals::Main::Visualschams::get().fakelag_chams_a);
				combobox(6, "Fakelag Chams", chams_selector, &Config::Visuals::Main::Visualschams::get().fakelag_chams);
				color_selector("localglow", &Config::Visuals::Main::Visualschams::get().local_glow_r, &Config::Visuals::Main::Visualschams::get().local_glow_g, &Config::Visuals::Main::Visualschams::get().local_glow_b, &Config::Visuals::Main::Visualschams::get().local_glow_a);
				checkbox("Local Glow", &Config::Visuals::Main::Visualschams::get().local_glow);
			}
			if (Config::Visuals::Main::Visualschams::get().Chams[3]) {
				color_selector_chams("handchams", &Config::Visuals::Main::Visualschams::get().hand_chams_r, &Config::Visuals::Main::Visualschams::get().hand_chams_g, &Config::Visuals::Main::Visualschams::get().hand_chams_b, &Config::Visuals::Main::Visualschams::get().hand_chams_a, &Config::Visuals::Main::Visualschams::get().hand_chams_w);
				combobox(6, "Hand Chams", chams_selector, &Config::Visuals::Main::Visualschams::get().hand_chams);
				checkbox("Hand Wireframe Chams", &Config::Visuals::Main::Visualschams::get().hand_chams_wireframe);
				color_selector_chams("weaponchams", &Config::Visuals::Main::Visualschams::get().weapon_chams_r, &Config::Visuals::Main::Visualschams::get().weapon_chams_g, &Config::Visuals::Main::Visualschams::get().weapon_chams_b, &Config::Visuals::Main::Visualschams::get().weapon_chams_a, &Config::Visuals::Main::Visualschams::get().weapon_chams_w);
				combobox(6, "Weapon Chams", chams_selector, &Config::Visuals::Main::Visualschams::get().weapon_chams);
				checkbox("Weapon Wireframe Chams", &Config::Visuals::Main::Visualschams::get().weapon_chams_wireframe);
			}
		}
		if (tab_selected == 4)//Visuals
		{
			std::string droppedweapons[4] = { "Icon","Text","Distance","Glow" };
			std::string sitem[5] = { "Keystrokes","Indicators","Watermark","Circles", "AA indicators" };
			groupbox(30, 77, GroupBoxSize_Width * 1.8, GroupBoxSize_Height / 2.1, "Dropps");
			color_selector("droppedweapon", &Config::Visuals::Main::Visualsworld::get().droppedweapon_color_r, &Config::Visuals::Main::Visualsworld::get().droppedweapon_color_g, &Config::Visuals::Main::Visualsworld::get().droppedweapon_color_b, &Config::Visuals::Main::Visualsworld::get().droppedweapon_color_a);
			MultiComboBox(4, "Dropped Weapon", droppedweapons, Config::Visuals::Main::Visualsworld::get().DroppedWeapons);
			color_selector("droppedweapon_ammo", &Config::Visuals::Main::Visualsworld::get().droppedweapon_ammo_color_r, &Config::Visuals::Main::Visualsworld::get().droppedweapon_ammo_color_g, &Config::Visuals::Main::Visualsworld::get().droppedweapon_ammo_color_b, &Config::Visuals::Main::Visualsworld::get().droppedweapon_ammo_color_a);
			checkbox("Dropped Weapon Ammo", &Config::Visuals::Main::Visualsworld::get().DroppedWeaponAmmo);
			color_selector("droppedgranades", &Config::Visuals::Main::Visualsworld::get().droppedgranades_color_a, &Config::Visuals::Main::Visualsworld::get().droppedgranades_color_g,&Config::Visuals::Main::Visualsworld::get().droppedgranades_color_b, &Config::Visuals::Main::Visualsworld::get().droppedgranades_color_a);
			checkbox("Dropped Grenades", &Config::Visuals::Main::Visualsworld::get().DroppedGrenades);
			color_selector("bomb", &Config::Visuals::Main::Visualsworld::get().bomb_color_a, &Config::Visuals::Main::Visualsworld::get().bomb_color_g, &Config::Visuals::Main::Visualsworld::get().bomb_color_b, &Config::Visuals::Main::Visualsworld::get().bomb_color_a);
			checkbox("Bomb", &Config::Visuals::Main::Visualsworld::get().Bomb);

			groupbox(30, 312.5, GroupBoxSize_Width * 1.8, GroupBoxSize_Height / 2.1 + 15, "World");
			checkbox("Grenade Warning", &Config::Visuals::Main::Visualsworld::get().GranadeWaring);
			checkbox("Penetration Crosshair", &Config::Visuals::Main::Visualsworld::get().PenetrationCrosshair);
			checkbox("Spectators", &Config::Visuals::Main::Visualsworld::get().Spectators);
			checkbox("Taser Range", &Config::Visuals::Main::Visualsworld::get().VisualizeTaserRange);
			checkbox("Spread Circle", &Config::Visuals::Main::Visualsworld::get().SpreadCircle);
			color_selector("granadetrajectory", &Config::Visuals::Main::Visualsworld::get().granadetrajectory_color_r, &Config::Visuals::Main::Visualsworld::get().granadetrajectory_color_g, &Config::Visuals::Main::Visualsworld::get().granadetrajectory_color_b, &Config::Visuals::Main::Visualsworld::get().granadetrajectory_color_a);
			checkbox("Granade Trajectory", &Config::Visuals::Main::Visualsworld::get().GranadeTrajectory);
			MultiComboBox(5, "Screen Effects", sitem, Config::Visuals::Main::Visualsmain::get().screen);
			text("ThridPerson Key");
			keybind(&Config::Visuals::Main::VisualsEffects::get().ThirdPerson, "a");

			groupbox(349, 77, GroupBoxSize_Width * 1.8, GroupBoxSize_Height, "Effects");
			std::string removals[5] = { "Flash","Smoke","Scope","Zoom", "Recoil" };
			std::string lc[4] = { "none","chams","skeleton","capsule" };
			std::string hits[5] = { "screen effect","hitsound","screen hitmarker","player hitmarker", "damage esp" };
			std::string impacts[3] = { "none","color box","red and blue box" };
			std::string skyboxes[26] = { "none","cs_baggage_skybox","cs_tibet","clearsky", "clearsky_hdr","embassy","italy","jungle","nukeblank","office","sky_cs15_daylight01_hdr","sky_cs15_daylight02_hdr","sky_cs15_daylight03_hdr","sky_cs15_daylight04_hdr","sky_csgo_cloudy01","sky_csgo_night02","sky_csgo_night02b","sky_csgo_night_flat","sky_day02_05_hdr","sky_day02_05","sky_dust","sky_l4d_rural02_ldr","vertigo_hdr","vertigoblue_hdr","vertigo","vietnam" };
			std::string tracers[9] = { "none","physbeam", "blueglow1", "bubble","glow01","purpleglow1", "purplelaser1", "radio","white" };
			MultiComboBox(5, "Remove Effect", removals, Config::Visuals::Main::VisualsEffects::get().RemoveEffect);
			color_selector("worldc", &Config::Visuals::Main::VisualsEffects::get().world_color_r, &Config::Visuals::Main::VisualsEffects::get().world_color_g, &Config::Visuals::Main::VisualsEffects::get().world_color_b, &Config::Visuals::Main::VisualsEffects::get().world_color_a);
			sliderfloat(100, "Brightness", &Config::Visuals::Main::VisualsEffects::get().Brightness, "%", 1);
			sliderfloat(100, "Asus Walls", &Config::Visuals::Main::VisualsEffects::get().AsusWalls, "%", 1);
			sliderfloat(100, "Asus Props", &Config::Visuals::Main::VisualsEffects::get().AsusProps, "%", 1);
			combobox(26, "Skybox", skyboxes, &Config::Visuals::Main::VisualsEffects::get().Skybox);
			button(mod, "Modulate");
			checkbox("Full Bright", &Config::Visuals::Main::VisualsEffects::get().FullBright);
			MultiComboBox(5, "Hit Effect", hits, Config::Visuals::Main::VisualsEffects::get().HitEffect);
			combobox(4, "DrawLagComp", lc, &Config::Visuals::Main::VisualsEffects::get().DrawLC);
			checkbox("Disable Post Processing", &Config::Visuals::Main::VisualsEffects::get().DisablePostProcessing);
			color_selector("bullet_col", &Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_r, &Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_g, &Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_b, &Config::Visuals::Main::VisualsEffects::get().bulletimapcts_color_a);
			combobox(3, "Bullet Impacts", impacts, &Config::Visuals::Main::VisualsEffects::get().BulletImapcts);
			color_selector("bullettr_col", &Config::Visuals::Main::VisualsEffects::get().bullettracers_color_r, &Config::Visuals::Main::VisualsEffects::get().bullettracers_color_g, &Config::Visuals::Main::VisualsEffects::get().bullettracers_color_b, &Config::Visuals::Main::VisualsEffects::get().bullettracers_color_a);
			checkbox("Bullet Tracers", &Config::Visuals::Main::VisualsEffects::get().BulletTracers);

		}
		if (tab_selected == 5) {
			groupbox(30, 77, GroupBoxSize_Width * 1.8, GroupBoxSize_Height, "Main");
			checkbox("Anti Untrusted", &Config::Misc::MiscMain::get().anti_untrasted);
			//checkbox("Disable Panorama blur", &Config::Misc::MiscMain::get().dispanblur);
			std::string mainweapons[3] = { "scar20/g3sg1","ssg 08","awp" };
			std::string secweapons[2] = { "dual barretas", "deagle/revoler" };
			std::string grenade[4] = { "flashbang", "smoke", "hegrenade", "fire grenade" };
			std::string other[4] = { "vest", "vesthelm", "taser", "defuser" };
			slider(130, "Override FOV", &Config::Misc::MiscMain::get().view_fov, "", 60);
			slider(130, "ViewModel FOV", &Config::Misc::MiscMain::get().view_model_fov, "", 60);
			checkbox("View-Model Changer ", &Config::Misc::MiscMain::get().viewmodelchanger);
			if (Config::Misc::MiscMain::get().viewmodelchanger) {
				slider(20, "ViewModel Changer X", &Config::Misc::MiscMain::get().viewmodel_x, "", -20);
				slider(20, "ViewModel Changer Y", &Config::Misc::MiscMain::get().viewmodel_y, "", -20);
				slider(20, "ViewModel Changer Z", &Config::Misc::MiscMain::get().viewmodel_z, "", -20);
			}
			checkbox("Auto BunnyHOP", &Config::Misc::MiscMain::get().auto_bunnyHop);
			checkbox("AutoStrafer", &Config::Misc::MiscMain::get().autostrafer);
			checkbox("Auto Zeus", &Config::Misc::MiscMain::get().auto_zeus);
			checkbox("Auto Knife", &Config::Misc::MiscMain::get().auto_knife);
			checkbox("Walk Bot", &Config::Misc::MiscMain::get().walk_bot);
			checkbox("Block Bot", &Config::Misc::MiscMain::get().block_bot);
			checkbox("Buy Bot", &Config::Misc::MiscMain::get().buy_bot);
			if (Config::Misc::MiscMain::get().buy_bot) {
				combobox(3, "Main Weapons", mainweapons, &Config::Misc::MiscMain::get().weapon_main);
				combobox(2, "Secondary Weapons", secweapons, &Config::Misc::MiscMain::get().weapon_sec);
				MultiComboBox(4, "Greandes", grenade, Config::Misc::MiscMain::get().granade);
				MultiComboBox(4, "Other:", other, Config::Misc::MiscMain::get().other);
			}
			checkbox("Infintiy duck", &Config::Misc::MiscMain::get().infduck);
			text("Fakeduck Key");
			keybind(&Config::Misc::MiscMain::get().fakeduck, "fd");

			groupbox(349, 77, GroupBoxSize_Width * 1.8, GroupBoxSize_Height / 2, "Others");
			checkbox("Name Stealer", &Config::Misc::MiscMain::get().name_stealer);
			checkbox("Log", &Config::Misc::MiscMain::get().log);
			checkbox("Clantag Spammer", &Config::Misc::MiscMain::get().clantag_spamer);

			groupbox(349, 302, GroupBoxSize_Width * 1.8, GroupBoxSize_Height / 2, "Radio");
			std::string radio_name[9] = { "DutchCore","HardStyle","BigFm","BigFm deutsch rap","Radio Record","Record Dubstep","Record Hardstyle","Hard-bass","Hip-Hop" };
			checkbox("Enable Radio", &Config::Misc::RadioMain::get().Enable);
			sliderfloat(1, "Radio volume", &Config::Misc::RadioMain::get().volume, "%", 0);
			combobox(9, "Station", radio_name, &Config::Misc::RadioMain::get().radio_station);
		}
		if (tab_selected == 6) {//Playerlist
			groupbox(30, 77, GroupBoxSize_Width * 1.8, GroupBoxSize_Height, "Players");
			groupbox(349, 77, GroupBoxSize_Width * 1.8, GroupBoxSize_Height, "Flags");

		}

		if (tab_selected == 7) {//Skins

			std::string knives[20] = { " default"," bayonet"," flip knife"," gut knife"," karambit"," m9 bayonet"," huntsman knife"," falchion knife"," bowie knife"," butterfly knife"," shadow daggers"," talon knife"," navaja knife"," stiletto knife"," ursus knife"," cs20 knife", " skeleton knife", " survival kinfe", " paracord knife", " nomad knife"};
			std::string ssg08_skins[7] = { "default", "dragonfire", "fuschia", "whiteout", "acid fade", "tiger thot", "fade" };
			groupbox(30, 77, GroupBoxSize_Width * 1.8, GroupBoxSize_Height, "Part 1");
			button(KnifeApplyCallbk, "update skins");
			combobox(20, "Knife Model", knives, &Config::SkinChanger::Knifes::get().knifemodel);
			combobox(6, "SSG08 Skin", ssg08_skins, &Config::SkinChanger::Knifes::get().ssg08_skin);
			text("isnt finished{model changer}");
		//	combobox(26, "Model Changer", model_changer, &Config::SkinChanger::Model::get().model_changer);

			button(modelchanger_plus, "+");
			button(modelchanger_minus, "+");
		}

		if (tab_selected == 8) {
			groupbox(30, 77, GroupBoxSize_Width * 1.8, GroupBoxSize_Height, "Xtra");
			combobox(10, "Configuration", config_slots, &save_slot);
			button(Save_Config, "Save");
			button(Load_Config, "Load");
			groupbox(349, 77, GroupBoxSize_Width * 1.85, GroupBoxSize_Height, "Other");
			color_selector("Menu Color", &Config::ConfigMenu::ConfigMain::get().menu_color_r, &Config::ConfigMenu::ConfigMain::get().menu_color_g,
				&Config::ConfigMenu::ConfigMain::get().menu_color_b, &Config::ConfigMenu::ConfigMain::get().menu_color_a);
			text("Main Menu Color");
			slider(100, "Slider1", &Config::ConfigMenu::ConfigMain::get().slider1, "", 1);
			slider(100, "Slider2", &Config::ConfigMenu::ConfigMain::get().slider2, "", 1);
			slider(50, "Slider3", &Config::ConfigMenu::ConfigMain::get().slider3, "", 1);
			slider(50, "Slider4", &Config::ConfigMenu::ConfigMain::get().slider4, "", 1);
			slider(500, "Slider5", &Config::ConfigMenu::ConfigMain::get().slider5, "", 1);
			slider(100, "Slider6", &Config::ConfigMenu::ConfigMain::get().slider6, "", 1);
		}
	}
	INTERFACES::Surface->clipping_enabled() = false;
}

void Menu::color_selector(std::string name, float* red, float* green, float* blue, float* alpha)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 127; // 163
	int ComboSizeH = 127; // 163
	static int offset = 0;
	if (name == "sadasd")
	{
		offset = 0;
	}
	else
	{
		offset = 0 /*14*/;
	}

	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 225, y_offset + 1, 15, 8) && !
			click_rest)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 225, y_offset + 1, 15, 8))
			click_rest = false;

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name)
			open = selected_opened;
	}


	static float r = 255;
	static float g = 0;
	static float b = 0;

	static float selected_r = 0;
	static float selected_g = 0;
	static float selected_b = 0;

	static Vector2D MousePos;
	COLORREF color;
	COLORREF selected_color;

	if (open)
	{
		dont_recieve_input = true;
		RENDER::FillRectangle(x_offset + 225 - ComboSize - 5, y_offset, ComboSize, ComboSizeH,
			CColor(21, 23, 27, MenuAlpha_Main));

		MENU_DRAW::OutlinedRect(x_offset + 225 - ComboSize - 5, y_offset, ComboSize, ComboSizeH,
			CColor(42, 46, 46, MenuAlpha_Main));
		MENU_DRAW::OutlinedRect(x_offset + 225 - ComboSize - 5 - 1, y_offset - 1, ComboSize + 2, ComboSizeH + 2,
			CColor(34, 38, 37, MenuAlpha_Main));

		float _pixel_value = 255 / (ComboSize - 5 - 17);

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 225 - ComboSize - 5 + 5,
			y_offset + ComboSizeH - 11, (ComboSize - 5 - 17),
			7) && menuOpened)
		{
			*alpha = abs(MENU_DRAW::GetMousePosition().x - (x_offset + 225 - ComboSize - 5 + 5)) * _pixel_value;
		}


		if (*alpha < 3)
		{
			*alpha = 0;
		}

		if (*alpha > 204)
		{
			*alpha = 255;
		}

		RENDER::FillRectangle(x_offset + 225 - ComboSize - 5 + 5, y_offset + ComboSizeH - 11, ComboSize - 5 - 17, 7,
			CColor(255, 255, 255, *alpha));
		MENU_DRAW::OutlinedRect(x_offset + 225 - ComboSize - 5 + 5, y_offset + ComboSizeH - 11, ComboSize - 5 - 17, 7,
			CColor(0, 0, 0, MenuAlpha_Main));


		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 233 - ComboSize + 101 + 1,
			y_offset + 5 + 1, 7 - 2, 107 - 1))
		{
			HDC hdc = GetDC(nullptr);
			color = GetPixel(hdc, MENU_DRAW::GetMousePosition().x, MENU_DRAW::GetMousePosition().y);

			r = GetRValue(color);
			g = GetGValue(color);
			b = GetBValue(color);
		}


		GradientH(x_offset + 225 - ComboSize - 5 + 5, y_offset + 5, ComboSize - 5 - 17, ComboSizeH - 20,
			CColor(255, 255, 255), CColor(r, g, b));
		GradientVEnd(x_offset + 225 - ComboSize - 5 + 5, y_offset + 40, ComboSize - 5 - 17, ComboSizeH - 20,
			CColor(0, 0, 0), CColor(0, 0, 0));


		//MENU_DRAW::OutlinedRect(MousePos.x - 4, MousePos.y - 4, 8, 8, Color(0, 0, 0, MenuAlpha_Main));
		MENU_DRAW::OutlinedRect(x_offset + 225 - ComboSize - 5 + 5, y_offset + 5, ComboSize - 5 - 17, ComboSizeH - 20,
			CColor(0, 0, 0, MenuAlpha_Main));

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 225 - ComboSize - 5 + 5 + 1,
			y_offset + 5 + 1, ComboSize - 5 - 17 - 2,
			ComboSizeH - 20 - 2))
		{
			HDC hdc = GetDC(nullptr);
			selected_color = GetPixel(hdc, MENU_DRAW::GetMousePosition().x, MENU_DRAW::GetMousePosition().y);

			selected_r = GetRValue(selected_color);
			selected_g = GetGValue(selected_color);
			selected_b = GetBValue(selected_color);

			*red = selected_r;
			*green = selected_g;
			*blue = selected_b;

			MousePos = Vector2D(MENU_DRAW::GetMousePosition().x, MENU_DRAW::GetMousePosition().y);
		}

		CColor rainbow; float hue;
		for (int i = 0; i < 100; i++)
		{
			float hue = (i * .01f);
			rainbow.FromHSV(hue, 1.f, 1.f, rainbow);
			RENDER::FillRectangle(x_offset + 233 - ComboSize + 101, y_offset + 5 + i, 7, 8, rainbow);
			MENU_DRAW::OutlinedRect(x_offset + 233 - ComboSize + 101, y_offset + 5, 7, 107, CColor(0, 0, 0, 255));
		}
	}
	else
	{
		dont_recieve_input = false;
	}


	if (*red > 20 && *green > 20 && *blue > 20)
	{
		RENDER::FillRectangle(x_offset + 225, y_offset, 15, 7, CColor(*red, *green, *blue, MenuAlpha_Main));
	}
	else
	{
		RENDER::FillRectangle(x_offset + 225, y_offset, 15, 7, CColor(*red, *green, *blue, MenuAlpha_Main));
	}


	MENU_DRAW::OutlinedRect(x_offset + 225 - 1, y_offset - 1, 15 + 2, 7 + 2, CColor(0, 0, 0, MenuAlpha_Main));

	if (open)
	{
		y_offset += ComboSizeH;
	}


}
void Menu::color_selector_chams(std::string name, float* red, float* green, float* blue, float* alpha, float* width)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 127; // 163
	int ComboSizeH = 138; // 163
	static int offset = 0;
	if (name == "sadasd")
	{
		offset = 0;
	}
	else
	{
		offset = 0 /*14*/;
	}

	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 225, y_offset + 1, 15, 8) && !
			click_rest)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 225, y_offset + 1, 15, 8))
			click_rest = false;

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name)
			open = selected_opened;
	}


	static float r = 255;
	static float g = 0;
	static float b = 0;

	static float selected_r = 0;
	static float selected_g = 0;
	static float selected_b = 0;

	static Vector2D MousePos;
	COLORREF color;
	COLORREF selected_color;

	if (open)
	{
		dont_recieve_input = true;
		RENDER::FillRectangle(x_offset + 225 - ComboSize - 5, y_offset, ComboSize, ComboSizeH,
			CColor(21, 23, 27, MenuAlpha_Main));

		MENU_DRAW::OutlinedRect(x_offset + 225 - ComboSize - 5, y_offset, ComboSize, ComboSizeH,
			CColor(42, 46, 46, MenuAlpha_Main));
		MENU_DRAW::OutlinedRect(x_offset + 225 - ComboSize - 5 - 1, y_offset - 1, ComboSize + 2, ComboSizeH + 2,
			CColor(34, 38, 37, MenuAlpha_Main));

		float _pixel_value = 255 / (ComboSize - 5 - 17);

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 225 - ComboSize - 5 + 5,
			y_offset + ComboSizeH - 11, (ComboSize - 5 - 17),
			7) && menuOpened)
		{
			*alpha = abs(MENU_DRAW::GetMousePosition().x - (x_offset + 225 - ComboSize - 5 + 5)) * _pixel_value;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 225 - ComboSize - 5 + 5,
			y_offset + ComboSizeH - 21, (ComboSize - 5 - 17),
			7) && menuOpened)
		{
			*width = abs(MENU_DRAW::GetMousePosition().x - (x_offset + 225 - ComboSize - 5 + 5)) * _pixel_value;
		}


		if (*alpha < 3)
		{
			*alpha = 0;
		}

		if (*alpha > 204)
		{
			*alpha = 255;
		}

		if (*width < 3)
		{
			*width = 0;
		}

		if (*width > 204)
		{
			*width = 255;
		}

		RENDER::FillRectangle(x_offset + 225 - ComboSize - 5 + 5, y_offset + ComboSizeH - 11, ComboSize - 5 - 17, 7,
			CColor(255, 255, 255, *alpha));
		MENU_DRAW::OutlinedRect(x_offset + 225 - ComboSize - 5 + 5, y_offset + ComboSizeH - 11, ComboSize - 5 - 17, 7,
			CColor(0, 0, 0, MenuAlpha_Main));

		RENDER::FillRectangle(x_offset + 225 - ComboSize - 5 + 5, y_offset + ComboSizeH - 21, ComboSize - 5 - 17, 7,
			CColor(255, 255, 255, *width));
		MENU_DRAW::OutlinedRect(x_offset + 225 - ComboSize - 5 + 5, y_offset + ComboSizeH - 21, ComboSize - 5 - 17, 7,
			CColor(0, 0, 0, MenuAlpha_Main));


		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 233 - ComboSize + 101 + 1,
			y_offset + 5 + 1, 7 - 2, 107 - 1))
		{
			HDC hdc = GetDC(nullptr);
			color = GetPixel(hdc, MENU_DRAW::GetMousePosition().x, MENU_DRAW::GetMousePosition().y);

			r = GetRValue(color);
			g = GetGValue(color);
			b = GetBValue(color);
		}


		GradientH(x_offset + 225 - ComboSize - 5 + 5, y_offset + 5, ComboSize - 5 - 17, ComboSizeH - 31,
			CColor(255, 255, 255), CColor(r, g, b));
		GradientVEnd(x_offset + 225 - ComboSize - 5 + 5, y_offset + 40, ComboSize - 5 - 17, ComboSizeH - 31,
			CColor(0, 0, 0), CColor(0, 0, 0));


		//MENU_DRAW::OutlinedRect(MousePos.x - 4, MousePos.y - 4, 8, 8, Color(0, 0, 0, MenuAlpha_Main));
		MENU_DRAW::OutlinedRect(x_offset + 225 - ComboSize - 5 + 5, y_offset + 5, ComboSize - 5 - 17, ComboSizeH - 31,
			CColor(0, 0, 0, MenuAlpha_Main));

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 225 - ComboSize - 5 + 5 + 1,
			y_offset + 5 + 1, ComboSize - 5 - 17 - 2,
			ComboSizeH - 31 - 2))
		{
			HDC hdc = GetDC(nullptr);
			selected_color = GetPixel(hdc, MENU_DRAW::GetMousePosition().x, MENU_DRAW::GetMousePosition().y);

			selected_r = GetRValue(selected_color);
			selected_g = GetGValue(selected_color);
			selected_b = GetBValue(selected_color);

			*red = selected_r;
			*green = selected_g;
			*blue = selected_b;

			MousePos = Vector2D(MENU_DRAW::GetMousePosition().x, MENU_DRAW::GetMousePosition().y);
		}

		CColor rainbow; float hue;
		for (int i = 0; i < 100; i++)
		{
			float hue = (i * .01f);
			rainbow.FromHSV(hue, 1.f, 1.f, rainbow);
			RENDER::FillRectangle(x_offset + 233 - ComboSize + 101, y_offset + 5 + i, 7, 8, rainbow);
			MENU_DRAW::OutlinedRect(x_offset + 233 - ComboSize + 101, y_offset + 5, 7, 107, CColor(0, 0, 0, 255));
		}
	}
	else
	{
		dont_recieve_input = false;
	}


	if (*red > 20 && *green > 20 && *blue > 20)
	{
		RENDER::FillRectangle(x_offset + 225, y_offset, 15, 7, CColor(*red, *green, *blue, MenuAlpha_Main));
	}
	else
	{
		RENDER::FillRectangle(x_offset + 225, y_offset, 15, 7, CColor(*red, *green, *blue, MenuAlpha_Main));
	}


	MENU_DRAW::OutlinedRect(x_offset + 225 - 1, y_offset - 1, 15 + 2, 7 + 2, CColor(0, 0, 0, MenuAlpha_Main));

	if (open)
	{
		y_offset += ComboSizeH;
	}


}

void Menu::hsv_to_rgb(float h, float s, float v, float& out_r, float& out_g, float& out_b) const
{
	if (s == 0.0f)
	{
		// gray
		out_r = out_g = out_b = v;
		return;
	}

	h = std::fmod(h, 1.0f) / (60.0f / 360.0f);
	int i = (int)h;
	float f = h - (float)i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (i)
	{
	case 0:
		out_r = v;
		out_g = t;
		out_b = p;
		break;

	case 1:
		out_r = q;
		out_g = v;
		out_b = p;
		break;

	case 2:
		out_r = p;
		out_g = v;
		out_b = t;
		break;

	case 3:
		out_r = p;
		out_g = q;
		out_b = v;
		break;

	case 4:
		out_r = t;
		out_g = p;
		out_b = v;
		break;

	case 5:
	default:
		out_r = v;
		out_g = p;
		out_b = q;
		break;
	}
}

CColor Menu::hsv_to_rgb(float h, float s, float v)
{
	float r = 255.f, g = 255.f, b = 255.f;

	hsv_to_rgb(h, s, v, r, g, b);

	return CColor(r, g, b, 255.f);
}

void Menu::colorp(CColor* clr, bool* open, float offset)
{
	int current_section_components_width = 246;
	int last_element_y = 0;
	int current_section_width = 250;
	int current_x_add = 0;
	int current_y_add = 0;
	Vector2D draw_pos = Vector2D(50, 50);
	//float p = 50.f / 360.f;
	int x = draw_pos.x + current_x_add + current_section_width - 15;
	int y = draw_pos.y + last_element_y + offset + 2;

	bool clr_box_in_hover = MENU_DRAW::MouseInRegion(x, y, 11, 8);

	RENDER::FillRectangle(x, y, 11, 8, *clr);
	bool was_m1_pressed = (GetAsyncKeyState(VK_LBUTTON) & 1);

	if (clr_box_in_hover && was_m1_pressed)
		* open = !*open;

	if (!*open)
		GradientH(x, y, 11, 8, CColor(255, 255, 255, 255), CColor(30, 30, 30, 255));

	bool picker_in_hover = false;

	if (*open)
	{
		int w = static_cast<int> (current_section_components_width / 2.f);
		x -= (current_section_components_width / 2.f) + 4;

		RENDER::FillRectangle(x + w + 1, y - 5 + 1, 1 - 2, 25 - 2, CColor(10, 10, 10, 255));

		for (auto pos = 0; pos < w; pos++)
		{
			const auto p = static_cast<float> (pos) / static_cast<float> (w);
			const auto hue = static_cast<int> (p * 360.f);
			const auto c = hsv_to_rgb(p, 1, 1);
			RENDER::FillRectangle(x + pos, y - 5, 1, 20, CColor(c.RGBA[0], c.RGBA[1], c.RGBA[2], 255));
		}

		bool in_hover = MENU_DRAW::MouseInRegion(x, y, w, 20);

		picker_in_hover = in_hover;

		bool in_click = in_hover && GetAsyncKeyState(VK_LBUTTON);

		if (in_click)
		{
			//console::write_line((g_cursor.get_pos().x - x));
			float percent = static_cast<float> (MENU_DRAW::GetMousePosition().x - x) / static_cast<float> (w);
			CColor c = hsv_to_rgb(percent, 1, 1);
			*clr = CColor(c.RGBA[0], c.RGBA[1], c.RGBA[2], 255);

			if (was_m1_pressed)
				* open = false;
		}

		//percent = ( g_cursor.get_pos().x - ( draw_pos.x + current_x_add + 15 ) ) / ( current_section_components_width - 15 - 15 );
		//if()
	}

	if (!clr_box_in_hover && !picker_in_hover && was_m1_pressed)
		* open = false;
}

void Menu::DoPlayerList(int x, int y, int w, int h) {
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	RENDER::FillRectangle(_pos.x + x, _pos.y + y, w, h, CColor(25, 26, 33, MenuAlpha_Main));
	MENU_DRAW::OutlinedRect(_pos.x + x, _pos.y + y, w, h, CColor(40, 44, 53, MenuAlpha_Main));

	RENDER::FillRectangle(_pos.x + x + 1, _pos.y + y + 1, w - 2, 15, CColor(31, 31, 38, MenuAlpha_Main));
	MENU_DRAW::Line(_pos.x + x + 1, _pos.y + y + 15, _pos.x + x + w - 2, _pos.y + y + 15, CColor(40, 44, 53, MenuAlpha_Main));

	MENU_DRAW::DrawT(_pos.x + x + (w / 2), _pos.y + y + 1, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, true, "Players");

	if (INTERFACES::Engine->IsInGame() && INTERFACES::Engine->IsConnected()) {
		for (int i = 0; i < 64; i++) {
			auto Entity = INTERFACES::ClientEntityList->GetClientEntity(i);

			if (!local_player || !Entity)
				continue;

			if (Entity->GetTeam() == local_player->GetTeam())
				continue;

			SDK::player_info_t EntityInfo;

			if (INTERFACES::Engine->GetPlayerInfo(i, &EntityInfo)) {
				auto SteamID = EntityInfo.iSteamID;

				PlayerList_Player[i] = SteamID;
				static int selected_player;

				RECT Area = { _pos.x + x, _pos.y + y + 17 + (16 * i), w, 15 };

				if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(Area.left, Area.top, Area.right, Area.bottom) && menuOpened) {
					selected_player = SteamID;
				}

				if (selected_player == SteamID) {
					RENDER::FillRectangle(Area.left, Area.top, Area.right, Area.bottom, CColor(28, 28, 35, MenuAlpha_Main));
				}
				else {
					RENDER::FillRectangle(Area.left, Area.top, Area.right, Area.bottom, CColor(21, 21, 28, MenuAlpha_Main));
				}

				MENU_DRAW::OutlinedRect(Area.left, Area.top, Area.right, Area.bottom, CColor(40, 44, 53, MenuAlpha_Main));

				MENU_DRAW::DrawT(Area.left + (Area.right / 2), Area.top + 1, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, true, EntityInfo.szName);

			}

		}
	}

}

void DrawLinebetter(int x0, int y0, int x1, int y1, CColor color)
{
	INTERFACES::Surface->DrawSetColor(color);
	INTERFACES::Surface->DrawLine(x0, y0, x1, y1);
}

void Menu::groupbox(int x, int y, int w, int h, std::string name, bool dont_draw, bool has_subtabs)
{

	RECT text_size = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, name.c_str());

	if (!dont_draw)
	{
		MENU_DRAW::OutlinedRect(_pos.x + x - 2, _pos.y + y - 2, w + 4, h + 4, CColor(0, 0, 0, 200));
		MENU_DRAW::OutlinedRect(_pos.x + x - 1, _pos.y + y - 1, w + 2, h + 2, CColor(57, 56, 57, 200));
		RENDER::FillRectangle(_pos.x + x, _pos.y + y, w, h, CColor(24, 24, 24, MenuAlpha_Main));
		MENU_DRAW::OutlinedRect(_pos.x + x, _pos.y + y, w, h, CColor(0, 0, 0, 200));
		//MENU_DRAW::OutlinedRect(_pos.x + x, _pos.y + y, w, h, Color(57, 56, 57, MenuAlpha_Main));

		//RENDER::FillRectangle(_pos.x + x + 13, _pos.y + y - (text_size.bottom / 2) + 2, text_size.right + 2, text_size.bottom + 2, Color(22, 21, 22, MenuAlpha_Main));
		RENDER::FillRectangle(_pos.x + x + 13, _pos.y + y - (text_size.bottom / 2) + 2, text_size.right + 6, text_size.bottom + 2, CColor(24, 24, 24, MenuAlpha_Main));
		MENU_DRAW::DrawT(_pos.x + x + 15, _pos.y + y - (text_size.bottom / 2), CColor(181, 181, 181, MenuAlpha_Main), MENU_FONTS::SPECIALSmallText, false, name.c_str());
		auto a = _pos;//
	}


	int ItemsToDraw = h / 24;
	Vector2D a = MENU_DRAW::GetMousePosition();
	// x left 2 right (im braindead)

	groupbox_top = _pos.y + y;

	if (has_subtabs)
	{
		y_offset = (_pos.y + (y + 34));
	}
	else
	{
		y_offset = (_pos.y + (y + 12));
	}


	x_offset = _pos.x + x;
	groupbox_bottom = (_pos.y + (y + h));
	groupbox_width = (_pos.x + (x + w));
}

void Menu::ScrollableGroupBox(int x, int y, int w, int h, std::string name, bool dont_draw, bool has_subtabs)
{
	static bool pressed = false;
	static int add;
	if (GetAsyncKeyState(VK_DOWN) & 1)
	{
		add += 16;
	}

	if (GetAsyncKeyState(VK_UP) & 1)
	{
		add -= 16;
	}

	RECT text_size = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, name.c_str());

	if (!dont_draw)
	{
		MENU_DRAW::OutlinedRect(_pos.x + x - 2, _pos.y + y - 2, w + 4, h + 4, CColor(0, 0, 0, 200));
		MENU_DRAW::OutlinedRect(_pos.x + x - 1, _pos.y + y - 1, w + 2, h + 2, CColor(57, 56, 57, 200));
		RENDER::FillRectangle(_pos.x + x, _pos.y + y, w, h, CColor(24, 24, 24, MenuAlpha_Main));
		MENU_DRAW::OutlinedRect(_pos.x + x, _pos.y + y, w, h, CColor(0, 0, 0, 200));
		//MENU_DRAW::OutlinedRect(_pos.x + x, _pos.y + y, w, h, Color(57, 56, 57, MenuAlpha_Main));

		//RENDER::FillRectangle(_pos.x + x + 13, _pos.y + y - (text_size.bottom / 2) + 2, text_size.right + 2, text_size.bottom + 2, Color(22, 21, 22, MenuAlpha_Main));
		RENDER::FillRectangle(_pos.x + x + 13, _pos.y + y - (text_size.bottom / 2) + 2, text_size.right + 6, text_size.bottom + 2, CColor(24, 24, 24, MenuAlpha_Main));
		MENU_DRAW::DrawT(_pos.x + x + 15, _pos.y + y - (text_size.bottom / 2), CColor(181, 181, 181, MenuAlpha_Main), MENU_FONTS::SPECIALSmallText, false, name.c_str());
		auto a = _pos;//
	}


	int ItemsToDraw = h / 24;
	Vector2D a = MENU_DRAW::GetMousePosition();
	// x left 2 right (im braindead)

	groupbox_top = _pos.y + y;

	if (has_subtabs)
	{
		y_offset = (_pos.y + (y + 34));
	}
	else
	{
		y_offset = (_pos.y + (y + 12));
	}


	x_offset = _pos.x + x;
	y_offset = y_offset + add;
	groupbox_bottom = (_pos.y + (y + h));
	groupbox_width = (_pos.x + (x + w));
}

void Menu::checkbox(std::string name, bool* item)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	if (groupbox_top >= y_offset + 16)
		return;

	int size = 9;

	static bool pressed = false;

	RECT text_size = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, name.c_str());

	if (!GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 8, y_offset, size + 17 + text_size.right,
		size) && !dont_recieve_input && menuOpened)
	{
		if (pressed)
			* item = !*item;
		pressed = false;
	}

	if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 8, y_offset, size + 17 + text_size.right,
		size) && !pressed)
		pressed = true;


	if (*item == true)
	{
		Gradientuptodown(x_offset + 9, y_offset + 1, size - 2, size - 2,
			CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b,
				MenuAlpha_Main), CColor(29, 29, 29, MenuAlpha_Main));

		MENU_DRAW::DrawT(x_offset + 22, y_offset - 3, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main), MENU_FONTS::SmallText_bold, false,
			name.c_str());
	}
	else
	{
		MENU_DRAW::DrawT(x_offset + 22, y_offset - 2, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
			name.c_str());

		Gradientuptodown(x_offset + 9, y_offset + 1, size - 2, size - 2, CColor(40, 40, 40, MenuAlpha_Main), CColor(29, 29, 29, MenuAlpha_Main));
	}


	MENU_DRAW::OutlinedRect(x_offset + 8, y_offset, size, size, CColor(0, 0, 0, MenuAlpha_Main / 1.05));

	//	MENU_DRAW::DrawT(x_offset + 40, y_offset - 4, Color(0, 0, 0, MenuAlpha_Main), g::SmallText, false, name.c_str());


	y_offset += 16;
}

void Menu::text(std::string name)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	if (groupbox_top >= y_offset + 16)
		return;

	int size = 9;


	RECT text_size = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, name.c_str());

	MENU_DRAW::DrawT(x_offset + 22, y_offset - 2, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
	name.c_str());


	y_offset += 16;
}

void Menu::combobox(int size, std::string name, std::string* itemname, int* item)
{
	if (groupbox_bottom <= y_offset + 25)
		return;

	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 180; // 163
	static int offset = 0;
	if (name == "")
	{
		offset = -3;
	}
	else
	{
		offset = 14;
	}

	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 20, y_offset + offset, ComboSize, 20)
			&& !click_rest && !dont_recieve_input)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 20, y_offset + offset, ComboSize, 20))
			click_rest = false;

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name)
			open = selected_opened;
	}


	MENU_DRAW::DrawT(x_offset + 21, y_offset - 3, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
		name.c_str());

	RENDER::FillRectangle(x_offset + 20, y_offset + offset, ComboSize, 20, CColor(34, 34, 34, MenuAlpha_Main));


	MENU_DRAW::OutlinedRect(x_offset + 20, y_offset + offset, ComboSize, 20, CColor(0, 0, 0, MenuAlpha_Main));
	//
	if (open)
	{
		RENDER::FillRectangle(x_offset + 20, y_offset + offset + 2 + 19, ComboSize, (size * 19),
			CColor(34, 34, 34, MenuAlpha_Main));
		MENU_DRAW::OutlinedRect(x_offset + 20, y_offset + offset + 2 + 19, ComboSize, (size * 19),
			CColor(0, 0, 0, MenuAlpha_Main));

		for (int i = 0; i < size; i++)
		{
			if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(
				x_offset + 20, y_offset + offset + 4 + 4 + 15 + i * 19, ComboSize, 19))
			{
				*item = i;
			}


			if (i == *item)
				MENU_DRAW::DrawT(x_offset + 20 + 5, y_offset + offset + 4 + 4 + 15 + i * 19,
					CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main), MENU_FONTS::SmallText_bold, false, itemname[i].c_str());
			else if (MENU_DRAW::MouseInRegion(x_offset + 20, y_offset + offset + 4 + 4 + 15 + i * 19, ComboSize, 19))
				MENU_DRAW::DrawT(x_offset + 20 + 5, y_offset + offset + 4 + 4 + 15 + i * 19,
					CColor(200, 200, 200, MenuAlpha_Main), MENU_FONTS::SmallText_bold, false, itemname[i].c_str());
			else
				MENU_DRAW::DrawT(x_offset + 20 + 5, y_offset + offset + 4 + 4 + 15 + i * 19,
					CColor(123, 123, 123, MenuAlpha_Main), MENU_FONTS::SmallText, false, itemname[i].c_str());
		}
	}


	MENU_DRAW::DrawT(x_offset + 20 + 5, y_offset + offset + 3, CColor(143, 143, 143, MenuAlpha_Main),
		MENU_FONTS::SmallText, false, itemname[*item].c_str());

	if (open) // i added 1 more to 42 bec the space between the main box and the drop down
	{
		if (name == "")
		{
			y_offset += 47 - 26 + (size * 19);
		}
		else
		{
			y_offset += 47 + (size * 19);
		}
	}
	else
	{
		if (name == "")
		{
			y_offset += 47 - 26;
		}
		else
		{
			y_offset += 41;
		}
	}
}

float newround(float var)
{
	// we use array of chars to store number 
	// as a string. 
	char str[40];

	// Print in string the value of var  
	// with two decimal point 
	sprintf(str, "%.2f", var);

	return var;
}

void Menu::fake_slider(int max, std::string name, int* item, std::string suffix, int step)
{
	if (groupbox_bottom <= y_offset + 10)
		return;

	bool dragging = false;

	float SliderSize = 150; // 163

	int howfarfromlegt = 8;

	float _pixel_value = max / SliderSize;
	static bool opened = false;
	if (menuOpened)
	{
		//if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + howfarfromlegt, y_offset + 14,
		//	SliderSize + (3 * 2), 8) && !dont_recieve_input && menuOpened)
		//{
		//	*item = abs(MENU_DRAW::GetMousePosition().x - (x_offset + howfarfromlegt)) * _pixel_value;
		//	dragging = true;
		//}

		static bool pressed_subtract = false;
		static bool pressed_add = false;

		/*
		//sub


		/*if (!GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 22, y_offset + 12, 8, 10))
		{
			if (pressed_subtract)
				*item -= step;
			pressed_subtract = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 22, y_offset + 12, 8, 10) && !pressed_subtract)
			pressed_subtract = true;

		//


		//add


		if (!GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 22 + SliderSize, y_offset + 12, 15, 10))
		{
			if (pressed_add)
				*item += step;
			pressed_add = false;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 22 + SliderSize, y_offset + 12, 15, 10) && !pressed_add)
			pressed_add = true;*/

			//

		if (*item > max) {
			*item = max;
		}

		if (*item < 0) {
			*item = 0;
		}
	}

	std::string namedisplay;

	namedisplay += name.c_str();

	std::string bruh;

	bruh += namedisplay.length();

	//namedisplay += " - ";

	//namedisplay += std::to_string(*item).c_str();

	if (*item > max)
	{
		*item = max;
	}

	if (*item < 0)
	{
		*item = 0;
	}

	MENU_DRAW::DrawT(x_offset + howfarfromlegt, y_offset, CColor(143, 143, 143, 100), MENU_FONTS::SmallText, false,
		"%s : %s%s", namedisplay.c_str(), std::to_string(*item).c_str(), suffix.c_str());

	//MENU_DRAW::DrawT(x_offset + howfarfromlegt + 20, y_offset - 3, Color(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
	//	bruh.c_str());

	//MENU_DRAW::DrawT(x_offset + howfarfromlegt + namedisplay.length(), y_offset - 3, Color(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText_bold, false, 
	//	" : %s%s", std::to_string(*item).c_str(), suffix.c_str());

	RENDER::FillRectangle(x_offset + howfarfromlegt, y_offset + 14, SliderSize, 8, CColor(34, 34, 34, 100));
	UINT hp = SliderSize - *item / max;

	int Red = 255 - ((SliderSize * (*item / _pixel_value) * 2.55));
	int Green = (SliderSize * (*item / _pixel_value) * 2.55);

	MENU_DRAW::OutlinedRect(x_offset + howfarfromlegt, y_offset + 14, SliderSize, 9, CColor(0, 0, 0, 100));

	if (*item && menuOpened)
	{
		if (*item == max) //to fix dumb math stufffff bec pixel size
			GradientH(x_offset + howfarfromlegt, y_offset + 15, SliderSize, 7, CColor(32, 32, 32, 100), CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 100));
		else
			GradientH(x_offset + howfarfromlegt, y_offset + 15, (*item / _pixel_value), 7, CColor(32, 32, 32, 100), CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 100));

		//RENDER::FillRectangle(x_offset + (*item / _pixel_value) + 7, y_offset + 12, 3, 12, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, 100));
	}

	std::string gay2;
	gay2 += std::to_string(*item);
	gay2 += suffix.c_str();

	RECT Text_Size_Meme = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, gay2.c_str());

	//if (std::to_string(*item).c_str() < std::to_string(10).c_str())
	//	MENU_DRAW::DrawT(x_offset + (SliderSize)-Text_Size_Meme.right + 10, y_offset, Color(143, 143, 143, MenuAlpha_Main), 
	//		MENU_FONTS::SmallText, true, "%s%s", std::to_string(*item).c_str(), suffix.c_str());
	//else if (std::to_string(*item).c_str() >= std::to_string(10).c_str() && std::to_string(*item).c_str() < std::to_string(100).c_str())
	//	MENU_DRAW::DrawT(x_offset + (SliderSize)-Text_Size_Meme.right + 12, y_offset, Color(143, 143, 143, MenuAlpha_Main),
	//		MENU_FONTS::SmallText, true, "%s%s", std::to_string(*item).c_str(), suffix.c_str());
	//else
	//	MENU_DRAW::DrawT(x_offset + (SliderSize)-Text_Size_Meme.right + 14, y_offset, Color(143, 143, 143, MenuAlpha_Main),
	//		MENU_FONTS::SmallText, true, "%s%s", std::to_string(*item).c_str(), suffix.c_str());

	//MENU_DRAW::DrawT(x_offset + 18, y_offset + 11, Color(100, 100, 100, MenuAlpha_Main), g::SmallText, false, "-");
	//MENU_DRAW::DrawT(x_offset + (SliderSize)-Text_Size_Meme.right + 9, y_offset, Color(143, 143, 143, MenuAlpha_Main),
		//MENU_FONTS::SmallText, true, "%s%s", std::to_string(*item).c_str(), suffix.c_str());
	//MENU_DRAW::DrawT(x_offset + 21 + SliderSize+ 20, y_offset + 11, Color(100, 100, 100, MenuAlpha_Main), g::SmallText, false, "+");
	//
	y_offset += 25;
}
void Menu::sliderfloat(float max, std::string name, float* item, std::string suffix, float step)
{
	if (groupbox_bottom <= y_offset + 10)
		return;

	bool dragging = false;

	float SliderSize = 180; // 163

	int howfarfromlegt = 20;

	float _pixel_value = max / SliderSize;
	static bool opened = false;
	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + howfarfromlegt, y_offset + 13,
			SliderSize + (3 * 2), 10) && !dont_recieve_input && menuOpened)
		{
			*item = abs(MENU_DRAW::GetMousePosition().x - (x_offset + howfarfromlegt)) * _pixel_value;
			dragging = true;
		}

		static bool pressed_subtract = false;
		static bool pressed_add = false;
		if (*item > max) {
			*item = max;
		}

		if (*item < step) {
			*item = step;
		}
	}

	std::string namedisplay;

	namedisplay += name.c_str();

	std::string bruh;

	bruh += namedisplay.length();

	if (*item > max)
	{
		*item = max;
	}

	if (*item < step)
	{
		*item = step;
	}
	//std::setprecision(2)
	//float valueround = newround(*item);

	MENU_DRAW::DrawT(x_offset + howfarfromlegt, y_offset, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
		"%s : %.2f%s", namedisplay.c_str(), /*std::to_string(*/*item/*).c_str()*/, suffix.c_str());

	RENDER::FillRectangle(x_offset + howfarfromlegt, y_offset + 15, SliderSize, 8, CColor(34, 34, 34, MenuAlpha_Main));
	UINT hp = SliderSize - *item / max - step;

	int Red = 255 - ((SliderSize * (*item / _pixel_value) * 2.55));
	int Green = (SliderSize * (*item / _pixel_value) * 2.55);

	MENU_DRAW::OutlinedRect(x_offset + howfarfromlegt, y_offset + 15, SliderSize, 9, CColor(0, 0, 0, MenuAlpha_Main));

	if (*item && menuOpened)
	{
		if (*item == max) //to fix dumb math stufffff bec pixel size
			GradientH(x_offset + howfarfromlegt, y_offset + 16, SliderSize, 7, CColor(32, 32, 32, MenuAlpha_Main), CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main));
		else
			GradientH(x_offset + howfarfromlegt, y_offset + 16, (*item / _pixel_value), 7, CColor(32, 32, 32, MenuAlpha_Main), CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main));

		RENDER::FillRectangle(x_offset + (*item / _pixel_value) + 19, y_offset + 14, 3, 11, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main));
	}

	std::string gay2;
	gay2 += std::to_string(*item);
	gay2 += suffix.c_str();

	RECT Text_Size_Meme = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, gay2.c_str());

	y_offset += 28;
}
void Menu::slider(int max, std::string name, int* item, std::string suffix, int step)
{
	if (groupbox_bottom <= y_offset + 10)
		return;

	bool dragging = false;

	float SliderSize = 180; // 163

	int howfarfromlegt = 20;

	float _pixel_value = max / SliderSize;
	static bool opened = false;
	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + howfarfromlegt, y_offset + 13,
			SliderSize + (3 * 2), 10) && !dont_recieve_input && menuOpened)
		{
			*item = abs(MENU_DRAW::GetMousePosition().x - (x_offset + howfarfromlegt)) * _pixel_value;
			dragging = true;
		}
		else if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + howfarfromlegt, y_offset + 10,
			SliderSize + (3 * 2), 7) && !dont_recieve_input && menuOpened && dragging)
		{
			*item = abs(MENU_DRAW::GetMousePosition().x - (x_offset + howfarfromlegt)) * _pixel_value;
			dragging = true;
		}
		else
			dragging = false;


		static bool pressed_subtract = false;
		static bool pressed_add = false;

		if (*item > max)
		{
			*item = max;
		}

		if (*item < step)
		{
			*item = step;
		}
	}

	std::string namedisplay;

	namedisplay += name.c_str();

	std::string bruh;

	bruh += namedisplay.length();


	if (*item > max)
	{
		*item = max;
	}

	if (*item < step)
	{
		*item = step;
	}
	//std::setprecision(2)
	float valueround = newround(*item);

	MENU_DRAW::DrawT(x_offset + howfarfromlegt, y_offset, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
		"%s : %s%s", namedisplay.c_str(), std::to_string(*item).c_str(), suffix.c_str());

	RENDER::FillRectangle(x_offset + howfarfromlegt, y_offset + 15, SliderSize, 8, CColor(34, 34, 34, MenuAlpha_Main));
	UINT hp = SliderSize - *item / max;

	int Red = 255 - ((SliderSize * (*item / _pixel_value) * 2.55));
	int Green = (SliderSize * (*item / _pixel_value) * 2.55);

	MENU_DRAW::OutlinedRect(x_offset + howfarfromlegt, y_offset + 15, SliderSize, 9, CColor(0, 0, 0, MenuAlpha_Main));

	if (*item && menuOpened)
	{
		if (*item == max) //to fix dumb math stufffff bec pixel size
			GradientH(x_offset + howfarfromlegt, y_offset + 16, SliderSize, 7, CColor(32, 32, 32, MenuAlpha_Main), CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main));
		else
			GradientH(x_offset + howfarfromlegt, y_offset + 16, (*item / _pixel_value), 7, CColor(32, 32, 32, MenuAlpha_Main), CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main));

		RENDER::FillRectangle(x_offset + (*item / _pixel_value) + 19, y_offset + 14, 3, 11, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main));
	}

	std::string gay2;
	gay2 += std::to_string(*item);
	gay2 += suffix.c_str();

	RECT Text_Size_Meme = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, gay2.c_str());

	y_offset += 28;
}

void Menu::MultiComboBox(float indexes, std::string name, std::string* itemname, bool* item)
{
	static bool multiPressed = false;
	bool pressed = false;
	bool open = false;
	static bool selectedOpened = false;
	static bool clickRest;
	static bool rest;
	static std::string nameSelected;
	std::string itemsSelected = "";
	int lastItem = 0;
	int ComboSize = 180;

	int offset = 0;
	if (name == "")
	{
		offset = 0;
	}
	else
	{
		offset = 14;
	}

	if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 20, y_offset + offset, ComboSize, 17) && !clickRest
		)
	{
		nameSelected = name;
		pressed = true;
		clickRest = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 20, y_offset + offset, ComboSize, 20))
		clickRest = false;

	if (pressed)
	{
		if (!rest)
			selectedOpened = !selectedOpened;

		rest = true;
	}
	else
		rest = false;

	if (nameSelected == name)
		open = selectedOpened;

	MENU_DRAW::DrawT(x_offset + 21, y_offset - 3, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
		name.c_str());

	RENDER::FillRectangle(x_offset + 20, y_offset + offset - 2, ComboSize, 20, CColor(34, 34, 34, MenuAlpha_Main));
	MENU_DRAW::OutlinedRect(x_offset + 20, y_offset + offset - 2, ComboSize, 20, CColor(0, 0, 0, MenuAlpha_Main));

	if (open)
	{
		INTERFACES::Surface->clipping_enabled() = false;
		RENDER::FillRectangle(x_offset + 20, y_offset + offset + 2 + 19, ComboSize, (indexes * 19),
			CColor(34, 34, 34, MenuAlpha_Main));
		MENU_DRAW::OutlinedRect(x_offset + 20, y_offset + offset + 2 + 19, ComboSize, (indexes * 19),
			CColor(0, 0, 0, MenuAlpha_Main));

		for (int i = 0; i < indexes; i++)
		{

			if (!GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(
				x_offset + 20, y_offset + offset + 4 + 4 + 15 + i * 19, ComboSize, 20))
			{
				if (multiPressed)
					item[i] = !item[i];
				multiPressed = false;
			}

			if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(
				x_offset + 20, y_offset + offset + 16 + (i * 19), ComboSize, 20) && !multiPressed)
				multiPressed = true;

			if (item[i])
				MENU_DRAW::DrawT(x_offset + 20 + 5, y_offset + offset + 4 + 4 + 15 + i * 19, CColor(Config::ConfigMenu::ConfigMain::get().menu_color_r, Config::ConfigMenu::ConfigMain::get().menu_color_g, Config::ConfigMenu::ConfigMain::get().menu_color_b, MenuAlpha_Main),
					MENU_FONTS::SmallText_bold, false, itemname[i].c_str());
			else if (MENU_DRAW::MouseInRegion(x_offset + 20, y_offset + offset + 4 + 4 + 15 + i * 19, ComboSize, 19))
				MENU_DRAW::DrawT(x_offset + 20 + 5, y_offset + offset + 4 + 4 + 15 + i * 19, CColor(200, 200, 200, MenuAlpha_Main),
					MENU_FONTS::SmallText_bold, false, itemname[i].c_str());
			else
				MENU_DRAW::DrawT(x_offset + 20 + 5, y_offset + offset + 4 + 4 + 15 + i * 19, CColor(185, 185, 185, MenuAlpha_Main),
					MENU_FONTS::SmallText, false, itemname[i].c_str());
		}
		INTERFACES::Surface->clipping_enabled() = true;
	}

	bool items = false;

	for (int i = 0; i < indexes; i++)
	{
		if (item[i])
		{
			if (lastItem < i)
				lastItem = i;
		}
	}

	for (int i = 0; i < indexes; i++)
	{
		if (item[i])
		{
			items = true;
			RECT TextSize = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, itemsSelected.c_str());
			RECT TextSizeGonaAdd = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, itemname[i].c_str());
			if (TextSize.right + TextSizeGonaAdd.right < ComboSize)
				itemsSelected += itemname[i] + ((lastItem == i) ? "" : ", ");
		}
	}

	if (!items)
		itemsSelected = "Disabled";

	MENU_DRAW::DrawT(x_offset + 20 + 5, y_offset + offset + 1, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
		itemsSelected.c_str());

	if (open) // i added 1 more to 42 bec the space between the main box and the drop down
		y_offset += 52 + (indexes * 16);
	else
		y_offset += 41;
}

char* KeyStringsStick[254] = {
	"INVLD", "M1", "M2", "BRK", "M3", "M4", "M5",
	"INVLD", "BSPC", "TAB", "INVLD", "INVLD", "INVLD", "ENTER", "INVLD", "INVLD", "SHI",
	"CTRL", "ALT", "PAU", "CAPS", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"ESC", "INVLD", "INVLD", "INVLD", "INVLD", "SPACE", "PGUP", "PGDOWN", "END", "HOME", "LEFT",
	"UP", "RIGHT", "DOWN", "INVLD", "PRNT", "INVLD", "PRTSCR", "INS", "DEL", "INVLD", "0", "1",
	"2", "3", "4", "5", "6", "7", "8", "9", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
	"V", "W", "X", "Y", "Z", "LFTWIN", "RGHTWIN", "INVLD", "INVLD", "INVLD", "NUM0", "NUM1",
	"NUM2", "NUM3", "NUM4", "NUM5", "NUM6", "NUM7", "NUM8", "NUM9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3",
	"F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20",
	"F21",
	"F22", "F23", "F24", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"NUM LOCK", "SCROLL LOCK", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "LSHFT", "RSHFT", "LCTRL",
	"RCTRL", "LMENU", "RMENU", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "NTRK", "PTRK", "STOP", "PLAY", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", ";", "+", ",", "-", ".", "/?", "~", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "{", "\\|", "}", "'\"", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD", "INVLD",
	"INVLD", "INVLD"
};

bool keys[256];
bool oldKeys[256];

bool GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	return false;
}

void Menu::keybind(int* item, std::string name)
{
	if (groupbox_bottom <= y_offset + 41)
		return;

	int ComboSize = 180; // 163
	static std::string nameSelected;


	static bool IsGettingKey = false;
	static int Key = 0;
	static bool GoodKeyName = false;
	CColor text_color = CColor(120, 120, 120, 255);
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		keys[x] = (GetAsyncKeyState(x));
	}

	char NameBuffer[128];
	char* KeyName = "Unbinded";
	RECT text_size = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, KeyName);
	static bool LetsBindy0 = false;
	/*
	if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 20, y_offset, 157, 20) && menuOpened)
	{
		if (!IsGettingKey)
		{
			IsGettingKey = true;
		}
	}

	if (IsGettingKey)
	{
		for (int i = 0; i < 255; i++)
		{
			if (GetKeyPress(i))
			{
				if (i == VK_ESCAPE)
				{
					Key = -1;
					*item = Key;
					IsGettingKey = false;
					return;
				}

				Key = i;
				*item = Key;
				IsGettingKey = false;
				return;
			}
		}
	}


	if (IsGettingKey)
	{
		KeyName = "<Waiting>";
		text_color = CColor(143, 143, 143, 255);
	}
	else
	{
		if (*item >= 0)
		{
			KeyName = KeyStringsStick[*item];
			if (KeyName)
			{
				GoodKeyName = true;
			}
			else
			{
				if (GetKeyNameText(*item << 16, NameBuffer, 127))
				{
					KeyName = NameBuffer;
					GoodKeyName = true;
				}
			}
		}

		if (!GoodKeyName)
		{
			KeyName = "<Unbinded>";
		}
	}

	if (manually_set_key)
	{
		Key = *item;
		manually_set_key = false;
	}
	*/
	if (*item == -1) {
		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 20, y_offset, 157, 20) && menuOpened)
		{
			if (!IsGettingKey)
			{
				IsGettingKey = true;
			}
		}
		if (IsGettingKey)
		{
			for (int i = 0; i < 255; i++)
			{
				if (GetKeyPress(i))
				{
					if (i == VK_ESCAPE)
					{
						Key = -1;
						*item = Key;
						IsGettingKey = false;
						return;
					}

					Key = i;
					*item = Key;
					IsGettingKey = false;
					return;
				}
			}
		}
		KeyName = "<Unbinded>";

	}
	else {
		KeyName = KeyStringsStick[*item];
		if (KeyName)
		{
			GoodKeyName = true;
		}
		else
		{
			if (GetKeyNameText(*item << 16, NameBuffer, 127))
			{
				KeyName = NameBuffer;
			}
		}
		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 20, y_offset, 157, 20) && menuOpened)
		{
			if (!IsGettingKey)
			{
				*item = -1;
				IsGettingKey = true;
			}
		}


	}


	RENDER::FillRectangle(x_offset + 20, y_offset, ComboSize, 20, CColor(34, 34, 34, MenuAlpha_Main));
	MENU_DRAW::OutlinedRect(x_offset + 20, y_offset, ComboSize, 20, CColor(0, 0, 0, MenuAlpha_Main));
	MENU_DRAW::DrawT(x_offset + 20 + 5, y_offset + 3, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false, KeyName);

	y_offset += 29;
}

unsigned int rgb(double hue)
{
	int h = int(hue * 256 * 6);
	int x = h % 0x100;

	int r = 0, g = 0, b = 0;
	switch (h / 256)
	{
	case 0: r = 255;
		g = x;
		break;
	case 1: g = 255;
		r = 255 - x;
		break;
	case 2: g = 255;
		b = x;
		break;
	case 3: b = 255;
		g = 255 - x;
		break;
	case 4: b = 255;
		r = x;
		break;
	case 5: r = 255;
		b = 255 - x;
		break;
	}

	return r + (g << 8) + (b << 16);
}

void color_spectrum(int x, int y)
{
	int Width = 168, Height = 179;

	for (int i = 0; i < Width; i++)
	{
		int div = Width / 6;
		int phase = i / div;
		float t = (i % div) / (float)div;
		int r, g, b;

		switch (phase)
		{
		case(0):
			r = 255;
			g = 255 * t;
			b = 0;
			break;
		case(1):
			r = 255 * (1.f - t);
			g = 255;
			b = 0;
			break;
		case(2):
			r = 0;
			g = 255;
			b = 255 * t;
			break;
		case(3):
			r = 0;
			g = 255 * (1.f - t);
			b = 255;
			break;
		case(4):
			r = 255 * t;
			g = 0;
			b = 255;
			break;
		case(5):
			r = 255;
			g = 0;
			b = 255 * (1.f - t);
			break;
		}

		for (int k = 0; k < Height; k++)
		{
			float sat = k / (float)Height;
			int _r = r + sat * (128 - r);
			int _g = g + sat * (128 - g);
			int _b = b + sat * (128 - b);
			INTERFACES::Surface->DrawSetColor(_r, _g, _b, 255);
			INTERFACES::Surface->DrawFilledRect(10 + i, 10 + k, 1, 1);
		}
	}
}

#include <conio.h>
#include <iostream>
#include <windows.h>
#include <stdlib.h>

void Menu::EmptySpace(std::string name)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	MENU_DRAW::DrawT(x_offset + 20, y_offset - 3, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
		name.c_str());

	y_offset += 16;
}



char* KeyDigits[254] = {
	nullptr, "Left Mouse", "Right Mouse", "Control+Break", "Middle Mouse", "Mouse 4", "Mouse 5",
	nullptr, "Backspace", "TAB", nullptr, nullptr, nullptr, "ENTER", nullptr, nullptr, "SHIFT", "CTRL", "ALT", "PAUSE",
	"CAPS LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "ESC", nullptr, nullptr, nullptr, nullptr,
	"SPACEBAR",
	"PG UP", "PG DOWN", "END", "HOME", "Left", "Up", "Right", "Down", nullptr, "Print", nullptr, "Print Screen",
	"Insert",
	"Delete", nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr,
	nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
	"V", "W", "X",
	"Y", "Z", "Left Windows", "Right Windows", nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
	"7", "8", "9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11",
	"F12",
	"F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", nullptr, nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr, nullptr, "NUM LOCK", "SCROLL LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, "LSHIFT", "RSHIFT", "LCONTROL", "RCONTROL", "LMENU", "RMENU", nullptr,
	nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "Next Track", "Previous Track", "Stop", "Play/Pause",
	nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[{", "\\|", "}]", "'\"", nullptr, nullptr, nullptr,
	nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};

void Menu::TextBox(std::string name, std::string* text)
{
	if (groupbox_bottom <= y_offset + 16)
		return;

	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 127; // 163
	int ComboSizeH = 127; // 163
	static int offset = 0;
	if (name == "")
	{
		offset = 0;
	}
	else
	{
		offset = 14;
	}

	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}


	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 225, y_offset + 1, 15, 8) && !
			click_rest)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 225, y_offset + 1, 15, 8))
			click_rest = false;

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name)
			open = selected_opened;
	}

	const char* strg = text->c_str();
	if (open)
	{
		for (int i = 0; i < 255; i++)
		{
			if (GetKeyPress(i))
			{
				if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
				{
					open = false;
					return;
				}

				if (i == VK_BACK && strlen(strg) != 0)
				{
					*text = text->substr(0, strlen(strg) - 1);
				}

				if (strlen(strg) < 6 && (i == 0x30 || i == 0x31 || i == 0x32 || i == 0x33 || i == 0x34 || i == 0x35 || i
					== 0x36 || i == 0x37 || i == 0x38 || i == 0x39 || i == VK_NUMPAD0 || i == VK_NUMPAD1 || i ==
					VK_NUMPAD2 || i == VK_NUMPAD3 || i == VK_NUMPAD4 || i == VK_NUMPAD5 || i == VK_NUMPAD6 || i ==
					VK_NUMPAD7 || i == VK_NUMPAD8 || i == VK_NUMPAD9))
				{
					*text = *text + KeyDigits[i];
					return;
				}
			}
		}
	}

	const char* cstr = text->c_str();
	RENDER::FillRectangle(x_offset + 20, y_offset, ComboSize, 20, CColor(81, 80, 81, MenuAlpha_Main));
	MENU_DRAW::OutlinedRect(x_offset + 20, y_offset, ComboSize, 20, CColor(0, 0, 0, MenuAlpha_Main));
	MENU_DRAW::DrawT(x_offset + 20 + 5, y_offset + 3, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, false,
		cstr);

	y_offset += 29;
}

void Menu::button(ButtonCallback_t callback, std::string title)
{
	if (groupbox_bottom <= y_offset + 18)
		return;

	int ComboSize = 180;

	static bool pressed = false;

	RECT text_size = MENU_DRAW::GetTextSizeRect(MENU_FONTS::SmallText, title.c_str());

	if (!GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 20, y_offset, ComboSize, 20) && menuOpened)
	{
		if (pressed)
		{
			if (callback)
				callback();
		}
		pressed = false;
	}

	if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(x_offset + 20, y_offset, ComboSize, 20) && !pressed)
		pressed = true;

	if (MENU_DRAW::MouseInRegion(x_offset + 20, y_offset, ComboSize, 20) and GetAsyncKeyState(VK_LBUTTON))
	{
		RENDER::FillRectangle(x_offset + 20, y_offset, ComboSize, 20, CColor(34, 34, 34, 255));

		MENU_DRAW::DrawT(x_offset + 20 + (ComboSize / 2) - (text_size.right / 2),
			y_offset + (20 / 2) - (text_size.bottom / 2) - 1, CColor(204, 204, 204, MenuAlpha_Main),
			MENU_FONTS::SmallText_bold, false, title.c_str());
	}
	else
	{
		RENDER::FillRectangle(x_offset + 20, y_offset, ComboSize, 20, CColor(34, 34, 34, 200));

		MENU_DRAW::DrawT(x_offset + 20 + (ComboSize / 2) - (text_size.right / 2),
			y_offset + (20 / 2) - (text_size.bottom / 2) - 1, CColor(204, 204, 204, MenuAlpha_Main),
			MENU_FONTS::SmallText, false, title.c_str());
	}

	MENU_DRAW::OutlinedRect(x_offset + 20, y_offset, ComboSize, 20, CColor(0, 0, 0, MenuAlpha_Main));
	y_offset += 26;
}


void Menu::ListBox(int x, int y, int w, int h, int size, std::string name, std::string *itemname, int* item) {
	RENDER::FillRectangle(_pos.x + x, _pos.y + y, w, h, CColor(25, 26, 33, MenuAlpha_Main));
	MENU_DRAW::OutlinedRect(_pos.x + x, _pos.y + y, w, h, CColor(40, 44, 53, MenuAlpha_Main));

	RENDER::FillRectangle(_pos.x + x + 1, _pos.y + y + 1, w - 2, 15, CColor(31, 31, 38, MenuAlpha_Main));
	MENU_DRAW::Line(_pos.x + x + 1, _pos.y + y + 15, _pos.x + x + w - 2, _pos.y + y + 15, CColor(40, 44, 53, MenuAlpha_Main));

	MENU_DRAW::DrawT(_pos.x + x + (w / 2), _pos.y + y + 1, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, true, name.c_str());

	for (int i = 0; i < size; i++) {
		RECT Area = { _pos.x + x, _pos.y + y + 17 + (16 * i), w, 15 };

		if (GetAsyncKeyState(VK_LBUTTON) && MENU_DRAW::MouseInRegion(Area.left, Area.top, Area.right, Area.bottom) && menuOpened) {
			*item = i;
		}

		if (*item == i) {
			RENDER::FillRectangle(Area.left, Area.top, Area.right, Area.bottom, CColor(28, 28, 35, MenuAlpha_Main));
		}
		else {
			RENDER::FillRectangle(Area.left, Area.top, Area.right, Area.bottom, CColor(21, 21, 28, MenuAlpha_Main));
		}

		MENU_DRAW::OutlinedRect(Area.left, Area.top, Area.right, Area.bottom, CColor(40, 44, 53, MenuAlpha_Main));

		MENU_DRAW::DrawT(Area.left + (Area.right / 2), Area.top + 1, CColor(143, 143, 143, MenuAlpha_Main), MENU_FONTS::SmallText, true, itemname[i].c_str());
	}
}
