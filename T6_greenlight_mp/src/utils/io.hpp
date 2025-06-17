#pragma once

namespace utils
{
	namespace io
	{
		bool is_correct_exe(std::uint32_t title_id);

		bool create_directory_recursive(const std::string& path);
	}
}