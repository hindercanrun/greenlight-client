#include "../std_include.hpp"
#include "cryptography.hpp"

namespace utils
{
	namespace cryptography
	{
		unsigned int one_at_a_time::compute(const std::string& data)
		{
			return compute(data.data(), data.size());
		}

		unsigned int one_at_a_time::compute(const char* key, const size_t len)
		{
			unsigned int hash, i;
			for (hash = i = 0; i < len; ++i)
			{
				hash += key[i];
				hash += (hash << 10);
				hash ^= (hash >> 6);
			}

			hash += (hash << 3);
			hash ^= (hash >> 11);
			hash += (hash << 15);

			return hash;
		}
	}
}