/* This file implements some of the functions described in
 * tads2/osifc.h.  We don't need to implement them all, as most of them
 * are provided by tads2/osnoui.c and tads2/osgen3.c.
 *
 * This file implements the "portable" subset of these functions;
 * functions that depend upon curses/ncurses are defined in oscurses.cc.
 */
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#ifdef HAVE_LANGINFO_CODESET
#include <langinfo.h>
#endif


#include "os.h"
#ifdef RUNTIME
// We utilize the Tads 3 Unicode character mapping facility even for
// Tads 2 (only in the interpreter).
#include "charmap.h"
// Need access to command line options: globalApp->options. Only at
// runtime - compiler uses different mechanism for command line
// options.
#include "frobtadsapp.h"
#endif // RUNTIME


/* Service routine.
 * Try to bring returned by system charset name to a name of one of
 * the encodings in cmaplib.t3r.  The proper way to do it is to
 * maintain an external database of charset aliases, which user can
 * edit.  But this adds unnecessary complexity, which we really don't
 * need.  So, the "database" is internal.
 *
 * TODO: Restore previous bahavior when we fix out curses interface.
 * For now, we always return "us-ascii".
 */
static const char*
get_charset_alias( const char* charset )
{
	const char* p;
	const char* c;

	// Special case.
	if (!stricmp(charset, "ANSI_X3.4-1968")) return "us-ascii";

	// If it's in ru_RU.KOI8-R form, try to extract charset part
	// (everything after last point).

	// Find the last point in the charset name.
	for (p = charset, c = charset ; *p != '\0' ; ++p) {
		if (*p == '.') c = p + 1;
	}

	// '.' was the last symbol in charset name?
	if (*c == '\0') c = charset;

	if (!stricmp(c, "KOI8-R")) return "koi8-r";

	if (!stricmp(c, "ISO-8859-1"))  return "iso1";
	if (!stricmp(c, "ISO-8859-2"))  return "iso2";
	if (!stricmp(c, "ISO-8859-3"))  return "iso3";
	if (!stricmp(c, "ISO-8859-4"))  return "iso4";
	if (!stricmp(c, "ISO-8859-5"))  return "iso5";
	if (!stricmp(c, "ISO-8859-6"))  return "iso6";
	if (!stricmp(c, "ISO-8859-7"))  return "iso7";
	if (!stricmp(c, "ISO-8859-8"))  return "iso8";
	if (!stricmp(c, "ISO-8859-9"))  return "iso9";
	if (!stricmp(c, "ISO-8859-10")) return "iso10";
	if (!stricmp(c, "8859-1"))      return "iso1";
	if (!stricmp(c, "8859-2"))      return "iso2";
	if (!stricmp(c, "8859-3"))      return "iso3";
	if (!stricmp(c, "8859-4"))      return "iso4";
	if (!stricmp(c, "8859-5"))      return "iso5";
	if (!stricmp(c, "8859-6"))      return "iso6";
	if (!stricmp(c, "8859-7"))      return "iso7";
	if (!stricmp(c, "8859-8"))      return "iso8";
	if (!stricmp(c, "8859-9"))      return "iso9";
	if (!stricmp(c, "8859-10"))     return "iso10";

	if (!stricmp(c, "CP1250"))  return "cp1250";
	if (!stricmp(c, "CP-1250")) return "cp1250";
	if (!stricmp(c, "CP_1250")) return "cp1250";
	if (!stricmp(c, "CP1251"))  return "cp1251";
	if (!stricmp(c, "CP-1251")) return "cp1251";
	if (!stricmp(c, "CP_1251")) return "cp1251";
	if (!stricmp(c, "CP1252"))  return "cp1252";
	if (!stricmp(c, "CP-1252")) return "cp1252";
	if (!stricmp(c, "CP_1252")) return "cp1252";
	if (!stricmp(c, "CP1253"))  return "cp1253";
	if (!stricmp(c, "CP-1253")) return "cp1253";
	if (!stricmp(c, "CP_1253")) return "cp1253";
	if (!stricmp(c, "CP1254"))  return "cp1254";
	if (!stricmp(c, "CP-1254")) return "cp1254";
	if (!stricmp(c, "CP_1254")) return "cp1254";
	if (!stricmp(c, "CP1255"))  return "cp1255";
	if (!stricmp(c, "CP-1255")) return "cp1255";
	if (!stricmp(c, "CP_1255")) return "cp1255";
	if (!stricmp(c, "CP1256"))  return "cp1256";
	if (!stricmp(c, "CP-1256")) return "cp1256";
	if (!stricmp(c, "CP_1256")) return "cp1256";
	if (!stricmp(c, "CP1257"))  return "cp1257";
	if (!stricmp(c, "CP-1257")) return "cp1257";
	if (!stricmp(c, "CP_1257")) return "cp1257";
	if (!stricmp(c, "CP1258"))  return "cp1258";
	if (!stricmp(c, "CP-1258")) return "cp1258";
	if (!stricmp(c, "CP_1258")) return "cp1258";

	if (!stricmp(c, "CP437"))  return "cp437";
	if (!stricmp(c, "CP-437")) return "cp437";
	if (!stricmp(c, "CP_437")) return "cp437";
	if (!stricmp(c, "PC437"))  return "cp437";
	if (!stricmp(c, "PC-437")) return "cp437";
	if (!stricmp(c, "PC_437")) return "cp437";
	if (!stricmp(c, "CP737"))  return "cp737";
	if (!stricmp(c, "CP-737")) return "cp737";
	if (!stricmp(c, "CP_737")) return "cp737";
	if (!stricmp(c, "PC737"))  return "cp737";
	if (!stricmp(c, "PC-737")) return "cp737";
	if (!stricmp(c, "PC_737")) return "cp737";
	if (!stricmp(c, "CP775"))  return "cp775";
	if (!stricmp(c, "CP-775")) return "cp775";
	if (!stricmp(c, "CP_775")) return "cp775";
	if (!stricmp(c, "PC775"))  return "cp775";
	if (!stricmp(c, "PC-775")) return "cp775";
	if (!stricmp(c, "PC_775")) return "cp775";
	if (!stricmp(c, "CP850"))  return "cp850";
	if (!stricmp(c, "CP-850")) return "cp850";
	if (!stricmp(c, "CP_850")) return "cp850";
	if (!stricmp(c, "PC850"))  return "cp850";
	if (!stricmp(c, "PC-850")) return "cp850";
	if (!stricmp(c, "PC_850")) return "cp850";
	if (!stricmp(c, "CP852"))  return "cp852";
	if (!stricmp(c, "CP-852")) return "cp852";
	if (!stricmp(c, "CP_852")) return "cp852";
	if (!stricmp(c, "PC852"))  return "cp852";
	if (!stricmp(c, "PC-852")) return "cp852";
	if (!stricmp(c, "PC_852")) return "cp852";
	if (!stricmp(c, "CP855"))  return "cp855";
	if (!stricmp(c, "CP-855")) return "cp855";
	if (!stricmp(c, "CP_855")) return "cp855";
	if (!stricmp(c, "PC855"))  return "cp855";
	if (!stricmp(c, "PC-855")) return "cp855";
	if (!stricmp(c, "PC_855")) return "cp855";
	if (!stricmp(c, "CP857"))  return "cp857";
	if (!stricmp(c, "CP-857")) return "cp857";
	if (!stricmp(c, "CP_857")) return "cp857";
	if (!stricmp(c, "PC857"))  return "cp857";
	if (!stricmp(c, "PC-857")) return "cp857";
	if (!stricmp(c, "PC_857")) return "cp857";
	if (!stricmp(c, "CP860"))  return "cp860";
	if (!stricmp(c, "CP-860")) return "cp860";
	if (!stricmp(c, "CP_860")) return "cp860";
	if (!stricmp(c, "PC860"))  return "cp860";
	if (!stricmp(c, "PC-860")) return "cp860";
	if (!stricmp(c, "PC_860")) return "cp860";
	if (!stricmp(c, "CP861"))  return "cp861";
	if (!stricmp(c, "CP-861")) return "cp861";
	if (!stricmp(c, "CP_861")) return "cp861";
	if (!stricmp(c, "PC861"))  return "cp861";
	if (!stricmp(c, "PC-861")) return "cp861";
	if (!stricmp(c, "PC_861")) return "cp861";
	if (!stricmp(c, "CP862"))  return "cp862";
	if (!stricmp(c, "CP-862")) return "cp862";
	if (!stricmp(c, "CP_862")) return "cp862";
	if (!stricmp(c, "PC862"))  return "cp862";
	if (!stricmp(c, "PC-862")) return "cp862";
	if (!stricmp(c, "PC_862")) return "cp862";
	if (!stricmp(c, "CP863"))  return "cp863";
	if (!stricmp(c, "CP-863")) return "cp863";
	if (!stricmp(c, "CP_863")) return "cp863";
	if (!stricmp(c, "PC863"))  return "cp863";
	if (!stricmp(c, "PC-863")) return "cp863";
	if (!stricmp(c, "PC_863")) return "cp863";
	if (!stricmp(c, "CP864"))  return "cp864";
	if (!stricmp(c, "CP-864")) return "cp864";
	if (!stricmp(c, "CP_864")) return "cp864";
	if (!stricmp(c, "PC864"))  return "cp864";
	if (!stricmp(c, "PC-864")) return "cp864";
	if (!stricmp(c, "PC_864")) return "cp864";
	if (!stricmp(c, "CP865"))  return "cp865";
	if (!stricmp(c, "CP-865")) return "cp865";
	if (!stricmp(c, "CP_865")) return "cp865";
	if (!stricmp(c, "PC865"))  return "cp865";
	if (!stricmp(c, "PC-865")) return "cp865";
	if (!stricmp(c, "PC_865")) return "cp865";
	if (!stricmp(c, "CP866"))  return "cp866";
	if (!stricmp(c, "CP-866")) return "cp866";
	if (!stricmp(c, "CP_866")) return "cp866";
	if (!stricmp(c, "PC866"))  return "cp866";
	if (!stricmp(c, "PC-866")) return "cp866";
	if (!stricmp(c, "PC_866")) return "cp866";
	if (!stricmp(c, "CP869"))  return "cp869";
	if (!stricmp(c, "CP-869")) return "cp869";
	if (!stricmp(c, "CP_869")) return "cp869";
	if (!stricmp(c, "PC869"))  return "cp869";
	if (!stricmp(c, "PC-869")) return "cp869";
	if (!stricmp(c, "PC_869")) return "cp869";
	if (!stricmp(c, "CP874"))  return "cp874";
	if (!stricmp(c, "CP-874")) return "cp874";
	if (!stricmp(c, "CP_874")) return "cp874";
	if (!stricmp(c, "PC874"))  return "cp874";
	if (!stricmp(c, "PC-874")) return "cp874";
	if (!stricmp(c, "PC_874")) return "cp874";

	// There are Mac OS Roman, Central European, Cyrillic, Greek,
	// Icelandic and Turkish charmaps in cmaplib.t3r. But what codes
	// system supposed to return?

	// Unrecognized.
	return charset;
}


/* Open text file for reading and writing.
 */
osfildef*
osfoprwt( const char* fname, os_filetype_t )
{
//	assert(fname != 0);
	// Try opening the file in read/write mode.
	osfildef* fp = fopen(fname, "r+");
	// If opening the file failed, it probably means that it doesn't
	// exist.  In that case, create a new file in read/write mode.
	if (fp == 0) fp = fopen(fname, "w+");
	return fp;
}


/* Open binary file for reading/writing.
 */
osfildef*
osfoprwb( const char* fname, os_filetype_t )
{
//	assert(fname != 0);
	osfildef* fp = fopen(fname, "r+b");
	if (fp == 0) fp = fopen(fname, "w+b");
	return fp;
}


/* Convert string to all-lowercase.
 */
char*
os_strlwr( char* s )
{
//	assert(s != 0);
	for (int i = 0; s[i] != '\0'; ++i) {
		s[i] = tolower(s[i]);
	}
	return s;
}


/* Seek to the resource file embedded in the current executable file.
 *
 * We don't support this (and probably never will).
 */
osfildef*
os_exeseek( const char*, const char* )
{
	return static_cast(osfildef*)(0);
}


/* Create and open a temporary file.
 */
osfildef*
os_create_tempfile( const char* fname, char* buf )
{
	if (fname != 0 and fname[0] != '\0') {
		// A filename has been specified; use it.
		return fopen(fname, "w+b");
	}

	//ASSERT(buf != 0);

	// No filename needed; create a nameless tempfile.
	buf[0] = '\0';
	return tmpfile();
}


/* Delete a temporary file created with os_create_tempfile().
 */
int
osfdel_temp( const char* fname )
{
	//ASSERT(fname != 0);

	if (fname[0] == '\0' or remove(fname) == 0) {
		// Either it was a nameless temp-file and has been
		// already deleted by the system, or deleting it
		// succeeded.  In either case, the operation was
		// successful.
		return 0;
	}
	// It was not a nameless tempfile and remove() failed.
	return 1;
}


/* Get the temporary file path.
 *
 * tads2/osnoui.c defines a DOS version of this routine when MSDOS is
 * defined.
 */
#ifndef MSDOS
void
os_get_tmp_path( char* buf )
{
	// Try the usual env. variable first.
	const char* tmpDir = getenv("TMPDIR");

	// If no such variable exists, try P_tmpdir (if defined in
	// <stdio.h>).
#ifdef P_tmpdir
	if (tmpDir == 0 or tmpDir[0] == '\0') {
		tmpDir = P_tmpdir;
	}
#endif

	// If we still lack a path, use "/tmp".
	if (tmpDir == 0 or tmpDir[0] == '\0') {
		tmpDir = "/tmp";
	}

	// If the directory doesn't exist or is not a directory, don't
	// provide anything at all (which means that the current
	// directory will be used).
	struct stat inf;
	int statRet = stat(tmpDir, &inf);
	if (statRet != 0 or (inf.st_mode & S_IFMT) != S_IFDIR) {
		buf[0] = '\0';
		return;
	}

	strcpy(buf, tmpDir);

	// Append a slash if necessary.
	size_t len = strlen(buf);
	if (buf[len - 1] != '/') {
		buf[len] = '/';
		buf[len + 1] = '\0';
	}
}
#endif // not MSDOS


/* Get a suitable seed for a random number generator.
 *
 * We don't just use the system-clock, but build a number as random as
 * the mechanisms of the standard C-library allow.  This is somewhat of
 * an overkill, but it's simple enough so here goes.  Someone has to
 * write a nuclear war simulator in TADS to test this thoroughly.
 */
void
os_rand( long* val )
{
	//assert(val != 0);
	// Use the current time as the first seed.
	srand(static_cast(unsigned int)(time(0)));

	// Create the final seed by generating a random number using
	// high-order bits, because on some systems the low-order bits
	// aren't very random.
	*val = 1 + static_cast(long)(static_cast(long double)(65535) * rand() / (RAND_MAX + 1.0));
}


/* Get the current system high-precision timer.
 *
 * We provide four (4) implementations of this function:
 *
 *   1. clock_gettime() is available
 *   2. gettimeofday() is available
 *   3. ftime() is available
 *   4. Neither is available - fall back to time()
 *
 * Note that HAVE_CLOCK_GETTIME, HAVE_GETTIMEOFDAY and HAVE_FTIME are
 * mutually exclusive; if one of them is defined, the others aren't.  No
 * need for #else here.
 *
 * Although not required by the TADS VM, these implementations will
 * always return 0 on the first call.
 */
#ifdef HAVE_CLOCK_GETTIME
// The system has the clock_gettime() function.
long
os_get_sys_clock_ms( void )
{
	// We need to remember the exact time this function has been
	// called for the first time, and use that time as our
	// zero-point.  On each call, we simply return the difference
	// in milliseconds between the current time and our zero point.
	static struct timespec zeroPoint;

	// Did we get the zero point yet?
	static bool initialized = false;

	// Not all systems provide a monotonic clock; check if it's
	// available before falling back to the global system-clock.  A
	// monotonic clock is guaranteed not to change while the system
	// is running, so we prefer it over the global clock.
	static const clockid_t clockType =
#ifdef HAVE_CLOCK_MONOTONIC
		CLOCK_MONOTONIC;
#else
		CLOCK_REALTIME;
#endif
	// We must get the current time in each call.
	struct timespec currTime;

	// Initialize our zero-point, if not already done so.
	if (not initialized) {
		clock_gettime(clockType, &zeroPoint);
		initialized = true;
	}

	// Get the current time.
	clock_gettime(clockType, &currTime);

	// Note that tv_nsec contains *nano*seconds, not milliseconds,
	// so we need to convert it; a millisec is 1.000.000 nanosecs.
	return (currTime.tv_sec - zeroPoint.tv_sec) * 1000
	     + (currTime.tv_nsec - zeroPoint.tv_nsec) / 1000000;
}
#endif // HAVE_CLOCK_GETTIME

#ifdef HAVE_GETTIMEOFDAY
// The system has the gettimeofday() function.
long
os_get_sys_clock_ms( void )
{
	static struct timeval zeroPoint;
	static bool initialized = false;
	// gettimeofday() needs the timezone as a second argument.  This
	// is obsolete today, but we don't care anyway; we just pass
	// something.
	struct timezone bogus = {0, 0};
	struct timeval currTime;

	if (not initialized) {
		gettimeofday(&zeroPoint, &bogus);
		initialized = true;
	}

	gettimeofday(&currTime, &bogus);

	// 'tv_usec' contains *micro*seconds, not milliseconds.  A
	// millisec is 1.000 microsecs.
	return (currTime.tv_sec - zeroPoint.tv_sec) * 1000 + (currTime.tv_usec - zeroPoint.tv_usec) / 1000;
}
#endif // HAVE_GETTIMEOFDAY


#ifdef HAVE_FTIME
// The system has the ftime() function.  ftime() is in <sys/timeb.h>.
#include <sys/timeb.h>
long
os_get_sys_clock_ms( void )
{
	static timeb zeroPoint;
	static bool initialized = false;
	struct timeb currTime;

	if (not initialized) {
		ftime(&zeroPoint);
		initialized = true;
	}

	ftime(&currTime);
	return (currTime.time - zeroPoint.time) * 1000 + (currTime.millitm - zeroPoint.millitm);
}
#endif // HAVE_FTIME


#ifndef HAVE_CLOCK_GETTIME
#ifndef HAVE_GETTIMEOFDAY
#ifndef HAVE_FTIME
// Use the standard time() function from the C library.  We lose
// millisecond-precision, but that's the best we can do with time().
long
os_get_sys_clock_ms( void )
{
	static time_t zeroPoint = time(0);
	return (time(0) - zeroPoint) * 1000;
}
#endif
#endif
#endif


/* Get filename from startup parameter, if possible.
 *
 * TODO: Find out what this is supposed to do.
 */
int
os_paramfile( char* )
{
	return 0;
}


/* Get a special directory path.
 *
 * If env. variables exist, they always override the compile-time
 * defaults.
 *
 * We ignore the argv parameter, since on Unix binaries aren't stored
 * together with data on disk.
 */
void
os_get_special_path( char* buf, size_t buflen, const char*, int id )
{
	const char* res;

	switch (id) {
	  case OS_GSP_T3_RES:
		res = getenv("T3_RESDIR");
		if (res == 0 or res[0] == '\0') {
			res = T3_RES_DIR;
		}
		break;
	  case OS_GSP_T3_INC:
		res = getenv("T3_INCDIR");
		if (res == 0 or res[0] == '\0') {
			res = T3_INC_DIR;
		}
		break;
	  case OS_GSP_T3_LIB:
		res = getenv("T3_LIBDIR");
		if (res == 0 or res[0] == '\0') {
			res = T3_LIB_DIR;
		}
		break;
	  case OS_GSP_T3_USER_LIBS:
		// There's no compile-time default for user libs.
		res = getenv("T3_USERLIBDIR");
		break;
	  default:
		// TODO: We could print a warning here to inform the
		// user that we're outdated.
		res = 0;
	}

	if (res != 0) {
		// Only use the detected path if it exists and is a
		// directory.
		struct stat inf;
		int statRet = stat(res, &inf);
		if (statRet == 0 and (inf.st_mode & S_IFMT) == S_IFDIR) {
			strncpy(buf, res, buflen - 1);
			return;
		}
	}
	// Indicate failure.
	buf[0] = '\0';
}


/* Generate a filename for a character-set mapping file.
 *
 * Follow DOS convention: start with the current local charset
 * identifier, then the internal ID, and the suffix ".tcp".  No path
 * prefix, which means look in current directory.  This is what we
 * want, because mapping files are supposed to be distributed with a
 * game, not with the interpreter.
 */
void
os_gen_charmap_filename( char* filename, char* internal_id, char* /*argv0*/ )
{
	char mapname[32];

	os_get_charmap(mapname, OS_CHARMAP_DISPLAY);

	// Theoretically, we can get mapname so long that with 4-letter
	// internal id and 4-letter extension '.tcp' it will be longer
	// than OSFNMAX. Highly unlikely, but...
	size_t len = strlen(mapname);
	if (len > OSFNMAX - 9) len = OSFNMAX - 9;

	memcpy(filename, mapname, len);
	strcat(filename, internal_id);
	strcat(filename, ".tcp");
}


/* Receive notification that a character mapping file has been loaded.
 */
void
os_advise_load_charmap( char* /*id*/, char* /*ldesc*/, char* /*sysinfo*/ )
{
}


/* Get the full filename (including directory path) to the executable
 * file, given the argv[0] parameter passed into the main program.
 *
 * On Unix, you can't tell what the executable's name is by just looking
 * at argv, so we always indicate failure.  No big deal though; this
 * information is only used when the interpreter's executable is bundled
 * with a game, and we don't support this at all.
 */
int
os_get_exe_filename( char*, size_t, const char* )
{
	return false;
}


/* Generate the name of the character set mapping table for Unicode
 * characters to and from the given local character set.
 */
void
os_get_charmap( char* mapname, int charmap_id )
{
	const char* charset = 0;  // Character set name.

#ifdef RUNTIME
	// If there was a command line option, it takes precedence.
	// User knows better, so do not try to modify his setting.
	if (globalApp->options.characterSet[0] != '\0') {
		// One charset for all.
		strncpy(mapname, globalApp->options.characterSet, 32);
		return;
	}
#endif

	// There is absolutely no robust way to determine the local
	// character set.  Roughly speaking, we have three options:
	//
	// Use nl_langinfo() function.  Not always available.
	//
	// Use setlocale(LC_CTYPE, NULL).  This only works if user set
	// locale which is actually installed on the machine, or else
	// it will return NULL.  But we don't need locale, we just need
	// to know what the user wants.
	//
	// Manually look up environment variables LC_ALL, LC_CTYPE and
	// LANG.
	//
	// However, not a single one will provide us with a reliable
	// name of local character set.  There is no standard way to
	// name charsets.  For a single set we can get almost anything:
	// Windows-1251, Win-1251, CP1251, CP-1251, ru_RU.CP1251,
	// ru_RU.CP-1251, ru_RU.CP_1251...  And the only way is to
	// maintain a database of aliases.

#if HAVE_LANGINFO_CODESET
	charset = nl_langinfo(CODESET);
#else
	charset = getenv("LC_CTYPE");
	if (charset == 0 or charset[0] == '\0') {
		charset = getenv("LC_ALL");
		if (charset == 0 or charset[0] == '\0') {
			charset = getenv("LANG");
		}
	}
#endif

	if (charset == 0) {
		strcpy(mapname, "us-ascii");
	} else {
		strcpy(mapname, get_charset_alias(charset));
	}
	return;
}


/* Translate a character from the HTML 4 Unicode character set to the
 * current character set used for display.
 *
 * Note that this function is used only by Tads 2.  Tads 3 does mappings
 * automatically.
 *
 * We omit this implementation when not compiling the interpreter (in
 * which case os_xlat_html4 will have been defined as an empty macro in
 * osfrobtads.h).  We do this because we don't want to introduce any
 * TADS 3 dependencies upon the TADS 2 compiler, which should compile
 * regardless of whether the TADS 3 sources are available or not.
 */
#ifndef os_xlat_html4
void
os_xlat_html4( unsigned int html4_char, char* result, size_t result_buf_len )
{
	// HTML 4 characters are Unicode.  Tads 3 provides just the
	// right mapper: Unicode to ASCII.  We make it static in order
	// not to create a mapper on each call and save CPU cycles.
	static CCharmapToLocalASCII mapper;
	result[mapper.map_char(html4_char, result, result_buf_len)] = '\0';
}
#endif


/* =====================================================================
 *
 * The functions defined below are not needed by the interpreter, or
 * have a curses-specific implementation and are therefore only used
 * when building the compiler (the compiler doesn't use curses, just
 * plain stdio).
 */
#ifndef RUNTIME

/* Wait for a character to become available from the keyboard.
 */
void
os_waitc( void )
{
	getchar();
}


/* Read a character from the keyboard.
 */
int
os_getc( void )
{
	return getchar();
}


/* Read a character from the keyboard and return the low-level,
 * untranslated key code whenever possible.
 */
int
os_getc_raw( void )
{
	return getchar();
}


/* Check for user break.
 *
 * We only provide a dummy here; we might do something more useful in
 * the future though.
 */
int
os_break( void )
{
	return false;
}


/* Yield CPU.
 *
 * We don't need this.  It's only useful for Windows 3.x and maybe pre-X Mac OS
 * and other systems with brain damaged multitasking.
 */
int
os_yield( void )
{
	return false;
}


/* Sleep for a while.
 *
 * The compiler doesn't need this.
 */
void
os_sleep_ms( long )
{
}

#endif /* ! RUNTIME */
