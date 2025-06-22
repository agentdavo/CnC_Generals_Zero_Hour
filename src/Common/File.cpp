#include "Common/File.h"
#include <filesystem>

namespace FileUtil {

bool exists(const std::string &path) {
    return std::filesystem::exists(std::filesystem::u8path(path));
}

bool createDirectory(const std::string &path) {
    if(path.empty())
        return false;
    std::error_code ec;
    std::filesystem::create_directories(std::filesystem::u8path(path), ec);
    return ec ? false : true;
}

std::vector<std::string> listFiles(const std::string &directory,
                                   const std::string &pattern,
                                   bool recursive) {
    namespace fs = std::filesystem;
    std::vector<std::string> result;
    fs::path base = fs::u8path(directory);
    fs::path pat  = fs::u8path(pattern);
    if(!fs::exists(base))
        return result;
    auto match = [&](const fs::path &p){
        if(p.filename() == pat.filename())
            return true;
        if(p.extension() == pat.extension() && pat.filename() == "*" + pat.extension().string())
            return true;
        if(pattern == "*")
            return true;
        return false;
    };
    if(recursive) {
        for(const auto &entry : fs::recursive_directory_iterator(base)) {
            if(entry.is_regular_file() && match(entry.path()))
                result.push_back(entry.path().u8string());
        }
    } else {
        for(const auto &entry : fs::directory_iterator(base)) {
            if(entry.is_regular_file() && match(entry.path()))
                result.push_back(entry.path().u8string());
        }
    }
    return result;
}

} // namespace FileUtil

