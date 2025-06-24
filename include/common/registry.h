#pragma once
// Cross platform stubs for Windows registry helpers
#include "game_engine/common/ascii_string.h"
#include "Lib/BaseType.h"

struct RegistryClass {};

Bool GetStringFromGeneralsRegistry(AsciiString path, AsciiString key, AsciiString& val);
Bool GetStringFromRegistry(AsciiString path, AsciiString key, AsciiString& val);
Bool GetUnsignedIntFromRegistry(AsciiString path, AsciiString key, UnsignedInt& val);
AsciiString GetRegistryLanguage(void);
AsciiString GetRegistryGameName(void);
UnsignedInt GetRegistryVersion(void);
UnsignedInt GetRegistryMapPackVersion(void);
