#pragma once

#include "Common/LocalFileSystem.h"

class Win32LocalFileSystem : public LocalFileSystem
{
public:
    Win32LocalFileSystem() = default;
    virtual ~Win32LocalFileSystem() = default;

    virtual void init() override {}
    virtual void reset() override {}
    virtual void update() override {}

    virtual File* openFile(const Char* filename, Int access = 0) override { return nullptr; }
    virtual Bool doesFileExist(const Char* filename) const override { return FALSE; }
    virtual void getFileListInDirectory(const AsciiString& currentDirectory,
                                        const AsciiString& originalDirectory,
                                        const AsciiString& searchName,
                                        FilenameList& filenameList,
                                        Bool searchSubdirectories) const override {}
    virtual Bool getFileInfo(const AsciiString& filename, FileInfo* fileInfo) const override { return FALSE; }
    virtual Bool createDirectory(AsciiString directory) override { return FALSE; }
};
