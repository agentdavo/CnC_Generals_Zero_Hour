#include "lvglDevice/Common/Win32BIGFileSystem.h"

Win32BIGFileSystem::Win32BIGFileSystem() {}
Win32BIGFileSystem::~Win32BIGFileSystem() {}

void Win32BIGFileSystem::init() {}
void Win32BIGFileSystem::update() {}
void Win32BIGFileSystem::reset() {}
void Win32BIGFileSystem::postProcessLoad() {}
void Win32BIGFileSystem::closeAllArchiveFiles() {}
ArchiveFile *Win32BIGFileSystem::openArchiveFile(const Char *) { return nullptr; }
void Win32BIGFileSystem::closeArchiveFile(const Char *) {}
void Win32BIGFileSystem::closeAllFiles() {}
Bool Win32BIGFileSystem::loadBigFilesFromDirectory(AsciiString, AsciiString, Bool) { return FALSE; }
