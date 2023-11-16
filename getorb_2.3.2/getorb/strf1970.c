/*STRF1970 -- Fortran mimic of the C strftime routine
 +
      FUNCTION STRF1970 (BUF, FMT, SEC)
      CHARACTER*(*) BUF, FMT
      INTEGER*4     SEC, STRF1970

  This function will make it easy on you to convert your system time
  (reckonned in seconds from 1970) into almost any sensible
  string representation (see 'Format specifications' below).

  At the function call the integer variable SEC will contain the number
  of UTC seconds since 1.0 January 1970. This time stamp is then transfered
  into a character string BUF based on the format specification FMT.

  The function returns an integer value that determines the lengths
  of the string after substitution according to the format, desregarding
  trailing spaces.

  If during substitution the capacity of the string buffer BUF is exceeded,
  the function returns a value STRF1970=0 and the buffer will be empty
  (all spaces).

  Limitation: FMT and BUF may be no longer than 256 characters.

  See also: STRF1985 and STRF2000

  Arguments:

      BUF    (output) : Output string. The lengths of this string is
                        returned as the value of the function STRF1970
      FMT     (input) : Format specification of the output string
                        (see below)
      SEC     (input) : Clock value in seconds from 1970, 1985, or 2000
      STRF1970  (out) : Functions all return the length of BUF

  Format specifications:

      A full list of the format specifications can be found in the manual
      of strftime ('man strftime'). This manual will give the exact list
      appropriate to your system. Nevertheless, we provide a list of the
      special sequences that can be used. But you can also use normal
      characters.

      %%  same as %
      %a  locale's abbreviated weekday name
      %A  locale's full weekday name
      %b  locale's abbreviated month name
      %B  locale's full month name
      %c  locale's appropriate date and time representation
      %C  locale's century number (the year divided by 100 and
          truncated to an integer) as a decimal number [00-99]
      %d  day of month ( 01 - 31 )
      %D  date as %m/%d/%y
      %e  day of month (1-31; single digits are preceded by a blank)
      %h  locale's abbreviated month name.
      %H  hour ( 00 - 23 )
      %I  hour ( 01 - 12 )
      %j  day number of year ( 001 - 366 )
      %KC locale's appropriate date and time representation
      %m  month number ( 01 - 12 )
      %M  minute ( 00 - 59 )
      %n  same as new-line
      %p  locale's equivalent of either AM or PM
      %r  time as %I:%M:%S [AM|PM]
      %R  time as %H:%M
      %S  seconds ( 00 - 61 ), allows for leap seconds
      %t  same as a tab
      %T  time as %H:%M:%S
      %U  week number of year ( 00 - 53 ), Sunday is the first day of week 1
      %w  weekday number ( 0 - 6 ), Sunday = 0
      %W  week number of year ( 00 - 53 ), Monday is the first day of week 1
      %x  locale's appropriate date representation
      %X  locale's appropriate time representation
      %y  year within century ( 00 - 99 )
      %Y  year as ccyy ( e.g. 1986)
      %Z  time zone name or no characters if no time zone exists

      Example: FMT="Today (%A %d/$b) I feel 100%%"
--
  26-Feb-1999 - Created by Remko Scharroo
  24-Aug-1999 - Fix string overflow
------------------------------------------------------------------------
*/
#include <string.h>
#include <time.h>
#include <sysdep.h>

#ifdef CAPITALS
#define strf1970 STRF1970
#endif

#ifdef UNDERSCOREBEFORE
#define strf1970 _strf1970
#endif                                                        
                                                              
#ifdef UNDERSCOREAFTER                                        
#define strf1970 strf1970_
#endif                                                        
 
int strf1970(buf, fmt, clock, bufsz, fmtsz)
char *buf, *fmt;
int bufsz, fmtsz;
long *clock;
{
    char temp1[256], temp2[256];
    int l = fmtsz - 1, len;
    strncpy(temp1, fmt, fmtsz);
    while (temp1[l] == ' ' && l >= 0) l--;
    temp1[l+1]='\0';
    len=strftime(temp2, bufsz+1, temp1, gmtime(clock));
    strncpy(buf, temp2, len);
    l=len;
    while (l < bufsz) {buf[l]=' '; l++; };
    return len;
}
