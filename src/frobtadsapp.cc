/* FrobTadsApplication class implementation.
 */
#include "common.h"

#include <memory>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <csignal>

#include "os.h"
#include "trd.h"
#include "tio.h" // for qasopn
#include "t3std.h"
#include "vmmain.h"
#include "vmvsn.h"
#include "vmmaincn.h"
#include "vmhostsi.h"
extern "C"
{
#include "osgen.h"
}

#include "frobtadsapp.h"
#include "frobappctx.h"


FrobTadsApplication* globalApp;


/* Wee need a signal handler to handle window resizes when running
 * inside a terminal.
 */
void
winResizeHandler( int )
{
    // Re-initialize the screen.
    globalApp->resizeEvent();

    // Change the corresponding global variables that TADS uses to
    // tell the size.
    G_oss_screen_width = globalApp->width();
    G_oss_screen_height = globalApp->height();

    // Tell TADS that the size just changed.
    osssb_on_resize_screen();
    // Flush the screen just in case.
    globalApp->flush();

    // Restore the timeout.
    // FIXME: This resets the timeout to its initial
    // value.  We want to actually set it to the
    // remaining timeout.
    //globalApp->fGameWindow->setTimeout(0);

    // Not sure why, but on some systems the handler has to be
    // installed over and over again after the signal is handled.
#if HAVE_SIGWINCH
    signal(SIGWINCH, winResizeHandler);
#endif
}


FrobTadsApplication::FrobTadsApplication( const FrobOptions& opts )
: options(opts), fRemainingTimeout(0), fColorsEnabled(false)
{
    // Initialize the global pointer to us.
    globalApp = this;

    // Install our window-resize signal handler.
#if HAVE_SIGWINCH
    signal(SIGWINCH, winResizeHandler);
#endif
}


int
FrobTadsApplication::fRunTads2( char* filename )
{
    // Create the Tads 2 application container context (appctx).  We
    // make it static for no other reason than automatic null
    // initialization of the fields (C++ guarantees that); unused
    // fields *must* be 0, or else the VM would crash.
    static appctxdef appctx;

    // Set the file I/O safety level callback.
    appctx.get_io_safety_level = getIoSafetyLevel;

    // trdmain() requires argc/argv style arguments.  We create some
    // to make it happy.
    char argv0[] = "frob";
    char* argv[2] = {argv0, filename};

    // If a replay file was specified, load it.
    if (this->options.replayFile != 0) {
        qasopn((char *)this->options.replayFile, 1);
    }

    // Run the Tads 2 VM.
    char savExt[] = "sav";
    int vmRet = trdmain(2, argv, &appctx, savExt);

    return vmRet;
}


int
FrobTadsApplication::fRunTads3( char* filename, int argc, const char* const* argv,
                                const char *savedState, class TadsNetConfig* netconfig )
{
    // Create the Tads 3 host and client services interfaces.
    CVmMainClientConsole clientifc;
    const auto hostifc = std::make_unique<CVmHostIfcStdio>("");

    // Set the file I/O safety level.
    hostifc->set_io_safety(this->options.safetyLevelR, this->options.safetyLevelW);

    // Set the network safety level.
    hostifc->set_net_safety(this->options.netSafetyLevelC, this->options.netSafetyLevelS);

    // Set up the VM/program parameters.
    vm_run_image_params params(&clientifc, hostifc.get(), filename);
    params.prog_argv = argv;
    params.prog_argc = argc;
    params.script_file = this->options.replayFile;
    params.seed_rand = this->options.seedRand;
    params.saved_state = savedState;
    params.netconfig = netconfig;

    // Invoke the VM to run the program.
    int vmRet = vm_run_image(&params);

    // Return the result from the VM ivocation.
    return vmRet;
}


int
FrobTadsApplication::runTads( const char* filename, int vm, int argc, const char *const *argv,
                              const char* savedState, class TadsNetConfig* netconfig )
{
    // We might strip the path from the filename later, in case we
    // change the current directory.
    size_t filenameLen = strlen(filename);
    char* finalFilenamePtr;
    const auto finalFilename = std::make_unique<char[]>(filenameLen + 1);
    strcpy(finalFilename.get(), filename);
    finalFilenamePtr = finalFilename.get();

    // We'll try setting the current directory to the game's
    // directory (if the user didn't tell us not to).
    if (this->options.changeDir) {
        const auto gameDirBuf = std::make_unique<char[]>(filenameLen + 1);
        gameDirBuf[0] = '\0'; // Paranoia.
        os_get_path_name(gameDirBuf.get(), filenameLen, filename);
        // Try changing the current directory.
        if (this->changeDirectory(gameDirBuf.get())) {
            // Success.  Since we changed the current
            // directory, we must adapt the filename by
            // stripping it from its path.  For this
            // purpose, we'll just store the position of the
            // filename's first character.
            finalFilenamePtr = os_get_root_name(finalFilename.get());
        }
    }

    // Initialize the screen.
    this->init();

    // The osgen layer uses these global variables to determine the
    // size of the screen; set them to the width and height of our
    // game window.
    G_oss_screen_width = this->width();
    G_oss_screen_height = this->height();

    // Initialize the osgen scrollback buffer.
    osssbini(this->options.scrollBufSize);

    // A kludge to circumvent a curses color problem; display one
    // character in reverse video then one in normal colors.  This
    // avoids the problem where the first block of text is shown in
    // wrong colors.
    int tmpColor = ossgetcolor(OSGEN_COLOR_STATUSLINE, OSGEN_COLOR_STATUSBG, 0, 0);
    ossdsp(0, 0, tmpColor, " ");
    globalApp->flush();
    tmpColor = ossgetcolor(OSGEN_COLOR_TEXT, OSGEN_COLOR_TEXTBG, 0, 0);
    ossdsp(0, 0, tmpColor, " ");
    globalApp->flush();

    // Run the VM.
    int vmRet = vm == 0 ? this->fRunTads2(finalFilenamePtr)
                        : this->fRunTads3(finalFilenamePtr, argc, argv, savedState, netconfig);

    // Pause.
    os_expause();

    // Delete the scrollback buffer.
    osssbdel();

    // Return the VM's exit code.
    return vmRet;
}


/* We implement changeDirectory() as a wrapper around chdir() or
 * SetCurrentDirectory(); whichever is available.  The former is in
 * <unistd.h>, the latter in <windows.h>.  If the system lacks both,
 * we'll always return false to indicate failure.
 */
#if HAVE_CHDIR
#include <unistd.h>
#endif
#if HAVE_SETCURRENTDIRECTORY
#include <windows.h>
#endif
bool
FrobTadsApplication::changeDirectory( const char* dir )
{
#if HAVE_CHDIR
    if (chdir(dir) == 0) return true;
    return false;
#else
#if HAVE_SETCURRENTDIRECTORY
    return SetCurrentDirectory(dir);
#else
    return false;
#endif
#endif
}
