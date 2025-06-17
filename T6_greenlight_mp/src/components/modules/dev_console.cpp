#include "..\..\std_include.hpp"
#include "dev_console.hpp"

#include "..\..\utils\hook.hpp"

namespace dev_console
{
/*
	namespace
	{
		utils::hook::detour is_dvar_command_hook;
		utils::hook::detour cmd_for_each_hook;

		bool is_command;

		bool is_dvar_command(const char* cmd)
		{
			is_command = game::Con_IsDvarCommand(cmd);
			return is_command;
		}

		void cmd_for_each(void (*callback)(const char* str))
		{
			if (!is_command)
			{
				auto invoke = cmd_for_each_hook.invoke<void(*)(void*)>();
				invoke(callback);
			}
		}
	}
*/

	void changes()
	{
		// console title
		utils::hook::set_string(0x8202A8DC, "COD_T6_R :: ");

		//is_dvar_command_hook.create(0x82301BA8, is_dvar_command);
		//cmd_for_each_hook.create(0x824AD4C0, cmd_for_each);
	}
}
