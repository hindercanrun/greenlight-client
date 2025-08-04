#pragma once

#define TITLE_ID 0x415608C3 // Black Ops 2

// This makes 'std::numeric_limits<int>::max()' work.
#define NOMINMAX

// Just some macros to make readability better.
#define MAX_CHARS		std::numeric_limits<int>::max()

#define DCBF(ptr)		__dcbf(0, ptr)
#define DCBST(ptr)		__dcbst(0, ptr)
#define SYNC()			__sync()

#include <cstdint>
#include <iostream>
#include <ctime>
#include <string>
#include <unordered_map>
#include <direct.h>
#include <fstream>
#include <algorithm>
#include <cstddef>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <functional>
#include <cstring>
#include <sstream>
#include <io.h>

#include <xtl.h>
#include <stdint.h>

// Standard library includes
#include <cstddef>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <cstring>
#include <direct.h>
#include <fstream>
#include <sstream>
#include <xtl.h>
#include <xbox.h>
#include <xgraphics.h>
#include <io.h>
#include <xjson.h>

#include "Game\Assets.h"
#include "Game\Structs.h"
#include "Game\Symbols.h"

extern "C"
{
	NTSYSAPI DWORD NTAPI XamGetCurrentTitleId(VOID);
}
