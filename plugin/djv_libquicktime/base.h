/*------------------------------------------------------------------------------
 plugin/djv_libquicktime/base.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_LIBQUICKTIME_BASE_H
#define DJV_LIBQUICKTIME_BASE_H

#include <djv_image/base.h>
#include <lqt/colormodels.h>
#include <lqt/lqt.h>

namespace djv_plugin {

//------------------------------------------------------------------------------
//!\namespace djv_plugin::libquicktime
//
//! libquicktime plugin - libquicktime movie file formats.
//!
//! Requires:
//!
//! - libquicktime - http://libquicktime.sourceforge.net
//!
//! Support:
//!
//! - Image data: RGBA, 8-bit
//! - File compression
//------------------------------------------------------------------------------

namespace libquicktime {
using namespace djv_base;
using namespace djv_image;

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "libquicktime";

static const List<String> extension_list = List<String>() <<
  ".qt" <<
  ".mov" <<
  ".avi" <<
//  ".mpg" <<
//  ".mpeg" <<
  ".mp4";
//  ".wmv" <<
//  ".divx" <<
//  ".flv"

//@}
//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

void quicktime_init();

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

List<String> label_codec();
List<String> label_codec_text();

extern const String
  error_video,
  error_codec,
  error_colormodel;

//@}
//------------------------------------------------------------------------------

}
}

#endif

