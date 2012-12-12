/*------------------------------------------------------------------------------
 plugin/djv_quicktime/base.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_QUICKTIME_BASE_H
#define DJV_QUICKTIME_BASE_H

#include <djv_image/base.h>
namespace qt {
#if defined(DJV_MACOSX)
#include <QuickTime/QuickTime.h>
#elif defined(DJV_WINDOWS)
#include <Movies.h>
#endif
#define qt_fixed1 ((qt::Fixed)0x00010000L) // XXX
}

namespace djv_plugin {

//------------------------------------------------------------------------------
//!\namespace djv_plugin::quicktime
//
//! QuickTime plugin - Apple QuickTime movie file format.
//!
//! Requires:
//!
//! - QuickTime - http://www.apple.com/quicktime
//!
//! Support:
//!
//! - Image data: RGBA, 8-bit
//! - File compression
//------------------------------------------------------------------------------

namespace quicktime {
using namespace djv_base;
using namespace djv_image;

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "QuickTime";

static const List<String> extension_list = List<String>() <<
  ".qt" <<
  ".mov" <<
  ".avi" <<
  ".mp4";

//@}
//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Video codec.

enum CODEC
{
  CODEC_RAW,
  CODEC_JPEG,
  CODEC_MJPEGA,
  CODEC_MJPEGB,
  CODEC_H263,
  CODEC_H264,
  CODEC_DVCNTSC,
  CODEC_DVCPAL,
  
  _CODEC_SIZE
};

//! Compression quality.

enum QUALITY
{
  QUALITY_LOSSLESS,
  QUALITY_MIN,
  QUALITY_MAX,
  QUALITY_LOW,
  QUALITY_NORMAL,
  QUALITY_HIGH,
  
  _QUALITY_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\name Utility
//------------------------------------------------------------------------------
//@{

//! QuickTime generic handle.

class Handle
{
public:

  Handle(qt::Handle = 0);
  
  ~Handle();

  void init(long) throw (Error);

  qt::Handle p;
};

//! QuickTime string handle.

class CFStringRef
{
public:
  
  CFStringRef(const String &);
  
  ~CFStringRef();

  qt::CFStringRef p;
};

//@}
//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

void quicktime_init() throw (Error);
void quicktime_del();

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

qt::CodecType quicktime_codec(CODEC);
qt::CodecQ quicktime_quality(QUALITY);

String & operator >> (String &, CODEC &) throw (String);
String & operator >> (String &, QUALITY &) throw (String);

String & operator << (String &, CODEC);
String & operator << (String &, QUALITY);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_codec();
const List<String> & label_quality();

extern const String
  error_init,
  error_version,
  error_open,
  error_gworld,
  error_data_ref,
  error_track,
  error_media,
  error_media_begin,
  error_media_end,
  error_media_add,
  error_media_sample,
  error_movie_add,
  error_compress;

//@}
//------------------------------------------------------------------------------

}
}

#endif

