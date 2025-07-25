#include "../../Std_Include.h"
#include "Drawing.h"

#include "../../Utils/Hook.h"
#include "../../Utils/String.h"

namespace Drawing
{
	Structs::Font_s** SmallFont;

	void DrawFPS()
	{
		if (!*SmallFont)
			return;

		float fps_colour_good[4] = { 0.6f, 1.0f, 0.0f, 1.0f };
		float fps_colour_ok[4] = {1.0f, 0.7f, 0.3f, 1.0f};
		float fps_colour_bad[4] = {1.0f, 0.3f, 0.3f, 1.0f};
		float fps_colour_very_bad[4] = { 1.0f, 0.1f, 0.1f, 1.0f };

		int fps = (int)(float)(1000.0 / Symbols::cg_perfInfo->frame.average);
		const float* fps_colour = (fps >= 60)
			? fps_colour_good
			: (fps > 50)
			? fps_colour_ok
			: fps_colour_bad;

		const char* fps_string = Utils::String::Va("%i", fps);
		Symbols::R_AddCmdDrawText(fps_string, std::numeric_limits<int>::max(), *SmallFont, 1245.0f, 30.0f, 1.0f, 1.0f, 0.0f, fps_colour, 3);
	}

	void DrawWatermark()
	{
		if (!*SmallFont)
			return;

		const float colour[4] = { 1.0f, 1.0f, 1.0f, 0.25f };

		const char* text = "Modification by kittyist";
		Symbols::R_AddCmdDrawText(text, std::numeric_limits<int>::max(), *SmallFont, 2.0f, 700.0f, 0.8f, 0.8f, 0.0f, colour, 3);

		const char* date_string = Utils::String::Va("Date: %s", Utils::String::GetTimeAndDate());
		Symbols::R_AddCmdDrawText(date_string, std::numeric_limits<int>::max(), *SmallFont, 2.0f, 720.0f, 0.8f, 0.8f, 0.0f, colour, 3);
	}

	Utils::Hook::Detour CL_DrawScreen_Hook;
	void CL_DrawScreen(int localClientNum)
	{
		Dvars::register_dvars();
		if (Dvars::mod_drawFPS->current.enabled)
			DrawFPS();
		if (Dvars::mod_drawWatermark->current.enabled)
			DrawWatermark();

		auto Invoke = CL_DrawScreen_Hook.Invoke<void(*)(int)>();
		Invoke(localClientNum);
	}

	void RegisterHooks()
	{
		SmallFont = reinterpret_cast<Structs::Font_s**>(0x842CE7E8);

		CL_DrawScreen_Hook.Create(0x8232BA80, CL_DrawScreen);
	}

	void UnregisterHooks()
	{
		CL_DrawScreen_Hook.Clear();
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