#include "common.h"
#include "missing.h"

#include <memory>
#include <cctype>
#include <cstring>

#if not HAVE_MEMICMP
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


#if not HAVE_STRICMP
int
stricmp( const char* s1, const char* s2 )
{
#if HAVE_STRCASECMP
    return strcasecmp(s1, s2);
#else
    const auto x1 = std::make_unique<char[]>(strlen(s1));
    const auto x2 = std::make_unique<char[]>(strlen(s2));

    for (size_t i = 0; s1[i] != '\0' and s2[i] != '\0'; ++i) {
        x1[i] = tolower(s1[i]);
        x2[i] = tolower(s2[i]);
    }
    return strcmp(x1.get(), x2.get());
#endif
}
#endif


#if not HAVE_STRNICMP
int
strnicmp( const char* s1, const char* s2, size_t n )
{
#if HAVE_STRNCASECMP
    return strncasecmp(s1, s2, n);
#else
    const auto x1 = std::make_unique<char[]>(n);
    const auto x2 = std::make_unique<char[]>(n);

    for (size_t i = 0; i < n and s1[i] != '\0' and s2[i] != '\0'; ++i) {
        x1[i] = tolower(s1[i]);
        x2[i] = tolower(s2[i]);
    }
    return strncmp(x1.get(), x2.get(), n);
#endif
}
#endif
