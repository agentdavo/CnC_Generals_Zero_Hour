/*
**    Command & Conquer Generals(tm)
**    Copyright 2025 Electronic Arts Inc.
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  LVGL Game Engine Implementation                                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "lvglgameengine.h"
#include "common/logger.h"
#include "lvgl_platform/lvgl_platform.h"

// Required includes for factory methods
#include "game_engine_device/lvgl_device/common/lvgllocalfilesystem.h"
#include "game_engine_device/lvgl_device/common/win32_big_file_system.h"
#include "game_engine/game_logic/gamelogic.h"
#include "game_engine_device/lvgl_device/game_client/lvglgameclient.h"
#include "game_engine_device/lvgl_device/common/lvglmodulefactory.h"
#include "game_engine_device/lvgl_device/common/lvglthingfactory.h"
#include "game_engine_device/lvgl_device/common/lvglfunctionlexicon.h"
#include "game_engine_device/lvgl_device/common/lvglradar.h"
#include "game_engine_device/lvgl_device/common/lvglwebbrowser.h"
#include "game_engine_device/lvgl_device/common/lvglparticlesystemmanager.h"
#include "game_engine_device/lvgl_device/common/lvglaudiomanager.h"

LvglGameEngine::LvglGameEngine()
{
    LOG_INFO("LvglGameEngine constructor");
}

LvglGameEngine::~LvglGameEngine()
{
    LOG_INFO("LvglGameEngine destructor");
}

void LvglGameEngine::init()
{
    LOG_INFO("LvglGameEngine::init() - calling base class init");
    // Call the base class initialization with empty args for now
    // The real command line args will be passed later
    int argc = 0;
    char **argv = nullptr;
    GameEngine::init(argc, argv);
}

void LvglGameEngine::reset()
{
    LOG_INFO("LvglGameEngine::reset()");
    GameEngine::reset();
}

void LvglGameEngine::update()
{
    // Process LVGL events
    LvglPlatform::poll_events();
    
    // Call base class update
    GameEngine::update();
}

void LvglGameEngine::serviceWindowsOS()
{
    // Cross-platform equivalent of servicing the OS
    // For LVGL, this means processing events
    LvglPlatform::poll_events();
}

////////////////////////////////////////////////////////////////////////////////
// Factory Methods - these create platform-specific implementations //////////
////////////////////////////////////////////////////////////////////////////////

LocalFileSystem* LvglGameEngine::createLocalFileSystem()
{
    LOG_INFO("Creating LVGL LocalFileSystem");
    return new LvglLocalFileSystem();
}

ArchiveFileSystem* LvglGameEngine::createArchiveFileSystem()
{
    LOG_INFO("Creating LVGL ArchiveFileSystem");
    return new Win32BIGFileSystem();  // This can be reused for cross-platform
}

GameLogic* LvglGameEngine::createGameLogic()
{
    LOG_INFO("Creating GameLogic");
    return new GameLogic();
}

GameClient* LvglGameEngine::createGameClient()
{
    LOG_INFO("Creating LVGL GameClient");
    return new LvglGameClient();
}

ModuleFactory* LvglGameEngine::createModuleFactory()
{
    LOG_INFO("Creating LVGL ModuleFactory");
    return new LvglModuleFactory();
}

ThingFactory* LvglGameEngine::createThingFactory()
{
    LOG_INFO("Creating LVGL ThingFactory");
    return new LvglThingFactory();
}

FunctionLexicon* LvglGameEngine::createFunctionLexicon()
{
    LOG_INFO("Creating LVGL FunctionLexicon");
    return new LvglFunctionLexicon();
}

Radar* LvglGameEngine::createRadar()
{
    LOG_INFO("Creating LVGL Radar");
    return new LvglRadar();
}

WebBrowser* LvglGameEngine::createWebBrowser()
{
    LOG_INFO("Creating LVGL WebBrowser");
    return new LvglWebBrowser();
}

ParticleSystemManager* LvglGameEngine::createParticleSystemManager()
{
    LOG_INFO("Creating LVGL ParticleSystemManager");
    return new LvglParticleSystemManager();
}

AudioManager* LvglGameEngine::createAudioManager()
{
    LOG_INFO("Creating LVGL AudioManager");
    return new LvglAudioManager();
}