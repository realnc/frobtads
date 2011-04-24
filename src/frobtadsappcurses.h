/* This is the curses-specific implementation of FrobTadsApplication.
 */
#ifndef FROBTADSAPPCURSES_H
#define FROBTADSAPPCURSES_H

#include "common.h"

#include "frobtadsapp.h"
#include "tadswindow.h"


class FrobTadsApplicationCurses: public FrobTadsApplication {
  private:
    // The window we use for I/O.
    FrobTadsWindow *fGameWindow;

    // Tads never tries to display a string that is longer than the
    // window's width.  We use this knowledge to optimize output
    // somewhat.  Instead of allocating and deallocating memory each
    // time we print a string (memory allocation is slow), we use
    // this small buffer instead and allocate memory only once when
    // a game window is created.
    chtype* fDispBuf;

  public:
    FrobTadsApplicationCurses( const FrobOptions& opts );
    ~FrobTadsApplicationCurses();


    /* ============================================================
     * Interface implementation.
     * ============================================================
     */
  protected:
    virtual void
    init();

    // We simply handle everything in init().
    virtual void
    resizeEvent()
    { this->init(); }

  public:
    virtual void
    moveCursor( int line, int column )
    { this->fGameWindow->moveCursor(line, column); }

    virtual void
    print( int line, int column, int attrs, const char* str )
    {
        int i;
        for (i = 0; str[i] != '\0'; ++i) this->fDispBuf[i] = str[i] | attrs;
        this->fDispBuf[i] = '\0';
        this->fGameWindow->printStr(line, column, this->fDispBuf);
    }

    virtual void
    flush()
    { this->fGameWindow->flush(); }

    virtual void
    clear( int top, int left, int bottom, int right, int attrs );

    virtual void
    scrollRegionUp( int top, int left, int bottom, int right, int attrs );

    virtual void
    scrollRegionDown( int top, int left, int bottom, int right, int attrs );

    virtual int
    getRawChar( bool cursorVisible, int timeout );

    virtual void
    sleep( int ms )
    { this->fGameWindow->flush(); napms(ms); }

    virtual int
    height() const
    { return this->fGameWindow->height(); }

    virtual int
    width() const
    { return this->fGameWindow->width(); }
};

#endif // FROBTADSAPPCURSES_H
