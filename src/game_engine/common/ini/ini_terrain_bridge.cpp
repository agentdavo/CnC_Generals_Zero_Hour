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

// FILE: INITerrainBridge.cpp /////////////////////////////////////////////////////////////////////
// Author: Colin Day, December 2001
// Desc:   Terrain bridge INI loading
///////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "PreRTS.h"	// This must go first in EVERY cpp file int the GameEngine

#include "common/ini.h"
#include "game_client/TerrainRoads.h"

//-------------------------------------------------------------------------------------------------
/** Parse Terrain Bridge entry */
//-------------------------------------------------------------------------------------------------
void INI::parseTerrainBridgeDefinition( INI* ini )
{
	AsciiString name;
	TerrainRoadType *bridge;

	// read the name
	const char* c = ini->getNextToken();
	name.set( c );	

	// find existing item if present or allocate new one
	bridge = TheTerrainRoads->findBridge( name );

	// if item is found it better already be a bridge
	if( bridge )
	{

		// sanity
		DEBUG_ASSERTCRASH( bridge->isBridge(), ("Redefining road '%s' as a bridge!\n", 
											 bridge->getName().str()) );
		throw INI_INVALID_DATA;

	}  // end if

	if( bridge == NULL )	
		bridge = TheTerrainRoads->newBridge( name );

	DEBUG_ASSERTCRASH( bridge, ("Unable to allcoate bridge '%s'\n", name.str()) );

	// parse the ini definition
	ini->initFromINI( bridge, bridge->getBridgeFieldParse() );

}  // end parseTerrainBridge




