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

// FILE: Win32GameEngine.h ////////////////////////////////////////////////////////////////////////
// Author: Colin Day, April 2001
// Description:
//   Device implementation of the game engine ... this is, of course, the
//   highest level of the game that creates the necessary interfaces to the
//   devices we need
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __WIN32GAMEENGINE_H_
#define __WIN32GAMEENGINE_H_

#include "common/GameEngine.h"
#include "game_logic/game_logic.h"
#include "GameNetwork/NetworkInterface.h"
#include "miles_audio_device/miles_audio_manager.h"
#include "win32_device/common/Win32BIGFileSystem.h"
#include "win32_device/common/Win32LocalFileSystem.h"
#include "w3d_device/common/W3DModuleFactory.h"
#include "w3d_device/game_logic/W3Dgame_logic.h"
#include "w3d_device/game_client/W3Dgame_client.h"
#include "w3d_device/game_client/W3DWebBrowser.h"
#include "w3d_device/common/W3DFunctionLexicon.h"
#include "w3d_device/common/W3DRadar.h"
#include "w3d_device/common/W3DFunctionLexicon.h"
#include "w3d_device/common/W3DThingFactory.h"

//-------------------------------------------------------------------------------------------------
/** Class declaration for the Win32 game engine */
//-------------------------------------------------------------------------------------------------
class Win32GameEngine : public GameEngine
{

public:
	Win32GameEngine();
	virtual ~Win32GameEngine();

	virtual void init(void);			 ///< initialization
	virtual void reset(void);			 ///< reset engine
	virtual void update(void);			 ///< update the game engine
	virtual void serviceWindowsOS(void); ///< allow windows maintenance in background

protected:
	virtual game_logic *creategame_logic(void);				  ///< factory for game logic
	virtual game_client *creategame_client(void);				  ///< factory for game client
	virtual ModuleFactory *createModuleFactory(void);		  ///< factory for creating modules
	virtual ThingFactory *createThingFactory(void);			  ///< factory for the thing factory
	virtual FunctionLexicon *createFunctionLexicon(void);	  ///< factory for function lexicon
	virtual LocalFileSystem *createLocalFileSystem(void);	  ///< factory for local file system
	virtual ArchiveFileSystem *createArchiveFileSystem(void); ///< factory for archive file system
	virtual NetworkInterface *createNetwork(void);			  ///< Factory for the network
	virtual Radar *createRadar(void);						  ///< Factory for radar
	virtual WebBrowser *createWebBrowser(void);				  ///< Factory for embedded browser
	virtual AudioManager *createAudioManager(void);			  ///< Factory for audio device
	virtual ParticleSystemManager *createParticleSystemManager(void);

protected:
	UINT m_previousErrorMode;
}; // end Win32GameEngine

// INLINE -----------------------------------------------------------------------------------------
inline game_logic *Win32GameEngine::creategame_logic(void) { return NEW W3Dgame_logic; }
inline game_client *Win32GameEngine::creategame_client(void) { return NEW W3Dgame_client; }
inline ModuleFactory *Win32GameEngine::createModuleFactory(void) { return NEW W3DModuleFactory; }
inline ThingFactory *Win32GameEngine::createThingFactory(void) { return NEW W3DThingFactory; }
inline FunctionLexicon *Win32GameEngine::createFunctionLexicon(void) { return NEW W3DFunctionLexicon; }
inline LocalFileSystem *Win32GameEngine::createLocalFileSystem(void) { return NEW Win32LocalFileSystem; }
inline ArchiveFileSystem *Win32GameEngine::createArchiveFileSystem(void) { return NEW Win32BIGFileSystem; }
inline ParticleSystemManager *Win32GameEngine::createParticleSystemManager(void) { return NEW W3DParticleSystemManager; }

inline NetworkInterface *Win32GameEngine::createNetwork(void) { return NetworkInterface::createNetwork(); }
inline Radar *Win32GameEngine::createRadar(void) { return NEW W3DRadar; }
inline WebBrowser *Win32GameEngine::createWebBrowser(void) { return NEW CComObject<W3DWebBrowser>; }
inline AudioManager *Win32GameEngine::createAudioManager(void) { return NEW MilesAudioManager; }

#endif // end __WIN32GAMEENGINE_H_
