// IMPORTANT!
//
// This test must be run with the local time zone set to America/Los_Angeles.
// A few of the line items will show differences if the test is run in a
// different time zone, due to differences in daylight savings rules and
// calendar day alignment when converting to/from UTC.  If you see any
// errors when diffing against the reference log, double-check that you're
// running with the correct system local time zone.

#include <tads.h>
#include <date.h>
#include <bignum.h>

tzcache: object
    tz = static new TimeZone('America/New_York')
    refdate = static new Date('2012-03-01 America/New_York')
    errcnt = 0
;

main()
{
#if 0
    testCons('11-4-2007 02:35:17.123', '2007-11-04 02:35:17.123');
    testCons('11-4-2007 02:35:17.1234', '2007-11-04 02:35:17.123');
    testCons('11-4-2007 02:35:17.1235', '2007-11-04 02:35:17.124');
    testCons('11-4-2007 02:35:17.1225', '2007-11-04 02:35:17.122');
    testCons('11-4-2007 02:35:17.12250', '2007-11-04 02:35:17.122');
    testCons('11-4-2007 02:35:17.122500001', '2007-11-04 02:35:17.123');
    testCons('11-4-2007 02:35:17.1226', '2007-11-04 02:35:17.123');
#endif
    testCons('2/25/12', '2012-02-25');
    testCons('1-1-2012 12:00 America/New_York', '2012-01-01 12:00');
    testCons('7-1-2012 12:00 America/New_York', '2012-07-01 12:00');
    testCons('1-1-2012 13:00 EST', '2012-01-01 13:00'); // EST during EST
    testCons('7-1-2012 13:00 EST', '2012-07-01 14:00'); // EST during EDT
    testCons('1-1-2012 13:00 CST', '2012-01-01 14:00'); // CST during EST
    testCons('7-1-2012 13:00 CST', '2012-07-01 15:00'); // CST during EDT
    testCons('1-1-2012 11:00 PST', '2012-01-01 14:00'); // PST spec during EST
    testCons('1-1-2012 11:00 PDT', '2012-01-01 13:00'); // PST spec during EDT
    testCons('7-1-2012 13:00 PST', '2012-07-01 17:00'); // PST spec during EDT
    testCons('7-1-2012 13:00 PDT', '2012-07-01 16:00'); // PDT spec during EDT
    testCons(0, 'U', '1970-01-01 UTC');  // the Unix Epoch
    testCons(24*60*60, 'U', '1970-01-02 UTC');  // one day after the Unix Epoch
    testCons(365*24*60*60, 'U', '1971-01-01 UTC'); // one year after the Unix Epoch
    testCons(0.0, 'U', '1970-01-01 UTC');  // Unix Epoch with a BigNumber
    testCons(100.0*365.25*24*60*60, 'U', '2070-01-01 UTC');  // Epoch + 100 years
    testCons(1721119.5, 'J', '0000-03-01 UTC'); // our Epoch in Julian days
    testCons(0, 'J', '-4713-11-24 12:00 UTC'); // Julian day Epoch in our system
    testCons(0, 'J', 'J -4712-01-01 12:00 UTC'); // Julian Epoch, Julian calendar
    testCons(2299160.75, 'J', '1582-10-15 06:00 UTC');
    testCons(2299161 + 0.3333333333333333, 'J', '1582-10-15 20:00 UTC');
    testCons(1970, 1, 1, '1970-01-01');
    testCons(1970, 7, 1, '1970-07-01');
    testCons(1944, 3, 1, '1944-03-01');
    testCons(1945, 9, 1, '1945-09-01');
    testCons(2012, 7, 11, '2012-07-11');
    testCons(2012, 7, 11, 13, 17, 19, 0, '2012-07-11 13:17:19');
    testCons(2012, 7, 11, 13, 17, 19, 999, '2012-07-11 13:17:19.999');
    testCons(2012, 7, 11, 13, 17, 19, 9999, '2012-07-11 13:17:28.999');
    testCons(2012, 1, 60, '2012-02-29');
    testCons(2012, 1, 11, 'Z', '2012-01-10 19:00'); // standard time
    testCons(2012, 7, 11, 'Z', '2012-07-10 20:00'); // daylight time
    testCons(2012, 7, 11, 'UTC', '2012-07-10 20:00');
    testCons(2012, 7, 11, '-8', '2012-07-11 04:00'); // -8 = PST, show in EDT
    testCons(2012, 7, 11, '-08', '2012-07-11 04:00');  // -08 = PST, show EDT
    testCons(2012, 7, 11, '-0830', '2012-07-11 04:30');
    testCons('1-2-1970', '1970-01-02');
    testCons('5-9', '05-09');
    testCons('5-09', '05-09');
    testCons('5-2009', '2009-05-01');
    testCons('12-22-78', '1978-12-22');
    testCons('1-17-2006', '2006-01-17');
    testCons('1-2-1970', '1970-01-02');
    testCons('2008-6-30', '2008-06-30');
    testCons('2008-06-30', '2008-06-30');
    testCons('1978-12-22', '1978-12-22');
    testCons('2008-6', '2008-06-01');
    testCons('2008-06', '2008-06-01');
    testCons('1978-12', '1978-12-01');
    testCons('2008-6-30', '2008-06-30');
    testCons('78-12-22', '1978-12-22');
    testCons('8-6-21', '2021-08-06');
    testCons('30-6-2008', 'Error');
    testCons('6-30-2008', '2008-06-30');
    testCons('22.12\t1978', '1978-01-01 22:12');
    testCons('12.22\t1978', '1978-12-22');
    testCons('30.6.08', 'Error');
    testCons('22\t12\t78', 'Error');
    testCons('12\t22\t78', '1978-12-22');
    testCons('30-June 2008', '2008-06-30');
    testCons('22DEC78', '1978-12-22');
    testCons('14 march 1879', '1879-03-14');
    testCons('14 III 1879', '1879-03-14');
    testCons('June 2008', '2008-06-01');
    testCons('DEc1978', '1978-12-01');
    testCons('march 1879', '1879-03-01');
    testCons('July 1st,  2008', '2008-07-01');
    testCons('april 17,1790', '1790-04-17');
    testCons('MAY.9,78', '1978-05-09');
    testCons('july 1st', '07-01');
    testCons('apr 17', '04-17');
    testCons('May.9', '05-09');
    testCons('1 July', '07-01');
    testCons('17 april', '04-17');
    testCons('17 apr', '04-17');
    testCons('22march', '03-22');
    testCons('9.MAY', '05-09');
    testCons('May-09-78', '1978-05-09');
    testCons('Apr-17-1790', '1790-04-17');
    testCons('78-Dec-22', '1978-12-22');
    testCons('22-Dec-78', '1978-12-22');
    testCons('22-Dec-23', '2023-12-22');
    testCons('1814-MAY-17', '1814-05-17');
    testCons('1978', '1978-01-01');
    testCons('78', 'Error');
    testCons('2008', '2008-01-01');
    testCons('March', '03-01');
    testCons('jun', '06-01');
    testCons('DEC', '12-01');
    testCons('03:00', '03:00');
    testCons('15:00', '15:00');
    testCons('15:00 5-10-2010', '2010-05-10 15:00');
    testCons('11-4-2007 02:35', '2007-11-04 02:35');
    testCons('june 27, 1991', '1991-06-27');
    testCons('15810726', '1581-07-26');
    testCons('19780417', '1978-04-17');
    testCons('18140517', '1814-05-17');
    testCons('08-06-30', '2030-08-06');
    testCons('-0002-07-26', '-0002-07-26');
    testCons('+1978-04-17', '1978-04-17');
    testCons('1814-05-17', '1814-05-17');
    testCons('3:00 pm', '15:00');
    testCons('3:00', '03:00');
    testCons('03:00', '03:00');
    testCons('3PM', '15:00');
    testCons('12PM', '12:00');
    testCons('11AM', '11:00');
    testCons('13:05PM', 'Error');
    testCons('12:05 P.M.', '12:05');
    testCons('1:12 P.M.', '13:12');
    testCons('12:10 am', '00:10');
    testCons('7:15 p.m.', '19:15');
    testCons('15/Oct/2000:13:55:36 -0700', '2000-10-15 16:55:36'); // -0700=PDT
    testCons('2008:08:07 18:11:31', '2008-08-07 18:11:31');
    testCons('2011W526', '2011-12-31');
    testCons('2011W527', '2012-01-01');
    testCons('2012W01', '2012-01-02');
    testCons('2008W27', '2008-06-30');
    testCons('2009-W28', '2009-07-06');
    testCons('2010W273', '2010-07-07');
    testCons('1988W13-3', '1988-03-30');
    testCons('1988W13-4', '1988-03-31');
    testCons('1988W13-5', '1988-04-01');
    testCons('1876W01-1', '1876-01-03');
    testCons('1875W52-1', '1875-12-27');
    testCons('1875W52-5', '1875-12-31');
    testCons('1875W52-6', '1876-01-01');
    testCons('1875W52-7', '1876-01-02');
    testCons('2052W527', '2052-12-29');
    testCons('2008-08-07 18:11:31', '2008-08-07 18:11:31');
    testCons('2008.197', '2008-07-15');
    testCons('2008199', '2008-07-17');
    testCons('2011365', '2011-12-31');
    testCons('2011366', '2012-01-01');
    testCons('2012365', '2012-12-30');
    testCons('2012366', '2012-12-31');
    testCons('2012367', 'Error');
    testCons('@0', '1969-12-31 19:00');
    testCons('@1215282385', '2008-07-05 14:26:25');
    testCons('2008-07-01T22:35:17.02', '2008-07-01 22:35:17.020');
    testCons('2011-05-15T11:22:33.44-06:00', '2011-05-15 13:22:33.440');
    testCons('20080701t223807', '2008-07-01 22:38:07');
    testCons('2008-7-1T9:3:37', '2008-07-01 09:03:37');
    testCons('1900-02-29', '1900-03-01'); // 1900 isn't a leap year
    testCons('1800-02-29', '1800-03-01'); // ... neither is 1800
    testCons('1700-02-29', '1700-03-01'); // ... or 1700
    testCons('2000-02-29', '2000-02-29'); // ... but 2000 is
    testCons('1600-02-29', '1600-02-29'); // ... as is 1600
    testCons('March 7, AD 37', '0037-03-07');
    testCons('March 11, 37 A.D.', '0037-03-11');
    testCons('19th November, C.E. 437', '0437-11-19');
    testCons('3 January, AD 1', '0001-01-03');
    testCons('2 January, AD 1', '0001-01-02');
    testCons('1 January, AD 1', '0001-01-01');
    testCons('Dec 31, 1 BC', '0000-12-31');
    testCons('Dec 30, 1 BC', '0000-12-30');
    testCons('Mar 1, 1 BC', '0000-03-01');
    testCons('Feb 29, 1 BC', '0000-02-29');
    testCons('Feb 28, 1 BC', '0000-02-28');
    testCons('3 January, 1 B.C.', '0000-01-03');
    testCons('April 15, 32 BCE', '-0031-04-15');
    testCons('1 BC February 29', '0000-02-29'); // 1 BC (year 0) is a leap year
    testCons('2 BC February 29', '-0001-03-01'); // 2 BC (-1) isn't
    testCons('4 BC February 29', '-0003-03-01'); // 4 BC (-3) isn't
    testCons('5 BC February 29', '-0004-02-29'); // 5 BC (-4) is a leap year
    testCons('101 BC February 29', '-0100-03-01'); // 101 BC (-100) isn't
    testCons('401 BC February 29', '-0400-02-29'); // 401 BC (-400) is
    testCons('10,500 AD Jan 3', '10500-01-03');
    testCons('5,000,000 BC Feb 7', '-4999999-02-07');
    testCons('5000000 Mar 31', '5000000-03-31');
    testCons('6000000 Apr 1', 'Error');

    testWeekday('2010-9', 1, 1);
    testWeekday('2010-10', 1, 1);
    testWeekday('2010-11', 1, 1);
    testWeekday('2010-9', 2, 2);
    testWeekday('2010-10', 3, 3);
    testWeekday('2010-11', 4, 4);
    testWeekday('2010-9', 1, -1);
    testWeekday('2010-10', 1, -2);
    testWeekday('2010-11', 2, -3);
    testWeekday('2010-12', 3, -4);
    testWeekday('2010-8', 7, 2);

    testISOWeekDate('2005-01-01', '2004-W53-6');
    testISOWeekDate('2005-01-02', '2004-W53-7');
    testISOWeekDate('2005-01-03', '2005-W01-1');
    testISOWeekDate('2005-12-31', '2005-W52-6');
    testISOWeekDate('2006-01-01', '2005-W52-7');
    testISOWeekDate('2006-01-02', '2006-W01-1');
    testISOWeekDate('2006-12-31', '2006-W52-7');
    testISOWeekDate('2007-01-01', '2007-W01-1');
    testISOWeekDate('2007-12-31', '2008-W01-1');
    testISOWeekDate('2008-01-01', '2008-W01-2');
    testISOWeekDate('2008-12-29', '2009-W01-1');
    testISOWeekDate('2008-12-31', '2009-W01-3');
    testISOWeekDate('2009-01-01', '2009-W01-4');
    testISOWeekDate('2009-12-31', '2009-W53-4');
    testISOWeekDate('2010-01-01', '2009-W53-5');
    testISOWeekDate('2010-01-02', '2009-W53-6');
    testISOWeekDate('2010-01-03', '2009-W53-7');
    testISOWeekDate('2008-12-28', '2008-W52-7');
    testISOWeekDate('2008-12-29', '2009-W01-1');
    testISOWeekDate('2008-12-30', '2009-W01-2');
    testISOWeekDate('2008-12-31', '2009-W01-3');
    testISOWeekDate('2009-01-01', '2009-W01-4');

    testInterval('2010-2-1', [1]);
    testInterval('2010-2-5', [0, 1]);
    testInterval('2010-3-7', [1, 2]);
    testInterval('2010-2-1', [0, 0, -1]);
    testInterval('2010-3-1', [0, 0, -5]);
    testInterval('2010-3-1', [0, -1]);
    testInterval('2010-8-7', [0, 0, 0, 5, 30]);
    testInterval('2010-8-7', [0, 0, 0, 1, 2, 100.7264]);
    testInterval('2010-8-7', [0, 0, 60]);
    testInterval('2010-8-7', [0, 0, 90]);
    testInterval('2010-8-7', [0, 0, 0, 24*10]);
    testInterval('2010-8-7', [0, 0, 0, 0, 24*60*10]);

    testAdd('2010-1-1', 7);
    testAdd('2010-1-1', -1);
    testAdd('2010-1-1', 0.5);
    testAdd('2010-1-1', -0.25);

    testSub('2011-1-1', 7);
    testSub('2011-1-1', -1);
    testSub('2011-1-1', .5);
    testSub('2011-1-1', -0.25);

    testSub('2011-1-2', '2011-1-1');
    testSub('2011-1-1', '2011-1-2');
    testSub('2011-2-1', '2011-1-1');
    testSub('2011-1-1 12:00', '2011-1-1');
    testSub('2011-2-1 12:00', '2011-1-1');
    testSub('2011-2-1', '2011-1-1 12:00');
    testSub('2011-1-1 12:00', '2011-1-1 18:00');

    testCmp('2011-1-1', '2011-1-1');
    testCmp('2011-1-1', '2011-1-1 13:00');
    testCmp('2011-1-1 18:00', '2011-1-1 12:00');
    testCmp('2011-1-1 18:00', '2011-1-2 12:00');

    if (tzcache.errcnt > 0)
        "\b*** <<tzcache.errcnt>> mismatch<<if tzcache.errcnt > 1>>es<<end>>
        found ***\n";
}

testCons([args])
{
    local curdate = tzcache.refdate.formatDate(
        '%Y-%m-%d %H:%M:%S.%N', tzcache.tz);
    local ref = args[args.length()];
    args = args.sublist(1, -1);

    /* check for a timezone encoded in the reference string */
    local reftz = tzcache.tz;
    if (rexMatch('(.*)%s+(<alpha|/>+)$', ref) != nil)
    {
        reftz = new TimeZone(rexGroup(2)[3]);
        ref = rexGroup(1)[3];
    }

    local formatter = &formatDate;
    if (ref.startsWith('J '))
    {
        ref = ref.substr(3);
        formatter = &formatJulianDate;
    }

    /* 
     *   if we only have one argument, or the last argument isn't a time
     *   zone name or TimeZone object, add the default time zone 
     */
    local dispargs = args;
    if ((args.length() == 1 && dataType(args[1]) == TypeSString)
        || (args.length() > 1
            && !(dataType(args[args.length()]) == TypeSString
                 || (dataType(args[args.length()]) == TypeObject
                     && args[args.length()].ofKind(TimeZone)))))
        args += reftz;

    /* if we have a string plus a timezone, add the reference date */
    if (args.length() == 2
        && dataType(args[1]) == TypeSString
        && (dataType(args[2]) == TypeObject && args[2].ofKind(TimeZone)))
        args += tzcache.refdate;

    /* fill in missing elements in the reference string */
    if (ref == '')
        ref = curdate;
    if (rexMatch('%d%d-%d%d$', ref) != nil)
        ref = '<<curdate.substr(0, 4)>>-<<ref>> 00:00:00.000';
    if (rexMatch('.* %d%d:%d%d:%d%d$', ref) != nil)
        ref += '.000';
    if (rexMatch('(.* )?%d%d:%d%d$', ref) != nil)
        ref += ':00.000';
    if (rexMatch('%d%d:%d%d:%d%d%.%d%d%d$', ref) != nil)
        ref = curdate.substr(1, 11) + ref;
    if (rexMatch('-?%d{4,7}-%d%d-%d%d$', ref) != nil)
        ref += ' 00:00:00.000';
    
    "new Date(<<showList(dispargs)>>) -&gt; ";
    try
    {
        local s = new Date(args...)
            .(formatter)('%Y-%m-%d %H:%M:%S.%N', reftz);
        "<<s>>";

        if (s != ref)
        {
            " *** Mismatch (ref=<<ref>>) ***";
            tzcache.errcnt += 1;
        }
        "\n";
    }
    catch (Exception e)
    {
        "error ";
        if (ref == 'Error')
            " (ok)\n";
        else
        {
            "<<e.displayException()>>\n";
            tzcache.errcnt += 1;
        }
    }
}

testWeekday(date, wday, which)
{
    local th = [1->'st', 2->'nd', 3->'rd', *->'th'];
    local days = ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'];
    "<<abs(which)>><<th[abs(which)]>> <<days[wday]>> on or <<if which > 0>>
    after<<else>>before<<end>> <<date>> -&gt; <<
      new Date(date).findWeekday(wday, which)>>\n";
}

testISOWeekDate(date, iso)
{
    local d = new Date('<<date>> America/Los_Angeles');
    local iw = d.getISOWeekDate(tzcache.tz);
    iw = '<<iw[1]>>-W<<%02d iw[2]>>-<<iw[3]>>';
    "ISO week date: <<date>> -&gt; <<iw>>";

    if (iw != iso)
    {
        " *** Mismatch ***";
        tzcache.errcnt += 1;
    }
    "\n";
}

testInterval(date, ival)
{
    "<<date>> + interval <<showList(ival)>> -&gt; <<
      new Date(date).addInterval(ival)>>\n";
}

testAdd(date, a)
{
    "<<date>> + <<a>> -&gt; <<new Date(date) + a>>\n";
}

testSub(date, a)
{
    "<<date>> - <<a>> -&gt; <<
      new Date(date) - (dataType(a) == TypeSString ? new Date(a) : a)>>\n";
}

testCmp(a, b)
{
    "compare <<a>>, <<b>>:
    <<if a < b>>less than; <<end>>
    <<if a <= b>>less or equal; <<end>>
    <<if a > b>>greater than; <<end>>
    <<if a >= b>>greater or equal; <<end>>
    <<if a == b>>equal<<else>>unequal<<end>>\n";
}

showList(l)
{
    for (local i in 1..l.length())
    {
        local e = l[i];
        "<<if i > 1>>, <<end>>";
        if (dataType(e) == TypeObject && e.ofKind(BigNumber))
            "<<%.4f e>>";
        else if (dataType(e) == TypeSString)
            "'<<e>>'";
        else
            "<<l[i]>>";
    }
}
