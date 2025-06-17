#include "..\..\std_include.hpp"
#include "patches.hpp"

#include "..\..\utils\hook.hpp"

namespace patches
{
	namespace
	{
		utils::hook::detour sys_init_hook;

		void sys_init()
		{
			symbols::Cmd_SetAutoComplete("exec", "", "cfg");
			sys_init_hook.invoke<void>();
		}

		void register_hooks()
		{
			// set D: to game: to allow game function reading
			utils::hook::set_string(0x82B5F688, "game:");

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

			sys_init_hook.create(0x825D6DA0, sys_init);
		}
	}

	void load()
	{
		register_hooks();
	}
}