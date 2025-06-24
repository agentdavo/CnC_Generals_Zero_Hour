/*
**	Command & Conquer Generals Zero Hour(tm)
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
#define _STLP_USE_NEWALLOC					1
//#define _STLP_USE_CUSTOM_NEWALLOC		STLSpecialAlloc
class STLSpecialAlloc;


// We actually don't use Windows for much other than timeGetTime, but it was included in 40 
// different .cpp files, so I bit the bullet and included it here.
// PLEASE DO NOT ABUSE WINDOWS OR IT WILL BE REMOVED ENTIRELY. :-)
//--------------------------------------------------------------------------------- System Includes 
#define WIN32_LEAN_AND_MEAN
#include <atlbase.h>
#include "common/windows.h"

#include <assert.h>
#include <ctype.h>
#include <direct.h>
#include <EXCPT.H>
#include <float.h>
#include <fstream.h>
#include <imagehlp.h>
#include <io.h>
#include <limits.h>
#include <lmcons.h>
#include <mapicode.h>
#include <math.h>
#include <memory.h>
#include <mmsystem.h>
#include <objbase.h>
#include <ocidl.h>
#include <process.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shlguid.h>
#include <snmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <TCHAR.H>
#include <time.h>
#include <vfw.h>
#include <winerror.h>
#include <wininet.h>
#include <winreg.h>

#ifndef DIRECTINPUT_VERSION
#	define DIRECTINPUT_VERSION	0x800
#endif

#include <dinput.h>

//------------------------------------------------------------------------------------ STL Includes
// srj sez: no, include STLTypesdefs below, instead, thanks
//#include <algorithm>
//#include <bitset>
//#include <hash_map>
//#include <list>
//#include <map>
//#include <queue>
//#include <set>
//#include <stack>
//#include <string>
//#include <vector>

//------------------------------------------------------------------------------------ RTS Includes
// Icky. These have to be in this order.
#include "lib/base_type.h"
#include "common/stl_type_defs.h"
#include "common/Errors.h"
#include "common/Debug.h"
#include "common/AsciiString.h"
#include "game_engine/common/subsystem_interface.h"

#include "common/GameCommon.h"
#include "common/GameMemory.h"
#include "common/GameType.h"
#include "common/GlobalData.h"

// You might not want Kindof in here because it seems like it changes frequently, but the problem
// is that Kindof is included EVERYWHERE, so it might as well be precompiled.
#include "common/ini.h"
#include "common/KindOf.h"
#include "common/DisabledTypes.h"
#include "common/NameKeyGenerator.h"
#include "game_client/ClientRandomValue.h"
#include "GameLogic/LogicRandomValue.h"
#include "common/ObjectStatusTypes.h"

#include "common/Thing.h"
#include "common/UnicodeString.h"

#endif /* __PRERTS_H__ */
