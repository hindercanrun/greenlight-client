#include "../Std_Include.h"
#include "UI.h"

#include "../../../Utils/Hook.h"

namespace UI
{
	Utils::Hook::Detour LUI_Shutdown_Hook;
	void LUI_Shutdown()
	{
		Symbols::Com_Printf(15, "LUI: Shutting down...\n");

		auto Invoke = LUI_Shutdown_Hook.Invoke<void(*)()>();
		Invoke();
	}

	Utils::Hook::Detour koreDefaultLogger_Hook;
	void koreDefaultLogger(struct lua_State* unkown, const char* msg, ...)
	{
		Symbols::Com_Printf(15, msg);
	}

	void RegisterHooks()
	{
		// Add some custom prints.
		LUI_Shutdown_Hook.Create(0x82910650, LUI_Shutdown);

		// Restore stripped logging functionality to Kore VM.
		koreDefaultLogger_Hook.Create(0x828B31A8, koreDefaultLogger);
	}

	void UnregisterHooks()
	{
		LUI_Shutdown_Hook.Remove();
		koreDefaultLogger_Hook.Remove();
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