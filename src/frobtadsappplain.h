#ifndef FROBTADSAPPPLAIN_H
#define FROBTADSAPPPLAIN_H

#include "common.h"

#include <thread>
#include <chrono>
#include "frobtadsapp.h"


class FrobTadsApplicationPlain: public FrobTadsApplication {
  public:
    FrobTadsApplicationPlain( const FrobOptions& opts )
    : FrobTadsApplication(opts)
    {
        // Just tell the osgen layer to use plain mode.
        os_plain();
    }


    /* ============================================================
     * Interface implementation.
     * ============================================================
     */
  protected:
    virtual void
    init()
    { }

    virtual void
    resizeEvent()
    { }

  public:
    virtual void
    moveCursor( int, int )
    { }

    virtual void
    print( int, int, int, const char* str )
    { printf("%s", str); }

    virtual void
    flush()
    { fflush(stdout); }

    virtual void
    clear( int, int, int, int, int )
    { }

    virtual void
    scrollRegionUp( int, int, int, int, int )
    { }

    virtual void
    scrollRegionDown( int, int, int, int, int )
    { }

    virtual int
    getRawChar( bool, int )
    { return getchar(); }

    virtual void
    sleep( int ms )
    {
        flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    virtual int
    height() const
    { return 25; }

    virtual int
    width() const
    { return 80; }
};

#endif // FROBTADSAPPPLAIN_H
