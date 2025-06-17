#pragma once

namespace dvar
{
	void register_bool(const char* name, bool value,
					   int flags, const char* desc);

	void register_int(const char* name, int value, int min_value,
					  int max_value, int flags, const char* desc);

	void register_int64(const char* name, int64_t value, int64_t min_value,
						int64_t max_value, int flags, const char* desc);

	void register_float(const char* name, float value, float min_value,
						float max_value, int flags, const char* desc);

	void register_vec2(const char* name, int x, int y, float min_value,
					   float max_value, int flags, const char* desc);

	void register_vec3(const char* name, int x, int y, int z, float min_value,
					   float max_value, int flags, const char* desc);

	void register_vec4(const char* name, int x, int y, int z, int w, float min_value,
					   float max_value, int flags, const char* desc);

	void register_string(const char* name, const char* value,
						 int flags, const char* desc);

	void register_enum(const char* name, const char** value,
					   int default_value, int flags, const char* desc);

	void register_colour(const char* name, float r, float g, float b,
						 float a, int flags, const char* desc);

	void register_linear_rgb(const char* name, int x, int y, int z, float min_value,
							 float max_value, int flags, const char* desc);

	void register_colour_xyz(const char* name, int x, int y, int z, float min_value,
							 float max_value, int flags, const char* desc);
}