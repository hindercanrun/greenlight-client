#pragma once

namespace Localized_Strings
{
	void Override(const char* Key, const char* Value);

	void Load();
	void Unload();
}