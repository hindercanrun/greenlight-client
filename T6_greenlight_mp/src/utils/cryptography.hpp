#pragma once

#include <string>

namespace utils
{
	namespace cryptography
	{
		namespace one_at_a_time
		{
			unsigned int compute(const std::string& data);
			unsigned int compute(const char* key, size_t len);
		};
	}
}