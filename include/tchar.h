#ifndef PORTABLE_TCHAR_H
#define PORTABLE_TCHAR_H

#ifdef _WIN32
# include <tchar.h>
#else
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <ctype.h>

typedef char TCHAR;

#ifndef _T
# define _T(x) x
#endif

#define _tfopen fopen
#define _tcslen strlen
#define _tcsclen strlen
#define _tcscmp strcmp
#define _tcsicmp strcasecmp
#define _tcscpy strcpy
#define _tcsncpy strncpy
#define _tcscat strcat
#define _tcschr strchr
#define _tcsrchr strrchr
#endif /* _WIN32 */

#endif /* PORTABLE_TCHAR_H */
