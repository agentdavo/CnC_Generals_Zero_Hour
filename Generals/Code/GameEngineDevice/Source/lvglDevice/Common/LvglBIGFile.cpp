#include "Common/LocalFile.h"
#include "Common/LocalFileSystem.h"
#include "Common/RAMFile.h"
#include "Common/StreamingArchiveFile.h"
#include "Common/GameMemory.h"
#include "Common/PerfTimer.h"
#include "Win32Device/Common/Win32BIGFile.h"

Win32BIGFile::Win32BIGFile() {
}

Win32BIGFile::~Win32BIGFile() {
}

File* Win32BIGFile::openFile(const Char *filename, Int access) {
    const ArchivedFileInfo *fileInfo = getArchivedFileInfo(AsciiString(filename));
    if (fileInfo == nullptr)
        return nullptr;

    RAMFile *ramFile = nullptr;
    if(BitTest(access, File::STREAMING))
        ramFile = newInstance( StreamingArchiveFile );
    else
        ramFile = newInstance( RAMFile );

    ramFile->deleteOnClose();
    if(!ramFile->openFromArchive(m_file, fileInfo->m_filename, fileInfo->m_offset, fileInfo->m_size)) {
        ramFile->close();
        ramFile = nullptr;
        return nullptr;
    }

    if((access & File::WRITE) == 0)
        return ramFile;

    File *localFile = TheLocalFileSystem->openFile(filename, access);
    if(localFile != nullptr)
        ramFile->copyDataToFile(localFile);

    ramFile->close();
    ramFile = nullptr;
    return localFile;
}

void Win32BIGFile::closeAllFiles() {
}

AsciiString Win32BIGFile::getName() {
    return m_name;
}

AsciiString Win32BIGFile::getPath() {
    return m_path;
}

void Win32BIGFile::setSearchPriority(Int new_priority) {
}

void Win32BIGFile::close() {
}

Bool Win32BIGFile::getFileInfo(const AsciiString& filename, FileInfo *fileInfo) const {
    const ArchivedFileInfo *tempFileInfo = getArchivedFileInfo(filename);
    if(tempFileInfo == nullptr)
        return FALSE;

    TheLocalFileSystem->getFileInfo(AsciiString(m_file->getName()), fileInfo);
    fileInfo->sizeHigh = 0;
    fileInfo->sizeLow = tempFileInfo->m_size;
    return TRUE;
}
