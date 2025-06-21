#pragma once

#include "Common/LocalFile.h"

class Win32LocalFile : public LocalFile
{
public:
    Win32LocalFile() = default;
    virtual ~Win32LocalFile() = default;
};
