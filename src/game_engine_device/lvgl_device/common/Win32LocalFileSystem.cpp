#include "game_engine_device/lvgl_device/common/win32localfilesystem.h"

void Win32LocalFileSystem::init() {}
void Win32LocalFileSystem::reset() {}
void Win32LocalFileSystem::update() {}

File *Win32LocalFileSystem::openFile(const Char *, Int) { return nullptr; }
Bool Win32LocalFileSystem::doesFileExist(const Char *) const { return FALSE; }
void Win32LocalFileSystem::getFileListInDirectory(const AsciiString &, const AsciiString &, const AsciiString &, FilenameList &, Bool) const {}
Bool Win32LocalFileSystem::getFileInfo(const AsciiString &, FileInfo *) const { return FALSE; }
Bool Win32LocalFileSystem::createDirectory(AsciiString) { return FALSE; }
