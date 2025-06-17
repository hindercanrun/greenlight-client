#include "..\std_include.hpp"
#include "game.hpp"

namespace game
{
	int Com_Compress(char* data_p)
	{
		if (!data_p)
			return false;
		
		char* in = data_p;
		char* out = data_p;

		bool new_line = false;
		bool white_space = false;
		
		while (*in)
		{
			char c = *in;

			// Skip "//" comments
			if (c == '/' && in[1] == '/')
			{
				while (*in && *in != '\n')
				{
					++in;
				}
			}
			// Skip "/* */" comments
			else if (c == '/' && in[1] == '*')
			{
				in += 2;
				while (*in && !(in[0] == '*' && in[1] == '/'))
				{
					++in;
				}

				if (*in)
					in += 2;
			}
			// Handle new lines
			else if (c == '\n' || c == '\r')
			{
				new_line = true;
				++in;
			}
			// Handle white space
			else if (c == ' ' || c == '\t')
			{
				white_space = true;
				++in;
			}
			// Copy actual content
			else
			{
				if (new_line)
				{
					*out++ = '\n';
					new_line = false;
					white_space = false;
				}
				else if (white_space)
				{
					*out++ = ' ';
					white_space = false;
				}

				// Handle quoted strings
				if (c == '"')
				{
					*out++ = *in++;
					while (*in && *in != '"')
					{
						*out++ = *in++;
					}

					if (*in == '"')
						*out++ = *in++;
				}
				else
				{
					*out++ = *in++;
				}
			}
		}

		*out = '\0';
		return static_cast<int>(out - data_p);
	}

	void* Z_MallocGarbage(int size, const char *name, int type)
	{
		char *buf = (char *)XMemAlloc(size + 164, 0);
		if (buf)
		{
			char *userBuf = buf + 164;
			//todo track_z_alloc(size + 72, name, type, userBuf, 0, 164); // tracking with adjusted pointer
			return userBuf;
		}
		return NULL;
	}

	void Z_Free(char *ptr, int type)
	{
		if (ptr)
		{
			//todo track_z_free(type, ptr, 164);
			XMemFree(ptr - 164, 0);
		}
	}
}