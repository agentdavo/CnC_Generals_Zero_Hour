#include "game_engine_device/lvgl_device/common/lvgllocalfilesystem.h"
#include "game_engine/common/ascii_string.h"
#include "game_engine/common/gamememory.h"
#include "game_engine/common/perftimer.h"
#include "common/local_file.h"
#if __cpp_lib_filesystem >= 201703L
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

File *LvglLocalFileSystem::openFile(const Char *filename, Int access)
{
    LocalFile *file = new LocalFile();
    fs::path p(filename);

    if(access & File::WRITE) {
        std::error_code ec;
        fs::create_directories(p.parent_path(), ec);
    }

    if(file->open(p.u8string().c_str(), access) == FALSE) {
        file->close();
        file->deleteInstance();
        return nullptr;
    }

    file->deleteOnClose();
    return file;
}

Bool LvglLocalFileSystem::doesFileExist(const Char *filename) const
{
    return fs::exists(fs::u8path(filename));
}

void LvglLocalFileSystem::getFileListInDirectory(const AsciiString &currentDirectory,
                                                 const AsciiString &originalDirectory,
                                                 const AsciiString &searchName,
                                                 FilenameList &filenameList,
                                                 Bool searchSubdirectories) const
{
    fs::path base = fs::u8path(originalDirectory.str()) / fs::u8path(currentDirectory.str());
    fs::path pattern = fs::u8path(searchName.str());
    if(!fs::exists(base))
        return;

    auto add_entry = [&](const fs::directory_entry &entry){
        if(entry.is_regular_file()) {
            fs::path rel = fs::relative(entry.path(), fs::u8path(originalDirectory.str()));
            AsciiString newName;
            newName = originalDirectory;
            newName.concat(rel.u8string().c_str());
            if(filenameList.find(newName) == filenameList.end()) {
                filenameList.insert(newName);
            }
        }
    };

    if(searchSubdirectories) {
        for(const auto &entry : fs::recursive_directory_iterator(base)) {
            if(entry.path().filename().native() == pattern.native()) {
                add_entry(entry);
            } else if(entry.is_regular_file() && fs::path(entry.path()).extension() == pattern.extension()) {
                if(pattern.filename() == "*" + pattern.extension().string())
                    add_entry(entry);
            }
        }
    } else {
        for(const auto &entry : fs::directory_iterator(base)) {
            if(entry.path().filename() == pattern.filename()) {
                add_entry(entry);
            } else if(entry.is_regular_file() && entry.path().extension() == pattern.extension()) {
                if(pattern.filename() == "*" + pattern.extension().string())
                    add_entry(entry);
            }
        }
    }
}

Bool LvglLocalFileSystem::getFileInfo(const AsciiString &filename, FileInfo *fileInfo) const
{
    fs::path p = fs::u8path(filename.str());
    if(!fs::exists(p))
        return FALSE;

    std::error_code ec;
    fileInfo->sizeLow = static_cast<Int>(fs::file_size(p, ec));
    fileInfo->sizeHigh = 0;
    auto ftime = fs::last_write_time(p, ec).time_since_epoch();
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(ftime).count();
    fileInfo->timestampHigh = static_cast<Int>(secs >> 32);
    fileInfo->timestampLow = static_cast<Int>(secs & 0xffffffff);
    return TRUE;
}

Bool LvglLocalFileSystem::createDirectory(AsciiString directory)
{
    std::error_code ec;
    fs::create_directories(fs::u8path(directory.str()), ec);
    return ec ? FALSE : TRUE;
}
