/*
 * Copyright 2014-2018 Neueda Ltd.
 */

#include "utils.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <time.h>
#include <sys/time.h>

#define SECONDS_PER_DAY (86400)

string
utils_getUtcDateTime (timePrecision prec)
{
    char       datetime[27] = {0};
    struct     timespec ts;
    time_t     secs;
    struct tm* tm;
    char       ns[11];
    uint64_t   nanos;
    
    clock_gettime (CLOCK_REALTIME, &ts);

    secs = ts.tv_sec;
    tm = gmtime (&secs);
    nanos = ts.tv_nsec;

#ifndef WIN32
    strftime (datetime, sizeof (datetime), "%Y%0m%0d-%H:%M:%S", tm);
#else
    strftime (datetime, sizeof (datetime), "%Y%m%d-%H:%M:%S", tm);
#endif

    snprintf (ns, sizeof (ns), ".%09llu", nanos);
    strncpy (datetime + strlen (datetime), ns, prec + 1);

    return string (datetime);
}

string
utils_getUtcDate ()
{
    char       date[9] = {0};
    time_t     t;
    struct tm* tm;

    t = time (NULL);
    tm = gmtime (&t);

    // 0 Flag not available in strftime for windows
#ifndef WIN32
    strftime (date, sizeof (date), "%Y%0m%0d", tm);
#else
    strftime (date, sizeof (date), "%Y%m%d", tm);
#endif

    return string (date);
}

string
utils_getUtcTime ()
{
    char       utc[9] = {0};
    time_t     t;
    struct tm* tm;

    t = time (NULL);
    tm = gmtime (&t);

    strftime (utc, sizeof (utc), "%H:%M:%S", tm);
    return string (utc);
}

/* return the number of seconds since midnight */
time_t
utils_secondsSinceMidnight ()
{
    time_t t1, t2;
    struct tm tms;

    time (&t1);
    localtime_r (&t1, &tms);

    tms.tm_hour = 0;
    tms.tm_min = 0;
    tms.tm_sec = 0;
    t2 = mktime (&tms);

    return t1 - t2;
}

// ltrim whitespace from start
void utils_ltrimWhiteSpace (string& s)
{
    s.erase (s.begin (),
             find_if (s.begin (),
                      s.end (),
                      not1 (ptr_fun<int, int> (isspace))));
}

// rtrim whitespace from end
void utils_rtrimWhiteSpace (string& s)
{
    s.erase (find_if (s.rbegin (), s.rend (),
             not1 (ptr_fun<int, int> (isspace))).base (), s.end ());
}

// trim from both ends
void utils_trimWhiteSpace (string& s)
{
    utils_ltrimWhiteSpace (s);
    utils_rtrimWhiteSpace (s);
}

/* Get the current time in HH:MM:SS.MS format */
void
utils_localTime (string& s)
{
    char tim[32] = {0};
    time_t t;
    struct tm tmp;
    struct timeval tv;

    t = time (NULL);
#ifndef _MSC_VER
    localtime_r (&t, &tmp);
#else
    localtime_s (&tmp, &t);
#endif
    strftime (tim, sizeof(tim), "%H:%M:%S", &tmp);
    s.assign (tim);
    gettimeofday (&tv, NULL);
    snprintf (tim, 4, ".%03d", (int)tv.tv_usec/1000);
    s += tim;
}

bool
utils_parseAddressPort (const string& s,
                        string& address,
                        uint16_t& port)
{
    size_t idx;
    string portStr;
    int p;

    idx = s.find_first_of (':');
    if (idx == string::npos)
        return false;

    address.assign (s, 0, idx);
    portStr.assign (s, idx + 1, s.size () - (idx + 1));
    if (!utils_parseNumber (portStr, p))
        return false;

    if (p < 0 || p > 65535)
        return false;

    port = p;
    return true;
}

int
utils_millisSinceMidnight ()
{
    struct timeval tv;
    int m = 0;

    gettimeofday (&tv, NULL);
    m = ((tv.tv_sec % SECONDS_PER_DAY) * 1000 + tv.tv_usec / 1000);

    return m;
}

void
utils_milliMicroSinceMidnight (int& m, int& mic)
{
    struct timeval tv;
    m = mic = 0;

    gettimeofday (&tv, NULL);
    m = ((tv.tv_sec % SECONDS_PER_DAY) * 1000 + tv.tv_usec / 1000);
    mic = tv.tv_usec;
}

uint64_t
utils_nanosSinceMidnight ()
{
    struct timespec ts;
    clock_gettime (CLOCK_REALTIME, &ts);

    uint64_t numberSecondsSinceMidnight = ts.tv_sec % SECONDS_PER_DAY;
    uint64_t numberOfSecondsSinceMidnightInNanos = numberSecondsSinceMidnight * (uint64_t)1000000000;
    
    return numberOfSecondsSinceMidnightInNanos + ts.tv_nsec;
}

#ifndef WIN32
/* parses a time string to a tm */
bool
utils_parseTime (const char* stm, tm& t)
{
    struct timeval tv;
    gettimeofday (&tv, NULL);
    time_t now = tv.tv_sec;
    localtime_r (&now, &t);

    char* ret = strptime (stm, "%H:%M:%S", &t);

    if (ret == NULL)
        return false;

    return true;
}

/* parses a datetime string to a tm */
bool
utils_parseDateTime (const char* stm, tm& t, const char* fmt)
{
    struct timeval tv;
    gettimeofday (&tv, NULL);
    time_t now = tv.tv_sec;
    localtime_r (&now, &t);

    char* ret = strptime (stm, fmt, &t);

    if (ret == NULL)
        return false;

    return true;
}
#endif

/* parses a string to bool */
bool
utils_parseBool (const string& val, bool& out)
{
    string s (val);
    for (u_int i = 0; i < s.length (); i++)
        s[i] = toupper (s[i]);

    if (s == "TRUE" || s == "T" || s == "Y")
    {
        out = true;
        return true;
    }
    else if (s == "FALSE" || s == "F" || s == "N")
    {
        out = false;
        return true;
    }

    return false;
}

bool
utils_findFileInEnvPath (const string& variable,
                         const string& filename,
                         string& result,
                         const string& delim)
{
    string fp;

    if(const char* e = getenv (variable.c_str ()))
    {
        string envVar (e);
        size_t pos = 0;
        string token;

        while ((pos = envVar.find (delim)) != string::npos)
        {
            token = envVar.substr (0, pos);
            envVar.erase (0, pos + delim.length ());

            fp = utils_filePathJoin (token, filename);
            if (utils_checkFileAccessible (fp))
            {
                result.assign (fp);
                return true;
            }
        }

        fp = utils_filePathJoin (envVar, filename);
        if (utils_checkFileAccessible (fp))
        {
            result.assign (fp);
            return true;
        }
    }

    string cwd (".");
    fp = utils_filePathJoin (cwd, filename);
    if (utils_checkFileAccessible (fp))
    {
        result.assign (fp);
        return true;
    }

    return false;
}
