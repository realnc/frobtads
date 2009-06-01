/* This file implements some of the functions described in
 * tads2/osifc.h.  Only functions needed by the TADS 3 compiler are
 * implemented here.
 *
 * The functions are "portable"; they don't make use of curses/ncurses.
 */
#include "common.h"

#include <stdio.h>
#include <string.h>
extern "C" {
#include <sys/stat.h>
#include <dirent.h>
}

#ifdef HAVE_GLOB_H
#include <glob.h>
#endif

#include "os.h"


/* Set the game title.
 *
 * Does nothing in the compiler.  This should actually be implemented in
 * tads3/os_stdio.cpp, but for some reason it's not.
 */
void
os_set_title( const char* )
{
}


/* Get the modification time for a file.
 */
int
os_get_file_mod_time( os_file_time_t* t, const char* fname )
{
	struct stat inf;

	if (stat(fname, &inf) != 0) return 1;
	t->t = inf.st_mtime;
	return 0;
}


/* Compare two file time values.
 */
int
os_cmp_file_times( const os_file_time_t* a, const os_file_time_t* b )
{
	if (a->t < b->t) return -1;
	if (a->t == b->t) return 0;
	return 1;
}


/* Install/uninstall the break handler.
 *
 * I don't think we need a break handler.
 */
void
os_instbrk( int )
{
}


/* We implement the file searching routines using <glob.h>.  It's not
 * available on all systems though, so we provide dummy fallbacks that
 * do nothing if <glob.h> doesn't exist.
 */
#ifdef HAVE_GLOB
/* Search context structure.
 */
struct oss_find_ctx_t
{
	glob_t* pglob;
	// Index of current filename in pglob->gl_pathv.
	size_t current;
	// Original search path prefix (we'll allocate more to fit the
	// string).
	char path[1];
};


/* Service routine for searching - build the full output path name.
 */
static void
oss_build_outpathbuf( char* outpathbuf, size_t outpathbufsiz, const char* path,
                      const char* fname )
{
	// If there's a full path buffer, build the full path.
	if (outpathbuf != 0) {
		size_t lp;
		size_t lf;

		// Copy the path prefix.
		lp = strlen(path);
		if (lp > outpathbufsiz - 1) {
			lp = outpathbufsiz - 1;
		}
		memcpy(outpathbuf, path, lp);

		// Add the filename if there's any room.
		lf = strlen(fname);
		if (lf > outpathbufsiz - lp - 1) {
			lf = outpathbufsiz - lp - 1;
		}
		memcpy(outpathbuf + lp, fname, lf);

		// Null-terminate the result.
		outpathbuf[lp + lf] = '\0';
	}
}


/* Find the first file matching a given pattern.
 */
void*
os_find_first_file( const char* dir, const char* pattern, char* outbuf, size_t outbufsiz,
                    int* isdir, char* outpathbuf, size_t outpathbufsiz)
{
	char realpat[OSFNMAX];
	size_t l;
	size_t path_len;
	const char* lastsep;
	const char* p;
	struct oss_find_ctx_t* ctx = 0;

	strcpy(realpat, dir);
	if ((l = strlen(realpat)) != 0 && realpat[l - 1] != '/') {
		realpat[l++] = '/';
	}

	if (pattern == 0) {
		strcpy(realpat + l, "*");
	} else {
		strcpy(realpat + l, pattern);
	}

	// Find the last separator in the original path.
	for (p = realpat, lastsep = 0 ; *p != '\0' ; ++p) {
		// If this is a separator, remember it.
		if (*p == '/') lastsep = p;
	}

	// If we found a separator, the path prefix is everything up to
	// and including the separator; otherwise, there's no path
	// prefix.
	if (lastsep != 0) {
		// The length of the path includes the separator.
		path_len = lastsep + 1 - realpat;
	} else {
		// There's no path prefix - everything is in the
		// current directory.
		path_len = 0;
	}

	// Allocate a context.
	ctx = static_cast(struct oss_find_ctx_t*)(malloc(sizeof(struct oss_find_ctx_t) + path_len));
	ctx->pglob = static_cast(glob_t *)(malloc(sizeof(glob_t)));
	ctx->current = -1;

	// Copy the path to the context. we may need it later to build
	// a full path.
	memcpy(ctx->path, realpat, path_len);
	ctx->path[path_len] = '\0';

	// Problem: before calling glob() we need to escape all [] in
	// realpat.  Count them first.
	int nbracket = 0;
	for (p = realpat; *p != '\0' ; ++p) {
		if (*p == '[' || *p == ']') ++nbracket;
	}

	// Allocate string for escaped pattern.
	char* escaped_realpat = static_cast(char*)(malloc(strlen(realpat) + nbracket + 1));

	// Copy pattern to new string, escaping all [ and ] with \.
	char* c = realpat;
	char* k = escaped_realpat;
	while (*c != '\0') {
		if (*c == '[' || *c == ']') {
			*k++ = '\\';
		}
		*k++ = *c++;
	}
	*k++ = '\0';

	int res = glob(escaped_realpat, GLOB_ERR | GLOB_MARK, NULL, ctx->pglob);
	if (res != 0) {
		// We don't really care what kind of error occured.
		// Just free the resources and return error. Note that
		// "no files found" is an error.
		if (ctx->pglob != 0) {
			globfree(ctx->pglob);
			free(ctx->pglob);
		}
		if (ctx != 0) {
			free(ctx);
		}
		free(escaped_realpat);
		*isdir = 0;
		return 0;
	}

	// If we get there, there was a match. set current index for
	// os_find_next().
	ctx->current = 0;

	// glob() returns filename with directory, but we need just the
	// basename. Copy first filename into the caller's buffer.
	char* ptr = &ctx->pglob->gl_pathv[0][path_len];
	l = strlen(ptr);

	if (l > outbufsiz - 1) {
		l = outbufsiz - 1;
	}
	memcpy(outbuf, ptr, l);
	outbuf[l] = '\0';

	// Build the full path, if desired.
	oss_build_outpathbuf(outpathbuf, outpathbufsiz, ctx->path, ptr);

	// Return the directory indication.
	*isdir = (ptr[l-1] == '/') ? true : false;

	free(escaped_realpat);
	return ctx;
}

/* Find the next matching file, continuing a search started with
 * os_find_first_file().
 */
void*
os_find_next_file( void* ctx0, char* outbuf, size_t outbufsiz, int* isdir, char* outpathbuf,
                   size_t outpathbufsiz )
{
	struct oss_find_ctx_t* ctx = static_cast(struct oss_find_ctx_t*)(ctx0);
	size_t l;

	// If the search has already ended, do nothing.
	if (ctx == 0) return 0;

	if (++ctx->current > ctx->pglob->gl_pathc - 1) {
		// No more files - delete the context and give up.
		globfree(ctx->pglob);
		free(ctx->pglob);
		free(ctx);
		// Indicate that the search is finished.
		return 0;
	}

	// Return the name. See comments in os_find_first_file().
	l = strlen(ctx->path);
	char* ptr = &ctx->pglob->gl_pathv[ctx->current][l];

	l = strlen(ptr);

	if (l > outbufsiz - 1) {
		l = outbufsiz - 1;
	}
	memcpy(outbuf, ptr, l);
	outbuf[l] = '\0';

	// Return the directory indication.
	*isdir = (ptr[l-1] == '/') ? true : false;

	// Build the full path, if desired.
	oss_build_outpathbuf(outpathbuf, outpathbufsiz, ctx->path, ptr);

	// Indicate that the search was successful by returning the
	// non-null context pointer -- the context has been updated for
	// the new position in the search, so we just return the same
	// context structure that we've been using all along.
	return ctx;
}

/* Cancel a search.
 */
void
os_find_close( void* ctx0 )
{
	struct oss_find_ctx_t* ctx = static_cast(struct oss_find_ctx_t*)(ctx0);

	// If the search was already cancelled, do nothing.
	if (ctx == 0) return;

	// Delete the search context.
	if (ctx->pglob != 0) {
		globfree(ctx->pglob);
		free(ctx->pglob);
	}
	free(ctx);
}

#else

/* <glob.h> isn't available.  We just provide do-nothing dummies.
 */
void*
os_find_first_file( const char* /*dir*/, const char* /*pattern*/, char* /*outbuf*/, size_t /*outbufsiz*/,
                    int* /*isdir*/, char* /*outpathbuf*/, size_t /*outpathbufsiz*/ )
{
	return 0;
}


void*
os_find_next_file( void* /*ctx*/, char* /*outbuf*/, size_t /*outbufsiz*/, int* /*isdir*/,
                   char* /*outpathbuf*/, size_t /*outpathbufsiz*/ )
{
	return 0;
}


void
os_find_close( void* /*ctx*/ )
{
}
#endif // HAVE_GLOB


/* Determine if the given filename refers to a special file.
 *
 * tads2/osnoui.c defines its own version when MSDOS is defined.
 */
#ifndef MSDOS
os_specfile_t
os_is_special_file( const char* fname )
{
	// We also check for "./" and "../" instead of just "." and
	// "..".  (We use OSPATHCHAR instead of '/' though.)
	const char selfWithSep[3] = {'.', OSPATHCHAR, '\0'};
	const char parentWithSep[4] = {'.', '.', OSPATHCHAR, '\0'};
	if ((strcmp(fname, ".") == 0) or (strcmp(fname, selfWithSep) == 0)) return OS_SPECFILE_SELF;
	if ((strcmp(fname, "..") == 0) or (strcmp(fname, parentWithSep) == 0)) return OS_SPECFILE_PARENT;
	return OS_SPECFILE_NONE;
}
#endif


/* Initialize.
 */
int
os_init( int*, char**, const char*, char*, int )
{
	return 0;
}


/* Uninitialize.
 */
void
os_uninit( void )
{
}


/* =====================================================================
 * These functions are currently not used by the compiler.  If the
 * linker barks, we must implement them.
 */

/* Get the creation time for a file.
 */
/*
int
os_get_file_cre_time( os_file_time_t* t, const char* fname )
{
}
*/

/* Get the access time for a file.
 */
/*
int
os_get_file_acc_time( os_file_time_t* t, const char* fname )
{
}
*/

/* Switch to a new working directory.
 */
/*
oid
os_set_pwd( const char* dir )
{
}
*/

/* Switch the working directory to the directory containing the given
 * file.
 */
/*
void
os_set_pwd_file( const char* filename )
{
}
*/
