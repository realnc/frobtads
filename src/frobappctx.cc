/* This file implements the appctx callbacks declared in frobappctx.h.
 */
#include "common.h"

#include "frobappctx.h"
#include "frobtadsapp.h"

int
getIoSafetyLevel( void* )
{
    return globalApp->options.safetyLevel;
}
