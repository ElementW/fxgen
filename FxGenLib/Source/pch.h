#ifndef PCH_H
#define PCH_H

#ifndef __GNUC__
#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif


// Standart Includes
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#ifdef __GNUC__
#include <sys/types.h>
#include <unistd.h>
#endif

#include "MemoryMgr.h"

#endif //PCH_H
