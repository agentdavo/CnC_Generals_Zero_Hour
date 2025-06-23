#include "common/AudioAffect.h"
#include "common/ArchiveFile.h"
#include "common/ArchiveFileSystem.h"
#include "common/File.h"
#include "common/GameAudio.h"
#include "common/GameMemory.h"
#include "common/LocalFileSystem.h"
#include "game_engine_device/lvgl_device/common/win32_big_file.h"
#include "game_engine_device/lvgl_device/common/win32_big_file_system.h"
#include "common/registry.h"
#include "common/win32_compat.h"
#include <memory>
#include <cstdint>
#include <cctype>
#include <cstring>

namespace {
static uint32_t from_be32(uint32_t v) {
    return ((v & 0xFF000000) >> 24) |
           ((v & 0x00FF0000) >> 8)  |
           ((v & 0x0000FF00) << 8)  |
           ((v & 0x000000FF) << 24);
}

static int ascii_strcasecmp(const char *a, const char *b) {
    while(*a && *b) {
        char ca = std::tolower(static_cast<unsigned char>(*a));
        char cb = std::tolower(static_cast<unsigned char>(*b));
        if(ca != cb)
            return ca - cb;
        ++a; ++b;
    }
    return std::tolower(static_cast<unsigned char>(*a)) - std::tolower(static_cast<unsigned char>(*b));
}
}

static const char *BIGFileIdentifier = "BIGF";

Win32BIGFileSystem::Win32BIGFileSystem() : ArchiveFileSystem() {}
Win32BIGFileSystem::~Win32BIGFileSystem() {}

void Win32BIGFileSystem::init() {
    DEBUG_ASSERTCRASH(TheLocalFileSystem != NULL, ("TheLocalFileSystem must be initialized before TheArchiveFileSystem."));
    if (TheLocalFileSystem == NULL) {
        return;
    }

    loadBigFilesFromDirectory("", "*.big");

    AsciiString installPath;
    GetStringFromGeneralsRegistry("", "InstallPath", installPath );
#ifndef _INTERNAL
    DEBUG_ASSERTCRASH(installPath != "", ("Be 1337! Go install Generals!"));
#endif
    if (installPath != "")
        loadBigFilesFromDirectory(installPath, "*.big");
}

void Win32BIGFileSystem::reset() {}
void Win32BIGFileSystem::update() {}
void Win32BIGFileSystem::postProcessLoad() {}

ArchiveFile * Win32BIGFileSystem::openArchiveFile(const Char *filename) {
    File *fp = TheLocalFileSystem->openFile(filename, File::READ | File::BINARY);
    AsciiString archiveFileName = filename;
    archiveFileName.toLower();
    Int archiveFileSize = 0;
    Int numLittleFiles = 0;

    std::unique_ptr<ArchiveFile> archiveFile = std::make_unique<Win32BIGFile>();

    DEBUG_LOG(("Win32BIGFileSystem::openArchiveFile - opening BIG file %s\n", filename));

    if (fp == NULL) {
        DEBUG_CRASH(("Could not open archive file %s for parsing", filename));
        return NULL;
    }

    char buffer[_MAX_PATH];
    fp->read(buffer, 4);
    buffer[4] = 0;
    if (strcmp(buffer, BIGFileIdentifier) != 0) {
        DEBUG_CRASH(("Error reading BIG file identifier in file %s", filename));
        fp->close();
        fp = NULL;
        return NULL;
    }

    fp->read(&archiveFileSize, 4);
    DEBUG_LOG(("Win32BIGFileSystem::openArchiveFile - size of archive file is %d bytes\n", archiveFileSize));

    fp->read(&numLittleFiles, 4);
    numLittleFiles = static_cast<Int>(from_be32(static_cast<uint32_t>(numLittleFiles)));
    DEBUG_LOG(("Win32BIGFileSystem::openArchiveFile - %d are contained in archive\n", numLittleFiles));

    fp->seek(0x10, File::START);
    std::unique_ptr<ArchivedFileInfo> fileInfo = std::make_unique<ArchivedFileInfo>();

    for (Int i = 0; i < numLittleFiles; ++i) {
        uint32_t filesize = 0;
        uint32_t fileOffset = 0;
        fp->read(&fileOffset, 4);
        fp->read(&filesize, 4);

        filesize = from_be32(filesize);
        fileOffset = from_be32(fileOffset);

        fileInfo->m_archiveFilename = archiveFileName;
        fileInfo->m_offset = fileOffset;
        fileInfo->m_size = filesize;

        Int pathIndex = -1;
        do {
            ++pathIndex;
            fp->read(buffer + pathIndex, 1);
        } while (buffer[pathIndex] != 0);

        Int filenameIndex = pathIndex;
        while ((buffer[filenameIndex] != '\\') && (buffer[filenameIndex] != '/') && (filenameIndex >= 0)) {
            --filenameIndex;
        }

        fileInfo->m_filename = (char *)(buffer + filenameIndex + 1);
        fileInfo->m_filename.toLower();
        buffer[filenameIndex + 1] = 0;

        AsciiString path = buffer;
        archiveFile->addFile(path, *fileInfo);
    }

    archiveFile->attachFile(fp);

    return archiveFile.release();
}

void Win32BIGFileSystem::closeArchiveFile(const Char *filename) {
    ArchiveFileMap::iterator it =  m_archiveFileMap.find(filename);
    if (it == m_archiveFileMap.end()) {
        return;
    }

    if (ascii_strcasecmp(filename, MUSIC_BIG) == 0) {
        TheAudio->stopAudio(AudioAffect_Music);
    }
    DEBUG_ASSERTCRASH(ascii_strcasecmp(filename, MUSIC_BIG) == 0, ("Attempting to close Archive file '%s', need to add code to handle its shutdown correctly.", filename));

    delete (it->second);
    m_archiveFileMap.erase(it);
}

void Win32BIGFileSystem::closeAllArchiveFiles() {}
void Win32BIGFileSystem::closeAllFiles() {}

Bool Win32BIGFileSystem::loadBigFilesFromDirectory(AsciiString dir, AsciiString fileMask, Bool overwrite) {
    FilenameList filenameList;
    TheLocalFileSystem->getFileListInDirectory(dir, AsciiString(""), fileMask, filenameList, TRUE);

    Bool actuallyAdded = FALSE;
    FilenameListIter it = filenameList.begin();
    while (it != filenameList.end()) {
        ArchiveFile *archiveFile = openArchiveFile((*it).str());

        if (archiveFile != NULL) {
            DEBUG_LOG(("Win32BIGFileSystem::loadBigFilesFromDirectory - loading %s into the directory tree.\n", (*it).str()));
            loadIntoDirectoryTree(archiveFile, *it, overwrite);
            m_archiveFileMap[(*it)] = archiveFile;
            DEBUG_LOG(("Win32BIGFileSystem::loadBigFilesFromDirectory - %s inserted into the archive file map.\n", (*it).str()));
            actuallyAdded = TRUE;
        }

        it++;
    }

    return actuallyAdded;
}
