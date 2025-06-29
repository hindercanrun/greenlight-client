#include "..\..\std_include.hpp"
#include "drawing.hpp"

#include "..\..\utils\hook.hpp"
#include "..\..\utils\string.hpp"

namespace drawing
{
	namespace
	{
		structs::Font_s** small_font;
		void draw_fps()
		{
			if (!*small_font)
				return;

			float fps_colour_good[4] = { 0.6f, 1.0f, 0.0f, 1.0f };
			float fps_colour_ok[4] = {1.0f, 0.7f, 0.3f, 1.0f};
			float fps_colour_bad[4] = {1.0f, 0.3f, 0.3f, 1.0f};
			float fps_colour_very_bad[4] = { 1.0f, 0.1f, 0.1f, 1.0f };

			int fps = (int)(float)(1000.0 / symbols::cg_perfInfo->frame.average);
			const float* fps_colour = (fps >= 60) ? fps_colour_good :
									  (fps > 50)  ? fps_colour_ok :
													fps_colour_bad;

			const char* fps_string = utils::string::va("%i", fps);
			symbols::R_AddCmdDrawText(fps_string, std::numeric_limits<int>::max(), *small_font,
				1245.0f, 30.0f, 1.0f, 1.0f, 0.0f, fps_colour, 3);
		}

		void draw_watermark()
		{
			if (!*small_font)
				return;

			const float colour[4] = { 1.0f, 1.0f, 1.0f, 0.25f };

			const char* text = "T6_greenlight_mp :: modification by hindercanrun";
			symbols::R_AddCmdDrawText(text, std::numeric_limits<int>::max(), *small_font,
				2.0f, 700.0f, 0.8f, 0.8f, 0.0f, colour, 3);

			const char* date_string = utils::string::va("date: %s", utils::string::get_time_and_date());
			symbols::R_AddCmdDrawText(date_string, std::numeric_limits<int>::max(), *small_font,
				2.0f, 720.0f, 0.8f, 0.8f, 0.0f, colour, 3);
		}

		utils::hook::detour draw_screen_hook;
		void draw_screen(int client_num)
		{
			dvars::register_dvars();
			if (dvars::mod_drawFPS->current.enabled)
				draw_fps();
			if (dvars::mod_drawWatermark->current.enabled)
				draw_watermark();

			auto invoke = draw_screen_hook.invoke<void(*)(int)>();
			invoke(client_num);
		}

		void register_hooks()
		{
			small_font = reinterpret_cast<structs::Font_s**>(0x842CE7E8);

			draw_screen_hook.create(0x8232BA80, draw_screen);

			// cg_drawfps font
			utils::hook::set_string(0x820141B4, "fonts/720/smallFont");

			// p-host string
			utils::hook::set_string(0x82007AD8, " host");
		}
	}

	void load()
	{
		register_hooks();
	}
}