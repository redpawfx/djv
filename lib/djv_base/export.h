/*------------------------------------------------------------------------------
 include/djv_base/export.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_EXPORT_H
#define DJV_EXPORT_H

#if defined(DJV_WINDOWS)
#if defined(djv_base_EXPORTS)
#define _DJV_BASE __declspec(dllexport)
#else
#define _DJV_BASE __declspec(dllimport)
#endif
#else
#define _DJV_BASE
#endif

#endif

