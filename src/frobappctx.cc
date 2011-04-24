/* This file implements the appctx callbacks declared in frobappctx.h.
 */
#include "common.h"

#include "frobappctx.h"
#include "frobtadsapp.h"

void
getIoSafetyLevel( void*, int* read, int* write )
{
    *read = globalApp->options.safetyLevelR;
    *write = globalApp->options.safetyLevelW;
}
