#pragma once

#ifdef _WIN32
#include "Common/windows.h"
extern HINSTANCE ApplicationHInstance;
#else
using HINSTANCE = void*;
extern HINSTANCE ApplicationHInstance;
#endif

int MapCacheBuilderMain(int argc, char** argv);
