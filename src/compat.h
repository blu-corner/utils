/*
 * Copyright 2014-2018 Neueda Ltd.
 */

#ifndef COMPAT_H
#define COMPAT_H

#include <sbfCommon.h> // brings in a lot of compat stuff

#ifdef WIN32

#pragma warning(disable: 4100)

typedef unsigned int u_int;

#endif

#endif // COMPAT_H
