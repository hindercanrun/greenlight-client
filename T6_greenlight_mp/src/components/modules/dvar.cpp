#include "../../std_include.hpp"
#include "dvar.hpp"

#include "../../utils/hook.hpp"

namespace dvar
{
	void register_bool(const char* name, bool value,
					   int flags, const char* desc)
	{
#ifdef _DEBUG
		game::Com_Printf(9, "registering bool dvar '%s'", name);
#endif

		game::Dvar_RegisterBool(name, value, flags, desc);
	}

	void register_int(const char* name, int value, int min_value,
					  int max_value, int flags, const char* desc)
	{
#ifdef _DEBUG
		game::Com_Printf(9, "registering int dvar '%s'", name);
#endif

		game::Dvar_RegisterInt(name, value, min_value, max_value, flags, desc);
	}

	void register_int64(const char* name, int64_t value, int64_t min_value,
						int64_t max_value, int flags, const char* desc)
	{
#ifdef DEBUG
		game::Com_Printf(9, "registering int64 dvar '%s'", name);
#endif
		game::Dvar_RegisterInt64(name, value, min_value, max_value, flags, desc);
	}

	void register_float(const char* name, float value, float min_value,
						float max_value, int flags, const char* desc)
	{
#ifdef DEBUG
		game::Com_Printf(9, "registering float dvar '%s'", name);
#endif
		game::Dvar_RegisterFloat(name, value, min_value, max_value, flags, desc);
	}

	void register_vec2(const char* name, int x, int y, float min_value,
					   float max_value, int flags, const char* desc)
	{
#ifdef DEBUG
		game::Com_Printf(9, "registering vec2 dvar '%s'", name);
#endif
		game::Dvar_RegisterVec2(name, x, y, min_value, max_value, flags, desc);
	}

	void register_vec3(const char* name, int x, int y, int z, float min_value,
					   float max_value, int flags, const char* desc)
	{
#ifdef DEBUG
		game::Com_Printf(9, "registering vec3 dvar '%s'", name);
#endif
		game::Dvar_RegisterVec3(name, x, y, z, min_value, max_value, flags, desc);
	}

	void register_vec4(const char* name, int x, int y, int z, int w, float min_value,
					   float max_value, int flags, const char* desc)
	{
#ifdef DEBUG
		game::Com_Printf(9, "registering vec4 dvar '%s'", name);
#endif
		game::Dvar_RegisterVec4(name, x, y, z, w, min_value, max_value, flags, desc);
	}

	void register_string(const char* name, const char* value,
						 int flags, const char* desc)
	{
#ifdef DEBUG
		game::Com_Printf(9, "registering string dvar '%s'", name);
#endif
		game::Dvar_RegisterString(name, value, flags, desc);
	}

	void register_enum(const char* name, const char** value,
					   int default_value, int flags, const char* desc)
	{
#ifdef DEBUG
		game::Com_Printf(9, "registering enum dvar '%s'", name);
#endif
		game::Dvar_RegisterEnum(name, value, default_value, flags, desc);
	}

	void register_colour(const char* name, float r, float g, float b,
						 float a, int flags, const char* desc)
	{
#ifdef DEBUG
		game::Com_Printf(9, "registering colour dvar '%s'", name);
#endif
		game::Dvar_RegisterColor(name, r, g, b, a, flags, desc);
	}

	void register_linear_rgb(const char* name, int x, int y, int z, float min_value,
							 float max_value, int flags, const char* desc)
	{
#ifdef DEBUG
		game::Com_Printf(9, "registering linear rgb dvar '%s'", name);
#endif
		game::Dvar_RegisterLinearRGB(name, x, y, z, min_value, max_value, flags, desc);
	}

	void register_colour_xyz(const char* name, int x, int y, int z, float min_value,
                             float max_value, int flags, const char* desc)
	{
#ifdef DEBUG
		game::Com_Printf(9, "registering colour xyz dvar '%s'", name);
#endif
		game::Dvar_RegisterColorXYZ(name, x, y, z, min_value, max_value, flags, desc);
	}
}