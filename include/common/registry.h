#pragma once
// Cross platform stubs for Windows registry helpers
#include "GameEngine/common/asciiString.h"
#include "Lib/BaseType.h"

struct RegistryClass {};

Bool GetStringFromGeneralsRegistry(AsciiString path, AsciiString key, AsciiString& val);
Bool GetStringFromRegistry(AsciiString path, AsciiString key, AsciiString& val);
Bool GetUnsignedIntFromRegistry(AsciiString path, AsciiString key, UnsignedInt& val);
AsciiString GetRegistryLanguage(void);
AsciiString GetRegistryGameName(void);
UnsignedInt GetRegistryVersion(void);
UnsignedInt GetRegistryMapPackVersion(void);
