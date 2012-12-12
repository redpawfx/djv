/*------------------------------------------------------------------------------
 include/djv_image/base.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_BASE_H
#define DJV_IMAGE_BASE_H

#include <djv_image/export.h>
#include <djv_base/base.h>
#include <GL/glew.h>
#if defined(DJV_WINDOWS)
#include <GL/wglew.h>
#elif defined(DJV_MACOSX)
#else
#include <GL/glxew.h>
#endif

//------------------------------------------------------------------------------
//!\namespace djv_image
//
//! Image library.
//------------------------------------------------------------------------------

namespace djv_image {
using namespace djv_base;
}

//------------------------------------------------------------------------------

#endif

