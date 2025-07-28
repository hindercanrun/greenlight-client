#include "../../Std_Include.h"
#include "Localized_Strings.h"

#include "../../Utils/Hook.h"
#include "../../Utils/String.h"

namespace Localized_Strings
{
	class SyncLock
	{
	public:
		SyncLock()
		{
			InitializeCriticalSection(&this->CritSec);
			this->Inited = TRUE;
		}

		~SyncLock()
		{
			if (this->Inited)
			{
				DeleteCriticalSection(&this->CritSec);
			}
		}

		void Lock()
		{
			EnterCriticalSection(&this->CritSec);
		}

		void Unlock()
		{
			LeaveCriticalSection(&this->CritSec);
		}

		CRITICAL_SECTION* Get()
		{
			return &this->CritSec;
		}

	private:
		CRITICAL_SECTION CritSec;
		bool Inited;
	};

	std::unordered_map<std::string, std::string>& getOverrides()
	{
		static std::unordered_map<std::string, std::string> Overrides;
		return Overrides;
	}

	SyncLock& getLock()
	{
		static SyncLock Lock;
		return Lock;
	}

	Utils::Hook::Detour SEH_StringEd_GetString_Hook;
	const char* SEH_StringEd_GetString(const char* pszReference)
	{
		SyncLock& Lock = getLock();
		Lock.Lock();

		auto& overrides = getOverrides();
		const auto entry = overrides.find(pszReference);
		if (entry != overrides.end())
		{
			Lock.Unlock();
			return Utils::String::Va("%s", entry->second.c_str());
		}

		Lock.Unlock();

		auto Invoke = SEH_StringEd_GetString_Hook.Invoke<const char*(*)(const char*)>();
		return Invoke(pszReference);
	}

	void Override(const char* Key, const char* Value)
	{
		SyncLock& Lock = getLock();
		Lock.Lock();
		getOverrides()[Key] = Value;
		Lock.Unlock();
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