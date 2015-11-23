#include "common.h"

#include "missing.h"

#include <ctype.h>
#include <string.h>

#ifndef HAVE_MEMICMP
int
memicmp( const void* s1, const void* s2, size_t len )
{
    char* x1 = new char[len];
    char* x2 = new char[len];
    const char* tmp1 = static_cast<const char*>(s1);
    const char* tmp2 = static_cast<const char*>(s2);

    for (size_t i = 0; i < len; ++i) {
        x1[i] = tolower(tmp1[i]);
        x2[i] = tolower(tmp2[i]);
    }
    int ret = memcmp(x1, x2, len);
    delete[] x1;
    delete[] x2;
    return ret;
}
#endif


#if !defined(HAVE_STRICMP) && !defined(HAVE_STRCASECMP)
int
stricmp( const char* s1, const char* s2 )
{
    char* x1 = new char[strlen(s1)];
    char* x2 = new char[strlen(s2)];

    for (size_t i = 0; s1[i] != '\0' and s2[i] != '\0'; ++i) {
        x1[i] = tolower(s1[i]);
        x2[i] = tolower(s2[i]);
    }
    int ret = strcmp(x1, x2);
    delete[] x1;
    delete[] x2;
    return ret;
}
#endif


#if !defined(HAVE_STRNICMP) && !defined(HAVE_STRNCASECMP)
int
strnicmp( const char* s1, const char* s2, size_t n )
{
    char* x1 = new char[n];
    char* x2 = new char[n];

    for (size_t i = 0; i < n and s1[i] != '\0' and s2[i] != '\0'; ++i) {
        x1[i] = tolower(s1[i]);
        x2[i] = tolower(s2[i]);
    }
    int ret = strncmp(s1, s2, n);
    delete[] x1;
    delete[] x2;
    return ret;
}
#endif


#ifndef HAVE_WCSLEN
size_t
wcslen( wchar_t* s )
{
    size_t len = 0;
    while (*s++)
        ++len;
    return len;
}
#endif

/* Copy a wide character string.
 */
#ifndef HAVE_WCSCPY
wchar_t*
wcscpy( wchar_t* dest, const wchar_t* src )
{
    wchar_t *start = dest;
    while ((*dest++ = *src++) != 0)
        ;
    return start;
}
#endif
