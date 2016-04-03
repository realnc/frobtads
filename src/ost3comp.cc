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
