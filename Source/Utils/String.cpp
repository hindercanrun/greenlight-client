#include "../Std_Include.h"
#include "String.h"

namespace Utils
{
	namespace String
	{
		const char* Va(const char* fmt, ...)
		{
			char string[2][32000]; // in case it's called by nested functions
			auto i = 0;

			char* buf = string[i];
			i = 1 - i;

			va_list args;
			va_start(args, fmt);
			vsprintf_s(buf, sizeof(string[0]), fmt, args);
			va_end(args);

			return buf;
		}

		const char* GetTimeAndDate()
		{
			static char buf[16];

			time_t now = time(NULL);
			now += 8 * 3600; // Adjust to UTC+8 if needed

			struct tm* timeinfo = localtime(&now);

			strftime(buf, sizeof(buf), "%I:%M:%S %p", timeinfo);
			return buf;
		}
	}
}