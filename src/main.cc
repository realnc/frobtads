/* FrobTADS main entrypoint.
 *
 * We don't use the default command line parsing that TADS provides
 * because a) it's clumsy, b) we need our own set of options (colors,
 * scrolling, etc) and c) Unix users expect certain standards
 * (--version, --help, bug-report email address, long vs. short options,
 * etc).
 */
#include "common.h"

#include <stdio.h>
#include <string.h>
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#include "frobtadsappcurses.h"
#include "frobtadsappplain.h"
#include "colors.h"
#include "options.h"

#include "os.h"
#include "trd.h"
#include "t3std.h"
#include "vmmain.h"
#include "vmvsn.h"
#include "vmmaincn.h"
#include "vmhostsi.h"
extern "C"
{
#include "osgen.h"
}


const char versionOutput[] =
"FrobTADS " PACKAGE_VERSION "\n"
"TADS 2 virtual machive v" TADS_RUNTIME_VERSION "\n"
"TADS 3 virtual machine v" T3VM_VSN_STRING " (" T3VM_IDENTIFICATION ")\n"
"FrobTADS copyright (C) 2009 Nikos Chantziaras.\n"
"TADS copyright (C) 2009 Michael J. Roberts.\n"
"FrobTADS comes with NO WARRANTY, to the extent permitted by law.\n"
"You may redistribute copies of FrobTADS under certain terms and conditions.\n"
"See the file named COPYING for more information.";


const char helpOutput[] =
"Options:\n"
"  -h, --help           This help text\n"
"  -v, --version        Version information\n"
"  -n, --no-colors      Disable colors\n"
"  -f, --force-colors   Try to enable colors even if the system claims that\n"
"                       colors aren't available\n"
"  -o, --no-defcolors   Don't use the terminal's default colors\n"
"  -t, --tcolor         Text color (default white)\n"
"  -b, --bcolor         Background color (default black)\n"
"  -l, --stat-tcolor    Statusline text color (default is -b)\n"
"  -g, --stat-bcolor    Statusline background color (default is -t)\n"
"  -c, --no-scrolling   Disable soft (line by line) scrolling\n"
"  -p, --no-pause       Don't pause prior to quiting\n"
"  -d, --no-chdir       Don't change the current directory\n"
"  -s, --safety-level   File I/O safety level (default is 2)\n"
"  -e, --scroll-buffer  Size of the scroll-back buffer. Must be between 8 and\n"
"                       8192kB (default is 512kB)\n"
"  -r, --restore        Load a saved game position\n"
"  -u, --undo-size      Multiply the availabe T3VM undo buffer by n. Must be\n"
"                       between 1 and 64 (default is 16; about 100 UNDOs)\n"
"  -k, --character-set  Use given charset as the keyboard and display\n"
"                       character set.\n"
"  -i, --interface      Use given screen interface (curses or plain). Default\n"
"                       is curses. (Advanced features like statusline, banners\n"
"                       and colors are not available when using the plain\n"
"                       interface.)\n"
"Color codes:\n"
"   0:black 1:red 2:green 3:yellow 4:blue 5:magenta 6:cyan 7:white\n"
"(Note that yellow is actually brown on some hardware, mostly PCs.)\n"
"\n"
"File I/O safety levels:\n"
"   0: Safety mechanism disabled (unlimited read/write access)\n"
"   1: Read everywhere, write in current directory only\n"
"   2: Read/write in current directory only\n"
"   3: Read in current directory only, no writing allowed\n"
"   4: File I/O disabled (no reading or writing allowed)\n"
"\n"
"Short options are case-sensitive, long options are not. Options may be given\n"
"in any order and may be specified both before and after the filename. If an\n"
"option that takes an argument is given multiple times the last value is used.\n"
"Options may be abbreviated: --no-scr will be recognized as --no-scrolling.\n"
"\n"
"Report bugs to <" PACKAGE_BUGREPORT ">.\n"
"Please include the output of the --version option with the report.";


int main( int argc, char** argv )
{
	// These are the command-line options we recognize.  See
	// options.h for details on the format.  We keep this list
	// sorted alphabetically, in order to easily detect duplicate
	// short options.
	const char* optv[] = {
		"b:bcolor <0..7>",
		"c|no-scrolling",
		"d|no-chdir",
		"e:scroll-buffer <8..8192>",
		"f|force-colors",
		"g:stat-bcolor <0..7>",
		"h|help",
		"i:interface <curses|plain>",
		"k:character-set <charset>",
		"l:stat-tcolor <0..7>",
		"n|no-colors",
		"o|no-defcolors",
		"p|no-pause",
		"r:restore <filename>",
		"s:safety-level <0..4>",
		"t:tcolor <0..7>",
		"u:undo-size <1..64>",
		"v|version",
		0
	};

#ifdef HAVE_SETLOCALE
	// First, initialise locale, if available. We need only
	// LC_CTYPE.  Don't try to mess with numeric formats!
	setlocale(LC_CTYPE, "");
#endif

	// These are used for actual command-line parsing.
	int optChar;
	const char* optArg;
	Options opts(argv[0], optv);
	OptArgvIter iter(argc - 1, &argv[1]);

	// Allow the filename to be specified between options.
	// TODO: Maybe we shouldn't do this.  We could ignore any
	// options that come after the filename and just pass them to
	// the main() function of the game (TADS 3).  But this would
	// make no sense for TADS 2.  Another solution is to pass only
	// the explicitly ignored options to TADS 3 (that means, any
	// options specified after a "-- " in the command-line.
	opts.ctrls(Options::PARSE_POS);

	// We set this to true if an invalid option was given (unknown
	// or ambigous), so that we print an error message only the
	// first time an error occurs.
	bool optionError = false;

	// A semi-functional, portable ostream as defined in options.h.
	ostream cerr(stderr);
	ostream cout(stdout);

	// Available screen interfaces.
	enum screenInterface {cursesInterface, plainInterface};
	// We will use curses by default.
	screenInterface interface = cursesInterface;

	// Increase the T3VM undo-size 16 times by default.
	frobVmUndoMaxRecords = defaultVmUndoMaxRecords * 16;
	FrobTadsApplication::FrobOptions frobOpts = {
		// We assume some defaults.  They might change while
		// parsing the command line.
		true,       // Use colors.
		false,      // Don't force colors.
		true,       // Use terminal's defaults for color pair 0.
		true,       // Enable soft-scrolling.
		true,       // Pause prior to exit.
		true,       // Change to the game's directory.
		FROB_WHITE, // Text.
		FROB_BLACK, // Background.
		-1,         // Statusline text; none yet.
		-1,         // Statusline background; none yet.
		512*1024,   // Scroll-back buffer size.
		// Default file I/O safety level is read/write access
		// in current directory only.
		VM_IO_SAFETY_READWRITE_CUR,
  		// TODO: Revert the default back to "\0" when Unicode output
  		// is finally implemented.
		"us-ascii"  // Character set.
	};

	// Name of the game to run.
	const char* filename = 0;

	// Saved game position to restore (optional).
	const char* savedPosFilename = 0;

	// Start parsing.  Stop when there are no more options to parse.
	while ((optChar = opts(iter, optArg)) != Options::ENDOPTS) switch (optChar) {
	  // --version
	  case 'v':
		cout << versionOutput << "\n\n";
		return 0;

	  // --help
	  case 'h':
		cout << "Usage: " << opts.name() << " [options] file\n";
		cout << helpOutput << "\n\n";
		return 0;
		break;

	  // --no-colors
	  case 'n':
		frobOpts.useColors = false;
		break;

	  // --force-colors
	  case 'f':
		frobOpts.forceColors = true;
		break;

	  // --no-defcolors
	  case 'o':
		frobOpts.defColors = false;
		break;

	  // --no-scrolling
	  case 'c':
		frobOpts.softScroll = false;
		break;

	  // --no-pause
	  case 'p':
		frobOpts.exitPause = false;
		break;

	  // --no-chdir
	  case 'd':
		frobOpts.changeDir = false;
		break;

	  case 't': // --tcolor
	  case 'b': // --bcolor
	  case 'l': // --stat-tcolor
	  case 'g': // --stat-bcolor
	  {
		if (optionError) break;
		// We'll convert the string argument to a number.
		int tmp;
		if (optArg == 0) {
			// Argument is missing.
			optionError = true;
			break;
		}
		// Convert string to number.
		if (sscanf(optArg, "%d", &tmp) == 0) {
			// Conversion failed; it was not a number.
			cerr << opts.name() << ": colors must be numerical.\n";
			optionError = true;
			break;
		}
		if (tmp < 0 or tmp > 7) {
			// Invalid color.
			cerr << opts.name() << ": a color must be between 0 and 7.\n";
			optionError = true;
			break;
		}
		switch (optChar) {
		  case 't': frobOpts.textColor = tmp; break;
		  case 'b': frobOpts.bgColor = tmp; break;
		  case 'l': frobOpts.statTextColor = tmp; break;
		  case 'g': frobOpts.statBgColor = tmp; break;
		}
		break;
	  }

	  // --safety-level
	  case 's': {
		if (optionError) break;
		int tmp;
		if (optArg == 0) {
			// Argument is missing.
			optionError = true;
			break;
		}
		if (sscanf(optArg, "%d", &tmp) == 0) {
			// The argument was not a number.
			cerr << opts.name() << ": safety level must be numerical.\n";
			optionError = true;
			break;
		}
		if (tmp < 0 or tmp > 4) {
			// Out of range.
			cerr << opts.name() << ": safety level must be between 0-4.\n";
			optionError = true;
			break;
		}
		frobOpts.safetyLevel = tmp;
		break;
	  }

	  // --scroll-buffer
	  case 'e': {
		if (optionError) break;
		unsigned int tmp;
		if (optArg == 0) {
			// Argument is missing.
			optionError = true;
			break;
		}
		if (sscanf(optArg, "%u", &tmp) == 0) {
			// The argument was not a number.
			cerr << opts.name() << ": buffer size must be numerical.\n";
			optionError = true;
			break;
		}
		if (tmp < 8 or tmp > 8192) {
			// Buffer is out of range.
			cerr << opts.name() << ": buffer size must be between 8-8192 kB.\n";
			optionError = true;
			break;
		}
		// Adjust from kB to bytes.
		tmp *= 1024;
		frobOpts.scrollBufSize = tmp;
		break;
	  }

	  // --restore
	  case 'r': {
		if (optionError) break;
		if (optArg == 0) {
			// Argument is missing.
			optionError = true;
			break;
		}
		savedPosFilename = optArg;
		break;
	  }

	  // --undo-size
	  case 'u': {
		if (optionError) break;
		int tmp;
		if (optArg == 0) {
			// Argument is missing.
			optionError = true;
			break;
		}
		if (sscanf(optArg, "%d", &tmp) == 0) {
			// The argument was not a number.
			cerr << opts.name() << ": undo multiplicator must be numerical.\n";
			optionError = true;
			break;
		}
		if (tmp < 1 or tmp > 64) {
			// Out of range.
			cerr << opts.name() << ": undo multiplicator must be between 1-64.\n";
			optionError = true;
			break;
		}
		frobVmUndoMaxRecords = defaultVmUndoMaxRecords * tmp;
		break;
	  }

	  // --character-set
	  case 'k':
		if (optionError) break;
		if (optArg == 0) {
			// Argument is missing.
			optionError = true;
			break;
		}
		strncpy(frobOpts.characterSet, optArg, 16);
		frobOpts.characterSet[15] = '\0';
		break;

	  // --interface
	  case 'i':
		if (optionError) break;
		if (optArg == 0) {
			optionError = true;
			break;
		}
		if (strcmp(optArg, "curses") == 0) {
			interface = cursesInterface;
		} else if (strcmp(optArg, "plain") == 0) {
			interface = plainInterface;
		} else {
			cerr << opts.name() << ": available interfaces: curses, plain.\n";
			optionError = true;
		}
		break;

	  // This occurs when the argument is something other than an
	  // option.  We treat it as the filename of the game to run.
	  case Options::POSITIONAL:
		if (optionError) break;
		if (filename != 0) {
			// User already specified a filename.
			cerr << opts.name() << ": more than one filename given.\n";
			optionError = true;
			break;
		}
		filename = optArg;
		break;

	  // Common errors below.
	  case Options::BADCHAR:
	  case Options::BADKWD:
		optionError = true;
		break;

	  case Options::AMBIGUOUS:
		optionError = true;
		break;
	}

	if (filename == 0 and not optionError) {
		if (savedPosFilename != 0) {
			// No filename given, but a saved game position
			// was specified.  Ask TADS to find out the
			// filename of the game that the savefile is
			// associated with.  We make the buffer static
			// since we need to store a pointer to it even
			// after exiting this if-block.
			static char detectedFilename[OSFNMAX + 1];
			const char* const argvDummy[] = {"frob", "-r", savedPosFilename};
			if (vm_get_game_arg(3, argvDummy, detectedFilename, OSFNMAX + 1)) {
				// Success.  Store the filename.
				filename = detectedFilename;
			} else {
				// Failed.
				optionError = true;
			}
		} else {
			optionError = true;
		}
		if (optionError) cerr << opts.name() << ": no filename given.\n";
	}

	if (optionError) {
		opts.usage(cerr, "filename[.gam|.t3]");
		return 1;
	}

	// Set up the statusline colors (if the user didn't change the
	// defaults).  We'll simply use the normal colors but with
	// foreground/background reversed.
	if (frobOpts.statTextColor == -1) frobOpts.statTextColor = frobOpts.bgColor;
	if (frobOpts.statBgColor == -1) frobOpts.statBgColor = frobOpts.textColor;

	// If the filename the user specified lacks an extension, try
	// these.
	const char* defExts[] = {"gam", "t3"};
	// The filename TADS detects (probably after trying the above
	// extensions).
	char actualFilename[OSFNMAX + 1];

	// Ask TADS to find out what the specified file is supposed to
	// be.
	switch (vm_get_game_type(filename, actualFilename, OSFNMAX, defExts, 2)) {
	  case VM_GGT_TADS2:
		// It's a Tads 2 game.  Fire-up the T2VM.
		switch (interface) {
		  case cursesInterface: return FrobTadsApplicationCurses(frobOpts).runTads(actualFilename, 0);
		  case plainInterface: return FrobTadsApplicationPlain(frobOpts).runTads(actualFilename, 0);
		}

	  case VM_GGT_TADS3: {
		// It's Tads 3.
		int t3vmRet;
		switch (interface) {
		  case cursesInterface:
			t3vmRet = FrobTadsApplicationCurses(frobOpts).runTads(actualFilename, 1);
			break;
		  case plainInterface:
			t3vmRet = FrobTadsApplicationPlain(frobOpts).runTads(actualFilename, 1);
			break;
		}
		// Show any unfreed memory blocks.  This does nothing if
		// Tads 3 has not been compiled with debugging support.
		t3_list_memory_blocks(0);
		return t3vmRet;
	  }

	  case VM_GGT_INVALID:
		// It's not a Tads game.
		cerr << opts.name() << ": " << filename << ": not a Tads game.\n";
		break;

	  case VM_GGT_NOT_FOUND:
		// No such file.
		cerr << opts.name() << ": " << filename << ": file not found.\n";
		break;

	  case VM_GGT_AMBIG:
		// Filename is ambiguous.
		cerr << opts.name() << ": " << filename
		     << ": ambiguous filename; please include the file suffix.\n";
		break;
	}
	// If we reached this point, an error occured.
	return 1;
}
