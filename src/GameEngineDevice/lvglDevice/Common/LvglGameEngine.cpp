#include "LvglGameEngine/LvglGameEngine.h"
#include "LvglPlatform/LvglPlatform.h"
#include "Common/Logger.h"

void LvglGameEngine::init()
{
    LOG_INFO("LvglGameEngine::init");
}

void LvglGameEngine::reset()
{
    LOG_INFO("LvglGameEngine::reset");
}

void LvglGameEngine::update()
{
    LOG_INFO("LvglGameEngine::update");
    LvglPlatform::poll_events();
}

void LvglGameEngine::serviceWindowsOS()
{
    LOG_INFO("LvglGameEngine::serviceWindowsOS");
    LvglPlatform::poll_events();
}
