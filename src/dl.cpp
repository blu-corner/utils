/*
 * Copyright 2014-2018 Neueda Ltd.
 */

#include "dl.h"

dl_handle
dl_open (const char* path)
{
#if defined(linux) || defined(__APPLE__)
    return dlopen (path, RTLD_LAZY);
#else
    return LoadLibrary (path);
#endif
}

dl_address
dl_symbol (dl_handle handle, const char* name)
{
#if defined(linux) || defined(__APPLE__)
    return dlsym (handle, name);
#else
    return GetProcAddress (handle, name);
#endif
}

const char*
dl_error (void)
{
#if defined(linux) || defined(__APPLE__)
    return dlerror ();
#else
    return (GetLastError() == ERROR_SUCCESS ? NULL : "error");
#endif
}
