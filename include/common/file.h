#pragma once

#include "game_engine/common/file.h"
#include <string>
#include <vector>

namespace FileUtil {

bool exists(const std::string &path);

bool createDirectory(const std::string &path);

std::vector<std::string> listFiles(const std::string &directory,
                                   const std::string &pattern = "*",
                                   bool recursive = false);

}

