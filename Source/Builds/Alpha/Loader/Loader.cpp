#include "../Std_Include.h"
#include "Loader.h"

#include "../Modules/Assets.h"
#include "../Modules/DevGui.h"
#include "../Modules/Drawing.h"
#include "../Modules/Localized_Strings.h"
#include "../Modules/Patches.h"
#include "../Modules/UI.h"

namespace Loader
{
	Module g_modules[MAX_MODULES];
	int g_moduleCount = 0;

	void RegisterModule(const char* name, void (*load)(), void (*unload)())
	{
		// Check for duplicates
		for (int i = 0; i < g_moduleCount; ++i)
		{
			if (strcmp(g_modules[i].name, name) == 0)
			{
				return; // Already registered
			}
		}

		if (g_moduleCount < MAX_MODULES)
		{
			g_modules[g_moduleCount].name = name;
			g_modules[g_moduleCount].load = load;
			g_modules[g_moduleCount].unload = unload;
			g_moduleCount++;
		}
	}

	void LoadAllModules()
	{
		for (int i = 0; i < g_moduleCount; ++i)
		{
			if (g_modules[i].load)
			{
				g_modules[i].load();
			}
		}
	}

	void UnloadAllModules()
	{
		for (int i = g_moduleCount - 1; i >= 0; --i)
		{
			if (g_modules[i].unload)
			{
				g_modules[i].unload();
			}
		}
	}

	void Load()
	{
		RegisterModule("Assets",				Assets::Load,				Assets::Unload);
		RegisterModule("DevGui",				DevGui::Load,				DevGui::Unload);
		RegisterModule("Drawing",				Drawing::Load,				Drawing::Unload);
		RegisterModule("Localized Strings",		Localized_Strings::Load,	Localized_Strings::Unload);
		RegisterModule("Patches",				Patches::Load,				Patches::Unload);
		RegisterModule("UI",					UI::Load,					UI::Unload);

		LoadAllModules();
	}

	void Unload()
	{
		UnloadAllModules();
		g_moduleCount = 0;
	}
}