#pragma once

#include "Common/CDManager.h"

class Win32CDDrive : public CDDrive
{
public:
    Win32CDDrive();
    virtual ~Win32CDDrive();
    virtual void refreshInfo() override;
};

class Win32CDManager : public CDManager
{
public:
    Win32CDManager();
    virtual ~Win32CDManager();

    virtual void init() override;
    virtual void update() override;
    virtual void reset() override;
    virtual void refreshDrives() override;

protected:
    virtual CDDriveInterface *createDrive() override;
};

