#include "../../Std_Include.h"
#include "Branding.h"

#include "../../Utils/Hook.h"
#include "../../Utils/String.h"

namespace Branding
{
	void RegisterHooks()
	{
		Utils::Hook::SetValue(0x82249CC8, 0x4E800020); // 'Nop' CG_DrawVersion so it doesn't show
		Utils::Hook::SetValue(0x825390D8, 0x4E800020); // 'Nop' UI_DrawBuildNumber so it doesn't show
	}

	void UnregisterHooks()
	{
		// Nothing to unregister yet
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