#include "../Std_Include.h"
#include "UI.h"

#include "../Utils/Hook.h"

namespace UI
{
	Utils::Hook::Detour UI_FeederItemText_GetUserName_Hook;
	const char* UI_FeederItemText_GetUserName(int userIndex)
	{
		static char name[XUSER_NAME_SIZE] = {};

		DWORD result = XUserGetName(0, name, XUSER_NAME_SIZE);
		if (result == ERROR_SUCCESS)
		{
			return name;
		}
		return "Unknown Solider";
	}

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
		// 'Fix' the feeder to properly display the players name instead of 'Player 1'
		UI_FeederItemText_GetUserName_Hook.Create(0x82519818, UI_FeederItemText_GetUserName);

		// Add some custom prints.
		LUI_CoD_Init_Hook.Create(0x828BDA08, LUI_CoD_Init);
		LUI_Shutdown_Hook.Create(0x828B7200, LUI_Shutdown);

		// Restore stripped logging functionality to Kore VM.
		koreDefaultLogger_Hook.Create(0x82854208, koreDefaultLogger);
	}

	void UnregisterHooks()
	{
		UI_FeederItemText_GetUserName_Hook.Remove();
		LUI_CoD_Init_Hook.Remove();
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