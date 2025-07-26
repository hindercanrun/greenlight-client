#include "../../Std_Include.h"
#include "Patches.h"

#include "../../Utils\Hook.h"

namespace Patches
{
	void RegisterHooks()
	{
		*(char*)0x8207C3B8 = '\0'; // Remove [timestamp][channel] string from log file.

		*(char*)0x8207A8DC = '\0'; // Remove 'exe name' from the log file
		*(char*)0x8207A8BC = '\0'; // Remove unneeded build info + timestamp from the log file

		// Redirect the console log file
		Utils::Hook::SetString(0x8207A8A4, "Mod/Logs/Console.log");
	}

	void UnregisterHooks()
	{
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