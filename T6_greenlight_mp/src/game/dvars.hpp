#pragma once

namespace dvars
{
	// first declare the dvars

	// custom dvars
	extern const structs::dvar_t* mod_drawFPS;
	extern const structs::dvar_t* mod_drawWatermark;

	// game dvars
	// none for now

	// now lets register them
	void register_dvars();
}