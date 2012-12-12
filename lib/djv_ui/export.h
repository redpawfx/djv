/*------------------------------------------------------------------------------
 lib/djv_ui/export.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_EXPORT_H
#define DJV_UI_EXPORT_H

#if defined(DJV_WINDOWS)
#if defined(djv_ui_EXPORTS)
#define _DJV_UI __declspec(dllexport)
#else
#define _DJV_UI __declspec(dllimport)
#endif
#else
#define _DJV_UI
#endif

#endif

