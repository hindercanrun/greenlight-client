#include "..\..\std_include.hpp"
#include "cmd.hpp"

#include "../../utils/hook.hpp"

namespace cmd
{
	namespace
	{
		static std::vector<structs::cmd_function_s*> cmds;
		structs::cmd_function_s* allocate_cmd()
		{
			auto* cmd = new structs::cmd_function_s;
			cmds.push_back(cmd);

			return cmd;
		}

		utils::hook::detour handle_missing_cmd_hook;
		void handle_missing_cmd(const char* msg, bool from_rcon)
		{
			if (!symbols::Dvar_FindVar(msg))
			{
				symbols::Com_Printf(24, "unknown dvar :: '%s'\n", msg);
			}
			else if (!symbols::Cmd_FindCommand(msg))
			{
				symbols::Com_Printf(24, "unknown command :: '%s'\n", msg);
			}
		}

		void register_hooks()
		{
			handle_missing_cmd_hook.create(0x824AD688, handle_missing_cmd);
		}
	}

	void add(const char* name, void(WINAPIV* func)())
	{
		symbols::Cmd_AddCommandInternal(name, func, allocate_cmd());
	}

	int params()
	{
		return symbols::Cmd_Argc();
	}

	const char* param(int value)
	{
		return symbols::Cmd_Argv(value);
	}

	void load()
	{
		register_hooks();
	}
}
