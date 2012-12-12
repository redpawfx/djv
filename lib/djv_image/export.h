/*------------------------------------------------------------------------------
 include/djv_image/export.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_EXPORT_H
#define DJV_IMAGE_EXPORT_H

#if defined(DJV_WINDOWS)
#if defined(djv_image_EXPORTS)
#define _DJV_IMAGE __declspec(dllexport)
#else
#define _DJV_IMAGE __declspec(dllimport)
#endif
#else
#define _DJV_IMAGE
#endif

#endif

