#include "..\..\std_include.hpp"
#include "lui.hpp"
#include "cmd.hpp"

#include "..\..\utils\hook.hpp"

namespace lui
{
	namespace
	{
		utils::hook::detour lua_init_hook;
		void lua_init(bool frontend)
		{
			symbols::Com_Printf(15, "LUI: Starting up...\n");

			auto invoke = lua_init_hook.invoke<void(*)(bool)>();
			invoke(frontend);
		}

		utils::hook::detour lua_shutdown_hook;
		void lua_shutdown()
		{
			symbols::Com_Printf(15, "LUI: Shutting down...\n");

			auto invoke = lua_shutdown_hook.invoke<void(*)()>();
			invoke();
		}

		utils::hook::detour kore_logger_hook;
		void kore_logger(struct lua_State* unkown, const char* msg, ...)
		{
			symbols::Com_Printf(15, msg);
		}

		void reload_f()
		{
			symbols::Com_Printf(15, "Reloading LUI...");

			symbols::Key_RemoveCatcher(0, -9);
			symbols::LUI_CoD_Shutdown();
			symbols::LUI_CoD_Init(false);
		}

		void register_cmds()
		{
			cmd::add("luiReload", reload_f);
		}

		void register_hooks()
		{
			lua_init_hook.create(0x828BDA08, lua_init);
			lua_shutdown_hook.create(0x828B7200, lua_shutdown);

			// restore kore logging
			kore_logger_hook.create(0x82854208, kore_logger);
		}
	}

	void load()
	{
		register_cmds();
		register_hooks();
	}
}