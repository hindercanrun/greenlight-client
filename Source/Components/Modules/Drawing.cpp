#include "../../Std_Include.h"
#include "Drawing.h"

#include "../../Utils/Hook.h"
#include "../../Utils/String.h"

namespace Drawing
{
	Structs::Font_s** Font;

	void DrawModFPS()
	{
		if (!*Font)
		{
			return;
		}

		float ColourGood[4] = { 0.6f, 1.0f, 0.0f, 1.0f };
		float ColourOkay[4] = { 1.0f, 0.7f, 0.3f, 1.0f };
		float ColourBad[4]  = { 1.0f, 0.3f, 0.3f, 1.0f };

		int Count = (int)(1000.0 / Symbols::cg_perfInfo->frame.average);
		float* TextColour = (Count >= 60)
			? ColourGood
			: (Count > 50)
			? ColourOkay
			: ColourBad;

		const char* TextString = Utils::String::Va("%i", Count); // Example: '60'
		Symbols::R_AddCmdDrawText(TextString, MAX_CHARS, *Font, 1245.0f, 30.0f, 1.0f, 1.0f, 0.0f, TextColour, 3);
	}

	void DrawModWatermark()
	{
		if (!*Font)
		{
			return;
		}

		float TextColour[4] = { 1.0f, 1.0f, 1.0f, 0.25f };

		const char* BrandingString = "Redlight";
		Symbols::R_AddCmdDrawText(BrandingString, MAX_CHARS, *Font, 2.0f, 700.0f, 0.8f, 0.8f, 0.0f, TextColour, 3);

		const char* TimeString = Utils::String::Va("Time: %s", Utils::String::GetTime());
		Symbols::R_AddCmdDrawText(TimeString, MAX_CHARS, *Font, 2.0f, 720.0f, 0.8f, 0.8f, 0.0f, TextColour, 3);
	}

	Utils::Hook::Detour CL_DrawScreen_Hook;
	void CL_DrawScreen(int localClientNum)
	{
		Dvars::register_dvars();
		if (Dvars::mod_drawFPS->current.enabled)
		{
			DrawModFPS();
		}

		if (Dvars::mod_drawWatermark->current.enabled)
		{
			DrawModWatermark();
		}

		auto Invoke = CL_DrawScreen_Hook.Invoke<void(*)(int)>();
		Invoke(localClientNum);
	}

	void RegisterHooks()
	{
		Font = (Structs::Font_s**)0x842CE7E8; // fonts/720/smallfont

		CL_DrawScreen_Hook.Create(0x8232BA80, CL_DrawScreen);

		Utils::Hook::SetValue(0x82249CC8, 0x4E800020); // 'Nop' CG_DrawVersion so it doesn't show
		Utils::Hook::SetValue(0x825390D8, 0x4E800020); // 'Nop' UI_DrawBuildNumber so it doesn't show
	}

	void UnregisterHooks()
	{
		CL_DrawScreen_Hook.Remove();
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