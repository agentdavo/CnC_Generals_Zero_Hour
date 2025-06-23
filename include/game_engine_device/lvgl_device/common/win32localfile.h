#pragma once

#include "common/local_file.h"

class Win32LocalFile : public LocalFile
{
public:
    Win32LocalFile();
    virtual ~Win32LocalFile();
};

