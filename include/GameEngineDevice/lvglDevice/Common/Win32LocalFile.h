#pragma once

#include "common/LocalFile.h"

class Win32LocalFile : public LocalFile
{
public:
    Win32LocalFile();
    virtual ~Win32LocalFile();
};

