#include "lvglDevice/common/Win32CDManager.h"

Win32CDDrive::Win32CDDrive() {}
Win32CDDrive::~Win32CDDrive() {}
void Win32CDDrive::refreshInfo() {}

Win32CDManager::Win32CDManager() {}
Win32CDManager::~Win32CDManager() {}

void Win32CDManager::init() {}
void Win32CDManager::update() {}
void Win32CDManager::reset() {}
void Win32CDManager::refreshDrives() {}
CDDriveInterface *Win32CDManager::createDrive() { return nullptr; }
