#include "../../Std_Include.h"
#include "Branding.h"

#include "../../Utils/Hook.h"
#include "../../Utils/String.h"

namespace Branding
{
	Utils::Hook::Detour Com_GetBuildVersion_Hook;
	const char* Com_GetBuildVersion()
	{
		const char* version = Utils::String::GetTimeAndDate();
		return version;
	}

	void RegisterHooks()
	{
		Com_GetBuildVersion_Hook.Create(0x824AF098, Com_GetBuildVersion);

		Utils::Hook::SetString(0x8207971C, "Call of Duty: Black Ops II - Release");
		Utils::Hook::SetString(0x8207CA7C, "%s%s build %s");
	}

	void UnregisterHooks()
	{
		Com_GetBuildVersion_Hook.Remove();
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