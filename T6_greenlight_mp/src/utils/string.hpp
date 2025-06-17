#pragma once

namespace utils
{
	namespace string
	{
		const char* va(const char *format, ...);

		bool starts_with(const std::string& text, const std::string& substring);
		bool ends_with(const std::string& text, const std::string& substring);

		const char* get_time_and_date();
	}
}