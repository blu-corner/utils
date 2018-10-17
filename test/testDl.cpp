#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "dl.h"

#ifndef WIN32
#define LIB_NAME "libtestinput" SBF_SHLIB_SUFFIX
#else
#define LIB_NAME "testinput" SBF_SHLIB_SUFFIX
#endif

typedef int dl_add_int (int, int);
typedef int dl_as_string (char *, int);

TEST (DL, INVALID_LIBRARY_NAME)
{
    dl_handle handle = dl_open (":invalid:shared:object:");
    ASSERT_EQ (NULL, handle);
    ASSERT_TRUE (dl_error ());
}

TEST (DL, LOAD_SHARED)
{
    dl_handle handle = dl_open (LIB_NAME);
    ASSERT_TRUE (handle);
    ASSERT_FALSE (dl_error ());
}

TEST (DL, LOAD_ADD_INTS_FUNCTION)
{
    dl_handle handle = dl_open (LIB_NAME);
    ASSERT_TRUE (handle);
    dl_add_int *f = (dl_add_int *) dl_symbol (handle, "add_ints");
    ASSERT_TRUE (f);
    ASSERT_EQ (f (1, 2), 3);
}

TEST (DL, LOAD_AS_STRING_FUNCTION)
{
    dl_handle handle = dl_open (LIB_NAME);
    ASSERT_TRUE (handle);
    dl_as_string *f = (dl_as_string *) dl_symbol (handle, "as_string");
    ASSERT_TRUE (f);
    char buffer[8];
    f (buffer, 128);
    ASSERT_STREQ (buffer, "128");
}

TEST (DL, LOAD_INVALID_FUNCTION)
{
    dl_handle handle = dl_open (LIB_NAME);
    ASSERT_TRUE (handle);
    void *f = dl_symbol (handle, ":invalid:function:");
    ASSERT_FALSE (f);
}

