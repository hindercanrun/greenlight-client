#include "../../Std_Include.h"
#include "DevGui.h"

#include "../../Utils\Hook.h"

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

	Utils::Hook::Detour CL_DevGuiFrame_Hook;
	void CL_DevGuiFrame(int localClientNum)
	{
		static bool hasRanBefore = FALSE;
		if (hasRanBefore)
		{
			return;
		}
		hasRanBefore = TRUE;
		Symbols::Cbuf_InsertText(0, "exec_addtext Mod/Configs/DevGui");
		Symbols::Cbuf_InsertText(0, "exec_addtext Mod/Configs/DevGui_Mp");
	}

	Utils::Hook::Detour Com_LoadLevelFastFiles_Hook;
	void Com_LoadLevelFastFiles(const char* name)
	{
		Symbols::Cbuf_InsertText(0, "exec_addtext Mod/Configs/DevGui");
		Symbols::Cbuf_InsertText(0, "exec_addtext Mod/Configs/DevGui_Mp");

		auto Invoke = Com_LoadLevelFastFiles_Hook.Invoke<void(*)(const char*)>();
		Invoke(name);
	}

	Utils::Hook::Detour Com_LoadUiFastFile_Hook;
	void Com_LoadUiFastFile()
	{
		Symbols::Cbuf_InsertText(0, "exec_addtext Mod/Configs/DevGui");
		Symbols::Cbuf_InsertText(0, "exec_addtext Mod/Configs/DevGui_Mp");

		auto Invoke = Com_LoadUiFastFile_Hook.Invoke<void(*)()>();
		Invoke();
	}

	void RegisterHooks()
	{
		// Match the Dev Gui safe area on PC!
		DevGui_GetScreenXPad_Hook.Create(0x8238B520, DevGui_GetScreenXPad);
		DevGui_GetScreenYPad_Hook.Create(0x8238B588, DevGui_GetScreenYPad);

		// Add our custom Dev Gui.
		//CL_DevGuiFrame_Hook.Create(0x82307620, CL_DevGuiFrame);
		//Com_LoadLevelFastFiles_Hook.Create(0x824BCCF8, Com_LoadLevelFastFiles);
		//Com_LoadUiFastFile_Hook.Create(0x824BCC00, Com_LoadUiFastFile);
	}

	void UnregisterHooks()
	{
		DevGui_GetScreenXPad_Hook.Remove();
		DevGui_GetScreenYPad_Hook.Remove();
		CL_DevGuiFrame_Hook.Remove();
		Com_LoadLevelFastFiles_Hook.Remove();
		Com_LoadUiFastFile_Hook.Remove();
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