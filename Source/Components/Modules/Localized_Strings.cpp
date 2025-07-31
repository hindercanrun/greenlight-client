#include "../Std_Include.h"
#include "Localized_Strings.h"

#include "../Utils/Hook.h"
#include "../Utils/String.h"

namespace Localized_Strings
{
	std::unordered_map<std::string, std::string>& getOverrides()
	{
		static std::unordered_map<std::string, std::string> overrides;
		return overrides;
	}

	Utils::Hook::Detour SEH_StringEd_GetString_Hook;
	const char* SEH_StringEd_GetString(const char* pszReference)
	{
		auto& overrides = getOverrides();
		const auto entry = overrides.find(pszReference);
		if (entry != overrides.end())
		{
			return Utils::String::Va("%s", entry->second.data());
		}

		auto Invoke = SEH_StringEd_GetString_Hook.Invoke<const char*(*)(const char*)>();
		return Invoke(pszReference);
	}

	void Override(const char* key, const char* value)
	{
		getOverrides()[key] = value;
	}

	void RegisterHooks()
	{
		SEH_StringEd_GetString_Hook.Create(0x825130F0, SEH_StringEd_GetString);
	}

	void UnregisterHooks()
	{
		SEH_StringEd_GetString_Hook.Remove();
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