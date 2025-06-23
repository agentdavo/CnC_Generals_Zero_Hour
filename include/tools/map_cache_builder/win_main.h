#pragma once

#ifdef _WIN32
#include "common/windows.h"
extern HINSTANCE ApplicationHInstance;
#else
using HINSTANCE = void *;
extern HINSTANCE ApplicationHInstance;
#endif

int MapCacheBuilderMain(int argc, char **argv);
