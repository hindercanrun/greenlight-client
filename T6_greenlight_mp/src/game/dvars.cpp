#include "../std_include.hpp"
#include "dvars.hpp"

namespace dvars
{
	// first declare the dvars

	// custom dvars
	const structs::dvar_t* mod_drawFPS = nullptr;
	const structs::dvar_t* mod_drawWatermark = nullptr;

	// game dvars
	// none for now

	// now lets register them
	void register_dvars()
	{
		mod_drawFPS = symbols::Dvar_RegisterBool(
			"mod_drawFPS",
			true,
			structs::DVAR_ARCHIVE,
			"Draw the FPS counter");
		mod_drawWatermark = symbols::Dvar_RegisterBool(
			"mod_drawWatermark",
			true,
			structs::DVAR_ARCHIVE,
			"Draw the watermark");
	}
}