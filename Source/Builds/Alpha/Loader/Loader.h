#pragma once

namespace Loader
{
	typedef struct Module
	{
		const char* name;
		void (*load)();
		void (*unload)();
	} Module;

#define MAX_MODULES 32
	extern Module g_modules[MAX_MODULES];
	extern int g_moduleCount;

	void Load();
	void Unload();
}