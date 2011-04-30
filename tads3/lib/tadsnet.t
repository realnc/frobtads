#charset "us-ascii"

#include <tads.h>
#include <tadsnet.h>

/* 
 *   Copyright (c) 1999, 2006 Michael J. Roberts
 *   
 *   This file is part of TADS 3
 *   
 *   This file defines classes and properties used with the tads-net
 *   intrinsic function set.  If you're using this function set, you should
 *   include this source file in your build by adding it to your project
 *   makefile.
 */

/* include the tads-net intrinsic function set interface definition */
#include <tadsnet.h>

/* ------------------------------------------------------------------------ */
/*
 *   A NetEvent instance describes an event read via the getNetEvent()
 *   function.
 *   
 *   In most cases, this base class will not be instantiated directly.
 *   getNetEvent() will always construct the appropriate subclass for the
 *   specific type of event being generated, if that subclass is defined in
 *   the game program.  However, it's possible that the game won't define
 *   all necessary subclasses.  For example, a game written for version 1
 *   of the networking package wouldn't include new subclasses added in
 *   version 2, because those subclasses weren't defined at the time the
 *   game was written.  When getNetEvent() needs to instantiate a subclass
 *   that isn't defined in the game program, it will instead create a base
 *   NetEvent object, which will simply store the subclass-specific
 *   arguments as a list.  This could be useful for debugging purposes,
 *   because it will at least let the programmer inspect the event details
 *   with the interactive debugger.  
 */
class NetEvent: object
    /*
     *   The event type.  This is a NetEvXxx value (see tadsnet.h)
     *   indicating which type of event this is.    
     */
    evType = nil

    /* 
     *   Construction.  getNetEvent() only constructs this object directly
     *   when the subclass it's looking for isn't defined in the game
     *   program.  
     */
    construct(t, [args])
    {
        evType = t;
        evArgs = args;
    }

    /*
     *   Extra event-specific arguments.  This is primarily for debugging
     *   purposes, since it's only used when getNetEvent() needs to
     *   construct a NetEvent subclass that isn't defined in the game.  In
     *   this case, the absence of a subclass definition in the game
     *   presumably means that the game isn't written to handle the type of
     *   event generated (for example, because it was written for an older
     *   interpreter version that didn't have the event type).  
     */
    evArgs = nil
;

/*
 *   Network Request Event.  This type of event occurs when a server (such
 *   as an HTTPServer object) receives a request from a network client.
 *   
 *   The evRequest member contains a request object describing the network
 *   request.  The class of this object depends on the type of server that
 *   received the request.  For example, for an HTTP server, this will be
 *   an HTTPRequest object.  To reply to the request, use the appropriate
 *   method(s) in the request object - for details, see the specific
 *   request classes for the server types you create in your program.
 */
class NetRequestEvent: NetEvent
    /* construction */
    construct(t, req)
    {
        inherited(t, req);
        evRequest = req;
    }

    evType = NetEvRequest

    /*
     *   The request object.  When the event type is NetEvRequest, this
     *   contains a request object describing the request.  The class of
     *   the request object varies according to the server type; you can
     *   use ofKind() to check which type of request it is.  For example,
     *   for an HTTP request, this will be an object of class HTTPRequest.
     */
    evRequest = nil
;

/*
 *   Network Timeout Event.  getNetEvent() returns this type of event when
 *   the timeout interval expires before any actual event occurs.  
 */
class NetTimeoutEvent: NetEvent
    evType = NetEvTimeout
;

/* ------------------------------------------------------------------------ */
/*
 *   A FileUpload represents a file uploaded by a network client via a
 *   protocol server, such as an HTTPServer.  
 */
class FileUpload: object
    construct(file, contentType, filename)
    {
        self.file = file;
        self.contentType = contentType;
        self.filename = filename;
    }

    /*
     *   A File object with the uploaded content.  This is open for
     *   read-only access.
     *   
     *   If the contentType parameter is a text type ("text/html",
     *   "text/plain", etc), and the interpreter recognizes the character
     *   set parameter in the contentType, the file is in Text mode
     *   (FileModeText) with the appropriate character mapper in effect.
     *   Otherwise, the file is in raw binary mode (FileModeRaw).  If you
     *   need the file to be opened in a different mode, you can use
     *   setFileMode() on the file to change the mode.  
     */
    file = nil

    /*
     *   The content type.  This a string giving the MIME type specified by
     *   the client with the upload.  This is the full content-type string,
     *   including any attributes, such "charset" for a text type.  This
     *   can be nil if the client doesn't specify a content-type at all.
     *   
     *   It's important to recognize that this information is supplied by
     *   the client, and is NOT validated by the protocol server.  At best
     *   you should consider it a suggestion, and at worst you should be
     *   suspicious of it.  The client could be wrong about the type, or
     *   could even maliciously misrepresent it.  You should always
     *   validate the actual contents if you're using the file in a way
     *   that relies upon well-formed data in any particular format.  
     */
    contentType = nil

    /*
     *   The client-side filename, if specified.  This is a string giving
     *   the name of the file on the client machine.  This generally has no
     *   particular meaning to the server, since we can't infer anything
     *   about the directory structure or naming conventions on an
     *   arbitrary client.  However, this might be useful for reference,
     *   such as showing information about the upload in a user interface.
     *   It's sometimes also marginally useful to know the suffix
     *   (extension) for making further guesses about the content type -
     *   although as with the content-type, you can't rely upon this, but
     *   can only use it as a suggestion from the client.
     *   
     *   The client won't necessarily specify a filename at all, in which
     *   case this will be nil.  
     */
    filename = nil
;
    

/* ------------------------------------------------------------------------ */
/*
 *   A NetException is the base class for network errors. 
 */
class NetException: Exception
    construct(msg?, errno?)
    {
        if (errMsg != nil)
            errMsg = 'Network error: <<msg>>';
        if (errno != nil)
            errMsg += ' (system error code <<errno>>)';
    }
    displayException() { "<<errMsg>>"; }

    /* a descriptive error message provided by the system */
    errMsg = 'Network error';
;

/*
 *   A NetSafetyException is thrown when the program attempts to perform a
 *   network operation that isn't allowed by the current network safety
 *   level settings.  The user controls the safety level; the program can't
 *   override this.  
 */
class NetSafetyException: NetException
    errMsg = 'Network operation prohibited by user-specified '
             + 'network safety level'
;

/*
 *   A SocketDisconnectException is thrown when attempting to read or write
 *   a network socket that's been closed, either by us or by the peer (the
 *   computer on the other end of the network connection).  If we didn't
 *   close the socket on this side, this error usually means simply that
 *   the peer program has terminated or otherwise disconnected, so we
 *   should consider the conversation terminated.  
 */
class SocketDisconnectException: NetException
    errMsg = 'Network socket disconnected by peer or closed'
;

/* export the objects and properties used in the tads-net function set */
export NetEvent 'TadsNet.NetEvent';
export NetRequestEvent 'TadsNet.NetRequestEvent';
export NetTimeoutEvent 'TadsNet.NetTimeoutEvent';
export NetException 'TadsNet.NetException';
export SocketDisconnectException 'TadsNet.SocketDisconnectException';
export NetSafetyException 'TadsNet.NetSafetyException';
export FileUpload 'TadsNet.FileUpload';

