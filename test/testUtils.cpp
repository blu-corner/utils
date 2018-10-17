/*
 * Copyright 2014-2018 Neueda
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "utils.h"
#include <time.h>
#include <list>
#include <iostream>

using namespace std;

TEST (UTILS, SECONDS_SINCE_MIDNIGHT)
{
    time_t reference, measure;
    for (int i = 0; i < 2; ++i) // two tries in case of second call change
    {
        measure = utils_secondsSinceMidnight ();
        time_t now = time (NULL);
        struct tm *tm = localtime (&now);
        reference = tm->tm_sec + tm->tm_min * 60 + tm->tm_hour * 3600;
        if (reference == measure)
        {
           break;
        }
    }
    ASSERT_EQ (reference, measure);
}

#ifndef WIN32
TEST(UTILS, TEST_PARSETIME_CORRECT_INPUT)
{
    std::string time = "18:31:01";
    struct tm rawtime;
    memset(&rawtime, 0, sizeof(struct tm));

    bool result = utils_parseTime(time.c_str(), rawtime);
    ASSERT_EQ(rawtime.tm_hour, 18);
    ASSERT_EQ(rawtime.tm_min, 31);
    ASSERT_EQ(rawtime.tm_sec, 1);
    ASSERT_TRUE(result);
}

TEST(UTILS, PARSETIME_INCORRECT_SECONDS)
{
    std::string time = "18:31:*";
    struct tm rawtime;
    memset(&rawtime, 0, sizeof(struct tm));

    bool result = utils_parseTime(time.c_str(), rawtime);
    ASSERT_EQ(rawtime.tm_hour, 18);
    ASSERT_EQ(rawtime.tm_min, 31);
    ASSERT_FALSE(result);
}

TEST(UTILS, PARSEDATETIME_CORRECT_INPUT)
{
    std::string time = "2017/02/25 18:31:01";
    struct tm rawtime;
    memset(&rawtime, 0, sizeof(struct tm));

    bool result = utils_parseDateTime(time.c_str(), rawtime);
    ASSERT_EQ(rawtime.tm_year + 1900, 2017);
    ASSERT_EQ(rawtime.tm_mon + 1, 2);
    ASSERT_EQ(rawtime.tm_mday, 25);
    ASSERT_EQ(rawtime.tm_hour, 18);
    ASSERT_EQ(rawtime.tm_min, 31);
    ASSERT_EQ(rawtime.tm_sec, 1);
    ASSERT_TRUE(result);
}

TEST(UTILS, PARSEDATETIME_INCORRECT_DAYS)
{
    std::string time = "2017/02/* 18:31:01";
    struct tm rawtime;
    memset(&rawtime, 0, sizeof(struct tm));

    bool result = utils_parseDateTime(time.c_str(), rawtime);
    //We can ensure just this two fields tm_year tm_mon
    //the rest are taken from the machine local time
    ASSERT_EQ(rawtime.tm_year + 1900, 2017);
    ASSERT_EQ(rawtime.tm_mon + 1, 2);
    ASSERT_FALSE(result);
}
#endif

TEST(UTILS, PARSEINT_CORRECT_INPUT)
{
    std::string value = "5";
    int parsedValue = 0;

    bool result = utils_parseNumber(value, parsedValue);

    ASSERT_EQ(parsedValue,5);
    ASSERT_TRUE(result);
}

TEST(UTILS, PARSEINT_INCORRECT_INPUT)
{
    int parsedValue = 0;
    std::string value = "A";

    bool result = utils_parseNumber(value, parsedValue);
    ASSERT_FALSE(result);

    value = "";
    result = utils_parseNumber(value,parsedValue);
    ASSERT_FALSE(result);
}

TEST(UTILS, PARSEDOUBLE_CORRECT_INPUT)
{
    std::string value = "5.01";
    double parsedValue = 0;

    bool result = utils_parseNumber(value, parsedValue);
    ASSERT_EQ(parsedValue, 5.01);
    ASSERT_TRUE(result);


    value = "5";
    result = utils_parseNumber(value, parsedValue);
    ASSERT_EQ(parsedValue, 5.0);
    ASSERT_TRUE(result);
}

TEST(UTILS, PARSEDOUBLE_INCORRECT_INPUT)
{
    double parsedValue = 0;
    std::string value = "A";

    bool result = utils_parseNumber(value, parsedValue);
    ASSERT_FALSE(result);

    value = "";
    result = utils_parseNumber(value,parsedValue);
    ASSERT_FALSE(result);
}

TEST(UTILS, LEFT_TRIM)
{
    std::string valueToTrim = " some string ";
    std::string expecteResult = "some string ";
    utils_ltrimWhiteSpace(valueToTrim);
    ASSERT_EQ(valueToTrim,expecteResult);
}

TEST(UTILS, RIGHT_TRIM)
{
    std::string valueToTrim = " some string ";
    std::string expecteResult = " some string";
    utils_rtrimWhiteSpace(valueToTrim);
    ASSERT_EQ(valueToTrim,expecteResult);
}

TEST(UTILS, TRIM_ALL)
{
    std::string valueToTrim = " some string ";
    std::string expecteResult = "some string";
    utils_trimWhiteSpace(valueToTrim);
    ASSERT_EQ(valueToTrim,expecteResult);
}

TEST(UTILS, PARSE_ADDRES_OK)
{
    std::string valueToParse = "127.0.0.1:8080";
    std::string address = "";
    uint16_t port = 0;
    bool result = utils_parseAddressPort(valueToParse,address,port);

    ASSERT_EQ(address,"127.0.0.1");
    ASSERT_EQ(port,8080);
    ASSERT_TRUE(result);
}

TEST(UTILS, PARSE_ADDRES_BAD_FORMAT_SEPARATOR)
{
    std::string valueToParse = "127.0.0.1-8080";
    std::string address = "";
    uint16_t port = 0;
    bool result = utils_parseAddressPort(valueToParse,address,port);

    ASSERT_FALSE(result);
}

TEST(UTILS, PARSE_ADDRES_PORT_BADFORMAT)
{
    std::string valueToParse = "127.0.0.1:****";
    std::string address = "";
    uint16_t port = 0;
    bool result = utils_parseAddressPort(valueToParse,address,port);

    ASSERT_FALSE(result);
}

TEST(UTILS, PARSE_ADDRES_PORT_INVALID_RANGE)
{
    std::string valueToParse = "127.0.0.1:65536";
    std::string address = "";
    uint16_t port = 0;

    bool result = utils_parseAddressPort(valueToParse,address,port);
    ASSERT_FALSE(result);

    valueToParse = "127.0.0.1:-1";
    result = utils_parseAddressPort(valueToParse,address,port);
    ASSERT_FALSE(result);
}

TEST (UTILS, NON_EMPTY_LOCAL_TIME)
{
    std::string time_string;
    utils_localTime (time_string);
    ASSERT_FALSE(time_string.empty());
}

TEST(UTILS, DOUBLE_ARE_EQUAL_TRUE)
{
    double d1 = 1.0000005;
    double d2 = 1.0000001;

    bool result = utils_doubleEq(d1,d2);
    ASSERT_TRUE(result);
}

TEST(UTILS, DOUBLE_ARE_EQUAL_FALSE)
{
    double d1 = 1.000005;
    double d2 = 1.000001;

    bool result = utils_doubleEq(d1,d2);
    ASSERT_FALSE(result);
}

TEST(UTILS, DOUBLE_LESS_THAN_TRUE)
{
    double d1 = 1.00001;
    double d2 = 1.00005;

    bool result = utils_doubleLt(d1,d2);
    ASSERT_TRUE(result);
}

TEST(UTILS, DOUBLE_LESS_THAN_FALSE)
{
    double d1 = 1.00001;
    double d2 = 1.00005;

    bool result = utils_doubleLt(d2,d1);
    ASSERT_FALSE(result);
}

TEST(UTILS, DOUBLE_GREATER_THAN_TRUE)
{
    double d1 = 1.00005;
    double d2 = 1.00001;

    bool result = utils_doubleGt(d1,d2);
    ASSERT_TRUE(result);
}

TEST(UTILS, DOUBLE_GREATER_THAN_FALSE)
{
    double d1 = 1.00005;
    double d2 = 1.00001;

    bool result = utils_doubleGt(d2,d1);
    ASSERT_FALSE(result);
}

TEST(UTILS, DOUBLE_LESS_THAN_OR_EQUAL_TRUE)
{
    double d1 = 1.00001;
    double d2 = 1.00005;
    bool result = utils_doubleLtEq(d1,d2);
    ASSERT_TRUE(result);

    d1 = 1.00000;
    d2 = 1.00000;
    result = utils_doubleLtEq(d1,d2);
    ASSERT_TRUE(result);
}

TEST(UTILS, DOUBLE_LESS_THAN_OR_EQUAL_FALSE)
{
    double d1 = 1.00001;
    double d2 = 1.00005;

    bool result = utils_doubleLtEq(d2,d1);
    ASSERT_FALSE(result);
}

TEST(UTILS, DOUBLE_GREATER_THAN_OR_EQUAL_TRUE)
{
    double d1 = 1.00005;
    double d2 = 1.00001;

    bool result = utils_doubleGtEq(d1,d2);
    ASSERT_TRUE(result);

    d1 = 1.00005;
    d2 = 1.00005;
    result = utils_doubleGtEq(d1,d2);
    ASSERT_TRUE(result);
}

TEST(UTILS, DOUBLE_GREATER_THAN_OR_EQUAL_FALSE)
{
    double d1 = 1.00005;
    double d2 = 1.00001;

    bool result = utils_doubleGtEq(d2,d1);
    ASSERT_FALSE(result);
}

TEST(UTILS, PARSE_STRING_TO_BOOL_PASS_FALSE)
{
    string fVals[6] = {"false", "False", "f", "F", "n", "N"};
    string tVals[6] = {"true", "True", "t", "T", "y", "Y"};
    bool res;
    bool out;

    int i;
    for (i = 0; i < 6; i++)
    {
        res = utils_parseBool (fVals[i], out);
        ASSERT_TRUE (res);
        ASSERT_FALSE (out);
    }

    for (i = 0; i < 6; i++)
    {
        res = utils_parseBool (tVals[i], out);
        ASSERT_TRUE (res);
        ASSERT_TRUE (out);
    }
}
