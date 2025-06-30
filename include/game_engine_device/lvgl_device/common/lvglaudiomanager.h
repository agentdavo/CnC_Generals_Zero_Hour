#pragma once

#include "game_engine/common/gameaudio.h"

/**
 * LVGL-specific AudioManager implementation
 * This is a stub implementation for cross-platform development
 */
class LvglAudioManager : public AudioManager
{
public:
    LvglAudioManager();
    virtual ~LvglAudioManager();

    // AudioManager interface implementation
    virtual void init() override;
    virtual void reset() override;
    virtual void update() override;
};