#include "game_engine_device/lvgl_device/common/win32bigfile.h"

Win32BIGFile::Win32BIGFile() {}
Win32BIGFile::~Win32BIGFile() {}

File* Win32BIGFile::openFile(const Char *, Int) { return nullptr; }
void Win32BIGFile::closeAllFiles() {}
AsciiString Win32BIGFile::getName() { return AsciiString(); }
AsciiString Win32BIGFile::getPath() { return AsciiString(); }
void Win32BIGFile::setSearchPriority(Int) {}
void Win32BIGFile::close() {}
Bool Win32BIGFile::getFileInfo(const AsciiString &, FileInfo *) const { return FALSE; }
