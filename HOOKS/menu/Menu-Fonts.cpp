#pragma once
#include "../../includes.h"
#include "../../SDK/ISurface.h"
#include "../../UTILS/render.h"
#include "../../SDK/IEngine.h"
#include "Menu-Fonts.h"
enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};
namespace MENU_FONTS {
	unsigned int CourierNew;
	unsigned int Tahomaboldsmall;
	unsigned int Tahomabold;
	unsigned int icon;
	unsigned int WeaponESP;
	unsigned int SmallText;
	unsigned int SmallText_bold;
	unsigned int WeaponIcon;
	unsigned int weapontabicon;
	unsigned int TabFont;
	unsigned int Tabicons;
	unsigned int IndicatorFont;
	unsigned int SPECIALSmallText;
	unsigned int Verdanashmol;

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

		weapontabicon = RENDER::CreateF("AstriumWep", 25, 700, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE | FONTFLAG_ANTIALIAS);
		Tahomaboldsmall = RENDER::CreateF("Tahoma", 12, 700, 0, 0, FONTFLAG_ANTIALIAS);
		Tahomabold = RENDER::CreateF("Tahoma", 16, 700, 0, 0, FONTFLAG_ANTIALIAS);
		CourierNew = RENDER::CreateF("Verdana", 13, 800, 0, 0, FONTFLAG_DROPSHADOW);
		Verdanashmol = RENDER::CreateF("Tahoma", 12, 400, 0, 0, FONTFLAG_ANTIALIAS);
		icon = RENDER::CreateF("Counter-Strike", 22, 4500, 0, 0, FONTFLAG_ANTIALIAS);
		SmallText = RENDER::CreateF("Tahoma", 12, 400, 0, 0, FONTFLAG_DROPSHADOW);
		SmallText_bold = RENDER::CreateF("Tahoma", 13, 400, 0, 0, FONTFLAG_ANTIALIAS);
		SPECIALSmallText = RENDER::CreateF("Tahoma", 12, 700, 0, 0, FONTFLAG_ANTIALIAS);
		WeaponESP = RENDER::CreateF("Small Fonts", 8, 400, 0, 0, FONTFLAG_OUTLINE);
		WeaponIcon = RENDER::CreateF("Counter-Strike", 24, 400, 0, 0, FONTFLAG_ANTIALIAS);
		TabFont = RENDER::CreateF("Verdana", 12, 200, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);
		Tabicons = RENDER::CreateF("Cherryfont", 26, 200, 0, 0, FONTFLAG_ANTIALIAS);
		IndicatorFont = RENDER::CreateF("Verdana", 24, 900, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	}
}