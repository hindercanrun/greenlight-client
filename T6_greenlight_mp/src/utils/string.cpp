#include "..\std_include.hpp"
#include "hook.hpp"

namespace utils
{
	namespace string
	{
		//13/05/2025: TODO: clean this up.
		const char* va(const char *format, ...)
		{
			char string[2][32000]; // in case it's called by nested functions
			auto i = 0;

			char* buf = string[i];
			i = 1 - i;

			va_list args;
			va_start(args, format);
			vsprintf_s(buf, sizeof(string[0]), format, args);
			va_end(args);

			return buf;
		}

		bool starts_with(const std::string& text, const std::string& substring)
		{
			return text.find(substring) == 0;
		}

		bool ends_with(const std::string& text, const std::string& substring)
		{
			if (substring.size() > text.size())
				return false;

			return std::equal(substring.rbegin(), substring.rend(), text.rbegin());
		}

		const char* get_time_and_date()
		{
			static char buf[32];

			// get current time
			std::time_t now = std::time(nullptr);
			now += 8 * 3600;

			std::tm* time = std::gmtime(&now); // your system time

			std::strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Y", time); // DD MM HH:MM:SS YYYY
			return buf;
		}
	}
}