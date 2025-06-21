#include "../../std_include.hpp"
#include "assertion.hpp"

#include "../../utils/hook.hpp"

namespace assertion
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
			// my custom assertion handler
			assert_hook.create(0x825661F0, assert);
		}
	}

	void load()
	{
		register_hooks();
	}
}