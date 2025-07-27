#include "../Std_Include.h"
#include "Dvars.h"

namespace Dvars
{
	// First declare the DVars.

	// Custom DVars
	const Structs::dvar_t* mod_drawFPS = nullptr;
	const Structs::dvar_t* mod_drawWatermark = nullptr;

	// Now lets register them.
	void register_dvars()
	{
		mod_drawFPS = Symbols::Dvar_RegisterBool(
			"mod_drawFPS",
			true,
			Structs::DVAR_ARCHIVE,
			"Draw the FPS counter");
		mod_drawWatermark = Symbols::Dvar_RegisterBool(
			"mod_drawWatermark",
			true,
			Structs::DVAR_ARCHIVE,
			"Draw the watermark");
	}
}