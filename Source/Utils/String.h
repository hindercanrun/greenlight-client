#pragma once

#include <iostream>
#include <ctime>
#include <xtl.h>

namespace Utils
{
	namespace String
	{
		const char* Va(const char* fmt, ...);
		const char* GetTime();
	}
}