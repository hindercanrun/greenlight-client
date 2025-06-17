#include "../../std_include.hpp"
#include "debug.hpp"

#include "../../utils/hook.hpp"

namespace debug
{
	namespace
	{
		utils::hook::detour build_ff_hook;

		// basically a debug print to tell the user if the ff was built successfully (without any errors)
		// ffs: code_post_gfx_mp, common_mp and level
		void build_ff(const char* ff)
		{
			game::Com_Printf(14, "Building fast file '%s'.\n", ff);

			game::RawFile* rawfile = game::DB_FindXAssetHeader(game::ASSET_TYPE_RAWFILE, ff, true, -1).rawfile;
			if (rawfile->len)
			{
				game::Com_PrintError(1, "Failed to build fast file '%s'.\n", ff);
				game::Com_PrintError(1, "There were errors when building fast file '%s'.\n", ff);
				game::Com_PrintError(1, rawfile->buffer);
			}

			game::Com_Printf(14, "Successfully built fast file '%s'.\n", ff);
		}
	}

	void changes()
	{
		build_ff_hook.create(0x823FD588, build_ff);
	}
}