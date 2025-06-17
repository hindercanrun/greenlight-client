#include "..\..\std_include.hpp"
#include "localized_strings.hpp"

#include "..\..\utils\hook.hpp"
#include "..\..\utils\string.hpp"

namespace localized_strings
{
	namespace
	{
		std::unordered_map<std::string, std::string>& get_localized_overrides()
		{
			static std::unordered_map<std::string, std::string> overrides;
			return overrides;
		}

		struct sync_lock
		{
			CRITICAL_SECTION crit_sec;
			bool inited;

			sync_lock() : inited(false)
			{
				InitializeCriticalSection(&crit_sec);
				inited = true;
			}

			~sync_lock()
			{
				if (inited)
					DeleteCriticalSection(&cs);
			}
		};

		sync_lock& get_sync_lock()
		{
			static sync_lock lock;
			return lock;
		}

		utils::hook::detour get_string_hook;
		const char* get_string(const char* reference)
		{
			auto& lock = get_sync_lock();
			EnterCriticalSection(&lock.crit_sec);

			auto& overrides = get_localized_overrides();
			const auto entry = overrides.find(reference);
			if (entry != overrides.end())
			{
				LeaveCriticalSection(&lock.crit_sec);
				
				const char* result = utils::string::va("%s", entry->second.data());
				return result;
			}

			LeaveCriticalSection(&lock.crit_sec);

			auto invoke = get_string_hook.invoke<const char*(*)(const char*)>();
			return invoke(reference);
		}
	}

	void override_string(const std::string& key, const std::string& value)
	{
		auto& lock = get_sync_lock();
		EnterCriticalSection(&lock.crit_sec);

		get_localized_overrides()[key] = value;

		LeaveCriticalSection(&lock.crit_sec);
	}

	void changes()
	{
		// allow localized strings to be modified by plugin
		get_string_hook.create(0x825130F0, get_string); // SEH_StringEd_GetString

		//test run
		override_string("MENU_MATCHMAKING_CAPS", "Custom Localized Strings!!");
	}
}