#include "..\std_include.hpp"
#include "loader.hpp"

// important modules
#include "modules\assertion.hpp"

// regular modules
#include "modules\branding.hpp"
#include "modules\cmd.hpp"
#include "modules\debug.hpp"
#include "modules\dev_gui.hpp"
#include "modules\drawing.hpp"
#include "modules\localized_strings.hpp"
#include "modules\lui.hpp"
#include "modules\patches.hpp"

namespace loader
{
	void load()
	{
		// TODO: maybe there's a better spot for this?
		//dvars::register_dvars();

		assertion::load();

		branding::load();
		cmd::load();
		debug::load();
		dev_gui::load();
		drawing::load();
		localized_strings::load();
		lui::load();
		patches::load();
	}
}