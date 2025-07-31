#include "../../Std_Include.h"
#include "../Loader.h"
#include "Patches.h"
#include "Localized_Strings.h"

#include "../../Utils/Hook.h"

namespace Patches
{
	// This function gets called once by the game, so it's okay to do our printing in.
	Utils::Hook::Detour FS_InitFilesystem_Hook;
	void FS_InitFilesystem()
	{
		DWORD start = GetTickCount();

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

	Utils::Hook::Detour Com_ExecStartupConfigs_Hook;
	void Com_ExecStartupConfigs(int localClientNum, const char* configFile)
	{
		static bool alreadyExecuted = false;
		if (alreadyExecuted)
		{
			return;
		}
		alreadyExecuted = true;

		auto Invoke = Com_ExecStartupConfigs_Hook.Invoke<void(*)(int, const char*)>();
		Invoke(localClientNum, configFile);
	}

#define TAB_SIZE	4
	char* FixTab(const char* input, char* outBuf, int outBufSize)
	{
		int col = 0;
		int outPos = 0;

		for (int i = 0; input[i] != '\0' && outPos < outBufSize - 1; ++i)
		{
			if (input[i] == '\t')
			{
				int spaces = TAB_SIZE - (col % TAB_SIZE);
				for (int s = 0; s < spaces && outPos < outBufSize - 1; ++s)
				{
					outBuf[outPos++] = ' ';
					col++;
				}
			}
			else
			{
				outBuf[outPos++] = input[i];
				if (input[i] == '\n')
				{
					col = 0;
				}
				else
				{
					col++;
				}
			}
		}
		outBuf[outPos] = '\0';
		return outBuf;
	}

	Utils::Hook::Detour CL_ConsolePrint_AddLine_Hook;
	char CL_ConsolePrint_AddLine(int localClientNum, int channel, const char* txt, int duration, int pixelWidth, char colour, int flags)
	{
#define TEXT_BUF_SIZE	4096
		static char txt2[TEXT_BUF_SIZE];
		FixTab(txt, txt2, sizeof(txt2));

		auto Invoke = CL_ConsolePrint_AddLine_Hook.Invoke<char(*)(int, int, const char*, int, int, char, int)>();
		return Invoke(localClientNum, channel, txt2, duration, pixelWidth, colour, flags);
	}

	Utils::Hook::Detour Com_OpenLogFile_Hook;
	void Com_OpenLogFile()
	{
		*Symbols::logfile = Symbols::FS_FOpenTextFileWrite("Redlight\\logs\\console.log");
	}

	void RegisterHooks()
	{
		FS_InitFilesystem_Hook.Create(0x82588FE0, FS_InitFilesystem); // Print loaded modules

		Utils::Hook::SetValue(0x82483100, 0x60000000); // Nop first Com_Printf in DebugReportProfileVars then call our hook.
		DebugReportProfileVars_Hook.Create(0x824830D8, DebugReportProfileVars); // Readd variadic formatting to fix formatting.

		Com_ExecStartupConfigs_Hook.Create(0x824B8550, Com_ExecStartupConfigs); // Make this only run once to prevent unneeded duplicate executing.

		CL_ConsolePrint_AddLine_Hook.Create(0x822FF2C8, CL_ConsolePrint_AddLine); // Fix \t showing as []

		Com_OpenLogFile_Hook.Create(0x824B5938, Com_OpenLogFile); // Replace with my own version

		Utils::Hook::SetValue(0x82316110, 0x60000000); // Nop LiveStorage_WaitOnStats to prevent delay on loading levels.
		Utils::Hook::SetValue(0x8276D2E8, 0x60000000); // Nop Live_Base_PumpForController to stop the game from trying to connect online.

		*(char*)0x8207C3B8 = '\0'; // Remove [timestamp][channel] string from log file.

		*(char*)0x8207A8DC = '\0'; // Remove 'exe name' from the log file
		*(char*)0x8207A8BC = '\0'; // Remove unneeded build info + timestamp from the log file

		*(char*)0x82029960 = '\0'; // Remove 'Build 363 xenon' from the console window as it's not needed

		// Redirect player config
		Utils::Hook::SetString(0x82079134, "Redlight/configs/config.cfg");

		// Below are just your general string edits.
		// Example:
		// Utils::Hook::SetString(<address>, <string>); // <comment> (optional)

		Utils::Hook::SetString(0x8218A174, "Creating Direct3D device...\n\n"); // Add a newline for cleaner logging
		Utils::Hook::SetString(0x8218A5D4, "Couldn't create Direct3D device: %s\n\n"); // Add a newline for cleaner logging + removed an 'a'
		Utils::Hook::SetString(0x82091F00, "GUMP(%s): %s"); // Remove newline for cleaner logging
		Utils::Hook::SetString(0x82040998, "DB allocation used %0.2f MB of memory\n");

		// Below are just your general dvar value edits.
		// Example:
		// Utils::Hook::SetValue(<address>, <value>); // <comment> (optional)

		Utils::Hook::SetValue(0x829D98EC, 0x38800000); // Force r_vsync to 0
		Utils::Hook::SetValue(0x824B6F40, 0x38800000); // Force com_maxfps to 0

		// Below are just your general localized string edits.
		// Example:
		// Localized_Strings::Override(string>, <string>); // <comment> (optional)

		// Change the offline MOTD to be our own one
		Localized_Strings::Override("TICKER_MESSAGE_CATEGORY_COD_CAPS", "Mod:");
		Localized_Strings::Override("TICKER_MESSAGE_DEFAULT_OFFLINE", "Redlight - Game Modification For Greenlight!");
	}

	void UnregisterHooks()
	{
		FS_InitFilesystem_Hook.Remove();
		DebugReportProfileVars_Hook.Remove();
		Com_ExecStartupConfigs_Hook.Remove();
		CL_ConsolePrint_AddLine_Hook.Remove();
		Com_OpenLogFile_Hook.Remove();
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