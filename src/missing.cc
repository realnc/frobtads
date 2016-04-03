#include "common.h"
#include "missing.h"

#include <memory>
#include <cctype>
#include <cstring>

#ifndef HAVE_MEMICMP
int
memicmp( const void* s1, const void* s2, size_t len )
{
    auto x1 = std::make_unique<char[]>(len);
    auto x2 = std::make_unique<char[]>(len);

    for (size_t i = 0; i < len; ++i) {
        x1[i] = tolower(static_cast<const char*>(s1)[i]);
        x2[i] = tolower(static_cast<const char*>(s2)[i]);
    }
    return memcmp(x1.get(), x2.get(), len);
}
#endif


#if !defined(HAVE_STRICMP) && !defined(HAVE_STRCASECMP)
int
stricmp( const char* s1, const char* s2 )
{
    auto x1 = std::make_unique<char[]>(strlen(s1));
    auto x2 = std::make_unique<char[]>(strlen(s2));

    for (size_t i = 0; s1[i] != '\0' and s2[i] != '\0'; ++i) {
        x1[i] = tolower(s1[i]);
        x2[i] = tolower(s2[i]);
    }
    return strcmp(x1.get(), x2.get());
}
#endif


#if !defined(HAVE_STRNICMP) && !defined(HAVE_STRNCASECMP)
int
strnicmp( const char* s1, const char* s2, size_t n )
{
    auto x1 = std::make_unique<char[]>(n);
    auto x2 = std::make_unique<char[]>(n);

    for (size_t i = 0; i < n and s1[i] != '\0' and s2[i] != '\0'; ++i) {
        x1[i] = tolower(s1[i]);
        x2[i] = tolower(s2[i]);
    }
    return strncmp(x1.get(), x2.get(), n);
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
