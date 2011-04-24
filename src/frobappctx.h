/* TADS 2 provides the host application (in this case FrobTADS itself)
 * with a so-called "application container context" (appctx).  For
 * example, the VM asks the application about the current file I/O
 * safety level by invoking a callback in the appctx.  In this file, we
 * declare the functions that we want to include in the appctx.
 *
 * More details can be found in tads2/appctx.h.
 *
 * Note that these functions are not required to be 'extern "C"', since
 * they are called by C code only through function-pointers that have
 * already been declared 'extern "C"' by the TADS base code.
 */
#ifndef FROBAPPCTX_H
#define FROBAPPCTX_H

#include "common.h"

// Reports the current file I/O safety level.
void getIoSafetyLevel( void*, int* read, int* write );

#endif // FROBAPPCTX_H
