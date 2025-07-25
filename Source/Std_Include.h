#pragma once

#ifndef IS_XENIA
#define IS_XENIA TRUE // Temp value for now.
#endif

#define TITLE_ID 0x415608C3 // New Moon

// This makes 'std::numeric_limits<int>::max()' work.
#define NOMINMAX

#define DCBF(ptr)		__dcbf(0, ptr)
#define DCBST(ptr)		__dcbst(0, ptr)
#define SYNC()			__sync()

#include <cstdint>
#include <iostream>
#include <ctime>

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
