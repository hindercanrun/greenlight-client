#include "..\..\std_include.hpp"
#include "branding.hpp"

#include "..\..\utils\hook.hpp"
#include "..\..\utils\string.hpp"

namespace branding
{
	namespace
	{
		game::Font_s** small_font;

		utils::hook::detour set_version_hook;
		const char* set_version()
		{
			static std::string version = std::string("0 ") + utils::string::get_time_and_date();
			return version.c_str();
		}
	}

	void changes()
	{
		small_font = reinterpret_cast<game::Font_s**>(0x842CE7E8);

		set_version_hook.create(0x824AF098, set_version); // Com_GetBuildVersion

		// version string
		utils::hook::set_string(0x8207971C, "Call of Duty: Black Ops II - Release");
		utils::hook::set_string(0x8207CA7C, "%s%s build %s");
	}
}
