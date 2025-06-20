#include "..\..\std_include.hpp"
#include "patches.hpp"

#include "..\..\utils\hook.hpp"

namespace patches
{
	namespace
	{
		utils::hook::detour assert_hook;
		void assert(const char* filename, int line, int type, const char* fmt, ...)
		{
			char message[1024];

			va_list va;
			va_start(va, fmt);
			_vsnprintf_s(message, sizeof(message), fmt, va);
			message[1023] = '\0';

			symbols::Com_Printf(0, "\n");
			symbols::Com_Printf(0, "****************************************\n");
			symbols::Com_Printf(0, "*  Assertion Info:\n");
			symbols::Com_Printf(0, "*  Message:       %s\n", message);
			symbols::Com_Printf(0, "*  Plat:          %s\n", "xenon");
			symbols::Com_Printf(0, "*  File:          %s\n", filename);
			symbols::Com_Printf(0, "*  Line:          %d\n", line);
			symbols::Com_Printf(0, "****************************************\n");
		}

		void register_hooks()
		{
			// set D: to game: to allow game function reading
			utils::hook::set_string(0x82B5F688, "game:");

			// my custom assertion handler
			assert_hook.create(0x825661F0, assert);

			// redirect:
			//  log file
			//	missing asset log
			//	games log
			//	fake ents log
			//	ent stats log
			//	texture list log
			//	map texture usage log
			//	ui viewer camera log
			//	static model info log
			//	assert log
			//	bug name log
			utils::hook::set_string(0x8207A894, "mod/logs/assert.log");
			utils::hook::set_string(0x8207A8A4, "mod/logs/console.log");
			utils::hook::set_string(0x820410B4, "missingasset.csv");
			//utils::hook::set_string(0x82000000, "mod/logs/games.log");
			//utils::hook::set_string(0x82013E20, "mod/logs/fake_ent_budget.csv");
			// utils::hook::set_string(0x8209D308, "mod/logs/entity_stats.txt");
			// utils::hook::set_string(0x8218559C, "mod/logs/texture_list.csv");
			// utils::hook::set_string(0x8200768C, "mod/logs/%s_texture_usage.csv");
			// utils::hook::set_string(0x820AA998, "mod/logs/ui_viewer_camera.csv");
			// utils::hook::set_string(0x82C993D4, "mod/logs/static_xmodel_info.csv");
			// utils::hook::set_string(0x8207C754, "mod/logs/%s_%s.log");

			// redirect config_mp.cfg
			utils::hook::set_string(0x82079134, "mod/configs/player.cfg");
		}
	}

	void load()
	{
		register_hooks();
	}
}