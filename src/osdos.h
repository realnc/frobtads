/* This gets included if we build FrobTADS in MS-DOG (and other DOGs).
 *
 * We don't actually support DOS; this file exists because of some
 * reason I don't fully comprehend...
 */
#ifndef OSDOS_H
#define OSDOS_H

#ifndef MSDOS
#define MSDOS
#endif

/* Normal path separator character.
 * DOS uses backslash rather than slash.*/
#define OSPATHCHAR '\\'

/* Other path separator characters. */
#define OSPATHALT "/:"

/* Path separator characters for URL conversions.
 * URL path separators - do not include ":", since we don't want to
 * convert this to a "/" in a URL */
#define OSPATHURL "\\/"

/* ASCII string giving the local newline sequence to write on output. */
#define OS_NEWLINE_SEQ  "\r\n"

/* This is only used by the Tads 3 compiler; we don't need it. */
/* Directory separator for PATH-style environment variables. */
/*#define OSPATHSEP ';'*/

/* The rest stays as-is. */
#include "osfrobtads.h"

#endif /* OSDOS_H */
