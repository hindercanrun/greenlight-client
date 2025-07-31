#include "../Std_Include.h"
#include "String.h"

namespace Utils
{
	namespace String
	{
		const char* Va(const char* fmt, ...)
		{
			static char buf[32000];

			va_list args;
			va_start(args, fmt);
			vsprintf_s(buf, sizeof(buf), fmt, args);
			va_end(args);

			return buf;
		}

		const char* GetTime()
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