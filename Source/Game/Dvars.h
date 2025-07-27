#pragma once

namespace Dvars
{
	// First declare the DVars.

	// Custom DVars
	extern const Structs::dvar_t* mod_drawFPS;
	extern const Structs::dvar_t* mod_drawWatermark;

	// Now lets register them.
	void register_dvars();
}