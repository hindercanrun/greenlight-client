#pragma once

#define TITLE_ID 0x415608C3 // New Moon

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

#include <xtl.h>
#include <stdint.h>

#include "Game\Structures\Assets.h"
#include "Game\Structures\Structs.h"
#include "Game\Dvars.h"
#include "Game\Symbols.h"

extern "C"
{
	NTSYSAPI DWORD NTAPI XamGetCurrentTitleId(VOID);
}
