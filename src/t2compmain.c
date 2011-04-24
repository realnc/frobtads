/* Main entrypoint for the TADS 2 compiler (tadsc).
 */

#include "common.h"

#include "tcd.h"


int main( int argc, char** argv )
{
    return tcdmain(argc, argv, 0);
}
