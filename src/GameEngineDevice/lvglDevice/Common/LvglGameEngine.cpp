#include "LvglGameEngine.h"
#include "LvglPlatform/LvglPlatform.h"

void LvglGameEngine::init()
{
    GameEngine::init();
}

void LvglGameEngine::reset()
{
    GameEngine::reset();
}

void LvglGameEngine::update()
{
    GameEngine::update();

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
    LvglPlatform::poll_events();
}
