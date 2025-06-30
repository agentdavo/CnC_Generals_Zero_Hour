#pragma once

#include "game_engine/common/gameengine.h"

// Forward declarations for factory methods
class LocalFileSystem;
class ArchiveFileSystem;
class GameLogic;
class GameClient;
class ModuleFactory;
class ThingFactory;
class FunctionLexicon;
class Radar;
class WebBrowser;
class ParticleSystemManager;
class AudioManager;

class LvglGameEngine : public GameEngine
{
public:
    LvglGameEngine();
    virtual ~LvglGameEngine();

    // GameEngine interface implementation
    virtual void init() override;
    virtual void reset() override;
    virtual void update() override;
    virtual void serviceWindowsOS() override;

protected:
    // Factory methods - create LVGL-specific implementations
    virtual LocalFileSystem* createLocalFileSystem() override;
    virtual ArchiveFileSystem* createArchiveFileSystem() override;
    virtual GameLogic* createGameLogic() override;
    virtual GameClient* createGameClient() override;
    virtual ModuleFactory* createModuleFactory() override;
    virtual ThingFactory* createThingFactory() override;
    virtual FunctionLexicon* createFunctionLexicon() override;
    virtual Radar* createRadar() override;
    virtual WebBrowser* createWebBrowser() override;
    virtual ParticleSystemManager* createParticleSystemManager() override;
    virtual AudioManager* createAudioManager() override;
};
