#include "../../includes.h"

#include "../../UTILS/interfaces.h"
#include "../hooks.h"
#include "../../SDK/IEngine.h"
#include "../../SDK/CInputSystem.h"
#include "../../SDK/CClientEntityList.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDk/ConVar.h"
#include "../../FEATURES/SkinChanger.h"

#include "Menu.h"

namespace PPGUI
{
	namespace MENU
	{
		bool testc;
		int testk;
		void Do()
		{
			auto SetColor = []() -> void
			{
				const auto theme_color = HELPERS::PPGUI_Color(255, 255, 2555, 255);

				ppgui_colors[PPGUI_COLOR_WINDOW_BODY] = HELPERS::PPGUI_Color(26, 32, 36, 255);
				ppgui_colors[PPGUI_COLOR_WINDOW_BODY_OUTLINE] = HELPERS::PPGUI_Color(16, 22, 26, 255);
				ppgui_colors[PPGUI_COLOR_WINDOW_TITLE_BAR] = HELPERS::PPGUI_Color(16, 22, 26, 255);
				ppgui_colors[PPGUI_COLOR_WINDOW_TITLE_BAR_TEXT] = theme_color;
				ppgui_colors[PPGUI_COLOR_WINDOW_TITLE_BAR_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_WINDOW_TITLE_BAR_BACKGROUND_TEXT] = HELPERS::PPGUI_Color(19, 160, 216, 0);

				ppgui_colors[PPGUI_COLOR_CHECKBOX_UNSELECTED_BODY] = HELPERS::PPGUI_Color(200, 200, 200, 255);
				ppgui_colors[PPGUI_COLOR_CHECKBOX_UNSELECTED_BODY_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 150);
				ppgui_colors[PPGUI_COLOR_CHECKBOX_SELECTED_BODY] = HELPERS::PPGUI_Color(123, 032, 012, 150);
				ppgui_colors[PPGUI_COLOR_CHECKBOX_SELECTED_BODY_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 150);
				ppgui_colors[PPGUI_COLOR_CHECKBOX_TEXT] = HELPERS::PPGUI_Color(200, 200, 200, 255);
				ppgui_colors[PPGUI_COLOR_CHECKBOX_HOVERED_TEXT] = HELPERS::PPGUI_Color::White();

				ppgui_colors[PPGUI_COLOR_BUTTON_BODY] = HELPERS::PPGUI_Color(80, 80, 80, 255);
				ppgui_colors[PPGUI_COLOR_BUTTON_BODY_CLICKED] = HELPERS::PPGUI_Color(100, 100, 100, 255);
				ppgui_colors[PPGUI_COLOR_BUTTON_OUTLINE] = HELPERS::PPGUI_Color(5, 75, 125, 100);
				ppgui_colors[PPGUI_COLOR_BUTTON_TEXT] = HELPERS::PPGUI_Color(200, 200, 200, 255);
				ppgui_colors[PPGUI_COLOR_BUTTON_TEXT_CLICKED] = theme_color;

				ppgui_colors[PPGUI_COLOR_GROUPBOX_BODY] = HELPERS::PPGUI_Color(34, 43, 48, 240);
				ppgui_colors[PPGUI_COLOR_GROUPBOX_BODY_HOVERED] = HELPERS::PPGUI_Color(34, 43, 48, 240);
				ppgui_colors[PPGUI_COLOR_GROUPBOX_OUTLINE] = HELPERS::PPGUI_Color(5, 75, 125, 100);
				ppgui_colors[PPGUI_COLOR_GROUPBOX_TEXT] = HELPERS::PPGUI_Color(200, 200, 200, 255);
				ppgui_colors[PPGUI_COLOR_GROUPBOX_TEXT_HOVERED] = HELPERS::PPGUI_Color::White();

				ppgui_colors[PPGUI_COLOR_SLIDER_BODY] = HELPERS::PPGUI_Color(80, 80, 80, 255);
				ppgui_colors[PPGUI_COLOR_SLIDER_BODY_SELECTED] = theme_color;
				ppgui_colors[PPGUI_COLOR_SLIDER_OUTLINE] = HELPERS::PPGUI_Color(5, 75, 125, 100);
				ppgui_colors[PPGUI_COLOR_SLIDER_TEXT_NAME] = HELPERS::PPGUI_Color(200, 200, 200, 255);
				ppgui_colors[PPGUI_COLOR_SLIDER_TEXT_NAME_HOVERED] = HELPERS::PPGUI_Color::White();
				ppgui_colors[PPGUI_COLOR_SLIDER_VALUE_TEXT] = HELPERS::PPGUI_Color::White();

				ppgui_colors[PPGUI_COLOR_SEPARATOR_BODY] = HELPERS::PPGUI_Color(100, 100, 100, 255);
				ppgui_colors[PPGUI_COLOR_SEPARATOR_TEXT] = HELPERS::PPGUI_Color::White();

				ppgui_colors[PPGUI_COLOR_KEYBIND_BODY] = HELPERS::PPGUI_Color(80, 80, 80, 0);
				ppgui_colors[PPGUI_COLOR_KEYBIND_BODY_SELECTED] = HELPERS::PPGUI_Color(110, 110, 110, 0);
				ppgui_colors[PPGUI_COLOR_KEYBIND_OUTLINE ] = HELPERS::PPGUI_Color(5, 75, 125, 0);
				ppgui_colors[PPGUI_COLOR_KEYBIND_TEXT_NAME] = HELPERS::PPGUI_Color(200, 200, 200, 255);
				ppgui_colors[PPGUI_COLOR_KEYBIND_TEXT_NAME_HOVERED] = HELPERS::PPGUI_Color::White();
				ppgui_colors[PPGUI_COLOR_KEYBIND_TEXT_VALUE] = HELPERS::PPGUI_Color::White();

				ppgui_colors[PPGUI_COLOR_TEXT_INPUT_BODY] = HELPERS::PPGUI_Color(80, 80, 80, 255);
				ppgui_colors[PPGUI_COLOR_TEXT_INPUT_BODY_SELECTED] = HELPERS::PPGUI_Color(110, 110, 110, 255);
				ppgui_colors[PPGUI_COLOR_TEXT_INPUT_OUTLINE] = HELPERS::PPGUI_Color(5, 75, 125, 100);
				ppgui_colors[PPGUI_COLOR_TEXT_INPUT_TEXT_VALUE] = HELPERS::PPGUI_Color::White();
				ppgui_colors[PPGUI_COLOR_TEXT_INPUT_TEXT_NAME] = HELPERS::PPGUI_Color(200, 200, 200, 255);
				ppgui_colors[PPGUI_COLOR_TEXT_INPUT_TEXT_HOVERED] = HELPERS::PPGUI_Color::White();

				ppgui_colors[PPGUI_COLOR_COMBOBOX_BODY_SELECTED] = HELPERS::PPGUI_Color(80, 80, 80, 255);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_OUTLINE_SELECTED] = HELPERS::PPGUI_Color(0, 0, 0, 150);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_BODY_ITEM] = HELPERS::PPGUI_Color(50, 50, 50, 255);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_OUTLINE_ITEM] = HELPERS::PPGUI_Color(0, 204, 250, 150);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_SELECTED_TEXT_ITEM] = theme_color;
				ppgui_colors[PPGUI_COLOR_COMBOBOX_TEXT_ITEM] = HELPERS::PPGUI_Color(200, 200, 200, 255);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_TEXT_SELECTED] = HELPERS::PPGUI_Color(200, 200, 200, 255);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_TEXT_SELECTED_HOVERED] = HELPERS::PPGUI_Color::White();
				ppgui_colors[PPGUI_COLOR_COMBOBOX_TEXT_NAME] = HELPERS::PPGUI_Color(200, 200, 200, 255);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_TEXT_NAME_HOVERED] = HELPERS::PPGUI_Color::White();

				ppgui_colors[PPGUI_COLOR_COLOR_PICKER_BODY_COLOR] = HELPERS::PPGUI_Color(50, 50, 50, 255);
				ppgui_colors[PPGUI_COLOR_COLOR_PICKER_OUTLINE] = HELPERS::PPGUI_Color(5, 75, 125, 100);
				ppgui_colors[PPGUI_COLOR_COLOR_PICKER_TEXT] = HELPERS::PPGUI_Color(200, 200, 200, 255);
				ppgui_colors[PPGUI_COLOR_COLOR_PICKER_TEXT_HOVERED] = HELPERS::PPGUI_Color::White();

				ppgui_colors[PPGUI_COLOR_TAB_BODY] = HELPERS::PPGUI_Color(40, 40, 40, 255);
				ppgui_colors[PPGUI_COLOR_TAB_BODY_SELECTED] = HELPERS::PPGUI_Color(70, 70, 70, 255);
				ppgui_colors[PPGUI_COLOR_TAB_TEXT] = HELPERS::PPGUI_Color(255, 255, 255, 255);
				ppgui_colors[PPGUI_COLOR_TAB_TEXT_SELECTED] = theme_color;
				ppgui_colors[PPGUI_COLOR_TAB_OUTLINE] = HELPERS::PPGUI_Color(5, 75, 125, 100);

				ppgui_colors[PPGUI_COLOR_TOOL_TIP_BODY] = HELPERS::PPGUI_Color(200, 200, 200, 255);
				ppgui_colors[PPGUI_COLOR_TOOL_TIP_OUTLINE] = HELPERS::PPGUI_Color(5, 75, 125, 100);
				ppgui_colors[PPGUI_COLOR_TOOL_TIP_TEXT] = HELPERS::PPGUI_Color(0, 0, 0, 255);
			};
			auto SetAttributes = []() -> void
			{
				ppgui_attributes[PPGUI_ATTRIBUTE_PADDING_HEIGHT] = 12;
				ppgui_attributes[PPGUI_ATTRIBUTE_PADDING_WIDTH] = 12;
				ppgui_attributes[PPGUI_ATTRIBUTE_SCROLL_SPEED] = 10;

				ppgui_attributes[PPGUI_ATTRIBUTE_WINDOW_TITLE_BAR_HEIGHT] = 30;

				ppgui_attributes[PPGUI_ATTRIBUTE_CHECKBOX_HEIGHT] = 10;
				ppgui_attributes[PPGUI_ATTRIBUTE_CHECKBOX_WIDTH] = 10;

				ppgui_attributes[PPGUI_ATTRIBUTE_BUTTON_HEIGHT] = 16;
				ppgui_attributes[PPGUI_ATTRIBUTE_BUTTON_WIDTH] = 100;
				ppgui_attributes[PPGUI_ATTRIBUTE_BUTTON_CLICK_DURATION] = 0.15f;

				ppgui_attributes[PPGUI_ATTRIBUTE_SLIDER_HEIGHT] = 6;
				ppgui_attributes[PPGUI_ATTRIBUTE_SLIDER_WIDTH] = 200;

				ppgui_attributes[PPGUI_ATTRIBUTE_KEYBIND_HEIGHT] = 14;
				ppgui_attributes[PPGUI_ATTRIBUTE_KEYBIND_WIDTH] = 100;

				ppgui_attributes[PPGUI_ATTRIBUTE_TEXT_INPUT_HEIGHT] = 14;
				ppgui_attributes[PPGUI_ATTRIBUTE_TEXT_INPUT_WIDTH] = 200;

				ppgui_attributes[PPGUI_ATTRIBUTE_COMBOBOX_WIDTH] = 200;
				ppgui_attributes[PPGUI_ATTRIBUTE_COMBOBOX_HEIGHT] = 14;
				ppgui_attributes[PPGUI_ATTRIBUTE_COMBOBOX_MAX_ITEMS_DISPLAYED] = 8;

				ppgui_attributes[PPGUI_ATTRIBUTE_COLOR_PICKER_WIDTH] = 30;
				ppgui_attributes[PPGUI_ATTRIBUTE_COLOR_PICKER_HEIGHT] = 10;
				ppgui_attributes[PPGUI_ATTRIBUTE_COLOR_PICKER_OPEN_WIDTH] = 250;
				ppgui_attributes[PPGUI_ATTRIBUTE_COLOR_PICKER_OPEN_HEIGHT] = 150;
				ppgui_attributes[PPGUI_ATTRIBUTE_COLOR_PICKER_SLIDER_WIDTH] = 14;
				ppgui_attributes[PPGUI_ATTRIBUTE_COLOR_PICKER_PADDING] = 8;
				ppgui_attributes[PPGUI_ATTRIBUTE_COLOR_PICKER_PIXELATION] = 6;

				ppgui_attributes[PPGUI_ATTRIBUTE_TAB_HEIGHT] = 16;
				ppgui_attributes[PPGUI_ATTRIBUTE_TAB_ITEM_PADDING] = 10;

				ppgui_attributes[PPGUI_ATTRIBUTE_TOOL_TIP_LINE_HEIGHT] = 16;
			};
			auto SetFonts = []()
			{
				ppgui_fonts[PPGUI_FONT_WINDOW_NAME] = FONTS::menu_window_font;
				ppgui_fonts[PPGUI_FONT_WINDOW_NAME_BEHIND] = FONTS::menu_window_blurr_font;

				ppgui_fonts[PPGUI_FONT_GROUPBOX] = FONTS::menu_groupbox_font;

				ppgui_fonts[PPGUI_FONT_SEPARATOR] = FONTS::menu_separator_font;

				ppgui_fonts[PPGUI_FONT_CHECKBOX] = FONTS::menu_checkbox_font;

				ppgui_fonts[PPGUI_FONT_BUTTON] = FONTS::menu_button_font;

				ppgui_fonts[PPGUI_FONT_SLIDER_NAME] = FONTS::menu_slider_font;
				ppgui_fonts[PPGUI_FONT_SLIDER_VALUE] = FONTS::menu_slider_value_font;

				ppgui_fonts[PPGUI_FONT_KEYBIND_NAME] = FONTS::menu_keybind_name_font;
				ppgui_fonts[PPGUI_FONT_KEYBIND_VALUE] = FONTS::menu_keybind_value_font;

				ppgui_fonts[PPGUI_FONT_TEXT_INPUT_NAME] = FONTS::menu_text_input_name_font;
				ppgui_fonts[PPGUI_FONT_TEXT_INPUT_VALUE] = FONTS::menu_text_input_value_font;

				ppgui_fonts[PPGUI_FONT_COMBOBOX_NAME] = FONTS::menu_combobox_name_font;
				ppgui_fonts[PPGUI_FONT_COMBOBOX_SELECTED_ITEM] = FONTS::menu_combobox_value_font;
				ppgui_fonts[PPGUI_FONT_COMBOBOX_ITEM_NAME] = FONTS::menu_combobox_value_font;

				ppgui_fonts[PPGUI_FONT_TAB_SELECTED_ITEM_NAME] = FONTS::menu_tab_font;
				ppgui_fonts[PPGUI_FONT_TAB_ITEM_NAME] = FONTS::menu_tab_font;

				ppgui_fonts[PPGUI_FONT_TOOL_TIP] = FONTS::menu_tool_tip_font;

				ppgui_fonts[PPGUI_FONT_COLOR_PICKER] = FONTS::menu_colorpicker_font;
			};

			auto SetDebugColor = []()
			{
				ppgui_colors[PPGUI_COLOR_WINDOW_BODY] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_WINDOW_BODY_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_WINDOW_TITLE_BAR] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_WINDOW_TITLE_BAR_TEXT] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_WINDOW_TITLE_BAR_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_WINDOW_TITLE_BAR_BACKGROUND_TEXT] = HELPERS::PPGUI_Color(0, 0, 0, 0);

				ppgui_colors[PPGUI_COLOR_CHECKBOX_UNSELECTED_BODY] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_CHECKBOX_UNSELECTED_BODY_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_CHECKBOX_SELECTED_BODY] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_CHECKBOX_SELECTED_BODY_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_CHECKBOX_TEXT] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_CHECKBOX_HOVERED_TEXT] = HELPERS::PPGUI_Color(0, 0, 0, 0);

				ppgui_colors[PPGUI_COLOR_BUTTON_BODY] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_BUTTON_BODY_CLICKED] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_BUTTON_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_BUTTON_TEXT] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_BUTTON_TEXT_CLICKED] = HELPERS::PPGUI_Color(0, 0, 0, 0);

				ppgui_colors[PPGUI_COLOR_GROUPBOX_BODY] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_GROUPBOX_BODY_HOVERED] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_GROUPBOX_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_GROUPBOX_TEXT] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_GROUPBOX_TEXT_HOVERED] = HELPERS::PPGUI_Color(0, 0, 0, 0);

				ppgui_colors[PPGUI_COLOR_SLIDER_BODY] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_SLIDER_BODY_SELECTED] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_SLIDER_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_SLIDER_TEXT_NAME] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_SLIDER_TEXT_NAME_HOVERED] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_SLIDER_VALUE_TEXT] = HELPERS::PPGUI_Color(0, 0, 0, 0);

				ppgui_colors[PPGUI_COLOR_SEPARATOR_BODY] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_SEPARATOR_TEXT] = HELPERS::PPGUI_Color(0, 0, 0, 0);

				ppgui_colors[PPGUI_COLOR_KEYBIND_BODY] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_KEYBIND_BODY_SELECTED] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_KEYBIND_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_KEYBIND_TEXT_NAME] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_KEYBIND_TEXT_NAME_HOVERED] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_KEYBIND_TEXT_VALUE] = HELPERS::PPGUI_Color(0, 0, 0, 0);

				ppgui_colors[PPGUI_COLOR_TEXT_INPUT_BODY] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_TEXT_INPUT_BODY_SELECTED] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_TEXT_INPUT_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_TEXT_INPUT_TEXT_VALUE] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_TEXT_INPUT_TEXT_NAME] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_TEXT_INPUT_TEXT_HOVERED] = HELPERS::PPGUI_Color(0, 0, 0, 0);

				ppgui_colors[PPGUI_COLOR_COMBOBOX_BODY_SELECTED] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_OUTLINE_SELECTED] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_BODY_ITEM] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_OUTLINE_ITEM] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_SELECTED_TEXT_ITEM] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_TEXT_ITEM] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_TEXT_SELECTED] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_TEXT_SELECTED_HOVERED] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_TEXT_NAME] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_COMBOBOX_TEXT_NAME_HOVERED] = HELPERS::PPGUI_Color(0, 0, 0, 0);

				ppgui_colors[PPGUI_COLOR_COLOR_PICKER_BODY_COLOR] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_COLOR_PICKER_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_COLOR_PICKER_TEXT] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_COLOR_PICKER_TEXT_HOVERED] = HELPERS::PPGUI_Color(0, 0, 0, 0);

				ppgui_colors[PPGUI_COLOR_TAB_BODY] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_TAB_BODY_SELECTED] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_TAB_TEXT] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_TAB_TEXT_SELECTED] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_TAB_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 0);

				ppgui_colors[PPGUI_COLOR_TOOL_TIP_BODY] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_TOOL_TIP_OUTLINE] = HELPERS::PPGUI_Color(0, 0, 0, 0);
				ppgui_colors[PPGUI_COLOR_TOOL_TIP_TEXT] = HELPERS::PPGUI_Color(0, 0, 0, 0);
			};

			///SetDebugColor();
			SetColor();
			SetAttributes();
			SetFonts();
			static char main_tabs[][PPGUI_MAX_STRING_LENGTH] =
			{
				"test",
				"test",
			};

			/// Start
			PPGUI_BeginFrame();

			/// Menu toggle
			static bool menu_open = false;
			if (menu.GetInput().DidClickKey(VK_INSERT))
			{
				menu_open = !menu_open;

				static auto mouse = INTERFACES::cvar->FindVar("cl_mouseenable");
				mouse->SetValue(!menu_open);
			}

			if (!menu_open)
			{
				PPGUI_EndFrame();
				return;
			}
			/// Draw mouse
			PPGUI_DrawMouse();
			PPGUI_BeginWindow("FrameWork", HELPERS::PPGUI_Vector2D(1100, 200), HELPERS::PPGUI_Vector2D(500, 600));
			{
				static int selected_main_tab = 0;
				switch (PPGUI_Tab(2, main_tabs, selected_main_tab, false))
				{
				case 0: { /// Ragebot
					PPGUI_BeginGroupbox("Test", 270);
					{
						PPGUI_Checkbox("teest", testc, false);
						PPGUI_AttachToPreviousWidget();
						PPGUI_Keybind("##teest", testk, false);
					} PPGUI_EndGroupbox();
					break;
				}
				}

			} PPGUI_EndWindow();

			/// End
			PPGUI_EndFrame();
		}
	}
}