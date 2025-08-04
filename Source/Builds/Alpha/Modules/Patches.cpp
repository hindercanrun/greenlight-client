#include "../Std_Include.h"
#include "Patches.h"

#include "../Loader/Loader.h"
#include "Localized_Strings.h"

#include "../../../Utils/Hook.h"

namespace Patches
{
	Utils::Hook::Detour printf_Hook;
	void _printf(const char* fmt, ...)
	{
		char buf[256];

		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, sizeof(buf), fmt, args);
		va_end(args);

		Symbols::Com_Printf(0, "%s", buf);
	}

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
	void DebugReportProfileVars(Structs::ControllerIndex controllerIndex, const char* fmt, ...)
	{
		char buf[256];

		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, sizeof(buf), fmt, args);
		va_end(args);

		Symbols::Com_Printf(33, "%s\n", buf);

		auto Invoke = DebugReportProfileVars_Hook.Invoke<void(*)(Structs::ControllerIndex, const char*)>();
		Invoke(controllerIndex, fmt);
	}

	Utils::Hook::Detour Com_ExecStartupConfigs_Hook;
	void Com_ExecStartupConfigs(Structs::LocalClientNum localClientNum, const char* configFile)
	{
		static bool alreadyRanFunction = FALSE;
		if (alreadyRanFunction)
		{
			return;
		}
		alreadyRanFunction = TRUE;

		auto Invoke = Com_ExecStartupConfigs_Hook.Invoke<void(*)(Structs::LocalClientNum, const char*)>();
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
	char CL_ConsolePrint_AddLine(Structs::LocalClientNum localClientNum, int channel, const char* txt, int duration, int pixelWidth, char colour, int flags)
	{
#define TEXT_BUF_SIZE	4096
		static char txt2[TEXT_BUF_SIZE];
		FixTab(txt, txt2, sizeof(txt2));

		auto Invoke = CL_ConsolePrint_AddLine_Hook.Invoke<char(*)(Structs::LocalClientNum, int, const char*, int, int, char, int)>();
		return Invoke(localClientNum, channel, txt2, duration, pixelWidth, colour, flags);
	}

	Utils::Hook::Detour Com_OpenLogFile_Hook;
	void Com_OpenLogFile()
	{
		*Symbols::logfile = Symbols::FS_FOpenTextFileWrite("Alpha\\logs\\console.log");
	}

	Utils::Hook::Detour Live_Base_Pump_Hook;
	void Live_Base_Pump()
	{
		return;
	}

	Utils::Hook::Detour Live_Base_PumpForController_Hook;
	void Live_Base_PumpForController(Structs::ControllerIndex controllerIndex)
	{
		return;
	}

	Utils::Hook::Detour Assert_MyHandler_Hook;
	void Assert_MyHandler(const char* filename, int line, int type, const char* fmt, ...)
	{
		// TODO: Implement symbols printing. For that extra cool debug info

		static const char* lastFilename = nullptr;
		static int lastLine = -1;
		static int repeatCount = 0;

		// Check if this is the same assert location as last time
		if (lastFilename && strcmp(lastFilename, filename) == 0 && lastLine == line)
		{
			repeatCount++;
		}
		else
		{
			lastFilename = filename;
			lastLine = line;
			repeatCount = 1;
		}

		char message[1024];

		va_list va;
		va_start(va, fmt);
		_vsnprintf_s(message, sizeof(message), fmt, va);
		message[sizeof(message) - 1] = '\0';

		Symbols::Com_Printf(0, "\n");
		Symbols::Com_Printf(0, "****************************************\n");
		Symbols::Com_Printf(0, "*  Assertion Info:\n");
		Symbols::Com_Printf(0, "*  Message:       %s\n", message);
		Symbols::Com_Printf(0, "*  File:          %s\n", filename);
		Symbols::Com_Printf(0, "*  Line:          %d\n", line);
		Symbols::Com_Printf(0, "****************************************\n");

		if (repeatCount >= 2)
		{
			Symbols::Com_Printf(0, "*  Warning:       Same assert triggered twice.\n");
			Symbols::Com_Printf(0, "****************************************\n");
		}
		else
		{
			Symbols::Com_Printf(0, "*  Note:          First time this assert occurred.\n");
			Symbols::Com_Printf(0, "****************************************\n");
		}
	}

	void RegisterHooks()
	{
		printf_Hook.Create(printf, _printf); // This is for when we can't use Com_Printf and only printf

		FS_InitFilesystem_Hook.Create(0x825CB2C0, FS_InitFilesystem); // Print loaded modules

		Utils::Hook::SetValue(0x824CA848, 0x60000000); // Nop first Com_Printf in DebugReportProfileVars then call our hook.
		DebugReportProfileVars_Hook.Create(0x824CA828, DebugReportProfileVars); // Readd variadic formatting to fix formatting.

		Com_ExecStartupConfigs_Hook.Create(0x8251C860, Com_ExecStartupConfigs); // Make this only run once to prevent unneeded duplicate executing.

		CL_ConsolePrint_AddLine_Hook.Create(0x82328F00, CL_ConsolePrint_AddLine); // Fix \t showing as []

		Com_OpenLogFile_Hook.Create(0x8251E1D0, Com_OpenLogFile); // Replace with my own version

		Utils::Hook::SetValue(0x82341810, 0x60000000); // Nop LiveStorage_WaitOnStats to prevent delay on loading levels.

		// Disable online pumping
		Live_Base_Pump_Hook.Create(0x827B5120, Live_Base_Pump);
		Live_Base_PumpForController_Hook.Create(0x827B4E00, Live_Base_PumpForController);

		Assert_MyHandler_Hook.Create(0x825AE2C0, Assert_MyHandler); // My custom assertion handler.

		*(char*)0x8208F5A0 = '\0'; // Remove [timestamp][channel] string from log file.

		*(char*)0x8202BD74 = '\0'; // Remove 'Build 445 xenon' from the console window as it's not needed

		// Below are just your general string edits.
		// Example:
		// Utils::Hook::SetString(<address>, <string>); // <comment> (optional)

		Utils::Hook::SetString(0x821AD05C, "Creating Direct3D device...\n\n"); // Add a newline for cleaner logging
		Utils::Hook::SetString(0x821AD2A0, "Couldn't create Direct3D device: %s\n\n"); // Add a newline for cleaner logging + removed an 'a'
		Utils::Hook::SetString(0x820A07C8, "GUMP(%s): %s"); // Remove newline for cleaner logging
		Utils::Hook::SetString(0x820436A0, "DB allocation used %0.2f MB of memory\n");

		// Below are just your general dvar value edits.
		// Example:
		// Utils::Hook::SetValue(<address>, <value>); // <comment> (optional)

		Utils::Hook::SetValue(0x82A7ABB0, 0x38800000); // Force r_vsync to 0
		Utils::Hook::SetValue(0x8251B91C, 0x38800000); // Force com_maxfps to 0

		// Below are just your general localized string edits.
		// Example:
		// Localized_Strings::Override(string>, <string>); // <comment> (optional)

		// Change the offline MOTD to be our own one
		Localized_Strings::Override("TICKER_MESSAGE_CATEGORY_COD_CAPS", "Mod:");
		Localized_Strings::Override("TICKER_MESSAGE_DEFAULT_OFFLINE", "Alpha - Game Modification For Alpha!");
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