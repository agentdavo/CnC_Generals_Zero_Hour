/*
**	Command & Conquer Generals(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

//////// win32_big_file_system.h ///////////////////////////
// Bryan Cleveland, August 2002
/////////////////////////////////////////////////////////////

#include "common/byte_swap.h"
#include "common/AudioAffect.h"
#include "common/ArchiveFile.h"
#include "common/ArchiveFileSystem.h"
#include "common/file.h"
#include "common/GameAudio.h"
#include "common/gamememory.h"
#include "common/LocalFileSystem.h"
#include "win32_device/common/win32_big_file.h"
#include "win32_device/common/win32_big_file_system.h"
#include <memory>
#include <filesystem>

#ifdef _INTERNAL
// for occasional debugging...
//#pragma optimize("", off)
//#pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")
#endif

static const char *BIGFileIdentifier = "BIGF";

Win32BIGFileSystem::Win32BIGFileSystem() : ArchiveFileSystem() {
}

Win32BIGFileSystem::~Win32BIGFileSystem() {
}

void Win32BIGFileSystem::init() {
	DEBUG_ASSERTCRASH(TheLocalFileSystem != NULL, ("TheLocalFileSystem must be initialized before TheArchiveFileSystem."));
	if (TheLocalFileSystem == NULL) {
		return;
	}

	loadBigFilesFromDirectory("", "*.big");
}

void Win32BIGFileSystem::reset() {
}

void Win32BIGFileSystem::update() {
}

void Win32BIGFileSystem::postProcessLoad() {
}

ArchiveFile * Win32BIGFileSystem::openArchiveFile(const std::filesystem::path &filename) {
        File *fp = TheLocalFileSystem->openFile(filename.generic_string().c_str(), File::READ | File::BINARY);
        AsciiString archiveFileName;
        archiveFileName = filename.generic_string().c_str();
	archiveFileName.toLower();
	Int archiveFileSize = 0;
	Int numLittleFiles = 0;

        std::unique_ptr<ArchiveFile> archiveFile(new Win32BIGFile);

        DEBUG_LOG(("Win32BIGFileSystem::openArchiveFile - opening BIG file %s\n", filename.generic_string().c_str()));

	if (fp == NULL) {
                DEBUG_CRASH(("Could not open archive file %s for parsing", filename.generic_string().c_str()));
		return NULL;
	}

        char id[5];
        fp->read(id, 4); // read the "BIG" at the beginning of the file.
        id[4] = 0;
        if (strcmp(id, BIGFileIdentifier) != 0) {
		DEBUG_CRASH(("Error reading BIG file identifier in file %s", filename));
		fp->close();
		fp = NULL;
		return NULL;
	}

	// read in the file size.
	fp->read(&archiveFileSize, 4);

	DEBUG_LOG(("Win32BIGFileSystem::openArchiveFile - size of archive file is %d bytes\n", archiveFileSize));

//	char t;

	// read in the number of files contained in this BIG file.
	// change the order of the bytes cause the file size is in reverse byte order for some reason.
        fp->read(&numLittleFiles, 4);
        numLittleFiles = static_cast<Int>(common::be_to_host(static_cast<std::uint32_t>(numLittleFiles)));

	DEBUG_LOG(("Win32BIGFileSystem::openArchiveFile - %d are contained in archive\n", numLittleFiles));
//	for (Int i = 0; i < 2; ++i) {
//		t = buffer[i];
//		buffer[i] = buffer[(4-i)-1];
//		buffer[(4-i)-1] = t;
//	}

        // seek to the beginning of the directory listing.
        fp->seek(0x10, File::START);
        // read in each directory listing.
        std::unique_ptr<ArchivedFileInfo> fileInfo(new ArchivedFileInfo);

	for (Int i = 0; i < numLittleFiles; ++i) {
		Int filesize = 0;
		Int fileOffset = 0;
		fp->read(&fileOffset, 4);
		fp->read(&filesize, 4);

                filesize = static_cast<Int>(common::be_to_host(static_cast<std::uint32_t>(filesize)));
                fileOffset = static_cast<Int>(common::be_to_host(static_cast<std::uint32_t>(fileOffset)));

		fileInfo->m_archiveFilename = archiveFileName;
		fileInfo->m_offset = fileOffset;
		fileInfo->m_size = filesize;
		
                // read the entry path
                std::string entryPathStr;
                char ch;
                do {
                        fp->read(&ch, 1);
                        if (ch != 0)
                                entryPathStr.push_back(ch);
                } while (ch != 0);

                std::filesystem::path entryPath(entryPathStr);
                fileInfo->m_filename = entryPath.filename().string().c_str();
                fileInfo->m_filename.toLower();
                AsciiString path;
                path = entryPath.parent_path().generic_string().c_str();
//		DEBUG_LOG(("Win32BIGFileSystem::openArchiveFile - adding file %s to archive file %s, file number %d\n", debugpath.str(), fileInfo->m_archiveFilename.str(), i));

		archiveFile->addFile(path, *fileInfo);
	}

        archiveFile->attachFile(fp);
        fileInfo.reset();

	// leave fp open as the archive file will be using it.

        return archiveFile.release();
}

void Win32BIGFileSystem::closeArchiveFile(const Char *filename) {
	// Need to close the specified big file
	ArchiveFileMap::iterator it =  m_archiveFileMap.find(filename);
	if (it == m_archiveFileMap.end()) {
		return;
	}

	if (stricmp(filename, MUSIC_BIG) == 0) {
		// Stop the current audio
		TheAudio->stopAudio(AudioAffect_Music);

		// No need to turn off other audio, as the lookups will just fail.
	}
	DEBUG_ASSERTCRASH(stricmp(filename, MUSIC_BIG) == 0, ("Attempting to close Archive file '%s', need to add code to handle its shutdown correctly.", filename));

	// may need to do some other processing here first.
	
	delete (it->second);
	m_archiveFileMap.erase(it);
}

void Win32BIGFileSystem::closeAllArchiveFiles() {
}

void Win32BIGFileSystem::closeAllFiles() {
}

Bool Win32BIGFileSystem::loadBigFilesFromDirectory(AsciiString dir, AsciiString fileMask, Bool overwrite) {

	FilenameList filenameList;
	TheLocalFileSystem->getFileListInDirectory(dir, AsciiString(""), fileMask, filenameList, TRUE);

	Bool actuallyAdded = FALSE;
        FilenameListIter it = filenameList.begin();
        while (it != filenameList.end()) {
                ArchiveFile *archiveFile = openArchiveFile(std::filesystem::path((*it).str()));

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
