#include "../includes.h"

#include "interfaces.h"

#include "../SDK/ISurface.h"
#include "render.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"


#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()

namespace global
{
	HWND csgo_hwnd = FindWindow(0, "Counter-Strike: Global Offensive");
	bool render_init;

	bool should_send_packet;
	bool m_round_changed;
	bool hasclantagged;
	bool DesyncEnabledSpecialChecks;

	bool should_buy;
	int missedshots[65];
	int hit[65];
	bool    Hit2[65];
	bool     Shot[65];
	bool Hited[65];
	int fired[65];
	float flHurtTimeKill; 
	float flHurtTimeKillmulti;
	int menual;
	int menualrlb;
	bool preservedelete = false;
	bool desync;

	float cheat_start_time;
	int packets_choked;
	int choke_amount;
	int freestandingsite;
	bool should_load_materials;
	std::string reason;

	bool should_choke_packets;

	Vector real_angles;
	Vector cmd_angles;
	Vector fake_angles;
	Vector fake_angelsdesync;
	Vector angles;
	Vector strafe_angle;
	Vector aim_point;

	int missed_shots[65];
	bool breaking_lby;

	bool modulate;

	int randomnumber;
	float	flHurtTime;
	bool DisableAA;
	bool Aimbotting;

	using msg_t = void(__cdecl*)(const char*, ...);
	msg_t Msg = reinterpret_cast< msg_t >(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));

	Vector FakePosition;
	int ground_tickz;
	bool CircleStraferActive;
	SDK::CUserCmd originalCMD;
}
namespace FONTS
{
	unsigned int menu_tab_font;
	unsigned int menu_checkbox_font;
	unsigned int menu_slider_font;
	unsigned int menu_slider_value_font;
	unsigned int menu_groupbox_font;
	unsigned int menu_combobox_name_font;
	unsigned int menu_combobox_value_font;
	unsigned int menu_window_font;
	unsigned int menu_separator_font;
	unsigned int menu_window_blurr_font;
	unsigned int menu_keybind_value_font;
	unsigned int menu_keybind_name_font;
	unsigned int menu_text_input_name_font;
	unsigned int menu_text_input_value_font;
	unsigned int menu_button_font;
	unsigned int menu_colorpicker_font;
	unsigned int menu_tool_tip_font;
	unsigned int visuals_icon_font;
	unsigned int visuals_esp_font;
	unsigned int visuals_xhair_font;
	unsigned int visuals_side_font;
	unsigned int visuals_name_font;
	unsigned int visuals_lby_font;
	unsigned int visuals_headshot_font;
	unsigned int visuals_esp_font_de;
	unsigned int visuals_grenade_pred_font;
	unsigned int  in_game_logging_font;
	unsigned int visuals_indector;
	unsigned int welcome_font;
	unsigned int killindicator;
	unsigned int watermarkalyal;
	unsigned int watermarktextfps;
	 unsigned int watermarkclock;
	 unsigned int watermarkfps;
	 unsigned int visuals_other_font;
	bool ShouldReloadFonts()
	{
		static int old_width, old_height;
		int width, height;
		INTERFACES::Engine->GetScreenSize(width, height);

		if (width != old_width || height != old_height)
		{
			old_width = width;
			old_height = height;
			return true;
		}
		return false;
	}
	void InitFonts()
	{
		menu_tab_font = RENDER::CreateF("Verdana", 16, 550, 0, 0, SDK::FONTFLAG_DROPSHADOW);
		menu_checkbox_font = RENDER::CreateF("Calibri", 18, 550, 0, 0, NULL);
		menu_slider_font = RENDER::CreateF("Calibri", 18, 550, 0, 0, NULL);
		menu_slider_value_font = RENDER::CreateF("Calibri", 16, 500, 0, 0, NULL);
		menu_groupbox_font = RENDER::CreateF("Calibri", 18, 550, 0, 0, NULL);
		menu_combobox_name_font = RENDER::CreateF("Calibri", 18, 550, 0, 0, NULL);
		menu_combobox_value_font = RENDER::CreateF("Calibri", 16, 550, 0, 0, NULL);
		menu_separator_font = RENDER::CreateF("Calibri", 18, 550, 0, 0, NULL);
		menu_keybind_value_font = RENDER::CreateF("Calibri", 16, 550, 0, 0, NULL);
		menu_keybind_name_font = RENDER::CreateF("Calibri", 18, 550, 0, 0, NULL);
		menu_text_input_name_font = RENDER::CreateF("Calibri", 18, 550, 0, 0, NULL);
		menu_text_input_value_font = RENDER::CreateF("Calibri", 16, 550, 0, 0, NULL);
		menu_button_font = RENDER::CreateF("Calibri", 18, 550, 0, 0, NULL);
		menu_window_blurr_font = RENDER::CreateF("Againts", 32, 550, 10, 0, SDK::FONTFLAG_ANTIALIAS);
		menu_window_font = RENDER::CreateF("Againts", 32, 550, 0, 10, SDK::FONTFLAG_ANTIALIAS);
		menu_colorpicker_font = RENDER::CreateF("Calibri", 18, 550, 0, 0, NULL);
		menu_tool_tip_font = RENDER::CreateF("Calibri", 18, 550, 0, 0, NULL);

		visuals_esp_font = RENDER::CreateF("Smallest Pixel-7", 10, 100, 0, 0, SDK::FONTFLAG_OUTLINE);
		visuals_xhair_font = RENDER::CreateF("Smallest Pixel-7", 18, 100, 0, 0, SDK::FONTFLAG_OUTLINE);
		visuals_icon_font = RENDER::CreateF("Csgo Images", 48, 300, 0, 0, SDK::FONTFLAG_DROPSHADOW | SDK::FONTFLAG_ANTIALIAS);
		visuals_side_font = RENDER::CreateF("Calibri", 46, 900, 0, 0, NULL);
		visuals_name_font = RENDER::CreateF("Smallest Pixel-7", 12, 100, 0, 0, SDK::FONTFLAG_OUTLINE);
		visuals_other_font = RENDER::CreateF("Smallest Pixel-7", 12, 100, 0, 0, NULL);
		visuals_lby_font = RENDER::CreateF("Verdana", 32, 650, 0, 0, SDK::FONTFLAG_ANTIALIAS | SDK::FONTFLAG_DROPSHADOW);
		visuals_grenade_pred_font = RENDER::CreateF("undefeated", 16, 400, 0, 0, SDK::FONTFLAG_DROPSHADOW | SDK::FONTFLAG_ANTIALIAS);
		visuals_esp_font_de = RENDER::CreateF("DINPro-Medium", 16, 100, 0, 0, NULL);
		welcome_font = RENDER::CreateF("Segoe Script", 100, 650, 0, 0, SDK::FONTFLAG_ANTIALIAS);
		visuals_headshot_font = RENDER::CreateF("Smallest Pixel-7", 14, 100, 0, 0, SDK::FONTFLAG_OUTLINE);
		killindicator = RENDER::CreateF("Segoe UI Black", 40, 100, 0, 0, NULL);

		visuals_indector = RENDER::CreateF("Verdana", 40, 900, 0, 0, SDK::FONTFLAG_ANTIALIAS);
		in_game_logging_font = RENDER::CreateF("Tahoma", 12, FW_BOLD, false, false, SDK::FONTFLAG_ANTIALIAS);
		watermarkfps = RENDER::CreateF("Bahnschrift SemiBold", 40, 400, 0, 0, NULL);
		watermarktextfps = RENDER::CreateF("Bahnschrift SemiBold", 20, 400, 0, 0, NULL);
		watermarkalyal = RENDER::CreateF("Bahnschrift SemiBold", 60, 400, 0, 0, NULL);
	}
}
namespace SETTINGS
{
	CSettings settings; 

	bool CSettings::Save(int cfgnum)
	{
		std::string file_name = "config";
		std::string file_path = "C:\\alyal.club\\" + file_name + std::to_string(cfgnum) + ".cfg";

		std::fstream file(file_path, std::ios::out | std::ios::in | std::ios::trunc);
		file.close();

		file.open(file_path, std::ios::out | std::ios::in);
		if (!file.is_open())
		{
			file.close();
			return false;
		}

		const size_t settings_size = sizeof(CSettings);
		for (int i = 0; i < settings_size; i++)
		{
			byte current_byte = *reinterpret_cast<byte*>(uintptr_t(this) + i);
			for (int x = 0; x < 8; x++)
			{
				file << (int)((current_byte >> x) & 1);
			}
		}

		file.close();

		return true;
	}
	bool CSettings::Load(int cfgnum)
	{
		std::string file_name = "config";
		CreateDirectory("C:\\alyal.club", NULL);

		std::string file_path = "C:\\alyal.club\\" + file_name + std::to_string(cfgnum) + ".cfg";

		std::fstream file;
		file.open(file_path, std::ios::out | std::ios::in);
		if (!file.is_open())
		{
			file.close();
			return false;
		}

		std::string line;
		while (file)
		{
			std::getline(file, line);

			const size_t settings_size = sizeof(CSettings);
			if (line.size() > settings_size * 8)
			{
				file.close();
				return false;
			}
			for (int i = 0; i < settings_size; i++)
			{
				byte current_byte = *reinterpret_cast<byte*>(uintptr_t(this) + i);
				for (int x = 0; x < 8; x++)
				{
					if (line[(i * 8) + x] == '1')
						current_byte |= 1 << x;
					else
						current_byte &= ~(1 << x);
				}
				*reinterpret_cast<byte*>(uintptr_t(this) + i) = current_byte;
			}
		}

		file.close();

		return true;
	}
	void CSettings::CreateConfig(std::string name)
	{
		CreateDirectory("C:\\alyal.club\\", NULL); CreateDirectory("C:\\alyal.club\\", NULL);
		std::ofstream ofs("C:\\alyal.club\\" + name + ".cfg");
	}

	/*void CSettings::ReadConfigs(LPCTSTR lpszFileName)
	{
		std::vector<std::string> configs;
		if (!strstr(lpszFileName, "gui.ini"))
		{
			configs.push_back(lpszFileName);
		}
	}
	
	void CSettings::RefreshConfigs()
	{
		std::vector<std::string> configs;
		std::fstream file;
		configs.clear();
		std::string file_path = "C:\\nnwareBETArecode\\*.cfg";
		INTERFACES::Engine->SearchFiles(file_path.c_str(), , FALSE);
	}*/

	bool CSettings::Remove(std::string file_name)
	{
		CreateDirectory("C:\\alyal.club", NULL);

		std::string file_path = "C:\\alyal.club\\" + file_name + ".cfg";
		remove(file_path.c_str());

		return true;
	}

	std::vector<std::string> CSettings::GetConfigs()
	{
		std::vector<std::string> configs;

		WIN32_FIND_DATA ffd;
		auto directory = "C:\\alyal.club\\*";
		auto hFind = FindFirstFile(directory, &ffd);

		while (FindNextFile(hFind, &ffd))
		{
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string file_name = ffd.cFileName;
				if (file_name.size() < 4) // .cfg
					continue;

				std::string end = file_name;
				end.erase(end.begin(), end.end() - 4); // erase everything but the last 4 letters
				if (end != ".cfg")
					continue;

				file_name.erase(file_name.end() - 4, file_name.end()); // erase the .cfg part
				configs.push_back(file_name);
			}
		}

		return configs;
	}
	bool CSettings::LoadLua(std::string file_name)
	{
		//std::string file_name = "config";
		CreateDirectory("C:\\alyal.club", NULL);

		std::string file_path = "C:\\alyal.club\\" + file_name + ".lua";

/*		std::fstream file;
		file.open(file_path, std::ios::out | std::ios::in);
		if (!file.is_open())
		{
			file.close();
			return false;
		}

		std::string line;
		while (file)
		{
			std::getline(file, line);

			const size_t settings_size = sizeof(CSettings);
			if (line.size() > settings_size * 8)
			{
				file.close();
				return false;
			}
			for (int i = 0; i < settings_size; i++)
			{
				byte current_byte = *reinterpret_cast<byte*>(uintptr_t(this) + i);
				for (int x = 0; x < 8; x++)
				{
					if (line[(i * 8) + x] == '1')
						current_byte |= 1 << x;
					else
						current_byte &= ~(1 << x);
				}
				*reinterpret_cast<byte*>(uintptr_t(this) + i) = current_byte;
			}
		}

		file.close();
		*/
		return true;
	}

	std::vector<std::string> CSettings::GetLuas()
	{
		std::vector<std::string> luas;

		WIN32_FIND_DATA ffd;
		auto directory = "C:\\alyal.club\\*";
		auto hFind = FindFirstFile(directory, &ffd);

		while (FindNextFile(hFind, &ffd))
		{
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string file_name = ffd.cFileName;
				if (file_name.size() < 4) // .cfg
					continue;

				std::string end = file_name;
				end.erase(end.begin(), end.end() - 4); // erase everything but the last 4 letters
				if (end != ".lua")
					continue;

				file_name.erase(file_name.end() - 4, file_name.end()); // erase the .cfg part
				luas.push_back(file_name);
			}
		}

		return luas;
	}
}