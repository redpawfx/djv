/*------------------------------------------------------------------------------
 include/djv_base/type.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_TYPE_H
#define DJV_TYPE_H

#include "config.h"
#include <stddef.h>
#if ! defined(DJV_WINDOWS)
#include <inttypes.h>
#endif
#include <float.h>
#include <memory>
#if defined(DJV_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------

typedef unsigned int uint;

#if defined(DJV_WINDOWS)

typedef INT8 int8_t;
typedef INT16 int16_t;
typedef INT32 int32_t;
typedef INT64 int64_t;

typedef UINT8 uint8_t;
typedef UINT16 uint16_t;
typedef UINT32 uint32_t;
typedef UINT64 uint64_t;

typedef int uid_t;
typedef int gid_t;

#endif

//------------------------------------------------------------------------------

#endif

