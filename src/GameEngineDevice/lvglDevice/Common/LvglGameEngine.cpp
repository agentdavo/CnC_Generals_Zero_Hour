#include "LvglGameEngine.h"
#include "LvglPlatform/LvglPlatform.h"
#include "Common/Logger.h"

void LvglGameEngine::init()
{
    LOG_INFO("LvglGameEngine::init");
    GameEngine::init();
}

void LvglGameEngine::reset()
{
    LOG_INFO("LvglGameEngine::reset");
    GameEngine::reset();
}

void LvglGameEngine::update()
{
    GameEngine::update();
    LOG_INFO("LvglGameEngine::update");

    if(!isActive()) {
        while(!isActive()) {
            LvglPlatform::poll_events();
            if(TheLAN != NULL) {
                TheLAN->setIsActive(isActive());
                TheLAN->update();
            }
            if(TheGameEngine->getQuitting() || TheGameLogic->isInInternetGame() || TheGameLogic->isInLanGame()) {
                break;
            }
        }
    }

    LvglPlatform::poll_events();
}

void LvglGameEngine::serviceWindowsOS()
{
    LOG_INFO("LvglGameEngine::serviceWindowsOS");
    LvglPlatform::poll_events();
}
