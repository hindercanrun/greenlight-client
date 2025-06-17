#include "..\..\std_include.hpp"
#include "lui.hpp"
#include "cmd.hpp"

#include "..\..\utils\hook.hpp"

namespace lui
{
	namespace
	{
		utils::hook::detour lua_init_hook;
		void lua_init_stub(bool frontend)
		{
			game::Com_Printf(15, "LUI: Starting up...\n");

			auto invoke = lua_init_hook.invoke<void(*)(bool)>();
			invoke(frontend);
		}

		utils::hook::detour shutdown_lua_hook;
		void shutdown_lua_stub()
		{
			game::Com_Printf(15, "LUI: Shutting down...\n");

			auto invoke = shutdown_lua_hook.invoke<void(*)()>();
			invoke();
		}

		utils::hook::detour kore_logger_hook;
		void kore_logger_stub(struct lua_State* unkown, const char* msg, ...)
		{
			game::Com_Printf(15, msg);
		}

		void reload_f()
		{
			game::Com_Printf(15, "Reloading LUI...");

			game::Key_RemoveCatcher(0, -9);
			game::LUI_CoD_Shutdown();
			game::LUI_CoD_Init(false);
		}
	}

	void register_cmds()
	{
		cmd::add("luiReload", reload_f);
	}

	void changes()
	{
		register_cmds();

		lua_init_hook.create(0x828BDA08, lua_init_stub); // LUI::LUI_Init
		shutdown_lua_hook.create(0x828B7200, shutdown_lua_stub); // LUI::LUI_Shutdown

		// restore kore logging
		kore_logger_hook.create(0x82854208, kore_logger_stub); // koreDefaultLogger
	}
}