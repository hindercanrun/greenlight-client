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
			symbols::Com_Printf(14, "Building fast file '%s'.\n", ff);

			structs::RawFile* rawfile = symbols::DB_FindXAssetHeader(structs::ASSET_TYPE_RAWFILE, ff, true, -1).rawfile;
			if (rawfile->len)
			{
				symbols::Com_PrintError(1, "Failed to build fast file '%s'.\n", ff);
				symbols::Com_PrintError(1, "There were errors when building fast file '%s'.\n", ff);
				symbols::Com_PrintError(1, rawfile->buffer);
			}

			symbols::Com_Printf(14, "Successfully built fast file '%s'.\n", ff);
		}

		void register_hooks()
		{
			build_ff_hook.create(0x823FD588, build_ff);
		}
	}

	void load()
	{
		register_hooks();
	}
}