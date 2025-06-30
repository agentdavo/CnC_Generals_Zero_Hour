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

// GameMain.cpp
// The main entry point for the game
// Author: Michael S. Booth, April 2001

#include "prerts.h"	// This must go first in EVERY cpp file in the GameEngine

#include "common/GameEngine.h"
#include "common/logger.h"


/**
 * This is the entry point for the game system.
 */
void GameMain( int argc, char *argv[] )
{
	LOG_INFO("===============================================================================");
	LOG_INFO("GameMain: Entering game system main entry point");
	LOG_INFO("GameMain: argc=%d", argc);
	for(int i = 0; i < argc; ++i) {
		LOG_INFO("GameMain: argv[%d] = '%s'", i, argv[i]);
	}
	LOG_INFO("===============================================================================");

	LOG_INFO("GameMain: Creating game engine via factory function...");
	// initialize the game engine using factory function
	TheGameEngine = CreateGameEngine();
	if(!TheGameEngine) {
		LOG_ERROR("GameMain: CRITICAL - CreateGameEngine() returned NULL!");
		return;
	}
	LOG_INFO("GameMain: Game engine created successfully at %p", (void*)TheGameEngine);

	LOG_INFO("GameMain: Initializing game engine with command line arguments...");
	TheGameEngine->init(argc, argv);
	LOG_INFO("GameMain: Game engine initialization completed");

	LOG_INFO("GameMain: Starting main game execution loop...");
	LOG_INFO("GameMain: From this point, UI and game systems will start loading");
	// run it
	TheGameEngine->execute();
	LOG_INFO("GameMain: Main game execution loop has ended");

	LOG_INFO("GameMain: Cleaning up game engine...");
	// since execute() returned, we are exiting the game
	delete TheGameEngine;
	TheGameEngine = NULL;
	LOG_INFO("GameMain: Game engine cleanup completed");
	LOG_INFO("===============================================================================");
	LOG_INFO("GameMain: Exiting game system main entry point");
	LOG_INFO("===============================================================================");
}

