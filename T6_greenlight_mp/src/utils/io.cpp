#include "..\std_include.hpp"
#include "io.hpp"

namespace utils
{
	namespace io
	{
		bool is_correct_exe(std::uint32_t title_id)
		{
			//check if the title is t6
			if (title_id != TITLE_ID) // 415608C3
			{
				symbols::Com_PrintError(1,
					"incorrect title id. current title id: %X, expecting: %X",
					title_id,
					TITLE_ID);
				return false;
			}

			// TODO: maybe implement checking for the executable name

			return true;
		}
	}
}