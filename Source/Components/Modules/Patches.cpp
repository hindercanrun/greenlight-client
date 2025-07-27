#include "../../Std_Include.h"
#include "Patches.h"
#include "../Loader.h"

#include "../../Utils/Hook.h"

namespace Patches
{
	// This function gets called once by the game, so it's okay to do our printing in.
	Utils::Hook::Detour FS_InitFilesystem_Hook;
	void FS_InitFilesystem()
	{
		DWORD start = Symbols::Sys_Milliseconds();

		// Print our loaded modules.
		Symbols::Com_Printf(0, "\nLoading modules...\n");
		for (int i = 0; i < Loader::g_moduleCount; ++i)
		{
			Symbols::Com_Printf(0, "%d: %s\n", i + 1, Loader::g_modules[i].name);
		}
		
		// Print the time it took to load them all.
		DWORD end = GetTickCount();	
		DWORD duration = end - start;
		Symbols::Com_Printf(0, "Loaded all modules in %lu ms\n\n", duration);

		auto Invoke = FS_InitFilesystem_Hook.Invoke<void(*)()>();
		Invoke();
	}

	Utils::Hook::Detour DebugReportProfileVars_Hook;
	void DebugReportProfileVars(int controllerIndex, const char* fmt, ...)
	{
		char buf[256];

		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, sizeof(buf), fmt, args);
		va_end(args);

		Symbols::Com_Printf(33, "%s\n", buf);

		auto Invoke = DebugReportProfileVars_Hook.Invoke<void(*)(int, const char*)>();
		Invoke(controllerIndex, fmt);
	}

	void RegisterHooks()
	{
		FS_InitFilesystem_Hook.Create(0x82588FE0, FS_InitFilesystem); // Print loaded modules

		Utils::Hook::SetValue(0x82483100, 0x60000000); // Nop first Com_Printf in DebugReportProfileVars then call our hook.
		DebugReportProfileVars_Hook.Create(0x824830D8, DebugReportProfileVars); // Readd variadic formatting to fix formatting.

		*(char*)0x8207C3B8 = '\0'; // Remove [timestamp][channel] string from log file.

		*(char*)0x8207A8DC = '\0'; // Remove 'exe name' from the log file
		*(char*)0x8207A8BC = '\0'; // Remove unneeded build info + timestamp from the log file

		// Redirect the console log file
		Utils::Hook::SetString(0x8207A8A4, "Mod/Logs/Console.log");
	}

	void UnregisterHooks()
	{
		FS_InitFilesystem_Hook.Remove();
		DebugReportProfileVars_Hook.Remove();
	}

	void Load()
	{
		RegisterHooks();
	}

	void Unload()
	{
		UnregisterHooks();
	}
}