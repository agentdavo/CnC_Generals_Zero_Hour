#pragma once

#include "Common/GameEngine.h"
#include "GameNetwork/NetworkInterface.h"
#include "GameLogic/GameLogic.h"

class Win32GameEngine : public GameEngine
{
public:
    Win32GameEngine() = default;
    virtual ~Win32GameEngine() = default;

    virtual void init();
    virtual void reset();
    virtual void update();
    virtual void serviceWindowsOS();

protected:
    virtual GameLogic *createGameLogic();
    virtual GameClient *createGameClient();
    virtual ModuleFactory *createModuleFactory();
    virtual ThingFactory *createThingFactory();
    virtual FunctionLexicon *createFunctionLexicon();
    virtual LocalFileSystem *createLocalFileSystem();
    virtual ArchiveFileSystem *createArchiveFileSystem();
    virtual NetworkInterface *createNetwork();
    virtual Radar *createRadar();
    virtual WebBrowser *createWebBrowser();
    virtual AudioManager *createAudioManager();
    virtual ParticleSystemManager *createParticleSystemManager();
};

