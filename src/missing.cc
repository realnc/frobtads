#include "common.h"
#include "missing.h"

#include <memory>
#include <cctype>
#include <cstring>

#ifndef HAVE_MEMICMP
int
memicmp( const void* s1, const void* s2, size_t len )
{
    const auto x1 = std::make_unique<char[]>(len);
    const auto x2 = std::make_unique<char[]>(len);

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
    const auto x1 = std::make_unique<char[]>(strlen(s1));
    const auto x2 = std::make_unique<char[]>(strlen(s2));

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
    const auto x1 = std::make_unique<char[]>(n);
    const auto x2 = std::make_unique<char[]>(n);

    for (size_t i = 0; i < n and s1[i] != '\0' and s2[i] != '\0'; ++i) {
        x1[i] = tolower(s1[i]);
        x2[i] = tolower(s2[i]);
    }
    return strncmp(x1.get(), x2.get(), n);
}
#endif
