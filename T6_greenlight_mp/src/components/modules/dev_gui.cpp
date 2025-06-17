#include "..\..\std_include.hpp"
#include "dev_gui.hpp"

#include "..\..\utils\hook.hpp"

namespace dev_gui
{
	namespace
	{
		utils::hook::detour get_screen_y_hook;
		utils::hook::detour get_screen_x_hook;
		utils::hook::detour devgui_frame_hook;
		utils::hook::detour load_lvl_ffs_hook;
		utils::hook::detour sub_824BCC00_hook;

		int get_screen_y_stub()
		{
			return 0;
		}

		int get_screen_x_stub()
		{
			return 0;
		}

		void devgui_frame_stub(int client_num)
		{
			static bool has_func_ran = false;
			if (has_func_ran)
			{
				return;
			}

			has_func_ran = true;

			// custom dev gui!
			game::Cbuf_InsertText(0, "exec_addtext mod/configs/devgui");
			game::Cbuf_InsertText(0, "exec_addtext mod/configs/devgui_mp");
		}

		void load_lvl_ffs_stub(const char* name)
		{
			// custom dev gui!
			game::Cbuf_InsertText(0, "exec_addtext mod/configs/devgui");
			game::Cbuf_InsertText(0, "exec_addtext mod/configs/devgui_mp");

			auto invoke = load_lvl_ffs_hook.invoke<void(*)(const char*)>();
			invoke(name);
		}

		void sub_824BCC00_stub()
		{
			// custom dev gui!
			game::Cbuf_InsertText(0, "exec_addtext mod/configs/devgui");
			game::Cbuf_InsertText(0, "exec_addtext mod/configs/devgui_mp");

			auto invoke = sub_824BCC00_hook.invoke<void(*)()>();
			invoke();
		}

		void safe_area()
		{
			get_screen_y_hook.create(0x8238B520, get_screen_y_stub);
			get_screen_x_hook.create(0x8238B588, get_screen_x_stub);
		}

		void devgui_cfgs()
		{
			game::Cbuf_InsertText(0, "exec_addtext mod/configs/devgui");
			game::Cbuf_InsertText(0, "exec_addtext mod/configs/devgui_mp");
		}
	}

	void changes()
	{
		//matches pc :)
		safe_area();

		devgui_frame_hook.create(0x82307620, devgui_frame_stub); // CL_DevGuiFrame
		load_lvl_ffs_hook.create(0x824BCCF8, load_lvl_ffs_stub); // Com_LoadLevelFastFiles
		sub_824BCC00_hook.create(0x824BCC00, sub_824BCC00_stub); // sub_824BCC00
	}
}