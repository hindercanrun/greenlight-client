#include "std_include.hpp"
#include "components\loader.hpp"

#include "utils\io.hpp"

BOOL APIENTRY DllMain(
	HANDLE handle,
	DWORD reason,
	LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH) 
	{
		// check if the title is BO2
		if (!utils::io::is_correct_exe(XamGetCurrentTitleId()))
			return FALSE;

		// load our components
		loader::load();
	}

	// 18/04/2025: Xenia doesn't have a dashboard system, so we can't use DLL_PROCESS_DETACH

	return TRUE;
}