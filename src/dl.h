/*
 * Copyright 2014-2018 Neueda Ltd.
 */

#ifndef DL_H
#define DL_H

#include "common.h"

#if defined(linux) || defined(__APPLE__)
typedef void* dl_handle;
typedef void* dl_address;
#else
typedef HMODULE dl_handle;
typedef FARPROC dl_address;
#endif

/* open the shared object at the given path */
dl_handle dl_open (const char* path);

/* retrieve the given symbol from handle */
dl_address dl_symbol (dl_handle handle, const char* name);

/* retrieve the last error */
const char* dl_error ();

#endif
