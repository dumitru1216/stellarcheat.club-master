#pragma once
#include <Windows.h>
#include <string>
#include <filesystem>
#include "../../includes.h"


struct ColorV2
{
	float red, green, blue, alpha;
};

class Menu
{
public:
	bool menuOpened;
	void Render();
	int PlayerList_Player[1024];
	void DoPlayerList(int x, int y, int w, int h);
	void colorp(CColor* clr, bool* open, float offset = 0.f);
	void hsv_to_rgb(float h, float s, float v, float& out_r, float& out_g, float& out_b) const;
	CColor hsv_to_rgb(float h, float s, float v);

	void ScrollableGroupBox(int x, int y, int w, int h, std::string name, bool dont_draw = false, bool has_subtabs = false);
	void groupbox(int x, int y, int w, int h, std::string name, bool dont_draw = false, bool has_subtabs = false);
	void gamebox(int x, int y, int w, int h, bool dont_draw);
	void repeatythingy(int subtabname, int totaltabs);
	void Drawgroupboxend(int x, int y, int w, int h);
	struct
	{
		bool Aimbot;
		int Hitchance;
		int HitchanceValue;
		int Mindmg;
		bool Resolver[2] = { false,false };
		int BodyScale;
		int HeadScale;
		bool MultiPoint;
		bool DelayShot;
		bool IgnoreLimbs;
		bool Autostop;
		bool OneTickChoke;
		bool BaimLethal;
		bool BaimPitch;
		bool BaimInAir;

		bool Antiaim;
		bool DesyncAngle;
		bool RandJitterInRange;
		int	JitterRange;
		int	Fakelag;
		bool FakeLagOnPeek;
		bool ChokeShotOnPeek;

		bool Esp;
		int Name;
		int HealthVal;
		int Weapon;
		bool Box;
		bool HealthBar;
		bool HitboxPoints;
		bool NoZoom;
		int Fov;
		bool Crosshair;

		bool Bhop;
		bool AutoStrafe;
		bool LegitBacktrack;
		bool Ak47meme;
		bool RemoveScope;
		int	Test;
	}Config;

private:
	Vector _pos = Vector(500, 200, 0);
	int ControlsX;
	int GroupTabBottom;
	int OffsetY;
	int screen_width;
	int screen_height;
	int y_offset;
	int x_offset;
	int MenuAlpha_Main;
	int MenuAlpha_Text;
	int groupbox_scroll_add;
	int groupbox_width;
	int groupbox_bottom;
	int groupbox_top;
	bool we_are_clipping;
	int how_many_controls;
	typedef void(*ButtonCallback_t)(void);
	void checkbox(std::string name, bool* item);
	void text(std::string name);
	void combobox(int size, std::string name, std::string *itemname, int* item);
	void EmptySpace(std::string name);
	void color_selector(std::string name, float * red, float * green, float * blue, float * alpha);
	void color_selector_chams(std::string name, float * red, float * green, float * blue, float * alpha, float * width);
	void TextBox(std::string name, std::string *text);
	void button(ButtonCallback_t callback, std::string title);
	void ListBox(int x, int y, int w, int h, int size, std::string name, std::string *itemname, int* item);
	void slider(int max, std::string name, int * item, std::string suffix, int step);
	void fake_slider(int max, std::string name, int* item, std::string suffix, int step);
	void sliderfloat(float max, std::string name, float * item, std::string suffix, float step);
	void MultiComboBox(float indexes, std::string name, std::string * itemname, bool * item);
	void keybind(int * item, std::string name);
	void ColorPicker(std::string name, ColorV2& item);
};

extern Menu g_Menu;