#include "../../Std_Include.h"
#include "LUI.h"

#include "../../Utils\Hook.h"

namespace LUI
{
	Utils::Hook::Detour LUI_CoD_Init_Hook;
	void LUI_CoD_Init(bool frontend)
	{
		Symbols::Com_Printf(15, "LUI: Starting up...\n");

		auto Invoke = LUI_CoD_Init_Hook.Invoke<void(*)(bool)>();
		Invoke(frontend);
	}

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
		LUI_CoD_Init_Hook.Create(0x828BDA08, LUI_CoD_Init);
		LUI_Shutdown_Hook.Create(0x828B7200, LUI_Shutdown);

		// Restore stripped logging functionality to Kore VM.
		koreDefaultLogger_Hook.Create(0x82854208, koreDefaultLogger);
	}

	void UnregisterHooks()
	{
		LUI_CoD_Init_Hook.Clear();
		LUI_Shutdown_Hook.Clear();
		koreDefaultLogger_Hook.Clear();
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