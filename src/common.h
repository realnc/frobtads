#ifndef COMMON_H
#define COMMON_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* ushort, uint, and ulong are usually in <sys/types.h>
 */
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

/* Check for the existence of uchar, ushort, uint and ulong and define
 * the appropriate OS_*_DEFINED macros.
 */
#ifdef __cplusplus
#ifdef CXX_UCHAR_DEFINED
#define OS_UCHAR_DEFINED
#endif
#ifdef CXX_USHORT_DEFINED
#define OS_USHORT_DEFINED
#endif
#ifdef CXX_UINT_DEFINED
#define OS_UINT_DEFINED
#endif
#ifdef CXX_ULONG_DEFINED
#define OS_ULONG_DEFINED
#endif
#else /* !__cplusplus */
#ifdef C_UCHAR_DEFINED
#define OS_UCHAR_DEFINED
#endif
#ifdef C_USHORT_DEFINED
#define OS_USHORT_DEFINED
#endif
#ifdef C_UINT_DEFINED
#define OS_UINT_DEFINED
#endif
#ifdef C_ULONG_DEFINED
#define OS_ULONG_DEFINED
#endif
#endif /* __cplusplus */

/* The intention is to work around a bug on old systems where we cannot
 * include both <time.h> and <sys/time.h>.  Anyway, for now, this does
 * not work, since the TADS base code includes <time.h> on its own.
 * This may be fixed in the future though.
 */
#ifdef __cplusplus
extern "C" {
#endif
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */
