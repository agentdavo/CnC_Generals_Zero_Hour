#include "lvglDevice/common/Win32GameEngine.h"

void Win32GameEngine::init() {}
void Win32GameEngine::reset() {}
void Win32GameEngine::update() {}
void Win32GameEngine::serviceWindowsOS() {}

GameLogic *Win32GameEngine::createGameLogic() { return nullptr; }
GameClient *Win32GameEngine::createGameClient() { return nullptr; }
ModuleFactory *Win32GameEngine::createModuleFactory() { return nullptr; }
ThingFactory *Win32GameEngine::createThingFactory() { return nullptr; }
FunctionLexicon *Win32GameEngine::createFunctionLexicon() { return nullptr; }
LocalFileSystem *Win32GameEngine::createLocalFileSystem() { return nullptr; }
ArchiveFileSystem *Win32GameEngine::createArchiveFileSystem() { return nullptr; }
NetworkInterface *Win32GameEngine::createNetwork() { return nullptr; }
Radar *Win32GameEngine::createRadar() { return nullptr; }
WebBrowser *Win32GameEngine::createWebBrowser() { return nullptr; }
AudioManager *Win32GameEngine::createAudioManager() { return nullptr; }
ParticleSystemManager *Win32GameEngine::createParticleSystemManager() { return nullptr; }
