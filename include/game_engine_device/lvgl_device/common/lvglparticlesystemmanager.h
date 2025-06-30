#pragma once

#include "game_client/ParticleSys.h"

/**
 * LVGL-specific ParticleSystemManager implementation
 * This is a stub implementation for cross-platform development
 */
class LvglParticleSystemManager : public ParticleSystemManager
{
public:
    LvglParticleSystemManager();
    virtual ~LvglParticleSystemManager();

    // ParticleSystemManager interface implementation
    virtual void init() override;
    virtual void reset() override;
    virtual void update() override;
};