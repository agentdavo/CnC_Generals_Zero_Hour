/*
**	Command & Conquer Generals(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// This file contains all the header files that shouldn't change frequently.
// Be careful what you stick in here, because putting files that change often in here will
// tend to cheese people's goats.

#ifndef __PRERTS_H__
#define __PRERTS_H__

//-----------------------------------------------------------------------------
// srj sez: this must come first, first, first.
#define _STLP_USE_NEWALLOC 1
// #define _STLP_USE_CUSTOM_NEWALLOC		STLSpecialAlloc
class STLSpecialAlloc;

// We actually don't use Windows for much other than timeGetTime, but it was included in 40
// different .cpp files, so I bit the bullet and included it here.
// PLEASE DO NOT ABUSE WINDOWS OR IT WILL BE REMOVED ENTIRELY. :-)
//--------------------------------------------------------------------------------- System Includes
#include <cassert>
#include <cctype>
#include <cfloat>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include "common/windows.h"
#else
#include "common/win32_compat.h"
#endif

//------------------------------------------------------------------------------------ STL Includes
// srj sez: no, include STLTypesdefs below, instead, thanks
// #include <algorithm>
// #include <bitset>
// #include <hash_map>
// #include <list>
// #include <map>
// #include <queue>
// #include <set>
// #include <stack>
// #include <string>
// #include <vector>

//------------------------------------------------------------------------------------ RTS Includes
// Icky. These have to be in this order.
#include "lib/base_type.h"
#include "game_engine/common/STLTypedefs.h"
#include "game_engine/common/Errors.h"
#include "game_engine/common/Debug.h"
#include "game_engine/common/ascii_string.h"
#include "game_engine/common/subsystem_interface.h"

#include "game_engine/common/GameCommon.h"
#include "game_engine/common/gamememory.h"
#include "game_engine/common/GameType.h"
#include "game_engine/common/GlobalData.h"

// You might not want Kindof in here because it seems like it changes frequently, but the problem
// is that Kindof is included EVERYWHERE, so it might as well be precompiled.
#include "game_engine/common/INI.h"
#include "game_engine/common/KindOf.h"
#include "game_engine/common/DisabledTypes.h"
#include "game_engine/common/NameKeyGenerator.h"
#include "game_engine/game_client/clientRandomValue.h"
#include "game_engine/game_logic/logicRandomValue.h"

#include "game_engine/common/thing.h"
#include "game_engine/common/unicodestring.h"

#endif /* __PRERTS_H__ */
