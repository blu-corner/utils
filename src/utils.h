/*
 * Copyright 2014-2018 Neueda Ltd.
 */

#ifndef _UTILS_HDR_
#define _UTILS_HDR_

#include "common.h"

#include <string>
#include <math.h>
#include <sys/time.h>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <sstream>

using namespace std;

/* return the number of seconds since midnight */
time_t utils_secondsSinceMidnight ();

/* ltrim whitespace */
void utils_ltrimWhiteSpace (string &s);

/* rtrim whitespace */
void utils_rtrimWhiteSpace (string &s);

/* trim all whitespace */
void utils_trimWhiteSpace (string &s);

/* Parse a address:port as address and port */
bool utils_parseAddressPort (const string& s,
                             string& address,
                             uint16_t& port);

/* Get the current time in HH:MM:SS.MS format */
void utils_localTime (string& s);

/* Get the current time in millis from midnight */
int utils_millisSinceMidnight ();

/* Get the millis and micros since midnight */
void utils_milliMicroSinceMidnight (int& m, int& mic);

/* Check if doubles are equal */
bool SBF_INLINE utils_doubleEq (double d1, double d2)
{
    return fabs (d1 - d2) < DOUBLE_EPSILON;
}

/* check if double d1 is less than double d2 */
bool SBF_INLINE utils_doubleLt (double d1, double d2)
{
    return (d2 - d1) > DOUBLE_EPSILON;
}

/* check if double d1 is greater than double d2 */
bool SBF_INLINE utils_doubleGt (double d1, double d2)
{
    return (d1 - d2) > DOUBLE_EPSILON;
}

/* check if double d1 is greater than or equal to double d2 */
bool SBF_INLINE utils_doubleGtEq (double d1, double d2)
{
    return !utils_doubleLt (d1, d2);
}

/* check if double d1 is less than or equal to double d2 */
bool SBF_INLINE utils_doubleLtEq (double d1, double d2)
{
    return !utils_doubleGt (d1, d2);
}

/* parses a number string to a double/int */
template <typename T>
bool utils_parseNumber (const string& s, T& val)
{
    istringstream iss(s);
    iss >> val;

    if (iss.fail ())
        return false;
    return true;
}

/* parse time string as tm */
bool utils_parseTime (const char* stm, tm& t);

/* parse date time string as tm */
bool utils_parseDateTime (const char* stm,
                          tm& t,
                          const char* fmt = "%Y/%m/%d %H:%M:%S");

/* parse string to bool */
bool utils_parseBool (const string& s, bool& out);

#endif /* _UTILS_HDR_ */
