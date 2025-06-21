#pragma once

#include "Common/CDManager.h"

class Win32CDManager : public CDManager
{
public:
    Win32CDManager() = default;
    virtual ~Win32CDManager() = default;

    virtual void init() override {}
    virtual void update() override {}
    virtual void reset() override {}
    virtual void refreshDrives() override {}

protected:
    virtual CDDriveInterface* createDrive() override { return nullptr; }
};
