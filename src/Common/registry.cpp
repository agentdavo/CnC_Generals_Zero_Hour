#include "Common/registry.h"
#include "GameEngine/Common/AsciiString.h"
#include "Lib/BaseType.h"
#include <cstdlib>

Bool GetStringFromGeneralsRegistry(AsciiString path, AsciiString key, AsciiString& val)
{
    const char* env = std::getenv("GENERALS_INSTALL_PATH");
    if(env)
    {
        val = env;
        return TRUE;
    }
    val = "";
    return FALSE;
}

Bool GetStringFromRegistry(AsciiString path, AsciiString key, AsciiString& val)
{
    return GetStringFromGeneralsRegistry(path, key, val);
}

Bool GetUnsignedIntFromRegistry(AsciiString path, AsciiString key, UnsignedInt& val)
{
    const char* env = std::getenv("GENERALS_VERSION");
    if(env)
    {
        val = static_cast<UnsignedInt>(std::strtoul(env, nullptr, 10));
        return TRUE;
    }
    return FALSE;
}

AsciiString GetRegistryLanguage(void)
{
    AsciiString val = "english";
    GetStringFromRegistry("", "Language", val);
    return val;
}

AsciiString GetRegistryGameName(void)
{
    AsciiString val = "GeneralsMPTest";
    GetStringFromRegistry("", "SKU", val);
    return val;
}

UnsignedInt GetRegistryVersion(void)
{
    UnsignedInt val = 65536;
    GetUnsignedIntFromRegistry("", "Version", val);
    return val;
}

UnsignedInt GetRegistryMapPackVersion(void)
{
    UnsignedInt val = 65536;
    GetUnsignedIntFromRegistry("", "MapPackVersion", val);
    return val;
}
