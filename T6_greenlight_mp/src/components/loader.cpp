#include "..\std_include.hpp"
#include "loader.hpp"

#include "modules\branding.hpp"
#include "modules\cmd.hpp"
#include "modules\debug.hpp"
#include "modules\dev_console.hpp"
#include "modules\dev_gui.hpp"
#include "modules\drawing.hpp"
#include "modules\localized_strings.hpp"
#include "modules\lui.hpp"
#include "modules\online.hpp"
#include "modules\patches.hpp"

namespace loader
{
	void load()
	{
		branding::changes();
		cmd::changes();
		debug::changes();
		dev_console::changes();
		dev_gui::changes();
		drawing::changes();
		localized_strings::changes();
		lui::changes();
		online::changes();
		patches::changes();
	}
}