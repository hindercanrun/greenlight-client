#pragma once

namespace Dvars
{
	// First declare the Dvars.

	// Custom dvars
	extern const Structs::dvar_t* mod_drawFPS;
	extern const Structs::dvar_t* mod_drawWatermark;

	// Game dvars
	// None for now.

	// Now lets register them.
	void register_dvars();
}