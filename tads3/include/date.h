#charset "us-ascii"
#pragma once

/*
 *   Copyright (c) 2001, 2006 Michael J. Roberts
 *   
 *   This file is part of TADS 3.
 *   
 *   This header defines the Date and TimeZone intrinsic classes.  
 */

/* include our base class definition */
#include "systype.h"


/* ------------------------------------------------------------------------ */
/*
 *   The Date intrinsic class stores a date-and-time value representing a
 *   particular point in time, and provides methods and operators for date
 *   arithmetic, formatting, and parsing.
 *   
 *   The date/time value is stored internally in terms of universal time
 *   (UTC).  This makes it independent of locations and time zones.  When a
 *   new Date object is created for a given calendar day or clock time, that
 *   source value is assumed to be in terms of the local wall clock time of
 *   the machine it's running on, but a different time zone can be explicitly
 *   specified instead; the Date object automatically translates that local
 *   time value to UTC for storage.  Likewise, when a Date is formatted to a
 *   string representation, or when calendar or clock components are
 *   extracted from it, the formatted or extracted value is translated by
 *   default to the local time zone, but a different time zone can be
 *   specified.
 *   
 *   You can create a Date object from a number of other representations for
 *   date values:
 *   
 *.   new Date() - creates a Date representing the current date and time.
 *   
 *.   new Date('string', refTZ?, refDate?) - parses the string as a date
 *.          value.  'refTZ' is an optional TimeZone object; if provided,
 *.          the date string will be interpreted as a local time in that
 *.          zone, unless the string contains a timezone specifier.  The
 *.          default timezone if 'refTZ' is missing is the local system's
 *.          time zone.  'refDate' is an optional Date object that's used
 *.          to fill in certain missing date elements in the string.  If
 *.          the string doesn't specify the year (e.g., 'March 1'), the
 *.          year is taken from refDate; if the string only specifies only
 *.          a time, the date is taken from refDate.  If the date uses a
 *.          two-digit year number ('5/15/92'), the century is inferred
 *.          from the reference date by finding the year closest to the
 *.          reference year (for example, if it's currently 2012, '92' is
 *.          interpreted as 1992, since that's closer to 2012 than is
 *.          2092).  The date parser accepts numerous common human-readable
 *.          formats and several standard computer formats.  For a full
 *.          list of, see the System Manual.  The parser uses the locale
 *.          settings from setLocaleInfo() to match month names, weekday
 *.          names, etc; the default settings are for US English format.
 *
 *.   new Date(number, 'J') - 'number' can be an integer or a BigNumber.
 *.          This creates a Date object representing the given number of
 *.          days after January 1, 4713 BCE on the Julian calendar, at
 *.          noon UTC.  The fractional part is the fraction of a day
 *.          past noon; for example, a fractional portion of 0.25
 *.          represents 1/4 of a day, or 6 hours, so it represents a
 *.          clock time of 18:00 UTC (6 hours past noon).
 *   
 *.   new Date(number, 'U') - 'number' can be an integer or a BigNumber.
 *.          This creates a Date object representing the given number of
 *.          seconds after standard Unix Epoch (1/1/1970 00:00:00 UTC),
 *.          or before the Epoch if the value is negative.  If 'number'
 *.          is a BigNumber, it can specify fractions of a second, which
 *.          the Date object will round to the nearest milliseconds.
 *.          This constructor format is provided because "seconds since
 *.          1/1/1970" is the standard representation of time on Unix-like
 *.          systems, and as a result it's also common in file formats.
 *   
 *.   new Date(year, month, day, tz?) - midnight on year/month/day (all
 *.          given as integers), in the given local time zone 'tz' (given
 *.          as a TimeZone object or a string giving a time zone name).
 *.          The default time zone if 'tz' is omitted is the system's
 *.          local time zone.  The year must be given as the full year
 *.          with century, e.g., 2012, not just 12; the latter is valid
 *.          but is taken literally as the first-century year 12.  The
 *.          month is 1-12 for January to December.  The day is simply
 *.          the calendar day of the month (e.g., 5 for January 5).  The
 *.          year can zero or negative.  Note that because year 0 is a
 *.          valid year in Date's calendar system, negative years are off
 *.          by one from the "BC" convention: in the AD/BC convention,
 *.          the year before AD 1 is 1 BC, not AD 0; so on our calendar,
 *.          year 0 corresponds to 1 BC, -1 is 2 BC, etc.
 *   
 *.   new Date(year, month, day, hour, minute, seconds, ms, tz?) - the
 *.          given date and time value, with each element as an integer.
 *.          The value is in terms of the given local time zone 'tz',
 *.          or the system's local time zone if 'tz' is omitted.  'ms'
 *.          is the number of milliseconds (0-999).
 *   
 *   Date arithmetic: the following operators perform calendar calculations
 *   on the date value:
 *   
 *.   Date + integer   - add days to the date
 *.   Date - integer   - subtract days from date
 *.   Date + BigNumber - add days (which can include fractional days)
 *.   Date - BigNumber - subtract days (which can include fractional days)
 *.   Date - Date      - number of days between dates (with fractional days)
 *   
 *   Adding an integer or BigNumber returns a new Date object representing
 *   the result; Date objects are immutable, so the original Date value isn't
 *   changed by the operation.
 *   
 *   Subtracting one Date from another yields a BigNumber with the difference
 *   in days between the two dates.  Note that this might have a fractional
 *   part, because the difference might not be whole days; for example,
 *   subtracting 13:00 from 19:00 on the same day yields 0.25, which is 1/4
 *   of a 24-hour day, or 6 hours..  Like all Date arithmetic, subtraction
 *   works in universal time, so the subtraction yields the true time
 *   difference between the events even if they were created from times in
 *   different time zones.  For example, subtracting 1 PM Eastern Time from 1
 *   PM Pacific Time on the same day yields 0.125 (1/8 of a day, or 3 hours).
 *   
 *   The comparison operators (< > <= >=) compare two Date values by order of
 *   occurrence in time.  For example, a > b is true if a is later than b.
 *   The comparison is done in universal time, so the comparison yields the
 *   actual event order, not the nominal local time order.  For example, 2 PM
 *   Eastern Time is earlier than 1 PM Pacific Time on any given day.
 *   
 *   Time zone specifications: when a time zone is specified in a constructor
 *   or method argument, you can supply a TimeZone object, or a string with
 *   the name of a time zone, using the same formats that the TimeZone
 *   constructor accepts.  See the TimeZone object for more information.  The
 *   default if you don't specify a time zone is 
 *   
 */
intrinsic class Date 'date/030000': Object
{
    /*
     *   Parse a date.  This is similar to the new Date('date string')
     *   constructor, but lets you specify your own custom format templates,
     *   and returns a detailed breakdown of the source string elements that
     *   were matched in the various fields.  'str' is the source string to
     *   be parsed.  'format' is an optional format string, or a list of
     *   format strings used to parse the date, or be nil if you simply want
     *   to use the built-in formats.  See the System Manual for the syntax
     *   of these strings.  If you want to specify one or more custom formats
     *   and also use the built-in formats, use a list for 'format', and
     *   include a nil element to include the standard formats.  'refDate' is
     *   the reference date, which is used to fill in certain missing fields;
     *   this works the same way as in the new Date('str', refDate)
     *   constructor, and defaults to the current time if omitted or nil.
     *   'refTZ' is the reference time zone, and defaults to the host
     *   system's local time zone if omitted or nil.
     *   
     *   The return value on success is a list: [date, tzobj, format, era,
     *   year, month, day, yearDay, weekDay, ampm, hour, minute, second, ms,
     *   unix, tz].  'date' is the parsed Date object.  'tzobj' is a TimeZone
     *   object if a timezone is specified in the date string, otherwise nil
     *   (in which case the date is implicitly in terms of 'refTZ', or the
     *   host system's local timezone if 'refTZ' is omitted).  'format' is
     *   the actual format string(s) that was/were matched, which can come
     *   from the custom format strings you supplied and/or the built-in
     *   formats; if multiple formats were matched, they'll be concatenated
     *   together to form this string.  The remaining fields are strings
     *   giving the literal source text matched for the individual date
     *   components, or nil for components not matched.
     *   
     *   If the date string can't be parsed, the return value is nil.
     */
    static parseDate(str, format?, refDate?, refTZ?);

    /*
     *   Parse a Julian date.  This works like parseDate(), except that the
     *   date is interpreted on the Julian calendar.  Julian dates have the
     *   same form as Gregorian dates, but a given day will have different
     *   nominal dates on the two calendars, except for most of the third
     *   century, where they happen to align.  The further in time a day is
     *   from the overlap in the third century, the more the dates will
     *   diverge on the two calendars; in the 21st century, the difference is
     *   about 13 days.
     */
    static parseJulianDate(str, format?, refDate?, refTZ?);

    /*
     *   Format the date/time value as a Gregorian calendar date, using the
     *   given format template string.  Returns a string with the formatted
     *   date/time.  The date/time is displayed in the given time zone (or
     *   the system's local time zone if 'tz' isn't specified).
     */
    formatDate(format, tz?);

    /*
     *   Format the date/time value as a Julian calendar date, using the
     *   given format template string. 
     */
    formatJulianDate(format, tz?);

    /*
     *   Compare to another Date object; returns an integer less than zero if
     *   this Date is before the other Date, zero if they refer to the same
     *   date, greater than zero if this Date is after the other Date.  The
     *   same comparisons can be done with the ordinary comparison operators
     *   (<, >, <=, >=, ==, !=), but this is convenient for sorting callbacks
     *   since it lets you get the greater/equal/less result in one shot.
     */
    compareTo(date);

    /* 
     *   Get the Gregorian calendar date represented by this Date object, in
     *   terms of local time in the given time zone (or the system's local
     *   time zone if 'tz' isn't specified).  Returns a list consisting of
     *   [year, month, day, weekday], each value represented as an integer.
     *   The weekday is 1 for Sunday, 2 for Monday, etc.  For example, June
     *   21, 2012 (a Thursday) is represented as [2012,6,21,5].
     */
    getDate(tz?);

    /*
     *   Get the Julian day number.  This is the number of days since January
     *   1, 4713 BCE on the (proleptic) Julian calendar, at noon UTC.  This
     *   is an important figure in astronomy.  It's also often useful as a
     *   common currency for converting between arbitrary calendars: you
     *   might not be able to find a published formula for converting between
     *   calendar X and calendar Y, but you can almost always find a formula
     *   for converting between any given calendar and the Julian day system.
     *   
     *   The return value is a BigNumber value giving the Julian day
     *   corresponding to this Date value, including a fractional part for
     *   the time past noon UTC on that date.
     *   
     *   Note that there's no local time zone involved in this calculation,
     *   since the Julian day number is specifically defined in terms of
     *   universal time.
     */
    getJulianDay();

    /*
     *   Get the Julian calendar date for this Date object, in terms of the
     *   local time in the given time zone (or the system's local time zone
     *   if 'tz' isn't specified).  Returns a list consisting of [year,
     *   month, day, weekday].  (The weekday on the Julian calendar is always
     *   the same as the weekday on the Gregorian calendar for a given Date
     *   value.)
     */
    getJulianDate(tz?);

    /*
     *   Get the ISO 8601 week date.  This returns a list with three
     *   elements, [year, week, day], where 'year' is the ISO year number
     *   containing the date, 'week' is the week number (1 to 53), and 'day'
     *   is the day of the week (1-7, Monday to Sunday, per the ISO 8601
     *   conventions).  The year on the ISO week calendar can differ from the
     *   year on the Gregorian calendar for dates during the first and last
     *   week of a year on Gregorian calendar, because year boundaries on the
     *   ISO calendar always occur on week boundaries.  For example, Jan 1,
     *   2005 has the ISO week date 2004-W53-6 - it's part of 2004 on the ISO
     *   week calendar because weeks can't be split across years, so the
     *   entire week belongs to 2004 on the ISO calendar.  This can work in
     *   both directions: Dec 31, 2007 has the ISO week date 2008-W01-1.
     */
    getISOWeekDate(tz?);

    /* 
     *   Get the wall clock time represented by this Date object, in terms of
     *   local time in the given time zone (or the system's local time zone
     *   is 'tz' isn't specified).  Returns a list of integers: [hour,
     *   minute, second, ms].  The hour is on a 24-hour clock, with 0 hours
     *   representing midnight and 23 representing 11 PM.  The 'ms' value is
     *   a value from 0 to 999 giving the milliseconds portion of the time.
     */
    getClockTime(tz?);

    /*
     *   Add an interval to this date, returning a new date object.  The
     *   interval is given as a list of integers: [years, months, days,
     *   hours, minutes, seconds].  The 'seconds' value can be a BigNumber
     *   with a fractional part (but anything smaller than milliseconds is
     *   discarded).  Elements can be omitted from the end of the list; for
     *   example, [0, 2] adds two months.  An element can be negative: [-1]
     *   subtracts one year.
     */
    addInterval(interval);

    /*
     *   Find a given day of the week relative to this date, in its local
     *   time zone.  'weekday' is the target weekday to find, as an integer:
     *   1 for Sunday, 2 for Monday, ..., 7 for Saturday.  'which' is an
     *   integer specifying which relative day to find: 1 means to find the
     *   next occurrence of the given weekday on or after this date, 2 means
     *   the second occurrence on or after this date, and so on.  -1 means
     *   the first occurrence on or before this date; -2 is the second
     *   occurrence on or before this date; etc.
     */
    findWeekday(weekday, which, tz?);


    /*
     *   Set a locale string.  This sets localized versions of the string
     *   values used for parsing and formatting date values.  By default,
     *   suitable English strings are used.  See the DateXxx index values for
     *   the individual locale strings that can be customized.
     *   
     *   Note that this is a static method that you call on the Date class
     *   object (e.g., Date.setLocaleInfo(DateMonthNames,
     *   'Sunday,Monday,...').
     *   
     *   There are two ways to call this method:
     *   
     *.  1. Date.setLocaleInfo([monthNames, monthAbbrs, weekdayNames, ...])
     *.  Each item in the list is a string.  This lets you set all of
     *   the locale items in one shot; the items are listed in order of
     *   the DateXxx index values.  You can omit items from the end of
     *   the list; any omitted items won't be changed.
     *   
     *   2. Date.setLocaleInfo(DateXxx, 'value', ...);
     *.  Specify DateXxx indices and the corresponding string values as
     *   alternating arguments.  These have to be provided in pairs.
     *   Any indices not listed won't be affected.
     *   
     *   All of the values are specified as strings.  Most are formatted as
     *   lists of words, separated by commas:
     *   
     *.    Date.setLocaleInfo(DateWeekdayAbbrs, 'Sun,Mon,Tue,Wed,Thu,Fri,Sat');
     *   
     *   For input parsing, you can provide synonyms, separated by '='.  For
     *   example, for month abbreviations, the English version accepts "Sep"
     *   and "Sept" as synonyms, which you can specify like so:
     *   
     *.    Date.setLocaleInfo(DateMonthAbbrs, 'Jan,Feb,Mar,Apr,May,Jun,'
     *.                       + 'Jul,Aug,Sep=Sept,Oct,Nov,Dec');
     *   
     *   When formatting output, the first item in a synonym list is the one
     *   displayed.
     */
    static setLocaleInfo(...);
}

/*
 *   Locale indices for Date.setLocaleInfo()
 */

/* full names of months - January,February,March,... */
#define DateMonthNames     0

/* month name abbreviations - Jan,Feb,Mar... */
#define DateMonthAbbrs     1

/* full names of weekdays - Sunday,Monday,Tuesday,... */
#define DateWeekdayNames   2

/* abbreviated weekday names - Sun,Mon,Tue,... */
#define DateWeekdayAbbrs   3

/* AM/PM indicators - AM,PM */
#define DateAMPM           4

/* "era" indicator - AD=CE,BC=BCE */
#define DateEra            5

/*
 *   Parsing filter for culture-specific date formats.  This can be 'us' to
 *   select the US-style formats, or 'eu' to select European-style formats.
 *   This controls day/month or month/day order when parsing numeric dates -
 *   the 'us' formats use the "month/day" style, as in 11/20 for November 20,
 *   and the 'eu' formats use "day/month", as in 20/11.
 */
#define DateParseFilter    6

/* 
 *   Ordinal suffixes for 1st, 2nd, 3rd, Nth, X1st, X2nd, Xrd.  'Nth' is the
 *   suffix for everything not otherwise enumerated.  'X1st' is the suffix
 *   for the 21st, 31st, 1041st, etc - this applies to all of the decades
 *   except the units and teens; likewise for 'X2nd' and 'X3rd'.  Specify
 *   only the suffix; e.g., for English, 'st,nd,rd,th,st,nd,rd'.  If all of
 *   the items after a given point are the same for the target language, you
 *   can omit them, and the last item in the list will be used for all
 *   missing items; e.g., French can specify simply 'er,e', and German can
 *   specify simply '.'.
 */
#define DateOrdSuffixes    7

/*
 *   Default local format for date/time stamps.  This is a format string
 *   suitable for use in formatDate().  The default is '%a %b %#d %T %Y'
 *   (which produces, e.g., 'Thu Feb 7 15:20:33 2009').
 */
#define DateFmtTimestamp   8

/* 
 *   Default local format for the time, without the date.  The default is
 *   '%H:%M:%S' ('18:44:39').
 */
#define DateFmtTime        9

/* 
 *   Default local format for the date, without the time.  The default is
 *   '%m/%d/%Y' (02/15/2012).
 */
#define DateFmtDate       10

/*
 *   Local short date format.  The default is '%m/%d/%y' (02/15/12). 
 */
#define DateFmtShortDate  11

/*
 *   12-hour clock format.  The default is '%#I:%M:%S %P' ('5:30:22 PM').
 */
#define DateFmt12Hour     12

/*
 *   24-hour clock format.  The default is '%H:%M' (17:30).
 */
#define DateFmt24Hour     13

/*
 *   24-hour clock format with seconds.  The default is '%H:%M:%S'
 *   (17:30:22). 
 */
#define DateFmt24HourSecs 14



/* ------------------------------------------------------------------------ */
/*
 *   TimeZone intrinsic class.  A TimeZone object represents a location entry
 *   in the IANA zoneinfo database.  It contains information on the
 *   location's wall clock time settings relative to universal time (UTC),
 *   allowing translations between local wall clock time and UTC.  The object
 *   stores the current clock settings in the location, the current ongoing
 *   rules for future switches between standard and daylight time (if
 *   applicable in the zone), and a full history of the past changes to the
 *   location's time settings, including standard/daylight time changes,
 *   redefinitions of the time zone, and changes in the location from one
 *   time zone to another.  (For example, some US cities that lie near zone
 *   borders have switched their time zones at various points in their
 *   history.)  The historical information for most zones goes back to the
 *   original establishment of standard time zones, typically in the late
 *   19th century, and for dates before that, the history usually includes
 *   the Local Mean Time settings for the location.  The history information
 *   allows the accurate reconstruction of the local time representation for
 *   virtually any date and time in the past, present, or future.
 *   
 *   Construction: 
 *   
 *.    new TimeZone() - creates a TimeZone object representing the local
 *.        system time zone.  Note that a TimeZone object created this way
 *.        will always represent the local zone.  If the game is saved on
 *.        one machine and restored on another that uses a different local
 *.        time zone, the restored object will represent the new machine's
 *.        local time zone after the restore.
 *   
 *.   new TimeZone(integer) - creates a TimeZone object representing the
 *.        given offset from UTC, in seconds.  Positive values are east of
 *.        UTC, negative values are west; for example, Pacific Standard
 *.        Time is 8 hours west of UTC, so you'd use -8*60*60 as the offset.
 *.        This type of TimeZone represents a fixed offset
 *   
 *.   new TimeZone('name') - creates a TimeZone object for the given zone
 *.        name.  This can use a number of formats:
 *.  
 *.        'America/New_York' - a name from the IANA zoneinfo database.
 *.            This is the best way to specify a zone because it's
 *.            unambiguous.
 *.  
 *.        'Z', 'UTC' - UTC (Universal Time Coordinated, also sometimes
 *.            called GMT/Greenwich Mean Time, or Z/Zulu time)
 *   
 *.        'EST' - a colloquial English abbreviation for a local time
 *.            zone.  Many of these are ambiguous, since some zone names
 *.            are used in several different regions.  For example,
 *.            'CST' is used in the US, Brazil, Australia, and China,
 *.            for time zones at different offsets from UTC.  When the
 *.            name is ambiguous, we use a fixed mapping that tends to
 *.            favor zones in the US and Europe.  This format is mapped
 *.            to a zoneinfo entry, so the actual underlying zone will
 *.            be one of the location-based entries.  For example, 'EST'
 *.            is mapped to 'America/New_York'.  This is important
 *.            because it means that the TimeZone object uses the full
 *.            rule set and history for the mapped zone, which might
 *.            differ from the history of the same nominal zone in other
 *.            locations; e.g., 'America/New_York' and 'Canada/Montreal'
 *.            are both on Eastern Time, but they have some differences
 *.            in their historical daylight savings rules.
 *   
 *.        'PST8PDT' - a POSIX TZ-style string, with the standard time
 *.            abbreviation, the standard time offset in hours (and
 *.            optionally minutes and seconds, with colons), the daylight
 *.            time abbreviation, and optionally the daylight time
 *.            offset (which defaults to one hour ahead of standard
 *.            time when not specified).  This is somewhat less ambiguous
 *.            than using just the zone abbreviation, but is still
 *   
 *.        '+0430' or 'UTC+0430' - four hours thirty minutes east of UTC;
 *.            this can also be written as '+4:30' or '+4:30:00'.  For a
 *.            whole number of hours, you can write it as simply '+4', for
 *.            example.  A negative number is west of UTC; e.g., Pacific
 *.            Standard Time is '-8'.  When using this format, the zone
 *.            represents a fixed time offset from UTC; it's not tied to
 *.            any location or named time zone, and doesn't use daylight
 *.            savings time.
 *   
 *   Note that the commonly used time zone names (e.g., PST, or Pacific
 *   Standard Time) aren't allowed.  The standard time zone names are
 *   ambiguous; for example, CST refers to at least four different time zones
 *   (USA Central Standard Time, Australia Central Standard Time, China
 *   Standard Time, and Cuba Summer Time).
 *   
 */
intrinsic class TimeZone 'timezone/030000' : Object
{
    /*
     *   Get the name or names for this timezone.  This returns a list of
     *   strings with the timezone's names, as defined in the IANA zoneinfo
     *   database.  The zoneinfo database names zones by location, usually
     *   using a combination of a continent major city, as in
     *   'America/New_York'.  Some zones have multiple aliases as a matter of
     *   convenience, such as when there are several major cities in a region
     *   that share the same timezone rules.  When a zone has aliases, the
     *   primary name is listed first, followed by the aliases.
     */
    getNames();

    /*
     *   Get the history item that applies to a given date, or the entire
     *   enumerated history of clock changes in this timezone.
     *   
     *   If 'date' is supplied, it must be a Date object.  This returns a
     *   list describing the single period in the timezone history that
     *   applies to the given date.  The list contains [date, offset, save,
     *   abbr], where 'date' is a Date object giving the starting date when
     *   the history item took effect; 'offset' is the offset from UTC in
     *   milliseconds of standard time in the zone during this period, using
     *   the zoneinfo convention that positive values are east of GMT; 'save'
     *   is the additional time added if daylight savings is in effect during
     *   this period, in milliseconds, or zero if standard time is in effect;
     *   and 'abbr' is a string giving the abbreviation for the zone during
     *   this period ('PST', 'EDT', etc).  Each period in the history is
     *   entirely in daylight or standard time; if 'save' is zero, standard
     *   time is in effect, otherwise daylight time.
     *   
     *   If 'date' is omitted or nil, this returns a list of all of the
     *   pre-computed changes in the timezone's history, including definition
     *   changes and daylight time changes.  Each list entry is a sublist of
     *   the form described above.
     *   
     *   In a full history, the first and last items are special.  The first
     *   item represents the settings in the location prior to the
     *   establishment of standard time zones; this is usually a "local mean
     *   time" setting (with abbreviation LMT) for the mean solar time at the
     *   location.  The last item represents the last pre-computed history
     *   entry, which is sometimes in the future; further transitions after
     *   this item might occur if the zone has ongoing rules.
     *   
     *   In many cases, the history list contains a number of periods that
     *   could have been inferred from the ongoing rules, so strictly
     *   speaking they don't need to be enumerated in the history.  When
     *   they're included, it's for faster run-time lookup.  TADS
     *   pre-computes rule-based transitions up to the present and a few
     *   years into the future, since history-based lookups are much faster
     *   than applying the rules.  We expect that the typical program will
     *   mostly work with dates close to the present time, so we pre-compute
     *   transitions for a few years into the future to speed things up for
     *   the typical case.  For changes after the last enumerated entry, TADS
     *   applies the rules, so transitions in the far future will be
     *   correctly figured when needed.
     */
    getHistory(date?);

    /*
     *   Get the ongoing rules that are in effect after the last enumerated
     *   history item.  This returns a list of the rules for future changes
     *   to the zone; each rule fires once annually, and encodes the day of
     *   year when the rule fires, and the new clock settings in effect after
     *   the rule fires.  Virtually all zones that use ongoing rules have
     *   exactly two: one for the annual change to daylight savings time in
     *   the spring, and one for the return to standard time in the fall.
     *   Each rule's firing date is specified in an abstract format designed
     *   to handle the variety of regional daylight savings schemes: "last
     *   Sunday in March", "second Sunday in November", "January 15", etc.
     *   
     *   Each rule in the list is described by a sublist: [abbr, offset,
     *   save, when, mm, typ, dd, wkday, time, zone].  'abbr' is a string
     *   with the time zone abbreviation while the rule is in effect; most
     *   zones use one abbreviation for standard time and another for
     *   daylight time, so each rule tells us the abbreviation to use while
     *   the rule is in effect.  'offset' is the standard time GMT offset, in
     *   milliseconds, while the rule is in effect, and 'save' is the
     *   additional offset for daylight savings time - so the full offset
     *   while the rule is in effect is offset+save.  'when' is a string with
     *   a human-readable description of the firing date: this will be of the
     *   form 'Mar last Sun' (for the last Sunday in March), 'Mar Sun>=1' for
     *   the first Sunday in March on or after March 1, 'Mar Sun<=28' for the
     *   last Sunday in March on or before March 28, 'Mar 7' for March 7, or
     *   'DOY 72' for the 72nd day of the year.  Next we have the same firing
     *   date information in a more computer-friendly format: 'mm' is the
     *   month number, 1-12 for Jan-Dec; 'typ' is an integer giving the type
     *   of date specification (0 for a fixed day of the month 'mm/dd', 1 for
     *   the last <weekday> of month <mm>, 2 for the first <weekday> of month
     *   <mm> on or after day <dd>, and 3 for the last <weekday> of month
     *   <mm> on or before day <dd>), 'dd' is the day of the month (which is
     *   ignored if 'typ' is 1), 'wkday' is the day of the week, 1-7 for
     *   Sunday-Saturday (which is ignored if 'typ' is 0).  'time' is the
     *   time of day the rule goes into effect, as milliseconds after
     *   midnight.  'zone' is a code for the timezone used to interpret the
     *   date and time; this is usually 'w' for local wall clock time (in
     *   other words, the local time zone that was in effect up until the
     *   moment this rule takes effect - so if this is a daylight savings
     *   rule, the rule is stated in terms of local standard time, and vice
     *   versa), but can also be 's' for local standard time (in other words,
     *   if the previous period was in daylight time, ignore that and read
     *   this rule's time in terms of local standard time instead), or 'u'
     *   for UTC.  Note that the zone has to be applied to the full
     *   date-and-time value, since an 's' or 'u' could conceivably cause the
     *   local date and the date in the rule's zone to differ by a day at the
     *   time of day of the rule.
     */
    getRules();

    /*
     *   Get the zone's location.  This returns a list: [country, lat, lon,
     *   comment], where 'country' is a string with the country code (a
     *   two-letter ISO 3166 code) for the country that contains the zone's
     *   main city, 'lat' is a string giving the latitude in the format +ddmm
     *   (degrees and minutes) or +ddmmss (and seconds), 'lon' is the
     *   longitude as a string in the format +dddmm or +dddmmss, and
     *   'comment' is a string with any comment text from the zoneinfo
     *   database.
     */
    getLocation();
}
