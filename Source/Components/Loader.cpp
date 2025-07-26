#include "../Std_Include.h"
#include "Loader.h"

#include "Modules\Branding.h"
#include "Modules\DevGui.h"
#include "Modules\Drawing.h"
#include "Modules\LUI.h"

namespace Loader
{
	void Load()
	{
		Branding::Load();
		DevGui::Load();
		Drawing::Load();
		LUI::Load();
	}

	void Unload()
	{
		Branding::Unload();
		DevGui::Unload();
		Drawing::Unload();
		LUI::Unload();
	}
}