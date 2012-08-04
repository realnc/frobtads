#charset "us-ascii"
#pragma once

/*
 *   Copyright (c) 2001, 2006 Michael J. Roberts
 *   
 *   This file is part of TADS 3.
 *   
 *   This header defines the FileName intrinsic class.  
 */


/* include our base class definition */
#include "systype.h"

/* if we're using FileName objects, we probably want File objects as well */
#include "file.h"


/* ------------------------------------------------------------------------ */
/*
 *   A FileName represents the name of a file in the local operating
 *   system.  The File object methods that take filename specifications
 *   accept FileName objects as well as ordinary strings.
 *   
 *   It's easier to write portable code if you manipulate filenames using
 *   the FileName class rather than using ordinary strings.  TADS runs on
 *   several different operating systems with differing syntax rules for
 *   constructing file names and directory paths.  The FileName class
 *   handles the different rules that apply on each system.
 *   
 *   Construction:
 *   
 *.    new FileName() - creates a FileName object representing the working
 *.            directory (e.g., '.' on Unix or Windows)
 *   
 *.    new FileName(str) - creates a FileName from a string 'str' giving a
 *.            file path in local filename syntax.
 *   
 *.    new FileName(path, name) - creates a FileName from a path and
 *.            a file name.  'path' and 'name' can each be a FileName
 *.            object or a string using local filename syntax.  The
 *.            new object represents the combined path
 *   
 *.    new FileName(specialID) - creates a FileName from one of the special
 *.            file identifiers defined in file.h.
 *   
 *   You can also use the fromUniversal() static method, which creates a
 *   FileName from a path in universal (URL-style) notation.
 *   
 *   String conversions: using a FileName object in a context where a
 *   string is required (such as displaying it) automatically converts the
 *   object to a string giving the local file name path.
 *   
 *   Operators:
 *   
 *.    FileName + string  => yields a new FileName object combining
 *.            the path given by FileName and the string naming a file,
 *.            in local naming conventions.
 *   
 *.    FileName + FileName  => yields a new FileName object combining
 *.            the two names, treating the first as a directory path.
 *   
 *.    FileName == string (or !=) OR
 *.    FileName == FileName 
 *.            Compares the FileName to the string or other filename path.
 *.            This does a superficial comparison of the path contents,
 *.            without attempting to correlate the path to the actual file
 *.            system layout.  For example, it doesn't resolve symbolic
 *.            links on Unix or apply working directories to local paths.
 */
intrinsic class FileName 'filename/030000' : Object
{
    /*
     *   Get the filename.  This returns a string with the filename this
     *   object represents, in the local syntax used by the host operating
     *   system, including the path and base filename portions.  (This is
     *   the same string returned for toString(self), and the same string
     *   used if the filename is displayed as though it were a string, such
     *   as with "<< >>".)
     */
    getName();

    /* 
     *   Get the base filename portion, without the path.  This returns a
     *   string giving the filename without any directory location
     *   information; for a Unix-style path or Windows-style path, this is
     *   simply the last element of the path.
     */
    getBaseName();

    /* 
     *   Get the path portion name, without the file name.  This returns a
     *   FileName object containing the path portion of the file name, with
     *   the last path element removed.  If the path only contains one path
     *   element (so it contains only a file name, not a directory path),
     *   this returns nil.
     */
    getPath();

    /*
     *   Create a new FileName object from a path in universal URL-style
     *   syntax.  Path elements are separated with "/" characters.  The
     *   universal path syntax is converted to the local path notation for
     *   the new FileName object.
     */
    static fromUniversal(path);

    /* 
     *   Get the universal URL-style notation for this file name.  Returns
     *   a string giving the universal notation for the file name
     *   (including any path portion).
     */
    toUniversal();

    /* 
     *   Add a path element (a string or FileName object) to the end of
     *   this filename, yielding a new FileName object with the combined
     *   path.  Uses the correct local syntax to combine the path elements.
     *   This yields the same results as FileName + element.
     *   
     *   The new FileName object is in canonical form, meaning that any
     *   internal relative path elements (e.g., Unix "." and "..") are
     *   processed by combining them with adjacent elements as appropriate.
     *   For example, adding ".." to the Unix path "a/b/c" yields "a/b".
     */
    addToPath(element);

    /* 
     *   Is this an absolute path on the local system?  An absolute path is
     *   one that contains a root folder specification, such as a Unix path
     *   starting with "/", Windows path starting with "C:\", or a Windows
     *   UNC name such as "\\SERVER\SHARE".
     *   
     *   Note that a Windows path can start with a drive letter without
     *   being absolute, as in "C:path\file" (that's relative to the
     *   working folder on the C: drive), and can start with a backslash
     *   without being absolute, as in "\path\file" (that's relative to the
     *   working drive letter).  Similar subtleties might apply to other
     *   systems; this routine figures it out using local conventions.
     */
    isAbsolute();

    /* 
     *   Get a FileName giving the absolute path to this file.  This
     *   applies the current working directory and/or volume (e.g., drive
     *   letter on Windows) to produce the full path in absolute notation,
     *   using the appropriate syntax for the local operating system.  If
     *   the name is already in absolute format, the result will usually be
     *   unchanged, although the exact syntax might be modified on some
     *   systems to change the name to a more canonical format.
     *   
     *   If it's not possible to convert the filename into an absolute
     *   path, returns nil.  
     */
    getAbsolutePath();

    /*
     *   Get the list of root directories on the local system.  Returns a
     *   list of FileName objects representing the root directories.  The
     *   list only includes roots that are accessible under the file safety
     *   settings for getFileInfo(); note that other operations, such as
     *   listing the directory contents, might not be allowed even if the
     *   metdata are accessible.
     *   
     *   Most Unix-like systems only have one root directory, usually
     *   called '/'.  Many other systems have a separate root directory for
     *   each volume or device; for example, Windows has a root folder for
     *   each drive letter, so the root list might contain paths like C:\,
     *   D:\, etc.  Some systems have no concept of a root directory at
     *   all, in which case the result will be an empty list; this is the
     *   case for the network storage server.
     */
    static getRootDirs();

   /*   
    *   Get the type of the file.  If the file named by this object exists,
    *   returns an integer with a bitwise combination of FileTypeXxx values
    *   indicating the type of the file.  If the file doesn't exist, or
    *   can't be accessed due to file system permissions or some other
    *   operating system error, the return value is nil.  Note that it's
    *   also possible for the return value to be zero, which means
    *   something different from nil: zero means that the file exists, but
    *   it doesn't fit any of the FileTypeXxx classifications.
    *   
    *   If the file is a symbolic link, the method's behavior depends on
    *   'asLink'.  A symbolic link is a special type of file supported on
    *   some operating systems that serves as a pointer or proxy for
    *   another file.  If the file is a link, and 'asLink' is omitted or
    *   nil, the method returns information on the target of the link; this
    *   is the default because symbolic links in generally act as
    *   transparent proxies for their targets, so for most purposes a
    *   caller should be interested in the target file's metadata.
    *   However, a symbolic link also has a separate identity of its own as
    *   a link, so callers might sometimes be interested in the metadata
    *   for the link rather than its target.  To get information on the
    *   link itself, set 'asLink' to true.  'asLink' has no effect for
    *   ordinary non-link files, and also has no effect for "hard" links on
    *   systems that support those as well.
    *   
    *   Most of the FileTypeXxx bits are mutually exclusive, but it's
    *   possible that more than one bit will be set, so test using '&'
    *   (e.g., (f.getFileType() & FileTypeDir)).
    *   
    *   The file safety settings must allow read access to the file.
    */
    getFileType(followLinks?);

    /*
     *   Get extended information on the file named by this object.  This
     *   retrieves the size of the file, timestamps, and the file's type,
     *   and returns the information as a FileInfo object (see file.t).  If
     *   the file doesn't exist, or can't be accessed for some other reason
     *   at the operating system level, returns nil.
     *   
     *   'asLink' has the same meaning as in getFileType(), and has no
     *   effect at all unless the file named is a symbolic link.
     *   
     *   The file safety settings must allow read access to the file.
     */
    getFileInfo(followLinks?);

    /* 
     *   Delete the disk file named by this object.  The file safety level
     *   must allow write access to the file; a file safety exception is
     *   thrown if not. 
     */
    deleteFile();

    /*
     *   Rename or move the file.  This changes the name and/or file path
     *   location of the file named by 'self' to the given new path, which
     *   can be a string giving a filename in local path notation, or a
     *   FileName object with the new name.  The file safety settings must
     *   allow write access to both the original file and the new file.
     *   The new file must not already exist.
     */
    renameFile(newname);

    /*
     *   Get a list of files in the directory named by this object.
     *   Returns a list of strings giving the names of the files.  The file
     *   names are given as plain file names, without directory paths.  To
     *   get full paths, simply combine them with this FileName object
     *   using addToPath() or the "+" operator.
     *   
     *   The file safety settings must allow read access to the directory's
     *   contents.
     *   
     *   Note that many systems, including Windows and Unix-like systems,
     *   explicitly include relative directory links for the target
     *   directory and its parent in the results.  On Unix and Windows,
     *   these are called "." (for the self link) and ".." (for the
     *   parent), but you shouldn't assume those names are special, because
     *   they vary on other systems.  Instead, to test for these special
     *   links, you should build the full path, and compare the result to
     *   self (the directory you're listing) and to self.getParent() (or
     *   more generally, use newpath.isParentOf()).
     */
    listDir();

    /*
     *   Invoke a callback for each file in the directory named by this
     *   object.  'func' is a callback function; for each file in the
     *   directory, this is invoked as func(f), where 'f' is a FileName
     *   object describing the file.  If 'recursive' is true, the method
     *   recursively scans the contents of subdirectories; if 'recursive'
     *   is nil or is omitted, only the direct contents of the directory
     *   are scanned.
     */
    forEachFile(func, recursive?);

    /* 
     *   Create a directory with the name contained in this object.  The
     *   file safety settings must allow write access to the parent folder.
     *   
     *   If 'createParents' is specified, it's a true or nil value
     *   specifying whether or not to create intermediate parent
     *   directories.  The default is nil if it's omitted.  If it's true,
     *   and 'dirname' contains multiple path elements, any parents of the
     *   named directory that don't already exist will be created as well.
     *   For example, on Linux, if dirname is '/a/b/c', and directory '/a'
     *   exists but not '/a/b', the routine will first create '/a/b' and
     *   then create '/a/b/c'.
     */
    createDirectory(createParents?);

    /* 
     *   Remove the directory named by this object.  The file safety settings
     *   must allow write access to the directory.
     *   
     *   If 'removeContents' is provided, it's a true or nil value specifying
     *   whether or not to delete the contents of the directory before
     *   deleting the directory itself.  If this is true, and the directory
     *   contains any files or subdirectories, the routine will attempt to
     *   delete those contents before deleting the directory itself.  Any
     *   subdirectories will be recursively emptied and removed.  For obvious
     *   reasons, use caution when using this flag.  If any of the contents
     *   can't be deleted, the function will stop and throw an error.  Note
     *   that if this occurs, the function might have successfully deleted
     *   some of the contents of the directory before encountering the error;
     *   those deletions won't be undone.
     *   
     *   If 'removeContents' is omitted or nil, and the directory isn't
     *   already empty, the method simply returns nil (indicating failure)
     *   without deleting anything.  This is the default because it helps
     *   avoid accidentally deleting contents that the application didn't
     *   explicitly choose to remove.  (Special system files that are always
     *   present, such as "." and ".." on Unix, don't count when determining
     *   if the directory is empty.)
     */
    removeDirectory(removeContents?);
}


/* ------------------------------------------------------------------------ */
/*
 *   File type constants.  These are returned from getFileType(), and from
 *   getFileInfo() in the fileType property.  These are bit flags, so test
 *   for them using (fileType & FileTypeXxx).
 */

/* ordinary file (on disk or similar storage device) */
#define FileTypeFile   0x0001

/* directory (folder) */
#define FileTypeDir    0x0002

/* character-mode device (e.g., console) */
#define FileTypeChar   0x0004

/* block-mode device (e.g., Linux raw disk device) */
#define FileTypeBlock  0x0008

/* pipe (sometimes called a FIFO) or similar interprocess channel */
#define FileTypePipe   0x0010

/* network socket */
#define FileTypeSocket 0x0020

/* symbolic link (a filename that links to another file or directory) */
#define FileTypeLink   0x0040

/* special system-defined directory link to self (such as Unix ".") */
#define FileTypeSelfLink    0x0080

/* special system-defined parent directory link (such as Unix "..") */
#define FileTypeParentLink  0x0100



/* ------------------------------------------------------------------------ */
/*
 *   File attribute constants.  These are returned from getFileInfo() in the
 *   fileAttrs property.  These are bit flags, so test for them using
 *   (fileAttrs & FileAttrXxx).
 */

/* 
 *   Hidden file.  When this attribute is set, the file should be omitted
 *   from default views in the user interface and from wildcard matches in
 *   user commands (e.g., "rm *").  On some systems, a naming convention is
 *   used to mark files as hidden, such as ".xxx" files on Unix; on other
 *   systems, there's formal file system metadata corresponding to this
 *   attribute, such as on Windows.  Note that actually hiding files marked
 *   as hidden is up to the user interface; at a programmatic level, hidden
 *   files are treated the same as any other file, and in particular they're
 *   included in listDir() results.  It's up to the caller to decide whether
 *   or not to filter hidden files out of listDir() results, and if so to do
 *   the filtering.  The hidden attribute isn't enforced as a security or
 *   permissions mechanism in the file system; it doesn't prevent a user from
 *   explicitly viewing or deleting a file.  It's merely designed as a
 *   convenience for the user, to reduce clutter in normal directory listings
 *   by filtering out system or application files (such as preference files,
 *   caches, or indices) that the user doesn't normally access directly.
 */
#define FileAttrHidden 0x0001

/*
 *   System file.  This is a file system attribute on some systems (notably
 *   Windows) that marks a file as belonging to or being part of the
 *   operating system.  For practical purposes, system files should be
 *   treated the same as hidden files; the only reason we distinguish
 *   "system" as a separate attribute from "hidden" is to allow applications
 *   to display the two attributes separately when presenting file
 *   information to the user, who might expect to see both attributes on
 *   systems where both exist.  There's no equivalent of this attribute on
 *   most systems other than DOS and Windows; it won't ever appear in a
 *   file's attributes on systems where there's no equivalent.
 */
#define FileAttrSystem 0x0002

/*
 *   The file is readable by the current process.  If this is set, it means
 *   that the program has the necessary ownership and access privileges to
 *   read the file.  It's not guaranteed that a given attempt to read the
 *   file will actually succeed, since other conditions could arise, such as
 *   physical media errors or locking by another process that prevents
 *   concurrent access.
 */
#define FileAttrRead   0x0004

/*
 *   The file is writable by the current process.  If this is set, it means
 *   that the program has the necessary ownership and access privileges to
 *   write to the file.  It's not guaranteed that a given attempt to write to
 *   the file will actually succeed, since other conditions could arise, such
 *   as insufficient disk space, physical media errors, or locking by another
 *   process that prevents concurrent access.
 */
#define FileAttrWrite  0x0008
