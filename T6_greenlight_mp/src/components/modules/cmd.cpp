#include "..\..\std_include.hpp"
#include "cmd.hpp"

#include "../../utils/hook.hpp"

namespace cmd
{

	namespace
	{
		std::vector<game::cmd_function_s*> cmds;

		utils::hook::detour handle_missing_cmd_hook;
		void handle_missing_cmd(const char* msg, bool from_rcon)
		{
			if (!game::Dvar_FindVar(msg))
			{
				game::Com_Printf(24, "unknown dvar :: '%s'\n", msg);
			}
			else if (!game::Cmd_FindCommand(msg))
			{
				game::Com_Printf(24, "unknown command :: '%s'\n", msg);
			}
		}
	}

	static game::cmd_function_s* allocate_cmd()
	{
		auto* cmd = new game::cmd_function_s;
		cmds.push_back(cmd);

		return cmd;
	}

	void add(const char* name, void(WINAPIV* func)())
	{
		game::Cmd_AddCommandInternal(name, func, allocate_cmd());
	}

	int params()
	{
		return game::Cmd_Argc();
	}

	const char* param(int value)
	{
		return game::Cmd_Argv(value);
	}

	void changes()
	{
		handle_missing_cmd_hook.create(0x824AD688, handle_missing_cmd);
	}
}
