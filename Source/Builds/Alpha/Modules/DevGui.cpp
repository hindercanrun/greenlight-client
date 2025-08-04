#include "../Std_Include.h"
#include "DevGui.h"

#include "../../../Utils/Hook.h"

namespace DevGui
{
	Utils::Hook::Detour DevGui_GetScreenXPad_Hook;
	int DevGui_GetScreenXPad()
	{
		return 0;
	}

	Utils::Hook::Detour DevGui_GetScreenYPad_Hook;
	int DevGui_GetScreenYPad()
	{
		return 0;
	}

	void RegisterHooks()
	{
		// Match the DevGui safe area on PC!
		DevGui_GetScreenXPad_Hook.Create(0x823B2C70, DevGui_GetScreenXPad);
		DevGui_GetScreenYPad_Hook.Create(0x823B2CC8, DevGui_GetScreenYPad);
	}

	void UnregisterHooks()
	{
		DevGui_GetScreenXPad_Hook.Remove();
		DevGui_GetScreenYPad_Hook.Remove();
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